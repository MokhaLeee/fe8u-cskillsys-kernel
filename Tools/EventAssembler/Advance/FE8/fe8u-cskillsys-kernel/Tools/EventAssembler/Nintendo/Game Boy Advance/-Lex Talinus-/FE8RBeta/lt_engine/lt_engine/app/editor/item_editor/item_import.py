import os
import xml.etree.ElementTree as ET
from app.editor.lib.csv.csv_data_exporter import update_db_with_item_csv

from app.utilities import str_utils, utils
from app.utilities.data import Data
from app.data.database.database import DB, Database
from app.data.database import items

from app.data.database.components import ComponentType

import app.engine.item_component_access as ICA

import logging


def update_db_from_csv(db: Database, csv_fn: str):
    with open(csv_fn) as f:
        csv_text = f.read()
        update_db_with_item_csv(db, csv_text)

def get_from_xml(parent_dir: str, xml_fn: str) -> list:
    item_xml = ET.parse(xml_fn)
    item_list = []
    for item in item_xml.getroot().findall('item'):
        try:
            new_item = load_item(item, parent_dir)
            item_list.append(new_item)
        except Exception as e:
            logging.error("Item %s Import Error: %s" % (item.find('id').text, e))
    return item_list

def get_wtype(final_components, item):
    wtype = item.find('weapontype').text
    if wtype in DB.weapons.keys():
        weapontype_comp = ICA.get_component('weapon_type')
        weapontype_comp.value = wtype
        final_components.append(weapontype_comp)
    wrank = item.find('LVL').text
    if wrank in DB.weapon_ranks.keys():
        weaponrank_comp = ICA.get_component('weapon_rank')
        weaponrank_comp.value = wrank
        final_components.append(weaponrank_comp)
    return wtype, wrank

def check_for_magic(final_components, wtype, parent_dir):
    if os.path.exists(parent_dir + '/weapon_triangle.txt'):
        with open(parent_dir + '/weapon_triangle.txt') as wfn:
            lines = [line.strip() for line in wfn.readlines() if line.strip()]
            weapon_order = [line.split(';')[0] for line in lines]
            is_magic = [True if len(line.split(';')) > 3 and line.split(';')[3] else False for line in lines]
            magic = False
            if wtype in weapon_order:
                magic = is_magic[weapon_order.index(wtype)]
            if magic:
                magic_comp = ICA.get_component('magic')
                final_components.append(magic_comp)

