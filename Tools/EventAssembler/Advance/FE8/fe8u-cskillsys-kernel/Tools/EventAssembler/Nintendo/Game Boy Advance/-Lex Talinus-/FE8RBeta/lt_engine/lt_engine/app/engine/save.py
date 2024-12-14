import os, shutil, glob, re
from datetime import datetime
from typing import Optional
import threading

try:
    import cPickle as pickle
except ImportError:
    import pickle

from app.utilities import str_utils
from app.data.database.database import DB

import app.engine.config as cf
from app.engine.objects.item import ItemObject
from app.engine.objects.skill import SkillObject

import logging

SAVE_THREAD = None

def GAME_NID():
    return str(DB.constants.value('game_nid'))

SUSPEND_LOC = 'saves/' + GAME_NID() + '-suspend.pmeta'

class SaveSlot():
    no_name = '--NO DATA--'

    def __init__(self, metadata_fn, idx):
        self.name = self.no_name
        self.playtime = 0
        self.realtime = 0
        self.kind = None  # Prep, Base, Suspend, Battle, Start
        self.mode = None
        self.idx = idx

        self.meta_loc = metadata_fn
        self.save_loc = metadata_fn[:-4]

        self.read()

    def read(self):
        if os.path.exists(self.meta_loc):
            with open(self.meta_loc, 'rb') as fp:
                save_metadata = pickle.load(fp)
            self.name = save_metadata['level_title']
            self.playtime = save_metadata['playtime']
            self.realtime = save_metadata['realtime']
            self.kind = save_metadata['kind']
            self.mode = save_metadata.get('mode')

    def get_name(self):
        if self.kind == 'turn_change':
            turn = int(re.findall(r'\d+', self.meta_loc)[-1])
            return self.name + (' - Turn %d' % turn)
        elif self.kind and cf.SETTINGS['debug']:
            return self.name + ' - ' + self.kind
        else:
            return self.name

    def __repr__(self):
        return '%d: %s' % (self.idx, self.get_name())

def dict_print(d):
    for k, v in d.items():
        if isinstance(v, dict):
            dict_print(v)
        else:
            s = "{0} : {1}".format(k, v)
            print(s)
            logging.error(s)

def save_io(s_dict, meta_dict, old_slot, slot, force_loc=None, name=None):
    if name:
        save_loc = 'saves/' + name + '.p'
    elif force_loc:
        save_loc = 'saves/' + GAME_NID() + '-' + force_loc + '.p'
    elif slot is not None:
        save_loc = 'saves/' + GAME_NID() + '-' + str(slot) + '.p'
    meta_loc = save_loc + 'meta'

    logging.info("Saving to %s", save_loc)

    with open(save_loc, 'wb') as fp:
        # pickle.dump(s_dict, fp, -1)
        try:
            pickle.dump(s_dict, fp)
        except TypeError as e:
            # There's a surface somewhere in the dictionary of things to save...
            dict_print(s_dict)
            print(e)
    with open(meta_loc, 'wb') as fp:
        pickle.dump(meta_dict, fp)

    # For restart
    if not force_loc:
        r_save = 'saves/' + GAME_NID() + '-restart' + str(slot) + '.p'
        r_save_meta = r_save + 'meta'
        # If the slot I'm overwriting is a start of map
        # Then rename it to restart file
        if meta_dict['kind'] == 'start':
            if save_loc != r_save:
                shutil.copy(save_loc, r_save)
                shutil.copy(meta_loc, r_save_meta)
        elif old_slot is not None:
            old_name = 'saves/' + GAME_NID() + '-restart' + str(old_slot) + '.p'
            old_name_meta = old_name + 'meta'
            if old_name != r_save and os.path.exists(old_name):
                shutil.copy(old_name, r_save)
                shutil.copy(old_name_meta, r_save_meta)

    # For preload
    if meta_dict['kind'] == 'start':
        preload_saves = glob.glob('saves/' + GAME_NID() + '-preload-' + str(meta_dict['level_nid']) + '-*.p')
        nids = [p.split('-')[-1][:-2] for p in preload_saves]
        unique_nid = str(str_utils.get_next_int('0', nids))
        preload_save = 'saves/' + GAME_NID() + '-preload-' + str(meta_dict['level_nid']) + '-' + unique_nid + '.p'
        preload_save_meta = 'saves/' + GAME_NID() + '-preload-' + str(meta_dict['level_nid']) + '-' + unique_nid + '.pmeta'

        shutil.copy(save_loc, preload_save)
        shutil.copy(meta_loc, preload_save_meta)

