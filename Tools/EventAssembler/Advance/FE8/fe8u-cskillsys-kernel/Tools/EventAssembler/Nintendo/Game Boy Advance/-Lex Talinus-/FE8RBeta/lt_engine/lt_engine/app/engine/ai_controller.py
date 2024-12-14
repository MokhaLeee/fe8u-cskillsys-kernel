import logging
import functools
import math
from typing import List

from app.constants import FRAMERATE
from app.data.database.database import DB
from app.engine import (action, combat_calcs, engine, equations, evaluate,
                        item_funcs, item_system, line_of_sight,
                        skill_system)
from app.engine.pathfinding import pathfinding
from app.engine.combat import interaction
from app.engine.game_state import game
from app.engine.movement import movement_funcs
from app.events import triggers
from app.events.regions import RegionType
from app.utilities import utils
from app.utilities.typing import Pos


class AIController():
    def __init__(self):
        # Controls whether we should be skipping through the AI's turns
        self.do_skip: bool = False

        self.reset()

    def skip(self):
        self.do_skip = True

    def end_skip(self):
        self.do_skip = False

    def reset(self):
        self.unit = None
        self.state = "Init"

        self.behaviour_idx = 0
        self.behaviour = None
        self.inner_ai = None

        self.did_something = False

        self.move_ai_complete = False
        self.attack_ai_complete = False
        self.canto_ai_complete = False

    def load_unit(self, unit):
        self.reset()
        self.unit = unit

    def is_done(self):
        return self.move_ai_complete and \
            self.attack_ai_complete and self.canto_ai_complete

    def clean_up(self):
        self.goal_position = None
        self.goal_item = None
        self.goal_target = None

    def set_next_behaviour(self):
        behaviours = DB.ai.get(self.unit.get_ai()).behaviours
        while self.behaviour_idx < len(behaviours):
            next_behaviour = behaviours[self.behaviour_idx]
            self.behaviour_idx += 1
            if not next_behaviour.condition or \
                    evaluate.evaluate(next_behaviour.condition, self.unit, position=self.unit.position):
                self.behaviour = next_behaviour
                break
        else:
            self.behaviour_idx = 0
            self.behaviour = None

    def get_behaviour(self):
        return self.behaviour

    def interrupt(self):
        self.move_ai_complete = True
        self.attack_ai_complete = True
        self.canto_ai_complete = True

    def act(self):
        logging.info("AI Act!")

        change = False
        if movement_funcs.check_region_interrupt(self.unit.position):
            self.interrupt()

        if not self.move_ai_complete:
            if self.think():
                change = self.move()
                self.move_ai_complete = True
        elif not self.attack_ai_complete:
            change = self.attack()
            self.attack_ai_complete = True
        elif not self.canto_ai_complete:
            if self.unit.has_attacked and skill_system.has_canto(self.unit, None):
                action.do(action.SetMovementLeft(self.unit, skill_system.canto_movement(self.unit, None)))
                self.canto_retreat()
                change = self.move()
            self.canto_ai_complete = True

        return self.did_something, change

    def move(self):
        if self.goal_position and self.goal_position != self.unit.position:
            normal_moves = game.path_system.get_valid_moves(self.unit, witch_warp=False)
            witch_warp = set(skill_system.witch_warp(self.unit))
            if self.goal_position in witch_warp and self.goal_position not in normal_moves:
                action.do(action.Warp(self.unit, self.goal_position))
            else:
                path = game.path_system.get_path(self.unit, self.goal_position)
                game.state.change('movement')
                # If we pass in speed=0 to Move it'll use your unit_speed setting
                speedup = 10 if self.do_skip else 0
                action.do(action.Move(self.unit, self.goal_position, path, speed=speedup))
            return True
        else:
            return False

    def attack(self):
        # Attacking or supporting
        if self.goal_target:  # Target is a position tuple
            if not self.goal_item:
                return False
            if not item_funcs.available(self.unit, self.goal_item):
                return False
            if self.goal_item in item_funcs.get_all_items(self.unit):
                if self.unit.can_equip(self.goal_item):
                    action.do(action.EquipItem(self.unit, self.goal_item))
            # Highlights
            if item_system.is_weapon(self.unit, self.goal_item):
                game.highlight.remove_highlights()
                splash_positions = item_system.splash_positions(self.unit, self.goal_item, self.goal_target)
                game.highlight.display_possible_attacks({self.goal_target})
                game.highlight.display_possible_attacks(splash_positions, light=True)
            elif item_system.is_spell(self.unit, self.goal_item):
                game.highlight.remove_highlights()
                splash_positions = item_system.splash_positions(self.unit, self.goal_item, self.goal_target)
                game.highlight.display_possible_spell_attacks({self.goal_target})
                game.highlight.display_possible_spell_attacks(splash_positions, light=True)

            primary_target = game.board.get_unit(self.goal_target)
            if primary_target:
                self.unit.strike_partner, primary_target.strike_partner = \
                    game.target_system.find_strike_partners(self.unit, primary_target, self.goal_item)

            # Used for steal
            if item_system.targets_items(self.unit, self.goal_item):
                # Choose most expensive item that is legal
                target = game.board.get_unit(self.goal_target)
                legal_items = [item for item in target.items if item_system.item_restrict(self.unit, self.goal_item, target, item)]
                items = sorted(legal_items, key=lambda x: item_system.sell_price(self.unit, x) or 0)
                self.goal_item.data['target_item'] = items[-1]

            # Combat
            # Checks to make sure the unit wasn't interrupted during movement
            interaction.start_combat(self.unit, self.goal_target, self.goal_item, ai_combat=True, skip=self.do_skip)
            return True
        # Interacting with regions
        elif self.goal_position and self.behaviour and self.behaviour.action == 'Interact':
            # Get region
            region = None
            for r in game.level.regions:
                if r.contains(self.goal_position) and r.region_type == RegionType.EVENT and r.sub_nid == self.behaviour.target_spec:
                    try:
                        if not r.condition or evaluate.evaluate(r.condition, self.unit, position=self.goal_position, local_args={'region': r}):
                            region = r
                            break
                    except:
                        logging.warning("Could not evaluate region conditional %s" % r.condition)
            if region:
                did_trigger = game.events.trigger(triggers.RegionTrigger(region.sub_nid, self.unit, self.unit.position, region))
                if not did_trigger:  # Just in case we need the generic one
                    did_trigger = game.events.trigger(triggers.OnRegionInteract(self.unit, self.unit.position, region))
                if did_trigger and region.only_once:
                    action.do(action.RemoveRegion(region))
                if did_trigger:
                    action.do(action.HasAttacked(self.unit))
                    return True
        return False

    def canto_retreat(self):
        valid_positions = self.get_true_valid_moves()
        enemy_positions = {u.position for u in game.units if u.position and skill_system.check_enemy(self.unit, u)}
        self.goal_position = utils.farthest_away_pos(self.unit.position, valid_positions, enemy_positions)

    def smart_retreat(self) -> bool:
        valid_positions = self.get_true_valid_moves()

        target_positions = get_targets(self.unit, self.behaviour)

        zero_move = item_funcs.get_max_range(self.unit)
        single_move = zero_move + equations.parser.movement(self.unit)
        double_move = single_move + equations.parser.movement(self.unit)

        target_positions = {(pos, utils.calculate_distance(self.unit.position, pos)) for pos in target_positions}

        if self.behaviour.view_range == -4:
            pass
        elif self.behaviour.view_range == -3:
            target_positions = {(pos, mag) for pos, mag in target_positions if mag < double_move}
        elif self.behaviour.view_range == -2:
            target_positions = {(pos, mag) for pos, mag in target_positions if mag < single_move}
        elif self.behaviour.view_range == -1:
            target_positions = {(pos, mag) for pos, mag in target_positions if mag < zero_move}
        else:
            target_positions = {(pos, mag) for pos, mag in target_positions if mag < self.behaviour.view_range}

        if target_positions and len(valid_positions) > 1:
            self.goal_position = utils.smart_farthest_away_pos(self.unit.position, valid_positions, target_positions)
            return True
        else:
            return False

    def get_true_valid_moves(self) -> set:
        # Guard AI
        if self.behaviour.view_range == -1 and not game.ai_group_active(self.unit.ai_group):
            return {self.unit.position}
        else:
            valid_moves = game.path_system.get_valid_moves(self.unit)
            other_unit_positions = {unit.position for unit in game.units if unit.position and unit is not self.unit}
            valid_moves -= other_unit_positions
            return valid_moves

    def think(self):
        time = engine.get_time()
        success = False
        self.did_something = False
        orig_pos = self.unit.position

        logging.info("*** AI Thinking... ***")

        while True:
            # Can spend up to half a frame thinking
            over_time: bool = engine.get_true_time() - time >= FRAMERATE/2
            logging.info("Current State: %s", self.state)

            if self.state == 'Init':
                self.start_time = engine.get_time()
                logging.info("Starting AI with nid: %s, position: %s, class: %s, AI: %s", self.unit.nid, self.unit.position, self.unit.klass, self.unit.get_ai())
                self.clean_up()
                # Get next behaviour
                self.set_next_behaviour()
                if self.behaviour:
                    logging.info(self.behaviour.action)
                    if self.behaviour.action == "None":
                        pass  # Try again
                    elif self.behaviour.action == "Attack":
                        self.inner_ai = self.build_primary()
                        self.state = "Primary"
                    elif self.behaviour.action == "Support":
                        self.inner_ai = self.build_primary()
                        self.state = "Primary"
                    elif self.behaviour.action == 'Steal':
                        self.inner_ai = self.build_primary()
                        self.state = "Primary"
                    elif self.behaviour.action == 'Interact':
                        self.inner_ai = self.build_secondary()
                        self.state = "Secondary"
                    elif self.behaviour.action == 'Move_to':
                        self.inner_ai = self.build_secondary()
                        self.state = "Secondary"
                    elif self.behaviour.action == "Move_away_from":
                        success = self.smart_retreat()
                        if success:
                            self.state = "Done"
                        else:
                            self.state = "Init"  # Try another behaviour
                else:
                    self.state = 'Done'

            elif self.state == 'Primary':
                done, self.goal_target, self.goal_position, self.goal_item = self.inner_ai.run()
                if done:
                    if self.goal_target:
                        if self.unit.ai_group and game.get_ai_group(self.unit.ai_group):
                            ai_group = game.get_ai_group(self.unit.ai_group)
                            triggered = ai_group.trigger(self.unit.nid, len(game.get_units_in_ai_group(self.unit.ai_group)))
                            if triggered:
                                logging.info("AI group %s activate!", self.unit.ai_group)
                                self.ai_group_ping(ai_group)
                                success = True
                            else:
                                logging.info("AI group %s is not ready to trigger", self.unit.ai_group)
                                # If we didn't trigger, that means this unit is not ready to participate
                                self.clean_up()
                                success = False
                        else:
                            success = True
                        self.state = "Done"
                    else:
                        self.inner_ai = self.build_secondary()
                        self.state = "Secondary"  # Try secondary
                elif over_time:
                    # Make sure to quick move back so that the in-between frames aren't flickering around
                    self.inner_ai.quick_move(self.inner_ai.orig_pos)

            elif self.state == 'Secondary':
                done, self.goal_position = self.inner_ai.run()
                if done:
                    if self.goal_position:
                        if self.goal_position != self.unit.position:
                            if self.unit.ai_group and game.get_ai_group(self.unit.ai_group):
                                ai_group = game.get_ai_group(self.unit.ai_group)
                                triggered = ai_group.trigger(self.unit.nid, len(game.get_units_in_ai_group(self.unit.ai_group)))
                                if triggered:
                                    self.ai_group_ping(ai_group)
                                    success = True
                                else:
                                    # If we didn't trigger, that means this unit is not ready to participate
                                    self.clean_up()
                                    success = False
                            else:
                                success = True
                        self.state = "Done"
                    else:
                        self.state = "Init"  # Try another behaviour

            if self.state == 'Done':
                self.did_something = success
                self.state = 'Init'
                return True

            if over_time:
                break

        return False

    def ai_group_ping(self, ai_group):
        action.do(action.AIGroupPing(ai_group.nid))
        for unit in game.units:
            if unit.team == self.unit.team and unit.ai_group == ai_group.nid:
                if not unit._has_moved and not unit._has_attacked:
                    unit.has_run_ai = False  # So it can be run through the AI state again

    def build_primary(self):
        valid_moves = self.get_true_valid_moves()
        return PrimaryAI(self.unit, valid_moves, self.behaviour)

    def build_secondary(self):
        return SecondaryAI(self.unit, self.behaviour)

