from __future__ import annotations
import math
from typing import Callable, List

from app.data.database.database import DB
from app.data.database.difficulty_modes import RNGOption
from app.engine import action, combat_calcs, item_funcs, item_system, skill_system
from app.engine.game_state import game
from app.engine.combat import playback as pb
from app.engine.combat.playback import PlaybackBrush
from app.utilities import static_random
from app.utilities.enums import Strike
from app.engine.combat.utils import resolve_weapon


import logging

class SolverState():
    name = None

    def get_next_state(self, solver):
        return None

    def process(self, solver, actions, playback):
        return None

    def process_command(self, command):
        if command.lower() in ('hit2', 'crit2', 'miss2'):
            return 'defender'
        elif command.lower() in ('hit1', 'crit1', 'miss1'):
            return 'attacker'
        elif command.lower() == 'end':
            return 'done'
        return None

class InitState(SolverState):
    name = 'init'

    def get_next_state(self, solver):
        command = solver.get_script()
        if command == '--':
            if solver.defender_has_vantage():
                return 'defender'
            else:
                return 'attacker'
        else:
            return self.process_command(command)

class AttackerState(SolverState):
    name = 'attacker'
    num_multiattacks = 1

    def get_next_state(self, solver):
        command = solver.get_script()

        can_double_in_pairup = not DB.constants.value('limit_attack_stance')

        if solver.attacker_alive() and (not solver.defender or solver.defender_alive()):
            if command == '--':
                if solver.defender:
                    if DB.constants.value('def_double') or skill_system.def_double(solver.defender):
                        defender_outspeed = combat_calcs.outspeed(solver.defender, solver.attacker, solver.def_item, solver.main_item, 'defense', solver.get_defense_info())
                    else:
                        defender_outspeed = 1
                    attacker_outspeed = combat_calcs.outspeed(solver.attacker, solver.defender, solver.main_item, solver.def_item, 'attack', solver.get_attack_info())
                else:
                    attacker_outspeed = defender_outspeed = 1

                if solver.attacker.strike_partner and \
                        (solver.num_attacks == 1 or can_double_in_pairup) and \
                        solver.num_subattacks >= self.num_multiattacks:
                    solver.num_subattacks = 0
                    return 'attacker_partner'
                elif solver.item_has_uses() and \
                        solver.num_subattacks < self.num_multiattacks:
                    return 'attacker'
                elif solver.item_has_uses() and \
                        solver.attacker_has_desperation() and \
                        solver.num_attacks < attacker_outspeed:
                    solver.num_subattacks = 0
                    return 'attacker'
                elif solver.allow_counterattack() and \
                        solver.num_defends < defender_outspeed:
                    solver.num_subdefends = 0
                    return 'defender'
                elif solver.item_has_uses() and \
                        solver.num_attacks < attacker_outspeed:
                    solver.num_subattacks = 0
                    return 'attacker'
                return None
            else:
                return self.process_command(command)
        elif solver.attacker_alive() and not solver.defender:
            if solver.attacker.strike_partner and \
                    (solver.num_attacks == 1 or can_double_in_pairup) and \
                    solver.num_subattacks >= self.num_multiattacks:
                solver.num_subattacks = 0
                return 'attacker_partner'
        else:
            return 'done'

    def process(self, solver, actions, playback):
        playback.append(pb.AttackerPhase())
        attack_info = solver.get_attack_info()
        # Check attack proc
        skill_system.start_sub_combat(actions, playback, solver.attacker, solver.main_item, solver.defender, resolve_weapon(solver.defender),  'attack', attack_info)
        for idx, item in enumerate(solver.items):
            defender = solver.defenders[idx]
            if DB.constants.value('double_splash'):
                splash = solver.splashes[idx]
            elif solver.num_attacks == 0 and solver.num_subattacks == 0:
                splash = solver.splashes[idx]
            else:
                splash = []
            target_pos = solver.target_positions[idx]
            if defender:
                skill_system.start_sub_combat(actions, playback, defender, resolve_weapon(defender), solver.attacker, solver.main_item, 'defense', attack_info)
                if solver.update_stats:
                    solver.update_stats(playback)
                solver.process(actions, playback, solver.attacker, defender, target_pos, item, resolve_weapon(defender), 'attack', attack_info)
                skill_system.end_sub_combat(actions, playback, defender, resolve_weapon(defender), solver.attacker, solver.main_item, 'defense', attack_info)
            for target in splash:
                skill_system.start_sub_combat(actions, playback, target, None, solver.attacker, solver.main_item, 'defense', attack_info)
                solver.process(actions, playback, solver.attacker, target, target_pos, item, None, 'splash', attack_info)
                skill_system.end_sub_combat(actions, playback, target, None, solver.attacker, solver.main_item, 'defense', attack_info)
            # Make sure that we run on_hit even if otherwise unavailable
            if not defender and not splash:
                solver.simple_process(actions, playback, solver.attacker, solver.attacker, target_pos, item, None, None, None)

        solver.num_subattacks += 1
        self.num_multiattacks = combat_calcs.compute_multiattacks(solver.attacker, solver.defender, solver.main_item, 'attack', attack_info)
        if solver.num_subattacks >= self.num_multiattacks:
            solver.num_attacks += 1
        # If we are trying to go for a subattack, but there is no defender and we don't do double splash
        # just skip the remaining attacks
        elif not DB.constants.value('double_splash') and all(defender is None for defender in solver.defenders):
            solver.num_subattacks = self.num_multiattacks
            solver.num_attacks += 1

        # End check attack proc
        skill_system.end_sub_combat(actions, playback, solver.attacker, solver.main_item, solver.defender, resolve_weapon(solver.defender),  'attack', attack_info)

