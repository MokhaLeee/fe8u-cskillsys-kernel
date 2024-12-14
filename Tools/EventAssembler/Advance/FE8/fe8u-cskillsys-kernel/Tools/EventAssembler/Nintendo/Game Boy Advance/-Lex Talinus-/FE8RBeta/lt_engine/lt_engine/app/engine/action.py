from __future__ import annotations
from app.data.database.item_components import ItemComponent
from app.data.database.supports import SupportPair
from app.utilities.typing import NID

import functools
import logging
import sys
import app.engine.config as cf
from typing import Any, Optional, Tuple

from app.constants import TILEHEIGHT, TILEWIDTH
from app.data.database.database import DB
from app.events.regions import RegionType
from app.events import triggers
from app.data.resources.resources import RESOURCES
from app.engine import (aura_funcs, banner, equations, item_funcs, item_system,
                        particles, skill_system, unit_funcs, animations)
from app.engine.game_state import game
from app.engine.objects.item import ItemObject
from app.engine.objects.skill import SkillObject
from app.engine.objects.unit import UnitObject
from app.engine.objects.region import RegionObject
from app.engine import engine
from app.utilities import utils, static_random
from app.engine.source_type import SourceType

def alters_game_state(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        func(*args, **kwargs)
        game.on_alter_game_state()
    return wrapper

def wrap_do_exec_reverse(_cls):
    for func in ['do', 'execute', 'reverse']:
      setattr(_cls, func, alters_game_state(getattr(_cls, func)))
    def wrapper():
        return _cls()
    return wrapper

class Action():
    def __init_subclass__(cls, **kwargs):
        return wrap_do_exec_reverse(_cls=cls)

    def __init__(self):
        pass

    # When used normally
    def do(self):
        pass

    # When put in forward motion by the turnwheel
    def execute(self):
        self.do()

    # When put in reverse motion by the turnwheel
    def reverse(self):
        pass

    def __repr__(self):
        s = "action.%s: " % self.__class__.__name__
        for attr in self.__dict__.items():
            name, value = attr
            s += '%s: %s, ' % (name, value)
        s = s[:-2]
        return s

    @staticmethod
    def save_obj(value):
        if isinstance(value, UnitObject):
            value = ('unit', value.nid)
        elif isinstance(value, ItemObject):
            value = ('item', value.uid)
        elif isinstance(value, SkillObject):
            value = ('skill', value.uid)
        elif isinstance(value, RegionObject):
            value = ('region', value.nid)
        elif isinstance(value, list):
            value = ('list', [Action.save_obj(v) for v in value])
        elif isinstance(value, Action):
            value = ('action', value.save())
        else:
            value = ('generic', value)
        return value

    def save(self):
        ser_dict = {}
        for attr in self.__dict__.items():
            name, value = attr
            value = self.save_obj(value)
            ser_dict[name] = value
        return (self.__class__.__name__, ser_dict)

    @staticmethod
    def restore_obj(value):
        if value[0] == 'unit':
            return game.get_unit(value[1])
        elif value[0] == 'item':
            return game.get_item(value[1])
        elif value[0] == 'skill':
            return game.get_skill(value[1])
        elif value[0] == 'region':
            return game.get_region(value[1])
        elif value[0] == 'list':
            return [Action.restore_obj(v) for v in value[1]]
        elif value[0] == 'action':
            name, value = value[1][0], value[1][1]
            action = getattr(sys.modules[__name__], name)
            return action.restore(value)
        else:
            return value[1]

    @classmethod
    def restore(cls, ser_dict):
        self = cls.__new__(cls)
        for name, value in ser_dict.items():
            setattr(self, name, self.restore_obj(value))
        return self

def recalc_unit(unit):
    unit.autoequip()
    if unit.position and game.tilemap and game.boundary:
        game.boundary.recalculate_unit(unit)

def recalculate_unit(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        func(*args, **kwargs)
        self = args[0]
        recalc_unit(self.unit)

    return wrapper

def recalculate_unit_sprite(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        self = args[0]

        old_sprite = self.unit.sprite
        animations = list((k, v) for k, v in old_sprite.animations.items())
        self.unit.reset_sprite()

        func(*args, **kwargs)

        new_sprite = self.unit.sprite
        for anim_nid, animation in animations:
            new_sprite.add_animation(anim_nid, contingent=animation.contingent)

    return wrapper

class Move(Action):
    """
    A basic, user-directed move
    """

    def __init__(self, unit, new_pos, path=None, event=False, follow=True, speed=0):
        self.unit = unit
        self.old_pos = self.unit.position
        self.new_pos = new_pos

        self.prev_movement_left = self.unit.movement_left
        self.new_movement_left = None

        self.path = path
        self.has_moved = self.unit.has_moved
        self.event = event
        self.follow = follow
        self.speed = speed or cf.SETTINGS['unit_speed']

    def do(self):
        if self.path is None:
            self.path = game.cursor.path[:]
        game.movement.begin_move(self.unit, self.path, self.event, self.follow, speed=self.speed)

    def execute(self):
        game.leave(self.unit)
        if self.new_movement_left is not None:
            self.unit.movement_left = self.new_movement_left
        self.unit.has_moved = True
        self.unit.position = self.new_pos
        game.arrive(self.unit)

    def reverse(self):
        game.leave(self.unit)
        self.new_movement_left = self.unit.movement_left
        self.unit.has_moved = self.has_moved
        self.new_pos = self.unit.position
        self.unit.position = self.old_pos
        game.arrive(self.unit)
        self.unit.movement_left = self.prev_movement_left

# Just another name for move
class CantoMove(Move):
    pass


class SimpleMove(Move):
    """
    A script directed move, no animation
    """

    def __init__(self, unit, new_pos):
        self.unit = unit
        self.old_pos = self.unit.position
        self.new_pos = new_pos
        self.update_fow_action = UpdateFogOfWar(self.unit)

    def do(self):
        game.leave(self.unit)
        self.unit.position = self.new_pos
        game.arrive(self.unit)
        self.update_fow_action.do()

    def execute(self):
        game.leave(self.unit)
        self.unit.position = self.new_pos
        game.arrive(self.unit)
        self.update_fow_action.execute()

    def reverse(self):
        self.update_fow_action.reverse()
        game.leave(self.unit)
        self.unit.position = self.old_pos
        game.arrive(self.unit)


class Teleport(SimpleMove):
    pass


class ForcedMovement(SimpleMove):
    def do(self):
        # Sprite transition
        x_offset = (self.old_pos[0] - self.new_pos[0]) * TILEWIDTH
        y_offset = (self.old_pos[1] - self.new_pos[1]) * TILEHEIGHT
        self.unit.sprite.offset = [x_offset, y_offset]
        self.unit.sprite.set_transition('fake_in')

        game.leave(self.unit)
        self.unit.position = self.new_pos
        game.arrive(self.unit)
        self.update_fow_action.do()


class Swap(Action):
    def __init__(self, unit1, unit2):
        self.unit1 = unit1
        self.unit2 = unit2
        self.pos1 = unit1.position
        self.pos2 = unit2.position
        self.update_fow_action1 = UpdateFogOfWar(self.unit1)
        self.update_fow_action2 = UpdateFogOfWar(self.unit2)

    def do(self):
        game.leave(self.unit1)
        game.leave(self.unit2)
        self.unit1.position, self.unit2.position = self.pos2, self.pos1
        game.arrive(self.unit2)
        game.arrive(self.unit1)
        self.update_fow_action1.do()
        self.update_fow_action2.do()

    def reverse(self):
        self.update_fow_action1.reverse()
        self.update_fow_action2.reverse()
        game.leave(self.unit1)
        game.leave(self.unit2)
        self.unit1.position, self.unit2.position = self.pos1, self.pos2
        game.arrive(self.unit2)
        game.arrive(self.unit1)


class Warp(SimpleMove):
    def do(self):
        self.unit.sprite.set_transition('warp_move')

        game.leave(self.unit)
        self.unit.position = self.new_pos
        game.arrive(self.unit)
        self.update_fow_action.do()


class Swoosh(SimpleMove):
    def do(self):
        self.unit.sprite.set_transition('swoosh_move')

        game.leave(self.unit)
        self.unit.position = self.new_pos
        game.arrive(self.unit)
        self.update_fow_action.do()


class FadeMove(SimpleMove):
    def do(self):
        self.unit.sprite.set_transition('fade_move')

        game.leave(self.unit)
        self.unit.position = self.new_pos
        game.arrive(self.unit)
        self.update_fow_action.do()

class PutUnitDown(Action):
    def __init__(self, unit, test=False):
        self.unit = unit
        self.test = test

    def do(self):
        game.arrive(self.unit, self.test)

    def reverse(self):
        game.leave(self.unit, self.test)

class PickUnitUp(Action):
    def __init__(self, unit, test=False):
        self.unit = unit
        self.test = test

    def do(self):
        game.leave(self.unit, self.test)

    def reverse(self):
        game.arrive(self.unit, self.test)

class ArriveOnMap(Action):
    def __init__(self, unit, pos):
        self.unit = unit
        self.place_on_map = PlaceOnMap(unit, pos)

    def do(self):
        self.place_on_map.do()
        game.arrive(self.unit)

    def reverse(self):
        game.leave(self.unit)
        self.place_on_map.reverse()

class WarpIn(ArriveOnMap):
    def do(self):
        self.place_on_map.do()
        self.unit.sprite.set_transition('warp_in')
        game.arrive(self.unit)


class SwooshIn(ArriveOnMap):
    def do(self):
        self.place_on_map.do()
        self.unit.sprite.set_transition('swoosh_in')
        game.arrive(self.unit)


class FadeIn(ArriveOnMap):
    def __init__(self, unit, pos, fade_direction=None):
        self.unit = unit
        self.fade_direction = fade_direction
        self.place_on_map = PlaceOnMap(unit, pos)

    def do(self):
        self.place_on_map.do()
        if game.tilemap.on_border(self.unit.position) or self.fade_direction:
            if self.unit.position[0] == 0 or self.fade_direction == 'west':
                self.unit.sprite.offset = [-TILEWIDTH, 0]
            elif self.unit.position[0] == game.tilemap.width - 1 or self.fade_direction == 'east':
                self.unit.sprite.offset = [TILEWIDTH, 0]
            elif self.unit.position[1] == 0 or self.fade_direction == 'north':
                self.unit.sprite.offset = [0, -TILEHEIGHT]
            elif self.unit.position[1] == game.tilemap.height - 1 or self.fade_direction == 'south':
                self.unit.sprite.offset = [0, TILEHEIGHT]
            self.unit.sprite.set_transition('fake_in')
        else:
            self.unit.sprite.set_transition('fade_in')
        game.arrive(self.unit)


class PlaceOnMap(Action):
    def __init__(self, unit, pos):
        self.unit = unit
        self.pos = pos
        self.update_fow_action = UpdateFogOfWar(self.unit)

    def do(self):
        self.unit.position = self.pos
        if self.unit.position:
            self.unit.previous_position = self.unit.position
        self.update_fow_action.do()

    def reverse(self):
        self.update_fow_action.reverse()
        self.unit.position = None


class LeaveMap(Action):
    def __init__(self, unit):
        self.unit = unit
        self.remove_from_map = RemoveFromMap(self.unit)

    def do(self):
        game.leave(self.unit)
        self.remove_from_map.do()

    def execute(self):
        game.leave(self.unit)
        self.remove_from_map.do()

    def reverse(self):
        self.remove_from_map.reverse()
        game.arrive(self.unit)


class WarpOut(LeaveMap):
    def do(self):
        game.leave(self.unit)
        self.unit.sprite.set_transition('warp_out')
        self.remove_from_map.do()


class SwooshOut(LeaveMap):
    def do(self):
        game.leave(self.unit)
        self.unit.sprite.set_transition('swoosh_out')
        self.remove_from_map.do()


class FadeOut(LeaveMap):
    def __init__(self, unit, fade_direction=None):
        self.unit = unit
        self.fade_direction = fade_direction
        self.remove_from_map = RemoveFromMap(self.unit)

    def do(self):
        game.leave(self.unit)
        if game.tilemap.on_border(self.unit.position) or self.fade_direction:
            if self.unit.position[0] == 0 or self.fade_direction == 'west':
                self.unit.sprite.offset = [-2, 0]
            elif self.unit.position[0] == game.tilemap.width - 1 or self.fade_direction == 'east':
                self.unit.sprite.offset = [2, 0]
            elif self.unit.position[1] == 0 or self.fade_direction == 'north':
                self.unit.sprite.offset = [0, -2]
            elif self.unit.position[1] == game.tilemap.height - 1 or self.fade_direction == 'south':
                self.unit.sprite.offset = [0, 2]
            self.unit.sprite.set_transition('fake_out')
        else:
            self.unit.sprite.set_transition('fade_out')
        self.remove_from_map.do()


class RemoveFromMap(Action):
    def __init__(self, unit):
        self.unit = unit
        self.old_pos = self.unit.position
        self.update_fow_action = UpdateFogOfWar(self.unit)

    def do(self):
        # In case the unit is currently still moving
        if game.movement.is_moving(self.unit):
            game.movement.stop(self.unit)
            self.unit.sprite.reset()

        self.unit.position = None
        self.update_fow_action.do()

    def reverse(self):
        self.update_fow_action.reverse()
        self.unit.position = self.old_pos
        if self.unit.position:
            self.unit.previous_position = self.unit.position

class RegisterUnit(Action):
    def __init__(self, unit):
        self.unit = unit

    def do(self):
        game.full_register(self.unit)

    def reverse(self):
        logging.debug("Unregistering unit %s and it's items and skills", self.unit.nid)
        for skill in reversed(self.unit.all_skills):
            game.unregister_skill(skill)
        for item in reversed(self.unit.items):
            game.unregister_item(item)
        game.unregister_unit(self.unit)

class IncrementTurn(Action):
    def do(self):
        from app.engine.game_state import game
        game.turncount += 1

    def reverse(self):
        game.turncount -= 1


class MarkPhase(Action):
    def __init__(self, phase_name):
        self.phase_name = phase_name


class LockTurnwheel(Action):
    def __init__(self, lock):
        self.lock = lock


class ChangePhaseMusic(Action):
    def __init__(self, phase, music):
        self.phase = phase
        self.old_music = game.level.music.get(phase, None)
        self.new_music = music

    def do(self):
        game.level.music[self.phase] = self.new_music

    def reverse(self):
        game.level.music[self.phase] = self.old_music


class Message(Action):
    def __init__(self, message):
        self.message = message


class SetGameVar(Action):
    def __init__(self, nid, val):
        self.nid = nid
        self.val = val
        self.old_val = game.game_vars[self.nid]

    def do(self):
        game.game_vars[self.nid] = self.val

    def reverse(self):
        game.game_vars[self.nid] = self.old_val


class SetLevelVar(Action):
    fog_nids = ('_fog_of_war', '_fog_of_war_radius', '_ai_fog_of_war_radius', '_other_fog_of_war_radius', '_fog_of_war_type')

    def __init__(self, nid, val):
        self.nid = nid
        self.val = val
        self.old_val = game.level_vars[self.nid]

    def _update_fog_of_war(self):
        if self.nid in self.fog_nids:
            for unit in game.units:
                if unit.position:
                    UpdateFogOfWar(unit).execute()

    def do(self):
        game.level_vars[self.nid] = self.val
        # Need to update fog of war when we change it
        self._update_fog_of_war()

    def reverse(self):
        game.level_vars[self.nid] = self.old_val
        self._update_fog_of_war()

class SetMovementLeft(Action):
    def __init__(self, unit, val):
        self.unit = unit
        self.val = val
        self.old_val = unit.movement_left

    def do(self):
        self.unit.movement_left = self.val

    def reverse(self):
        self.unit.movement_left = self.old_val

class Wait(Action):
    def __init__(self, unit):
        self.unit = unit
        self.action_state = self.unit.get_action_state()
        self.update_fow_action = UpdateFogOfWar(self.unit)
        self.regions_removed = self.remove_interrupt_regions()

    def do(self):
        self.unit.has_moved = True
        self.unit.has_traded = True
        self.unit.has_attacked = True
        self.unit.finished = True
        self.unit.current_move = None
        self.unit.sprite.change_state('normal')
        self.update_fow_action.do()
        for region in self.regions_removed:
            region.do()
        if game.cursor and game.cursor.cur_unit == self.unit:
            game.cursor.cur_unit = None
        if self.unit.traveler:
            self.unit.lead_unit = True
            game.get_unit(self.unit.traveler).lead_unit = False

    def reverse(self):
        self.unit.set_action_state(self.action_state)
        self.update_fow_action.reverse()
        for region in self.regions_removed:
            region.reverse()

    def remove_interrupt_regions(self):
        regions_to_remove = []
        for region in game.level.regions:
            if self.unit.position and region.contains(self.unit.position) and region.interrupt_move:
                if region.region_type == RegionType.EVENT:
                    did_trigger = game.events.trigger(triggers.RegionTrigger(region.sub_nid, self.unit, self.unit.position, region))
                if (region.region_type != RegionType.EVENT or did_trigger) and region.only_once:
                    regions_to_remove.append(RemoveRegion(region))
        return regions_to_remove


class UpdateFogOfWar(Action):
    def __init__(self, unit):
        self.unit = unit
        self.prev_pos = None

    def do(self):
        # Handle fog of war
        self.prev_pos = game.board.fow_vantage_point.get(self.unit.nid)
        fog_of_war_radius = game.board.get_fog_of_war_radius(self.unit.team)
        sight_range = skill_system.sight_range(self.unit) + fog_of_war_radius
        game.board.update_fow(self.unit.position, self.unit, sight_range)
        game.boundary.reset_fog_of_war()

    def reverse(self):
        # Handle fog of war
        fog_of_war_radius = game.board.get_fog_of_war_radius(self.unit.team)
        sight_range = skill_system.sight_range(self.unit) + fog_of_war_radius
        game.board.update_fow(self.prev_pos, self.unit, sight_range)
        game.boundary.reset_fog_of_war()


class ResetUnitVars(Action):
    def __init__(self, unit):
        self.unit = unit
        self.old_current_hp = self.unit.get_hp()
        self.old_current_mana = self.unit.get_mana()

    def do(self):
        self.unit.set_hp(min(self.unit.get_hp(), equations.parser.hitpoints(self.unit)))
        self.unit.set_mana(min(self.unit.get_mana(), equations.parser.get_mana(self.unit)))

    def reverse(self):
        self.unit.set_hp(self.old_current_hp)
        self.unit.set_mana(self.old_current_mana)


class SetPreviousPosition(Action):
    def __init__(self, unit):
        self.unit = unit
        self.old_previous_position = self.unit.previous_position

    def do(self):
        self.unit.previous_position = self.unit.position

    def reverse(self):
        self.unit.previous_position = self.old_previous_position

class SetPersistent(Action):
    def __init__(self, unit):
        self.unit = unit
        self.old_persistent = self.unit.persistent

    def do(self):
        self.unit.persistent = True

    def reverse(self):
        self.unit.persistent = self.old_persistent

class Reset(Action):
    def __init__(self, unit):
        self.unit = unit
        self.movement_left = self.unit.movement_left
        self.action_state = self.unit.get_action_state()

    def do(self):
        self.unit.reset()
        self.unit.movement_left = equations.parser.movement(self.unit)

    def reverse(self):
        self.unit.set_action_state(self.action_state)
        self.unit.movement_left = self.movement_left


class ResetAll(Action):
    def __init__(self, units):
        self.actions = [Reset(unit) for unit in units]

    def do(self):
        for action in self.actions:
            action.do()

    def reverse(self):
        for action in self.actions:
            action.reverse()


class HasAttacked(Reset):
    def do(self):
        self.unit.has_attacked = True

class HasTraded(Reset):
    def do(self):
        self.unit.has_traded = True

class HasNotAttacked(Reset):
    def do(self):
        self.unit.has_attacked = False

class HasNotTraded(Reset):
    def do(self):
        self.unit.has_traded = False


# === RESCUE ACTIONS ========================================================
class Rescue(Action):
    def __init__(self, unit, rescuee):
        self.unit = unit
        self.rescuee = rescuee
        self.old_pos = self.rescuee.position
        self.update_fow_rescuee = UpdateFogOfWar(self.rescuee)
        self.subactions = []

    def do(self):
        self.subactions.clear()
        self.unit.traveler = self.rescuee.nid
        # TODO Add transition

        game.leave(self.rescuee)
        self.rescuee.position = None
        self.unit.has_rescued = True

        if not skill_system.ignore_rescue_penalty(self.unit) and 'Rescue' in DB.skills.keys():
            self.subactions.append(AddSkill(self.unit, 'Rescue', source=self.rescuee.nid, source_type=SourceType.TRAVELER))

        for action in self.subactions:
            action.do()
        self.update_fow_rescuee.do()

    def execute(self):
        self.unit.traveler = self.rescuee.nid

        game.leave(self.rescuee)
        self.rescuee.position = None
        self.unit.has_rescued = True

        for action in self.subactions:
            action.execute()
        self.update_fow_rescuee.execute()

    def reverse(self):
        self.rescuee.position = self.old_pos
        game.arrive(self.rescuee)
        self.unit.traveler = None
        self.unit.has_rescued = False

        self.update_fow_rescuee.reverse()
        for action in self.subactions:
            action.reverse()


class Drop(Action):
    def __init__(self, unit, droppee, pos):
        self.unit = unit
        self.droppee = droppee
        self.pos = pos
        if self.droppee.team == game.phase.get_current():
            # Only "wait" units that are in our phase
            self.droppee_wait_action = Wait(self.droppee)
        else:
            self.droppee_wait_action = None
        self.subactions = []

    def do(self):
        self.subactions.clear()
        self.droppee.position = self.pos
        game.arrive(self.droppee)
        self.droppee.sprite.change_state('normal')
        if self.droppee_wait_action:
            self.droppee_wait_action.do()

        self.unit.traveler = None
        self.unit.has_dropped = True

        self.subactions.append(RemoveSkill(self.unit, "Rescue", source=self.droppee.nid, source_type=SourceType.TRAVELER))
        for action in self.subactions:
            action.do()

        if utils.calculate_distance(self.unit.position, self.pos) == 1:
            self.droppee.sprite.set_transition('fake_in')
            self.droppee.sprite.offset = [(self.unit.position[0] - self.pos[0]) * TILEWIDTH,
                                          (self.unit.position[1] - self.pos[1]) * TILEHEIGHT]

    def execute(self):
        self.droppee.position = self.pos
        game.arrive(self.droppee)
        self.droppee.sprite.change_state('normal')
        if self.droppee_wait_action:
            self.droppee_wait_action.execute()

        for action in self.subactions:
            action.execute()

        self.unit.traveler = None
        self.unit.has_dropped = True

    def reverse(self):
        self.unit.traveler = self.droppee.nid

        if self.droppee_wait_action:
            self.droppee_wait_action.reverse()
        game.leave(self.droppee)
        self.droppee.position = None
        self.unit.has_dropped = False

        for action in self.subactions:
            action.reverse()


class Give(Action):
    def __init__(self, unit, other):
        self.unit = unit
        self.other = other
        self.subactions = []

    def do(self):
        self.subactions.clear()

        self.other.traveler = self.unit.traveler
        if not skill_system.ignore_rescue_penalty(self.other) and 'Rescue' in DB.skills.keys():
            self.subactions.append(AddSkill(self.other, 'Rescue', source=self.other.traveler, source_type=SourceType.TRAVELER))

        self.unit.traveler = None
        self.subactions.append(RemoveSkill(self.unit, "Rescue", source=self.other.traveler, source_type=SourceType.TRAVELER))

        self.unit.has_given = True

        for action in self.subactions:
            action.do()

    def reverse(self):
        self.unit.traveler = self.other.traveler
        self.other.traveler = None
        self.unit.has_given = False

        for action in self.subactions:
            action.reverse()


class Take(Action):
    def __init__(self, unit, other):
        self.unit = unit
        self.other = other
        self.subactions = []

    def do(self):
        self.subactions.clear()

        self.unit.traveler = self.other.traveler
        if not skill_system.ignore_rescue_penalty(self.unit) and 'Rescue' in DB.skills.keys():
            self.subactions.append(AddSkill(self.unit, 'Rescue', source=self.unit.traveler, source_type=SourceType.TRAVELER))

        self.other.traveler = None
        self.subactions.append(RemoveSkill(self.other, "Rescue", source=self.unit.traveler, source_type=SourceType.TRAVELER))

        self.unit.has_taken = True

        for action in self.subactions:
            action.do()

    def reverse(self):
        self.other.traveler = self.unit.traveler
        self.unit.traveler = None
        self.unit.has_taken = False

        for action in self.subactions:
            action.reverse()

# === PAIR UP ACTIONS =======================================================
class PairUp(Action):
    def __init__(self, unit: UnitObject, target: UnitObject):
        self.unit = unit
        self.target = target
        self.old_pos = self.unit.position
        self.unit_gauge = self.unit.get_guard_gauge()
        self.target_gauge = self.target.get_guard_gauge()
        self.update_fow_unit = UpdateFogOfWar(self.unit)
        self.subactions = []

    def do(self):
        self.subactions.clear()
        self.target.traveler = self.unit.nid

        move_left = self.unit.movement_left
        self.subactions.append(Reset(self.unit))
        self.subactions.append(SetMovementLeft(self.unit, move_left))
        skill_system.on_pairup(self.unit, self.target)
        game.leave(self.unit)
        self.unit.position = None

        self.unit.lead_unit = False
        self.target.lead_unit = True

        logging.info(self.unit.nid + " was at " + str(self.old_pos) + " but paired up with " + self.target.nid + " at " + str(self.target.position))

        self.target.set_guard_gauge(self.unit_gauge + self.target_gauge)
        self.unit.set_guard_gauge(0)

        for action in self.subactions:
            action.do()
        self.update_fow_unit.do()

    def execute(self):
        self.target.traveler = self.unit.nid

        skill_system.on_pairup(self.unit, self.target)
        game.leave(self.unit)
        self.unit.position = None

        logging.info(self.unit.nid + " was at " + str(self.old_pos) + " but paired up with " + self.target.nid + " at " + str(self.target.position))

        self.target.set_guard_gauge(self.unit_gauge + self.target_gauge)
        self.unit.set_guard_gauge(0)

        for action in self.subactions:
            action.execute()
        self.update_fow_unit.execute()

    def reverse(self):
        self.unit.position = self.old_pos
        game.arrive(self.unit)
        self.target.traveler = None
        skill_system.on_separate(self.unit, self.target)

        self.unit.lead_unit = False
        self.target.lead_unit = False

        logging.info("The pair up between " + self.unit.nid + " and " + self.target.nid + " was reversed")

        self.unit.set_guard_gauge(self.unit_gauge)
        self.target.set_guard_gauge(self.target_gauge)

        self.update_fow_unit.reverse()
        for action in self.subactions:
            action.reverse()
        self.unit.sprite.change_state('normal')


class SwitchPaired(Action):
    def __init__(self, leader, follower):
        self.leader = leader
        self.follower = follower
        self.pos = leader.position
        self.orig_guard_gauge = leader.get_guard_gauge()
        self.update_fow_leader = UpdateFogOfWar(self.leader)
        self.update_fow_follower = UpdateFogOfWar(self.follower)

    def do(self):
        self.leader.traveler = None
        self.follower.traveler = self.leader.nid
        skill_system.on_separate(self.follower, self.leader)
        skill_system.on_pairup(self.leader, self.follower)
        self.follower.set_guard_gauge(self.orig_guard_gauge)
        self.leader.set_guard_gauge(0)

        logging.info(self.leader.nid + " and " + self.follower.nid + " switched. The first was leader but is now follower, and vice versa.")

        game.leave(self.leader)
        self.leader.position = None
        self.follower.position = self.pos
        game.arrive(self.follower)
        self.update_fow_leader.do()
        self.update_fow_follower.do()
        # What is this if statement doing?
        if not self.follower.lead_unit:
            self.follower.has_moved = True

    def reverse(self):
        self.leader.traveler = self.follower.nid
        self.follower.traveler = None
        self.leader.set_guard_gauge(self.orig_guard_gauge)
        self.follower.set_guard_gauge(0)

        logging.info(self.leader.nid + " and " + self.follower.nid + " reversed their earlier switch")

        skill_system.on_separate(self.leader, self.follower)
        skill_system.on_pairup(self.follower, self.leader)

        self.update_fow_leader.reverse()
        self.update_fow_follower.reverse()
        game.leave(self.follower)
        self.follower.position = None
        self.leader.position = self.pos
        game.arrive(self.leader)
        # What is this if statement doing?
        if not self.leader.lead_unit:
            self.leader.has_moved = True


# This is shamelessly copied from Drop, but I've kept it separate in case a madlad wants Rescue and Pair Up
class Separate(Action):
    def __init__(self, unit, droppee, pos):
        self.unit = unit
        self.droppee = droppee
        self.pos = pos
        self.droppee_wait_action = Wait(self.droppee)
        self.old_gauge = self.unit.get_guard_gauge()

    def do(self):
        self.droppee.position = self.pos
        game.arrive(self.droppee)
        self.droppee.sprite.change_state('normal')
        self.droppee_wait_action.do()

        self.unit.traveler = None
        self.unit.has_dropped = True

        self.droppee.set_guard_gauge(self.old_gauge//2)
        self.unit.set_guard_gauge(self.old_gauge//2)

        self.unit.lead_unit = False
        self.droppee.lead_unit = False

        skill_system.on_separate(self.droppee, self.unit)

        if self.unit.position and self.pos and utils.calculate_distance(self.unit.position, self.pos) == 1 and not self.unit.is_dying:
            self.droppee.sprite.set_transition('fake_in')
            self.droppee.sprite.offset = [(self.unit.position[0] - self.pos[0]) * TILEWIDTH,
                                          (self.unit.position[1] - self.pos[1]) * TILEHEIGHT]

    def execute(self):
        self.droppee.position = self.pos
        game.arrive(self.droppee)
        self.droppee.sprite.change_state('normal')
        self.droppee_wait_action.execute()

        self.unit.traveler = None
        self.unit.has_dropped = True

        self.droppee.set_guard_gauge(self.old_gauge//2)
        self.unit.set_guard_gauge(self.old_gauge//2)

        skill_system.on_separate(self.droppee, self.unit)

    def reverse(self):
        self.unit.traveler = self.droppee.nid

        self.droppee_wait_action.reverse()
        game.leave(self.droppee)
        self.droppee.position = None
        self.unit.has_dropped = False

        self.unit.lead_unit = True
        self.droppee.lead_unit = False

        skill_system.on_pairup(self.droppee, self.unit)

        self.unit.set_guard_gauge(self.old_gauge)
        self.droppee.set_guard_gauge(0)

class RemovePartner(Action):
    '''Removes the unit's partner and the rescue status if applicable'''
    def __init__(self, unit):
        self.unit = unit
        self.partner = self.unit.traveler
        self.status_action = \
            RemoveSkill(self.unit, "Rescue", source=self.partner, source_type=SourceType.TRAVELER)

    def do(self):
        self.unit.traveler = None
        self.status_action.do()

    def reverse(self):
        self.unit.traveler = self.partner
        self.status_action.reverse()

class IncGauge(Action):
    def __init__(self, unit, amount):
        self.unit = unit
        self.amount = amount
        self.old_gauge = unit.get_guard_gauge()

    def do(self):
        self.unit.set_guard_gauge(self.unit.get_guard_gauge() + self.amount)

    def reverse(self):
        self.unit.set_guard_gauge(self.old_gauge)

class SetGauge(Action):
    def __init__(self, unit, val):
        self.unit = unit
        self.val = val
        self.old_gauge = unit.get_guard_gauge()

    def do(self):
        self.unit.set_guard_gauge(self.val)

    def reverse(self):
        self.unit.set_guard_gauge(self.old_gauge)

class BuiltGuard(Action):
    '''
    Has its own action because it's only check between turns instead of phases.
    '''
    def __init__(self, unit):
        self.unit = unit

    def do(self):
        self.unit.built_guard = not self.unit.built_guard

    def reverse(self):
        self.unit.built_guard = not self.unit.built_guard

class Transfer(Action):
    def __init__(self, unit, other):
        self.unit = unit
        self.other = other
        self.unit_gauge = unit.get_guard_gauge()
        self.other_gauge = other.get_guard_gauge()

    def do(self):
        if self.unit.traveler:
            skill_system.on_separate(game.get_unit(self.unit.traveler), self.unit)
            self.unit.lead_unit = True # Fixes a bug where a unit that had been lead was transfered and thought they were still lead
            game.get_unit(self.unit.traveler).lead_unit = False
        if self.other.traveler:
            skill_system.on_separate(game.get_unit(self.other.traveler), self.other)

        if self.unit.traveler and self.other.traveler:
            merge = self.unit.get_guard_gauge()//2 + self.other.get_guard_gauge()//2
            self.unit.set_guard_gauge(merge)
            self.other.set_guard_gauge(merge)
        elif self.unit.traveler:
            val = self.unit.get_guard_gauge()//2
            self.unit.set_guard_gauge(val)
            self.other.set_guard_gauge(self.other.get_guard_gauge() + val)
        elif self.other.traveler:
            val = self.other.get_guard_gauge()//2
            self.other.set_guard_gauge(val)
            self.unit.set_guard_gauge(self.unit.get_guard_gauge() + val)

        if self.unit.traveler:
            logging.info(self.unit.traveler + " was paired with " + self.unit.nid + " but transfered to " + self.other.nid)
        else:
            logging.info(self.other.traveler + " was paired with " + self.other.nid + " but transfered to " + self.unit.nid)

        self.unit.traveler, self.other.traveler = self.other.traveler, self.unit.traveler

        if self.other.traveler:
            skill_system.on_pairup(game.get_unit(self.other.traveler), self.other)
        if self.unit.traveler:
            skill_system.on_pairup(game.get_unit(self.unit.traveler), self.unit)

        self.unit.has_given = True

    def reverse(self):
        if self.unit.traveler:
            skill_system.on_separate(game.get_unit(self.unit.traveler), self.unit)
        if self.other.traveler:
            skill_system.on_separate(game.get_unit(self.other.traveler), self.other)

        self.other.traveler, self.unit.traveler = self.unit.traveler, self.other.traveler
        self.unit.has_given = False

        self.unit.set_guard_gauge(self.unit_gauge)
        self.other.set_guard_gauge(self.other_gauge)

        if self.unit.traveler:
            skill_system.on_pairup(game.get_unit(self.unit.traveler), self.unit)
        if self.other.traveler:
            skill_system.on_pairup(game.get_unit(self.other.traveler), self.other)

# === ITEM ACTIONS ==========================================================
class PutItemInConvoy(Action):
    def __init__(self, item, party_nid=None):
        self.item = item
        self.party_nid = party_nid
        self.owner_nid = self.item.owner_nid

    def do(self):
        self.item.change_owner(None)
        party = game.get_party(self.party_nid)
        party.convoy.append(self.item)

    def reverse(self):
        party = game.get_party(self.party_nid)
        party.convoy.remove(self.item)
        self.item.change_owner(self.owner_nid)


class TakeItemFromConvoy(Action):
    def __init__(self, unit, item, party_nid=None):
        self.unit = unit
        self.item = item
        self.party_nid = party_nid

    @recalculate_unit
    def do(self):
        party = game.get_party(self.party_nid)
        party.convoy.remove(self.item)
        self.unit.add_item(self.item)

    @recalculate_unit
    def reverse(self):
        self.unit.remove_item(self.item)
        party = game.get_party(self.party_nid)
        party.convoy.append(self.item)


class RemoveItemFromConvoy(Action):
    def __init__(self, item, party_nid=None):
        self.item = item
        self.party_nid = party_nid

    def do(self):
        party = game.get_party(self.party_nid)
        party.convoy.remove(self.item)

    def reverse(self):
        party = game.get_party(self.party_nid)
        party.convoy.append(self.item)


class MoveItem(Action):
    def __init__(self, owner, unit, item):
        self.owner = owner
        self.unit = unit
        self.item = item

    @recalculate_unit
    def do(self):
        self.owner.remove_item(self.item)
        self.unit.add_item(self.item)

        if self.owner.position and game.tilemap and game.boundary:
            game.boundary.recalculate_unit(self.owner)

    @recalculate_unit
    def reverse(self):
        self.unit.remove_item(self.item)
        self.owner.add_item(self.item)

        if self.owner.position and game.tilemap and game.boundary:
            game.boundary.recalculate_unit(self.owner)


class TradeItemWithConvoy(Action):
    def __init__(self, unit, convoy_item, unit_item):
        self.unit = unit
        self.convoy_item = convoy_item
        self.unit_item = unit_item
        self.unit_idx = self.unit.items.index(self.unit_item)

    @recalculate_unit
    def do(self):
        self.unit.remove_item(self.unit_item)
        game.party.convoy.remove(self.convoy_item)
        game.party.convoy.append(self.unit_item)
        self.unit.insert_item(self.unit_idx, self.convoy_item)

    @recalculate_unit
    def reverse(self):
        self.unit.remove_item(self.convoy_item)
        game.party.convoy.remove(self.unit_item)
        game.party.convoy.append(self.convoy_item)
        self.unit.insert_item(self.unit_idx, self.unit_item)


class GiveItem(Action):
    def __init__(self, unit, item):
        self.unit = unit
        self.item = item

    @recalculate_unit
    def do(self):
        if self.unit.team == 'player' or not item_funcs.inventory_full(self.unit, self.item):
            self.unit.add_item(self.item)

    @recalculate_unit
    def reverse(self):
        if self.item in self.unit.items:
            self.unit.remove_item(self.item)

class DropItem(Action):
    def __init__(self, unit, item):
        self.unit = unit
        self.item = item
        self.is_droppable: bool = item.droppable

    @recalculate_unit
    def do(self):
        self.item.droppable = False
        self.unit.add_item(self.item)

    @recalculate_unit
    def reverse(self):
        self.item.droppable = self.is_droppable
        self.unit.remove_item(self.item)

class MakeItemDroppable(Action):
    def __init__(self, unit, item):
        self.unit = unit
        self.item = item
        self.items = self.unit.items[:]
        self.is_droppable: list = [i.droppable for i in self.items]
        self.was_droppable: bool = item.droppable

    def do(self):
        for item in self.unit.items:
            item.droppable = False
        self.item.droppable = True

    def reverse(self):
        for idx, item in enumerate(self.items):
            item.droppable = self.is_droppable[idx]
        self.item.droppable = self.was_droppable

class SetDroppable(Action):
    def __init__(self, item, value):
        self.item = item
        self.was_droppable = item.droppable
        self.value = value

    def do(self):
        self.item.droppable = self.value

    def reverse(self):
        self.item.droppable = self.was_droppable

class StoreItem(Action):
    def __init__(self, unit, item):
        self.unit = unit
        self.item = item
        self.item_index = self.unit.items.index(self.item)

    @recalculate_unit
    def do(self):
        self.unit.remove_item(self.item)
        game.party.convoy.append(self.item)

    @recalculate_unit
    def reverse(self):
        game.party.convoy.remove(self.item)
        self.unit.insert_item(self.item_index, self.item)

class RemoveItem(StoreItem):
    @recalculate_unit
    def do(self):
        self.unit.remove_item(self.item)

    @recalculate_unit
    def reverse(self):
        self.unit.insert_item(self.item_index, self.item)


class EquipItem(Action):
    def __init__(self, unit, item):
        self.unit = unit
        self.item = item
        if item_system.is_accessory(unit, item):
            self.current_equipped = self.unit.equipped_accessory
        else:
            self.current_equipped = self.unit.equipped_weapon

    def do(self):
        self.unit.equip(self.item)

    def reverse(self):
        self.unit.unequip(self.item)
        if self.current_equipped:
            self.unit.equip(self.current_equipped)


class UnequipItem(Action):
    def __init__(self, unit, item):
        self.unit = unit
        self.item = item
        self.is_equipped_weapon = self.item is self.unit.equipped_weapon
        self.is_equipped_accesory = self.item is self.unit.equipped_accessory

    def do(self):
        if self.is_equipped_weapon or self.is_equipped_accesory:
            self.unit.unequip(self.item)

            # Unequip now auto-equips the next valid item
            all_items = item_funcs.get_all_items(self.unit)
            for item in all_items:
                if item is not self.item and (item_system.is_accessory(self.unit, item) ^ self.is_equipped_weapon):
                    if self.unit.can_equip(item):
                        self.unit.equip(item)
                        break

    def reverse(self):
        if self.is_equipped_weapon or self.is_equipped_accesory:
            self.unit.equip(self.item)


class BringToTopItem(Action):
    """
    Assumes item is in inventory
    """

    def __init__(self, unit, item):
        self.unit = unit
        self.item = item
        self.old_idx = unit.items.index(item)

    def do(self):
        self.unit.bring_to_top_item(self.item)

    def reverse(self):
        self.unit.insert_item(self.old_idx, self.item)


class TradeItem(Action):
    def __init__(self, unit1, unit2, item1, item2):
        self.unit1 = unit1
        self.unit2 = unit2
        self.item1 = item1
        self.item2 = item2
        self.item_index1 = unit1.items.index(item1) if item1 else DB.constants.total_items() - 1
        self.item_index2 = unit2.items.index(item2) if item2 else DB.constants.total_items() - 1

    def swap(self, unit1, unit2, item1, item2, item_index1, item_index2):
        # Do the swap
        if item1:
            unit1.remove_item(item1)
            unit2.insert_item(item_index2, item1)
        if item2:
            unit2.remove_item(item2)
            unit1.insert_item(item_index1, item2)

    def do(self):
        self.swap(self.unit1, self.unit2, self.item1, self.item2, self.item_index1, self.item_index2)

        recalc_unit(self.unit1)
        recalc_unit(self.unit2)

    def reverse(self):
        self.swap(self.unit1, self.unit2, self.item2, self.item1, self.item_index2, self.item_index1)

        recalc_unit(self.unit1)
        recalc_unit(self.unit2)

class RepairItem(Action):
    def __init__(self, item):
        self.item = item
        self.old_uses = self.item.data.get('uses')
        self.old_c_uses = self.item.data.get('c_uses')

    def do(self):
        if self.old_uses is not None and self.item.uses:
            self.item.data['uses'] = self.item.data['starting_uses']
        if self.old_c_uses is not None and self.item.c_uses:
            self.item.data['c_uses'] = self.item.data['starting_c_uses']

    def reverse(self):
        if self.old_uses is not None and self.item.uses:
            self.item.data['uses'] = self.old_uses
        if self.old_c_uses is not None and self.item.c_uses:
            self.item.data['c_uses'] = self.old_c_uses

class ChangeItemName(Action):
    def __init__(self, item, name):
        self.item = item
        self.old_name = item.name
        self.new_name = name

    def do(self):
        self.item.name = self.new_name

    def reverse(self):
        self.item.name = self.old_name

class ChangeItemDesc(Action):
    def __init__(self, item, desc):
        self.item = item
        self.old_desc = item.desc
        self.new_desc = desc

    def do(self):
        self.item.desc = self.new_desc

    def reverse(self):
        self.item.desc = self.old_desc

class AddItemToMultiItem(Action):
    def __init__(self, owner_nid, item, subitem):
        self.owner_nid = owner_nid
        self.item = item
        self.subitem = subitem

    def do(self):
        self.subitem.owner_nid = self.owner_nid
        self.item.subitem_uids.append(self.subitem.uid)
        self.item.subitems.append(self.subitem)
        self.subitem.parent_item = self.item

        unit = game.get_unit(self.owner_nid)
        if unit and unit.position and game.tilemap and game.boundary:
            game.boundary.recalculate_unit(unit)

    def reverse(self):
        self.subitem.owner_nid = None
        self.item.subitem_uids.remove(self.subitem.uid)
        self.item.subitems.remove(self.subitem)
        self.subitem.parent_item = None

        unit = game.get_unit(self.owner_nid)
        if unit and unit.position and game.tilemap and game.boundary:
            game.boundary.recalculate_unit(unit)

class RemoveItemFromMultiItem(Action):
    def __init__(self, owner_nid, item, subitem):
        self.owner_nid = owner_nid
        self.item = item
        self.subitem = subitem

    def do(self):
        self.subitem.owner_nid = None
        self.item.subitem_uids.remove(self.subitem.uid)
        self.item.subitems.remove(self.subitem)
        self.subitem.parent_item = None

        unit = game.get_unit(self.owner_nid)
        if unit and unit.position and game.tilemap and game.boundary:
            game.boundary.recalculate_unit(unit)

    def reverse(self):
        self.subitem.owner_nid = self.owner_nid
        self.item.subitem_uids.append(self.subitem.uid)
        self.item.subitems.append(self.subitem)
        self.subitem.parent_item = self.item

        unit = game.get_unit(self.owner_nid)
        if unit and unit.position and game.tilemap and game.boundary:
            game.boundary.recalculate_unit(unit)

class AddItemComponent(Action):
    def __init__(self, item, component_nid, component_value):
        self.item = item
        self.component_nid = component_nid
        self.component_value = component_value
        self._did_add = False

    def do(self):
        import app.engine.item_component_access as ICA
        self._did_add = False
        component = ICA.restore_component((self.component_nid, self.component_value))
        if not component:
            logging.error("AddItemComponent: Couldn't find item component with nid %s", self.component_nid)
            return
        self.item.components.append(component)
        self.item.__dict__[self.component_nid] = component
        # Assign parent to component
        component.item = self.item
        if component.defines('init'):
            component.init(self.item)
        self._did_add = True

    def reverse(self):
        if self._did_add:
            self.item.components.remove_key(self.component_nid)
            del self.item.__dict__[self.component_nid]
            self._did_add = False

class ModifyItemComponent(Action):
    def __init__(self, item, component_nid, new_component_value, component_property=None, additive: bool = False):
        self.item: ItemObject = item
        self.component_nid = component_nid
        self.property_name: Optional[NID] = None
        self.prev_component_value = None
        self.component_value = None
        if self.component_nid in self.item.components.keys():
            component = self.item.components.get(self.component_nid)
            # @TODO(mag): add validation for this with the cool new validators
            if isinstance(component.value, dict):
                self.property_name = component_property
                self.prev_component_value = component.value[self.property_name]
            else:
                self.prev_component_value = component.value
            if not additive:
                self.component_value = new_component_value
            else:
                self.component_value = self.prev_component_value + new_component_value

    def do(self):
        if self.component_nid in self.item.components.keys():
            component = self.item.components.get(self.component_nid)
            if self.property_name and isinstance(component.value, dict):
                component.value[self.property_name] = self.component_value
            else:
                component.value = self.component_value

    def reverse(self):
        if self.component_nid in self.item.components.keys():
            component = self.item.components.get(self.component_nid)
            if self.property_name and isinstance(self.component.value, dict):
                component.value[self.property_name] = self.prev_component_value
            else:
                component.value = self.prev_component_value

class RemoveItemComponent(Action):
    def __init__(self, item, component_nid):
        self.item = item
        self.component_nid = component_nid
        self.component_value = None
        self._did_remove = False

    def do(self):
        self._did_remove = False
        if self.component_nid in self.item.components.keys():
            component = self.item.components.get(self.component_nid)
            self.component_value = component.value
            self.item.components.remove_key(self.component_nid)
            del self.item.__dict__[self.component_nid]
            self._did_remove = True
        else:
            logging.warning("remove_item_component: component with nid %s not found for item %s", self.component_nid, self.item)

    def reverse(self):
        import app.engine.item_component_access as ICA
        if self._did_remove:
            component = ICA.restore_component((self.component_nid, self.component_value))
            self.item.components.append(component)
            self.item.__dict__[self.component_nid] = component
            # Assign parent to component
            component.item = self.item
            self._did_remove = False

class AddSkillComponent(Action):
    def __init__(self, skill, component_nid, component_value):
        self.skill = skill
        self.component_nid = component_nid
        self.component_value = component_value
        self._did_add = False

    def do(self):
        import app.engine.skill_component_access as SCA
        self._did_add = False
        component = SCA.restore_component((self.component_nid, self.component_value))
        if not component:
            logging.error("AddSkillComponent: Couldn't find skill component with nid %s", self.component_nid)
            return
        self.skill.components.append(component)
        self.skill.__dict__[self.component_nid] = component
        # Assign parent to component
        component.skill = self.skill
        if component.defines('init'):
            component.init(self.skill)
        self._did_add = True

    def reverse(self):
        if self._did_add:
            self.skill.components.remove_key(self.component_nid)
            del self.skill.__dict__[self.component_nid]
            self._did_add = False

class ModifySkillComponent(Action):
    def __init__(self, skill, component_nid, new_component_value, component_property=None, additive: bool = False):
        self.skill: SkillObject = skill
        self.component_nid = component_nid
        self.property_name: Optional[NID] = None
        self.prev_component_value = None
        self.component_value = None
        if self.component_nid in self.skill.components.keys():
            component = self.skill.components.get(self.component_nid)
            if isinstance(component.value, dict):
                self.property_name = component_property
                self.prev_component_value = component.value[self.property_name]
            else:
                self.prev_component_value = component.value
            if not additive:
                self.component_value = new_component_value
            else:
                self.component_value = self.prev_component_value + new_component_value

    def do(self):
        if self.component_nid in self.skill.components.keys():
            component = self.skill.components.get(self.component_nid)
            if self.property_name and isinstance(component.value, dict):
                component.value[self.property_name] = self.component_value
            else:
                component.value = self.component_value

    def reverse(self):
        if self.component_nid in self.skill.components.keys():
            component = self.skill.components.get(self.component_nid)
            if self.property_name and isinstance(self.component.value, dict):
                component.value[self.property_name] = self.prev_component_value
            else:
                component.value = self.prev_component_value

class RemoveSkillComponent(Action):
    def __init__(self, skill, component_nid):
        self.skill = skill
        self.component_nid = component_nid
        self.component_value = None
        self._did_remove = False

    def do(self):
        self._did_remove = False
        if self.component_nid in self.skill.components.keys():
            component = self.skill.components.get(self.component_nid)
            self.component_value = component.value
            self.skill.components.remove_key(self.component_nid)
            del self.skill.__dict__[self.component_nid]
            self._did_remove = True
        else:
            logging.warning("remove_skill_component: component with nid %s not found for skill %s", self.component_nid, self.skill)

    def reverse(self):
        import app.engine.skill_component_access as SCA
        if self._did_remove:
            component = SCA.restore_component((self.component_nid, self.component_value))
            self.skill.components.append(component)
            self.skill.__dict__[self.component_nid] = component
            # Assign parent to component
            component.skill = self.skill
            self._did_remove = False

class SetObjData(Action):
    def __init__(self, obj, keyword, value):
        self.obj = obj
        self.keyword = keyword
        self.value = value
        self.old_value = None

    def do(self):
        if self.keyword in self.obj.data:
            self.old_value = self.obj.data[self.keyword]
            self.obj.data[self.keyword] = self.value

    def reverse(self):
        if self.keyword in self.obj.data:
            self.obj.data[self.keyword] = self.old_value

class SetItemOwner(Action):
    def __init__(self, obj: ItemObject, nid: NID):
        self.obj = obj
        self.new_nid = nid
        self.old_nid = self.obj.owner_nid

    def do(self):
        self.obj.change_owner(self.new_nid)

    def reverse(self):
        self.obj.change_owner(self.old_nid)

class SetSkillOwner(Action):
    def __init__(self, obj: SkillObject, nid: NID):
        self.obj = obj
        self.new_nid = nid
        self.old_nid = self.obj.owner_nid

    def do(self):
        self.obj.owner_nid = self.new_nid

    def reverse(self):
        self.obj.owner_nid = self.old_nid

class GainMoney(Action):
    def __init__(self, party_nid, money):
        self.party_nid = party_nid
        self.money = money
        self.old_money = None

    def do(self):
        party = game.get_party(self.party_nid)
        self.old_money = party.money
        # Can't go below zero
        if party.money + self.money < 0:
            self.money = -party.money
        party.money += self.money

    def reverse(self):
        party = game.get_party(self.party_nid)
        party.money = self.old_money


class GiveBexp(Action):
    def __init__(self, party_nid, bexp):
        self.party_nid = party_nid
        self.bexp = bexp
        self.old_bexp = None

    def do(self):
        party = game.get_party(self.party_nid)
        self.old_bexp = party.bexp
        # Can't go below zero
        if party.bexp + self.bexp < 0:
            self.bexp = -party.bexp
        party.bexp += self.bexp

    def reverse(self):
        party = game.get_party(self.party_nid)
        party.bexp = self.old_bexp


class GainExp(Action):
    def __init__(self, unit, exp_gain):
        self.unit = unit
        self.old_exp = self.unit.exp
        self.exp_gain = exp_gain

    def do(self):
        self.unit.set_exp((self.old_exp + self.exp_gain) % 100)

    def reverse(self):
        self.unit.set_exp(self.old_exp)


class SetExp(GainExp):
    def do(self):
        self.unit.set_exp(self.exp_gain)


class IncLevel(Action):
    """
    Assumes unit did not promote
    """

    def __init__(self, unit):
        self.unit = unit

    def do(self):
        self.unit.level += 1

    def reverse(self):
        self.unit.level -= 1


class SetLevel(Action):
    def __init__(self, unit, level):
        self.unit = unit
        self.old_level = unit.level
        self.new_level = level

    def do(self):
        self.unit.level = self.new_level

    def reverse(self):
        self.unit.level = self.old_level


class AutoLevel(Action):
    def __init__(self, unit, diff, growth_method=None):
        self.unit = unit
        self.diff = diff
        self.old_stats = self.unit.stats.copy()
        self.old_growth_points = self.unit.growth_points.copy()
        self.old_hp = self.unit.get_hp()
        self.old_mana = self.unit.get_mana()
        self.growth_method = growth_method
        self.stat_changes = {}

    def do(self):
        self.stat_changes = unit_funcs.auto_level(self.unit, self.unit.get_internal_level(), self.diff, self.growth_method)

    def reverse(self):
        self.unit.stats = self.old_stats
        self.unit.growth_points = self.old_growth_points
        self.unit.set_hp(self.old_hp)
        self.unit.set_mana(self.old_mana)


class GrowthPointChange(Action):
    def __init__(self, unit, old_growth_points, new_growth_points):
        self.unit = unit
        self.old_growth_points = old_growth_points
        self.new_growth_points = new_growth_points

    def do(self):
        self.unit.growth_points = self.new_growth_points

    def reverse(self):
        self.unit.growth_points = self.old_growth_points


class ApplyStatChanges(Action):
    def __init__(self, unit, stat_changes, increase_current_stats: bool = True):
        self.unit = unit
        self.stat_changes = stat_changes
        self.increase_current_stats = increase_current_stats

        self.current_hp = self.unit.get_hp()
        self.current_mana = self.unit.get_mana()

    def do(self):
        unit_funcs.apply_stat_changes(self.unit, self.stat_changes, self.increase_current_stats)

    def reverse(self):
        negative_changes = {k: -v for k, v in self.stat_changes.items()}
        unit_funcs.apply_stat_changes(self.unit, negative_changes, self.increase_current_stats)
        self.unit.set_hp(self.current_hp)
        self.unit.set_mana(self.current_mana)


class ApplyGrowthChanges(Action):
    def __init__(self, unit, stat_changes):
        self.unit = unit
        self.stat_changes = stat_changes

    def do(self):
        unit_funcs.apply_growth_changes(self.unit, self.stat_changes)

    def reverse(self):
        negative_changes = {k: -v for k, v in self.stat_changes.items()}
        unit_funcs.apply_growth_changes(self.unit, negative_changes)


class ChangeStatCapModifiers(Action):
    def __init__(self, unit, stat_changes):
        self.unit = unit
        self.stat_changes = stat_changes

    def do(self):
        for nid, value in self.stat_changes.items():
            if nid not in self.unit.stat_cap_modifiers:
                self.unit.stat_cap_modifiers[nid] = 0
            self.unit.stat_cap_modifiers[nid] += value

    def reverse(self):
        negative_changes = {k: -v for k, v in self.stat_changes.items()}
        for nid, value in negative_changes.items():
            self.unit.stat_cap_modifiers[nid] += value


class Promote(Action):
    def __init__(self, unit, new_class_nid):
        self.unit = unit
        self.old_exp = self.unit.exp
        self.old_level = self.unit.level
        self.old_klass = self.unit.klass
        self.new_klass = new_class_nid

        self.current_hp = self.unit.get_hp()
        self.current_mana = self.unit.get_mana()

        promotion_gains = DB.classes.get(self.new_klass).promotion
        current_stats = self.unit.stats
        new_klass_maxes = DB.classes.get(self.new_klass).max_stats
        new_klass_bases = DB.classes.get(self.new_klass).bases

        self.stat_changes = {nid: 0 for nid in DB.stats.keys()}
        for stat_nid in DB.stats.keys():
            stat_value = promotion_gains.get(stat_nid, 0)
            if stat_value == -99:  # Just use the new klass base
                self.stat_changes[stat_nid] = new_klass_bases.get(stat_nid, 0) - current_stats[stat_nid]
            elif stat_value == -98:  # Use the new klass base only if it's bigger
                self.stat_changes[stat_nid] = max(0, new_klass_bases.get(stat_nid, 0) - current_stats[stat_nid])
            else:
                max_gain_possible = new_klass_maxes.get(stat_nid, 0) + unit.stat_cap_modifiers.get(stat_nid, 0) - current_stats[stat_nid]
                self.stat_changes[stat_nid] = min(stat_value, max_gain_possible)

        wexp_gain = DB.classes.get(self.new_klass).wexp_gain
        self.new_wexp = {nid: 0 for nid in DB.weapons.keys()}
        for weapon in DB.weapons:
            gain = wexp_gain.get(weapon.nid)
            if gain:
                self.new_wexp[weapon.nid] = gain.wexp_gain

        self.subactions = []

    def get_data(self):
        return self.stat_changes, self.new_wexp

    @recalculate_unit_sprite
    def do(self):
        self.subactions.clear()
        for act in self.subactions:
            act.do()

        self.unit.klass = self.new_klass
        if DB.constants.value('promote_level_reset'):
            self.unit.set_exp(0)
            self.unit.level = 1

        unit_funcs.apply_stat_changes(self.unit, self.stat_changes)

    @recalculate_unit_sprite
    def reverse(self):
        self.unit.klass = self.old_klass
        self.unit.set_exp(self.old_exp)
        self.unit.level = self.old_level

        reverse_stat_changes = {k: -v for k, v in self.stat_changes.items()}
        unit_funcs.apply_stat_changes(self.unit, reverse_stat_changes)
        self.unit.set_hp(self.current_hp)
        self.unit.set_mana(self.current_mana)

        for act in self.subactions:
            act.reverse()
        self.subactions.clear()


class ClassChange(Action):
    def __init__(self, unit, new_class_nid):
        self.unit = unit
        self.old_exp = self.unit.exp
        self.old_level = self.unit.level
        self.old_klass = self.unit.klass
        self.new_klass = new_class_nid

        self.current_hp = self.unit.get_hp()
        self.current_mana = self.unit.get_mana()

        current_stats = self.unit.stats
        old_klass_bases = DB.classes.get(self.old_klass).bases
        new_klass_bases = DB.classes.get(self.new_klass).bases
        new_klass_maxes = DB.classes.get(self.new_klass).max_stats

        self.stat_changes = {nid: 0 for nid in DB.stats.keys()}
        for stat_nid in self.stat_changes.keys():
            change = new_klass_bases.get(stat_nid, 0) - old_klass_bases.get(stat_nid, 0)
            current_stat = current_stats.get(stat_nid)
            new_value = utils.clamp(change, -current_stat, new_klass_maxes.get(stat_nid, 0) + unit.stat_cap_modifiers.get(stat_nid, 0) - current_stat)
            self.stat_changes[stat_nid] = new_value

        wexp_gain = DB.classes.get(self.new_klass).wexp_gain
        self.new_wexp = {nid: 0 for nid in DB.weapons.keys()}
        for weapon_nid in self.new_wexp.keys():
            weapon_info = wexp_gain.get(weapon_nid, DB.weapons.default(DB))
            self.new_wexp[weapon_nid] = weapon_info.wexp_gain

        self.subactions = []

    def get_data(self):
        return self.stat_changes, self.new_wexp

    @recalculate_unit_sprite
    def do(self):
        self.subactions.clear()
        for act in self.subactions:
            act.do()

        self.unit.klass = self.new_klass

        if DB.constants.value('class_change_level_reset'):
            self.unit.set_exp(0)
            self.unit.level = 1

        unit_funcs.apply_stat_changes(self.unit, self.stat_changes)

    @recalculate_unit_sprite
    def reverse(self):
        self.unit.klass = self.old_klass

        self.unit.set_exp(self.old_exp)
        self.unit.level = self.old_level

        reverse_stat_changes = {k: -v for k, v in self.stat_changes.items()}
        unit_funcs.apply_stat_changes(self.unit, reverse_stat_changes)
        self.unit.set_hp(self.current_hp)
        self.unit.set_mana(self.current_mana)

        for act in self.subactions:
            act.reverse()
        self.subactions.clear()

class GainWexp(Action):
    """
    # Given a unit and an item, gain some of amount of weapon experience for that item's weapon type
    """
    def __init__(self, unit, item, wexp_gain):
        self.unit = unit
        self.item = item
        self.wexp_gain = wexp_gain

    def increase_wexp(self) -> Tuple[int, int]:
        weapon_type = item_system.weapon_type(self.unit, self.item)
        if not weapon_type:
            return 0, 0
        wexp_cap = unit_funcs.get_weapon_cap(self.unit, weapon_type)
        old_value = self.unit.wexp[weapon_type]
        self.unit.wexp[weapon_type] += self.wexp_gain
        self.unit.wexp[weapon_type] = utils.clamp(self.unit.wexp[weapon_type], 0, wexp_cap)
        return old_value, self.unit.wexp[weapon_type]

    def do(self):
        self.old_value, self.current_value = self.increase_wexp()
        if self.current_value > self.old_value:
            for weapon_rank in reversed(DB.weapon_ranks):
                if self.old_value < weapon_rank.requirement and self.current_value >= weapon_rank.requirement:
                    weapon_type = item_system.weapon_type(self.unit, self.item)
                    game.alerts.append(banner.GainWexp(self.unit, weapon_rank.rank, weapon_type))
                    game.state.change('alert')
                    break

    def execute(self):
        self.old_value, self.current_value = self.increase_wexp()

    def reverse(self):
        weapon_type = item_system.weapon_type(self.unit, self.item)
        if not weapon_type:
            return
        self.unit.wexp[weapon_type] = self.old_value


class AddWexp(Action):
    """
    # Given a unit and a weapon type, gain some of amount of weapon experience
    """
    def __init__(self, unit, weapon_type, wexp_gain):
        self.unit = unit
        self.weapon_type = weapon_type
        self.wexp_gain = wexp_gain

    def increase_wexp(self):
        wexp_cap = unit_funcs.get_weapon_cap(self.unit, self.weapon_type)
        old_value = self.unit.wexp[self.weapon_type]
        self.unit.wexp[self.weapon_type] += self.wexp_gain
        self.unit.wexp[self.weapon_type] = utils.clamp(self.unit.wexp[self.weapon_type], 0, wexp_cap)
        return old_value, self.unit.wexp[self.weapon_type]

    def do(self):
        self.old_value, self.current_value = self.increase_wexp()
        if self.current_value > self.old_value:
            for weapon_rank in reversed(DB.weapon_ranks):
                if self.old_value < weapon_rank.requirement and self.current_value >= weapon_rank.requirement:
                    game.alerts.append(banner.GainWexp(self.unit, weapon_rank.rank, self.weapon_type))
                    game.state.change('alert')
                    break

    def execute(self):
        self.old_value, self.current_value = self.increase_wexp()

    def reverse(self):
        self.unit.wexp[self.weapon_type] = self.old_value


class SetWexp(Action):
    """
    # Given a unit and a weapon type, set their wexp to a certain value
    """
    def __init__(self, unit, weapon_type, wexp):
        self.unit = unit
        self.weapon_type = weapon_type
        self.old_wexp = self.unit.wexp[self.weapon_type]
        wexp_cap = unit_funcs.get_weapon_cap(self.unit, self.weapon_type)
        self.wexp = utils.clamp(wexp, 0, wexp_cap)

    def do(self):
        self.unit.wexp[self.weapon_type] = self.wexp
        if self.wexp > self.old_wexp:
            for weapon_rank in reversed(DB.weapon_ranks):
                if self.old_wexp < weapon_rank.requirement and self.wexp >= weapon_rank.requirement:
                    game.alerts.append(banner.GainWexp(self.unit, weapon_rank.rank, self.weapon_type))
                    game.state.change('alert')
                    break

    def execute(self):
        self.unit.wexp[self.weapon_type] = self.wexp

    def reverse(self):
        self.unit.wexp[self.weapon_type] = self.old_wexp


class ChangeHP(Action):
    def __init__(self, unit, num):
        self.unit = unit
        self.num = num
        self.old_hp = self.unit.get_hp()

    def do(self):
        self.old_hp = self.unit.get_hp()
        self.unit.set_hp(self.old_hp + self.num)

    def reverse(self):
        self.unit.set_hp(self.old_hp)

class SetName(Action):
    def __init__(self, unit, new_name):
        self.unit = unit
        self.new_name = new_name
        self.old_name = self.unit.name

    def do(self):
        self.unit.name = self.new_name

    def reverse(self):
        self.unit.name = self.old_name

class SetNid(Action):
    """Changes the NID of a UnitObject.

    This is extremely dangerous, and should only be used for
    converting generic NIDs into unique NIDs. That is why
    we only allow this operation to be carried out on generics.
    """
    def __init__(self, unit, new_nid):
        self.unit = unit
        self.new_nid = new_nid
        self.old_nid = self.unit.nid

    def do(self):
        if self.unit.generic:
            # Leave and arrive are necessary because the game
            # board has references to unit's by their nid
            if self.unit.position:
                game.leave(self.unit)
            if self.unit.nid in game.unit_registry:
                del game.unit_registry[self.unit.nid]
            self.unit.nid = self.new_nid
            game.register_unit(self.unit)
            if self.unit.position:
                game.arrive(self.unit)

    def reverse(self):
        if self.unit.generic:
            if self.unit.position:
                game.leave(self.unit)
            if self.unit.nid in game.unit_registry:
                del game.unit_registry[self.unit.nid]
            self.unit.nid = self.old_nid
            game.register_unit(self.unit)
            if self.unit.position:
                game.arrive(self.unit)

class SetHP(Action):
    def __init__(self, unit, new_hp):
        self.unit = unit
        self.new_hp = new_hp
        self.old_hp = self.unit.get_hp()

    def do(self):
        self.unit.set_hp(self.new_hp)

    def reverse(self):
        self.unit.set_hp(self.old_hp)

class ChangeMana(Action):
    def __init__(self, unit, num):
        self.unit = unit
        self.num = num
        self.old_mana = self.unit.get_mana()

    def do(self):
        self.unit.set_mana(self.old_mana + self.num)

    def reverse(self):
        self.unit.set_mana(self.old_mana)

class SetMana(Action):
    def __init__(self, unit, new_mana):
        self.unit = unit
        self.new_mana = new_mana
        self.old_mana = self.unit.get_mana()

    def do(self):
        self.unit.set_mana(self.new_mana)

    def reverse(self):
        self.unit.set_mana(self.old_mana)

class ChangeFatigue(Action):
    def __init__(self, unit, num):
        self.unit = unit
        self.num = num
        self.old_fatigue = self.unit.get_fatigue()
        self.subactions = []

    def do(self):
        self.subactions.clear()
        if skill_system.ignore_fatigue(self.unit):
            return

        self.unit.set_fatigue(self.old_fatigue + self.num)

        if game.game_vars.get('_fatigue') == 2:
            if 'Fatigued' in DB.skills.keys():
                if self.unit.get_fatigue() >= self.unit.get_max_fatigue():
                    self.subactions.append(AddSkill(self.unit, 'Fatigued', source='game', source_type=SourceType.FATIGUE))
                elif 'Fatigued' in [skill.nid for skill in self.unit.skills]:
                    self.subactions.append(RemoveSkill(self.unit, 'Fatigued', source='game', source_type=SourceType.FATIGUE))
            if 'Rested' in DB.skills.keys():
                if self.unit.get_fatigue() < self.unit.get_max_fatigue():
                    self.subactions.append(AddSkill(self.unit, 'Rested', source='game', source_type=SourceType.FATIGUE))
                elif 'Rested' in [skill.nid for skill in self.unit.skills]:
                    self.subactions.append(RemoveSkill(self.unit, 'Rested', source='game', source_type=SourceType.FATIGUE))

        for action in self.subactions:
            action.do()

    def reverse(self):
        for action in self.subactions:
            action.reverse()
        self.unit.set_fatigue(self.old_fatigue)

class ChangeField(Action):
    def __init__(self, unit: UnitObject, key: str, value: str | float | int, should_increment: bool = False):
        self.unit = unit
        self.value = value
        self.key = key
        self.old_value = self.unit._fields.get(key, '')
        self.should_increment = should_increment

    def do(self):
        if self.should_increment and not isinstance(self.value, str):
            if self.key in self.unit._fields and not isinstance(self.unit._fields[self.key], str):
                self.unit.set_field(self.key, self.unit.get_field(self.key) + self.value)
                return
        self.unit.set_field(self.key, self.value)

    def reverse(self):
        self.unit.set_field(self.key, self.old_value)

class SetUnitNote(Action):
    def __init__(self, unit: UnitObject, key: str, value: str):
        self.unit = unit
        self.key = key
        self.value = value
        categories = [cat for cat, note in self.unit.notes]
        if key in categories:
            self.idx = categories.index(key)
            self.old_note = self.unit.notes[self.idx]
        else:
            self.idx = None
            self.old_note = None

    def do(self):
        if self.idx is not None:
            self.unit.notes.pop(self.idx)
            self.unit.notes.insert((self.key, self.value))
        else:
            self.unit.notes.append((self.key, self.value))

    def reverse(self):
        if self.old_note:
            self.unit.notes.pop(self.idx)
            self.unit.notes.insert(self.old_note)
        else:
            self.unit.notes.pop()

class RemoveUnitNote(Action):
    def __init__(self, unit: UnitObject, key: str):
        self.unit = unit
        self.key = key
        categories = [cat for cat, note in self.unit.notes]
        if key in categories:
            self.deletion_idx = categories.index(key)
            self.old_note = self.unit.notes[self.deletion_idx]
        else:
            self.deletion_idx = None
            self.old_note = None        

    def do(self):
        if self.deletion_idx is not None:
            self.unit.notes.pop(self.deletion_idx)

    def reverse(self):
        if self.old_note:
            self.unit.notes.insert(self.deletion_idx, self.old_note)

class Die(Action):
    def __init__(self, unit):
        self.unit = unit
        self.old_pos = unit.position
        self.leave_map = LeaveMap(self.unit)
        if DB.support_constants.value('break_supports_on_death') and game.current_mode.permadeath:
            self.lock_all_support_ranks = \
                [LockAllSupportRanks(pair.nid) for pair in game.supports.get_pairs(self.unit.nid)]
        else:
            self.lock_all_support_ranks = []
        self.drop = None

        self.initiative_action = None
        if DB.constants.value('initiative'):
            self.initiative_action = RemoveInitiative(self.unit)

    def do(self):
        if self.unit.traveler:
            drop_me = game.get_unit(self.unit.traveler)
            if DB.constants.value('pairup'):
                self.drop = Separate(self.unit, drop_me, self.unit.position)
            else:
                self.drop = Drop(self.unit, drop_me, self.unit.position)
            self.drop.do()
            if game.phase.get_current() != drop_me.team:
                Reset(drop_me).do()
            # TODO Drop Sound

        if DB.constants.value('initiative') and self.initiative_action:
            self.initiative_action.do()

        self.leave_map.do()
        for act in self.lock_all_support_ranks:
            act.do()
        self.unit.dead = True
        self.unit.is_dying = False

    def reverse(self):
        # Remember who was resurrected briefly
        if '_resurrect' not in game.level_vars:
            game.level_vars['_resurrect'] = set()
        game.level_vars['_resurrect'].add(self.unit.nid)

        self.unit.dead = False
        self.unit.sprite.set_transition('normal')
        self.unit.sprite.change_state('normal')

        if DB.constants.value('initiative') and self.initiative_action:
            self.initiative_action.reverse()

        for act in self.lock_all_support_ranks:
            act.reverse()
        self.leave_map.reverse()
        if self.drop:
            self.drop.reverse()

class Resurrect(Action):
    def __init__(self, unit):
        self.unit = unit
        self.old_dead = self.unit.dead

    def do(self):
        self.unit.dead = False
        self.unit.sprite.set_transition('normal')
        self.unit.sprite.change_state('normal')

    def reverse(self):
        self.unit.dead = self.old_dead


class UpdateRecords(Action):
    def __init__(self, record_type, data):
        self.record_type = record_type
        self.data = data

    def do(self):
        game.records.append(self.record_type, self.data)

    def reverse(self):
        game.records.pop(self.record_type)


class ReverseRecords(Action):
    def __init__(self, record_type, data):
        self.record_type = record_type
        self.data = data

    def do(self):
        game.records.pop(self.record_type)

    def reverse(self):
        game.records.append(self.record_type, self.data)


class IncrementSupportPoints(Action):
    def __init__(self, nid, points):
        self.nid = nid
        self.inc = points

        if self.nid not in game.supports.support_pairs:
            game.supports.create_pair(self.nid)
        pair = game.supports.support_pairs[self.nid]
        self.saved_data = pair.save()

    def do(self):
        pair = game.supports.support_pairs[self.nid]
        pair.increment_points(self.inc)

    def reverse(self):
        pair = game.supports.support_pairs[self.nid]
        pair.points = int(self.saved_data['points'])
        pair.locked_ranks = self.saved_data['locked_ranks']
        pair.points_gained_this_chapter = int(self.saved_data['points_gained_this_chapter'])
        pair.ranks_gained_this_chapter = int(self.saved_data['ranks_gained_this_chapter'])


class UnlockSupportRank(Action):
    def __init__(self, nid, rank):
        self.nid = nid
        self.rank = rank
        self.was_locked: bool = False
        if self.nid not in game.supports.support_pairs:
            game.supports.create_pair(self.nid)

    def do(self):
        self.was_locked = False
        pair = game.supports.support_pairs[self.nid]
        if self.rank in pair.locked_ranks:
            self.was_locked = True
            pair.locked_ranks.remove(self.rank)
        if self.rank not in pair.unlocked_ranks:
            pair.unlocked_ranks.append(self.rank)

    def reverse(self):
        pair = game.supports.support_pairs[self.nid]
        if self.rank in pair.unlocked_ranks:
            pair.unlocked_ranks.remove(self.rank)
        if self.was_locked and self.rank not in pair.locked_ranks:
            pair.locked_ranks.append(self.rank)


class DisableSupportRank(Action):
    def __init__(self, nid, rank):
        self.nid = nid
        self.rank = rank
        self.was_unlocked: bool = False
        if self.nid not in game.supports.support_pairs:
            game.supports.create_pair(self.nid)
        pair = game.supports.support_pairs[self.nid]
        self.locked_ranks = pair.locked_ranks[:]
        self.unlocked_ranks = pair.unlocked_ranks[:]

    def do(self):
        pair = game.supports.support_pairs[self.nid]
        if self.rank in pair.unlocked_ranks:
            pair.unlocked_ranks.remove(self.rank)
        if self.rank in pair.locked_ranks:
            pair.locked_ranks.remove(self.rank)

    def reverse(self):
        pair = game.supports.support_pairs[self.nid]
        pair.locked_ranks = self.locked_ranks
        pair.unlocked_ranks = self.unlocked_ranks


class LockAllSupportRanks(Action):
    """
    Done on death of a unit in the pair
    To free up slots for other units
    """

    def __init__(self, nid):
        self.nid = nid
        if self.nid not in game.supports.support_pairs:
            game.supports.create_pair(self.nid)
        pair = game.supports.support_pairs[self.nid]
        self.unlocked_ranks = pair.unlocked_ranks[:]

    def do(self):
        pair = game.supports.support_pairs[self.nid]
        for rank in pair.unlocked_ranks:
            pair.locked_ranks.append(rank)
        pair.unlocked_ranks.clear()

    def reverse(self):
        pair = game.supports.support_pairs[self.nid]
        for rank in self.unlocked_ranks:
            if rank in pair.locked_ranks:
                pair.locked_ranks.remove(rank)
        pair.unlocked_ranks = self.unlocked_ranks


class ChangeAI(Action):
    def __init__(self, unit, ai):
        self.unit = unit
        self.ai = ai
        self.old_ai = self.unit.ai

    @recalculate_unit
    def do(self):
        self.unit.ai = self.ai

    @recalculate_unit
    def reverse(self):
        self.unit.ai = self.old_ai


class ChangeRoamAI(Action):
    def __init__(self, unit, ai):
        self.unit = unit
        self.roam_ai = ai
        self.old_ai = self.unit.roam_ai
        self._added_to_roam_ai = False

    @recalculate_unit
    def do(self):
        self.unit.roam_ai = self.roam_ai
        for s in game.state.state:
            if s.name == 'free_roam' and not s.contains_ai_unit(self.unit):
                s.add_ai_unit(self.unit)
                self._added_to_roam_ai = True

    @recalculate_unit
    def reverse(self):
        self.unit.roam_ai = self.old_ai
        if self._added_to_roam_ai:
            for s in game.state.state:
                if s.name == 'free_roam' and s.contains_ai_unit(self.unit):
                    s.remove_ai_unit(self.unit)
        self._added_to_roam_ai = False  # Reset for later


class ChangeAIGroup(Action):
    def __init__(self, unit, ai_group):
        self.unit = unit
        self.ai_group = ai_group
        self.old_ai = self.unit.ai_group

    def do(self):
        self.unit.ai_group = self.ai_group

    def reverse(self):
        self.unit.ai_group = self.old_ai_group


class AIGroupPing(Action):
    def __init__(self, ai_group: NID):
        self.ai_group = ai_group
        self.old_active = game.ai_group_active(self.ai_group)

    def do(self):
        ai_group = game.get_ai_group(self.ai_group)
        ai_group.active = True

    def reverse(self):
        ai_group = game.get_ai_group(self.ai_group)
        ai_group.active = self.old_active


class ChangeParty(Action):
    def __init__(self, unit, party_nid: str):
        self.unit = unit
        self.party_nid = party_nid
        self.old_party_nid = self.unit.party

    def do(self):
        self.unit.party = self.party_nid

    def reverse(self):
        self.unit.party = self.old_party_nid


class ChangeFaction(Action):
    def __init__(self, unit, faction_nid: str):
        self.unit = unit
        self.faction_nid = faction_nid
        self.old_faction_nid = self.unit.faction
        self.old_name = self.unit.name
        self.old_desc = self.unit.desc

    def do(self):
        self.unit.faction = self.faction_nid
        if self.unit.generic:
            self.unit.name = DB.factions.get(self.faction_nid).name
            self.unit.desc = DB.factions.get(self.faction_nid).desc

    def reverse(self):
        self.unit.faction = self.old_faction_nid
        self.unit.name = self.old_name
        self.unit.desc = self.old_desc


class ChangeTeam(Action):
    def __init__(self, unit, team):
        self.unit = unit
        self.team = team
        self.old_team = self.unit.team
        self.action = Reset(self.unit)
        self.ai_action = ChangeAI(self.unit, 'None')
        self.fog_action1 = UpdateFogOfWar(self.unit)
        self.fog_action2 = UpdateFogOfWar(self.unit)

    def do(self):
        true_pos = self.unit.position
        if self.unit.position:
            game.leave(self.unit)
        self.unit.position = None  # Remove from map so update fog of war will remove from map
        self.fog_action1.do()  # Remove unit from the fog grid for its old team
        self.unit.team = self.team
        self.action.do()
        if self.team == 'player':
            # Make sure player unit's don't keep their AI
            self.ai_action.do()
        self.unit.position = true_pos  # Add unit back to map
        if self.unit.position:
            game.arrive(self.unit)
        self.fog_action2.do()  # Add to fog of war with new team
        if game.boundary:
            game.boundary.reset_unit(self.unit)
        self.unit.sprite.load_sprites()

    def reverse(self):
        true_pos = self.unit.position
        if self.unit.position:
            game.leave(self.unit)
        self.unit.position = None
        self.fog_action2.reverse()  # Remove new team's FOW
        self.unit.team = self.old_team
        if self.team == 'player':
            self.ai_action.reverse()
        self.action.reverse()
        self.fog_action1.reverse()  # Put unit back onto map
        self.unit.position = true_pos
        if self.unit.position:
            game.arrive(self.unit)
        if game.boundary:
            game.boundary.reset_unit(self.unit)
        self.unit.sprite.load_sprites()


class ChangePortrait(Action):
    def __init__(self, unit, portrait_nid):
        self.unit = unit
        self.old_portrait = self.unit.portrait_nid
        self.new_portrait = portrait_nid

    def do(self):
        self.unit.portrait_nid = self.new_portrait

    def reverse(self):
        self.unit.portrait_nid = self.old_portrait

class ChangeUnitDesc(Action):
    def __init__(self, unit, desc):
        self.unit = unit
        self.old_desc = unit.desc
        self.new_desc = desc

    def do(self):
        self.unit.desc = self.new_desc

    def reverse(self):
        self.unit.desc = self.old_desc

class ChangeAffinity(Action):
    def __init__(self, unit, affinity):
        self.unit = unit
        self.old_affinity = unit.affinity
        self.new_affinity = affinity

    def do(self):
        self.unit.affinity = self.new_affinity

    def reverse(self):
        self.unit.affinity = self.old_affinity

class AddTag(Action):
    def __init__(self, unit, tag):
        self.unit = unit
        self.tag = tag

    def do(self):
        self.unit._tags.append(self.tag)

    def reverse(self):
        if self.tag in self.unit._tags:
            self.unit._tags.remove(self.tag)


class RemoveTag(Action):
    def __init__(self, unit, tag):
        self.unit = unit
        self.tag = tag
        self.did_remove = False

    def do(self):
        if self.tag in self.unit._tags:
            self.unit._tags.remove(self.tag)
            self.did_remove = True

    def reverse(self):
        if self.did_remove:
            self.unit._tags.append(self.tag)


class AddTalk(Action):
    def __init__(self, unit1_nid, unit2_nid):
        self.unit1 = unit1_nid
        self.unit2 = unit2_nid

    def do(self):
        game.talk_options.append((self.unit1, self.unit2))

    def reverse(self):
        if (self.unit1, self.unit2) in game.talk_options:
            game.talk_options.remove((self.unit1, self.unit2))


class RemoveTalk(Action):
    def __init__(self, unit1_nid, unit2_nid):
        self.unit1 = unit1_nid
        self.unit2 = unit2_nid
        self.did_remove = False

    def do(self):
        if (self.unit1, self.unit2) in game.talk_options:
            game.talk_options.remove((self.unit1, self.unit2))
            self.did_remove = True

    def reverse(self):
        if self.did_remove:
            game.talk_options.append((self.unit1, self.unit2))


class AddLore(Action):
    def __init__(self, lore_nid):
        self.lore_nid = lore_nid

    def do(self):
        game.unlocked_lore.append(self.lore_nid)

    def reverse(self):
        if self.lore_nid in game.unlocked_lore:
            game.unlocked_lore.remove(self.lore_nid)


class RemoveLore(Action):
    def __init__(self, lore_nid):
        self.lore_nid = lore_nid
        self.did_remove = False

    def do(self):
        if self.lore_nid in game.unlocked_lore:
            game.unlocked_lore.remove(self.lore_nid)
            self.did_remove = True

    def reverse(self):
        if self.did_remove:
            game.unlocked_lore.append(self.lore_nid)


class LogDialog(Action):
    def __init__(self, speaker: str, plain_text: str):
        self.speaker = speaker
        self.plain_text = plain_text

    def do(self):
        game.dialog_log.append((self.speaker, self.plain_text))

    def reverse(self):
        game.dialog_log.pop()

class SetGameBoardBounds(Action):
    def __init__(self, bounds):
        self.old_bounds = game.board.bounds
        self.new_bounds = bounds

    def do(self):
        game.board.set_bounds(*self.new_bounds)

    def reverse(self):
        game.board.set_bounds(*self.old_bounds)

def _region_leave(region):
    # Force all affected units to leave
    region_positions = region.get_all_positions()
    for unit in game.units:
        if unit.position in region_positions:
            game.leave(unit)

def _region_arrive(region):
    # Force all affected units to arrive
    region_positions = region.get_all_positions()
    for unit in game.units:
        if unit.position in region_positions:
            game.arrive(unit)

class AddRegion(Action):
    def __init__(self, region):
        self.region = region
        self.did_add = False
        self.subactions = []

    def do(self):
        self.subactions.clear()
        if self.region.nid in game.level.regions:
            logging.warning("AddRegion Action: RegionObject with nid %s already in level", self.region.nid)
        else:
            if self.region.region_type == RegionType.TERRAIN:
                _region_leave(self.region)

            game.get_region_under_pos.cache_clear()
            game.level.regions.append(self.region)
            self.did_add = True
            # Remember to add the status from the unit
            if self.region.region_type == RegionType.STATUS:
                for unit in game.units:
                    if unit.position and self.region.contains(unit.position):
                        # add region status does the action, we just need to remember it here
                        add_skill_action = game.add_region_status(unit, self.region, False)
                        if add_skill_action:
                            self.subactions.append(add_skill_action)

            # Reset movement and opacity grids
            elif self.region.region_type == RegionType.TERRAIN:
                for position in self.region.get_all_positions():
                    game.board.reset_pos(game.level.tilemap, position)
                game.boundary.reset()
                _region_arrive(self.region)

            # Update fog of war if appropriate
            elif self.region.region_type == RegionType.FOG:
                update_fow_action = AddFogRegion(self.region)
                update_fow_action.do()
                self.subactions.append(update_fow_action)
            elif self.region.region_type == RegionType.VISION:
                update_fow_action = AddVisionRegion(self.region)
                update_fow_action.do()
                self.subactions.append(update_fow_action)

    def reverse(self):
        if self.did_add:
            if self.region.region_type == RegionType.TERRAIN:
                _region_leave(self.region)

            for act in self.subactions:
                act.reverse()
            game.get_region_under_pos.cache_clear()
            game.level.regions.delete(self.region)

            # Reset movement and opacity grids
            if self.region.region_type == RegionType.TERRAIN:
                for position in self.region.get_all_positions():
                    game.board.reset_pos(game.level.tilemap, position)
                game.boundary.reset()
                _region_arrive(self.region)

class ChangeRegionCondition(Action):
    def __init__(self, region, condition):
        self.region = region
        self.old_condition = self.region.condition
        self.new_condition = condition

    def do(self):
        self.region.condition = self.new_condition

    def reverse(self):
        self.region.condition = self.old_condition

class DecrementTimeRegion(Action):
    def __init__(self, region):
        self.region = region

    def do(self):
        self.region.time_left = int(self.region.time_left) - 1

    def reverse(self):
        self.region.time_left = int(self.region.time_left) + 1

class RemoveRegion(Action):
    def __init__(self, region):
        self.region = region
        self.did_remove = False
        self.subactions = []

    def do(self):
        self.subactions.clear()
        if self.region.nid in game.level.regions.keys():
            # Remember to remove the status from the unit
            if self.region.region_type == RegionType.STATUS:
                for unit in game.units:
                    if unit.position and self.region.contains(unit.position):
                        self.subactions.append(RemoveSkill(unit, self.region.sub_nid, source=self.region.nid, source_type=SourceType.REGION))

            # Update fog of war if appropriate
            elif self.region.region_type == RegionType.FOG:
                update_fow_action = RemoveFogRegion(self.region)
                self.subactions.append(update_fow_action)
            elif self.region.region_type == RegionType.VISION:
                update_fow_action = RemoveVisionRegion(self.region)
                self.subactions.append(update_fow_action)

            if self.region.region_type == RegionType.TERRAIN:
                _region_leave(self.region)

            for act in self.subactions:
                act.do()

            game.get_region_under_pos.cache_clear()
            game.level.regions.delete(self.region)
            self.did_remove = True

            # Reset movement and opacity grids
            if self.region.region_type == RegionType.TERRAIN:
                for position in self.region.get_all_positions():
                    game.board.reset_pos(game.level.tilemap, position)
                game.boundary.reset()
                _region_arrive(self.region)
        else:
            logging.error("RemoveRegion Action: Could not find region with nid %s", self.region.nid)

    def reverse(self):
        if self.did_remove:
            if self.region.region_type == RegionType.TERRAIN:
                _region_leave(self.region)

            game.get_region_under_pos.cache_clear()
            game.level.regions.append(self.region)

            for act in self.subactions:
                act.reverse()

            # Reset movement and opacity girds
            if self.region.region_type == RegionType.TERRAIN:
                for position in self.region.get_all_positions():
                    game.board.reset_pos(game.level.tilemap, position)
                game.boundary.reset()
                _region_arrive(self.region)

class AddFogRegion(Action):
    def __init__(self, region):
        self.region = region

    def do(self):
        game.board.add_fog_region(self.region)
        game.boundary.reset_fog_of_war()

    def reverse(self):
        game.board.remove_fog_region(self.region)
        game.boundary.reset_fog_of_war()

class RemoveFogRegion(Action):
    def __init__(self, region):
        self.region = region

    def do(self):
        game.board.remove_fog_region(self.region)
        game.boundary.reset_fog_of_war()

    def reverse(self):
        game.board.add_fog_region(self.region)
        game.boundary.reset_fog_of_war()

class AddVisionRegion(Action):
    def __init__(self, region):
        self.region = region

    def do(self):
        game.board.add_vision_region(self.region)
        game.boundary.reset_fog_of_war()

    def reverse(self):
        game.board.remove_vision_region(self.region)
        game.boundary.reset_fog_of_war()

class RemoveVisionRegion(Action):
    def __init__(self, region):
        self.region = region

    def do(self):
        game.board.remove_vision_region(self.region)
        game.boundary.reset_fog_of_war()

    def reverse(self):
        game.board.add_vision_region(self.region)
        game.boundary.reset_fog_of_war()

def _leave(layer):
    # Force all affected units to leave
    layer_positions = layer.terrain.keys()
    for unit in game.units:
        if unit.position in layer_positions:
            game.leave(unit)

def _arrive(layer):
    # Force all affected units to arrive
    layer_positions = layer.terrain.keys()
    for unit in game.units:
        if unit.position in layer_positions:
            game.arrive(unit)

class ShowLayer(Action):
    def __init__(self, layer_nid, transition):
        self.layer_nid = layer_nid
        self.transition = transition

    def do(self):
        layer = game.level.tilemap.layers.get(self.layer_nid)
        _leave(layer)
        # Actually change the layer
        if self.transition == 'immediate':
            layer.quick_show()
            game.level.tilemap.reset()
        else:
            layer.show()
        _arrive(layer)
        game.board.reset_tile_grids(game.level.tilemap)
        game.boundary.reset()

    def execute(self):
        layer = game.level.tilemap.layers.get(self.layer_nid)
        _leave(layer)
        layer.quick_show()
        game.level.tilemap.reset()
        _arrive(layer)
        game.board.reset_tile_grids(game.level.tilemap)
        game.boundary.reset()

    def reverse(self):
        layer = game.level.tilemap.layers.get(self.layer_nid)
        _leave(layer)
        layer.quick_hide()
        game.level.tilemap.reset()
        _arrive(layer)
        game.board.reset_tile_grids(game.level.tilemap)
        game.boundary.reset()


class HideLayer(Action):
    def __init__(self, layer_nid, transition):
        self.layer_nid = layer_nid
        self.transition = transition

    def do(self):
        layer = game.level.tilemap.layers.get(self.layer_nid)
        _leave(layer)
        if self.transition == 'immediate':
            layer.quick_hide()
            game.level.tilemap.reset()
        else:
            layer.hide()
        _arrive(layer)
        game.board.reset_tile_grids(game.level.tilemap)
        game.boundary.reset()

    def execute(self):
        layer = game.level.tilemap.layers.get(self.layer_nid)
        _leave(layer)
        layer.quick_hide()
        game.level.tilemap.reset()
        _arrive(layer)
        game.board.reset_tile_grids(game.level.tilemap)
        game.boundary.reset()

    def reverse(self):
        layer = game.level.tilemap.layers.get(self.layer_nid)
        _leave(layer)
        layer.quick_show()
        game.level.tilemap.reset()
        _arrive(layer)
        game.board.reset_tile_grids(game.level.tilemap)
        game.boundary.reset()

class ChangeBGTileMap(Action):
    def __init__(self, new_tilemap):
        self.new_tilemap = new_tilemap
        self.old_tilemap = game.bg_tilemap

    def do(self):
        game.level.bg_tilemap = self.new_tilemap

    def reverse(self):
        game.level.bg_tilemap = self.old_tilemap

class AddWeather(Action):
    def __init__(self, weather_nid, position):
        self.weather_nid = weather_nid
        self.position = position

    def do(self):
        new_ps = particles.create_system(self.weather_nid, game.tilemap.width, game.tilemap.height, self.position)
        game.tilemap.weather.append(new_ps)

    def reverse(self):
        bad_weather = [ps for ps in game.tilemap.weather if ps.nid == self.weather_nid and ps.pos == self.position]
        if bad_weather:
            game.tilemap.weather.remove(bad_weather[0])


class RemoveWeather(Action):
    def __init__(self, weather_nid, position):
        self.weather_nid = weather_nid
        self.position = position
        self.did_remove = False

    def do(self):
        bad_weather = [ps for ps in game.tilemap.weather if ps.nid == self.weather_nid and ps.pos == self.position]
        if bad_weather:
            game.tilemap.weather.remove(bad_weather[0])
            self.did_remove = True

    def reverse(self):
        if self.did_remove:
            new_ps = particles.create_system(self.weather_nid, game.tilemap.width, game.tilemap.height, self.position)
            game.tilemap.weather.append(new_ps)

class AddMapAnim(Action):
    def __init__(self, nid: NID, pos: Tuple[int, int], speed_mult: float, blend_mode: engine.BlendMode, upper_layer: bool=False):
        self.nid: NID = nid
        self.pos: Tuple[int, int] = pos
        self.speed_mult: float = speed_mult
        self.blend_mode: engine.BlendMode = blend_mode
        self.is_upper_layer: bool = upper_layer

    def do(self):
        anim = RESOURCES.animations.get(self.nid)
        anim = animations.MapAnimation(anim, self.pos, loop=True, speed_adj=self.speed_mult)
        anim.set_tint(self.blend_mode)
        if self.is_upper_layer:
            game.tilemap.high_animations.append(anim)
        else:
            game.tilemap.animations.append(anim)

    def reverse(self):
        if self.is_upper_layer:
            for anim in game.tilemap.high_animations[:]:
                if anim.nid == self.nid and anim.xy_pos == self.pos:
                    game.tilemap.high_animations.remove(anim)
                    break
        else:
            for anim in game.tilemap.animations[:]:
                if anim.nid == self.nid and anim.xy_pos == self.pos:
                    game.tilemap.animations.remove(anim)
                    break

class RemoveMapAnim(Action):
    def __init__(self, nid, pos, upper_layer: bool = False):
        self.nid = nid
        self.pos = pos
        self.speed_mult = 1
        self.blend = False
        self.did_remove = False
        self.is_upper_layer = upper_layer

    def do(self):
        if self.is_upper_layer:
            for anim in game.tilemap.high_animations[:]:
                if anim.nid == self.nid and anim.xy_pos == self.pos:
                    self.speed_mult = anim.speed_adj
                    self.blend = anim.tint
                    game.tilemap.high_animations.remove(anim)
                    self.did_remove = True
        else:
            for anim in game.tilemap.animations[:]:
                if anim.nid == self.nid and anim.xy_pos == self.pos:
                    self.speed_mult = anim.speed_adj
                    self.blend = anim.tint
                    game.tilemap.animations.remove(anim)
                    self.did_remove = True


    def reverse(self):
        if self.did_remove:
            if self.is_upper_layer:
                anim = RESOURCES.animations.get(self.nid)
                anim = animations.MapAnimation(anim, self.pos, loop=True, speed_adj=self.speed_mult)
                anim.set_tint(self.blend)
                game.tilemap.high_animations.append(anim)

            else:
                anim = RESOURCES.animations.get(self.nid)
                anim = animations.MapAnimation(anim, self.pos, loop=True, speed_adj=self.speed_mult)
                anim.set_tint(self.blend)
                game.tilemap.animations.append(anim)

class AddAnimToUnit(Action):
    def __init__(self, nid, unit: UnitObject, speed_mult, blend):
        self.nid = nid
        self.unit = unit
        self.speed_mult = speed_mult
        self.blend = blend
        self.did_add = False

    def do(self):
        anim = RESOURCES.animations.get(self.nid)
        anim = animations.Animation(anim, (-16, -16), loop=True, speed_adj=self.speed_mult)
        anim.set_tint(self.blend)
        self.did_add = self.unit.sprite.add_animation(anim)

    def reverse(self):
        if self.did_add:
            self.unit.sprite.remove_animation(self.nid)

class RemoveAnimFromUnit(Action):
    def __init__(self, nid, unit: UnitObject):
        self.nid = nid
        self.unit = unit
        self.speed_mult = 1
        self.did_remove = False

    def do(self):
        if self.nid in self.unit.sprite.animations.keys():
            self.speed_mult = self.unit.sprite.animations[self.nid].speed_adj
        self.did_remove = self.unit.sprite.remove_animation(self.nid)

    def reverse(self):
        if self.did_remove:
            anim = RESOURCES.animations.get(self.nid)
            anim = animations.MapAnimation(anim, (-16, -16), loop=True, speed_adj=self.speed_mult)
            self.unit.sprite.add_animation(anim)

class ChangeObjective(Action):
    def __init__(self, key, string):
        self.key = key
        self.string = string
        self.old_objective = game.level.objective[self.key]

    def do(self):
        game.level.objective[self.key] = self.string

    def reverse(self):
        game.level.objective[self.key] = self.old_objective


class OnlyOnceEvent(Action):
    def __init__(self, event_nid):
        self.event_nid = event_nid

    def do(self):
        game.already_triggered_events.append(self.event_nid)

    def reverse(self):
        game.already_triggered_events.remove(self.event_nid)


class RecordRandomState(Action):
    def __init__(self, old, new):
        self.old = old
        self.new = new

    def do(self):
        pass

    def execute(self):
        static_random.set_combat_random_state(self.new)

    def reverse(self):
        static_random.set_combat_random_state(self.old)


class RecordOtherRandomState(Action):
    def __init__(self, old, new):
        self.old = old
        self.new = new

    def do(self):
        pass

    def execute(self):
        static_random.set_other_random_state(self.new)

    def reverse(self):
        static_random.set_other_random_state(self.old)


class TriggerCharge(Action):
    def __init__(self, unit, skill):
        self.unit = unit
        self.skill = skill

    def do(self):
        self.old_charge = self.skill.data.get('charge', None)
        skill_system.trigger_charge(self.unit, self.skill)
        self.new_charge = self.skill.data.get('charge', None)

    def reverse(self):
        if self.new_charge is not None:
            self.skill.data['charge'] = self.old_charge

class IncInitiativeTurn(Action):
    def __init__(self):
        self.old_idx = game.initiative.current_idx

    def do(self):
        game.initiative.next()

    def reverse(self):
        game.initiative.current_idx = self.old_idx

class InsertInitiative(Action):
    def __init__(self, unit):
        self.unit = unit

    def do(self):
        game.initiative.insert_unit(self.unit)

    def reverse(self):
        game.initiative.remove_unit(self.unit)

class RemoveInitiative(Action):
    def __init__(self, unit):
        self.unit = unit
        self.old_idx = game.initiative.get_index(self.unit)
        self.initiative = game.initiative.get_initiative(self.unit)

    def do(self):
        game.initiative.next()
        game.initiative.remove_unit(self.unit)

    def reverse(self):
        game.initiative.insert_at(self.unit, self.old_idx, self.initiative)
        game.initiative.back()

class MoveInInitiative(Action):
    def __init__(self, unit, offset):
        self.unit = unit
        self.offset = offset
        self.old_idx = game.initiative.get_index(self.unit)
        self.new_idx = self.old_idx + self.offset

    def do(self):
        game.initiative.remove_unit(self.unit)
        self.new_idx = game.initiative.insert_at(self.unit, self.new_idx)

    def reverse(self):
        game.initiative.remove_unit(self.unit)
        game.initiative.insert_at(self.unit, self.old_idx)

class AddSkill(Action):
    def __init__(self, unit, skill, initiator=None, source=None, source_type=SourceType.DEFAULT):
        self.unit = unit
        self.initiator = initiator
        # Check if we just passed in the skill nid to create
        if isinstance(skill, str):
            skill_obj = item_funcs.create_skill(unit, skill)
        else:
            skill_obj = skill
        if skill_obj:
            if self.initiator:
                skill_obj.initiator_nid = self.initiator.nid
            skill_system.init(skill_obj)
            if skill_obj.uid not in game.skill_registry:
                game.register_skill(skill_obj)
        self.skill_obj: SkillObject = skill_obj
        self.source = source
        self.source_type = source_type
        self.subactions = []
        self.reset_action = ResetUnitVars(self.unit)

        self.did_something = False

    @recalculate_unit
    def do(self):
        self.subactions.clear()
        if not self.skill_obj:
            return
        logging.debug("AddSkill.do: Adding Skill %s to %s", self.skill_obj, self.unit)

        popped_skill_obj = self.unit.add_skill(self.skill_obj, self.source, self.source_type, test=True)
        # Skill failed to add due to not displacing another skill and itself being displaceable
        if popped_skill_obj and popped_skill_obj == self.skill_obj:
            logging.info("Skill %s could not be added as no instance could be displaced" % self.skill_obj.nid)
            return
        if popped_skill_obj:
            logging.info("Skill %s is at max stacks, removing oldest displaceable instance" % self.skill_obj.nid)
            self.subactions.append(RemoveSkill(self.unit, popped_skill_obj))

        # Actually add skill
        skill_system.before_add(self.unit, self.skill_obj)
        self.skill_obj.owner_nid = self.unit.nid
        self.unit.add_skill(self.skill_obj, self.source, self.source_type)

        if self.skill_obj.aura and self.skill_obj in self.unit.all_skills and \
                self.unit.position and game.board and game.tilemap:
            game.boundary.unregister_unit_auras(self.unit)
            aura_funcs.propagate_aura(self.unit, self.skill_obj, game)
            game.boundary.register_unit_auras(self.unit)

        skill_system.after_add(self.unit, self.skill_obj)

        for action in self.subactions:
            action.execute()

        self.did_something = True

        # Handle affects movement
        self.reset_action.execute()

    @recalculate_unit
    def reverse(self):
        if not self.did_something:
            return
        self.reset_action.reverse()
        if not self.skill_obj:
            return

        logging.debug("AddSkill.reverse: Removing Skill %s from %s", self.skill_obj, self.unit)
        if self.skill_obj in self.unit.all_skills:
            # Actually remove skill
            skill_system.before_remove(self.unit, self.skill_obj)
            self.unit.remove_skill(self.skill_obj, self.source, self.source_type)
            self.skill_obj.owner_nid = None

            if self.skill_obj.aura and self.unit.position and game.board and game.tilemap:
                game.boundary.unregister_unit_auras(self.unit)
                aura_funcs.release_aura(self.unit, self.skill_obj, game)
                game.boundary.register_unit_auras(self.unit)

            skill_system.after_remove(self.unit, self.skill_obj)
        else:
            logging.error("Skill %s not in %s's skills", self.skill_obj.nid, self.unit)

        # Return displaced skills
        for action in self.subactions:
            action.reverse()

class RemoveSkill(Action):
    def __init__(self, unit, skill, count=-1, source=None, source_type=SourceType.DEFAULT):
        self.unit = unit
        self.skill = skill  # Skill obj or skill nid str
        self.removed_skills = []
        self.count = count
        self.source = source
        self.source_type = source_type
        self.old_owner_nid = None
        self.reset_action = ResetUnitVars(self.unit)

    def _remove_skill(self, skill, true_remove):
        if not self.unit.remove_skill(skill, self.source, self.source_type, test=True):
            logging.warning("No removable instance of Skill %s in %s's skills", self.skill, self.unit)
            return False

        skill_system.before_remove(self.unit, skill)
        if true_remove:
            skill_system.before_true_remove(self.unit, skill)
        skill.owner_nid = None
        removed_source, removed_source_type = self.unit.remove_skill(skill, self.source, self.source_type)
        self.removed_skills.append((skill, removed_source, removed_source_type))

        if skill.aura and self.unit.position and game.board and game.tilemap:
            game.boundary.unregister_unit_auras(self.unit)
            aura_funcs.release_aura(self.unit, skill, game)
            game.boundary.register_unit_auras(self.unit)

        skill_system.after_remove(self.unit, skill)
        if true_remove:
            skill_system.after_true_remove(self.unit, skill)

        return True

    def _remove(self, true_remove=True):
        self.removed_skills.clear()
        to_remove = self.count
        if isinstance(self.skill, str):
            for skill in self.unit.all_skills[:]:
                if skill.nid == self.skill and to_remove != 0:
                    removed = self._remove_skill(skill, true_remove)
                    if removed:
                        to_remove -= 1
            if to_remove > 0:
                logging.warning("%d removable instances of Skill %s not found in %s's skills", to_remove, self.skill, self.unit)
        else:
            if self.skill in self.unit.all_skills:
                self._remove_skill(self.skill, true_remove)
            else:
                logging.warning("Skill %s not in %s's skills", self.skill.nid, self.unit)


        # Handle affects movement
        self.reset_action.execute()

    @recalculate_unit
    def do(self):
        # Actually call on true remove hook
        self._remove(True)

    @recalculate_unit
    def execute(self):
        # Don't call on true remove hook
        self._remove(False)

    @recalculate_unit
    def reverse(self):
        self.reset_action.reverse()
        for skill, source, source_type in self.removed_skills:
            skill_system.before_add(self.unit, skill)
            skill.owner_nid = self.unit.nid
            self.unit.add_skill(skill, source, source_type)

            if skill.aura and self.unit.position and game.board and game.tilemap:
                game.boundary.unregister_unit_auras(self.unit)
                aura_funcs.propagate_aura(self.unit, skill, game)
                game.boundary.register_unit_auras(self.unit)

            skill_system.after_add(self.unit, skill)


# === Master Functions for adding to the action log ===
def do(action):
    from app.engine.game_state import game
    game.action_log.action_depth += 1
    action.do()
    game.action_log.action_depth -= 1
    if game.action_log.is_recording() and game.action_log.action_depth <= 0:
        game.action_log.append(action)


def execute(action):
    game.action_log.action_depth += 1
    action.execute()
    game.action_log.action_depth -= 1
    if game.action_log.is_recording() and game.action_log.action_depth <= 0:
        game.action_log.append(action)


def reverse(action):
    # The only time this function is called is when
    # reversing a move or canto move
    logging.debug("Reverse Action %s", action)
    # game.action_log.action_depth += 1
    # action.reverse()
    # game.action_log.action_depth -= 1
    if game.action_log.is_recording() and game.action_log.action_depth <= 0:
        # Handles reversing the action
        game.action_log.hard_remove(action)
    else: # Right now, this section will never happen
        game.action_log.action_depth += 1
        action.reverse()
        game.action_log.action_depth -= 1
