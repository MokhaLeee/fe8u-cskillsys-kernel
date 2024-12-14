from enum import IntEnum
import math
from typing import Optional, Tuple

from app.engine.objects.unit import UnitObject
from app.constants import TILEHEIGHT, TILEWIDTH
from app.counters import generic3counter
from app.data.database.database import DB
from app.engine import engine, image_mods, skill_system
from app.engine.cursor import BaseCursor
from app.engine.game_state import GameState
from app.engine.input_manager import get_input_manager
from app.engine.sprites import SPRITES
from app.utilities.utils import frames2ms
from app.engine.engine import Surface

class LevelCursorDrawState(IntEnum):
    Hidden = 0
    Visible = 1
    Combat = 2
    Turnwheel = 3
    Formation = 4

class LevelCursor(BaseCursor):
    def __init__(self, game: GameState):
        super().__init__(camera=game.camera, game_board=game.board)
        # this is frame-accurate to GBA
        self.cursor_counter = generic3counter(frames2ms(20), frames2ms(2), frames2ms(8))
        self.game = game
        self.cur_unit = None
        self.path = []
        self.draw_state = LevelCursorDrawState.Hidden
        self.speed_state = False

        self._sprite = SPRITES.get('cursor')
        self._sprite_dim = (32, 32)
        self.format_sprite(self._sprite)

        self._display_arrows: bool = False
        self.arrows = []
        self._last_valid_position = None  # Last position within movement borders
        self.stopped_at_move_border = False

        self.position = (self.get_bounds()[0], self.get_bounds()[1])

    def get_hover(self) -> Optional[UnitObject]:
        unit = self.game.board.get_unit(self.position)
        if unit and 'Tile' not in unit.tags and self.game.board.in_vision(unit.position):
            return unit
        return None

    def get_bounds(self) -> Tuple[int, int, int, int]:
        self.game_board = self.game.board
        return super().get_bounds()

    def hide(self):
        super().hide()
        self.draw_state = LevelCursorDrawState.Hidden

    def show(self):
        super().show()
        self.draw_state = LevelCursorDrawState.Visible

    def combat_show(self):
        super().show()
        self.draw_state = LevelCursorDrawState.Combat

    def set_turnwheel_sprite(self):
        super().show()
        self.draw_state = LevelCursorDrawState.Turnwheel

    def formation_show(self):
        super().show()
        self.draw_state = LevelCursorDrawState.Formation

    def set_speed_state(self, val: bool):
        self.speed_state = val
        if val:
            self._transition_speed = 2
        else:
            self._transition_speed = 1

    def set_pos(self, pos):
        super().set_pos(pos)
        self.game.ui_view.remove_unit_display()

    def _get_path(self) -> list:
        if not self._last_valid_position:
            self.path.clear()
            return self.path

        if self.path:
            if self._last_valid_position in self.path:
                idx = self.path.index(self._last_valid_position)
                self.path = self.path[idx:]
                return self.path
            elif self._last_valid_position in self.game.target_system.get_adjacent_positions(self.path[0]):
                self.path.insert(0, self._last_valid_position)
                if self.game.path_system.check_path(self.cur_unit, self.path):
                    return self.path

        self.path = self.game.path_system.get_path(self.cur_unit, self._last_valid_position, use_limit=True)
        return self.path

    def move(self, dx, dy, mouse=False, sound=True):
        super().move(dx, dy, mouse, sound)

        if self.game.highlight.check_in_move(self.position):
            self._last_valid_position = self.position

        if self._display_arrows:
            self.path = self._get_path()
            self.construct_arrows(self.path[::-1])

        # Remove unit info display
        if dx != 0 or dy != 0:
            self.game.ui_view.remove_unit_display()

    def autocursor(self, immediate=False):
        player_units = [unit for unit in self.game.units if unit.team == 'player' and unit.position]
        lord_units = [unit for unit in player_units if 'Lord' in unit.tags]
        if lord_units:
            self.set_pos(lord_units[0].position)
            if immediate:
                self.camera.force_center(*self.position)
            else:
                self.camera.set_center(*self.position)
        elif player_units:
            self.set_pos(player_units[0].position)
            if immediate:
                self.camera.force_center(*self.position)
            else:
                self.camera.set_center(*self.position)

    def show_arrows(self):
        self._display_arrows = True

    def place_arrows(self):
        self.path.clear()
        self._display_arrows = True

    def construct_arrows(self, path):
        self.arrows.clear()
        if len(path) <= 1:
            return
        for idx in range(len(path)):
            if idx == 0:  # Start of path
                direction = (path[idx + 1][0] - path[idx][0], path[idx + 1][1] - path[idx][1])
                if direction == (1, 0):  # Right
                    self.arrows.append(Arrow(0, 0, path[idx], idx))
                elif direction == (-1, 0):  # Left
                    self.arrows.append(Arrow(1, 1, path[idx], idx))
                elif direction == (0, -1):  # Up
                    self.arrows.append(Arrow(0, 1, path[idx], idx))
                elif direction == (0, 1):  # Down
                    self.arrows.append(Arrow(1, 0, path[idx], idx))
            elif idx == len(path) - 1:  # End of path
                direction = (path[idx][0] - path[idx - 1][0], path[idx][1] - path[idx - 1][1])
                if direction == (1, 0):  # Right
                    self.arrows.append(Arrow(6, 0, path[idx], idx))
                elif direction == (-1, 0):  # Left
                    self.arrows.append(Arrow(7, 1, path[idx], idx))
                elif direction == (0, -1):  # Up
                    self.arrows.append(Arrow(6, 1, path[idx], idx))
                elif direction == (0, 1):  # Down
                    self.arrows.append(Arrow(7, 0, path[idx], idx))
            else:  # Neither beginning nor end of path
                next_p = path[idx + 1]
                current_p = path[idx]
                prev_p = path[idx - 1]
                direction = (next_p[0] - prev_p[0], next_p[1] - prev_p[1])
                modifier = (current_p[0] - prev_p[0], current_p[1] - prev_p[1])
                if direction == (2, 0) or direction == (-2, 0):  # Right or Left
                    self.arrows.append(Arrow(3, 0, path[idx], idx))
                elif direction == (0, 2) or direction == (0, -2):  # Up or Down
                    self.arrows.append(Arrow(2, 0, path[idx], idx))
                elif direction == (1, -1) or direction == (-1, 1):  # Topleft or Bottomright
                    if modifier == (0, -1) or modifier == (-1, 0):
                        self.arrows.append(Arrow(4, 0, path[idx], idx))
                    elif modifier == (1, 0) or modifier == (0, 1):
                        self.arrows.append(Arrow(5, 1, path[idx], idx))
                elif direction == (1, 1) or direction == (-1, -1):  # Topright or Bottomleft
                    if modifier == (0, -1) or modifier == (1, 0):
                        self.arrows.append(Arrow(5, 0, path[idx], idx))
                    else:
                        self.arrows.append(Arrow(4, 1, path[idx], idx))

    def remove_arrows(self):
        self._last_valid_position = None
        self._display_arrows = False
        self.arrows.clear()

    def take_input(self):
        self.fluid.update()
        if self.stopped_at_move_border:
            directions = self.fluid.get_directions(double_speed=self.speed_state, slow_speed=True)
        else:
            directions = self.fluid.get_directions(double_speed=self.speed_state)

        if self.game.highlight.check_in_move(self.position):
            if directions:
                # If we would move off the current move
                if ('LEFT' in directions and not get_input_manager().just_pressed('LEFT') and
                        not self.game.highlight.check_in_move((self.position[0] - 1, self.position[1]))) or \
                        ('RIGHT' in directions and not get_input_manager().just_pressed('RIGHT') and
                         not self.game.highlight.check_in_move((self.position[0] + 1, self.position[1]))) or \
                        ('UP' in directions and not get_input_manager().just_pressed('UP') and
                         not self.game.highlight.check_in_move((self.position[0], self.position[1] - 1))) or \
                        ('DOWN' in directions and not get_input_manager().just_pressed('DOWN') and
                         not self.game.highlight.check_in_move((self.position[0], self.position[1] + 1))):
                    # Then we can just keep going
                    if self.stopped_at_move_border:
                        self.stopped_at_move_border = False
                    else:  # Ooh, we gotta stop the cursor movement
                        directions.clear()
                        self.fluid.reset()
                        self.stopped_at_move_border = True
                else:
                    self.stopped_at_move_border = False
        else:
            self.stopped_at_move_border = False

        self._handle_move(directions)

    def get_image(self) -> Surface:
        self.cursor_counter.update(engine.get_time())
        left = self.cursor_counter.count * TILEWIDTH * 2
        hovered_unit = self.get_hover()
        base_size = 32
        if self.draw_state == LevelCursorDrawState.Formation:
            if self.game.check_for_region(self.position, 'formation'):
                return engine.subsurface(self.formation_sprite, (0, 0, base_size, base_size))
            else:
                return engine.subsurface(self.formation_sprite, (base_size, 0, base_size, base_size))
        elif self.draw_state == LevelCursorDrawState.Combat:
            return engine.subsurface(self.red_sprite, (left, 0, base_size, base_size))
        elif self.draw_state == LevelCursorDrawState.Turnwheel:  # Green for turnwheel
            return engine.subsurface(self.green_sprite, (left, 0, base_size, base_size))
        elif hovered_unit and hovered_unit.team == 'player' and not hovered_unit.finished:
            return self.active_sprite
        else:
            return engine.subsurface(self.passive_sprite, (left, 0, base_size, base_size))

    def format_sprite(self, sprite):
        self.passive_sprite = engine.subsurface(sprite, (0, 0, 128, 32))
        self.red_sprite = engine.subsurface(sprite, (0, 32 , 128, 32))
        self.active_sprite = engine.subsurface(sprite, (0, 64, 32, 32))
        self.formation_sprite = engine.subsurface(sprite, (64, 64, 64, 32))
        self.green_sprite = engine.subsurface(sprite, (0, 96, 128, 32))

    def draw(self, surf, cull_rect):
        if self.draw_state != LevelCursorDrawState.Hidden:
            surf = super().draw(surf, cull_rect)
        return surf

    def draw_arrows(self, surf, cull_rect):
        if self._display_arrows:
            for arrow in self.arrows:
                surf = arrow.draw(surf, cull_rect)

            draw_unit_sprite = False
            if self.cur_unit:    
                if self.path and len(self.path) > 1 and \
                        self.position == self.path[0]:
                    draw_unit_sprite = True
                    x_offset = self.path[0][0] - self.path[1][0]
                    y_offset = self.path[0][1] - self.path[1][1]
                elif self.position in set(skill_system.witch_warp(self.cur_unit)):
                    draw_unit_sprite = True
                    x_offset, y_offset = 0, 0

            if draw_unit_sprite and DB.constants.value('translucent_unit_sprite'):
                # Draw unit's sprite
                if x_offset > 0:
                    state = 'right'
                elif x_offset < 0:
                    state = 'left'
                elif y_offset < 0:
                    state = 'up'
                else:
                    state = 'down'
                active_sprite = self.cur_unit.sprite.create_image(state)
                active_sprite = image_mods.make_translucent(active_sprite.convert_alpha(), .5)
                x_pos = self.position[0] * TILEWIDTH - cull_rect[0] - max(0, (active_sprite.get_width() - 16)//2)
                y_pos = self.position[1] * TILEHEIGHT - cull_rect[1] - 24
                surf.blit(active_sprite, (x_pos, y_pos))
        return surf

class Arrow(object):
    sprite = SPRITES.get('movement_arrows')
    sprite = image_mods.make_translucent(sprite, 0.1)

    def __init__(self, x, y, position, idx):
        self.image = engine.subsurface(self.sprite, (x * TILEWIDTH, y * TILEHEIGHT, TILEWIDTH, TILEHEIGHT))
        self.position = position
        self.idx = idx

    def draw(self, surf, cull_rect):
        if DB.constants.value('translucent_unit_sprite'):
            t = math.sin(math.radians((engine.get_time()//5 - self.idx * 6) % 180))
            new_color = image_mods.blend_colors((200, 40, 0), (0, 0, 0), t)
            image = image_mods.change_color(self.image, new_color)
        else:
            image = self.image

        x, y = self.position
        topleft = x * TILEWIDTH - cull_rect[0], y * TILEHEIGHT - cull_rect[1]        
        surf.blit(image, topleft)
        return surf
