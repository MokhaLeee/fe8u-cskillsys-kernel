from app.engine import engine
from app.engine.input_manager import get_input_manager

class FluidScroll():
    def __init__(self, speed=67, slow_speed=3):
        self.reset()

        self.left_update = 0
        self.right_update = 0
        self.up_update = 0
        self.down_update = 0
        self.fast_speed = speed
        self.slow_speed = int(speed*slow_speed)
        self.move_counter = 0

    def reset(self):
        self.move_left = False
        self.move_right = False
        self.move_up = False
        self.move_down = False

    def update_speed(self, speed=64, slow_speed=3):
        self.fast_speed = speed
        self.slow_speed = int(speed*slow_speed)

    def update(self, hold=True):
        if (hold and get_input_manager().is_pressed('LEFT')) or \
                get_input_manager().just_pressed('LEFT'):
            self.move_right = False
            self.move_left = True
        else:
            self.move_left = False

        if (hold and get_input_manager().is_pressed('RIGHT')) or \
                get_input_manager().just_pressed('RIGHT'):
            self.move_left = False
            self.move_right = True
        else:
            self.move_right = False

        if (hold and get_input_manager().is_pressed('UP')) or \
                get_input_manager().just_pressed('UP'):
            self.move_down = False
            self.move_up = True
        else:
            self.move_up = False

        if (hold and get_input_manager().is_pressed('DOWN')) or \
                get_input_manager().just_pressed('DOWN'):
            self.move_up = False
            self.move_down = True
        else:
            self.move_down = False

        if get_input_manager().just_pressed('LEFT'):
            self.left_update = 0
        if get_input_manager().just_pressed('RIGHT'):
            self.right_update = 0
        if get_input_manager().just_pressed('UP'):
            self.up_update = 0
        if get_input_manager().just_pressed('DOWN'):
            self.down_update = 0

        # If we changed a direction, reset the move counter
        if get_input_manager().just_pressed('LEFT') or get_input_manager().just_pressed('RIGHT') or \
                get_input_manager().just_pressed('UP') or get_input_manager().just_pressed('DOWN'):
            self.move_counter = 0

        # If we didn't make any moves, reset the move counter
        if not any((self.move_left, self.move_right, self.move_down, self.move_up)):
            self.move_counter = 0

        if any((get_input_manager().just_pressed(direction) for direction in ('LEFT', 'RIGHT', 'UP', 'DOWN'))):
            return True  # This is a first push
        return False

    def get_directions(self, double_speed=False, slow_speed=False):
        directions = []
        current_time = engine.get_time()

        if slow_speed:
            speed = self.slow_speed
        elif self.move_counter >= 2:
            speed = self.fast_speed
        else:
            speed = self.slow_speed

        if double_speed:
            speed //= 2

        if self.move_left and current_time - self.left_update > speed:
            directions.append('LEFT')
        if self.move_right and current_time - self.right_update > speed:
            directions.append('RIGHT')
        if self.move_up and current_time - self.up_update > speed:
            directions.append('UP')
        if self.move_down and current_time - self.down_update > speed:
            directions.append('DOWN')

        if directions:  # If any move was made
            self.set_all_updates(current_time)
            self.move_counter += 1

        # If we didn't make any moves, reset the move counter
        if not any((self.move_left, self.move_right, self.move_down, self.move_up)):
            self.move_counter = 0
        return directions

    def set_all_updates(self, time):
        self.left_update = time
        self.right_update = time
        self.up_update = time
        self.down_update = time
