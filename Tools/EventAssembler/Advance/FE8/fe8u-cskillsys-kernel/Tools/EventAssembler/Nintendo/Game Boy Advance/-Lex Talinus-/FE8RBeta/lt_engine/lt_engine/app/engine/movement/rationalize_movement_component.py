from __future__ import annotations

from typing import Tuple

from app.engine import action
from app.engine.game_state import game
from app.engine.movement.movement_component import MovementComponent
from app.utilities import utils

import logging

class RationalizeMovementComponent(MovementComponent):
    """
    # Used for moving all unit's that are off kilter of the 
    # correct unit.position to the correct unit.position
    """
    grid_move = False
    
    speed = 6  # In tiles per seconds
    epsilon = 0.01  # In tiles -- when the unit is close enough

    def __init__(self, unit):
        super().__init__(unit, follow=False, muted=True)
        # This is the copy we will work with
        self.position = self.unit.sprite.get_roam_position()
        # This is where we shall go
        occupants = game.board.get_units(self.unit.position)
        if len(occupants) == 1 and occupants[0] is self.unit:
            self.goal = self.unit.position
        else:  # Somebody else is occupying your position
            # Find a new nearby position to call home
            position = game.target_system.get_nearest_open_tile(self.unit, self.unit.position)
            if position:
                self.goal = position
            else:
                logging.warning("Somehow wasn't able to find a nearby open tile")
                self.goal = game.target_system.get_nearest_open_tile(self.unit, (0, 0))
                if self.goal is None:
                    logging.error("Really couldn't find a valid position to rationalize unit to")
                    self.goal = (0, 0)

        self.start()

    def reset_position(self):
        self.position = self.unit.sprite.get_roam_position()

    def get_camera_position(self) -> Tuple[float, float]:
        return self.position

    def get_end_goal(self):
        return self.goal

    def start(self):
        # What the unit's velocity is
        x_vector = self.goal[0] - self.unit.sprite.get_roam_position()[0]
        y_vector = self.goal[1] - self.unit.sprite.get_roam_position()[1]
        x_vector, y_vector = utils.normalize((x_vector, y_vector))
        self.x_vel = self.speed * x_vector
        self.y_vel = self.speed * y_vector
        self.unit.sprite.change_state('moving')
        self.unit.sprite.handle_net_position((self.x_vel, self.y_vel))
        action.PickUnitUp(self.unit).do()

    def finish(self, surprise=False):
        self.unit.sprite.change_state('normal')
        self.unit.sprite.set_roam_position(None)
        self.unit.sprite.reset()
        self.unit.position = self.goal
        action.PutUnitDown(self.unit).do()
        self.active = False

    def update(self, current_time: int):
        delta_time_ms = (current_time - self._last_update)
        # Never make delta time too large
        delta_time_ms = min(delta_time_ms, utils.frames2ms(4))
        delta_time = delta_time_ms / 1000  # Get delta time in seconds
        self._last_update = current_time

        if not self.active:
            return

        if not self.unit.position:
            logging.error("Unit %s is no longer on the map", self.unit)
            self.active = False
            return

        x, y = self.position
        dx = self.x_vel * delta_time
        dy = self.y_vel * delta_time
        next_x = x + dx
        if dx > 0 and x + dx > self.goal[0]:
            next_x = min(x + dx, self.goal[0])
        elif dx < 0 and x + dx < self.goal[0]:
            next_x = max(x + dx, self.goal[0])
        next_y = y + dy
        if dy > 0 and y + dy > self.goal[1]:
            next_y = min(y + dy, self.goal[1])
        elif dy < 0 and y + dy < self.goal[1]:
            next_y = max(y + dy, self.goal[1])
        self.position = (next_x, next_y)

        # Assign the position to the image
        self.unit.sprite.set_roam_position(self.position)

        # If we are really close to our goal position
        # Just finish up
        if (abs(self.position[0] - self.goal[0]) < self.epsilon) and \
                (abs(self.position[1] - self.goal[1]) < self.epsilon):
            self.finish()
