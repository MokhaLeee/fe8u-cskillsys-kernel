import logging
import unittest
from unittest.mock import MagicMock

from app.engine.text_evaluator import TextEvaluator
from app.tests.mocks.mock_game import get_mock_game


class TextEvaluatorTests(unittest.TestCase):
    def setUp(self):
        self.mock_game = get_mock_game()
        mock_unit = MagicMock()
        mock_unit.name = "Erika" # deliberate spelling
        get_unit = lambda name: mock_unit
        self.mock_game.get_unit = get_unit
        self.mock_game.level_vars = {"TimesRescued": 10}

        self.text_evaluator = TextEvaluator(logging.getLogger(), self.mock_game)
        logging.disable(logging.CRITICAL)

    def tearDown(self):
        logging.disable(logging.NOTSET)

    def testBasicEval(self):
        text = "lorem ipsum {e:1+2}"
        evaled = self.text_evaluator._evaluate_all(text)
        self.assertEqual(evaled, "lorem ipsum 3")

    def testNestedEval(self):
        text = "{e:1}+{e:1+{e:1}}"
        evaled = self.text_evaluator._evaluate_all(text)
        self.assertEqual(evaled, "1+2")