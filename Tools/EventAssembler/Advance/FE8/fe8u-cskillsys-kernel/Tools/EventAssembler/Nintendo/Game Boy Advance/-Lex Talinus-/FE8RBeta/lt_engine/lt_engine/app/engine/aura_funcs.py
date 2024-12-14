from dataclasses import dataclass
from typing import List
from app.utilities.typing import Color3, NID
from app.engine.objects.unit import UnitObject
from app.data.database.database import DB

from app.engine import action, skill_system, line_of_sight
from app.engine.source_type import SourceType

import logging

@dataclass
class AuraInfo():
    parent_skill: NID
    aura_skill_nid: NID
    aura_range: int
    aura_target: str
    show_aura: bool = False
    aura_color: Color3 = None

def get_all_aura_info(unit: UnitObject) -> List[AuraInfo]:
    all_aura_info: List[AuraInfo] = []
    for skill in unit.skills:
        if skill.aura:
            aura_info = AuraInfo(skill.nid, skill.aura.value,
                                 skill.aura_range.value, skill.aura_target.value)
            if skill.show_aura:
                aura_info.show_aura = True
                aura_info.aura_color = skill.show_aura.value
            all_aura_info.append(aura_info)
    return all_aura_info

def pull_auras(unit, game, test=False):
    for aura_data in game.board.get_auras(unit.position):
        child_aura_uid, target = aura_data
        child_skill = game.get_skill(child_aura_uid)
        owner_nid = child_skill.parent_skill.owner_nid
        owner = game.get_unit(owner_nid)
        if owner is not unit:
            apply_aura(owner, unit, child_skill, target, test)

def apply_aura(owner, unit, child_skill, target, test=False):
    if target == 'enemy' and skill_system.check_enemy(owner, unit) or \
            target == 'ally' and skill_system.check_ally(owner, unit) or \
            target == 'unit':
        # Confirm that we have line of sight
        if DB.constants.value('aura_los') and \
                not line_of_sight.line_of_sight({owner.position}, {unit.position}, 99):
            return
        logging.debug("Applying Aura %s to %s", child_skill, unit)
        if test:
            # Doesn't need to use action system
            unit.add_skill(child_skill, source=child_skill.parent_skill.uid, source_type=SourceType.AURA)
        else:
            act = action.AddSkill(unit, child_skill, source=child_skill.parent_skill.uid, source_type=SourceType.AURA)
            # action.do(act)
            act.do()

def remove_aura(unit, child_skill, test=False):
    if child_skill in unit.all_skills:
        logging.debug("Removing Aura %s from %s", child_skill, unit)
        if test:
            unit.remove_skill(child_skill, source=child_skill.parent_skill.uid, source_type=SourceType.AURA)
        else:
            act = action.RemoveSkill(unit, child_skill, source=child_skill.parent_skill.uid, source_type=SourceType.AURA)
            # action.do(act)
            act.do()

def propagate_aura(unit, skill, game):
    game.board.reset_aura(skill.subskill)
    aura_range = skill.aura_range.value
    aura_range = set(range(1, aura_range + 1))
    positions = game.target_system.get_shell({unit.position}, aura_range, game.board.bounds)
    for pos in positions:
        game.board.add_aura(pos, skill.subskill, skill.aura_target.value)
        # Propagate my aura to others
        other = game.board.get_unit(pos)
        if other:
            apply_aura(unit, other, skill.subskill, skill.aura_target.value)

def repopulate_aura(unit, skill, game):
    """
    Called only on loading a level in order to
    re-populate the game.board with the aura connections
    """
    game.board.reset_aura(skill.subskill)
    aura_range = skill.aura_range.value
    aura_range = set(range(1, aura_range + 1))
    positions = game.target_system.get_shell({unit.position}, aura_range, game.board.bounds)
    for pos in positions:
        game.board.add_aura(pos, skill.subskill, skill.aura_target.value)

def release_aura(unit, skill, game):
    logging.debug("Releasing Aura %s (owned by %s)", skill, unit)
    for pos in list(game.board.get_aura_positions(skill.subskill)):
        game.board.remove_aura(pos, skill.subskill)
        # Release aura from others
        other = game.board.get_unit(pos)
        if other:
            remove_aura(other, skill.subskill)
    game.board.reset_aura(skill.subskill)
