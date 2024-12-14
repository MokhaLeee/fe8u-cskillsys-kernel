import unittest
from unittest.mock import MagicMock, patch
import app.engine.dialog

from app.events.speak_style import SpeakStyle, SpeakStyleLibrary

class CsvExporterTests(unittest.TestCase):
    def setUp(self):
        from app.data.resources.resources import RESOURCES
        from app.engine import fonts
        RESOURCES.load('testing_proj.ltproj')
        fonts.load_fonts()
        self.db = SpeakStyleLibrary()

    def tearDown(self) -> None:
        pass

    def test_speak_style_matches_dialog_args(self):
        with patch('app.engine.dialog.engine', new=MagicMock()):
            with patch('app.engine.dialog.create_base_surf', new=lambda *args: None):
                full_style = SpeakStyle('a', 'b', (1, 2), 25, 5.0, 'blue', 'text', 'some_bg', 3, True, 'tail', 2.5, 'None', set('FLAG1'))
                # should not throw
                app.engine.dialog.Dialog.from_style(full_style, 'testing text', None, 11)

if __name__ == '__main__':
    unittest.main()
