from __future__ import annotations

from typing import Dict, List, Optional, Tuple
from app.engine.objects.unit import UnitObject
from app.utilities.typing import NID

from app.data.database.database import DB
from app.engine.game_state import game
from app.engine import action, ai_controller, engine, equations, evaluate, item_funcs
from app.engine.roam import roam_ai_action
from app.engine.movement.roam_ai_movement_component import RoamAIMovementComponent
from app.engine.objects.region import RegionObject
from app.events.regions import RegionType
from app.events import triggers
from app.utilities import utils

import logging

BASE_SPEED_DENOMINATOR = 100.
RECALCULATE_TIME = 333  # ms

class FreeRoamAIHandler:
    def __init__(self):
        self.active: bool = True
        self.roam_ais: Dict[NID, RoamAI] = {}
        # Keep a reference to the movement components
        # we added to the main movement system
        # to be able to stop them later
        # And also to set their paths they should be using
        self.components: Dict[NID, RoamAIMovementComponent] = {}
        for unit in game.get_all_units():
            self._add_movement_component(unit)
        self.start_all_units()

    def _add_movement_component(self, unit: UnitObject) -> Optional[RoamAIMovementComponent]:
        if unit.get_roam_ai() and DB.ai.get(unit.get_roam_ai()).roam_ai:
            self.roam_ais[unit.nid] = RoamAI(unit)
            mc = RoamAIMovementComponent(unit)
            self.components[unit.nid] = mc
            return mc
        return None

    def add_unit(self, unit: UnitObject):
        mc = self._add_movement_component(unit)
        if mc:
            mc.start()
            game.movement.add(mc)

    def remove_unit(self, unit: UnitObject):
        mc = self.components.get(unit.nid)
        if mc:
            mc.finish()

    def contains_unit(self, unit: UnitObject) -> Optional[RoamAIMovementComponent]:
        return self.components.get(unit.nid)

    def update(self):
        if not self.active:
            return
        for unit in game.get_all_units():
            roam_ai = self.get_roam_ai(unit)
            if not roam_ai or not unit.position:
                continue
            if not roam_ai.state:
                roam_ai.think()
            roam_ai.act()
            # Every frame, make sure our movement component has the right path
            # if roam_ai.path:
            self.components[roam_ai.unit.nid].set_path(roam_ai.path)

    def stop_all_units(self):
        self.active = False
        for mc in self.components.values():
            mc.finish()

    def start_all_units(self):
        self.active = True
        for mc in self.components.values():
            mc.start()
            game.movement.add(mc)

    def reset_all_units(self):
        for mc in self.components.values():
            mc.reset_position()
        for roam_ai in self.roam_ais.values():
            roam_ai.reset_path()

    def get_roam_ai(self, unit):
        return self.roam_ais.get(unit.nid)