def suspend_game(game_state, kind, slot: int = None, name=None):
    """
    Saves game state to file
    """
    logging.debug("Suspending game...")
    s_dict, meta_dict = game_state.save()
    logging.debug("Suspend state: %s", game_state.state.state_names())
    logging.debug("Suspend temp state: %s", game_state.state.temp_state)
    meta_dict['kind'] = kind
    meta_dict['time'] = datetime.now()
    old_save_slot = game_state.current_save_slot
    if slot is not None:
        game_state.current_save_slot = slot  # Where we are saving it to, so we can get it back later

    if kind == 'suspend':
        force_loc = 'suspend'
    else:
        force_loc = None

    global SAVE_THREAD
    SAVE_THREAD = threading.Thread(target=save_io, args=(s_dict, meta_dict, old_save_slot, slot, force_loc, name))
    SAVE_THREAD.start()

def load_game(game_state, save_slot: SaveSlot):
    """
    Load game state from file
    """
    save_loc = save_slot.save_loc
    logging.info("Loading from %s", save_loc)
    with open(save_loc, 'rb') as fp:
        s_dict = pickle.load(fp)
    game_state.build_new()
    game_state.load(s_dict)
    game_state.current_save_slot = save_slot.idx

    set_next_uids(game_state)

def set_next_uids(game_state):
    if game_state.item_registry:
        ItemObject.next_uid = max(game_state.item_registry.keys()) + 1
    else:
        ItemObject.next_uid = 100
    if game_state.skill_registry:
        SkillObject.next_uid = max(game_state.skill_registry.keys()) + 1
    else:
        SkillObject.next_uid = 100
    logging.info("Setting next item uid: %d" % ItemObject.next_uid)
    logging.info("Setting next skill uid: %d" % SkillObject.next_uid)

def load_saves():
    save_slots = []
    for num in range(0, int(DB.constants.value('num_save_slots'))):
        meta_fp = 'saves/' + GAME_NID() + '-' + str(num) + '.pmeta'
        ss = SaveSlot(meta_fp, num)
        save_slots.append(ss)
    return save_slots

def load_restarts():
    save_slots = []
    for num in range(0, int(DB.constants.value('num_save_slots'))):
        meta_fp = 'saves/' + GAME_NID() + '-restart' + str(num) + '.pmeta'
        ss = SaveSlot(meta_fp, num)
        save_slots.append(ss)
    return save_slots

def get_all_saves():
    """
    Grabs all the turn_change saves
    """
    save_slots = []
    name = 'saves/' + GAME_NID() + '-turn_change-*-*.pmeta'
    for meta_fn in glob.glob(name):
        ss = SaveSlot(meta_fn, 0)
        save_slots.append(ss)
    save_slots = sorted(save_slots, key=lambda x: x.realtime, reverse=True)
    return save_slots

def remove_suspend():
    if not cf.SETTINGS['debug'] and os.path.exists(SUSPEND_LOC):
        os.remove(SUSPEND_LOC)

def delete_suspend():
    if os.path.exists(SUSPEND_LOC):
        os.remove(SUSPEND_LOC)

def delete_save(game_state, num: Optional[int] = None):
    """
    If num is not provided, deletes current save
    """ 
    if game_state.current_save_slot is not None:
        num = game_state.current_save_slot
    if num is None:
        logging.error("delete_save: num not provided and no current save slot")
        return
    meta_fn = 'saves/' + GAME_NID() + '-' + str(num) + '.pmeta'
    save_fn = 'saves/' + GAME_NID() + '-' + str(num) + '.p'
    r_save_fn = 'saves/' + GAME_NID() + '-restart' + str(num) + '.p'
    if os.path.exists(meta_fn):
        os.remove(meta_fn)
    if os.path.exists(save_fn):
        os.remove(save_fn)
    if os.path.exists(r_save_fn):
        os.remove(r_save_fn)

def get_save_title(save_slots):
    options = [save_slot.get_name() for save_slot in save_slots]
    colors = [DB.difficulty_modes.get(save_slot.mode).color if (save_slot.mode and DB.difficulty_modes.get(save_slot.mode)) else 'green' for save_slot in save_slots]
    return options, colors

def check_save_slots():
    global SAVE_SLOTS, RESTART_SLOTS
    SAVE_SLOTS = load_saves()
    RESTART_SLOTS = load_restarts()

SAVE_SLOTS = load_saves()
RESTART_SLOTS = load_restarts()
