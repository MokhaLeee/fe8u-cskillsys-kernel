from __future__ import annotations

import json
import logging
import os
from pathlib import Path
import re
import shutil
from typing import Any, Dict, List

from app.data.category import Categories, CategorizedCatalog
from app.data.database import (ai, constants, difficulty_modes, equations,
                               factions, items, klass, levels, lore, mcost,
                               minimap, overworld, parties,
                               raw_data, skills, stats, supports, tags, teams,
                               terrain, translations, units, varslot, weapons)
from app.data.database.game_flags import GameFlags
from app.events import event_prefab
from app.utilities.data_order import parse_order_keys_file
from app.utilities.serialization import load_json, save_json
from app.utilities.typing import NID

CATEGORY_SUFFIX = '.category'

class Database(object):
    save_data_types = ("constants", "stats", "equations", "mcost", "terrain", "weapon_ranks",
                       "weapons", "teams", "factions", "items", "skills", "tags", "game_var_slots",
                       "classes", "support_constants", "support_ranks", "affinities", "units",
                       "support_pairs", "ai", "parties", "difficulty_modes",
                       "translations", "lore", "levels", "events", "overworlds", "raw_data", 'game_flags')
    save_as_chunks = ("events", 'items', 'skills', 'units', 'classes', 'levels')

    def __init__(self):
        self.current_proj_dir = None

        self.constants = constants.constants
        self.teams = teams.TeamCatalog()
        self.stats = stats.StatCatalog()
        self.equations = equations.EquationCatalog()
        self.mcost = mcost.McostGrid()
        self.terrain = terrain.TerrainCatalog()
        self.minimap = minimap.MinimapCatalog()
        self.weapon_ranks = weapons.RankCatalog()
        self.weapons = weapons.WeaponCatalog()
        self.factions = factions.FactionCatalog()
        self.items = items.ItemCatalog()
        self.skills = skills.SkillCatalog()
        self.tags = tags.TagCatalog(['Lord', 'Boss', 'Armor', 'Horse', 'Mounted', 'Dragon', 'ZeroMove', 'AutoPromote', 'NoAutoPromote'])
        self.game_var_slots = varslot.VarSlotCatalog([])
        self.classes = klass.ClassCatalog()

        self.support_constants = supports.constants
        self.support_ranks = supports.SupportRankCatalog(['C', 'B', 'A'])
        self.affinities = supports.AffinityCatalog()

        self.units = units.UnitCatalog()

        self.support_pairs = supports.SupportPairCatalog()

        self.parties = parties.PartyCatalog()
        self.ai = ai.AICatalog()
        self.difficulty_modes = difficulty_modes.DifficultyModeCatalog()

        self.overworlds = overworld.OverworldCatalog()

        self.levels = levels.LevelCatalog()
        self.events = event_prefab.EventCatalog()

        self.translations = translations.TranslationCatalog()
        self.lore = lore.LoreCatalog()

        self.raw_data = raw_data.RawDataCatalog()

        self.game_flags = GameFlags()

    @property
    def music_keys(self) -> List[str]:
        keys = []
        for team in self.teams:
            keys.append("%s_phase" % team.nid)
        for team in self.teams:
            keys.append("%s_battle" % team.nid)
        keys.append("boss_battle")
        return keys

    # Disk Interaction Functions
    def load_categories(self, data_dir: str, key: str) -> Dict[NID, List[str]]:
        full_data_dir = os.path.join(data_dir, key)
        single_data_file_loc = os.path.join(data_dir, '.%s_categories' % key)
        categories = Categories()
        if os.path.exists(full_data_dir):
            category_path = os.path.join(full_data_dir, '.categories')
            try:
                if os.path.exists(category_path):
                    with open(category_path) as load_file:
                        categories = Categories.load(json.load(load_file))
            except:
                logging.error("category file %s not found or corrupted" % category_path)
        elif os.path.exists(single_data_file_loc):
            with open(single_data_file_loc) as load_file:
                categories = Categories.load(json.load(load_file))
        return categories

    def json_load(self, data_dir: str, key: str) -> Dict | List:
        data_path = Path(data_dir, key)
        if data_path.exists(): # data type is a directory, browse within
            data_fnames = os.listdir(data_path)
            ordering = []
            if '.orderkeys' in data_fnames:
                ordering = parse_order_keys_file(Path(data_dir, key, '.orderkeys'))
            data_fnames: List[Path] = [Path(data_dir, key, fname) for fname in data_fnames if fname.endswith('.json')]
            data_fnames = sorted(data_fnames, key=lambda fname: ordering.index(fname.stem) if fname.stem in ordering else 99999)
            full_data = []
            for fname in data_fnames:
                full_data += load_json(fname)
            return full_data
        else:   # data type is a singular file
            save_loc = Path(data_dir, key + '.json')
            if not save_loc.exists():
                logging.warning("%s does not exist!", save_loc)
                return None
            return load_json(save_loc)

    # === Saving and loading important data functions ===
    def restore(self, save_obj):
        for data_type in self.save_data_types:
            logging.info("Database: Restoring %s..." % (data_type))
            data = getattr(self, data_type)
            data.restore(save_obj[data_type])
            # Also restore the categories if it has any
            if isinstance(data, CategorizedCatalog):
                data.categories = Categories.load(save_obj.get(data_type + CATEGORY_SUFFIX, {}))

    def save(self):
        # import time
        to_save = {}
        for data_type in self.save_data_types:
            # logging.info("Saving %s..." % data_type)
            # time1 = time.time_ns()/1e6
            data = getattr(self, data_type)
            to_save[data_type] = data.save()
            # also save the categories if it has any
            if isinstance(data, CategorizedCatalog):
                to_save[data_type + CATEGORY_SUFFIX] = data.categories.save()
            # time2 = time.time_ns()/1e6 - time1
            # logging.info("Time taken: %s ms" % time2)
        return to_save

    def serialize(self, proj_dir) -> bool:
        # Returns whether we were successful
        from app.editor.settings import MainSettingsController
        main_settings = MainSettingsController()

        data_dir = os.path.join(proj_dir, 'game_data')
        if not os.path.exists(data_dir):
            os.mkdir(data_dir)
        logging.info("Serializing data in %s..." % data_dir)

        import time
        start = time.perf_counter() * 1000

        to_save = self.save()
        # This section is what takes so long!
        try:
            for key, value in to_save.items():
                # divide save data into chunks based on key value
                if key in self.save_as_chunks and main_settings.get_should_save_as_chunks():
                    save_dir = os.path.join(data_dir, key)
                    if os.path.exists(save_dir):
                        shutil.rmtree(save_dir)
                    os.mkdir(save_dir)
                    orderkeys: List[str] = []
                    for idx, subvalue in enumerate(value):
                        # ordering
                        if 'nid' in subvalue:
                            name = subvalue['nid']
                        elif 'name' in subvalue:
                            if 'level_nid' in subvalue.keys(): # to handle the wonky event nid property
                                name = (subvalue['level_nid'] if subvalue['level_nid'] else 'global') + "_" + subvalue['name']
                            else:
                                name = subvalue['name']
                        else:
                            name = str(idx).zfill(6)
                        name = re.sub(r'[\\/*?:"<>|]', "", name)
                        name = name.replace(' ', '_')
                        orderkeys.append(name)
                        save_loc = Path(save_dir, name + '.json')
                        # logging.info("Serializing %s to %s" % ('%s/%s.json' % (key, name), save_loc))
                        save_json(save_loc, [subvalue])
                    save_json(Path(save_dir, '.orderkeys'), orderkeys)
                else:  # Save as a single file
                    # Which means deleting the old directory
                    save_dir = Path(data_dir, key)
                    if os.path.exists(save_dir):
                        shutil.rmtree(save_dir)
                    save_loc = Path(data_dir, key + '.json')
                    # logging.info("Serializing %s to %s" % (key, save_loc))
                    save_json(save_loc, value)

        except OSError as e:  # In case we ran out of memory
            logging.error("Editor was unable to save your project. Free up memory in your hard drive or try saving somewhere else, otherwise progress will be lost when the editor is closed.")
            logging.exception(e)
            return False

        end = time.perf_counter() * 1000
        logging.info("Total Time Taken for Database: %s ms" % (end - start))
        logging.info("Done serializing!")
        return True

    def load(self, proj_dir):
        self.current_proj_dir = proj_dir
        data_dir = Path(proj_dir, 'game_data')
        logging.info("Deserializing data from %s..." % data_dir)

        import time
        start = time.perf_counter() * 1000

        save_obj = {}
        for key in self.save_data_types:
            save_obj[key] = self.json_load(data_dir, key)
            # Load any of the categories we need
            if Path(data_dir, key + CATEGORY_SUFFIX + '.json').exists():
                save_obj[key + CATEGORY_SUFFIX] = self.json_load(data_dir, key + CATEGORY_SUFFIX)

        self.restore(save_obj)

        # load categories
        # @TODO(rainlash) Remove this old method of restoring/loading categories at 2024/1/1
        for key in self.save_data_types:
            if key + CATEGORY_SUFFIX not in save_obj:  # Because we already restored it
                key_categories = self.load_categories(data_dir, key)
                catalog = getattr(self, key)
                if hasattr(catalog, 'categories'):
                    getattr(self, key).categories = key_categories

        # TODO -- This is a shitty fix that should be superseded
        from app.engine import equations
        equations.clear()
        from app.engine import achievements, persistent_records
        achievements.reset()
        persistent_records.reset()
        # -- End shitty fix

        end = time.perf_counter() * 1000

        logging.info("Total Time Taken for Database: %s ms" % (end - start))
        logging.info("Done deserializing!")

DB = Database()

# Testing
# Run "python -m app.data.database.database" from main directory