class AttackerPartnerState(SolverState):
    name = 'attacker_partner'
    num_multiattacks = 1
    # Nearly identical to attacker state except contains no possibility that attacker partner is the next in line to attack

    def get_next_state(self, solver):
        command = solver.get_script()

        if solver.attacker_alive() and (not solver.defender or solver.defender_alive()):
            if command == '--':
                if solver.defender:
                    if DB.constants.value('def_double') or skill_system.def_double(solver.defender):
                        defender_outspeed = \
                            combat_calcs.outspeed(solver.defender, solver.attacker, solver.def_item, solver.main_item, 'defense', solver.get_defense_info())
                    else:
                        defender_outspeed = 1
                    attacker_outspeed = \
                        combat_calcs.outspeed(solver.attacker, solver.defender, solver.main_item, solver.def_item, 'attack', solver.get_attack_info())
                else:
                    attacker_outspeed = defender_outspeed = 1

                if solver.item_has_uses() and \
                        solver.num_subattacks < self.num_multiattacks:
                    return 'attacker_partner'
                elif solver.item_has_uses() and \
                        solver.attacker_has_desperation() and \
                        solver.num_attacks < attacker_outspeed:
                    solver.num_subattacks = 0
                    return 'attacker'
                elif solver.allow_counterattack() and \
                        solver.num_defends < defender_outspeed:
                    solver.num_subdefends = 0
                    return 'defender'
                elif solver.item_has_uses() and \
                        solver.num_attacks < attacker_outspeed:
                    solver.num_subattacks = 0
                    return 'attacker'
                return None
            else:
                return self.process_command(command)
        else:
            return 'done'

    def process(self, solver, actions, playback):
        playback.append(pb.AttackerPartnerPhase())
        # Check attack proc
        atk_p = solver.attacker.strike_partner
        attack_info = solver.get_attack_info()
        skill_system.start_sub_combat(actions, playback, atk_p, solver.main_item, solver.defender, resolve_weapon(solver.defender), 'attack', attack_info)
        for idx, item in enumerate(solver.items):
            defender = solver.defenders[idx]
            splash = solver.splashes[idx]
            target_pos = solver.target_positions[idx]
            if defender:
                skill_system.start_sub_combat(actions, playback, defender, resolve_weapon(defender), atk_p, solver.main_item, 'defense', attack_info)
                if solver.update_stats:
                    solver.update_stats(playback)
                solver.process(actions, playback, atk_p, defender, target_pos, item, resolve_weapon(defender), 'attack', attack_info, assist=True)
                skill_system.end_sub_combat(actions, playback, defender, resolve_weapon(defender), atk_p, solver.main_item, 'defense', attack_info)
            for target in splash:
                skill_system.start_sub_combat(actions, playback, target, None, atk_p, solver.main_item, 'defense', attack_info)
                solver.process(actions, playback, atk_p, target, target_pos, item, None, 'attack', attack_info, assist=True)
                skill_system.end_sub_combat(actions, playback, target, None, atk_p, solver.main_item, 'defense', attack_info)
            # Make sure that we run on_hit even if otherwise unavailable
            if not defender and not splash:
                solver.simple_process(actions, playback, atk_p, atk_p, target_pos, item, None, None, None)

        solver.num_subattacks += 1
        # End check attack proc
        skill_system.end_sub_combat(actions, playback, atk_p, solver.main_item, solver.defender, resolve_weapon(solver.defender), 'attack', attack_info)

