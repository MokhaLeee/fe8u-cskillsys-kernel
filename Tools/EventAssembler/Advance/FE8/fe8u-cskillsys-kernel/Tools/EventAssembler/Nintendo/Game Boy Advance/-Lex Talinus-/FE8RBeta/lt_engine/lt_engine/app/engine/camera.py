from __future__ import annotations
from app.utilities.algorithms.interpolation import tcubic_easing

import math
from typing import TYPE_CHECKING, Callable, Tuple, List

from app.constants import TILEX, TILEY, TILEWIDTH, TILEHEIGHT
from app.engine import engine
from app.utilities import utils

if TYPE_CHECKING:
    from app.engine.game_state import GameState

class Camera():
    def __init__(self, game: GameState):
        self.game = game

        # Where the camera is going
        self.target_x = 0
        self.target_y = 0
        # Where the camera actually is
        self.current_x = 0
        self.current_y = 0

        # How fast the camera should move
        self.speed = 8.0  # Linear speed based on distance from target

        # Whether the camera should be panning at a constant speed
        self.pan_mode = False
        self.pan_speed = 0.125
        self.pan_targets = []

        # if we want to set a custom algorithm for the camera movement
        # custom algorithms should take in original_position, new_position, and current_time
        self.pan_algorithm: Callable[[float, float, int], Tuple[float, float]] = None

        self.last_at_rest_time: int = engine.get_time()
        self.last_at_rest_position: Tuple[float, float] = (self.current_x, self.current_y)

        # For screenshake
        self.no_shake = [(0, 0)]
        self.shake = self.no_shake
        self.shake_idx = 0
        self.shake_end_at = 0

    def get_next_position(self) -> Tuple[float, float]:
        diff_x = self.target_x - self.current_x
        diff_y = self.target_y - self.current_y

        diff_time = engine.get_time() - self.last_at_rest_time
        if self.pan_algorithm:
            return self.pan_algorithm(self.last_at_rest_position, (self.target_x, self.target_y), diff_time)
        elif self.pan_mode:
            new_x = self.pan_speed * utils.sign(diff_x) + self.current_x
            new_y = self.pan_speed * utils.sign(diff_y) + self.current_y
            return (new_x, new_y)
        elif diff_x or diff_y:
            dist = utils.distance((self.current_x, self.current_y), (self.target_x, self.target_y))
            total_speed = utils.clamp(dist / self.speed, min(dist, 0.25), 1.0)  # max of 0.5 is faithful to GBA, but I like the snappyness of 1.0
            angle = math.atan2(abs(diff_y), abs(diff_x))
            x_push = math.cos(angle)
            y_push = math.sin(angle)
            new_x = total_speed * x_push * utils.sign(diff_x) + self.current_x
            new_y = total_speed * y_push * utils.sign(diff_y) + self.current_y
            return (new_x, new_y)
        else:
            return (self.current_x, self.current_y)

    def _shift_x(self, x):
        if x <= self.target_x + 2:
            self.target_x -= 1
        elif x >= (TILEX + self.target_x - 3):
            self.target_x += 1

    def _shift_y(self, y):
        if y <= self.target_y + 1:
            self.target_y -= 1
        elif y >= (TILEY + self.target_y - 2):
            self.target_y += 1

    def cursor_x(self, x):
        self._shift_x(x)

    def cursor_y(self, y):
        self._shift_y(y)

    def mouse_x(self, x):
        self._shift_x(x)

    def mouse_y(self, y):
        self._shift_y(y)

    def mouse_xy(self, x, y):
        """
        Gives mouse position
        """
        self.mouse_x(x)
        self.mouse_y(y)

    def _change_x(self, x):
        if x <= self.target_x + 3:
            new_x = x - 3
        elif x >= self.target_x + TILEX - 3:
            new_x = x - TILEX + 3
        else:
            new_x = self.target_x
        return new_x

    def _change_y(self, y):
        if y <= self.target_y + 3:
            new_y = y - 3
        elif y >= self.target_y + TILEY - 3:
            new_y = y - TILEY + 3
        else:
            new_y = self.target_y
        return new_y

    def _center_x(self, x):
        return utils.clamp(x - TILEX//2, 0, self.game.tilemap.width - TILEX)

    def _center_y(self, y):
        return utils.clamp(y - TILEY//2, 0, self.game.tilemap.height - TILEY)

    def set_xy(self, x, y):
        x = self._change_x(x)
        self.target_x = x
        y = self._change_y(y)
        self.target_y = y

    def force_xy(self, x, y):
        x = self._change_x(x)
        self.current_x = self.target_x = x
        y = self._change_y(y)
        self.current_y = self.target_y = y

    def set_center(self, x, y):
        x = self._center_x(x)
        self.target_x = x
        y = self._center_y(y)
        self.target_y = y

    def force_center(self, x, y):
        x = self._center_x(x)
        self.current_x = self.target_x = x
        y = self._center_y(y)
        self.current_y = self.target_y = y

    def set_center2(self, pos1, pos2):
        x1, y1 = pos1
        x2, y2 = pos2
        mid_x = (x1 + x2) // 2
        mid_y = (y1 + y2) // 2
        self.set_center(mid_x, mid_y)

    def get_x(self):
        return self.current_x

    def get_y(self):
        return self.current_y

    def get_xy(self):
        return self.get_x(), self.get_y()

    def get_shake(self):
        return self.shake[self.shake_idx]

    def at_rest(self):
        return self.current_x == self.target_x and self.current_y == self.target_y

    def set_target_limits(self, tilemap):
        if self.target_x < 0:
            self.target_x = 0
        elif self.target_x > tilemap.width - TILEX:
            self.target_x = max(0, tilemap.width - TILEX)
        if self.target_y < 0:
            self.target_y = 0
        elif self.target_y > tilemap.height - TILEY:
            self.target_y = max(0, tilemap.height - TILEY)

    def set_current_limits(self, tilemap):
        if self.current_x < 0:
            self.current_x = 0
        elif self.current_x > tilemap.width - TILEX:
            self.current_x = max(0, tilemap.width - TILEX)
        if self.current_y < 0:
            self.current_y = 0
        elif self.current_y > tilemap.height - TILEY:
            self.current_y = max(0, tilemap.height - TILEY)

    def do_slow_pan(self, duration):
        # queues up a slower algorithm for the next pan
        self.pan_algorithm = lambda a, b, t: tcubic_easing(a, b, t/duration)

    def set_shake(self, shake: List[Tuple[int, int]], duration: int = 0):
        """
        shake - A List of camera offset tuples that will be looped over each frame to create the screen shake effect
        duration - How long the effect should last (in milliseconds). If 0 or negative, effect is permanent until reset_shake is called
        """
        self.shake = shake
        self.shake_idx = 0
        if duration > 0:
            self.shake_end_at = engine.get_time() + duration

    def reset_shake(self):
        self.shake = self.no_shake
        self.shake_idx = 0
        self.shake_end_at = 0

    def update(self):
        # Make sure target is within bounds
        self.set_target_limits(self.game.tilemap)

        # Move camera around
        (new_x, new_y) = self.get_next_position()
        self.current_x = new_x
        self.current_y = new_y

        # If close enough to target, just make it so
        diff_x = self.target_x - self.current_x
        diff_y = self.target_y - self.current_y
        if abs(diff_x) <= 0.125:
            self.current_x = self.target_x
        if abs(diff_y) <= 0.125:
            self.current_y = self.target_y

        if self.at_rest():
            # all custom algorithms should only last one pan
            self.pan_algorithm = None
            self.last_at_rest_time = engine.get_time()
            self.last_at_rest_position = (self.current_x, self.current_y)

        if self.pan_targets and self.at_rest():
            self.target_x, self.target_y = self.pan_targets.pop()

        # Make sure we do not go offscreen -- maybe shouldn't happen?
        # Could happen when map size changes?
        self.set_current_limits(self.game.tilemap)

        # Update screenshake
        self.shake_idx += 1
        self.shake_idx %= len(self.shake)
        if self.shake_end_at and engine.get_time() > self.shake_end_at:
            self.reset_shake()
