from app.engine.game_state import game
from app.utilities import utils
from app.data.database.database import DB
from app.data.database import weapons
from app.engine import equations, item_system, item_funcs, skill_system, line_of_sight
from app.engine.combat.utils import resolve_weapon

def get_weapon_rank_bonus(unit, item):
    weapon_type = item_system.weapon_type(unit, item)
    if not weapon_type:
        return None
    rank_bonus = DB.weapons.get(weapon_type).rank_bonus
    wexp = unit.wexp[weapon_type]
    best_combat_bonus = None
    highest_requirement = -1
    for combat_bonus in rank_bonus:
        if combat_bonus.weapon_rank == 'All':
            return combat_bonus
        req = DB.weapon_ranks.get(combat_bonus.weapon_rank).requirement
        if wexp >= req and req > highest_requirement:
            highest_requirement = req
            best_combat_bonus = combat_bonus
    return best_combat_bonus

def get_support_rank_bonus(unit, target=None):
    from app.engine.game_state import game

    if not unit.position:
        return [], []
    # If target, only check for when can attack same unit
    if target and DB.support_constants.value('bonus_range') != 0:
        return [], []
    pairs = game.supports.get_bonus_pairs(unit.nid)
    bonuses = []
    for pair in pairs:
        if not pair.unlocked_ranks:
            continue
        prefab = DB.support_pairs.get(pair.nid)
        if unit.nid == prefab.unit1:
            other_unit = game.get_unit(prefab.unit2)
        else:
            other_unit = game.get_unit(prefab.unit1)
        if not (other_unit and other_unit.position):
            continue
        # If unit has already been counted
        if other_unit in [_[1] for _ in bonuses]:
            continue
        if target and target.position:
            # Unit and other unit can both attack target
            if target.position in game.target_system.get_attackable_positions(other_unit, force=True):
                pass
            else:
                continue
        elif not game.supports.check_bonus_range(unit, other_unit):
            continue
        if not pair.unlocked_ranks:
            continue
        highest_rank = pair.unlocked_ranks[-1]
        support_rank_bonus = game.supports.get_bonus(unit, other_unit, highest_rank)
        bonuses.append((support_rank_bonus, other_unit))
    num_allies_allowed = DB.support_constants.value('bonus_ally_limit')
    if num_allies_allowed and len(bonuses) > num_allies_allowed:
        # Get the X highest bonuses
        bonuses = sorted(bonuses, key=lambda x: DB.support_ranks.index(x[0].support_rank), reverse=True)
        bonuses = bonuses[:num_allies_allowed]
    allies = [_[1] for _ in bonuses]
    bonuses = [_[0] for _ in bonuses]
    return bonuses, allies

def compute_advantage(unit1, unit2, item1, item2, advantage=True):
    if not item1 or not item2:
        return None
    item1_weapontype = item_system.weapon_type(unit1, item1)
    item2_weapontype = item_system.weapon_type(unit2, item2)
    if not item1_weapontype or not item2_weapontype:
        return None
    if item_system.ignore_weapon_advantage(unit1, item1) or \
            item_system.ignore_weapon_advantage(unit2, item2):
        return None

    w_mod1 = item_system.modify_weapon_triangle(unit1, item1)
    w_mod2 = item_system.modify_weapon_triangle(unit2, item2)
    final_w_mod = utils.sign(w_mod1) * utils.sign(w_mod2) * max(abs(w_mod1), abs(w_mod2))

    if advantage:
        bonus = DB.weapons.get(item1_weapontype).advantage
    else:
        bonus = DB.weapons.get(item1_weapontype).disadvantage
    # bonus is a CombatBonusList
    highest_requirement_met = -1
    new_adv = None
    for adv in bonus:  
        if adv.weapon_type == 'All' or adv.weapon_type == item2_weapontype:
            if adv.weapon_rank == 'All':
                new_adv = weapons.CombatBonus.copy(adv)
                new_adv.modify(final_w_mod)
                return new_adv
            # Figure out which Weapon Rank Bonus is highest that we meet
            requirement = DB.weapon_ranks.get(adv.weapon_rank).requirement
            if unit1.wexp[item1_weapontype] >= requirement and requirement > highest_requirement_met:
                highest_requirement_met = requirement
                new_adv = weapons.CombatBonus.copy(adv)
                new_adv.modify(final_w_mod)
    return new_adv

