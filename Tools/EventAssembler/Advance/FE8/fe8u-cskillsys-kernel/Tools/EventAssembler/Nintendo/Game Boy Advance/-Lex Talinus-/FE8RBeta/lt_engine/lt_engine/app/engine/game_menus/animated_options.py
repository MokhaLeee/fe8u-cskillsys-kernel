from __future__ import annotations

from typing import Optional

from app.data.database.database import DB
from app.engine import help_menu, text_funcs
from app.engine.game_menus.string_options import BaseOption
from app.engine.game_state import game
from app.engine.graphics.text.text_renderer import (anchor_align, render_text,
                                                    text_width)
from app.engine.objects.unit import UnitObject
from app.engine.unit_sprite import UnitSprite
from app.sprites import SPRITES
from app.utilities.enums import HAlignment
from app.utilities.typing import NID


class MapSpriteOptionUtils():
    @staticmethod
    def draw_map_sprite(surf, sprite: UnitSprite, x: int, y: int, active=False):
        if active:
            map_sprite = sprite.create_image('active')
        else:
            map_sprite = sprite.create_image('passive')
        surf.blit(map_sprite, (x - 20, y - 24 - 1))


class BasicUnitOption(BaseOption[UnitObject]):
    def __init__(self, idx: int, unit: Optional[UnitObject], display_value: str | None = None,  width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: NID = 'white',
                 align: HAlignment = HAlignment.LEFT):
        super().__init__(idx, unit, display_value, width, height, ignore)
        self._disp_value = text_funcs.translate(
            display_value or self.get_name())
        self._align = align
        self._color = text_color
        self._font = font

    @classmethod
    def from_nid(cls, idx, unit_nid: NID, display_value: str | None = None, width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: NID = 'white',
                 align: HAlignment = HAlignment.LEFT):
        unit_object = game.unit_registry.get(unit_nid)
        if not unit_object: # unit is unloaded/in DB?
            unit_prefab = DB.units.get(unit_nid, None)
            if unit_prefab:
                unit_object = UnitObject.from_prefab(unit_prefab)
        return cls(idx, unit_object, display_value, width, height, ignore, font, text_color, align)

    @classmethod
    def from_unit(cls, idx, value: UnitObject, display_value: str | None = None, width: int = 0,
                  height: int = 0, ignore: bool = False, font: NID = 'text', text_color: NID = 'white',
                  align: HAlignment = HAlignment.LEFT):
        return cls(idx, value, display_value, width, height, ignore, font, text_color, align)

    def width(self):
        return self._width or 104

    def set(self, val: Optional[UnitObject], disp_val: Optional[str] = None):
        self._value = val
        self._disp_value = text_funcs.translate(disp_val or self.get_name())

    def get_name(self):
        if self._value:
            return self._value.name
        return 'None'

    def get_color(self):
        if self.get_ignore():
            return 'grey'
        return 'white'

    @staticmethod
    def is_oversize():
        return True

    def get_help_box(self):
        if not self._help_box and self._value:
            self._help_box = help_menu.HelpDialog(
                self._value.desc, name=self._value.name)
        return self._help_box

    def draw_option(self, surf, x, y, active=False):
        display_text = self._disp_value or self.get_name()
        font = self._font
        if text_width(font, display_text) > self.width() - 20:
            font = 'narrow'
        blit_loc = anchor_align(x, self.width(), self._align, (20, 5)), y
        color = self.get_color()
        if self._value:
            MapSpriteOptionUtils.draw_map_sprite(
                surf, self._value.sprite, x, y, active)
        render_text(surf, [font], [display_text], [color], blit_loc)

    def draw(self, surf, x, y):
        self.draw_option(surf, x, y, False)

    def draw_highlight(self, surf, x, y, menu_width):
        # Draw actual highlight surf
        highlight_surf = SPRITES.get('menu_highlight')
        width = highlight_surf.get_width()
        for slot in range((menu_width - 10)//width):
            left = x + 5 + slot*width
            top = y + 9
            surf.blit(highlight_surf, (left, top))
        self.draw_option(surf, x, y, True)


class BasicKlassOption(BaseOption[str]):
    def __init__(self, idx: int, klass: str, display_value: str | None = None,  width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: NID = 'white',
                 align: HAlignment = HAlignment.LEFT):
        super().__init__(idx, klass, display_value, width, height, ignore)
        self._disp_value = text_funcs.translate(display_value or self._value)
        self._align = align
        self._color = text_color
        self._font = font

    def width(self):
        return self._width or 104

    def set(self, val: str, disp_val: Optional[str] = None):
        self._value = val
        self._disp_value = text_funcs.translate(disp_val or self._value)

    def get_color(self):
        if self.get_ignore():
            return 'grey'
        return 'white'

    @staticmethod
    def is_oversize():
        return True

    def get_help_box(self):
        db_class = DB.classes.get(self._value)
        if not self._help_box and db_class:
            self._help_box = help_menu.HelpDialog(
                db_class.desc, name=db_class.name)
        return self._help_box

    def draw_option(self, surf, x, y, active=False):
        display_text = self._disp_value or self._value
        font = self._font
        if text_width(font, display_text) > self.width() - 20:
            font = 'narrow'
        blit_loc = anchor_align(x, self.width(), self._align, (20, 5)), y
        color = self.get_color()
        # TODO find out how to get sprite from klass
        # MapSpriteOptionUtils.draw_map_sprite(surf, self._value.sprite, x, y, active)
        render_text(surf, [font], [display_text], [color], blit_loc)

    def draw(self, surf, x, y):
        self.draw_option(surf, x, y, False)

    def draw_highlight(self, surf, x, y, menu_width):
        # Draw actual highlight surf
        highlight_surf = SPRITES.get('menu_highlight')
        width = highlight_surf.get_width()
        for slot in range((menu_width - 10)//width):
            left = x + 5 + slot*width
            top = y + 9
            surf.blit(highlight_surf, (left, top))
        self.draw_option(surf, x, y, True)
