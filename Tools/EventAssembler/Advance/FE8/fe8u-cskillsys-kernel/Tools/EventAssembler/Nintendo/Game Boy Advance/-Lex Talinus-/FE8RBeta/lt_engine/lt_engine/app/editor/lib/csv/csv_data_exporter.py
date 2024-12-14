from app.data.database.database_validator import DatabaseValidatorEngine
import logging
from typing import List
from app.data.database.components import ComponentType, convert_type_from_string
from app.data.database.database import Database
from app.data.resources.resources import Resources
from app.data.database.items import ItemCatalog, ItemPrefab
from app.data.database.klass import ClassCatalog
from app.data.database.units import UnitCatalog, UnitPrefab
from app.data.database.weapons import WexpGain
import app.engine.item_components.weapon_components as weapon_components
import app.engine.item_components.base_components as base_components
import app.engine.item_components.usable_components as usable_components
import app.engine.item_components.target_components as target_components
import app.engine.item_component_access as ICA

def csv_str_as_grid(csv_str: str) -> List[List[str]]:
    csv_str = csv_str.strip()
    return [line.split(',') for line in csv_str.split('\n')]

def item_db_to_csv(item_db: ItemCatalog) -> str:
    """Generates a csv of all relevant weapons.

    Because items is a massive, massive umbrella, this function will define 'relevant'
    as being any item which has hit, might, or weapon type. Of these weapons, this will list the typical FE
    properties:
    - Hit
    - Crit
    - Might
    - Rank and Type
    - Weight
    - Uses
    - Value
    - Range

    Returns:
        str: csv-ified string.
    """
    if not item_db._list:
        return ""
    def get_or_null(i, at):
        if i[at]:
            return i[at].value
        else:
            return '--'
    # header
    headers = ['name', 'nid', weapon_components.Hit.nid, weapon_components.Crit.nid, weapon_components.Damage.nid,
               weapon_components.WeaponType.nid, weapon_components.WeaponRank.nid, weapon_components.Weight.nid,
               usable_components.Uses.nid, target_components.MinimumRange.nid, target_components.MaximumRange.nid,
               base_components.Value.nid]
    ss = ','.join([header.upper() for header in headers]) + "\n"
    for item in item_db:
        if item.hit or item.might or item.weapon_type:
            data = [item.name, item.nid]
            for attribute in headers[2:]:
                data.append(get_or_null(item, attribute))
            ss += ','.join([str(dat) for dat in data]) + "\n"
    return ss

def unit_db_to_csv(unit_db: UnitCatalog):
    if not unit_db._list:
        return ""
    first_elem = unit_db._list[0]
    bases_d = first_elem.get_stat_lists()[0]
    weapons_l = list(first_elem.wexp_gain.keys())
    stats_l = list(bases_d.keys())
    # header
    headers = ["NAME", "NID"]
    headers += [stat + '_base' for stat in stats_l] + [stat + '_growth' for stat in stats_l] + weapons_l
    ss = ','.join(headers) + '\n'
    for unit in unit_db:
        data = [unit.name, unit.nid]
        ubases, ugrowths, ustatcapmodifiers = unit.get_stat_lists()
        wranks = unit.wexp_gain
        data += [ubases[stat] for stat in stats_l] + [ugrowths[stat] for stat in stats_l] + [wranks[weapon].wexp_gain for weapon in weapons_l]
        ss += ','.join([str(dat) for dat in data]) + '\n'
    return ss

def klass_db_to_csv(klass_db: ClassCatalog):
    if not klass_db._list:
        return ""
    first_elem = klass_db._list[0]
    bases_d, growths_d, gains_d, growth_bonuses_d, stat_caps = first_elem.get_stat_lists()
    weapons_l = list(first_elem.wexp_gain.keys())
    stats_l = list(bases_d.keys())
    # header
    headers = ["NAME", "NID"]
    headers += [stat + '_base' for stat in stats_l] + [stat + '_growth' for stat in stats_l] + weapons_l
    ss = ','.join(headers) + '\n'
    for unit in klass_db:
        data = [unit.name, unit.nid]
        ubases, ugrowths, ugains, ubonuses, ucaps = unit.get_stat_lists()
        wranks = unit.wexp_gain
        data += [ubases.get(stat, 0) for stat in stats_l] + [ugrowths.get(stat, 0) for stat in stats_l] + [bool(wranks[weapon].usable) for weapon in weapons_l]
        ss += ','.join([str(dat) for dat in data]) + '\n'
    return ss

