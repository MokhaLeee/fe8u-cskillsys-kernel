import logging
from typing import Any, Tuple

from app.constants import TILEHEIGHT, TILEWIDTH, TILEX, TILEY
from app.engine import engine
from app.engine.camera import Camera
from app.engine.fluid_scroll import FluidScroll
from app.engine.input_manager import get_input_manager
from app.engine.sound import get_sound_thread
from app.engine.sprites import SPRITES
from app.utilities.direction import Direction
from app.utilities.utils import frames2ms, tclamp, tmult, tuple_sub
from app.engine.engine import Surface

class BaseCursor():
    """Basic Cursor Class, contains universal cursor functionality.

    Camera and GameBoard are optional, but unlock some additional functionality
    such as automatic camera manipulation, and cursor movement boundaries.
    """
    TRANSITION_FRAMES = 4
    
    def __init__(self, camera: Camera = None, game_board=None):
        # internal scroll controller
        self.fluid: FluidScroll = FluidScroll(frames2ms(4), 3.25)

        # fellow controllers
        self.camera = camera
        self.game_board = game_board

        # some state information
        self.mouse_mode: bool = False
        self.position: Tuple[int, int] = (0, 0)
        self._bounds: Tuple[int, int, int, int] = None
        self.visible: bool = False

        # default settings
        self._sprite: Surface = SPRITES.get('cursor')
        self._sprite_dim: Tuple[int, int] = (32, 32)

        # used for animating between squares
        # gba formula: 4 frames, linear interpolation
        self.offset_x, self.offset_y = 0, 0
        self._transition_duration = frames2ms(self.TRANSITION_FRAMES)
        self._transition_speed = 1
        self._transition_remaining: Tuple[int, int] = (0, 0)
        self._transition_direction: Tuple[Direction, Direction] = (Direction.LEFT, Direction.UP)
        self._transition_start: Tuple[int, int] = (0, 0)

    def hide(self):
        self.visible = False

    def show(self):
        self.visible = True

    def is_hidden(self) -> bool:
        return not self.visible

    def get_bounds(self) -> Tuple[int, int, int, int]:
        """Boundaries of the cursor traversal. Useful if you don't want
        the cursor to hover over the edges of the map. Format is
        min left, min top, max right, max bottom, i.e. the furthest you can go
        left, top, right, and bottom. Defaults to (0, 0, game.tilemap.width - 1, game.tilemap.height - 1)

        Returns:
            Tuple[int, int, int, int]: boundary for cursor
        """
        if self._bounds:
            return self._bounds
        elif self.game_board:
            return self.game_board.bounds
        else:
            return (0, 0, TILEX - 1, TILEY - 1)

    @property
    def transition_duration(self):
        return self._transition_duration / self._transition_speed

    @property
    def transition_progress(self):
        return tclamp(tmult(self._transition_remaining, 1 / self.transition_duration), (0, 0), (2, 2))

    def get_image(self) -> Surface:
        """Returns the current image of the cursor.
        Use this to change/animate the cursor sprite.

        Returns:
            Surface: Sprite to be blitted
        """
        sprite = engine.subsurface(self._sprite, (0, 0, self._sprite_dim[0], self._sprite_dim[1]))
        return sprite

    def get_hover(self) -> Any:
        """Gets the object beneath the cursor.

        Returns:
            Any: any arbitrary object. Not defined for BaseCursor.
        """
        raise NotImplementedError('Tried to get_hover without implementing!')

    def autocursor(self, immediate=False):
        """Automatically moves the cursor to a point of interest.

        Args:
            immediate (bool): whether to ease the camera over
                to the point of interest or not.
        """
        if self.camera:
            if immediate:
                self.camera.force_center(0, 0)
            else:
                self.camera.set_center(0, 0)

    def set_pos(self, pos):
        logging.debug("New position %s", pos)
        bounds = self.get_bounds()
        if bounds[0] <= pos[0] <= bounds[2] and bounds[1] <= pos[1] <= bounds[3]:
            self.position = pos
            self.offset_x, self.offset_y = 0, 0
            if self.camera:
                self.camera.set_xy(*self.position)
        else:
            logging.error("Attempted to set cursor's position out of bounds! %s, %s", pos, bounds)

    def move(self, dx, dy, mouse=False, sound=True):
        x, y = self.position

        # Cursor Sound
        if mouse:
            pass  # No cursor sound in mouse mode, cause it's annoying
        else:
            get_sound_thread().stop_sfx('Select 5')
            if sound:
                get_sound_thread().play_sfx('Select 5')

        # queue transition
        transition_start = engine.get_time()
        if mouse:
            duration = self.transition_duration / 2
        else:
            duration = self.transition_duration
        final_x, final_y = x, y
        if dx != 0 and self._transition_remaining[0] == 0: # stationary, queued another move
            self._transition_direction = (Direction.parse_map_direction(dx, 0), self._transition_direction[1])
            self._transition_remaining = (duration, self._transition_remaining[1])
            self._transition_start = (transition_start, self._transition_start[1])
            final_x = x + dx
        elif (dx != 0 and
              self._transition_remaining[0] >= 0 and
              self._transition_remaining[0] <= self.transition_duration):
            this_transition_direction = (Direction.parse_map_direction(dx, 0), self._transition_direction[1])
            if this_transition_direction == self._transition_direction: # in motion; start a "slide"
                self._transition_start = (self._transition_start[0] + duration, self._transition_start[1])
                final_x = x + dx

        if dy != 0 and self._transition_remaining[1] == 0:
            self._transition_direction = (self._transition_direction[0], Direction.parse_map_direction(0, dy))
            self._transition_remaining = (self._transition_remaining[0], duration)
            self._transition_start = (self._transition_start[0], transition_start)
            final_y = y + dy
        elif (dy != 0 and
              self._transition_remaining[1] >= 0 and
              self._transition_remaining[1] <= self.transition_duration):
            this_transition_direction = (self._transition_direction[0], Direction.parse_map_direction(0, dy))
            if this_transition_direction == self._transition_direction: # in motion; start a "slide"
                self._transition_start = (self._transition_start[0], self._transition_start[1] + duration)
                final_y = y + dy

        self.position = final_x, final_y

    def _handle_move(self, directions):
        # handle the move
        dx, dy = 0, 0
        from_mouse = False

        # Handle keyboard first
        if directions:
            if 'LEFT' in directions and self.position[0] > self.get_bounds()[0]:
                dx = -1
            elif 'RIGHT' in directions and self.position[0] < self.get_bounds()[2]:
                dx = 1
            if 'UP' in directions and self.position[1] > self.get_bounds()[1]:
                dy = -1
            elif 'DOWN' in directions and self.position[1] < self.get_bounds()[3]:
                dy = 1
            self.mouse_mode = False

        # Handle mouse
        mouse_position = get_input_manager().get_mouse_position()
        if mouse_position:
            self.mouse_mode = True
        if self.mouse_mode:
            # Get the actual mouse position, irrespective if actually used recently
            mouse_pos = get_input_manager().get_real_mouse_position()
            if mouse_pos:
                from_mouse = True
                new_pos_x = (mouse_pos[0] + self.camera.get_x() * TILEWIDTH) // TILEWIDTH
                new_pos_y = (mouse_pos[1] + self.camera.get_y() * TILEHEIGHT) // TILEHEIGHT
                new_pos = int(new_pos_x), int(new_pos_y)
                new_pos = tclamp(new_pos, self.get_bounds()[:2], self.get_bounds()[2:])
                dpos = new_pos[0] - self.position[0], new_pos[1] - self.position[1]
                dx = dpos[0]
                dy = dpos[1]
                # self._transition_speed = 2

        if dx != 0 or dy != 0:
            # adjust camera accordingly
            self.move(dx, dy, mouse=from_mouse, sound=True)
            if self.camera:
                if from_mouse:
                    self.camera.mouse_x(self.position[0])
                    self.camera.mouse_y(self.position[1])
                else:
                    self.camera.cursor_x(self.position[0])
                    self.camera.cursor_y(self.position[1])

    def take_input(self):
        self.fluid.update()
        is_speed_state = self._transition_speed > 1
        directions = self.fluid.get_directions(double_speed=is_speed_state)

        self._handle_move(directions)

    def update_offset(self):
        # update offset for movement
        if self._transition_remaining != (0, 0):
            # update transition time
            current_time = engine.get_time()
            xdt = current_time - self._transition_start[0]
            ydt = current_time - self._transition_start[1]
            self._transition_remaining = (
                max(0, self.transition_duration - xdt),
                max(0, self.transition_duration - ydt))
            # update offset based on progress
            ox = TILEWIDTH * self.transition_progress[0] * Direction.which_horizontal_dir(self._transition_direction[0])
            oy = TILEHEIGHT * -1 * self.transition_progress[1] * Direction.which_vertical_dir(self._transition_direction[1])
            self.offset_x, self.offset_y = ox, oy
        else:
            # self._transition_speed = 1
            self.offset_x, self.offset_y = 0, 0

    def draw(self, surf: Surface, cull_rect: Tuple[int, int, int, int]):
        self.update_offset()
        if self.visible:
            image = self.get_image()
            x, y = self.position
            left = x * TILEWIDTH - max(0, (image.get_width() - TILEWIDTH)//2) - self.offset_x
            top = y * TILEHEIGHT - max(0, (image.get_height() - TILEHEIGHT)//2) - self.offset_y
            surf.blit(image, (left - cull_rect[0], top - cull_rect[1]))
        return surf
