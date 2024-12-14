import math, string

from app.constants import TILEX, WINWIDTH, WINHEIGHT
from app.data.database.database import DB
from app.engine.game_menus import menu_options
from app.utilities import utils

from app.data.database import lore
from app.engine.sprites import SPRITES
from app.engine.fonts import FONT
from app.engine.input_manager import get_input_manager
from app.engine import engine, image_mods, icons, help_menu, item_system, gui, item_funcs
from app.engine.gui import ScrollBar
from app.engine.base_surf import create_base_surf
from app.engine.objects.item import ItemObject
from app.engine.objects.unit import UnitObject
from app.engine.game_state import game
from app.engine.achievements import Achievement

def draw_unit_top(surf, topleft, unit):
    x, y = topleft
    white_surf = SPRITES.get('prep_top')
    surf.blit(white_surf, (x - 6, y - white_surf.get_height()))
    icons.draw_chibi(surf, unit.portrait_nid, (x + 3, y - 35))
    FONT['text'].blit_center(unit.name, surf, (x + 68, y - 35))
    FONT['text-blue'].blit_right(str(unit.level), surf, (x + 72, y - 19))
    FONT['text-blue'].blit_right(str(unit.exp), surf, (x + 97, y - 19))

def make_bg_surf(shimmer):
    bg_surf = create_base_surf(104, 16 * DB.constants.total_items() + 8, 'menu_bg_base')
    if shimmer:
        img = SPRITES.get('menu_shimmer%d' % shimmer)
        bg_surf.blit(img, (bg_surf.get_width() - img.get_width() - 1, bg_surf.get_height() - img.get_height() - 5))
    bg_surf = image_mods.make_translucent(bg_surf, 0.1)
    return bg_surf

def draw_unit_face(surf, topleft, unit, right):
    x, y = topleft
    face_image, offset = icons.get_portrait(unit)
    if right:
        face_image = engine.flip_horiz(face_image)
    face_image = face_image.convert_alpha()
    face_image = engine.subsurface(face_image, (0, max(0, offset - 4), 96, 76))
    face_image = image_mods.make_translucent(face_image, 0.5)
    left = x + 4 + 1
    top = y + (16 * DB.constants.total_items() + 8) - 4 - 76
    engine.blit(surf, face_image, (left, top))

def draw_unit_items(surf, topleft, unit, include_top=False, include_bottom=True, include_face=False, right=True, shimmer=0):
    x, y = topleft
    if include_top:
        draw_unit_top(surf, topleft, unit)

    if include_bottom:
        bg_surf = make_bg_surf(shimmer)
        surf.blit(bg_surf, (x, y))

        if include_face:
            draw_unit_face(surf, topleft, unit, right)

        # Blit items
        for idx, item in enumerate(unit.nonaccessories):
            item_option = menu_options.ItemOption(idx, item)
            item_option.draw(surf, topleft[0] + 1, topleft[1] + idx * 16 + 4)
        for idx, item in enumerate(unit.accessories):
            item_option = menu_options.ItemOption(idx, item)
            item_option.draw(surf, topleft[0] + 1, topleft[1] + item_funcs.get_num_items(unit) * 16 + idx * 16 + 4)


def draw_unit_bexp(surf, topleft, unit, new_exp, new_bexp, current_bexp, include_top=False, include_bottom=True,
                   include_face=False, right=True, shimmer=0):
    x, y = topleft
    if include_top:
        draw_unit_top(surf, topleft, unit)

    if include_bottom:
        bg_surf = make_bg_surf(shimmer)
        surf.blit(bg_surf, (x, y))

        if include_face:
            draw_unit_face(surf, topleft, unit, right)

        # Blit bonus exp
        button_right = SPRITES.get('buttons').subsurface(1, 19, 13, 12)
        FONT['text-blue'].blit('Current/New EXP', surf, (topleft[0] + 2, topleft[1] + 4))
        FONT['text'].blit(str(unit.exp), surf, (topleft[0] + 2, topleft[1] + 20))
        surf.blit(button_right, (topleft[0] + 20, topleft[1] + 23))
        FONT['text'].blit(str(new_exp), surf, (topleft[0] + 35, topleft[1] + 20))

        FONT['text-blue'].blit('Current/New BEXP', surf, (topleft[0] + 2, topleft[1] + 36))
        FONT['text'].blit(str(current_bexp), surf, (topleft[0] + 2, topleft[1] + 52))
        surf.blit(button_right, (topleft[0] + 42, topleft[1] + 55))
        FONT['text'].blit(str(new_bexp), surf, (topleft[0] + 57, topleft[1] + 52))

class Cursor():
    def __init__(self, sprite=None):
        self.counter = 0
        self.anim = [0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1]
        if not sprite:
            self.sprite = SPRITES.get('menu_hand')
        else:
            self.sprite = sprite
        self.y_offset = 0

    def y_offset_up(self):
        self.y_offset = 12

    def y_offset_down(self):
        self.y_offset = -12

    def update(self):
        self.counter = (self.counter + 1) % len(self.anim)

    def update_y_offset(self):
        if self.y_offset > 0:
            self.y_offset = max(0, self.y_offset - 4)
        elif self.y_offset < 0:
            self.y_offset = min(0, self.y_offset + 4)

    def draw(self, surf, x, y):
        surf.blit(self.sprite, (x - 12 + self.anim[self.counter], y + 3 + self.y_offset))
        self.update_y_offset()
        return surf

    def draw_vert(self, surf, x, y):
        surf.blit(self.sprite, (x - 12, y + 3 + self.y_offset + self.anim[self.counter]))
        self.update_y_offset()
        return surf

