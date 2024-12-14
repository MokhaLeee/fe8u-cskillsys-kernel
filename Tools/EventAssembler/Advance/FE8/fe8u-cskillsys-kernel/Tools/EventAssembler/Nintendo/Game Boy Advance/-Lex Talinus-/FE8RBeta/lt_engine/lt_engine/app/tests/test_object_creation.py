import unittest

from app.utilities.data import Data
from app.data.database.database import DB

class ObjectCreationUnitTests(unittest.TestCase):
    """
    # Tests that the databases correctly add the object to the database
    # Does not currently test creation of:
    # - equations
    # - mcost rows/columns
    # - tags
    # - weapon ranks
    # - game var slots
    # - support ranks
    # - support pairs
    # - affinities
    # - raw data
    # - levels
    # - translations
    # Constants, Support Constants, and Minimap data don't have new information added to them by the user
    """
    def setUp(self):
        DB.load('testing_proj.ltproj')

    def _test_create_new(self, data: Data):
        """
        # Generic test used by the rest of the testing functions below.
        # Counts number of elements before and after and makes sure we've increased by one.
        # Also makes sure the command itself just works.
        """
        number_of_elems = len(data)
        data.create_new(DB)
        # confirm we added an element
        self.assertEqual(number_of_elems + 1, len(data))

    def test_create_unit(self):
        data = DB.units
        self._test_create_new(data)

    def test_create_class(self):
        data = DB.classes
        self._test_create_new(data)

    def test_create_team(self):
        data = DB.teams
        self._test_create_new(data)

    def test_create_item(self):
        data = DB.items
        self._test_create_new(data)

    def test_create_skill(self):
        data = DB.skills
        self._test_create_new(data)

    def test_create_faction(self):
        data = DB.factions
        self._test_create_new(data)

    def test_create_party(self):
        data = DB.parties
        self._test_create_new(data)

    def test_create_weapon_type(self):
        data = DB.weapons
        self._test_create_new(data)

    def test_create_stat(self):
        data = DB.stats
        self._test_create_new(data)

    def test_create_lore(self):
        data = DB.lore
        self._test_create_new(data)

    def test_create_difficulty_mode(self):
        data = DB.difficulty_modes
        self._test_create_new(data)

    def test_create_ai(self):
        data = DB.ai
        self._test_create_new(data)

    def test_create_terrain(self):
        data = DB.terrain
        self._test_create_new(data)

    def test_create_event(self):
        data = DB.events
        self._test_create_new(data)