def can_counterattack(attacker, aweapon, defender, dweapon) -> bool:
    if not dweapon:
        return False
    if not item_funcs.available(defender, dweapon):
        return False
    if not item_system.can_be_countered(attacker, aweapon):
        return False
    if not item_system.can_counter(defender, dweapon):
        return False
    if DB.constants.value('line_of_sight'):
        if not item_system.ignore_line_of_sight(defender, dweapon) and len(line_of_sight.line_of_sight([defender.position], [attacker.position], 99)) == 0:
            return False
    
    if not attacker.position:
        return True
    valid_targets = game.target_system.targets_in_range(defender, dweapon)
    if attacker.position in valid_targets:
        return True
    if skill_system.distant_counter(defender):
        return True
    if skill_system.close_counter(defender) and utils.calculate_distance(attacker.position, defender.position) == 1:
        return True
    return False

def accuracy(unit, item=None):
    if not item:
        item = unit.get_weapon()
    if not item:
        return None

    accuracy = item_system.hit(unit, item)
    if accuracy is None:
        return None

    equation = item_system.accuracy_formula(unit, item)
    if equation == 'HIT':
        equation = skill_system.accuracy_formula(unit)
    accuracy += equations.parser.get(equation, unit)

    weapon_rank_bonus = get_weapon_rank_bonus(unit, item)
    if weapon_rank_bonus:
        accuracy += int(weapon_rank_bonus.accuracy)

    support_rank_bonuses, support_allies = get_support_rank_bonus(unit)
    for bonus in support_rank_bonuses:
        accuracy += float(bonus.accuracy)
    accuracy = int(accuracy)

    if DB.constants.value('lead'):
        stars = sum(u.stats.get('LEAD', 0) for u in game.get_all_units() if u.team == unit.team)
        accuracy += stars * equations.parser.get('LEAD_HIT', unit)

    accuracy += item_system.modify_accuracy(unit, item)
    accuracy += skill_system.modify_accuracy(unit, item)

    return accuracy

def avoid(unit, item, item_to_avoid=None):
    if not item_to_avoid:
        equation = skill_system.avoid_formula(unit)
    else:
        equation = item_system.avoid_formula(unit, item_to_avoid)
        if equation == 'AVOID':
            equation = skill_system.avoid_formula(unit)
    avoid = equations.parser.get(equation, unit)

    support_rank_bonuses, support_allies = get_support_rank_bonus(unit)
    for bonus in support_rank_bonuses:
        avoid += float(bonus.avoid)
    avoid = int(avoid)

    if DB.constants.value('lead'):
        target_stars = sum(u.stats.get('LEAD', 0) for u in game.get_all_units() if u.team == unit.team)
        avoid += target_stars * equations.parser.get('LEAD_AVOID', unit)

    if item:
        avoid += item_system.modify_avoid(unit, item)
    avoid += skill_system.modify_avoid(unit, item_to_avoid)
    return avoid

def crit_accuracy(unit, item=None):
    if not item:
        item = unit.get_weapon()
    if not item:
        return None

    crit_accuracy = item_system.crit(unit, item)
    if crit_accuracy is None:
        return None

    equation = item_system.crit_accuracy_formula(unit, item)
    if equation == 'CRIT_HIT':
        equation = skill_system.crit_accuracy_formula(unit)
    crit_accuracy += equations.parser.get(equation, unit)

    weapon_rank_bonus = get_weapon_rank_bonus(unit, item)
    if weapon_rank_bonus:
        crit_accuracy += int(weapon_rank_bonus.crit)

    support_rank_bonuses, support_allies = get_support_rank_bonus(unit)
    for bonus in support_rank_bonuses:
        crit_accuracy += float(bonus.crit)
    crit_accuracy = int(crit_accuracy)

    crit_accuracy += item_system.modify_crit_accuracy(unit, item)
    crit_accuracy += skill_system.modify_crit_accuracy(unit, item)

    return crit_accuracy