class PrimaryAI():
    def __init__(self, unit, valid_moves, behaviour):
        self.max_tp = 0

        self.unit = unit
        self.orig_pos = self.unit.position
        self.orig_item = self.unit.items[0] if self.unit.items else None
        self.behaviour = behaviour

        if self.behaviour.action == "Attack":
            self.items = [item for item in item_funcs.get_all_items(self.unit) if
                          item_funcs.available(self.unit, item)]
            self.extra_abilities = skill_system.get_extra_abilities(self.unit)
            for ability in self.extra_abilities.values():
                self.items.append(ability)
        elif self.behaviour.action == 'Support':
            self.items = [item for item in item_funcs.get_all_items(self.unit) if
                          item_funcs.available(self.unit, item)]
            self.extra_abilities = skill_system.get_extra_abilities(self.unit)
            for ability in self.extra_abilities.values():
                self.items.append(ability)
        elif self.behaviour.action == 'Steal':
            self.items = []
            self.extra_abilities = skill_system.get_extra_abilities(self.unit)
            for ability in self.extra_abilities.values():
                if ability.name == 'Steal':
                    self.items.append(ability)

        self.behaviour_targets = get_targets(self.unit, self.behaviour)

        logging.info("Testing Items: %s", self.items)

        self.item_index = 0
        self.move_index = 0
        self.target_index = 0

        self.valid_moves = list(valid_moves)
        logging.debug(f"Valid Moves: {self.valid_moves}")

        self.best_target = None
        self.best_position = None
        self.best_item = None

        self.item_setup()

    def item_setup(self):
        if self.item_index < len(self.items):
            item = self.items[self.item_index]
            logging.info("Testing %s" % item)
            if self.unit.can_equip(item):
                action.do(action.EquipItem(self.unit, item))
            self.get_all_valid_targets()
            self.possible_moves = self.get_possible_moves()
            logging.info(self.possible_moves)

    def get_all_valid_targets(self):
        item = self.items[self.item_index]
        logging.info("Determining targets for item: %s", item)
        self.valid_targets = list(game.target_system.get_ai_targets(self.unit, self.valid_moves, [item]))
        # Only if we already have some legal targets (ie, ourself)
        if self.valid_targets and 0 in item_funcs.get_range(self.unit, item):
            self.valid_targets += self.valid_moves  # Hack to target self in all valid positions
            self.valid_targets = list(set(self.valid_targets))  # Only uniques
        logging.info("Valid Targets: %s", self.valid_targets)

    def get_possible_moves(self) -> List[Pos]:
        """Given an item and a target, find all positions in valid_moves that I can strike the target at."""
        if self.target_index < len(self.valid_targets) and self.item_index < len(self.items):
            item = self.items[self.item_index]
            target = self.valid_targets[self.target_index]
            moves: List[Pos] = game.target_system.get_possible_attack_positions(self.unit, target, self.valid_moves, item)
            return moves
        else:
            return []

    def quick_move(self, move):
        action.PickUnitUp(self.unit, True).do()
        self.unit.position = move
        action.PutUnitDown(self.unit, True).do()

    def run(self):
        if self.item_index >= len(self.items):
            self.quick_move(self.orig_pos)
            if self.orig_item and self.unit.can_equip(self.orig_item):
                action.do(action.EquipItem(self.unit, self.orig_item))
            return (True, self.best_target, self.best_position, self.best_item)

        elif self.target_index >= len(self.valid_targets):
            self.target_index = 0
            self.item_index += 1
            self.item_setup()

        elif self.move_index >= len(self.possible_moves):
            self.move_index = 0
            self.target_index += 1
            self.possible_moves = self.get_possible_moves()

        else:
            target = self.valid_targets[self.target_index]
            item = self.items[self.item_index]
            # If too many legal targets, just try for the best move first
            # Otherwise it spends way too long trying every possible position to strike from
            if len(self.valid_targets) > 10:
                enemy_positions = {u.position for u in game.units if u.position and skill_system.check_enemy(self.unit, u)}
                move = utils.farthest_away_pos(self.orig_pos, self.possible_moves, enemy_positions)
                if not move:
                    move = self.possible_moves[self.move_index]
            else:
                move = self.possible_moves[self.move_index]

            if self.unit.position != move:
                self.quick_move(move)

            # Check line of sight
            line_of_sight_flag = True
            if DB.constants.value('line_of_sight') and not item_system.ignore_line_of_sight(self.unit, item):
                item_range = item_funcs.get_range(self.unit, item)
                if item_range:
                    max_item_range = max(item_range)
                    valid_targets = line_of_sight.line_of_sight([move], [target], max_item_range)
                    if not valid_targets:
                        line_of_sight_flag = False
                else:
                    line_of_sight_flag = False

            if line_of_sight_flag:
                self.determine_utility(move, target, item)
            self.move_index += 1
            # If too many legal targets, do not bother with every possible move
            if len(self.valid_targets) > 10:
                self.move_index = len(self.possible_moves)

        # Not done yet
        return (False, self.best_target, self.best_position, self.best_item)

    def determine_utility(self, move, target_pos, item):
        tp = 0
        assert self.unit.position == move
        if game.target_system.check_target_from_position(self.unit, item, target_pos):
            main_target_pos, splash = game.target_system.get_target_from_position(self.unit, item, target_pos)
            tp = self.compute_priority(main_target_pos, splash, move, item)

        target = game.board.get_unit(target_pos)
        # Don't target self if I've already moved and I'm not targeting my new position
        if target is self.unit and target_pos != self.unit.position:
            return

        # Don't bother using Primary AI if we're not able to attack
        # Will just fall through to Secondary AI
        if item_system.no_attack_after_move(self.unit, item) or skill_system.no_attack_after_move(self.unit):
            if move != self.orig_pos:
                return

        logging.info("Choice %.5f - Weapon: %s, Position: %s, Target: %s, Target Position: %s", tp, item, move, target.nid if target else '--', target_pos)
        if tp > self.max_tp:
            self.best_target = target_pos
            self.best_position = move
            self.best_item = item
            self.max_tp = tp

    def compute_priority(self, main_target_pos, splash, move, item) -> float:
        tp = 0
        main_target = game.board.get_unit(main_target_pos)
        # Only count main target if it's one of the legal targets
        if main_target and main_target_pos in self.behaviour_targets:
            ai_priority = item_system.ai_priority(self.unit, item, main_target, move)
            ai_priority_multiplier = skill_system.ai_priority_multiplier(main_target)

            # If no ai priority hook defined
            if ai_priority is None:
                pass
            else:
                total_priority = ai_priority * ai_priority_multiplier
                tp += total_priority

            if item_system.damage(self.unit, item) is not None and \
                    skill_system.check_enemy(self.unit, main_target):
                ai_priority = self.default_priority(main_target, item, move)
                tp += ai_priority * ai_priority_multiplier

        for splash_pos in splash:
            target = game.board.get_unit(splash_pos)
            # Only count splash target if it's one of the legal targets
            if not target or splash_pos not in self.behaviour_targets:
                continue
            ai_priority = item_system.ai_priority(self.unit, item, target, move)
            ai_priority_multiplier = skill_system.ai_priority_multiplier(target)
            if ai_priority is None:
                pass
            else:
                total_priority = ai_priority * ai_priority_multiplier
                tp += total_priority

            if item_system.damage(self.unit, item):
                accuracy = utils.clamp(combat_calcs.compute_hit(self.unit, target, item, target.get_weapon(), "attack", (0, 0))/100., 0, 1)
                raw_damage = combat_calcs.compute_damage(self.unit, target, item, target.get_weapon(), "attack", (0, 0))
                lethality = utils.clamp(raw_damage / float(target.get_hp()), 0, 1)
                ai_priority = 3 if lethality * accuracy >= 1 else lethality * accuracy
                if skill_system.check_enemy(self.unit, target):
                    tp += ai_priority * ai_priority_multiplier
                elif skill_system.check_ally(self.unit, target):
                    tp -= ai_priority * ai_priority_multiplier
        return tp

    def default_priority(self, main_target, item, move):
        # Default method
        terms = []
        offense_term = 0
        defense_term = 1

        raw_damage = combat_calcs.compute_damage(self.unit, main_target, item, main_target.get_weapon(), "attack", (0, 0))
        crit_damage = combat_calcs.compute_damage(self.unit, main_target, item, main_target.get_weapon(), "attack", (0, 0), crit=True)

        # Damage I do compared to target's current hp
        lethality = utils.clamp(raw_damage / float(main_target.get_hp()), 0, 1)
        crit_lethality = utils.clamp(crit_damage / float(main_target.get_hp()), 0, 1)
        # Accuracy
        hit_comp = combat_calcs.compute_hit(self.unit, main_target, item, main_target.get_weapon(), "attack", (0, 0))
        if hit_comp:
            accuracy = utils.clamp(hit_comp/100., 0, 1)
        else:
            accuracy = 0
        crit_comp = combat_calcs.compute_crit(self.unit, main_target, item, main_target.get_weapon(), "attack", (0, 0))
        if crit_comp:
            crit_accuracy = utils.clamp(crit_comp/100., 0, 1)
        else:
            crit_accuracy = 0

        # Determine if I would get countered
        # Even if I wouldn't get countered, check anyway how much damage I would take
        target_weapon = main_target.get_weapon()
        target_damage = combat_calcs.compute_damage(main_target, self.unit, target_weapon, item, "defense", (0, 0))
        if not target_damage:
            target_damage = 0
        target_damage = utils.clamp(target_damage/main_target.get_hp(), 0, 1)
        target_accuracy = combat_calcs.compute_hit(main_target, self.unit, target_weapon, item, "defense", (0, 0))
        if not target_accuracy:
            target_accuracy = 0
        target_accuracy = utils.clamp(target_accuracy/100., 0, 1)
        # If I wouldn't get counterattacked, much less important, so multiply by 10 %
        if not combat_calcs.can_counterattack(self.unit, item, main_target, target_weapon):
            target_damage *= 0.3
            target_accuracy *= 0.3

        num_attacks = combat_calcs.outspeed(self.unit, main_target, item, target_weapon, "attack", (0, 0))
        first_strike = lethality * accuracy if lethality >= 1 else 0

        if num_attacks > 1 and target_damage >= 1:
            # Calculate chance I actually get to strike more than once
            num_attacks -= (target_accuracy * (1 - first_strike))

        offense_term += 3 if lethality * accuracy >= 1 else lethality * accuracy * num_attacks
        crit_term = (crit_lethality - lethality) * crit_accuracy * accuracy * num_attacks
        offense_term += crit_term
        defense_term -= target_damage * target_accuracy * (1 - first_strike)
        if offense_term <= 0:
            if accuracy <= 0 and DB.constants.value('attack_zero_hit'):
                logging.info("Accuracy is bad, but continuing with stupid AI")
            elif lethality <= 0 and DB.constants.value('attack_zero_dam'):
                logging.info("Zero Damage, but continuing with stupid AI")
            else:
                logging.info("Offense: %.2f, Defense: %.2f", offense_term, defense_term)
                return 0

        # Only here to break ties
        # Tries to minimize how far the unit should move
        max_distance = equations.parser.movement(self.unit)
        if max_distance > 0:
            distance_term = (max_distance - utils.calculate_distance(move, self.orig_pos)) / float(max_distance)
        else:
            distance_term = 1

        logging.info("Damage: %.2f, Accuracy: %.2f, Crit Accuracy: %.2f", lethality, accuracy, crit_accuracy)
        logging.info("Offense: %.2f, Defense: %.2f, Distance: %.2f", offense_term, defense_term, distance_term)
        ai_prefab = DB.ai.get(self.unit.get_ai())
        offense_bias = ai_prefab.offense_bias
        offense_weight = offense_bias * (1 / (offense_bias + 1))
        defense_weight = 1 - offense_weight
        terms.append((offense_term, offense_weight))
        terms.append((defense_term, defense_weight))
        terms.append((distance_term, .0001))

        return utils.process_terms(terms)

