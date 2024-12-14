from __future__ import annotations

from typing import Callable, Generic, Optional, TypeVar

from app.constants import COLORKEY
from app.data.resources.resources import RESOURCES
from app.engine import engine, help_menu, icons, text_funcs
from app.engine.game_state import game
from app.engine.graphics.text.text_renderer import (anchor_align, render_text,
                                                    text_width)
from app.sprites import SPRITES
from app.utilities.enums import HAlignment
from app.utilities.typing import NID, Protocol

T = TypeVar('T')


class IMenuOption(Generic[T]):
    get: Callable[[], T]
    set: Callable[[T, str], None]
    height: Callable[[], int]
    width: Callable[[], int]
    set_width: Callable[[int], None]
    get_ignore: Callable[[], bool]
    set_ignore: Callable[[bool], None]
    set_help_box: Callable[[help_menu.HelpDialog], None]
    get_help_box: Callable[[], Optional[help_menu.HelpDialog]]

    @property
    def help_box(self) -> help_menu.HelpDialog:
        pass

    @property
    def ignore(self) -> bool:
        pass
    draw: Callable[[engine.Surface, int, int], None]
    draw_highlight: Callable[[engine.Surface, int, int, int], None]
    is_oversize: Callable[[], bool]


class BaseOption(IMenuOption[T]):
    def __init__(self, idx: int, value: Optional[T], display_value: Optional[str] = None, width: int = 0,
                 height: int = 0, ignore: bool = False):
        self.idx: int = idx
        self._value: Optional[T] = value
        self._disp_value: str = display_value or ""
        self._width: int = width
        self._height: int = height
        self._ignore: bool = ignore
        self._help_box: Optional[help_menu.HelpDialog] = None

    def get(self):
        return self._value

    def set(self, val: T, disp_val: Optional[str] = None):
        raise NotImplementedError()

    def height(self):
        # 16 is standard text height
        return self._height or 16

    def width(self):
        # arbitrarily chosen
        return self._width or 80

    def set_width(self, width: int):
        self._width = width

    def get_ignore(self):
        return self._ignore

    def set_ignore(self, ignore: bool):
        self._ignore = ignore

    def set_help_box(self, help_box: help_menu.HelpDialog):
        self._help_box = help_box

    def get_help_box(self):
        return self._help_box

    @property
    def help_box(self) -> Optional[help_menu.HelpDialog]:
        return self.get_help_box()

    @help_box.setter
    def help_box(self, help_box: help_menu.HelpDialog):
        self.set_help_box(help_box)

    @property
    def ignore(self) -> bool:
        return self.get_ignore()

    @ignore.setter
    def ignore(self, ig: bool):
        self.set_ignore(ig)

    @staticmethod
    def is_oversize():
        return False

    def draw(self, surf, x, y):
        raise NotImplementedError()

    def draw_highlight(self, surf, x, y, menu_width):
        highlight_surf = SPRITES.get('menu_highlight')
        width = highlight_surf.get_width()
        for slot in range((menu_width - 10)//width):
            left = x + 5 + slot*width
            top = y + 9
            surf.blit(highlight_surf, (left, top))
        self.draw(surf, x, y)
        return surf


class TextOption(BaseOption[str]):
    def __init__(self, idx: int, value: str, display_value: str | None = None, width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: NID = 'white',
                 align: HAlignment = HAlignment.LEFT):
        super().__init__(idx, value, display_value, width, height, ignore)
        self._disp_value = text_funcs.translate(display_value or value)
        self._align = align
        self._color = text_color
        self._font = font

    def set(self, val: str, disp_val: Optional[str] = None):
        self._value = val
        self._disp_value = text_funcs.translate(disp_val or val)

    def width(self):
        return self._width or text_width(self._font, self._disp_value) + 24

    def get_color(self):
        if self.get_ignore():
            return 'grey'
        return self._color

    def set_color(self, color: NID):
        self._color = color

    def draw(self, surf, x, y):
        blit_loc = anchor_align(x, self.width(), self._align, (5, 5)), y
        render_text(surf, [self._font], [self._disp_value], [
                    self.get_color()], blit_loc, self._align)


class NarrowOption(TextOption):
    def __init__(self, idx: int, value: str, display_value: str | None = None, width: int = 0, height: int = 0, ignore: bool = False, font: NID = 'text', text_color: NID = 'white', align: HAlignment = HAlignment.LEFT):
        super().__init__(idx, value, display_value, width,
                         height, ignore, font, text_color, align)

    def width(self):
        return self._width or text_width(self._font, self._disp_value)


class NullOption(BaseOption[None]):
    def __init__(self, idx: int, display_text: str = 'Nothing', width: int = 0, font: NID = 'text', text_color: NID = 'white', align: HAlignment = HAlignment.LEFT):
        super().__init__(idx, None, display_text, width)
        self._align = align
        self._color = text_color
        self._font = font

    def set(self, val, disp_val):
        return

    def draw(self, surf, x, y):
        blit_loc = anchor_align(x, self.width(), self._align, (5, 5)), y
        render_text(surf, [self._font], [self._disp_value],
                    [self._color], blit_loc, self._align)


class BasicPortraitOption(BaseOption[str]):
    def __init__(self, idx: int, portrait_nid: str, width: int = 0,
                 height: int = 0, ignore: bool = False):
        super().__init__(idx, portrait_nid, portrait_nid, width, height, ignore)

    def width(self):
        return self._width or 96

    def height(self):
        return self._height or 80

    def set(self, portrait_nid: str, _: Optional[str] = None):
        self._value = portrait_nid

    def draw(self, surf, x, y):
        portrait = RESOURCES.portraits.get(self._value)
        if portrait:
            main_portrait_coords = (0, 0, 96, 80)
            if not portrait.image:
                portrait.image = engine.image_load(portrait.full_path)
            portrait.image = portrait.image.convert()
            engine.set_colorkey(portrait.image, COLORKEY, rleaccel=True)
            main_portrait = engine.subsurface(
                portrait.image, main_portrait_coords)
            surf.blit(main_portrait, (x, y))

    def draw_highlight(self, surf, x, y, menu_width):
        self.draw(surf, x, y)


class BasicChibiOption(BaseOption[str]):
    def __init__(self, idx: int, portrait_nid: str, width: int = 0,
                 height: int = 0, ignore: bool = False):
        super().__init__(idx, portrait_nid, portrait_nid, width, height, ignore)

    def width(self):
        return self._width or 32

    def height(self):
        return self._height or 32

    def set(self, portrait_nid: str, _: Optional[str] = None):
        self._value = portrait_nid

    def draw(self, surf, x, y):
        chibi_surf = engine.create_surface((32, 32), True)
        chibi = icons.draw_chibi(chibi_surf, self._value, (0, 0))
        surf.blit(chibi, (x, y))

    def draw_highlight(self, surf, x, y, menu_width):
        self.draw(surf, x, y)
