"""
# A collection of useful functions for handling
# enntity movement
"""
from __future__ import annotations

from typing import Tuple, TYPE_CHECKING

from app.data.database.database import DB
from app.engine import equations, skill_system
from app.engine.game_state import game

if TYPE_CHECKING:
    from app.engine.objects.unit import UnitObject

def get_movement_group(unit_to_move: UnitObject):
    movement_group = skill_system.movement_type(unit_to_move)
    if not movement_group:
        movement_group = DB.classes.get(unit_to_move.klass).movement_group
    return movement_group

def get_mcost(unit_to_move: UnitObject, pos: Tuple[int, int]) -> int:
    if DB.terrain:
        terrain_nid = game.get_terrain_nid(game.tilemap, pos)
        terrain = DB.terrain.get(terrain_nid)
        if not terrain:
            terrain = DB.terrain[0]
        if unit_to_move:
            movement_group = get_movement_group(unit_to_move)
        else:
            movement_group = DB.classes[0].movement_group
        mcost = DB.mcost.get_mcost(movement_group, terrain.mtype)
    else:
        mcost = 1
    return mcost

def check_traversable(unit_to_move: UnitObject, pos: Tuple[int, int]) -> bool:
    if not game.board.check_bounds(pos):
        return False
    mcost = get_mcost(unit_to_move, pos)
    movement = equations.parser.movement(unit_to_move)
    return mcost <= movement

def check_weakly_traversable(unit_to_move: UnitObject, pos: Tuple[int, int]) -> bool:
    if not game.board.check_bounds(pos):
        return False
    mcost = get_mcost(unit_to_move, pos)
    movement = equations.parser.movement(unit_to_move)
    return mcost <= 5 or mcost <= movement

def check_simple_traversable(pos: Tuple[int, int]) -> bool:
    if not game.board.check_bounds(pos):
        return False
    mcost = get_mcost(None, pos)
    return mcost <= 5

def check_position(unit: UnitObject, new_position: Tuple[int, int], 
                   is_final_pos: bool = True, event: bool = False) -> bool:
    """
    # Check if we run into an enemy or an interrupting region
    # Returns True if position is OK
    """
    # Interruption regions take precedence, even over event movements
    # Only applies if there's not already a unit standing on the position
    if not game.board.get_unit(unit.position):
        interrupted = check_region_interrupt(unit.position)
        if interrupted:
            return False
    # Event movement is nearly always valid
    if event:
        return True
    elif skill_system.pass_through(unit):
        # If this is the final position
        if is_final_pos and game.board.get_unit(new_position):
            return False
        else:
            return True
    else:
        other_team = game.board.get_team(new_position)
        if not other_team or unit.team in DB.teams.get_allies(other_team):
            return True # Allies, this is fine
        else:  # Enemies
            return False

def check_region_interrupt(pos: Tuple[int, int]):
    """
    # Checks if the position is in a region that interrupts.
    # Returns region that would interrupt
    """
    for region in game.level.regions:
        if region.contains(pos) and region.interrupt_move:
            return region
    return False