def handle_unit_spec(all_targets, behaviour):
    target_spec = behaviour.target_spec
    if not target_spec:
        return all_targets
    invert = bool(behaviour.invert_targeting)
    # Uses ^ (which is XOR) to handle inverting the targeting
    if target_spec[0] == "Tag":
        all_targets = [pos for pos in all_targets if any((target_spec[1] in u.tags) ^ invert for u in game.board.get_units(pos))]
    elif target_spec[0] == "Class":
        all_targets = [pos for pos in all_targets if any((u.klass == target_spec[1]) ^ invert for u in game.board.get_units(pos))]
    elif target_spec[0] == "Name":
        all_targets = [pos for pos in all_targets if any((u.name == target_spec[1]) ^ invert for u in game.board.get_units(pos))]
    elif target_spec[0] == 'Faction':
        all_targets = [pos for pos in all_targets if any((u.faction == target_spec[1]) ^ invert for u in game.board.get_units(pos))]
    elif target_spec[0] == 'Party':
        all_targets = [pos for pos in all_targets if any((u.party == target_spec[1]) ^ invert for u in game.board.get_units(pos))]
    elif target_spec[0] == 'ID':
        all_targets = [pos for pos in all_targets if any((u.nid == target_spec[1]) ^ invert for u in game.board.get_units(pos))]
    elif target_spec[0] == 'Team':
        all_targets = [pos for pos in all_targets if any((u.team == target_spec[1]) ^ invert for u in game.board.get_units(pos))]
    return all_targets

