import unittest
from unittest.mock import MagicMock, patch, PropertyMock

from app.engine.game_board import GameBoard
from app.engine.target_system import TargetSystem
from app.engine.objects.unit import UnitObject
from app.engine.pathfinding.path_system import PathSystem
from app.engine.pathfinding.node import Node

from app.utilities.grid import Grid

from app.tests.mocks.mock_game import get_mock_game

class PathSystemTests(unittest.TestCase):
    """
    Tests the path_system functions which help with running pathfinding
    """

    def setUp(self):
        from app.data.database.database import DB
        DB.load('testing_proj.ltproj')
        self.game = get_mock_game()
        self.game.target_system = TargetSystem(game=self.game)
        self.path_system = PathSystem(game=self.game)

        tilemap = MagicMock(name='tilemap')
        tilemap.width = 29
        tilemap.height = 29
        self.game.board = GameBoard(tilemap)
        self.game.board.bounds = (0, 0, 28, 28)
        self.game.units = []

        self.player_unit = UnitObject('player')
        self.player_unit.klass = 'Citizen'
        self.player_unit.team = 'player'
        self.player_unit.finished = False
        self.player_unit.position = (1, 1)
        self.player_unit.movement_left = 5

        # Set up a simple grid for use in travel algorithm tests
        width, height = 9, 9
        self.simple_grid = Grid((width, height))
        for x in range(width):
            for y in range(height):
                self.simple_grid.append(Node(x, y, True, 1))

    def test_valid_moves(self):
        with patch('app.engine.equations.parser.movement') as movement_mock, \
             patch('app.engine.objects.unit.UnitObject.movement_left', new_callable=PropertyMock) as movement_left_mock:
            movement_instance = movement_mock.return_value
            movement_instance.method.return_value = 5
            movement_left_mock.return_value = 5

            valid_moves = self.path_system.get_valid_moves(self.player_unit)
            self.assertGreater(len(valid_moves), 0, 'get_valid_moves did not return a valid move')
            self.assertIn((1, 1), valid_moves, 'current position is not valid_moves')

    def test_get_path(self):
        goal = (28, 14)

        # Yes, path is reversed throughout the whole engine
        # path[0] is the end of the unit's path
        # path[-1] is their starting point

        with patch('app.engine.equations.parser.movement') as movement_mock, \
             patch('app.engine.objects.unit.UnitObject.movement_left', new_callable=PropertyMock) as movement_left_mock:
            movement_instance = movement_mock.return_value
            movement_instance.method.return_value = 5
            movement_left_mock.return_value = 5
            
            # Test AStar
            path = self.path_system.get_path(self.player_unit, goal)
            self.assertGreater(len(path), 0, 'path is empty')
            self.assertEqual(path[0], goal, 'Did not find the end')
            self.assertEqual(path[-1], (1, 1), 'Did not start at the beginning')

            # Test Limit
            path = self.path_system.get_path(self.player_unit, goal, use_limit=True)
            self.assertEqual(len(path), 0, 'path is not empty')

            # Test ThetaStar
            path = self.path_system.get_path(self.player_unit, goal, free_movement=True)
            self.assertEqual(len(path), 2, 'path is not a perfect diagonal')
            self.assertEqual(path[0], goal, 'Did not find the end')
            self.assertEqual(path[-1], (1, 1), 'Did not start at the beginning')

    def test_check_path(self):
        with patch('app.engine.equations.parser.movement') as movement_mock, \
             patch('app.engine.movement.movement_funcs.get_mcost') as mcost_mock, \
             patch('app.engine.objects.unit.UnitObject.movement_left', new_callable=PropertyMock) as movement_left_mock:
            movement_instance = movement_mock.return_value
            movement_instance.method.return_value = 5
            mcost_mock.return_value = 1
            movement_left_mock.return_value = 5

            good_path = [(0, 0), (1, 0), (1, 1), (2, 1), (2, 2), (2, 3)]
            good_path_valid = self.path_system.check_path(self.player_unit, good_path)
            self.assertEqual(good_path_valid, True)

            # Bad adjacencies
            bad_path = [(0, 0), (1, 1), (2, 1), (2, 2)]
            bad_path_valid = self.path_system.check_path(self.player_unit, bad_path)
            self.assertEqual(bad_path_valid, False, 'Did not catch that not every position in path is adjacent')

            # Too long
            bad_path = [(0, 0), (1, 0), (2, 0), (3, 0), (3, 1), (3, 2), (3, 3), (3, 4)]
            bad_path_valid = self.path_system.check_path(self.player_unit, bad_path)
            self.assertEqual(bad_path_valid, False, 'A too long path was not caught')

    def test_travel_algorithm(self):
        path = [(0, 0), (1, 0), (1, 1), (2, 1), (2, 2), (2, 3)]
        final_pos = self.path_system.travel_algorithm(path, 3, self.player_unit, self.simple_grid)
        self.assertEqual(final_pos, (1, 1), 'Did not find correct final position')

        orig_position = self.player_unit.position
        final_pos = self.path_system.travel_algorithm([], 3, self.player_unit, self.simple_grid)
        self.assertEqual(final_pos, orig_position, 'Unit should always end up at original position')


if __name__ == '__main__':
    unittest.main()