class DefenderState(SolverState):
    name = 'defender'
    num_multiattacks = 1

    def get_next_state(self, solver):
        command = solver.get_script()

        can_double_in_pairup = not DB.constants.value('limit_attack_stance')

        if solver.attacker_alive() and solver.defender_alive():
            if command == '--':
                if DB.constants.value('def_double') or skill_system.def_double(solver.defender):
                    defender_outspeed = combat_calcs.outspeed(solver.defender, solver.attacker, solver.def_item, solver.main_item, 'defense', solver.get_defense_info())
                else:
                    defender_outspeed = 1

                attacker_outspeed = combat_calcs.outspeed(solver.attacker, solver.defender, solver.main_item, solver.def_item, 'attack', solver.get_attack_info())

                if solver.defender.strike_partner and \
                        (solver.num_defends == 1 or can_double_in_pairup) and \
                        solver.num_subdefends >= self.num_multiattacks:
                    solver.num_subdefends = 0
                    return 'defender_partner'
                if solver.allow_counterattack() and \
                        solver.num_subdefends < self.num_multiattacks:
                    return 'defender'
                elif solver.allow_counterattack() and \
                        solver.defender_has_desperation() and \
                        solver.num_defends < defender_outspeed:
                    solver.num_subdefends = 0
                    return 'defender'
                elif solver.item_has_uses() and \
                        solver.num_attacks < attacker_outspeed:
                    solver.num_subattacks = 0
                    return 'attacker'
                elif solver.allow_counterattack() and \
                        solver.num_defends < defender_outspeed:
                    solver.num_subdefends = 0
                    return 'defender'
                return None
            else:
                return self.process_command(command)
        else:
            return 'done'

    def process(self, solver, actions, playback):
        playback.append(pb.DefenderPhase())
        attack_info = solver.get_defense_info()
        # Check for proc skills
        skill_system.start_sub_combat(actions, playback, solver.defender, solver.def_item, solver.attacker, solver.main_item, 'attack', attack_info)
        skill_system.start_sub_combat(actions, playback, solver.attacker, solver.main_item, solver.defender, solver.def_item, 'defense', attack_info)

        if solver.update_stats:
            solver.update_stats(playback)
        solver.process(actions, playback, solver.defender, solver.attacker, solver.attacker.position, solver.def_item, solver.main_item, 'defense', attack_info)

        # Remove defending unit's proc skills (which is solver.attacker)
        skill_system.end_sub_combat(actions, playback, solver.attacker, solver.main_item, solver.defender, solver.def_item, 'defense', attack_info)

        solver.num_subdefends += 1
        self.num_multiattacks = combat_calcs.compute_multiattacks(solver.defender, solver.attacker, solver.def_item, 'defense', attack_info)
        if solver.num_subdefends >= self.num_multiattacks:
            solver.num_defends += 1

        # Remove attacking unit's proc skills (which is solver.defender)
        skill_system.end_sub_combat(actions, playback, solver.defender, solver.def_item, solver.attacker, solver.main_item, 'attack', attack_info)