def get_targets(unit, behaviour):
    all_targets = []
    if behaviour.target == 'Unit':
        all_targets = [u.position for u in game.units if u.position]
    elif behaviour.target == 'Enemy':
        all_targets = [u.position for u in game.units if u.position and skill_system.check_enemy(unit, u)]
    elif behaviour.target == 'Ally':
        all_targets = [u.position for u in game.units if u.position and skill_system.check_ally(unit, u)]
    elif behaviour.target == 'Event':
        target_spec = behaviour.target_spec
        all_targets = []
        for region in game.level.regions:
            try:
                if region.region_type == RegionType.EVENT and region.sub_nid == target_spec and (not region.condition or evaluate.evaluate(region.condition, unit, local_args={'region': region})):
                    all_targets += region.get_all_positions()
            except:
                logging.warning("Region Condition: Could not parse %s" % region.condition)
        all_targets = list(set(all_targets))  # Remove duplicates
    elif behaviour.target == 'Position':
        if behaviour.target_spec == "Starting":
            if unit.starting_position:
                all_targets = [unit.starting_position]
        else:
            all_targets = [tuple(behaviour.target_spec)]
    if behaviour.target in ('Unit', 'Enemy', 'Ally'):
        all_targets = handle_unit_spec(all_targets, behaviour)

    if behaviour.target != 'Position':
        if DB.constants.value('ai_fog_of_war'):
            all_targets = [
                pos for pos in all_targets if
                game.board.in_vision(pos, unit.team) or
                (game.board.get_unit(pos) and 'Tile' in game.board.get_unit(pos).tags) # Can always targets Tiles
            ]
    return all_targets