class Simple():
    """
    Abstract menu class. Must implement personal draw function
    """

    def __init__(self, owner, options, topleft=None, background='menu_bg_base', info=None):
        self.owner = owner
        self.topleft = topleft
        self.background = background

        self.current_index = 0

        self.display_total_uses = False
        self.limit = 1000
        self.hard_limit = False
        self.scroll = 0
        self.y_offset = 0  # Make room for header

        self.options = []
        self.create_options(options, info)

        self.cursor = Cursor()
        self.scroll_bar = ScrollBar()
        self.next_scroll_time = 0
        self.draw_cursor = 1  # 0 No draw, 1 Regular, 2 Draw but no move

        self.takes_input = True
        self.info_flag = False

    def set_takes_input(self, val):
        self.takes_input = val

    def set_limit(self, val):
        self.limit = max(2, val)

    def set_color(self, colors):
        for idx, option in enumerate(self.options):
            option.color = colors[idx]

    def set_ignore(self, ignores):
        for idx, option in enumerate(self.options):
            if idx >= len(ignores):
                option.ignore = True
            else:
                option.ignore = ignores[idx]
        first_valid = self.get_first_option()
        if first_valid:
            self.current_index = first_valid.idx

    def set_cursor(self, val):
        self.draw_cursor = val

    def set_hard_limit(self, val):
        self.hard_limit = val
        self.update_options()

    def set_text(self, idx, text):
        self.options[idx].set_text(text)

    def toggle_info(self):
        self.info_flag = not self.info_flag

    def create_options(self, options, info_descs=None):
        self.options.clear()
        if not options:
            null_option = menu_options.NullOption(0)
            self.options.append(null_option)
        else:
            for idx, option in enumerate(options):
                option = menu_options.BasicOption(idx, option)
                if info_descs:
                    option.help_box = help_menu.HelpDialog(info_descs[idx])
                self.options.append(option)

    def get_current(self):
        if self.options:
            return self.options[self.current_index].get()

    def get_current_option(self):
        if self.options:
            return self.options[self.current_index]

    def get_first_option(self):
        for option in self.options:
            if not option.ignore:
                return option

    def get_last_option(self):
        for option in reversed(self.options):
            if not option.ignore:
                return option

    def get_current_index(self):
        return self.current_index

    def set_selection(self, option):
        for idx, opt in enumerate(self.options):
            if opt.get() == option:
                self.current_index = idx

    def mouse_move(self, idx):
        if engine.get_time() > self.next_scroll_time:
            did_scroll = self.move_to(idx)
            if did_scroll:
                self.next_scroll_time = engine.get_time() + 50

    def move_to(self, idx):
        scroll = self.scroll
        idx = utils.clamp(idx, 0, len(self.options) - 1)
        if self.options[idx].ignore:
            return False
        while self.current_index < idx:
            self.move_down(True)  # Higher idxs
        while self.current_index > idx:
            self.move_up(True)
        # If we did scroll
        return scroll != self.scroll

    def move_down(self, first_push=True):
        if first_push:
            self.current_index += 1
            if self.current_index > len(self.options) - 1:
                self.current_index = 0
                self.scroll = 0
            elif self.current_index > self.scroll + self.limit - 2 and self.scroll + self.limit < len(self.options):
                self.scroll += 1
            else:
                self.cursor.y_offset_down()
        else:
            if self.current_index < len(self.options) - 1:
                self.current_index += 1
                if self.current_index > self.scroll + self.limit - 2 and self.scroll + self.limit < len(self.options):
                    self.scroll += 1
                else:
                    self.cursor.y_offset_down()
        if self.limit < len(self.options):
            self.scroll = min(len(self.options) - self.limit, self.scroll)
        return first_push or self.current_index < len(self.options) - 1

    def move_up(self, first_push=True):
        if first_push:
            self.current_index -= 1
            if self.current_index < 0:
                self.current_index = len(self.options) - 1
                self.scroll = self.current_index - self.limit + 1
            elif self.current_index < self.scroll + 1:
                self.scroll -= 1
            else:
                self.cursor.y_offset_up()
        else:
            if self.current_index > 0:
                self.current_index -= 1
                if self.current_index < self.scroll + 1:
                    self.scroll -= 1
                else:
                    self.cursor.y_offset_up()
        self.scroll = max(0, self.scroll)
        return first_push or self.current_index > 0

    def update_options(self, options=None):
        if options is not None:
            bare_options = options
        else:
            bare_options = [option.get() for option in self.options if option.get() is not None]
        self.create_options(bare_options)
        self.current_index = utils.clamp(self.current_index, 0, len(self.options) - 1)

    def get_menu_width(self):
        max_width = max(option.width() for option in self.options)
        return max_width - max_width%8

    def get_menu_height(self):
        return self.y_offset + sum(option.height() for option in self.options[:self.limit]) + 8

    def get_topleft(self):
        if not self.topleft:
            if game.cursor.position[0] > TILEX//2 + game.camera.get_x():
                return (14, 8)
            else:
                return (WINWIDTH - self.get_menu_width() - 14, 8)
        elif self.topleft == 'center':
            return (WINWIDTH//2 - self.get_menu_width()//2, WINHEIGHT//2 - self.get_menu_height()//2)
        elif isinstance(self.topleft, tuple):
            if self.topleft[0] == 'center':
                return (WINWIDTH//2 - self.get_menu_width()//2, self.topleft[1])
            elif self.topleft[1] == 'center':
                return (self.topleft[0], WINHEIGHT//2 - self.get_menu_height()//2)
            else:
                return self.topleft
        elif isinstance(self.topleft, Simple):
            parent_topleft = self.topleft.get_topleft()
            if parent_topleft[0] < WINWIDTH//2:
                return (parent_topleft[0] - 20 + self.topleft.get_menu_width(), parent_topleft[1] + self.topleft.current_index * 16 + 4 - self.topleft.scroll * 16)
            else:
                return (WINWIDTH - 40 - self.topleft.get_menu_width(), parent_topleft[1] + self.topleft.current_index * 16 + 8 - self.topleft.scroll * 16)
        else:
            return self.topleft

    def update(self):
        if self.draw_cursor == 1:
            self.cursor.update()

    def draw_scroll_bar(self, surf, topleft):
        right = topleft[0] + self.get_menu_width()
        topright = (right, topleft[1])
        self.scroll_bar.draw(surf, topright, self.scroll, self.limit, len(self.options))

    # For mouse handling
    def get_rects(self):
        return NotImplementedError

    def handle_mouse(self) -> bool:
        mouse_position = get_input_manager().get_mouse_position()
        if mouse_position:
            mouse_x, mouse_y = mouse_position
            idxs, option_rects = self.get_rects()
            for idx, option_rect in zip(idxs, option_rects):
                x, y, width, height = option_rect
                if x <= mouse_x <= x + width and y <= mouse_y <= y + height:
                    self.mouse_move(idx)
                    return True
        return False

class Choice(Simple):
    disp_value = False

    def __init__(self, owner, options, topleft=None, background='menu_bg_base', info=None):
        self.horizontal = False
        self.is_convoy = False
        self.highlight = True
        self._bg_surf = None  # Stored bg
        super().__init__(owner, options, topleft, background, info)

        self.gem = True
        self.shimmer = 0

        self.stationary_cursor = Cursor()
        self.fake_cursor_idx = None

    def set_horizontal(self, val):
        self.horizontal = val
        self.update_options()

    def set_total_uses(self, val):
        self.display_total_uses = val
        self.update_options()

    def set_highlight(self, val):
        self.highlight = val
        self.update_options()

    def set_convoy(self, val):
        self.is_convoy = val
        self.update_options()

    def set_fake_cursor(self, val):
        self.fake_cursor_idx = val

    def create_options(self, options, info_descs=None):
        self.options.clear()
        if not options and not self.hard_limit:
            null_option = menu_options.NullOption(0)
            self.options.append(null_option)
        else:
            for idx, option in enumerate(options):
                if isinstance(option, ItemObject):
                    if self.display_total_uses:
                        option = menu_options.FullItemOption(idx, option)
                    elif self.is_convoy:
                        option = menu_options.ConvoyItemOption(idx, option, self.owner)
                    else:
                        option = menu_options.ItemOption(idx, option)
                    option.help_box = option.get_help_box()
                    self.options.append(option)
                elif isinstance(option, lore.Lore):
                    option = menu_options.LoreOption(idx, option)
                    self.options.append(option)
                else:
                    if self.horizontal:
                        option = menu_options.HorizOption(idx, option)
                    elif option:
                        option = menu_options.BasicOption(idx, option)
                    else:
                        option = menu_options.BasicOption(idx, option)
                        option.display_text = ' ' * 20  # 80 pixels
                    if info_descs:
                        desc = info_descs[idx]
                        if isinstance(desc, ItemObject): # Uses special item description for items
                            option.help_box = help_menu.ItemHelpDialog(desc)
                        elif desc:
                            option.help_box = help_menu.HelpDialog(desc)
                    self.options.append(option)

        if self.hard_limit:
            for num in range(self.limit - len(options)):
                option = menu_options.EmptyOption(len(options) + num)
                if self.is_convoy:
                    option._width = 112
                self.options.append(option)

    def move_down(self, first_push=True):
        if all(option.ignore for option in self.options):
            return False  # Skip

        did_move = False
        if first_push:
            did_move = super().move_down(True)
            while self.options[self.current_index].ignore:
                self.cursor.y_offset = 0  # Reset y offset
                did_move = super().move_down(True)
            if self.get_current_option() == self.get_first_option():
                self.cursor.y_offset = 0
        else:
            if any(not option.ignore for option in self.options[self.current_index+1:]):
                did_move = super().move_down(False)
                while self.options[self.current_index].ignore:
                    did_move = super().move_down(False)

        if self.horizontal:
            self.cursor.y_offset = 0
        if did_move and self.show_face():
            self.update_bg()  # Unstore bg
        return did_move

    def move_up(self, first_push=True):
        if all(option.ignore for option in self.options):
            return False  # Skip

        did_move = False
        if first_push:
            did_move = super().move_up(True)
            while self.options[self.current_index].ignore:
                self.cursor.y_offset = 0
                did_move = super().move_up(True)
            if self.get_current_option() == self.get_last_option():
                self.cursor.y_offset = 0

        else:
            if any(not option.ignore for option in self.options[:self.current_index]):
                did_move = super().move_up(False)
                while self.options[self.current_index].ignore:
                    did_move = super().move_up(False)

        if self.horizontal:
            self.cursor.y_offset = 0
        if did_move and self.show_face():
            self.update_bg()  # Unstore bg
        return did_move

    def update_options(self, options=None):
        super().update_options(options)
        if self.show_face():
            self.update_bg()  # Unstore bg

    def get_menu_width(self):
        if self.horizontal:
            return sum(option.width() + 8 for option in self.options) + 16
        else:
            return super().get_menu_width()

    def get_menu_height(self):
        if self.horizontal:
            return 24
        else:
            return super().get_menu_height()

    def show_face(self):
        return self.is_convoy or self.disp_value == 'sell'

    def update_bg(self):
        self._bg_surf = None

    def create_bg_surf(self):
        # Handle clear background
        if not self.background:
            if self._bg_surf and self._bg_surf.get_width() == self.get_menu_width() and self._bg_surf.get_height() == self.get_menu_height():
                pass
            else:
                self._bg_surf = engine.create_surface((self.get_menu_width(), self.get_menu_height()), transparent=True)
            return self._bg_surf

        if self.horizontal:
            width = self.get_menu_width()
            if self._bg_surf and self._bg_surf.get_width() == width:
                pass
            else:
                self._bg_surf = create_base_surf(width, 24, self.background)
                self._bg_surf = image_mods.make_translucent(self._bg_surf, .5)
            return self._bg_surf
        else:
            if self._bg_surf and self._bg_surf.get_width() == self.get_menu_width() + 2 and self._bg_surf.get_height() == self.get_menu_height() + 4:
                pass
            else:
                bg_surf = create_base_surf(self.get_menu_width(), self.get_menu_height(), self.background)
                surf = engine.create_surface((bg_surf.get_width() + 2, bg_surf.get_height() + 4), transparent=True)
                surf.blit(bg_surf, (2, 4))
                if self.gem:
                    if self.gem == 'brown':
                        surf.blit(SPRITES.get('menu_gem_brown'), (0, 0))
                    else:
                        surf.blit(SPRITES.get('menu_gem_small'), (0, 0))
                if self.shimmer != 0:
                    sprite = SPRITES.get('menu_shimmer%d' % self.shimmer)
                    surf.blit(sprite, (surf.get_width() - 1 - sprite.get_width(), surf.get_height() - 5 - sprite.get_height()))
                if self.show_face():
                    # Draw face
                    item = self.get_current()
                    unit = None
                    if item:
                        unit = game.get_unit(item.owner_nid)
                    if unit:
                        face_image, _ = icons.get_portrait(unit)
                        face_image = face_image.convert_alpha()
                        # face_image = engine.subsurface(face_image, (0, 0, 96, 76))
                        face_image = image_mods.make_translucent(face_image, 0.5)
                        surf.blit(face_image, (surf.get_width()//2 - face_image.get_width()//2 + 4, surf.get_height() - face_image.get_height() - 5))
                surf = image_mods.make_translucent(surf, .1)
                self._bg_surf = surf
            return self._bg_surf

    def draw(self, surf):
        if self.horizontal:
            surf = self.horiz_draw(surf)
        else:
            surf = self.vert_draw(surf)
            if self.info_flag:
                surf = self.vert_draw_info(surf)
        return surf

    def vert_draw_info(self, surf):
        help_box = self.options[self.current_index].help_box
        if not help_box:
            return surf
        topleft = self.get_topleft()
        idxs, rects = self.get_rects()
        rect = rects[self.current_index - self.scroll]
        if topleft[0] < WINWIDTH // 2:
            help_box.draw(surf, (rect[0] - 4, rect[1] + 16))
        else:
            help_box.draw(surf, (rect[0] + self.get_menu_width(), rect[1] + 16), right=True)
        return surf

    def vert_draw(self, surf, offset=None):
        topleft = self.get_topleft()
        if offset:
            topleft = (topleft[0] + offset[0], topleft[1] + offset[1])

        bg_surf = self.create_bg_surf()
        surf.blit(bg_surf, (topleft[0] - 2, topleft[1] - 4))

        draw_scroll_bar = False
        if len(self.options) > self.limit:
            draw_scroll_bar = True
            self.draw_scroll_bar(surf, topleft)

        start_index = self.scroll
        end_index = self.scroll + self.limit
        choices = self.options[start_index:end_index]
        running_height = self.y_offset
        menu_width = self.get_menu_width()
        if choices:
            for idx, choice in enumerate(choices):
                top = topleft[1] + 4 + running_height
                left = topleft[0]

                if self.highlight and idx + self.scroll == self.current_index and self.takes_input and self.draw_cursor:
                    choice.draw_highlight(surf, left, top, menu_width - 8 if draw_scroll_bar else menu_width)
                # elif self.highlight and idx + self.scroll == self.fake_cursor_idx:
                    # choice.draw_highlight(surf, left, top, menu_width)
                else:
                    choice.draw(surf, left, top)
                if idx + self.scroll == self.fake_cursor_idx:
                    self.stationary_cursor.draw(surf, left, top)
                if idx + self.scroll == self.current_index and self.takes_input and self.draw_cursor:
                    self.cursor.draw(surf, left, top)

                running_height += choice.height()
        else:
            FONT['text-grey'].blit("Nothing", surf, (self.topleft[0] + 16, self.topleft[1] + 4))
        return surf

    def horiz_draw(self, surf):
        topleft = self.get_topleft()

        bg_surf = self.create_bg_surf()
        surf.blit(bg_surf, topleft)

        start_index = self.scroll
        end_index = self.scroll + self.limit
        choices = self.options[start_index:end_index]
        running_width = 0
        if choices:
            for idx, choice in enumerate(choices):
                top = topleft[1] + 4
                left = topleft[0] + running_width
                width = choice.width()

                if self.highlight and idx == self.current_index and self.takes_input and self.draw_cursor:
                    choice.draw_highlight(surf, left, top, width + 14)
                elif self.highlight and idx == self.fake_cursor_idx:
                    choice.draw_highlight(surf, left, top, width + 14)
                else:
                    choice.draw(surf, left, top)
                if idx == self.fake_cursor_idx:
                    self.stationary_cursor.draw(surf, left, top)
                if idx == self.current_index and self.takes_input and self.draw_cursor:
                    self.cursor.draw(surf, left, top)

                running_width += choice.width() + 8
        else:
            FONT['text-grey'].blit("Nothing", bg_surf, (self.topleft[0] + 8, self.topleft[1] + 4))
        return surf

    # For mouse handling
    def get_rects(self):
        if self.horizontal:
            return self.get_horiz_rects()
        else:
            return self.get_vert_rects()

    def get_vert_rects(self):
        topleft = self.get_topleft()
        end_index = self.scroll + self.limit
        choices = self.options[self.scroll:end_index]
        running_height = self.y_offset
        idxs, rects = [], []
        for idx, choice in enumerate(choices):
            top = topleft[1] + 4 + running_height
            left = topleft[0]
            rect = (left, top, choice.width(), choice.height())
            rects.append(rect)
            idxs.append(self.scroll + idx)

            running_height += choice.height()
        return idxs, rects

    def get_horiz_rects(self):
        topleft = self.get_topleft()
        choices = self.options
        running_width = 0
        idxs, rects = [], []
        for idx, choice in enumerate(choices):
            top = topleft[1] + 4
            left = topleft[0] + running_width
            rect = (left, top, choice.width(), choice.height())
            rects.append(rect)
            idxs.append(idx)

            running_width += choice.width() + 8
        return idxs, rects

class Inventory(Choice):
    def create_options(self, options, info_desc=None):
        self.options.clear()
        # Assumes all options are Item Objects
        accessories = [option for option in options if item_system.is_accessory(self.owner, option)]
        items = [option for option in options if option not in accessories]
        num_items = item_funcs.get_num_items(self.owner)
        num_accessories = item_funcs.get_num_accessories(self.owner)
        # Get items
        for idx, item in enumerate(items):
            option = menu_options.ItemOption(idx, item)
            option.help_box = option.get_help_box()
            self.options.append(option)
        # Get empty options in the middle
        for num in range(num_items - len(items)):
            option = menu_options.EmptyOption(len(self.options) + num)
            self.options.append(option)
        # Get accessories
        for idx, item in enumerate(accessories):
            option = menu_options.ItemOption(idx, item)
            option.help_box = option.get_help_box()
            self.options.append(option)
        # Get empty options at the end
        for num in range(num_accessories - len(accessories)):
            option = menu_options.EmptyOption(len(self.options) + num)
            self.options.append(option)

class Shop(Choice):
    default_option = menu_options.ValueItemOption

    def __init__(self, owner, options, topleft=None, disp_value='sell', background='menu_bg_base', info=None, stock=None):
        self.disp_value = disp_value
        self.stock = stock
        super().__init__(owner, options, topleft, background, info)

    def get_menu_width(self):
        if self.stock:
            return 168
        else:
            return 152

    def decrement_stock(self):
        if self.stock:
            self.options[self.current_index].stock -= 1

    def get_stock(self):
        if self.stock:
            return self.options[self.current_index].stock
        else:
            return -1

    def set_stock(self, stock):
        self.stock = stock
        for idx, option in enumerate(self.options):
            option.stock[idx] = stock

    def create_options(self, options, info_descs=None):
        self.options.clear()
        for idx, option in enumerate(options):
            if self.stock:
                option = menu_options.StockValueItemOption(idx, option, self.disp_value, self.stock[idx])
            else:
                option = self.default_option(idx, option, self.disp_value)
            option.help_box = option.get_help_box()
            self.options.append(option)

        if self.hard_limit:
            for num in range(self.limit - len(options)):
                option = menu_options.EmptyOption(len(options) + num)
                if self.stock:
                    option._width = 168
                self.options.append(option)

class RepairShop(Shop):
    default_option = menu_options.RepairValueItemOption

class Trade(Simple):
    """
    Menu used for trading items between two units
    Built from two choice menus
    """

    trade_name_surf = SPRITES.get('trade_name')

    def __init__(self, initiator, partner):
        self.owner = initiator
        self.partner = partner
        self.info_flag = False

        full_items1 = self.get_items(self.owner)
        full_items2 = self.get_items(self.partner)

        self.menu1 = Choice(self.owner, full_items1, (11, 68))
        self.menu1.set_limit(DB.constants.total_items())
        self.menu1.set_hard_limit(True)  # Makes hard limit
        self.menu2 = Choice(self.partner, full_items2, (125, 68))
        self.menu2.set_limit(DB.constants.total_items())
        self.menu2.set_hard_limit(True)  # Makes hard limit
        self.menu2.set_cursor(0)

        self.selecting_hand = (0, 0)
        self.other_hand = None

        self._selected_option = None

    def get_items(self, unit):
        items = unit.nonaccessories
        accessories = unit.accessories
        if len(items) < item_funcs.get_num_items(unit):
            items = items[:] + [''] * (item_funcs.get_num_items(unit) - len(items))
        if len(accessories) < item_funcs.get_num_accessories(unit):
            accessories = accessories[:] + [''] * (item_funcs.get_num_accessories(unit) - len(accessories))
        return items + accessories

    def selected_option(self):
        return self._selected_option

    def unset_selected_option(self):
        self._selected_option = None
        self.selecting_hand = self.other_hand
        self.other_hand = None
        self.menu1.set_fake_cursor(None)
        self.menu2.set_fake_cursor(None)
        # handle cursor
        if self.selecting_hand[0] == 0:
            self.menu1.move_to(self.selecting_hand[1])
            self.selecting_hand = (0, self.menu1.current_index)
            self.menu1.set_cursor(1)
            self.menu2.set_cursor(0)
        else:
            self.menu2.move_to(self.selecting_hand[1])
            self.selecting_hand = (1, self.menu2.current_index)
            self.menu1.set_cursor(0)
            self.menu2.set_cursor(1)
        self.handle_mouse()  # If we are using mouse, we can ignore most of this

    def set_selected_option(self):
        self.other_hand = self.selecting_hand
        if self.selecting_hand[0] == 0:
            self._selected_option = self.menu1.options[self.selecting_hand[1]]
            good_options = [option for option in self.menu2.options if not option.ignore and option.get()]
            empty_options = [idx for idx, option in enumerate(self.menu2.options) if option.ignore or not option.get()]
            if len(good_options) > DB.constants.total_items():
                self.menu2.move_to(self.selecting_hand[1])
            else:
                if empty_options:
                    self.menu2.move_to(empty_options[0])
                else:
                    self.menu2.move_to(len(good_options))
            self.menu2.cursor.y_offset = 0
            self.selecting_hand = (1, self.menu2.current_index)
            self.menu1.set_fake_cursor(self.other_hand[1])
            self.menu2.set_cursor(1)
            self.menu1.set_cursor(0)
        else:
            self._selected_option = self.menu2.options[self.selecting_hand[1]]
            good_options = [option for option in self.menu1.options if not option.ignore and option.get()]
            empty_options = [idx for idx, option in enumerate(self.menu1.options) if option.ignore or not option.get()]
            if len(good_options) > DB.constants.total_items():
                self.menu1.move_to(self.selecting_hand[1])
            else:
                if empty_options:
                    self.menu1.move_to(empty_options[0])
                else:
                    self.menu1.move_to(len(good_options))
            self.menu1.cursor.y_offset = 0
            self.selecting_hand = (0, self.menu1.current_index)
            self.menu2.set_fake_cursor(self.other_hand[1])
            self.menu1.set_cursor(1)
            self.menu2.set_cursor(0)
        self.handle_mouse()  # If we are using mouse, we can ignore most of this

    def get_current_option(self):
        if self.selecting_hand[0] == 0:
            return self.menu1.options[self.selecting_hand[1]]
        else:
            return self.menu2.options[self.selecting_hand[1]]

    def update_options(self):
        full_items1 = self.get_items(self.owner)
        full_items2 = self.get_items(self.partner)
        self.menu1.update_options(full_items1)
        self.menu2.update_options(full_items2)

    def move_down(self, first_push=True):
        old_index = self.selecting_hand[1]
        if self.selecting_hand[0] == 0:
            self.menu1.current_index = self.selecting_hand[1]
            self.menu1.move_down(first_push)
            self.selecting_hand = (0, self.menu1.current_index)
        else:
            self.menu2.current_index = self.selecting_hand[1]
            self.menu2.move_down(first_push)
            self.selecting_hand = (1, self.menu2.current_index)
        return self.selecting_hand[1] != old_index

    def move_up(self, first_push=True):
        old_index = self.selecting_hand[1]
        if self.selecting_hand[0] == 0:
            self.menu1.current_index = self.selecting_hand[1]
            self.menu1.move_up(first_push)
            self.selecting_hand = (0, self.menu1.current_index)
        else:
            self.menu2.current_index = self.selecting_hand[1]
            self.menu2.move_up(first_push)
            self.selecting_hand = (1, self.menu2.current_index)
        return self.selecting_hand[1] != old_index

    def cursor_left(self):
        self.menu1.set_cursor(1)
        self.menu1.cursor.y_offset = 0
        self.menu2.set_cursor(0)
        if self.menu2.info_flag:
            self.menu1.toggle_info()
            self.menu2.toggle_info()

    def move_left(self):
        # Can't move left if no items
        if self.selecting_hand[0] == 1:
            idx = utils.clamp(self.selecting_hand[1], 0, len([option for option in self.menu1.options if not option.ignore]) - 1)
            self.menu1.move_to(idx)
            self.selecting_hand = (0, self.menu1.current_index)
            self.cursor_left()
            return True
        return False

    def cursor_right(self):
        self.menu2.set_cursor(1)
        self.menu2.cursor.y_offset = 0
        self.menu1.set_cursor(0)
        if self.menu1.info_flag:
            self.menu1.toggle_info()
            self.menu2.toggle_info()

    def move_right(self):
        if self.selecting_hand[0] == 0:
            idx = utils.clamp(self.selecting_hand[1], 0, len([option for option in self.menu2.options if not option.ignore]) - 1)
            self.menu2.move_to(idx)
            self.selecting_hand = (1, self.menu2.current_index)
            self.cursor_right()
            return True
        return False

    def toggle_info(self):
        if self.selecting_hand[0] == 0:
            self.menu1.toggle_info()
        else:
            self.menu2.toggle_info()

    def update(self):
        self.menu1.update()
        self.menu2.update()

    def draw(self, surf):
        # Draw trade names
        surf.blit(self.trade_name_surf, (-4, -1))
        surf.blit(self.trade_name_surf, (WINWIDTH - self.trade_name_surf.get_width() + 4, -1))
        FONT['text'].blit(self.owner.name, surf, (24 - FONT['text'].width(self.owner.name)//2, 0))
        FONT['text'].blit(self.partner.name, surf, (WINWIDTH - 24 - FONT['text'].width(self.partner.name)//2, 0))

        # Draw Portraits
        # Owner
        owner_surf = engine.create_surface((96, 80), transparent=True)
        icons.draw_portrait(owner_surf, self.owner, (0, 0))
        owner_surf = engine.subsurface(owner_surf, (0, 3, 96, 68))
        owner_surf = engine.flip_horiz(owner_surf)
        surf.blit(owner_surf, (11 + 52 - 48, 0))

        # Partner
        partner_surf = engine.create_surface((96, 80), transparent=True)
        icons.draw_portrait(partner_surf, self.partner, (0, 0))
        partner_surf = engine.subsurface(partner_surf, (0, 3, 96, 68))
        surf.blit(partner_surf, (125 + 52 - 48, 0))

        if self.menu1.info_flag:
            self.menu2.draw(surf)
            self.menu1.draw(surf)
        else:
            self.menu1.draw(surf)
            self.menu2.draw(surf)

        return surf

    def handle_mouse(self) -> bool:
        mouse_position = get_input_manager().get_mouse_position()
        did_move = False
        if mouse_position:
            mouse_x, mouse_y = mouse_position
            idxs1, option_rects1 = self.menu1.get_rects()
            idxs2, option_rects2 = self.menu2.get_rects()
            # Menu1
            for idx, option_rect in zip(idxs1, option_rects1):
                x, y, width, height = option_rect
                if x <= mouse_x <= x + width and y <= mouse_y <= y + height:
                    self.menu1.mouse_move(idx)
                    did_move = True
                    if self.selecting_hand[0] == 1:
                        self.cursor_left()
                    self.selecting_hand = (0, idx)
            # Menu2
            for idx, option_rect in zip(idxs2, option_rects2):
                x, y, width, height = option_rect
                if x <= mouse_x <= x + width and y <= mouse_y <= y + height:
                    self.menu2.mouse_move(idx)
                    did_move = True
                    if self.selecting_hand[0] == 0:
                        self.cursor_right()
                    self.selecting_hand = (1, idx)
        return did_move

class Table(Simple):
    def __init__(self, owner, options, layout, topleft=None, background='menu_bg_base', info=None):
        self.mode = None
        super().__init__(owner, options, topleft, background, info)

        self.rows, self.columns = layout
        self.limit = self.rows
        self.gem = False
        self.shimmer = 0

        self.stationary_cursor = Cursor()
        self.fake_cursor_idx = None

    def set_mode(self, mode):
        self.mode = mode
        self.update_options()

    def set_fake_cursor(self, val):
        self.fake_cursor_idx = val

    def create_options(self, options, info_descs=None):
        self.options.clear()
        if not options:
            null_option = menu_options.NullOption(0)
            self.options.append(null_option)
        for idx, option in enumerate(options):
            if isinstance(option, UnitObject):
                option = menu_options.UnitOption(idx, option)
                option.set_mode(self.mode)
            elif isinstance(option, Achievement):
                option = menu_options.AchievementOption(idx, option)
            else:
                option = menu_options.BasicOption(idx, option)
            self.options.append(option)

    def move_to(self, idx):
        scroll = self.scroll
        idx = utils.clamp(idx, 0, len(self.options) - 1)
        if self.options[idx].ignore:
            return
        self.current_index = idx
        row, col = self._true_coords(self.current_index)
        self.scroll = utils.clamp(self.scroll, row - self.rows + 1, row + self.rows - 1)
        # If we did scroll
        return scroll != self.scroll

    def _true_coords(self, idx: int) -> tuple:
        return (idx // self.columns, idx % self.columns)

    def _exists(self, row: int, col: int) -> bool:
        if col >= self.columns or row < 0 or col < 0:
            return False
        return (row * self.columns + col) < len(self.options)

    def _idx_coords(self, row: int, col: int) -> int:
        return row * self.columns + col

    def _get_bottom(self, col: int) -> int:
        if col >= len(self.options) % self.columns:
            return len(self.options) // self.columns - 1
        else:
            return len(self.options) // self.columns

    def _get_right(self, row: int) -> int:
        if row >= len(self.options) // self.columns:
            return (len(self.options) % self.columns) - 1
        else:
            return self.columns - 1

    def move_down(self, first_push=True):
        if all(option.ignore for option in self.options):
            return
        old_index = self.current_index
        row, col = self._true_coords(old_index)
        idx = old_index
        while True:
            if self.mode == 'objective_menu':
                row += 5
            else:
                row += 1
            if self._exists(row, col):
                pass
            elif first_push:
                row = 0
                self.scroll = 0
            else:
                # Set to most recent good option
                row = max(r for r in range(row) if self._exists(r, col) and not self.options[self._idx_coords(r, col)].ignore)
                idx = self._idx_coords(row, col)
                break
            idx = self._idx_coords(row, col)
            if row > self.scroll + self.rows - 5 and self.mode == 'objective_menu':
                self.scroll += 5
            elif row > self.scroll + self.rows - 2:
                self.scroll += 1
            elif row != 0:
                self.cursor.y_offset_down()
            if not self.options[idx].ignore:
                break
        self.current_index = idx
        if old_index == self.current_index:
            self.cursor.y_offset = 0
        num_rows = math.ceil(len(self.options) / self.columns)
        self.scroll = utils.clamp(self.scroll, 0, max(0, num_rows - self.rows))
        return old_index != self.current_index

    def move_up(self, first_push=True):
        if all(option.ignore for option in self.options):
            return
        old_index = self.current_index
        row, col = self._true_coords(old_index)
        num_rows = math.ceil(len(self.options) / self.columns)
        idx = old_index
        while True:
            if self.mode == 'objective_menu':
                row -= 5
            else:
                row -= 1
            if self._exists(row, col):
                pass
            elif first_push:
                row = self._get_bottom(col)
                self.scroll = num_rows - self.rows
            else:
                # Set to most recent good option
                row = min(r for r in range(num_rows) if self._exists(r, col) and not self.options[self._idx_coords(r, col)].ignore)
                idx = self._idx_coords(row, col)
                break
            idx = self._idx_coords(row, col)
            if row < self.scroll + 4 and self.mode == 'objective_menu':
                self.scroll -= 5
            elif row < self.scroll + 1:
                self.scroll -= 1
            elif row != self._get_bottom(col):
                self.cursor.y_offset_up()
            if not self.options[idx].ignore:
                break
        self.current_index = idx
        self.scroll = max(0, self.scroll)
        if old_index == self.current_index:
            self.cursor.y_offset = 0
        return old_index != self.current_index

    def move_right(self, first_push=True):
        if all(option.ignore for option in self.options):
            return
        old_index = self.current_index
        row, col = self._true_coords(old_index)
        idx = old_index
        num_rows = math.ceil(len(self.options) / self.columns)
        while True:
            col += 1
            if self._exists(row, col):
                pass
            elif idx >= len(self.options) - 1:
                break  # Don't move right because we are on the last row
            elif row < num_rows - 1:
                row += 1
                col = 0
                if row > self.scroll + self.rows - 2:
                    self.scroll += 1
                    self.scroll = utils.clamp(self.scroll, 0, max(0, num_rows - self.rows))
            else:
                # Set to most recent good option
                idx = max(i for i in range(len(self.options)) if not self.options[i].ignore)
                break
            idx = self._idx_coords(row, col)
            if not self.options[idx].ignore:
                break
        if not self.options[idx].ignore:
            self.current_index = idx
        return old_index != self.current_index

    def move_left(self, first_push=True):
        if all(option.ignore for option in self.options):
            return
        old_index = self.current_index
        row, col = self._true_coords(old_index)
        idx = old_index
        while True:
            col -= 1
            if self._exists(row, col):
                pass
            elif row > 0:
                row -= 1
                col = self._get_right(row)
                if row < self.scroll + 1:
                    self.scroll -= 1
                    self.scroll = max(0, self.scroll)
            else:
                # Set to most recent good option
                idx = min(i for i in range(len(self.options)) if not self.options[i].ignore)
                break
            idx = self._idx_coords(row, col)
            if not self.options[idx].ignore:
                break
        if not self.options[idx].ignore:
            self.current_index = idx
        return old_index != self.current_index

    def get_menu_width(self):
        max_width = max(option.width() - option.width()%8 for option in self.options)
        total_width = max_width * self.columns
        if self.mode == 'objective_menu':
            total_width = 120
        else:
            total_width = total_width - total_width%8
        if self.mode in ('unit', 'prep_manage'):
            total_width += 32
        return total_width

    def get_menu_height(self):
        max_height = max(option.height() for option in self.options)
        return (max_height - max_height%8) * self.rows + 8

    def create_bg_surf(self):
        bg_surf = create_base_surf(self.get_menu_width(), self.get_menu_height(), self.background)
        surf = engine.create_surface((bg_surf.get_width() + 2, bg_surf.get_height() + 4), transparent=True)
        surf.blit(bg_surf, (2, 4))
        if self.gem:
            if self.gem == 'brown':
                surf.blit(SPRITES.get('menu_gem_brown'), (0, 0))
            else:
                surf.blit(SPRITES.get('menu_gem_small'), (0, 0))
        if self.shimmer != 0:
            sprite = SPRITES.get('menu_shimmer%d' % self.shimmer)
            surf.blit(sprite, (surf.get_width() - sprite.get_width() - 1, surf.get_height() - sprite.get_height() - 5))
        surf = image_mods.make_translucent(surf, .1)
        return surf

    def draw_info(self, surf):
        help_box = self.options[self.current_index].help_box
        if not help_box:
            return surf
        topleft = self.get_topleft()
        idxs, rects = self.get_rects()
        rect = rects[self.current_index - self.scroll]
        if topleft[0] < WINWIDTH // 2:
            help_box.draw(surf, (rect[0] - 4, rect[1] + 16))
        else:
            help_box.draw(surf, (rect[0] + self.get_menu_width(), rect[1] + 16), right=True)
        return surf

    def draw_scroll_bar(self, surf, topleft):
        right = topleft[0] + self.get_menu_width()
        topright = (right, topleft[1])
        num_rows = math.ceil(len(self.options) / self.columns)
        option_height = 32 if self.mode == 'achievements' else 16
        self.scroll_bar.draw(surf, topright, self.scroll, self.rows, num_rows, option_height)

    def draw(self, surf):
        topleft = self.get_topleft()
        bg_surf = self.create_bg_surf()
        surf.blit(bg_surf, (topleft[0] - 2, topleft[1] - 4))

        draw_scroll_bar = False
        if len(self.options) > self.rows * self.columns:
            draw_scroll_bar = True
            self.draw_scroll_bar(surf, topleft)

        start_index = self.scroll * self.columns
        end_index = (self.scroll + self.rows) * self.columns
        choices = self.options[start_index:end_index]
        width = max(option.width() for option in self.options)
        width -= width%8
        height = max(option.height() for option in self.options)
        if choices:
            for idx, choice in enumerate(choices):
                top = topleft[1] + 4 + (idx // self.columns * height)
                left = topleft[0] + (idx % self.columns * width)
                if self.mode == 'objective_menu':
                    win_con = game.level.objective['win']
                    win_count = str(win_con).count(',')
                    if idx + (self.scroll * self.columns) < self.takes_input:
                        surf.blit(SPRITES.get('lowlight'), (topleft[0] + 2, topleft[1] + 12))
                        FONT['text-yellow'].blit("Win Conditions", surf, (topleft[0] + 4, topleft[1] + 4))
                        surf.blit(SPRITES.get('lowlight'), (topleft[0] + 2, topleft[1] + 44 + win_count * 16))
                        FONT['text-yellow'].blit("Loss Conditions", surf, (topleft[0] + 4, topleft[1] + 36 + win_count * 16))
                        if win_count > 3:
                            surf.blit(SPRITES.get('lowlight'), (topleft[0] + 2, topleft[1] + 12))
                            FONT['text-yellow'].blit("Win Conditions", surf, (topleft[0] + 4, topleft[1] + 4))
                    elif idx + (self.scroll * self.columns) == self.current_index and self.takes_input:
                        if self.current_index <= 6:
                            surf.blit(SPRITES.get('lowlight'), (topleft[0] + 2, topleft[1] + 12 + (win_count - self.scroll + 2) * 16))
                            FONT['text-yellow'].blit("Loss Conditions", surf, (topleft[0] + 4, topleft[1] + 4 + (win_count - self.scroll + 2) * 16))
                        elif self.current_index > 6 and win_count >= self.scroll - 2:
                            surf.blit(SPRITES.get('lowlight'), (topleft[0] + 2, topleft[1] + 12 + (win_count - self.scroll + 2) * 16))
                            FONT['text-yellow'].blit("Loss Conditions", surf, (topleft[0] + 4, topleft[1] + 4 + (win_count - self.scroll + 2) * 16))
                    choice.draw(surf, left, top)
                else:
                    if self.mode in ('unit', 'prep_manage'):
                        left += 16
                    if idx + (self.scroll * self.columns) == self.current_index and self.takes_input and self.draw_cursor:
                        choice.draw_highlight(surf, left, top, width - 8 if draw_scroll_bar else width)
                    elif idx + (self.scroll * self.columns) == self.fake_cursor_idx:
                        choice.draw_highlight(surf, left, top, width - 8 if draw_scroll_bar else width)
                    else:
                        choice.draw(surf, left, top)
                    if idx + (self.scroll * self.columns) == self.fake_cursor_idx:
                        self.stationary_cursor.draw(surf, left, top)
                    if idx + (self.scroll * self.columns) == self.current_index and self.takes_input and self.draw_cursor:
                        self.cursor.draw(surf, left, top)
        else:
            FONT['text-grey'].blit("Nothing", bg_surf, (topleft[0] + 16, topleft[1] + 4))

    # For mouse handling
    def get_rects(self):
        topleft = self.get_topleft()

        start_index = self.scroll * self.columns
        end_index = (self.scroll + self.limit) * self.columns
        choices = self.options[start_index:end_index]
        width = max(option.width() for option in self.options)
        width -= width%8
        height = max(option.height() for option in self.options)

        idxs, rects = [], []
        for idx, choice in enumerate(choices):
            top = topleft[1] + 4 + (idx // self.columns * height)
            left = topleft[0] + (idx % self.columns * width)
            if self.mode in ('unit', 'prep_manage'):
                left += 16
            rect = (left, top, width, height)
            rects.append(rect)
            idxs.append(start_index + idx)
        return idxs, rects

class Convoy():
    trade_name_surf = SPRITES.get('trade_name')

    def __init__(self, owner, topleft, include_other_units=True, disp_value=False):
        self.unit = self.owner = owner  # Unit that's at the convoy
        self.topleft = topleft
        self.disp_value = disp_value
        self.takes_input = True
        self.include_other_units = include_other_units

        self.order = [w.nid for w in DB.weapons]
        self.build_menus()

        self._info_flag = False  # Whether to show item info
        self.selection_index = 0  # 0 is inventory, 1+ is convoy
        self.menu_index = 0  # Position for convoy
        self.locked = False  # Whether you are locked to convoy or inventory
        self.goto_flag = False  # Whether you should go based on selectio index or menu index

        # Handle arrows
        x, y = self.topleft
        self.menu_width = 152 if self.disp_value else 112
        self.left_arrow = gui.ScrollArrow('left', (x - 4, y - 14))
        self.right_arrow = gui.ScrollArrow('right', (x + self.menu_width - 4, y - 14), 0.5)

        self.owner_surf = create_base_surf(96, 24)
        self.owner_surf = image_mods.make_translucent(self.owner_surf, .4)

        for name, menu in self.menus.items():
            menu.set_takes_input(False)

    def build_menus(self):
        sorted_dict = self.get_sorted_dict()
        self.menus = {}
        for w_type in self.order:
            new_menu = Choice(self.owner, sorted_dict[w_type], self.topleft)
            new_menu.set_convoy(True)
            new_menu.set_limit(7)
            new_menu.set_hard_limit(True)
            new_menu.gem = False
            new_menu.shimmer = 2
            self.menus[w_type] = new_menu

        height = DB.constants.total_items() * 16 + 8
        self.inventory = Inventory(self.owner, self.owner.items, (12, WINHEIGHT - height - 4))
        self.inventory.gem = False
        self.inventory.shimmer = 2

    def get_sorted_dict(self):
        convoy = game.party.convoy
        all_items = []
        all_items += convoy
        if self.include_other_units:
            for unit in game.get_units_in_party():
                if unit.nid != self.unit.nid:
                    items = item_funcs.get_all_tradeable_items(unit)
                    all_items += items

        sorted_dict = {}
        for w_type in self.order:
            if w_type != 'Default':
                sorted_dict[w_type] = [item for item in all_items if item_system.weapon_type(self.unit, item) == w_type]
        sorted_dict['Default'] = [item for item in all_items if item_system.weapon_type(self.unit, item) is None]
        for key, value in sorted_dict.items():
            value.sort(key=lambda item: item_system.special_sort(self.unit, item) or 0)
            value.sort(key=lambda item: item.name)
            value.sort(key=lambda item: item_system.sell_price(self.unit, item) or 0)
            value.sort(key=lambda item: item_system.full_price(self.unit, item) or 0)
            value.sort(key=lambda item: bool(item.owner_nid))

        return sorted_dict

    def update_options(self):
        if self.inventory:
            self.inventory.update_options(self.owner.items)
        sorted_dict = self.get_sorted_dict()
        for name, menu in self.menus.items():
            menu.update_options(sorted_dict[name])

    def get_menu(self) -> Choice:
        return self.menus[self.order[self.menu_index]]

    def get_current(self):
        if self.selection_index == 0:
            return self.inventory.get_current()
        else:
            return self.get_menu().get_current()

    def get_convoy_current(self):
        return self.get_menu().get_current()

    def get_inventory_current(self):
        return self.inventory.get_current()

    def get_current_index(self):
        if self.selection_index == 0:
            return self.inventory.get_current_index()
        else:
            return self.get_menu().get_current_index()

    def get_scrolled_index(self):
        return self.get_menu().scroll

    def get_context(self):
        if self.selection_index == 0:
            return 'inventory'
        else:
            return 'convoy'

    def move_to_inventory(self):
        self.get_menu().set_takes_input(False)
        self.selection_index = 0
        self.inventory.set_takes_input(True)
        self.locked = True

    def move_to_convoy(self):
        self.inventory.set_takes_input(False)
        self.selection_index = self.menu_index + 1
        self.get_menu().set_takes_input(True)
        self.locked = True

    def move_to_item_type(self, item):
        wtype = item_system.weapon_type(self.unit, item)
        if wtype:
            self.menu_index = self.order.index(wtype)
        else:
            self.menu_index = len(self.order) - 1
        self.goto_flag = True

    def unlock(self):
        self.locked = False

    def move_down(self, first_push=True):
        if self.selection_index == 0:
            return self.inventory.move_down(first_push)
        else:
            return self.get_menu().move_down(first_push)

    def move_up(self, first_push=True):
        if self.selection_index == 0:
            return self.inventory.move_up(first_push)
        else:
            return self.get_menu().move_up(first_push)

    def move_left(self, first_push=True):
        if self.selection_index == 0:
            if self.locked:
                return False
            if first_push:
                self.selection_index = len(self.order)
                self.menu_index = len(self.order) - 1
                self.inventory.set_takes_input(False)
                self.get_menu().set_takes_input(True)
            else:
                return False
        elif self.selection_index == 1:
            if self.locked:
                self.get_menu().set_takes_input(False)
                self.selection_index = len(self.order)
                self.menu_index = len(self.order) - 1
                self.get_menu().set_takes_input(True)
            else:
                self.get_menu().set_takes_input(False)
                self.selection_index = 0
                self.inventory.set_takes_input(True)
        else:
            self.get_menu().set_takes_input(False)
            self.selection_index -= 1
            self.menu_index = self.selection_index - 1
            self.get_menu().set_takes_input(True)
            self.left_arrow.pulse()
        return True

    def move_right(self, first_push=True):
        if self.selection_index == 0:
            if self.locked:
                return False
            if first_push:
                if self.goto_flag:
                    self.selection_index = self.menu_index + 1
                else:
                    self.selection_index = 1
                    self.menu_index = 0
                self.goto_flag = False
                self.inventory.set_takes_input(False)
                self.get_menu().set_takes_input(True)
            else:
                return False
        elif self.selection_index == len(self.order):
            if self.locked:
                self.get_menu().set_takes_input(False)
                self.selection_index = 1
                self.menu_index = 0
                self.get_menu().set_takes_input(True)
            else:
                self.get_menu().set_takes_input(False)
                self.selection_index = 0
                self.inventory.set_takes_input(True)
        else:
            self.get_menu().set_takes_input(False)
            self.selection_index += 1
            self.menu_index = self.selection_index - 1
            self.get_menu().set_takes_input(True)
            self.right_arrow.pulse()
        return True

    def toggle_info(self):
        self._info_flag = not self._info_flag
        """
        if self.selection_index == 0:
            self.inventory.toggle_info()
        else:
            self.get_menu().toggle_info()
        """

    @property
    def info_flag(self):
        return self._info_flag
        """
        if self.selection_index == 0:
            return self.inventory.info_flag
        else:
            return self.get_menu().info_flag
        """

    def draw_info(self, surf):
        if self.selection_index == 0:
            self.inventory.vert_draw_info(surf)
        else:
            self.get_menu().vert_draw_info(surf)
        return surf

    def set_takes_input(self, val):
        self.takes_input = val

    def update(self):
        self.get_menu().update()
        if self.inventory:
            self.inventory.update()

    def draw(self, surf):
        surf.blit(self.trade_name_surf, (-4, -1))
        FONT['text'].blit(self.owner.name, surf, (24 - FONT['text'].width(self.owner.name)//2, 0))

        # Draw Portrait to left of menu
        # Owner
        if not self.disp_value:
            owner_surf = engine.create_surface((96, 80), transparent=True)
            icons.draw_portrait(owner_surf, self.owner, (0, 0))
            owner_surf = engine.subsurface(owner_surf, (0, 0, 96, 68))
            owner_surf = engine.flip_horiz(owner_surf)
            surf.blit(owner_surf, (18, 0))

        # Get item owner
        surf.blit(self.owner_surf, (156, 0))
        item = self.get_current()
        unit = None
        if item:
            unit = game.get_unit(item.owner_nid)

        # Draw item icons
        dist = (self.menu_width - 10)/len(self.order)
        for idx, weapon_nid in enumerate(reversed(self.order)):
            true_idx = len(self.order) - idx - 1
            if true_idx == self.selection_index - 1:
                pass
            else:
                topleft = self.topleft[0] + 3 + int(true_idx * dist), self.topleft[1] - 14
                icons.draw_weapon(surf, weapon_nid, topleft, gray=True)
        for idx, weapon_nid in enumerate(self.order):
            if idx == self.selection_index - 1:
                topleft = (self.topleft[0] + 3 + int(idx * dist), self.topleft[1] - 14)
                icons.draw_weapon(surf, weapon_nid, topleft)
                surf.blit(SPRITES.get('weapon_shine'), topleft)

        self.get_menu().draw(surf)
        if self.inventory:
            self.inventory.draw(surf)

        # Draw item owner
        if unit and self.takes_input:
            unit_str = "Owner: %s" % unit.name
        else:
            unit_str = "Owner: ---"
        FONT['text'].blit(unit_str, surf, (160, 4))

        self.left_arrow.draw(surf)
        self.right_arrow.draw(surf)
        return surf

    def handle_mouse(self) -> bool:
        mouse_position = get_input_manager().get_mouse_position()
        did_move = False
        if mouse_position:
            mouse_x, mouse_y = mouse_position

            main_menu = self.menus[self.order[self.selection_index - 1]]
            idxs, option_rects = main_menu.get_rects()
            for idx, option_rect in zip(idxs, option_rects):
                x, y, width, height = option_rect
                if x <= mouse_x <= x + width and y <= mouse_y <= y + height:
                    main_menu.mouse_move(idx)
                    did_move = True

            if self.inventory:  # Markets, which inherit from me, don't have inventories
                idxs, option_rects = self.inventory.get_rects()
                for idx, option_rect in zip(idxs, option_rects):
                    x, y, width, height = option_rect
                    if x <= mouse_x <= x + width and y <= mouse_y <= y + height:
                        self.inventory.mouse_move(idx)
                        did_move = True
        return did_move

class Market(Convoy):
    def __init__(self, owner, options, topleft, disp_value=None, show_stock: bool = False):
        self.options = options
        self.disp_value = disp_value
        self.show_stock = show_stock
        super().__init__(owner, topleft, disp_value=disp_value)
        self.selection_index = 1
        self.menu_index = 0
        self.inventory = None

    def build_menus(self):
        sorted_dict = self.get_sorted_dict()
        self.menus = {}
        for w_type in self.order:
            if self.show_stock:
                stock = [game.market_items[item.nid] for item in sorted_dict[w_type]]
            else:
                stock = None
            new_menu = Shop(self.owner, sorted_dict[w_type], self.topleft, self.disp_value, stock=stock)
            new_menu.set_limit(7)
            new_menu.set_hard_limit(True)
            new_menu.gem = False
            new_menu.shimmer = 2
            self.menus[w_type] = new_menu

    def get_sorted_dict(self):
        if self.options:
            all_items = self.options
        else:
            convoy = game.party.convoy
            all_items = []
            all_items += convoy
            for unit in game.get_units_in_party():
                items = item_funcs.get_all_tradeable_items(unit)
                all_items += items

        sorted_dict = {}
        for w_type in self.order:
            sorted_dict[w_type] = [item for item in all_items if item_system.weapon_type(self.unit, item) == w_type]
        sorted_dict['Default'] = [item for item in all_items if item_system.weapon_type(self.unit, item) is None]
        for key, value in sorted_dict.items():
            value.sort(key=lambda item: item_system.special_sort(self.unit, item) or 0)
            value.sort(key=lambda item: item.name)
            value.sort(key=lambda item: item_system.sell_price(self.unit, item) or 0)
            value.sort(key=lambda item: item_system.full_price(self.unit, item) or 0)
            value.sort(key=lambda item: bool(item.owner_nid))

        return sorted_dict

    def update_options(self):
        if self.inventory:
            self.inventory.update_options(self.owner.items)
        sorted_dict = self.get_sorted_dict()
        for name, menu in self.menus.items():
            if self.show_stock:
                menu.stock = [game.market_items[item.nid] for item in sorted_dict[name]]
            menu.update_bg()
            menu.update_options(sorted_dict[name])

    def get_stock(self):
        if self.show_stock:
            return self.menus[self.order[self.selection_index - 1]].get_stock()
        return -1

    def decrement_stock(self):
        if self.show_stock:
            self.menus[self.order[self.selection_index - 1]].decrement_stock()

    def get_current(self):
        return self.menus[self.order[self.selection_index - 1]].get_current()

    def get_current_index(self):
        return self.menus[self.order[self.selection_index - 1]].get_current_index()

    def get_context(self):
        return 'convoy'

    def move_down(self, first_push=True):
        return self.menus[self.order[self.selection_index - 1]].move_down(first_push)

    def move_up(self, first_push=True):
        return self.menus[self.order[self.selection_index - 1]].move_up(first_push)

    def move_left(self, first_push=True):
        if self.selection_index == 1:
            if first_push:
                self.selection_index = len(self.order)
                self.menu_index = len(self.order) - 1
            else:
                self.selection_index = 1
        else:
            self.selection_index -= 1
            self.menu_index = self.selection_index - 1
            self.left_arrow.pulse()

    def move_right(self, first_push=True):
        if self.selection_index == len(self.order):
            if first_push:
                self.selection_index = 1
                self.menu_index = 0
            else:
                self.selection_index = len(self.order)
        else:
            self.selection_index += 1
            self.menu_index = self.selection_index - 1
            self.right_arrow.pulse()

    # def toggle_info(self):
    #     self.menus[self.order[self.selection_index - 1]].toggle_info()

    def draw_info(self, surf):
        self.menus[self.order[self.selection_index - 1]].vert_draw_info(surf)
        return surf

    def set_takes_input(self, val):
        self.takes_input = val
        for menu in self.menus.values():
            menu.takes_input = val

class Main(Simple):
    def __init__(self, options, option_bg):
        self.limit = 1000
        self.hard_limit = False
        self.scroll = 0

        self.cursor1 = Cursor(SPRITES.get('cursor_dragon'))
        self.cursor2 = Cursor(engine.flip_horiz(SPRITES.get('cursor_dragon')))
        self.next_scroll_time = 0
        self.option_bg = option_bg

        self.options = []
        self.options = self.create_options(options)
        self.current_index = 0

        self.center = WINWIDTH//2, WINHEIGHT//2

    @property
    def cursor(self):
        return self.cursor1

    def create_options(self, options):
        self.options.clear()
        for idx, option in enumerate(options):
            option = menu_options.TitleOption(idx, option, self.option_bg)
            self.options.append(option)
        return self.options

    def update(self):
        self.cursor1.update()
        self.cursor2.update()

    def draw(self, surf, center=(WINWIDTH//2, WINHEIGHT//2), show_cursor=True):
        self.center = center
        num_options = len(self.options)
        for idx, option in enumerate(self.options):
            top = center[1] - (num_options/2.0 - idx) * (option.height() + 1)
            if self.current_index == idx:
                option.draw_highlight(surf, center[0], top)
            else:
                option.draw(surf, center[0], top)

        if show_cursor:
            height = center[1] - 12 - (num_options/2.0 - self.current_index) * (option.height() + 1)
            self.cursor1.draw_vert(surf, center[0] - option.width()//2 - 8 - 8, height)
            self.cursor2.draw_vert(surf, center[0] + option.width()//2 - 8 + 8, height)

    def get_rects(self):
        idxs, rects = [], []
        num_options = len(self.options)
        for idx, option in enumerate(self.options):
            top = self.center[1] - (num_options/2.0 - idx) * (option.height() + 1)
            left = self.center[0] - option.width()//2
            rect = (left, top, option.width(), option.height())
            rects.append(rect)
            idxs.append(self.scroll + idx)
        return idxs, rects

class ChapterSelect(Main):
    def __init__(self, options, colors):
        self.colors = colors
        super().__init__(options, 'chapter_select')
        self.use_rel_y = len(options) > 3
        self.rel_pos_y = 0

    def set_color(self, idx, color):
        self.colors[idx] = color
        self.options[idx].set_bg_color(color)

    def set_colors(self, colors):
        self.colors = colors
        for idx, option in enumerate(self.options):
            option.set_bg_color(self.colors[idx])

    def create_options(self, options):
        self.options.clear()
        for idx, option in enumerate(options):
            option = menu_options.ChapterSelectOption(idx, option, self.option_bg, self.colors[idx])
            self.options.append(option)
        return self.options

    def move_down(self, first_push=True):
        super().move_down(first_push)
        if self.use_rel_y:
            self.rel_pos_y -= 30

    def move_up(self, first_push=True):
        super().move_up(first_push)
        if self.use_rel_y:
            self.rel_pos_y += 30

    def update(self):
        super().update()
        if self.use_rel_y:
            if self.rel_pos_y > 0:
                self.rel_pos_y = max(0, self.rel_pos_y - 8)
            elif self.rel_pos_y < 0:
                self.rel_pos_y = min(0, self.rel_pos_y + 8)

    def draw(self, surf, center=(WINWIDTH//2, WINHEIGHT//2), show_cursor=True, flicker=False):
        self.center = center
        num_options = len(self.options)
        start_index = max(0, self.current_index - 3)
        num_shown = len(self.options[start_index:self.current_index + 3])
        for idx, option in enumerate(self.options[start_index:self.current_index + 3], start_index):
            diff = idx - self.current_index
            if self.use_rel_y:
                top = center[1] + diff * (option.height() + 1) + self.rel_pos_y
            else:
                top = center[1] + idx * (option.height() + 1) - (num_options * (option.height() + 1)//2)
            if self.current_index == idx:
                if flicker:
                    option.draw_flicker(surf, center[0], top)
                else:
                    option.draw_highlight(surf, center[0], top)
            else:
                option.draw(surf, center[0], top)

        if show_cursor and self.options:
            option = self.options[0]
            if self.use_rel_y:
                height = center[1] - 12
            else:
                height = center[1] - 12 - (num_shown/2.0 - (self.current_index - start_index)) * (option.height() + 1)
            self.cursor1.draw_vert(surf, center[0] - option.width()//2 - 8 - 8, height)
            self.cursor2.draw_vert(surf, center[0] + option.width()//2 - 8 + 8, height)

    def get_rects(self):
        idxs, rects = [], []
        num_options = len(self.options)
        start_index = max(0, self.current_index - 3)
        for idx, option in enumerate(self.options[start_index:self.current_index + 3], start_index):
            diff = idx - self.current_index
            if self.use_rel_y:
                top = self.center[1] + diff * (option.height() + 1) + self.rel_pos_y
            else:
                top = self.center[1] + idx * (option.height() + 1) - (num_options * (option.height() + 1)//2)
            left = self.center[0] - option.width()//2
            rect = (left, top, option.width(), option.height())
            rects.append(rect)
            idxs.append(self.scroll + idx)
        return idxs, rects

class ModeSelect(Main):
    def __init__(self, options):
        super().__init__(options, None)
        self.center = 68, WINHEIGHT//2

    @property
    def cursor(self):
        return self.cursor1

    def create_options(self, options):
        self.options.clear()
        for idx, option in enumerate(options):
            option = menu_options.ModeOption(idx, option)
            self.options.append(option)
        return self.options

    def update(self):
        self.cursor1.update()
        self.cursor2.update()

    def draw(self, surf, show_cursor=True):
        center = self.center
        num_options = len(self.options)
        for idx, option in enumerate(self.options):
            top = center[1] - (num_options/2.0 - idx) * (option.height() + 1)
            if self.current_index == idx:
                option.draw_highlight(surf, center[0], top)
            else:
                option.draw(surf, center[0], top)

        if show_cursor:
            height = center[1] - 12 - (num_options/2.0 - self.current_index) * (option.height() + 1)
            self.cursor1.draw_vert(surf, center[0] - option.width()//2 - 8 - 8, height)
            self.cursor2.draw_vert(surf, center[0] + option.width()//2 - 8 + 8, height)

class KeyboardMenu(Table):
    def __init__(self, header, character_limit=16, illegal_characters=[]):
        self.header = header
        self.character_limit = character_limit
        self.illegal_characters = illegal_characters
        self.name = ''
        self.topleft = (9, 42)

        self.all_characters = {}
        self.all_characters['uppercase'] = string.ascii_uppercase
        self.all_characters['lowercase'] = string.ascii_lowercase
        self.all_characters['uppercase_UTF8'] = \
            ['Á','À','Â','Ä','Å','Ç','Ð','É','È','Ê','Ë','Í','Ì','Î','Ï','Ñ','Ó','Ò','Ô','Ö','Ø','Þ','Ú','Ù','Û','Ü','Ý','Ÿ','Ƿ','Æ','Œ']
        self.all_characters['lowercase_UTF8'] = \
            ['á','à','â','ä','å','ç','ð','é','è','ê','ë','í','ì','î','ï','ñ','ó','ò','ô','ö','ø','þ','ú','ù','û','ü','ý','ÿ','ƿ','æ','œ']
        self.all_characters['numbers_and_punctuation'] = \
            ['0','1','2','3','4','5','6','7','8','9','!','¡','?','¿','&','-','+',';',':',"'",',','.','"'] + ([' '] * (26 - 7))

        for group in self.illegal_characters:
            if group in self.all_characters:
                self.all_characters.pop(group)

        self.options = []
        for character_list in self.all_characters.values():
            self.options.extend(character_list)

        # Build background
        self.backsurf = SPRITES.get('NamingScreen')

        super().__init__(None, self.options, (7, 26), self.topleft, self.backsurf, None)

    def create_options(self, options, info_descs=None):
        self.options.clear()
        for idx, option in enumerate(options):
            option = menu_options.SingleCharacterOption(idx, option)
            self.options.append(option)

    def updateName(self, character):
        if character == 'back':
            self.name = self.name[:-1]
        elif character == 'space':
            self.name += ' '
        else:
            self.name += character

    def draw(self, surf):
        surf.blit(self.backsurf, (0, 0))

        if self.header:
            FONT['text-blue'].blit(self.header, surf, (3, 2))

        draw_scroll_bar = False
        if len(self.options) > self.rows * self.columns:
            draw_scroll_bar = True
            self.draw_scroll_bar(surf, self.topleft)

        start_index = self.scroll * self.columns
        end_index = (self.scroll + self.rows) * self.columns
        choices = self.options[start_index:end_index]
        width = 8
        height = max(option.height() for option in self.options)
        if choices:
            for idx, choice in enumerate(choices):
                top = self.topleft[1] + 4 + (idx // self.columns * height)
                left = self.topleft[0] + (idx % self.columns * width)

                if idx + (self.scroll * self.columns) == self.current_index and self.takes_input and self.draw_cursor:
                    choice.draw_highlight(surf, left, top, width - 8 if draw_scroll_bar else width)
                elif idx + (self.scroll * self.columns) == self.fake_cursor_idx:
                    choice.draw_highlight(surf, left, top, width - 8 if draw_scroll_bar else width)
                else:
                    choice.draw(surf, left, top)
                if idx + (self.scroll * self.columns) == self.fake_cursor_idx:
                    self.stationary_cursor.draw(surf, left, top)
                if idx + (self.scroll * self.columns) == self.current_index and self.takes_input and self.draw_cursor:
                    self.cursor.draw(surf, left, top)

        else:
            FONT['text-grey'].blit("Nothing", surf, (self.topleft[0] + 16, self.topleft[1] + 4))

        FONT['text'].blit(self.name, surf, (6, 20))
        if len(self.name) < self.character_limit:
            self.draw_underscore(surf)

    def draw_underscore(self, surf):
        size = FONT['text'].size(self.name)
        left = 6 + size[0]
        top = 17 + size[1]
        underscore = SPRITES.get('Underscore')
        surf.blit(underscore, (left, top))