class DefenderPartnerState(SolverState):
    name = 'defender_partner'
    num_multiattacks = 1
    # Nearly identical to defender state except contains no possibility that defender partner is the next in line to attack

    def get_next_state(self, solver):
        command = solver.get_script()
        if solver.attacker_alive() and solver.defender_alive():
            if command == '--':
                if DB.constants.value('def_double') or skill_system.def_double(solver.defender):
                    defender_outspeed = \
                        combat_calcs.outspeed(solver.defender, solver.attacker, solver.def_item, solver.main_item, 'defense', solver.get_defense_info())
                else:
                    defender_outspeed = 1
                attacker_outspeed = \
                    combat_calcs.outspeed(solver.attacker, solver.defender, solver.main_item, solver.def_item, 'attack', solver.get_attack_info())

                if solver.allow_counterattack() and \
                        solver.num_subdefends < self.num_multiattacks:
                    return 'defender_partner'
                elif solver.allow_counterattack() and \
                        solver.defender_has_desperation() and \
                        solver.num_defends < defender_outspeed:
                    solver.num_subdefends = 0
                    return 'defender'
                elif solver.item_has_uses() and \
                        solver.num_attacks < attacker_outspeed:
                    solver.num_subattacks = 0
                    return 'attacker'
                elif solver.allow_counterattack() and \
                        solver.num_defends < defender_outspeed:
                    solver.num_subdefends = 0
                    return 'defender'
                return None
            else:
                return self.process_command(command)
        else:
            return 'done'

    def process(self, solver, actions, playback):
        playback.append(pb.DefenderPartnerPhase())
        def_p = solver.defender.strike_partner
        attack_info = solver.get_defense_info()
        # Check for proc skills
        skill_system.start_sub_combat(actions, playback, def_p, solver.def_item, solver.attacker, solver.main_item, 'attack', attack_info)
        skill_system.start_sub_combat(actions, playback, solver.attacker, solver.main_item, def_p, solver.def_item, 'defense', attack_info)

        if solver.update_stats:
            solver.update_stats(playback)
        solver.process(actions, playback, def_p, solver.attacker, solver.attacker.position, solver.def_item, solver.main_item, 'defense', attack_info, assist=True)

        # Remove defending unit's proc skills (which is solver.attacker)
        skill_system.end_sub_combat(actions, playback, solver.attacker, solver.main_item, def_p, solver.def_item, 'defense', attack_info)
        solver.num_subdefends += 1
        # Remove attacking unit's proc skills (which is solver.defender)
        skill_system.end_sub_combat(actions, playback, def_p, solver.def_item, solver.attacker, solver.main_item, 'attack', attack_info)