class RoamAI:
    def __init__(self, unit):
        self.unit = unit
        self.reset()

    def reset(self):
        self.state = None
        self.path: List[Tuple[int, int]] = []

        self.behaviour_idx: int = 0
        self.behaviour = None
        self.desired_proximity = 0
        self.speed_mult: float = 1.

        self._last_recalculate = engine.get_time()

    def reset_for_next_behaviour(self):
        self.state = None
        self.reset_path()

    def reset_path(self):
        self.path.clear()

    def set_next_behaviour(self):
        def check_condition(next_behaviour) -> bool:
            if not next_behaviour.condition or \
                    evaluate.evaluate(next_behaviour.condition, self.unit, position=self.unit.position):
                self.behaviour = next_behaviour
                return True
            return False

        counter = 0  # To make sure we don't have an infinite loop
        behaviours = DB.ai.get(self.unit.get_roam_ai()).behaviours
        while self.behaviour_idx < len(behaviours) and counter < 99:
            counter += 1
            next_behaviour = behaviours[self.behaviour_idx]
            self.behaviour_idx += 1
            # Turn back to the beginning
            if self.behaviour_idx >= len(behaviours):
                self.behaviour_idx = 0
            if check_condition(next_behaviour):
                break            
        else:
            logging.error("No AI behaviour of %s with a valid condition was found", self.unit.get_roam_ai())
            self.behaviour_idx = 0
            next_behaviour = None

    def get_path(self, pos) -> List[Tuple[int, int]]:
        return game.path_system.get_path(self.unit, pos, free_movement=True)

    def _calc_state(self) -> bool:
        # Returns whether we should try again
        if self.behaviour.action == 'None':
            return False
        elif self.behaviour.action == "Wait":
            start_time = engine.get_time()
            to_wait = self.behaviour.target_spec or 0
            self.state = roam_ai_action.Wait(self.unit, start_time + to_wait)
            return False
        elif self.behaviour.action == "Move_to":
            target: Optional[Tuple[int, int]] = self.approach()
            if target:
                self.state = roam_ai_action.MoveTo(self.unit, target, self.behaviour.desired_proximity)
            return False
        elif self.behaviour.action == "Interact":
            region: Optional[RegionObject] = self.find_region()
            if region:
                self.state = roam_ai_action.Interact(self.unit, region, self.behaviour.desired_proximity)
            return False
        elif self.behaviour.action == "Move_away_from":
            target: Optional[Tuple[int, int]] = self.retreat()
            if target:
                self.state = roam_ai_action.MoveTo(self.unit, target, self.behaviour.desired_proximity)
            return False
        # Some behaviour that is currently not supported for roaming
        return True

    def think(self):
        counter = 0
        while counter < 99:
            counter += 1
            self.set_next_behaviour()
            if self.behaviour:

                self.speed_mult = self.behaviour.roam_speed / BASE_SPEED_DENOMINATOR

                try_again = self._calc_state()
                if not try_again:
                    return
            else:  # No behaviour
                return
        logging.error("Infinite loop detected in %s's %s AI", self.unit, self.unit.get_roam_ai())

    def get_filtered_target_positions(self) -> List[Tuple[Tuple[int, int], float]]:
        target_positions = ai_controller.get_targets(self.unit, self.behaviour)

        zero_move = item_funcs.get_max_range(self.unit)
        single_move = zero_move + equations.parser.movement(self.unit)
        double_move = single_move + equations.parser.movement(self.unit)

        target_positions = [(pos, utils.calculate_distance(self.unit.position, pos)) for pos in target_positions]

        # Filter away some of the positions
        if self.behaviour.view_range == -4:
            pass
        elif self.behaviour.view_range == -3:
            target_positions = [(pos, mag) for pos, mag in target_positions if mag < double_move]
        elif self.behaviour.view_range == -2:
            target_positions = [(pos, mag) for pos, mag in target_positions if mag < single_move]
        elif self.behaviour.view_range == -1:
            target_positions = [(pos, mag) for pos, mag in target_positions if mag < zero_move]
        else:
            target_positions = [(pos, mag) for pos, mag in target_positions if mag < self.behaviour.view_range]
        return target_positions

    def approach(self) -> Optional[Tuple[int, int]]:
        target_positions = self.get_filtered_target_positions()
        # Remove mag, keep pos
        target_positions = [t[0] for t in target_positions]
        target_positions = list(sorted(target_positions, key=lambda pos: utils.calculate_distance(self.unit.position, pos)))

        if target_positions:
            target = target_positions[0]
            return target
        else:
            return None

    def retreat(self) -> Optional[Tuple[int, int]]:
        """
        # Returns best position furthest away from the target
        """
        valid_positions = game.path_system.get_valid_moves(self.unit)
        target_positions = self.get_filtered_target_positions()

        if target_positions:
            target = utils.smart_farthest_away_pos(self.unit.position, valid_positions, target_positions)
            return target
        else:
            return None

    def find_region(self) -> Optional[RegionObject]:
        """
        # Find the closest region to interact with
        """
        regions = []
        pos = self.unit.position
        for r in game.level.regions:
            if r.region_type == RegionType.EVENT and r.sub_nid == self.behaviour.target_spec:
                try:
                    if not r.condition or evaluate.evaluate(r.condition, self.unit, local_args={'region': r}):
                        regions.append(r)
                except:
                    logging.warning("Could not evaluate region conditional %s" % r.condition)
        # Find the closest
        regions = list(sorted(regions, key=lambda region: min(utils.calculate_distance(pos, rpos) for rpos in region.get_all_positions())))
        if regions:
            region = regions[0]
            return region
        else:
            return None

    def act(self):
        if not self.state:
            # Don't act if we don't have a state to act with
            return
        if self.state.action_type == roam_ai_action.RoamAIAction.MOVE:
            # Can recalculate the path because it's been a while
            if engine.get_time() - self._last_recalculate >= RECALCULATE_TIME:
                self._calc_state()
                self.path = self.get_path(self.state.target)
                self._last_recalculate = engine.get_time()
            self.move(self.state.target, self.state.desired_proximity)
        elif self.state.action_type == roam_ai_action.RoamAIAction.WAIT:
            self.wait(self.state.time)
        elif self.state.action_type == roam_ai_action.RoamAIAction.INTERACT:
            self.make_path(self.state.region.center)
            # Then try to interact (will probably fail unless we are close enough)
            self.interact(self.state.region, self.state.desired_proximity)

    def wait(self, target_time):
        if engine.get_time() > target_time:
            self.reset_for_next_behaviour()

    def interact(self, region: RegionObject, proximity: float):
        positions = [pos for pos in region.get_all_positions() if utils.calculate_distance(self.unit.position, pos) <= proximity]
        if positions:
            pos = list(sorted(positions, key=lambda pos: utils.calculate_distance(self.unit.position, pos)))[0]
            did_trigger = game.events.trigger(triggers.RegionTrigger(region.sub_nid, self.state.unit, pos, region))
            if not did_trigger:  # Just in case we need the generic one
                did_trigger = game.events.trigger(triggers.OnRegionInteract(self.state.unit, pos, region))
            if did_trigger and region.only_once:
                action.do(action.RemoveRegion(region))
            if did_trigger:
                action.do(action.HasAttacked(self.unit))
            self.reset_for_next_behaviour()

    def move(self, target: Tuple[int, int], proximity: float):
        """
        # Called every update while the unit is to be moving
        # Checks if the path should be recalculated
        """
        if not target:
            self.reset_for_next_behaviour()
            return
        position = self.unit.sprite.position
        if utils.calculate_distance(position, target) <= proximity:
            # Turns out we really are close
            self.reset_for_next_behaviour()
            return
            
        self.make_path(target)

    def make_path(self, target: Tuple[int, int]):
        # Check whether the path has diverged too much
        if self.path and self.path[0] != target:
            self.path = self.get_path(target)
        # If we actually just don't have a path
        if not self.path:
            self.path = self.get_path(target)