def dump_as_csv(db: Database, resources: Resources):
    db_validator = DatabaseValidatorEngine(db, resources)
    db_validator.repair()
    # csv dump functions
    dump = []
    try:
        dump.append(('units', unit_db_to_csv(db.units)))
    except Exception as e:
        logging.error("failed to dump units", e)
    try:
        dump.append(('classes', klass_db_to_csv(db.classes)))
    except Exception as e:
        logging.error("failed to dump class", e)
    try:
        dump.append(('items', item_db_to_csv(db.items)))
    except Exception as e:
        logging.error("failed to dump items", e)
    return dump

def update_db_with_unit_csv(db: Database, unit_csv_str):
    data = csv_str_as_grid(unit_csv_str)
    headers = data[0]
    for unit_data in data[1:]:
        if len(unit_data) != len(headers):
            logging.error("mismatch between unit data and headers: %s, %s", str(unit_data), str(headers))
            continue
        name = unit_data[0]
        nid = unit_data[1]
        if nid in db.units.keys():
            curr_unit = db.units.get(nid)
        else:
            curr_unit = UnitPrefab(nid, name)
            db.units.append(curr_unit)

        for i in range(len(unit_data)):
            if headers[i] in ('NAME', 'NID'):
                continue
            if '_base' in headers[i]:
                base_stat_name = headers[i].replace('_base', '')
                curr_unit.bases[base_stat_name] = int(unit_data[i])
            elif '_growth' in headers[i]:
                growth_stat_name = headers[i].replace('_growth', '')
                curr_unit.growths[growth_stat_name] = int(unit_data[i])
            elif headers[i] in db.weapons.keys(): # wrank
                weapon = headers[i]
                rank = int(unit_data[i])
                if rank > 0:
                    curr_unit.wexp_gain[weapon] = WexpGain(True, rank, 251)
                else:
                    curr_unit.wexp_gain[weapon] = WexpGain(False, rank, 251)


def validate_type(db: Database, expose_type: ComponentType, value: str):
    if expose_type == ComponentType.WeaponType:
        if value in db.weapons.keys():
            return True
        else:
            return False
    elif expose_type == ComponentType.WeaponRank:
        if value in db.weapon_ranks.keys():
            return True
        else:
            return False
    else:
        return True

def update_db_with_item_csv(db: Database, item_csv_str):
    data = csv_str_as_grid(item_csv_str)
    headers = data[0]
    for item_data in data[1:]:
        if len(item_data) != len(headers):
            logging.error("mismatch between item data and headers: %s, %s", str(item_data), str(headers))
            continue
        name = item_data[0]
        nid = item_data[1]
        if nid in db.items.keys():
            curr_item = db.items.get(nid)
        else:
            curr_item = ItemPrefab(nid, name, "")
            db.items.append(curr_item)

        for i in range(len(item_data)):
            if headers[i].lower() in ('name', 'nid'):
                continue
            if '--' in item_data[i] or not item_data[i]:
                continue
            # set the component value or, if it doesn't exist, add it
            component_name = headers[i].lower()
            if curr_item[component_name]:
                expose_type = curr_item[component_name].expose
                converted = convert_type_from_string(item_data[i], expose_type)
                if validate_type(db, expose_type, converted):
                    curr_item[component_name].value = converted
                else:
                    logging.error("invalid value type: %s, %s, %s", converted, str(expose_type), item_data)
                    continue
            else:
                new_component = ICA.get_component(component_name)
                if not new_component:
                    logging.error("Unknown component name %s", component_name)
                    continue
                if not new_component.expose:
                    continue
                new_component_value = convert_type_from_string(item_data[i], new_component.expose)
                if validate_type(db, new_component.expose, new_component_value):
                    new_component.value = new_component_value
                else:
                    logging.error("invalid value type: %s, %s, %s", converted, str(expose_type), item_data)
                    continue

                curr_item.components.append(new_component)
                curr_item[new_component.nid] = new_component
