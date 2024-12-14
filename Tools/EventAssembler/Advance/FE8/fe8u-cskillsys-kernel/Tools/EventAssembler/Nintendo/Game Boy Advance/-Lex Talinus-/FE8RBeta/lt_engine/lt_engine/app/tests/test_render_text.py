import unittest
import logging

class RenderTextUnitTests(unittest.TestCase):
    def setUp(self):
        # Patch out logging below CRITICAL DAMAGE
        logging.disable(logging.CRITICAL)

    def tearDown(self):
        # re-enable logging
        logging.disable(logging.NOTSET)

    def test_fix_tags(self):
        from app.engine.graphics.text.text_renderer import fix_tags

        test1 = ["You must push the <red>RED", "button</> or else you will die!"]
        expected_text = ["You must push the <red>RED</>", "<red>button</> or else you will die!"]
        self.assertEqual(fix_tags(test1), expected_text)

        test2 = ["<yellow>Tough Ring"]
        expected_text = ["<yellow>Tough Ring</>"]
        self.assertEqual(fix_tags(test2), expected_text)

    def test_remove_tags(self):
        from app.engine.graphics.text.text_renderer import remove_tags

        test1 = ["You must push the <red>RED", "button</> or else you will die!"]
        expected_text = ["You must push the RED", "button or else you will die!"]
        self.assertEqual(remove_tags(test1), expected_text)

        test2 = ["<yellow>Tough Ring"]
        expected_text = ["Tough Ring"]
        self.assertEqual(remove_tags(test2), expected_text)
