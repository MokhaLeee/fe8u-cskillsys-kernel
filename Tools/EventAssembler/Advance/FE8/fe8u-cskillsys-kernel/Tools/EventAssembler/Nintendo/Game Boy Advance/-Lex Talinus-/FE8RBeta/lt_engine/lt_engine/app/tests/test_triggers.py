import dataclasses
import unittest

from app.events.triggers import ALL_TRIGGERS, RegionTrigger
from app.tests.mocks.mock_game import get_mock_game

class TriggerTests(unittest.TestCase):
    def testTriggersAreDocumented(self):
        documented_triggers = ALL_TRIGGERS.copy()
        documented_triggers.append(RegionTrigger)
        for trigger in documented_triggers:
            doc = trigger.__doc__
            self.assertFalse(doc.startswith(trigger.__name__ + "("), "EventTrigger %s does not have documentation in triggers.py" % trigger.__name__)
            for field in dataclasses.fields(trigger):
                name = field.name
                self.assertTrue(name in doc, "Doc for field %s missing from docstring for EventTrigger %s" % (name, trigger.__name__))