def crit_avoid(unit, item, item_to_avoid=None):
    if not item_to_avoid:
        equation = skill_system.crit_avoid_formula(unit)
    else:
        equation = item_system.crit_avoid_formula(unit, item_to_avoid)
        if equation == 'CRIT_AVOID':
            equation = skill_system.crit_avoid_formula(unit)
    avoid = equations.parser.get(equation, unit)

    support_rank_bonuses, support_allies = get_support_rank_bonus(unit)
    for bonus in support_rank_bonuses:
        avoid += float(bonus.dodge)
    avoid = int(avoid)

    if item:
        avoid += item_system.modify_crit_avoid(unit, item)
    avoid += skill_system.modify_crit_avoid(unit, item_to_avoid)
    return avoid

def damage(unit, item=None):
    if not item:
        item = unit.get_weapon()
    if not item:
        return None

    might = item_system.damage(unit, item)
    if might is None:
        return None

    equation = item_system.damage_formula(unit, item)
    if equation == 'DAMAGE':
        equation = skill_system.damage_formula(unit)
    might += equations.parser.get(equation, unit)

    weapon_rank_bonus = get_weapon_rank_bonus(unit, item)
    if weapon_rank_bonus:
        might += int(weapon_rank_bonus.damage)

    support_rank_bonuses, support_allies = get_support_rank_bonus(unit)
    for bonus in support_rank_bonuses:
        might += float(bonus.damage)
    might = int(might)

    might += item_system.modify_damage(unit, item)
    might += skill_system.modify_damage(unit, item)

    return might

def defense(atk_unit, def_unit, item, item_to_avoid=None):
    if not item_to_avoid:
        equation = skill_system.resist_formula(def_unit)
    else:
        equation = item_system.resist_formula(atk_unit, item_to_avoid)
        if equation == 'DEFENSE':
            equation = skill_system.resist_formula(def_unit)
    res = equations.parser.get(equation, def_unit)

    support_rank_bonuses, support_allies = get_support_rank_bonus(def_unit)
    for bonus in support_rank_bonuses:
        res += float(bonus.resist)
    res = int(res)

    if item:
        res += item_system.modify_resist(def_unit, item)
    res += skill_system.modify_resist(def_unit, item_to_avoid)
    return res

def attack_speed(unit, item=None):
    if not item:
        item = unit.get_weapon()
    if not item:
        return defense_speed(unit, item)

    equation = item_system.attack_speed_formula(unit, item)
    if equation == 'ATTACK_SPEED':
        equation = skill_system.attack_speed_formula(unit)
    attack_speed = equations.parser.get(equation, unit)

    weapon_rank_bonus = get_weapon_rank_bonus(unit, item)
    if weapon_rank_bonus:
        attack_speed += int(weapon_rank_bonus.attack_speed)

    support_rank_bonuses, support_allies = get_support_rank_bonus(unit)
    for bonus in support_rank_bonuses:
        attack_speed += float(bonus.attack_speed)
    attack_speed = int(attack_speed)

    attack_speed += item_system.modify_attack_speed(unit, item)
    attack_speed += skill_system.modify_attack_speed(unit, item)
    # TODO
    # Support bonus

    if not DB.constants.value('allow_negative_as') and attack_speed < 0:
        attack_speed = 0

    return attack_speed

def defense_speed(unit, item, item_to_avoid=None):
    if not item_to_avoid:
        equation = skill_system.defense_speed_formula(unit)
    else:
        equation = item_system.defense_speed_formula(unit, item_to_avoid)
        if equation == 'DEFENSE_SPEED':
            equation = skill_system.defense_speed_formula(unit)
    speed = equations.parser.get(equation, unit)

    support_rank_bonuses, support_allies = get_support_rank_bonus(unit)
    for bonus in support_rank_bonuses:
        speed += float(bonus.defense_speed)
    speed = int(speed)

    if item:
        speed += item_system.modify_defense_speed(unit, item)
    speed += skill_system.modify_defense_speed(unit, item_to_avoid)

    if not DB.constants.value('allow_negative_as') and speed < 0:
        speed = 0
        
    return speed

