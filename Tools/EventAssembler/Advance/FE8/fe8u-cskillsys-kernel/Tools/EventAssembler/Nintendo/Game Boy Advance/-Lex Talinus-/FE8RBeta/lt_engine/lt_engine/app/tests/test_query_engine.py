import logging
import unittest
from unittest.mock import MagicMock, patch, call

import app.editor.lib.csv.csv_data_exporter as csv
from app.engine.query_engine import GameQueryEngine
from app.tests.mocks.mock_game import get_mock_game
from app.tests.mocks.mock_object import Object

class QueryEngineTests(unittest.TestCase):
    def setUp(self):
        self.game = get_mock_game()
        self.query_engine = GameQueryEngine(logging.Logger("logger"), self.game)

    def tearDown(self) -> None:
        pass

    def test_resolve_to_unit(self):
        eirika_obj = Object(nid='Eirika', position=(2, 4))

        def mock_get_unit_return(value):
            if value == 'Eirika':
                return eirika_obj
            raise ValueError()
        self.game.get_unit = MagicMock(side_effect=mock_get_unit_return)

        res = self.query_engine._resolve_to_unit('Eirika')
        self.assertEqual(res.position, eirika_obj.position)

        res = self.query_engine._resolve_to_unit(eirika_obj)
        self.assertEqual(res.position, eirika_obj.position)

    def test_resolve_pos(self):
        TEST_POS = (2, 4)
        eirika_obj = Object(nid='Eirika', position=TEST_POS)
        def mock_get_unit_return(value):
            if value == 'Eirika':
                return eirika_obj
            raise ValueError()
        self.game.get_unit = MagicMock(side_effect=mock_get_unit_return)

        res = self.query_engine._resolve_pos((2, 4))
        self.assertEqual(res, TEST_POS)

        res = self.query_engine._resolve_pos('Eirika')
        self.assertEqual(res, TEST_POS)

        res = self.query_engine._resolve_pos(eirika_obj)
        self.assertEqual(res, TEST_POS)

if __name__ == '__main__':
    unittest.main()
