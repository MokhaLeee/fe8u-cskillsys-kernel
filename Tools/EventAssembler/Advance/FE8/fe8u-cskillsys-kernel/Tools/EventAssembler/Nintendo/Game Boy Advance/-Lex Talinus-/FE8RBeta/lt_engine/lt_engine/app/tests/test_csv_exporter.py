from typing import List
import unittest
from unittest.mock import MagicMock, patch, call
from app.data.database.items import ItemCatalog
from app.data.database.klass import ClassCatalog
from app.data.database.units import UnitCatalog

import app.editor.lib.csv.csv_data_exporter as csv

class CsvExporterTests(unittest.TestCase):
    def setUp(self):
        from app.data.database.database import Database
        self.db = Database()
        self.db.load('testing_proj.ltproj')

    def tearDown(self) -> None:
        pass

    def test_unit_dump(self):
        unit_db = UnitCatalog()
        # test shortened version
        for i in range(2):
            unit_db.append(self.db.units[i])
        dmp = csv.unit_db_to_csv(unit_db)
        self.assertEqual(dmp,
"""NAME,NID,HP_base,STR_base,MAG_base,SKL_base,SPD_base,LCK_base,DEF_base,CON_base,MOV_base,RES_base,HP_growth,STR_growth,MAG_growth,SKL_growth,SPD_growth,LCK_growth,DEF_growth,CON_growth,MOV_growth,RES_growth,Sword,Lance,Axe,Bow,Staff,Light,Anima,Dark,Default
Eirika,Eirika,16,4,2,8,9,5,3,5,5,1,70,40,20,60,60,60,30,0,0,30,1,0,0,0,0,0,0,0,0
Seth,Seth,30,14,7,13,12,13,11,11,8,8,90,50,25,45,45,25,40,0,0,30,181,181,0,0,0,0,0,0,0
""")

    def test_klass_dump(self):
        klass_db = ClassCatalog()
        # shorten
        for i in range(2):
            klass_db.append(self.db.classes[i])
        dmp = csv.klass_db_to_csv(klass_db)
        self.assertEqual(dmp,
"""NAME,NID,HP_base,STR_base,MAG_base,SKL_base,SPD_base,LCK_base,DEF_base,CON_base,MOV_base,RES_base,LEAD_base,HP_growth,STR_growth,MAG_growth,SKL_growth,SPD_growth,LCK_growth,DEF_growth,CON_growth,MOV_growth,RES_growth,LEAD_growth,Sword,Lance,Axe,Bow,Staff,Light,Anima,Dark,Default
Citizen,Citizen,10,0,0,0,0,0,0,5,5,0,0,65,0,0,0,0,0,0,0,0,0,0,False,False,False,False,False,False,False,False,False
Lord,Eirika_Lord,16,4,2,8,9,0,3,5,5,0,0,90,45,22,40,45,40,15,0,0,20,0,True,False,False,False,False,False,False,False,False
"""
)

    def test_item_dmp(self):
        item_db = ItemCatalog()
        # shorten
        for i in range(2):
            item_db.append(self.db.items[i])
        dmp = csv.item_db_to_csv(item_db)
        self.assertEqual(dmp,
"""NAME,NID,HIT,CRIT,DAMAGE,WEAPON_TYPE,WEAPON_RANK,WEIGHT,USES,MIN_RANGE,MAX_RANGE,VALUE
Iron Sword,Iron Sword,90,0,5,Sword,E,5,46,1,1,460
Slim Sword,Slim Sword,100,5,3,Sword,E,2,30,1,1,480
""")

    def test_update_unit_from_csv(self):
        # update Eirika's STR to 25, HP to 35, HP Growth to 45, and and give her 55 dark wexp
        csvstr = (
"""NAME,NID,HP_base,STR_base,MAG_base,SKL_base,SPD_base,LCK_base,DEF_base,CON_base,MOV_base,RES_base,HP_growth,STR_growth,MAG_growth,SKL_growth,SPD_growth,LCK_growth,DEF_growth,CON_growth,MOV_growth,RES_growth,Sword,Lance,Axe,Bow,Staff,Light,Anima,Dark,Default
Eirika,Eirika,35,25,2,8,9,5,3,5,5,1,45,40,20,60,60,60,30,0,0,30,1,0,0,0,0,0,0,55,0
""")
        csv.update_db_with_unit_csv(self.db, csvstr)
        eirika = self.db.units.get('Eirika')
        self.assertEqual(eirika.bases['HP'], 35)
        self.assertEqual(eirika.bases['STR'], 25)
        self.assertEqual(eirika.growths['HP'], 45)
        self.assertEqual(eirika.wexp_gain['Dark'].wexp_gain, 55)

        # adds a new unit called Eric with the same stats
        new_unit_csvstr = (
"""NAME,NID,HP_base,STR_base,MAG_base,SKL_base,SPD_base,LCK_base,DEF_base,CON_base,MOV_base,RES_base,HP_growth,STR_growth,MAG_growth,SKL_growth,SPD_growth,LCK_growth,DEF_growth,CON_growth,MOV_growth,RES_growth,Sword,Lance,Axe,Bow,Staff,Light,Anima,Dark,Default
Eric,Eric,35,25,2,8,9,5,3,5,5,1,45,40,20,60,60,60,30,0,0,30,1,0,0,0,0,0,0,55,0
""")
        csv.update_db_with_unit_csv(self.db, new_unit_csvstr)
        eric = self.db.units.get('Eric')
        self.assertEqual(eric.bases['HP'], 35)
        self.assertEqual(eric.bases['STR'], 25)
        self.assertEqual(eric.growths['HP'], 45)
        self.assertEqual(eric.wexp_gain['Dark'].wexp_gain, 55)

    def test_update_items_from_csv(self):
        # update slim sword to 99 might and 10000 value and D rank
        csvstr = (
"""NAME,NID,HIT,CRIT,DAMAGE,WEAPON_TYPE,WEAPON_RANK,WEIGHT,USES,MIN_RANGE,MAX_RANGE,VALUE
Slim Sword,Slim Sword,100,5,99,Sword,D,2,30,1,1,10000
""")
        csv.update_db_with_item_csv(self.db, csvstr)
        slim = self.db.items.get('Slim Sword')
        self.assertEqual(slim.damage.value, 99)
        self.assertEqual(slim.value.value, 10000)
        self.assertEqual(slim.weapon_rank.value, 'D')

        # add new item called "Sim Sword" with same stats
        csvstr = (
"""NAME,NID,HIT,CRIT,DAMAGE,WEAPON_TYPE,WEAPON_RANK,WEIGHT,USES,MIN_RANGE,MAX_RANGE,VALUE
Sim Sword,Sim Sword,100,5,99,Sword,D,2,30,1,1,10000
""")
        csv.update_db_with_item_csv(self.db, csvstr)
        new_sim = self.db.items.get('Sim Sword')
        self.assertEqual(new_sim.damage.value, 99)
        self.assertEqual(new_sim.value.value, 10000)
        self.assertEqual(new_sim.weapon_rank.value, 'D')

if __name__ == '__main__':
    unittest.main()