def compute_hit(unit, target, item, def_item, mode, attack_info):
    if not item:
        return None

    hit = accuracy(unit, item)
    if hit is None:
        return 10000

    # Handles things like effective accuracy
    hit += item_system.dynamic_accuracy(unit, item, target, resolve_weapon(target), mode, attack_info, hit)

    # Weapon Triangle
    triangle_bonus = 0
    adv = compute_advantage(unit, target, item, def_item)
    disadv = compute_advantage(unit, target, item, def_item, False)
    if adv:
        triangle_bonus += int(adv.accuracy)
    if disadv:
        triangle_bonus += int(disadv.accuracy)

    adv = compute_advantage(target, unit, def_item, item)
    disadv = compute_advantage(target, unit, def_item, item, False)
    if adv:
        triangle_bonus -= int(adv.avoid)
    if disadv:
        triangle_bonus -= int(disadv.avoid)
    hit += triangle_bonus

    # Three Houses style support bonus (only works on attack)
    if mode in ('attack', 'splash'):
        # Attacker's accuracy bonus
        support_rank_bonuses, support_allies = get_support_rank_bonus(unit, target)
        for bonus in support_rank_bonuses:
            hit += float(bonus.accuracy)
    if mode == 'defense':
        # Attacker's avoid bonus
        support_rank_bonuses, support_allies = get_support_rank_bonus(target, unit)
        for bonus in support_rank_bonuses:
            hit -= float(bonus.avoid)
    hit = int(hit)

    hit -= avoid(target, def_item, item)

    hit += skill_system.dynamic_accuracy(unit, item, target, resolve_weapon(target), mode, attack_info, hit)
    hit -= skill_system.dynamic_avoid(target, resolve_weapon(target), unit, item, mode, attack_info, hit)

    return utils.clamp(hit, 0, 100)

def compute_crit(unit, target, item, def_item, mode, attack_info):
    if not item:
        return None

    crit = crit_accuracy(unit, item)
    if crit is None:
        return None

    # Handles things like effective accuracy
    crit += item_system.dynamic_crit_accuracy(unit, item, target, resolve_weapon(target), mode, attack_info, crit)

    # Weapon Triangle
    triangle_bonus = 0
    adv = compute_advantage(unit, target, item, def_item)
    disadv = compute_advantage(unit, target, item, def_item, False)
    if adv:
        triangle_bonus += int(adv.crit)
    if disadv:
        triangle_bonus += int(disadv.crit)

    adv = compute_advantage(target, unit, def_item, item)
    disadv = compute_advantage(target, unit, def_item, item, False)
    if adv:
        triangle_bonus -= int(adv.dodge)
    if disadv:
        triangle_bonus -= int(disadv.dodge)
    crit += triangle_bonus

    # Three Houses style support bonus (only works on attack)
    if mode in ('attack', 'splash'):
        # Attacker's crit bonus
        support_rank_bonuses, support_allies = get_support_rank_bonus(unit, target)
        for bonus in support_rank_bonuses:
            crit += float(bonus.crit)
    if mode == 'defense':
        # Attacker's dodge bonus
        support_rank_bonuses, support_allies = get_support_rank_bonus(target, unit)
        for bonus in support_rank_bonuses:
            crit -= float(bonus.dodge)
    crit = int(crit)

    crit -= crit_avoid(target, def_item, item)

    crit += skill_system.dynamic_crit_accuracy(unit, item, target, resolve_weapon(target), mode, attack_info, crit)
    crit -= skill_system.dynamic_crit_avoid(target, resolve_weapon(target), unit, item, mode, attack_info, crit)

    crit *= skill_system.crit_multiplier(unit, item, target, resolve_weapon(target), mode, attack_info, crit)
    crit = int(crit)

    return utils.clamp(crit, 0, 100)