class SecondaryAI():
    def __init__(self, unit, behaviour):
        self.unit = unit
        self.behaviour = behaviour
        self.view_range = self.behaviour.view_range
        if self.view_range == -4 or game.ai_group_active(self.unit.ai_group):
            self.view_range = -3  # Try this first

        self.available_targets = []

        # Determine all targets
        self.all_targets = get_targets(self.unit, behaviour)

        self.zero_move = item_funcs.get_max_range(self.unit)
        self.single_move = self.zero_move + equations.parser.movement(self.unit)
        self.double_move = self.single_move + equations.parser.movement(self.unit)

        movement_group = movement_funcs.get_movement_group(self.unit)
        self.grid = game.board.get_movement_grid(movement_group)
        self.pathfinder = \
            pathfinding.AStar(self.unit.position, None, self.grid)

        self.widen_flag = False  # Determines if we've widened our search
        self.reset()

    def reset(self):
        self.max_tp = 0
        self.best_target = 0
        self.best_path = None

        limit = self.get_limit()
        self.available_targets = [t for t in self.all_targets if utils.calculate_distance(self.unit.position, t) <= limit]

        self.best_position = None

    def get_limit(self) -> int:
        # Make sure we don't exceed double move
        if self.widen_flag or self.view_range == -4:
            limit = 99
        elif self.view_range == -3:
            limit = self.double_move
        elif self.view_range == -2:
            if self.behaviour.action in ('Attack', 'Support', 'Steal'):
                limit = -1  # Because the primary AI should have already taken care of this...
            else:
                limit = self.single_move
        elif self.view_range == -1:
            limit = -1
        else:
            limit = self.view_range
        return limit

    def run(self):
        if self.available_targets:
            target = self.available_targets.pop()
            # Find a path to the target
            path = self.get_path(target)
            if not path:
                logging.info("No valid path to %s.", target)
                return False, None
            # We found a path
            tp = self.compute_priority(target, len(path))
            logging.info("Path to %s. -- %s", target, tp)
            if tp > self.max_tp:
                self.max_tp = tp
                self.best_target = target
                self.best_path = path

        elif self.best_target:
            self.best_position = game.path_system.travel_algorithm(self.best_path, self.unit.movement_left, self.unit, self.grid)
            logging.info("Best Target: %s", self.best_target)
            logging.info("Best Position: %s", self.best_position)
            return True, self.best_position

        else:
            if (self.behaviour.view_range == -4 or game.ai_group_active(self.unit.ai_group)) and not self.widen_flag:
                logging.info("Widening search!")
                self.widen_flag = True
                self.view_range = -4
                self.available_targets = [t for t in self.all_targets if t not in self.available_targets]
            else:  # No targets possible
                return True, None
        return False, None

    def get_path(self, goal_pos):
        self.pathfinder.set_goal_pos(goal_pos)

        if self.behaviour.target == 'Event':
            adj_good_enough = False
        elif self.behaviour.target == 'Position' and not game.board.get_unit(goal_pos):
            adj_good_enough = False  # Don't move adjacent if it's not necessary
        else:
            adj_good_enough = True

        limit = self.get_limit()
        if skill_system.pass_through(self.unit):
            can_move_through = lambda adj: True
        else:
            can_move_through = functools.partial(game.board.can_move_through, self.unit.team)
        path = self.pathfinder.process(can_move_through, adj_good_enough=adj_good_enough, limit=limit)
        self.pathfinder.reset()
        return path

    def default_priority(self, enemy):
        hp_max = equations.parser.hitpoints(enemy)
        weakness_term = float(hp_max - enemy.get_hp()) / hp_max

        items = [item for item in item_funcs.get_all_items(self.unit) if
                 item_funcs.available(self.unit, item)]

        terms = []
        tp, highest_damage_term, highest_status_term = 0, 0, 0

        for item in items:
            status_term = 1 if item.status_on_hit else 0
            true_damage = 0
            if item_system.is_weapon(self.unit, item) or item_system.is_spell(self.unit, item):
                raw_damage = combat_calcs.compute_damage(self.unit, enemy, item, enemy.get_weapon(), 'attack', (0, 0))
                hit = utils.clamp(combat_calcs.compute_hit(self.unit, enemy, item, enemy.get_weapon(), 'attack', (0, 0))/100., 0, 1)
                if raw_damage:
                    true_damage = raw_damage * hit
                else:
                    true_damage = 0

            if true_damage <= 0 and status_term <= 0:
                continue  # If no damage could be dealt, ignore
            damage_term = min(float(true_damage / hp_max), 1.)
            new_tp = damage_term + status_term/2
            if new_tp > tp:
                tp = new_tp
                highest_damage_term = damage_term
                highest_status_term = status_term

        if highest_status_term == 0 and highest_damage_term == 0:
            # Just don't include any of this
            return terms
        terms.append((highest_damage_term, 15))
        terms.append((highest_status_term, 10))
        terms.append((weakness_term, 15))
        return terms

    def compute_priority(self, target, distance=0):
        terms = []
        if distance:
            distance_term = 1 - math.log(distance)/4.
        else:
            target_distance = utils.calculate_distance(self.unit.position, target)
            distance_term = 1 - math.log(target_distance)/4.
        terms.append((distance_term, 60))

        enemy = game.board.get_unit(target)
        if self.behaviour.action == "Attack" and enemy:
            new_terms = self.default_priority(enemy)
            if new_terms:
                terms += new_terms
            else:
                return 0
        elif self.behaviour.action == 'Support' and enemy:
            ally = enemy
            # Try to help others since we already checked ourself in Primary AI
            if ally is self.unit:
                return 0
            else:
                max_hp = ally.get_max_hp()
                missing_health = max_hp - ally.get_hp()
                help_term = utils.clamp(missing_health / float(max_hp), 0, 1)
                terms.append((help_term, 100))

        elif self.behaviour.action == "Steal" and enemy:
            return 0  # TODO: For now, Steal just won't work with secondary AI
        else:
            pass

        return utils.process_terms(terms)
