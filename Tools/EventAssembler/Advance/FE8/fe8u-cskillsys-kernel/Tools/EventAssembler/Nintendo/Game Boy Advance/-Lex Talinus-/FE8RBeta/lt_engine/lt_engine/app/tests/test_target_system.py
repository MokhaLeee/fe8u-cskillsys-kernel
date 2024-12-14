import unittest
from unittest.mock import MagicMock, Mock

import time

from app.engine.item_components.target_components import EvalSpecialRange, MaximumRange, MinimumRange, TargetsEnemies
from app.engine.objects.item import ItemObject
from app.engine.game_board import GameBoard

from app.engine.target_system import TargetSystem
from app.tests.mocks.mock_game import get_mock_game
from app.engine.objects.unit import UnitObject

class TargetSystemUnitTests(unittest.TestCase):
    def setUp(self):
        from app.data.database.database import DB
        DB.load('testing_proj.ltproj')
        self.game = get_mock_game()
        self.target_system = self.create_target_system()

        tilemap = MagicMock()
        tilemap.width = 29
        tilemap.height = 29
        self.game.board = GameBoard(tilemap)
        self.game.board.bounds = (0, 0, 28, 28)
        self.game.units = []

        self.player_unit = UnitObject('player')         
        self.player_unit.team = 'player'

        self.ally_unit = UnitObject('ally')
        self.ally_unit.team = 'player'

        self.enemy_unit = UnitObject('enemy')
        self.enemy_unit.team = 'enemy'

    def mock_weapon(self, target_pos, min_range, max_range):
        sword = ItemObject('weapon', 'weapon', 'a test weapon')
        targeting_component = TargetsEnemies()
        if target_pos:
            targeting_component.valid_targets = Mock(return_value=set([target_pos]))
        else:
            targeting_component.valid_targets = Mock(return_value=set())
        min_range_component = MinimumRange(value=min_range)
        max_range_component = MaximumRange(value=max_range)
        sword.components = [targeting_component, min_range_component, max_range_component]
        return sword

    def create_target_system(self):
        return TargetSystem(game=self.game)

    def test_get_shell(self):
        valid_moves = {(0, 0)}
        potential_range = {5}
        bounds = (0, 0, 10, 10)
        # Used optionally
        manhattan_restriction = {(x, y) for x in range(10) for y in range(10) if x == 0 or y == 0}

        # Test basic shell
        valid_positions = self.target_system.get_shell(valid_moves, potential_range, bounds)
        self.assertNotIn((0, 0), valid_positions)
        self.assertIn((0, 5), valid_positions)
        self.assertIn((5, 0), valid_positions)
        self.assertIn((4, 1), valid_positions)
        self.assertNotIn((-5, 0), valid_positions)

        # Test mnahattan restriction
        valid_positions = self.target_system.get_shell(valid_moves, potential_range, bounds, manhattan_restriction)
        self.assertNotIn((0, 0), valid_positions)
        self.assertIn((0, 5), valid_positions)
        self.assertIn((5, 0), valid_positions)
        self.assertNotIn((4, 1), valid_positions)
        self.assertNotIn((-5, 0), valid_positions)

    def test_get_possible_attack_positions(self):
        self.player_unit.position = (0, 0)
        self.enemy_unit.position = (0, 1)
        self.game.units.append(self.player_unit)
        self.game.units.append(self.enemy_unit)

        valid_moves = [(0, 2), (1, 1), (0, 1), (0, 0)]

        weapon = self.mock_weapon(self.enemy_unit.position, 1, 1)

        expected_positions = {(0, 2), (0, 0), (1, 1)}
        possible_positions = self.target_system.get_possible_attack_positions(self.player_unit, (0, 1), valid_moves, weapon)
        self.assertEqual(set(possible_positions), expected_positions)

    def test_get_possible_attack_positions_range_restrict(self):
        self.player_unit.position = (0, 0)
        self.enemy_unit.position = (0, 1)
        self.game.units.append(self.player_unit)
        self.game.units.append(self.enemy_unit)

        valid_moves = [(0, 2), (1, 1), (0, 1), (0, 0)]

        weapon = self.mock_weapon(self.enemy_unit.position, 1, 1)
        eval_special_range = EvalSpecialRange(value='y > 0')
        weapon.components.append(eval_special_range)

        expected_positions = [(0, 0)]
        possible_positions = self.target_system.get_possible_attack_positions(self.player_unit, (0, 1), valid_moves, weapon)
        self.assertEqual(possible_positions, expected_positions)

    def test_get_attackable_positions(self):
        self.player_unit.position = (1, 1)
        weapon = self.mock_weapon(None, 1, 1)

        valid_positions = self.target_system.get_attackable_positions(self.player_unit, weapon)
        expected_positions = {(1, 0), (0, 1), (2, 1), (1, 2)}
        self.assertEqual(valid_positions, expected_positions)

        # Test with bounds
        self.game.board.bounds = (1, 1, 27, 27)
        valid_positions = self.target_system.get_attackable_positions(self.player_unit, weapon)
        expected_positions = {(2, 1), (1, 2)}
        self.assertEqual(valid_positions, expected_positions)

    def test_get_attackable_positions_global_range(self):
        self.player_unit.position = (1, 1)
        weapon = self.mock_weapon(None, 1, 99)

        valid_positions = self.target_system.get_attackable_positions(self.player_unit, weapon)

        num_positions = (self.game.board.bounds[2] - self.game.board.bounds[0] + 1) * (self.game.board.bounds[3] - self.game.board.bounds[1] + 1)
        self.assertEqual(len(valid_positions), num_positions)

    def test_targets_in_range_adjacent(self):
        """
        An adjacent enemy should be a valid target
        """
        self.player_unit.position = (0, 0)
        self.enemy_unit.position = (0, 1)
        self.game.units.append(self.player_unit)
        self.game.units.append(self.enemy_unit)

        weapon = self.mock_weapon(self.enemy_unit.position, 1, 1)
        
        targets = self.target_system.targets_in_range(self.player_unit, weapon)
        self.assertEqual(len(targets), 1)
        self.assertIn((0, 1), targets)

    def test_targets_in_range_adjacent_ally(self):
        """
        An adjacent ally is not a valid target to attack
        """
        self.player_unit.position = (0, 0)
        self.ally_unit.position = (0, 1)
        self.game.units.append(self.player_unit)
        self.game.units.append(self.ally_unit)

        weapon = self.mock_weapon(None, 1, 1)

        targets = self.target_system.targets_in_range(self.player_unit, weapon)
        self.assertEqual(len(targets), 0)

    def test_targets_in_range_too_far(self):
        """
        This enemy is too far from the player's position
        """
        self.player_unit.position = (0, 0)
        self.enemy_unit.position = (0, 3)
        self.game.units.append(self.player_unit)
        self.game.units.append(self.enemy_unit)

        weapon = self.mock_weapon(self.enemy_unit.position, 1, 1)

        targets = self.target_system.targets_in_range(self.player_unit, weapon)
        self.assertEqual(len(targets), 0)

if __name__ == '__main__':
    unittest.main()
