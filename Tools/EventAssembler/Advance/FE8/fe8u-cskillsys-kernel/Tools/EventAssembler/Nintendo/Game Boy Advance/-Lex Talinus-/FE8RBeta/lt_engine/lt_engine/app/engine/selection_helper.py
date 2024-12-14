from app.utilities import utils
from app.constants import TILEWIDTH, TILEHEIGHT
from app.engine.input_manager import get_input_manager

from app.engine.game_state import game

class SelectionHelper():
    def __init__(self, pos_list):
        self.pos_list = list(pos_list)

    def count(self):
        return len(self.pos_list)

    def remove_target(self, pos):
        if pos in self.pos_list:
            self.pos_list.remove(pos)

    def handle_mouse(self):
        mouse_position = get_input_manager().get_mouse_position()
        if mouse_position:
            new_pos_x = int(mouse_position[0] // TILEWIDTH + game.camera.get_x())
            new_pos_y = int(mouse_position[1] // TILEHEIGHT + game.camera.get_y())
            new_pos = new_pos_x, new_pos_y
            if new_pos in self.pos_list:
                return new_pos
        return None

    # For a given position, determine which position in self.pos_list is closest
    def get_closest(self, position):
        if self.pos_list:
            return min(self.pos_list, key=lambda pos: utils.calculate_distance(pos, position))
        else:
            return None

    def get_least_hp(self, position):
        if self.pos_list:
            return min(self.pos_list, key=lambda pos: (game.board.get_unit(pos).get_hp() / game.board.get_unit(pos).get_max_hp() if game.board.get_unit(pos) else 9999))
        else:
            return None

    # For a given position, get the next in the list
    def get_next(self, position):
        if position in self.pos_list:
            idx = self.pos_list.index(position)
            new_idx = (idx + 1) % len(self.pos_list)
            new_pos = self.pos_list[new_idx]
            return new_pos
        elif self.pos_list:
            return self.pos_list[0]
        else:
            return None

    # For a given position, determine which position in self.pos_list is the closest position in the downward direction
    def get_down(self, position):
        min_distance, closest = 100, None
        for pos in self.pos_list:
            if pos[1] > position[1]: # If further down than the position
                dist = utils.calculate_distance(pos, position)
                if dist < min_distance:
                    closest = pos
                    min_distance = dist
        if closest is None: # Nothing was found in the down direction
            # Just find the closest
            closest = self.get_closest(position)
        return closest

    def get_up(self, position):
        min_distance, closest = 100, None
        for pos in self.pos_list:
            if pos[1] < position[1]: # If further up than the position
                dist = utils.calculate_distance(pos, position)
                if dist < min_distance:
                    closest = pos
                    min_distance = dist
        if closest is None: # Nothing was found in the down direction
            # Just find the closest
            closest = self.get_closest(position)
        return closest

    def get_right(self, position):
        min_distance, closest = 100, None
        for pos in self.pos_list:
            if pos[0] > position[0]: # If further right than the position
                dist = utils.calculate_distance(pos, position)
                if dist < min_distance:
                    closest = pos
                    min_distance = dist
        if closest is None: # Nothing was found in the down direction
            # Just find the closest
            closest = self.get_closest(position)
        return closest

    def get_left(self, position):
        min_distance, closest = 100, None
        for pos in self.pos_list:
            if pos[0] < position[0]: # If further left than the position
                dist = utils.calculate_distance(pos, position)
                if dist < min_distance:
                    closest = pos
                    min_distance = dist
        if closest is None: # Nothing was found in the down direction
            # Just find the closest
            closest = self.get_closest(position)
        return closest