def load_item(item, parent_dir):
    nids = DB.items.keys()
    nid = str_utils.get_next_name(item.find('id').text, nids)
    name = item.get('name')
    desc = item.find('desc').text
    icon_nid = item.find('spritetype').text
    icon_index = item.find('spriteid').text.split(',')
    if len(icon_index) == 1:
        icon_index = (0, int(icon_index[0]))
    else:
        icon_index = (int(icon_index[0]), int(icon_index[1]))

    components = item.find('components').text.split(',') if item.find('components').text else []
    final_components = Data()
    for component in components:
        if component == 'weapon':
            weapon_comp = ICA.get_component('weapon')
            final_components.append(weapon_comp)
            target_comp = ICA.get_component('target_enemy')
            final_components.append(target_comp)
            exp_comp = ICA.get_component('level_exp')
            final_components.append(exp_comp)
            damage_comp = ICA.get_component('damage')
            damage_comp.value = int(item.find('MT').text)
            final_components.append(damage_comp)
            hit_comp = ICA.get_component('hit')
            hit_comp.value = int(item.find('HIT').text)
            final_components.append(hit_comp)
            wtype, wrank = get_wtype(final_components, item)
            check_for_magic(final_components, wtype, parent_dir)

        elif component == 'spell':
            spell_comp = ICA.get_component('spell')
            final_components.append(spell_comp)

            if item.find('targets') is not None:
                target = item.find('targets').text
                if target == 'Enemy':
                    target_comp = ICA.get_component('target_enemy')
                    final_components.append(target_comp)
                    exp_comp = ICA.get_component('level_exp')
                    final_components.append(exp_comp)
                elif target == 'Ally':
                    target_comp = ICA.get_component('target_ally')
                    final_components.append(target_comp)
                elif target == 'Unit':
                    target_comp = ICA.get_component('target_unit')
                    final_components.append(target_comp)

            wtype, wrank = get_wtype(final_components, item)
            check_for_magic(final_components, wtype, parent_dir)

        elif component == 'usable':
            usable = ICA.get_component('usable')
            final_components.append(usable)
            target_comp = ICA.get_component('target_ally')
            final_components.append(target_comp)

        elif component == 'status':
            comp = ICA.get_component('status_on_hit')
            comp.value = item.find('status').text
            final_components.append(comp)

        elif component == 'reverse':
            comp = ICA.get_component('reaver')
            final_components.append(comp)

        elif component == 'permanent_stat_increase':
            comp = ICA.get_component('permanent_stat_change')
            logging.warning("%s: Could not determine value for component %s" % (nid, 'permanent_stat_change'))
            comp.value = []
            final_components.append(comp)

        elif component == 'booster':
            comp = ICA.get_component('usable_in_base')
            final_components.append(comp)

        elif component == 'sfx_on_hit':
            comp = ICA.get_component('map_hit_sfx')
            comp.value = item.find('sfx_on_hit').text
            final_components.append(comp)

        elif component == 'sfx_on_cast':
            comp = ICA.get_component('map_cast_sfx')
            comp.value = item.find('sfx_on_cast').text
            final_components.append(comp)

        elif component == 'aoe_anim':
            comp = ICA.get_component('map_cast_anim')
            final_components.append(comp)

        elif component == 'aoe':
            target = item.find('aoe').text
            if len(target.split(',')) == 2:
                mode, magnitude = target.split(',')
            else:
                mode, magnitude = target, 0
            if str_utils.is_int(magnitude):
                pass
            else:
                logging.warning("%s: Could not determine value for component %s" % (nid, component))
                continue
            if mode == 'EnemyBlast':
                comp = ICA.get_component('enemy_blast_aoe')
                comp.value = int(magnitude)
            elif mode == 'Blast':
                comp = ICA.get_component('blast_aoe')
                comp.value = int(magnitude)
            elif mode == 'Cleave':
                comp = ICA.get_component('enemy_cleave_aoe')
            elif mode == 'AllAllies':
                comp = ICA.get_component('all_allies_aoe')
            elif mode == 'AllEnemies':
                comp = ICA.get_component('all_enemies_aoe')
            elif mode == 'Line':
                comp = ICA.get_component('line_aoe')
            else:
                logging.warning("%s: Could not determine value for component %s" % (nid, component))
                continue
            final_components.append(comp)

        elif component == 'movement':
            target = item.find('movement').text
            mode, magnitude = target.split(',')
            if mode == 'Shove':
                comp = ICA.get_component('shove')
                comp.value = int(magnitude)
            elif mode == 'Swap':
                comp = ICA.get_component('swap')
            elif mode == 'Warp':
                comp = ICA.get_component('warp')
            else:
                logging.warning("%s: Could not determine value for component %s" % (nid, component))
                continue
            final_components.append(comp)

        elif component in ('beneficial', 'detrimental'):
            # This is not necessary with BEEG update
            # All of beneficials varied functions
            # Mostly AI and targeting now need to be done
            # in their respective places
            pass

        elif item.find(component) is not None:
            comp = ICA.get_component(component)
            if comp:
                try:
                    value = item.find(component).text
                    if comp.expose == ComponentType.Int:
                        value = int(value)
                    elif comp.expose == ComponentType.Float:
                        value = float(value)
                    elif comp.expose == ComponentType.Color3 or comp.expose == ComponentType.Color4:
                        value = [utils.clamp(int(c), 0, 255) for c in value.split(',')]
                    elif isinstance(comp.expose, tuple):
                        logging.warning("%s: Could not determine value for component %s" % (nid, component))
                        value = []
                    comp.value = value
                except Exception as e:
                    logging.warning("%s: Could not determine value for component %s" % (nid, component))
                final_components.append(comp)
            else:
                logging.warning("%s: Could not determine corresponding LT maker component for %s" % (nid, component))
        else:
            comp = ICA.get_component(component)
            if comp:
                final_components.append(comp)
            else:
                logging.warning("%s: Could not determine corresponding LT maker component for %s" % (nid, component))

    # Handle item value
    value = int(item.find('value').text)
    if value > 0:
        comp = ICA.get_component('value')
        if item.find('uses') is not None:
            value *= int(item.find('uses').text)
        comp.value = value
        final_components.append(comp)

    # Handle item range
    rng = item.find('RNG').text.split('-')
    if len(rng) == 1 and int(rng[0]) > 0:
        comp = ICA.get_component('min_range')
        comp.value = int(rng[0])
        final_components.append(comp)
        comp = ICA.get_component('max_range')
        comp.value = int(rng[0])
        final_components.append(comp)
    elif len(rng) == 2:
        comp = ICA.get_component('min_range')
        comp.value = int(rng[0])
        final_components.append(comp)
        if rng[1] == 'MAG/2':
            comp = ICA.get_component('equation_max_range')
            comp.value = 'MAG_RANGE'
        else:
            comp = ICA.get_component('max_range')
            comp.value = int(rng[1])
        final_components.append(comp)

    new_item = items.ItemPrefab(nid, name, desc, icon_nid, icon_index, final_components)
    return new_item
