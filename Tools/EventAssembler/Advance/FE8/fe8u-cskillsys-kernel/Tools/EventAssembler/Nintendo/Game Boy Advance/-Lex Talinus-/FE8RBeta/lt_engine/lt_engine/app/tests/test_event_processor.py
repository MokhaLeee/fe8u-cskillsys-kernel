import logging
import os
import unittest
from pathlib import Path
from unittest.mock import MagicMock

from app.engine.text_evaluator import TextEvaluator
from app.events import event_commands
from app.events.event_processor import EventProcessor, IteratorInfo
from app.events.triggers import GenericTrigger
from app.tests.mocks.mock_game import get_mock_game


class EventProcessorUnitTests(unittest.TestCase):
    def setUp(self):
        from app.data.resources.resources import RESOURCES
        RESOURCES.load('testing_proj.ltproj')
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

    def test_conditional_handling(self):
        script_path = Path(__file__).parent / 'data' / 'processor' / 'conditionals.event'
        processor = EventProcessor('conditionals', script_path.read_text(), self.text_evaluator)

        # basic tests
        self.assertEqual(processor._find_end(1), 6)
        self.assertEqual(processor._jump_conditional(1), 2)
        self.assertEqual(processor._find_end(3), 6)

        # elif
        self.assertEqual(processor._jump_conditional(8), 12)
        self.assertEqual(processor._jump_conditional(9), 10)
        self.assertEqual(processor._jump_conditional(11), 12)
        self.assertEqual(processor._find_end(11), 15)

        # nested for
        self.assertEqual(processor._jump_conditional(18), 19)
        self.assertEqual(processor._find_end(19), 21)

    def test_event_processor_handles_conditionals(self):
        script_path = Path(__file__).parent / 'data' / 'processor' / 'second_conditional.event'
        processor = EventProcessor('conditionals', script_path.read_text(), self.text_evaluator)

        called_commands = []
        while not processor.finished():
            next_command = processor.fetch_next_command()
            if next_command:
                called_commands.append(next_command)
        self.assertEqual(len(called_commands), 1)
        self.assertTrue(isinstance(called_commands[0], event_commands.Speak))
        self.assertEqual(called_commands[0].parameters['Text'], '2')

    def test_parse_events(self):
        script_path = Path(__file__).parent / 'data' / 'processor' / 'test.event'
        processor = EventProcessor('test', script_path.read_text(), self.text_evaluator)

        # normal command
        mu_speak = processor.fetch_next_command()
        self.assertTrue(isinstance(mu_speak, event_commands.Speak))
        self.assertEqual(mu_speak.parameters['SpeakerOrStyle'], 'MU')
        self.assertEqual(mu_speak.parameters['Text'], 'I am a custom named character.')

        # eval
        seth_speak = processor.fetch_next_command()
        self.assertTrue(isinstance(seth_speak, event_commands.Speak))
        self.assertEqual(seth_speak.parameters['SpeakerOrStyle'], 'Seth')
        self.assertEqual(seth_speak.parameters['Text'], 'Princess Erika!')

        # variable
        rescue_speak = processor.fetch_next_command()
        self.assertTrue(isinstance(rescue_speak, event_commands.Speak))
        self.assertEqual(rescue_speak.parameters['SpeakerOrStyle'], 'MU')
        self.assertEqual(rescue_speak.parameters['Text'], "You've rescued me 10 times.")

        # if/else/processing
        iftrue_speak = processor.fetch_next_command()
        self.assertTrue(isinstance(iftrue_speak, event_commands.Speak))
        self.assertEqual(iftrue_speak.parameters['SpeakerOrStyle'], 'MU')
        self.assertEqual(iftrue_speak.parameters['Text'], "A bit ridiculous, isn't it?")

        # for
        eirika_for = processor.fetch_next_command()
        seth_for = processor.fetch_next_command()
        self.assertTrue(isinstance(eirika_for, event_commands.Speak))
        self.assertEqual(eirika_for.parameters['SpeakerOrStyle'], 'Eirika')
        self.assertEqual(eirika_for.parameters['Text'], "My name is Eirika.")
        self.assertTrue(isinstance(seth_for, event_commands.Speak))
        self.assertEqual(seth_for.parameters['SpeakerOrStyle'], 'Seth')
        self.assertEqual(seth_for.parameters['Text'], "My name is Seth.")

    def test_save_restore_iterators(self):
        TEST_NID = 'some_nid'
        TEST_LINE_NUM = 25
        TEST_ITEMS = ['a', 'b', 'c']
        iterator = IteratorInfo(TEST_NID, TEST_LINE_NUM, TEST_ITEMS)
        # advance twice
        iterator.iterator.next()
        iterator.iterator.next()
        self.assertEqual(iterator.iterator.get(), 'c')
        new_iterator = IteratorInfo.restore(iterator.save())
        self.assertEqual(new_iterator.nid, TEST_NID)
        self.assertEqual(new_iterator.line, TEST_LINE_NUM)
        self.assertEqual(new_iterator.iterator.items, TEST_ITEMS)
        self.assertEqual(new_iterator.iterator.get(), 'c')

    def test_save_restore_processor_state(self):
        script_path = Path(__file__).parent / 'data' / 'processor' / 'test.event'
        processor = EventProcessor('test', script_path.read_text(), self.text_evaluator)

        processor.fetch_next_command()
        processor.fetch_next_command()
        processor.fetch_next_command()
        processor.fetch_next_command()
        # processor just returned "speak;MU;A bit ridiculous, isn't it?"

        new_processor = EventProcessor.restore(processor.save(), self.text_evaluator)
        self.assertEqual(new_processor.nid, 'test')
        next_command = new_processor.fetch_next_command()
        self.assertTrue(isinstance(next_command, event_commands.Speak))
        self.assertEqual(next_command.parameters['SpeakerOrStyle'], 'Eirika')
        self.assertEqual(next_command.parameters['Text'], "My name is Eirika.")