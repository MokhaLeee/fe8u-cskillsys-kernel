from app.engine.combat.solver import CombatPhaseSolver

from app.engine import skill_system, item_system, action
from app.engine.combat.simple_combat import SimpleCombat

from app.engine.objects.unit import UnitObject
from app.engine.objects.item import ItemObject

from app.utilities import static_random
from app.engine.combat.utils import resolve_weapon

class BaseCombat(SimpleCombat):
    alerts: bool = True
    """
    Handles in base and in prep screen "use" of items
    """

    def __init__(self, attacker: UnitObject, main_item: ItemObject,
                 main_target: UnitObject, script: list, total_rounds: int = 1):
        self._counter: int = 0
        
        self.attacker = attacker
        self.defender = main_target
        self.main_item = main_item
        self.def_item = None
        if self.defender:
            self.def_item = resolve_weapon(self.defender)

        self.state_machine = CombatPhaseSolver(
            self.attacker, self.main_item, [self.main_item],
            [self.defender], [[]], [self.defender.position],
            self.defender, self.def_item, script, total_rounds)

        self.full_playback = []
        self.playback = []
        self.actions = []

        self.start_combat()
        self.start_event()

    def start_combat(self):
        self.initial_random_state = static_random.get_combat_random_state()

        skill_system.pre_combat(self.full_playback, self.attacker, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')
        if self.attacker is not self.defender:
            skill_system.pre_combat(self.full_playback, self.defender, self.def_item, self.attacker, self.main_item, 'defense')

        skill_system.start_combat(self.full_playback, self.attacker, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')
        item_system.start_combat(self.full_playback, self.attacker, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')

        if self.attacker is not self.defender:
            skill_system.start_combat(self.full_playback, self.defender, self.def_item, self.attacker, self.main_item, 'defense')
            if self.def_item:
                item_system.start_combat(self.full_playback, self.defender, self.def_item, self.attacker, self.main_item, 'defense')

    def cleanup_combat(self):
        skill_system.cleanup_combat(self.full_playback, self.attacker, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')
        if self.attacker is not self.defender:
            skill_system.cleanup_combat(self.full_playback, self.defender, self.def_item, self.attacker, self.main_item, 'defense')

    def end_combat(self):
        skill_system.end_combat(self.full_playback, self.attacker, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')
        item_system.end_combat(self.full_playback, self.attacker, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')
        if self.attacker.strike_partner:
            skill_system.end_combat(self.full_playback, self.attacker.strike_partner, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')
            item_system.end_combat(self.full_playback, self.attacker.strike_partner, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')
            self.attacker.strike_partner = None
        if self.defender.strike_partner:
            skill_system.end_combat(self.full_playback, self.defender.strike_partner, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')
            item_system.end_combat(self.full_playback, self.defender.strike_partner, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')
            self.attacker.strike_partner = None
        if self.attacker is not self.defender:
            skill_system.end_combat(self.full_playback, self.defender, self.def_item, self.attacker, self.main_item, 'defense')
            if self.def_item:
                item_system.end_combat(self.full_playback, self.defender, self.def_item, self.attacker, self.main_item, 'defense')

        skill_system.deactivate_all_combat_arts(self.attacker)

        skill_system.post_combat(self.full_playback, self.attacker, self.main_item, self.defender, resolve_weapon(self.defender), 'attack')
        if self.attacker is not self.defender:
            skill_system.post_combat(self.full_playback, self.defender, self.def_item, self.attacker, self.main_item, 'defense')

        self.final_random_state = static_random.get_combat_random_state()
        action.do(action.RecordRandomState(self.initial_random_state, self.final_random_state))

    def _all_units(self) -> list:
        """
        Returns list of all units taking in this combat
        """
        all_units = [self.attacker]
        if self.attacker is not self.defender:
            all_units.append(self.defender)
        return all_units

    def update(self):
        if self._counter == 0:
            while self.state_machine.get_state():
                self.actions, self.playback = self.state_machine.do()
                self.full_playback += self.playback
                self._apply_actions()
                self.state_machine.setup_next_state()
            self._counter += 1
            return False
        else:
            self.clean_up()
            return True

    def handle_state_stack(self):
        pass
