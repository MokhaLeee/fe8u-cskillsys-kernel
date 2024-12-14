import unittest

from app.events.event_commands import GiveItem

class EventInspectorTests(unittest.TestCase):
    def setUp(self):
        from app.data.database.database import Database
        self.db = Database()
        self.db.load('testing_proj.ltproj')
        self.event_inspector = self.db.events.inspector

    def tearDown(self) -> None:
        pass

    def test_unit_dump(self):
        give_item_events = self.event_inspector.find_all_calls_of_command(GiveItem())
        self.assertEqual(len(give_item_events), 1)
        self.assertEqual(list(give_item_events.values())[0].to_plain_text(), 'give_item;101;Hand Axe')

if __name__ == '__main__':
    unittest.main()