class CombatPhaseSolver():
    states = {'init': InitState,
              'attacker': AttackerState,
              'defender': DefenderState,
              'attacker_partner': AttackerPartnerState,
              'defender_partner': DefenderPartnerState}

    def __init__(self, attacker, main_item, items, defenders,
                 splashes, target_positions, defender, def_item,
                 script=None, total_rounds=1, update_stats: Callable[List[PlaybackBrush]] = None):
        self.attacker = attacker
        self.main_item = main_item
        self.items = items
        self.defenders = defenders
        self.splashes = splashes
        self.target_positions = target_positions
        self.defender = defender
        self.def_item = def_item

        self.state = InitState()
        self.reset()

        # For having multi round ("arena") combats
        self.num_rounds = 0
        self.total_rounds = total_rounds

        # For event combats
        self.script = list(reversed(script)) if script else []
        self.current_command = '--'

        # Used to update the Combat's GUI at just the right time!
        self.update_stats: Callable[List[PlaybackBrush]] = update_stats

    def reset(self):
        self.num_attacks, self.num_defends = 0, 0
        self.num_subattacks, self.num_subdefends = 0, 0

    def get_attack_info(self) -> tuple:
        return self.num_attacks, self.num_subattacks

    def get_defense_info(self) -> tuple:
        return self.num_defends, self.num_subdefends

    def get_state(self):
        return self.state

    def do(self):
        actions, playback = [], []
        self.state.process(self, actions, playback)
        return actions, playback

    def get_next_state(self) -> str:
        # This is just used to determine what the next state will be
        if self.state:
            return self.state.name
        return None

    def setup_next_state(self):
        # Does actually change the state
        next_state = self.state.get_next_state(self)
        logging.debug("Next State: %s" % next_state)
        if next_state == 'done':
            self.state = None
        elif next_state:
            self.state = self.states[next_state]()
        else:  # Round complete
            self.num_rounds += 1
            self.reset()
            if self.num_rounds >= self.total_rounds:
                self.state = None
            else:
                self.state = InitState()

    def get_script(self):
        if self.script:
            self.current_command = self.script.pop()
        else:
            self.current_command = '--'
        return self.current_command

    def generate_roll(self):
        rng_mode = game.mode.rng_choice
        if rng_mode == RNGOption.CLASSIC:
            roll = static_random.get_combat()
        elif rng_mode == RNGOption.TRUE_HIT:
            roll = (static_random.get_combat() + static_random.get_combat()) // 2
        elif rng_mode == RNGOption.TRUE_HIT_PLUS:
            roll = (static_random.get_combat() + static_random.get_combat() + static_random.get_combat()) // 3
        elif rng_mode == RNGOption.FATES_HIT:
            roll = static_random.get_combat()
        elif rng_mode == RNGOption.GRANDMASTER:
            roll = 0
        else:  # Default to True Hit
            logging.error("Not a valid rng_mode: %s (defaulting to true hit)", game.mode.rng_choice)
            roll = (static_random.get_combat() + static_random.get_combat()) // 2
        return roll

    def generate_crit_roll(self):
        return static_random.get_combat()
    
    def calculate_fates_hit(self, hit):
        """
        Modified slightly from the actual Fates formula to instead compare against values from 0 - 100, 
        rather than from 0 - 10000. This is so we can use the existing functions in the engine that
        rely on combat rolls being 0-99, rather than 0-9999.
        The only change is dividing the value by 100 and then rounding it to the nearest integer.
        """
        return int(round(hit + (40 / 3) * (hit / 100) * math.sin(math.radians((0.02 * hit - 1) * 180))))

    def process(self, actions, playback, attacker, defender, def_pos, item, def_item, mode, attack_info, assist=False):
        # Is the item I am processing the first one?
        first_item = item in (self.main_item, self.def_item, self.items[0])
        if assist:
            item = attacker.get_weapon()

        to_hit = combat_calcs.compute_hit(attacker, defender, item, def_item, mode, attack_info)
        if game.mode.rng_choice == RNGOption.FATES_HIT:
            to_hit = self.calculate_fates_hit(to_hit)

        if self.current_command.lower() in ('hit1', 'hit2', 'crit1', 'crit2'):
            roll = -1
        elif self.current_command.lower() in ('miss1', 'miss2'):
            roll = 100
        else:
            roll = self.generate_roll()

        guard_hit = False
        if DB.constants.value('pairup') and item_system.is_weapon(attacker, item) and skill_system.check_enemy(attacker, defender):
            if defender.get_guard_gauge() >= defender.get_max_guard_gauge() and defender.traveler:
                guard_hit = True
                roll = -1

        if roll < to_hit:
            crit = False
            if DB.constants.value('crit') or skill_system.crit_anyway(attacker) or self.current_command in ('crit1', 'crit2') \
                    and not guard_hit:
                to_crit = combat_calcs.compute_crit(attacker, defender, item, def_item, mode, attack_info)
                if self.current_command.lower() in ('crit1', 'crit2'):
                    crit = True
                elif self.current_command.lower() in ('hit1', 'hit2', 'miss1', 'miss2'):
                    crit = False
                elif to_crit is not None:
                    crit_roll = self.generate_crit_roll()
                    if crit_roll < to_crit:
                        crit = True

            if crit and not guard_hit:
                skill_system.before_crit(actions, playback, attacker, item, defender, resolve_weapon(defender), mode, attack_info)
                item_system.on_crit(actions, playback, attacker, item, defender, resolve_weapon(defender), def_pos, mode, attack_info, first_item)
                if defender:
                    playback.append(pb.MarkCrit(attacker, defender, self.attacker, item))
            elif roll >= to_hit - DB.constants.value('glancing_hit') and not guard_hit:
                item_system.on_glancing_hit(actions, playback, attacker, item, defender, resolve_weapon(defender), def_pos, mode, attack_info, first_item)
                if defender:
                    playback.append(pb.MarkHit(attacker, defender, self.attacker, item, guard_hit))
                    playback.append(pb.MarkGlancingHit(attacker, defender, self.attacker, item))
            else:
                if guard_hit: # Mocks the playback that would be created in weapon_components
                    playback.append(pb.DamageHit(attacker, item, defender, 0, 0))
                    playback.append(pb.HitSound('No Damage'))
                    playback.append(pb.HitAnim('MapNoDamage', defender))
                else:
                    item_system.on_hit(actions, playback, attacker, item, defender, resolve_weapon(defender), def_pos, mode, attack_info, first_item)
                if defender:
                    playback.append(pb.MarkHit(attacker, defender, self.attacker, item, guard_hit))
            if not guard_hit:
                strike = Strike.CRIT if crit else Strike.HIT
                item_system.after_strike(actions, playback, attacker, item, defender, resolve_weapon(defender), mode, attack_info, strike)
                skill_system.after_strike(actions, playback, attacker, item, defender, resolve_weapon(defender), mode, attack_info, strike)
                skill_system.after_take_strike(actions, playback, defender, def_item, attacker, item, mode, attack_info, strike)
        else:
            item_system.on_miss(actions, playback, attacker, item, defender, resolve_weapon(defender), def_pos, mode, attack_info, first_item)
            item_system.after_strike(actions, playback, attacker, item, defender, resolve_weapon(defender), mode, attack_info, Strike.MISS)
            skill_system.after_strike(actions, playback, attacker, item, defender, resolve_weapon(defender), mode, attack_info, Strike.MISS)
            skill_system.after_take_strike(actions, playback, defender, def_item, attacker, item, mode, attack_info, Strike.MISS)
            if defender:
                playback.append(pb.MarkMiss(attacker, defender, self.attacker, item))

        # Gauge is set to 0. Damage is negated elsewhere
        if DB.constants.value('pairup') and item_system.is_weapon(attacker, item) and skill_system.check_enemy(attacker, defender):
            if defender.get_guard_gauge() >= defender.get_max_guard_gauge():
                action.do(action.SetGauge(defender, 0))
            elif defender.traveler:
                action.do(action.IncGauge(defender, defender.get_gauge_inc()))
            if attacker.traveler:
                action.do(action.IncGauge(attacker, attacker.get_gauge_inc()))

    def simple_process(self, actions, playback, attacker, defender, def_pos, item, def_item, mode, attack_info):
        # Is the item I am processing the first one?
        first_item = item is self.main_item or item is self.items[0]

        item_system.on_hit(actions, playback, attacker, item, defender, resolve_weapon(defender), def_pos, mode, (0, 0), first_item)
        if defender:
            playback.append(pb.MarkHit(attacker, defender, self.attacker, item, False))

    def attacker_alive(self):
        return self.attacker.get_hp() > 0 or skill_system.ignore_dying_in_combat(self.attacker)

    def defender_alive(self):
        return self.defender and (self.defender.get_hp() > 0 or skill_system.ignore_dying_in_combat(self.defender))

    def defender_has_vantage(self) -> bool:
        return self.defender and self.allow_counterattack() and \
            (skill_system.vantage(self.defender) or skill_system.disvantage(self.attacker))

    def attacker_has_desperation(self) -> bool:
        return skill_system.desperation(self.attacker)

    def defender_has_desperation(self) -> bool:
        return self.defender and self.allow_counterattack() and \
            skill_system.desperation(self.defender)

    def allow_counterattack(self) -> bool:
        return self.defender and combat_calcs.can_counterattack(self.attacker, self.main_item, self.defender, self.def_item)

    def item_has_uses(self):
        return item_funcs.available(self.attacker, self.main_item)

    def target_item_has_uses(self):
        return self.defender and self.def_item and item_funcs.available(self.defender, self.def_item)
