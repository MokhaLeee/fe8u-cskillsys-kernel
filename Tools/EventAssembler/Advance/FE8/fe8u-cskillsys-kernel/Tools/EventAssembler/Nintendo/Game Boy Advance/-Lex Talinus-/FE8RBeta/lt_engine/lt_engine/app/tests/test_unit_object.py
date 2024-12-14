import unittest
from unittest.mock import MagicMock, patch, call

from app.engine.objects.unit import UnitObject
from app.tests.mocks.mock_game import get_mock_game

class UnitObjectUnitTests(unittest.TestCase):
    def setUp(self):
        from app.data.database.database import DB
        DB.load('testing_proj.ltproj')
        from app.engine import equations
        equations.clear()
        self.game = get_mock_game()
        self.db_unit = UnitObject.from_prefab(DB.units.get("Eirika"))                # eirika, prefab
        # self.level_unit = UnitObject.from_prefab(DB.levels[0].units.get("Eirika"))   # eirika, level unit
        # self.generic_unit = UnitObject.from_prefab(DB.levels[0].units.get("101"))    # warrior, generic level unit

    def test_db_unit_constructor(self):
        unit = self.db_unit
        from app.data.database.database import DB
        from app.data.database.units import UnitPrefab
        self.assertEqual(unit.nid, "Eirika")
        self.assertEqual(unit.generic, False)
        self.assertEqual(unit.persistent, True)
        self.assertEqual(unit.ai, None)
        self.assertEqual(unit.ai_group, None)
        self.assertEqual(unit.faction, None)
        self.assertEqual(unit.team, "player")
        self.assertEqual(unit.portrait_nid, "Eirika")
        self.assertEqual(unit.affinity, "Light")
        self.assertEqual(unit.notes, [])
        self.assertEqual(unit._fields, {})
        self.assertEqual(unit.klass, "Eirika_Lord")
        self.assertEqual(unit.variant, None)

        self.assertEqual(unit.name, "Eirika")
        self.assertEqual(unit.desc, "The princess of the Kingdom of\nRenais. She's elegant and kind.")
        self.assertEqual(unit.tags, set({'Lord', 'Sword'}))
        self.assertEqual(unit.party, None)
        self.assertEqual(unit.level, 1)
        self.assertEqual(unit.exp, 0)
        self.assertEqual(unit.stats, {
            "HP": 16,
            "STR": 4,
            "MAG": 2,
            "SKL": 8,
            "SPD": 9,
            "LCK": 5,
            "DEF": 3,
            "CON": 5,
            "MOV": 5,
            "RES": 1
        })
        self.assertEqual(unit.growths, {
             "HP": 70,
            "STR": 40,
            "MAG": 20,
            "SKL": 60,
            "SPD": 60,
            "LCK": 60,
            "DEF": 30,
            "CON": 0,
            "MOV": 0,
            "RES": 30
        })

    def test_load_from_save(self):
        unit_data = {'_fields': {},
                    'action_state': (False, False, False, False, False, False, False, False),
                    'affinity': 'Light',
                    'ai': 'None',
                    'roam_ai': 'None',
                    'ai_group': None,
                    'ai_group_active': False,
                    'built_guard': False,
                    'current_fatigue': 0,
                    'current_guard_gauge': 0,
                    'current_hp': 16,
                    'current_mana': 20,
                    'dead': False,
                    'desc': "The princess of the Kingdom of\nRenais. She's elegant and kind.",
                    'exp': 0,
                    'faction': None,
                    'generic': False,
                    'growth_points': {'CON': 0,
                                    'DEF': 0,
                                    'HP': 0,
                                    'LCK': 0,
                                    'MAG': 0,
                                    'MOV': 0,
                                    'RES': 0,
                                    'SKL': 0,
                                    'SPD': 0,
                                    'STR': 0},
                    'growths': {'CON': 0,
                                'DEF': 30,
                                'HP': 70,
                                'LCK': 60,
                                'MAG': 20,
                                'MOV': 0,
                                'RES': 30,
                                'SKL': 60,
                                'SPD': 60,
                                'STR': 40},
                    'items': [100, 108],
                    'klass': 'Eirika_Lord',
                    'level': 1,
                    'name': 'Eirika',
                    'nid': 'Eirika',
                    'notes': [],
                    'party': 'Eirika',
                    'persistent': True,
                    'portrait_nid': 'Eirika',
                    'position': None,
                    'skills': [(110, 'bob', ('klass', False, True)), (120, (1,1), ('terrain', False, False))],
                    'starting_position': (4, 5),
                    'stats': {'CON': 5,
                            'DEF': 3,
                            'HP': 16,
                            'LCK': 5,
                            'MAG': 2,
                            'MOV': 5,
                            'RES': 1,
                            'SKL': 8,
                            'SPD': 9,
                            'STR': 4},
                    'tags': ['Lord'],
                    'team': 'player',
                    'traveler': None,
                    'variant': None,
                    'wexp': {'Anima': 0,
                            'Axe': 0,
                            'Bow': 0,
                            'Dark': 0,
                            'Default': 0,
                            'Lance': 0,
                            'Light': 0,
                            'Staff': 0,
                            'Sword': 1}}
        unit: UnitObject = UnitObject.restore(unit_data, self.game)

        self.assertEqual(unit.nid, "Eirika")
        self.assertEqual(unit.generic, False)
        self.assertEqual(unit.persistent, True)
        self.assertEqual(unit.ai, 'None')
        self.assertEqual(unit.roam_ai, 'None')
        self.assertEqual(unit.ai_group, None)
        self.assertEqual(unit.faction, None)
        self.assertEqual(unit.team, "player")
        self.assertEqual(unit.portrait_nid, "Eirika")
        self.assertEqual(unit.affinity, "Light")
        self.assertEqual(unit.notes, [])
        self.assertEqual(unit._fields, {})
        self.assertEqual(unit.klass, "Eirika_Lord")
        self.assertEqual(unit.variant, None)

        self.assertEqual(unit.name, "Eirika")
        self.assertEqual(unit.desc, "The princess of the Kingdom of\nRenais. She's elegant and kind.")
        self.assertEqual(unit.tags, set({'Lord', 'Sword'}))
        self.assertEqual(unit.party, 'Eirika')
        self.assertEqual(unit.level, 1)
        self.assertEqual(unit.exp, 0)
        self.assertEqual(unit.stats, {
            "HP": 16,
            "STR": 4,
            "MAG": 2,
            "SKL": 8,
            "SPD": 9,
            "LCK": 5,
            "DEF": 3,
            "CON": 5,
            "MOV": 5,
            "RES": 1
        })
        self.assertEqual(unit.growths, {
             "HP": 70,
            "STR": 40,
            "MAG": 20,
            "SKL": 60,
            "SPD": 60,
            "LCK": 60,
            "DEF": 30,
            "CON": 0,
            "MOV": 0,
            "RES": 30
        })

        item_calls = [call(100), call(108)]
        self.game.get_item.assert_has_calls(item_calls)
        skill_calls = [call(110), call(120)]
        self.game.get_skill.assert_has_calls(skill_calls)

if __name__ == '__main__':
    unittest.main()
