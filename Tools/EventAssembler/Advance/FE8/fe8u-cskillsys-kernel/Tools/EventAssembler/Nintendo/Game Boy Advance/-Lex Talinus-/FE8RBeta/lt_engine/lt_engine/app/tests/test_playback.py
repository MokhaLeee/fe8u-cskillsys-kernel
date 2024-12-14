import unittest
import logging

class PlaybackUnitTests(unittest.TestCase):
    def setUp(self):
        # Patch out logging below CRITICAL DAMAGE
        logging.disable(logging.CRITICAL)

    def tearDown(self):
        # re-enable logging
        logging.disable(logging.NOTSET)

    def test_check_playback_names(self):
        from app.utilities import str_utils
        from app.engine.combat import playback

        playbacks = playback.PlaybackBrush.__subclasses__()
        for brush in playbacks:
            # Make sure the playback's class name and nid follow conventions
            class_name = str_utils.camel_to_snake(brush.__name__)
            self.assertEqual(class_name, brush.nid)
