import logging

from app.engine import action, evaluate, phase
from app.engine.game_state import game
from app.engine.input_manager import get_input_manager
from app.engine.objects.unit import UnitObject
from app.engine.roam import free_roam_ai
from app.engine.movement.roam_player_movement_component import RoamPlayerMovementComponent
from app.engine.movement import movement_funcs
from app.engine.sound import get_sound_thread
from app.engine.state import MapState
from app.events import triggers
from app.events.regions import RegionType
from app.utilities import utils

class FreeRoamState(MapState):
    name = 'free_roam'

    TALK_RANGE = 1.0

    def start(self):
        self.roam_unit = None
        self.movement_component = None

        self.ai_handler = free_roam_ai.FreeRoamAIHandler()

    def add_ai_unit(self, unit: UnitObject):
        self.ai_handler.add_unit(unit)

    def remove_ai_unit(self, unit: UnitObject):
        self.ai_handler.remove_unit(unit)

    def contains_ai_unit(self, unit: UnitObject):
        return self.ai_handler.contains_unit(unit)

    def begin(self):
        game.cursor.hide()
        phase.fade_in_phase_music()
        if not self.ai_handler.active:
            self.ai_handler.start_all_units()

        self.ai_handler.reset_all_units()

        if game.is_roam() and game.get_roam_unit():
            roam_unit = game.get_roam_unit()
            if self.roam_unit and self.roam_unit != roam_unit:
                # Now get the new unit
                self._assign_unit(roam_unit)
            elif self.roam_unit:
                # Don't need to do anything -- just reusing the same unit
                pass
            elif roam_unit:  # Make a new self.roam_unit
                self._assign_unit(roam_unit)
            else:
                logging.error("Unable to find roaming unit %s", game.roam_info.roam_unit_nid)

        elif self.roam_unit:  # Have a roam unit and we shouldn't...
            self.roam_unit = None

        if not self.roam_unit or not self.roam_unit.position:
            self.leave()

    def leave(self):
        game.state.back()
        self.rationalize_all_units()
        game.set_roam(False)
        game.cursor.autocursor(immediate=True)
        # Leave this state
        return 'repeat'

    def _assign_unit(self, unit):
        self.roam_unit = unit
        self.roam_unit.sprite.set_roam_position(self.roam_unit.position)
        self.movement_component = RoamPlayerMovementComponent(unit)
        game.movement.add(self.movement_component)
        game.cursor.cur_unit = self.roam_unit

    def rationalize_all_units(self):
        """
        # Done whenever we would leave the roam state and we need the units to go to normal positions
        """
        self.ai_handler.stop_all_units()
        self.roam_unit = None
        if self.movement_component:
            self.movement_component.finish()
        game.state.change('free_roam_rationalize')

    def get_closest_unit(self, must_have_talk=False):
        """
        # Returns a unit that roam unit can talk to.
        # Returns the closest unit if more than one is available.
        # Returns None if no good targets.
        """
        if not self.roam_unit:
            return None
        units = []
        my_pos = self.roam_unit.sprite.position
        if not my_pos:
            logging.warning("Roam unit does not have a position")
            return None
        # Specifically not `game.get_all_units()` in order to support units with Tile tag being talkable
        all_units = [unit for unit in game.units if unit.position and not unit.dead and not unit.is_dying]
        for unit in all_units:
            has_talk = (self.roam_unit.nid, unit.nid) in game.talk_options
            if unit is not self.roam_unit and \
                    utils.calculate_distance(my_pos, unit.position) < self.TALK_RANGE and \
                    (has_talk if must_have_talk else True):
                units.append(unit)
        units = list(sorted(units, key=lambda unit: utils.calculate_distance(my_pos, unit.position)))
        if units:
            return units[0]
        return None

    def get_visit_region(self):
        """
        # Returns the first region that is close enough to visit
        """
        if not self.roam_unit:
            return None
        region = game.get_region_under_pos(self.roam_unit.position, RegionType.EVENT)
        if region:
            try:
                truth = evaluate.evaluate(region.condition, self.roam_unit,
                                          position=self.roam_unit.position,
                                          local_args={'region': region})
                if truth:
                    return region
            except Exception as e:
                logging.error("%s: Could not evaluate {%s}" % (e, region.condition))
        return None

    def check_region_interrupt(self):
        if not self.roam_unit:
            return
        region = movement_funcs.check_region_interrupt(self.roam_unit.position)
        if region:
            did_trigger = game.events.trigger(triggers.RoamingInterrupt(self.roam_unit, self.roam_unit.position, region))
            if did_trigger:
                if region.only_once:
                    action.do(action.RemoveRegion(region))
                self.rationalize_all_units()

    def check_select(self):
        """
        # Called whenever the player presses SELECT
        """
        other_unit = self.get_closest_unit(must_have_talk=True)
        region = self.get_visit_region()

        if other_unit:
            get_sound_thread().play_sfx('Select 2')
            did_trigger = game.events.trigger(triggers.OnTalk(self.roam_unit, other_unit, None))
            if did_trigger:
                # Rely on the talk event itself to remove the trigger
                # Behaves more like other things in the engine
                # action.do(action.RemoveTalk(self.roam_unit.nid, other_unit.nid))
                self.rationalize_all_units()
        elif region:
            get_sound_thread().play_sfx('Select 2')
            did_trigger = game.events.trigger(triggers.RegionTrigger(region.sub_nid, self.roam_unit, self.roam_unit.position, region))
            if not did_trigger:  # maybe this uses the more dynamic region trigger
                did_trigger = game.events.trigger(triggers.OnRegionInteract(self.roam_unit, self.roam_unit.position, region))
            if did_trigger:
                if region.only_once:
                    action.do(action.RemoveRegion(region))
                self.rationalize_all_units()
        else:
            get_sound_thread().play_sfx('Error')

    def check_info(self):
        """
        # Called whenever the player presses INFO
        """
        other_unit = self.get_closest_unit()
        did_trigger = game.events.trigger(triggers.RoamPressInfo(self.roam_unit, other_unit))
        if did_trigger:
            self.rationalize_all_units()
        else:
            get_sound_thread().play_sfx('Select 1')
            game.memory['next_state'] = 'info_menu'
            game.memory['current_unit'] = self.roam_unit
            game.state.change('transition_to')

    def check_aux(self):
        """
        # Called whenever the player presses AUX
        """
        other_unit = self.get_closest_unit()
        self.rationalize_all_units()
        did_trigger = game.events.trigger(triggers.RoamPressAux(self.roam_unit, other_unit))
        if not did_trigger:
            game.state.change('option_menu')

    def check_start(self):
        """
        # Called whenever the player presses START
        """
        other_unit = self.get_closest_unit()
        self.rationalize_all_units()
        did_trigger = game.events.trigger(triggers.RoamPressStart(self.roam_unit, other_unit))
        if not did_trigger:
            game.state.change('option_menu')

    def take_input(self, event):
        if not self.roam_unit:
            return

        # Handle movement controls
        # SPRINT
        if get_input_manager().is_pressed('BACK'):
            self.movement_component.set_sprint(True)
        else:
            self.movement_component.set_sprint(False)

        inputs = []
        for button in ['LEFT', 'RIGHT', 'UP', 'DOWN']:
            if get_input_manager().is_pressed(button) or get_input_manager().just_pressed(button):
                inputs.append(button)
        # Convert inputs to vector
        x, y = 0, 0
        if 'LEFT' in inputs:
            x = -1
        elif 'RIGHT' in inputs:
            x = 1
        if 'UP' in inputs:
            y = -1
        elif 'DOWN' in inputs:
            y = 1

        self.movement_component.set_acceleration((x, y))

        # Now check regular events
        if event == 'SELECT':
            self.check_select()

        elif event == 'AUX':
            self.check_aux()

        elif event == 'INFO':
            self.check_info()

        elif event == 'START':
            self.check_start()

    def update(self):
        super().update()
        self.ai_handler.update()
        game.movement.update()
        self.check_region_interrupt()
