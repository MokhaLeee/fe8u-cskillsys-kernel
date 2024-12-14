from __future__ import annotations

from typing import List, Tuple

from app.constants import TILEWIDTH, TILEHEIGHT
from app.engine.game_state import game
from app.engine import action, engine
from app.engine.movement.movement_component import MovementComponent
from app.engine.movement import movement_funcs
from app.engine.sound import get_sound_thread
from app.utilities import utils

import logging

class UnitPathMovementComponent(MovementComponent):
    """
    # Used for moving a unit along a path
    """
    grid_move = True

    def __init__(self, unit, path: List[Tuple[int, int]], event=False,
                 follow=True, muted=False, speed: int = 0):
        super().__init__(unit, follow, muted)
        self.path = path
        self.goal = self.path[0] if self.path else None
        self.event: bool = event
        # How fast we move to each new position in the path
        self.speed = int(speed)
        self.start()

    def get_camera_position(self):
        return self.unit.position

    def should_camera_center(self) -> bool:
        return self.follow and self.event

    def get_end_goal(self):
        return self.goal

    def start(self):
        self.unit.sprite.change_state('moving')
        if self.path and self.path[-1] == self.unit.position:
            self.path.pop()  # Pop off the current position
        if self.path:
            next_position = self.path[-1]
            net_position = (next_position[0] - self.unit.position[0], next_position[1] - self.unit.position[1])
            self.unit.sprite.handle_net_position(net_position)
        action.PickUnitUp(self.unit).do()
        if not self.muted:
            self.unit.sound.play()
        self._last_update = engine.get_time()

    def update(self, current_time: int):
        if not self.active:
            return

        if not self.unit.position:
            logging.error("Unit %s is no longer on the map", self.unit)
            self.active = False
            return

        dt = current_time - self._last_update
        progress: float = utils.clamp(dt / max(self.speed, 1), 0, 1)

        # Update sprite
        if self.path:
            next_position = self.path[-1]
            net_position = (next_position[0] - self.unit.position[0], next_position[1] - self.unit.position[1])
            self.unit.sprite.handle_net_position(net_position)
            if progress >= 1:
                self.unit.sprite.reset()
            else:
                self.unit.sprite.offset[0] = int(TILEWIDTH * progress * net_position[0])
                self.unit.sprite.offset[1] = int(TILEHEIGHT * progress * net_position[1])

        if progress >= 1:
            self._last_update = current_time

            if self.path:
                self._handle_path()
            if not self.path:  # Path is empty, we are done
                surprise = movement_funcs.check_region_interrupt(self.unit.position)
                self.finish(surprise=surprise)

    def _handle_path(self):
        next_position = self.path.pop()
        if self.unit.position != next_position:
            if movement_funcs.check_position(
                    self.unit, next_position,
                    self.goal == next_position, self.event):
                logging.debug("%s moved to %s", self.unit, next_position)
                mcost = movement_funcs.get_mcost(self.unit, next_position)
                self.unit.consume_movement(mcost)
            else:  # This new position ain't valid
                logging.debug("%s can't move any further", self.unit)
                self.finish(surprise=True)
                return

        self.unit.position = next_position

    def finish(self, surprise=False):
        """
        # Called when the unit has finished their movement
        # surprise will be True when the unit has run into an obstacle
        # (enemy unit, interrupt region, etc) that was not expected to
        # be there, and therefore their movement was interrupted

        """
        if surprise:
            get_sound_thread().play_sfx('Surprise')
            self.unit.sprite.change_state('normal')
            self.unit.sprite.reset()
            self.unit.sprite.add_animation('MapSurprise', loop=False)
            action.do(action.HasAttacked(self.unit))
            if self.unit.team == 'player':
                game.state.clear()
                game.state.change('free')
                game.state.change('wait')
                # Dummy state change so that game.state.back() at the end of the movement state
                # will just get rid of this instead
                game.state.change('wait')
            if game.ai.unit is self.unit:
                game.ai.interrupt()

        action.PutUnitDown(self.unit).do()
        if self.unit.sound:
            self.unit.sound.stop()

        if self.event:
            self.unit.sprite.change_state('normal')
            action.do(action.Reset(self.unit))
            action.do(action.UpdateFogOfWar(self.unit))
        else:
            self.unit.has_moved = True

        self.active = False
