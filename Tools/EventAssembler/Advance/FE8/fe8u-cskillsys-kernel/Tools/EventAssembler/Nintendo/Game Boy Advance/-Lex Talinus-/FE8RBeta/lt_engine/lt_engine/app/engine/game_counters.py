from app.utilities.utils import frames2ms
from app.engine import engine
from app.counters import generic3counter, movement_counter, simplecounter

class Counters():
    def __init__(self):
        self.reset()

    def reset(self):
        self._passive_sprite_counter = generic3counter(frames2ms(32), frames2ms(4))
        self._active_sprite_counter = generic3counter(frames2ms(20), frames2ms(4))
        self._move_sprite_counter = simplecounter((frames2ms(13), frames2ms(6), frames2ms(13), frames2ms(6)))
        self._fast_move_sprite_counter = simplecounter((frames2ms(6), frames2ms(3), frames2ms(6), frames2ms(3)))
        self._attack_movement_counter = movement_counter()
        self._arrow_counter = simplecounter((frames2ms(16), frames2ms(16), frames2ms(16)))
        self._x2_counter = simplecounter([frames2ms(3)] * 18)
        self._flag_counter = simplecounter([frames2ms(15)] * 4)
        self._fps6_360counter = simplecounter([frames2ms(6)] * 360)
        self._fps2_360counter = simplecounter([frames2ms(2)] * 360)

    @property
    def passive_sprite_counter(self):
        current_time = engine.get_time()
        self._passive_sprite_counter.update(current_time)
        return self._passive_sprite_counter

    @property
    def active_sprite_counter(self):
        current_time = engine.get_time()
        self._active_sprite_counter.update(current_time)
        return self._active_sprite_counter

    @property
    def move_sprite_counter(self):
        current_time = engine.get_time()
        self._move_sprite_counter.update(current_time)
        return self._move_sprite_counter

    @property
    def fast_move_sprite_counter(self):
        current_time = engine.get_time()
        self._fast_move_sprite_counter.update(current_time)
        return self._fast_move_sprite_counter

    @property
    def attack_movement_counter(self):
        current_time = engine.get_time()
        self._attack_movement_counter.update(current_time)
        return self._attack_movement_counter

    @property
    def arrow_counter(self):
        current_time = engine.get_time()
        self._arrow_counter.update(current_time)
        return self._arrow_counter

    @property
    def x2_counter(self):
        current_time = engine.get_time()
        self._x2_counter.update(current_time)
        return self._x2_counter

    @property
    def flag_counter(self):
        current_time = engine.get_time()
        self._flag_counter.update(current_time)
        return self._flag_counter

    @property
    def fps6_360counter(self):
        current_time = engine.get_time()
        self._fps6_360counter.update(current_time)
        return self._fps6_360counter

    @property
    def fps2_360counter(self):
        current_time = engine.get_time()
        self._fps2_360counter.update(current_time)
        return self._fps2_360counter


ANIMATION_COUNTERS = Counters()
