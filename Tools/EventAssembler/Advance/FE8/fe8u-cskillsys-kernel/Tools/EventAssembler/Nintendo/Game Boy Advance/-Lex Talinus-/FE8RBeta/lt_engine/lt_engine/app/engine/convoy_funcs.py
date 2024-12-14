import random
from app.engine import item_system, item_funcs, equations, action
from app.engine.game_state import game

def can_restock(item) -> bool:
    convoy = game.party.convoy
    return bool(item.uses and item.data['uses'] < item.data['starting_uses'] and item.nid in [i.nid for i in convoy if i is not item])

def restock(item):
    convoy = game.party.convoy
    if not can_restock(item):
        return

    other_items = sorted([i for i in convoy if i.nid == item.nid and i is not item], key=lambda i: i.data['uses'])
    for i in other_items:
        diff_needed = item.data['starting_uses'] - item.data['uses']
        if diff_needed > 0:
            if i.data['uses'] >= diff_needed:
                i.data['uses'] -= diff_needed
                item.data['uses'] += diff_needed
                if i.data['uses'] <= 0:
                    convoy.remove(i)
            else:
                item.data['uses'] += i.data['uses']
                i.data['uses'] = 0
                convoy.remove(i)
        else:
            break

def restock_convoy():
    convoy = game.party.convoy
    items = [i for i in convoy if can_restock(i)]
    items = sorted(items, key=lambda i: i.data['uses'], reverse=True)
    for item in items[:]:
        if item.data['uses'] > 0:
            restock(item)

def optimize_all():
    all_units = game.get_units_in_party()
    for unit in all_units:
        for item in item_funcs.get_all_tradeable_items(unit):
            store_item(item, unit)
    restock_convoy()
    # Only optimize for units that are actually on the field of battle
    units = [unit for unit in game.get_units_in_party() if unit.position]
    random.shuffle(units)
    # Distribute Weapons
    weapons = [item for item in game.party.convoy if item_system.is_weapon(None, item)]
    weapons = sorted(weapons, key=lambda item: (item_system.weapon_rank(None, item) or '', 1000 - item.data.get('uses', 0)))
    for weapon in weapons:
        units_that_can_wield = [
            unit for unit in units
            if not item_funcs.inventory_full(unit, weapon) and
            item_funcs.available(unit, weapon) and
            weapon.nid not in [i.nid for i in unit.items]]
        units_that_can_wield = sorted(units_that_can_wield, key=lambda u: len(u.items), reverse=True)
        if units_that_can_wield:
            unit = units_that_can_wield.pop()
            take_item(weapon, unit)
            
    # Distribute Spells
    spells = [item for item in game.party.convoy if item_system.is_spell(None, item)]
    spells = sorted(spells, key=lambda item: (item_system.weapon_rank(None, item) or '', 1000 - item.data.get('uses', 0)))
    for spell in spells:
        units_that_can_wield = [
            unit for unit in units
            if not item_funcs.inventory_full(unit, spell) and
            item_funcs.available(unit, spell) and
            spell.nid not in [i.nid for i in unit.items]]
        units_that_can_wield = sorted(units_that_can_wield, key=lambda u: len(u.items), reverse=True)
        if units_that_can_wield:
            unit = units_that_can_wield.pop()
            take_item(spell, unit)

    # Distribute healing items
    healing_items = sorted([item for item in game.party.convoy if item.heal], key=lambda i: (i.heal.value, 1000 - i.data.get('uses', 0)))
    # Sort by max hp
    for item in reversed(healing_items):
        units_that_can_wield = [
            unit for unit in units
            if not item_funcs.inventory_full(unit, item) and
            not any(item.heal for item in unit.items)]
        units_that_can_wield = sorted(units_that_can_wield, key=lambda u: equations.parser.hitpoints(u))
        if units_that_can_wield:
            unit = units_that_can_wield.pop()
            take_item(item, unit)
    # Done!

def optimize(unit):
    for item in item_funcs.get_all_tradeable_items(unit):
        store_item(item, unit)
    # Distribute Weapons
    weapons = [
        item for item in game.party.convoy 
        if item_system.is_weapon(None, item) and 
        item_funcs.available(unit, item)]
    weapons = sorted(weapons, key=lambda item: (item_system.weapon_rank(None, item) or '', 1000 - item.data.get('uses', 0)))
    # Distribute Spells
    spells = [
        item for item in game.party.convoy 
        if item_system.is_spell(None, item) and
        item_funcs.available(unit, item)]
    spells = sorted(spells, key=lambda item: (item_system.weapon_rank(None, item) or '', 1000 - item.data.get('uses', 0)))

    # Give two spells if possible
    num_weapons = 2
    if spells:
        for _ in range(2):
            for spell in spells:
                if not item_funcs.inventory_full(unit, spell) and \
                        spell.nid not in [i.nid for i in unit.items]:
                    take_item(spell, unit)
                    break
    else:
        num_weapons = 4
    for _ in range(num_weapons):
        for weapon in weapons:
            if not item_funcs.inventory_full(unit, weapon) and \
                    weapon.nid not in [i.nid for i in unit.items]:
                take_item(weapon, unit)
                break

    # Distribute healing items
    healing_items = sorted([item for item in game.party.convoy if item.heal], key=lambda i: (i.heal.value, 1000 - i.data.get('uses', 0)))
    for item in reversed(healing_items):
        if not item_funcs.inventory_full(unit, item) and \
                item.nid not in [i.nid for i in unit.items]:
            take_item(item, unit)
            break
    # Done!

def take_item(item, unit):
    action.do(action.TakeItemFromConvoy(unit, item))

def give_item(item, owner, unit):
    action.do(action.MoveItem(owner, unit, item))

def store_item(item, unit):
    action.do(action.StoreItem(unit, item))

def trade_items(convoy_item, unit_item, unit):
    if convoy_item.owner_nid:
        new_unit = game.get_unit(convoy_item.owner_nid)
        action.do(action.TradeItem(unit, new_unit, unit_item, convoy_item))
    else:
        action.do(action.TradeItemWithConvoy(unit, convoy_item, unit_item))