def compute_damage(unit, target, item, def_item, mode, attack_info, crit=False, assist=False):
    if not item:
        return None

    might = damage(unit, item)
    if might is None:
        return None

    # Handles things like effective damage
    might += item_system.dynamic_damage(unit, item, target, def_item, mode, attack_info, might)
    might += skill_system.dynamic_damage(unit, item, target, def_item, mode, attack_info, might)

    # Weapon Triangle
    triangle_bonus = 0
    adv = compute_advantage(unit, target, item, def_item)
    disadv = compute_advantage(unit, target, item, def_item, False)
    if adv:
        triangle_bonus += int(adv.damage)
    if disadv:
        triangle_bonus += int(disadv.damage)

    adv = compute_advantage(target, unit, def_item, item)
    disadv = compute_advantage(target, unit, def_item, item, False)
    if adv:
        triangle_bonus -= int(adv.resist)
    if disadv:
        triangle_bonus -= int(disadv.resist)
    might += triangle_bonus

    # Three Houses style support bonus (only works on attack)
    if mode in ('attack', 'splash'):
        # Attacker's damage bonus
        support_rank_bonuses, support_allies = get_support_rank_bonus(unit, target)
        for bonus in support_rank_bonuses:
            might += float(bonus.damage)
    if mode == 'defense':
        # Attacker's resist bonus
        support_rank_bonuses, support_allies = get_support_rank_bonus(target, unit)
        for bonus in support_rank_bonuses:
            might -= float(bonus.resist)
    might = int(might)

    total_might = might

    might -= defense(unit, target, def_item, item)
    might -= skill_system.dynamic_resist(target, resolve_weapon(target), unit, item, mode, attack_info, might)

    if assist:
        might //= 2

    # So that crit can correctly multiply when the damage dealt would be less than min_damage
    might = int(max(DB.constants.value('min_damage'), might))

    if crit or skill_system.crit_anyway(unit):
        # Multiply Damage
        equation = skill_system.critical_multiplier_formula(unit)
        crit_mult = equations.parser.get(equation, unit)
        might *= crit_mult

        # Add damage
        equation = skill_system.critical_addition_formula(unit)
        crit_add = equations.parser.get(equation, unit)
        might += crit_add

        # Thracia Crit
        equation = skill_system.thracia_critical_multiplier_formula(unit)
        thracia_crit = equations.parser.get(equation, unit)
        if thracia_crit:
            might += total_might * thracia_crit

    might *= skill_system.damage_multiplier(unit, item, target, resolve_weapon(target), mode, attack_info, might)
    might *= skill_system.resist_multiplier(target, resolve_weapon(target), unit, item, mode, attack_info, might)

    return int(max(DB.constants.get('min_damage').value, might))

def compute_assist_damage(unit, target, item, def_item, mode, attack_info, crit=False):
    return compute_damage(unit, target, item, def_item, mode, attack_info, crit, assist=True)

def compute_true_speed(unit, target, item, def_item, mode, attack_info) -> int:
    speed = attack_speed(unit, item)

    # Handles things like effective damage
    speed += item_system.dynamic_attack_speed(unit, item, target, resolve_weapon(target), mode, attack_info, speed)

    # Weapon Triangle
    triangle_bonus = 0

    adv = compute_advantage(unit, target, item, def_item)
    disadv = compute_advantage(unit, target, item, def_item, False)
    if adv:
        triangle_bonus += int(adv.attack_speed)
    if disadv:
        triangle_bonus += int(disadv.attack_speed)

    adv = compute_advantage(target, unit, def_item, item)
    disadv = compute_advantage(target, unit, def_item, item, False)
    if adv:
        triangle_bonus -= int(adv.defense_speed)
    if disadv:
        triangle_bonus -= int(disadv.defense_speed)

    # Three Houses style support bonus (only works on attack)
    if mode in ('attack', 'splash'):
        # Attacker's attack_speed bonus
        support_rank_bonuses, support_allies = get_support_rank_bonus(unit, target)
        for bonus in support_rank_bonuses:
            speed += float(bonus.attack_speed)
    if mode == 'defense':
        # Attacker's defense_speed bonus
        support_rank_bonuses, support_allies = get_support_rank_bonus(target, unit)
        for bonus in support_rank_bonuses:
            speed -= float(bonus.defense_speed)
    speed = int(speed)

    speed -= defense_speed(target, def_item, item)

    speed += skill_system.dynamic_attack_speed(unit, item, target, resolve_weapon(target), mode, attack_info, speed)
    speed -= skill_system.dynamic_defense_speed(target, resolve_weapon(target), unit, item, mode, attack_info, speed)
    return speed

def outspeed(unit, target, item, def_item, mode, attack_info) -> bool:
    if not item:
        return 1
    if not item_system.can_double(unit, item):
        return 1
    if skill_system.no_double(unit):
        return 1

    speed = compute_true_speed(unit, target, item, def_item, mode, attack_info)

    return 2 if speed >= equations.parser.speed_to_double(unit) else 1

def compute_multiattacks(unit, target, item, mode, attack_info):
    if not item:
        return None

    num_attacks = 1
    num_attacks += item_system.dynamic_multiattacks(unit, item, target, resolve_weapon(target), mode, attack_info, num_attacks)
    num_attacks += skill_system.dynamic_multiattacks(unit, item, target, resolve_weapon(target), mode, attack_info, num_attacks)

    return num_attacks
