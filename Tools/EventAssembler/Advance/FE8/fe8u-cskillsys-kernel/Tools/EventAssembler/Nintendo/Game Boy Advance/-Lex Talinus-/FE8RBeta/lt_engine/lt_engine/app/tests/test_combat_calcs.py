from dataclasses import dataclass
from typing import Tuple
from app.utilities import utils
import unittest
from unittest.mock import MagicMock, Mock, patch, call
from app.engine.combat_calcs import can_counterattack


@dataclass
class MockUnit():
    position: Tuple[int, int] = (0, 0)
    distant_counter = False
    close_counter = False
    can_be_seen = True

@dataclass
class MockItem():
    available = True
    min_range = 1
    max_range = 1
    ignore_los = False
    counterable = True
    can_counter = True

def _available(_,  item: MockItem):
    return item.available

def _counterable(_, item: MockItem):
    return item.counterable

def _can_counter(_, item: MockItem):
    return item.can_counter

def _distant_counter(unit: MockUnit):
    return unit.distant_counter

def _close_counter(unit: MockUnit):
    return unit.close_counter

def _ignore_los(_, item: MockItem):
    return item.ignore_los


class CombatCalcTests(unittest.TestCase):
    def setUp(self):
        from app.data.database.database import DB
        DB.load('testing_proj.ltproj')
        DB.constants.get('line_of_sight').set_value(False)
        self.attacker = MockUnit(position=(0, 0))
        self.defender = MockUnit(position=(0, 1))
        self.aweapon = MockItem()
        self.dweapon = MockItem()

        self.patchers = [
            patch('app.engine.item_funcs.available', _available),
            patch('app.engine.item_system.can_be_countered', _counterable),
            patch('app.engine.item_system.can_counter', _can_counter),
            patch('app.engine.item_system.ignore_line_of_sight', _ignore_los),
            patch('app.engine.skill_system.distant_counter', _distant_counter),
            patch('app.engine.skill_system.close_counter', _close_counter),
            patch('app.engine.line_of_sight.line_of_sight', self.does_defender_have_los_on_attacker),
        ]
        for patcher in self.patchers:
            patcher.start()

    def reset_state(self):
        self.attacker = MockUnit(position=(0, 0))
        self.defender = MockUnit(position=(0, 1))
        self.aweapon = MockItem()
        self.dweapon = MockItem()
    
    def is_attacker_in_range(self, a: MockUnit, b: MockItem):
        dist = utils.calculate_distance(self.attacker.position, self.defender.position)
        if dist >= self.dweapon.min_range and dist <= self.dweapon.max_range:
            return [self.attacker.position]
        return []
    
    def does_defender_have_los_on_attacker(self, a, b, c):
        if self.attacker.can_be_seen:
            return [self.attacker.position]
        return []

    def tearDown(self):
        for patcher in self.patchers:
            patcher.stop()

    def check_counter(self, should_counter, msg):
        from app.engine.game_state import game
        game.target_system = MagicMock()
        game.target_system.targets_in_range = self.is_attacker_in_range
        self.assertEqual(can_counterattack(self.attacker, self.aweapon, self.defender, self.dweapon), should_counter, msg)
    
    """
    Tests begin here
    """
    def test_counter_logic(self):
        from app.data.database.database import DB
        self.check_counter(True, "Cannot counter even though both at one range")
        self.attacker.position = (0, -1)
        self.check_counter(False, "Incorrect counter at two distance")
        self.defender.distant_counter = True
        self.check_counter(True, "Cannot counter even though defender has distant counter")
        self.aweapon.counterable = False
        self.check_counter(False, "Incorrect counter against uncounterable weapon")
        self.aweapon.counterable = True
        self.dweapon.can_counter = False
        self.check_counter(False, "Incorrect counter with weapon that cannot counter")

        self.reset_state()
        self.dweapon.min_range = 2
        self.dweapon.max_range = 2
        self.attacker.position = (0, -1)
        self.check_counter(True, "Should counter at two range with two range weapon")
        self.attacker.position = (0, 0)
        self.check_counter(False, "Should not counter at one range with two range weapon")
        self.dweapon.min_range = 1
        self.check_counter(True, "Should counter at one range with 1-2 range weapon")
        self.dweapon.min_range = 2
        self.defender.close_counter = True
        self.check_counter(True, "Should counter at two range with close counter")
        self.attacker.position = (0, -2)
        self.check_counter(False, "Should not counter at three range")

        self.reset_state()
        self.dweapon = None
        self.check_counter(False, "Should not counter with no weapon")
        self.dweapon = MockItem()
        self.dweapon.available = False
        self.check_counter(False, "Should not counter with unavailable weapon")

        self.reset_state()
        DB.constants.get("line_of_sight").set_value(True)
        self.check_counter(True, "Should counter with LOS")
        self.attacker.can_be_seen = False
        self.check_counter(False, "Should not counter without LOS")
        DB.constants.get("line_of_sight").set_value(False)
        self.check_counter(True, "Should counter if LOS is disabled, even if no LOS")