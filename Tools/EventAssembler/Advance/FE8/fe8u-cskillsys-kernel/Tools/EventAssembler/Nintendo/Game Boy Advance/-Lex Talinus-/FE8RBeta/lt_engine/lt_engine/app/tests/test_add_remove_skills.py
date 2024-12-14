import unittest
from unittest.mock import MagicMock, Mock, patch, call
from app.engine.objects.item import ItemObject
from app.engine.game_board import GameBoard
from app.engine.target_system import TargetSystem
from app.engine.objects.unit import UnitObject
from app.engine.action import AddSkill, RemoveSkill, Action
from app.engine.source_type import SourceType

class FakeResetUnitVars:
    def __init__(self, unit):
        return

    def do(self):
        return

    def reverse(self):
        return
        
    def execute(self):
        return

class AddRemoveSkillTests(unittest.TestCase):
    def setUp(self):
        from app.data.database.database import DB
        DB.load('testing_proj.ltproj')
        self.test_unit = UnitObject('player')
        self.test_skill = MagicMock()
        self.test_skill.nid = 'Potatomancy'
        self.test_skill.stack = None
        self.test_skill_stack = MagicMock()
        self.test_skill_stack.nid = 'Spud_Stack'
        self.test_skill_stack.stack = MagicMock()
        self.test_skill_stack.stack.value = 3
        self.patchers = [
            patch('app.engine.action.ResetUnitVars', FakeResetUnitVars),
        ]
        for patcher in self.patchers:
            patcher.start()
        pass

    def tearDown(self):
        pass
    
    def test_add_removable_skill(self):
        '''
        You can add a skill to a unit under normal circumstances
        '''
        with unittest.mock.patch('app.engine.action.ResetUnitVars', FakeResetUnitVars) as plz_work:
            AddSkill(self.test_unit, self.test_skill).do()
            AddSkill(self.test_unit, 'Rescue').do()
            
            self.assertIn('Potatomancy', [s.nid for s in self.test_unit.all_skills])
            self.assertIn('Rescue', [s.nid for s in self.test_unit.all_skills])
        
    def test_remove_removable_skill(self):
        '''
        You can remove a skill from a unit under normal circumstances
        '''
        with unittest.mock.patch('app.engine.action.ResetUnitVars', FakeResetUnitVars) as plz_work:
            AddSkill(self.test_unit, self.test_skill).do()
            self.assertIn('Potatomancy', [s.nid for s in self.test_unit.all_skills])
            RemoveSkill(self.test_unit, self.test_skill).do()
            self.assertNotIn('Potatomancy', [s.nid for s in self.test_unit.all_skills])
            
            AddSkill(self.test_unit, self.test_skill).do()
            self.assertIn('Potatomancy', [s.nid for s in self.test_unit.all_skills])
            RemoveSkill(self.test_unit, 'Potatomancy', source='Bollocks', source_type=SourceType.KLASS).do()
            self.assertNotIn('Potatomancy', [s.nid for s in self.test_unit.all_skills])
        
    def test_add_remove_unremovable_skill(self):
        '''
        You can remove a unremovable skill from a unit only via the introducing effect, matching both NID and type
        '''
        with unittest.mock.patch('app.engine.action.ResetUnitVars', FakeResetUnitVars) as plz_work:
            AddSkill(self.test_unit, self.test_skill, source='Idaho', source_type=SourceType.TERRAIN).do()
            self.assertIn('Potatomancy', [s.nid for s in self.test_unit.all_skills])
            
            RemoveSkill(self.test_unit, 'Potatomancy').do()
            self.assertIn('Potatomancy', [s.nid for s in self.test_unit.all_skills])
            
            RemoveSkill(self.test_unit, 'Potatomancy', source='Idaho', source_type=SourceType.REGION).do()
            self.assertIn('Potatomancy', [s.nid for s in self.test_unit.all_skills])
            
            RemoveSkill(self.test_unit, 'Potatomancy', source='Bombay', source_type=SourceType.TERRAIN).do()
            self.assertIn('Potatomancy', [s.nid for s in self.test_unit.all_skills])
            
            RemoveSkill(self.test_unit, 'Potatomancy', source='Idaho', source_type=SourceType.TERRAIN).do()
            self.assertNotIn('Potatomancy', [s.nid for s in self.test_unit.all_skills])
        
    def test_displace_displaceable_skill(self):
        '''
        Adding a new copy of a skill displaces the existing one
        '''
        with unittest.mock.patch('app.engine.action.ResetUnitVars', FakeResetUnitVars) as plz_work:
            test_skill_dup = MagicMock()
            test_skill_dup.nid = 'Rescue'
            test_skill_dup.stack = None

            AddSkill(self.test_unit, test_skill_dup).do()
            self.assertIn(test_skill_dup, self.test_unit.all_skills)
        
            AddSkill(self.test_unit, 'Rescue').do()
            self.assertNotIn(test_skill_dup, self.test_unit.all_skills)
            self.assertIn('Rescue', [s.nid for s in self.test_unit.all_skills])
        
    def test_displace_undisplaceable_skill(self):
        '''
        You can remove a unremovable skill from a unit only via the introducing effect, matching both NID and type
        '''
        with unittest.mock.patch('app.engine.action.ResetUnitVars', FakeResetUnitVars) as plz_work:
            AddSkill(self.test_unit, self.test_skill, source='Idaho', source_type=SourceType.TERRAIN).do()
            self.assertIn(self.test_skill, self.test_unit.all_skills)
            
            test_skill_dup = MagicMock()
            test_skill_dup.nid = 'Potatomancy'
            test_skill_dup.stack = None
            AddSkill(self.test_unit, test_skill_dup).do()
            self.assertIn(self.test_skill, self.test_unit.all_skills)
            self.assertNotIn(test_skill_dup, self.test_unit.all_skills)
            
            AddSkill(self.test_unit, test_skill_dup, source='Bungus_Blade', source_type=SourceType.ITEM).do()
            self.assertIn(self.test_skill, self.test_unit.all_skills)
            self.assertIn(test_skill_dup, self.test_unit.all_skills)
            
    def test_displace_stacks(self):
        '''
        Adding a new copy of a stack skill displaces the oldest existing displaceable copy
        '''
        with unittest.mock.patch('app.engine.action.ResetUnitVars', FakeResetUnitVars) as plz_work:
            AddSkill(self.test_unit, self.test_skill_stack, source='Magmancer', source_type=SourceType.KLASS).do()
            
            test_skill_stack2 = MagicMock()
            test_skill_stack2.nid = 'Spud_Stack'
            test_skill_stack2.stack = MagicMock()
            test_skill_stack2.stack.value = 3
            AddSkill(self.test_unit, test_skill_stack2).do()
            
            test_skill_stack3 = MagicMock()
            test_skill_stack3.nid = 'Spud_Stack'
            test_skill_stack3.stack = MagicMock()
            test_skill_stack3.stack.value = 3
            AddSkill(self.test_unit, test_skill_stack3).do()
            self.assertIn(self.test_skill_stack, self.test_unit.all_skills)
            self.assertIn(test_skill_stack2, self.test_unit.all_skills)
            self.assertIn(test_skill_stack3, self.test_unit.all_skills)
            
            test_skill_stack4 = MagicMock()
            test_skill_stack4.nid = 'Spud_Stack'
            test_skill_stack4.stack = MagicMock()
            test_skill_stack4.stack.value = 3
            AddSkill(self.test_unit, test_skill_stack4).do()
            self.assertIn(self.test_skill_stack, self.test_unit.all_skills)
            self.assertNotIn(test_skill_stack2, self.test_unit.all_skills)
            self.assertIn(test_skill_stack3, self.test_unit.all_skills)
            self.assertIn(test_skill_stack4, self.test_unit.all_skills)
            
    def test_remove_stacks(self):
        '''
        Removing a stack skill removes all removable copies (including hidden ones), or the oldest removable copy for single count
        '''
        with unittest.mock.patch('app.engine.action.ResetUnitVars', FakeResetUnitVars) as plz_work:
            AddSkill(self.test_unit, self.test_skill_stack, source='Fart_Aura', source_type=SourceType.AURA).do()
            
            test_skill_stack2 = MagicMock()
            test_skill_stack2.nid = 'Spud_Stack'
            test_skill_stack2.stack = MagicMock()
            test_skill_stack2.stack.value = 3
            AddSkill(self.test_unit, test_skill_stack2, source='Magmancer', source_type=SourceType.KLASS).do()
            
            test_skill_stack3 = MagicMock()
            test_skill_stack3.nid = 'Spud_Stack'
            test_skill_stack3.stack = MagicMock()
            test_skill_stack3.stack.value = 3
            AddSkill(self.test_unit, test_skill_stack3, source='Magmancer', source_type=SourceType.KLASS).do()
            
            test_skill_stack4 = MagicMock()
            test_skill_stack4.nid = 'Spud_Stack'
            test_skill_stack4.stack = MagicMock()
            test_skill_stack4.stack.value = 3
            AddSkill(self.test_unit, test_skill_stack4, source='Ophie', source_type=SourceType.PERSONAL).do()
            
            test_skill_stack5 = MagicMock()
            test_skill_stack5.nid = 'Spud_Stack'
            test_skill_stack5.stack = MagicMock()
            test_skill_stack5.stack.value = 3
            AddSkill(self.test_unit, test_skill_stack5, source='Magmancer', source_type=SourceType.KLASS).do()
            self.assertIn(self.test_skill_stack, self.test_unit.all_skills)
            self.assertIn(test_skill_stack2, self.test_unit.all_skills)
            self.assertIn(test_skill_stack3, self.test_unit.all_skills)
            self.assertIn(test_skill_stack4, self.test_unit.all_skills)
            self.assertIn(test_skill_stack5, self.test_unit.all_skills)
            
            RemoveSkill(self.test_unit, 'Spud_Stack', count=1).do()
            self.assertIn(self.test_skill_stack, self.test_unit.all_skills)
            self.assertNotIn(test_skill_stack2, self.test_unit.all_skills)
            self.assertIn(test_skill_stack3, self.test_unit.all_skills)
            self.assertIn(test_skill_stack4, self.test_unit.all_skills)
            self.assertIn(test_skill_stack5, self.test_unit.all_skills)
            
            RemoveSkill(self.test_unit, 'Spud_Stack').do()
            self.assertIn(self.test_skill_stack, self.test_unit.all_skills)
            self.assertNotIn(test_skill_stack2, self.test_unit.all_skills)
            self.assertNotIn(test_skill_stack3, self.test_unit.all_skills)
            self.assertNotIn(test_skill_stack4, self.test_unit.all_skills)
            self.assertNotIn(test_skill_stack5, self.test_unit.all_skills)
            

if __name__ == '__main__':
    unittest.main()