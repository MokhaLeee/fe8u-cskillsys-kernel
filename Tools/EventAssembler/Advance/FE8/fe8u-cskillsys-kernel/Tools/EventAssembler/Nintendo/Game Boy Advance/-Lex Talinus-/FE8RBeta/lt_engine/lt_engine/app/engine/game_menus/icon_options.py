from __future__ import annotations

from enum import Enum
from typing import Optional, Tuple

from app.data.database.database import DB
from app.engine import help_menu, icons, item_funcs, item_system, text_funcs
from app.engine.game_menus.string_options import BaseOption
from app.engine.game_state import game
from app.engine.graphics.text.text_renderer import (anchor_align, render_text,
                                                    text_width)
from app.engine.objects.item import ItemObject
from app.engine.objects.skill import SkillObject
from app.utilities.enums import HAlignment
from app.utilities.typing import NID


class ItemOptionUtils():
    @staticmethod
    def draw_icon(surf, x, y, item: ItemObject):
        icon = icons.get_icon(item)
        if icon:
            surf.blit(icon, (x + 2, y))

    @staticmethod
    def draw_without_uses(surf, x, y, item: ItemObject, font: NID, color: NID, width: int,
                          align: HAlignment = HAlignment.LEFT, disp_text: Optional[str] = None):
        display_text = disp_text or item.name
        if text_width(font, display_text) > width - 20:
            font = 'narrow'
        blit_loc = anchor_align(x, width, align, (20, 5)), y
        if align == HAlignment.RIGHT:
            ItemOptionUtils.draw_icon(surf, blit_loc[0] - 20 - text_width(font, display_text), y, item)
        elif align == HAlignment.CENTER:
            blit_loc = (blit_loc[0] + 10, blit_loc[1])
            ItemOptionUtils.draw_icon(surf, blit_loc[0] - 20 - text_width(font, display_text) // 2, y, item)
        else:
            ItemOptionUtils.draw_icon(surf, x, y, item)
        render_text(surf, [font], [display_text], [color], blit_loc, align)

    @staticmethod
    def draw_with_uses(surf, x, y, item: ItemObject, font: NID, color: NID, uses_color: NID,
                       width: int, align: HAlignment = HAlignment.LEFT,
                       disp_text: Optional[str] = None):
        display_text = disp_text or item.name
        uses_font = font
        if text_width(font, display_text) > width - 36:
            font = 'narrow'
        blit_loc = anchor_align(x, width, align, (20, 16)), y
        if align == HAlignment.RIGHT:
            ItemOptionUtils.draw_icon(surf, blit_loc[0] - 20 - text_width(font, display_text), y, item)
        elif align == HAlignment.CENTER:
            blit_loc = (blit_loc[0] + 10, blit_loc[1])
            ItemOptionUtils.draw_icon(surf, blit_loc[0] - 20 - text_width(font, display_text) // 2, y, item)
        else:
            ItemOptionUtils.draw_icon(surf, x, y, item)
        render_text(surf, [font], [display_text], [color], blit_loc, align)
        uses_string = '--'
        if item.uses:
            uses_string = str(item.data['uses'])
        elif item.parent_item and item.parent_item.uses and item.parent_item.data['uses']:
            uses_string = str(item.parent_item.data['uses'])
        elif item.c_uses:
            uses_string = str(item.data['c_uses'])
        elif item.parent_item and item.parent_item.c_uses and item.parent_item.data['c_uses']:
            uses_string = str(item.parent_item.data['c_uses'])
        elif item.cooldown:
            uses_string = str(item.data['cooldown'])
        uses_loc = anchor_align(x, width, HAlignment.RIGHT, (0, 5)), y
        render_text(surf, [uses_font], [uses_string], [
                    uses_color], uses_loc, HAlignment.RIGHT)

    @staticmethod
    def draw_with_full_uses(surf, x, y, item: ItemObject, font: NID, color: NID, uses_color: NID,
                            width: int, align: HAlignment = HAlignment.LEFT,
                            disp_text: Optional[str] = None):
        main_font = font
        display_text = disp_text or item.name
        if text_width(main_font, display_text) > width - 56:
            main_font = 'narrow'
        uses_font = font
        blit_loc = anchor_align(x, width, align, (20, 36)), y
        if align == HAlignment.RIGHT:
            ItemOptionUtils.draw_icon(surf, blit_loc[0] - 20 - text_width(font, display_text), y, item)
        elif align == HAlignment.CENTER:
            blit_loc = (blit_loc[0] + 10, blit_loc[1])
            ItemOptionUtils.draw_icon(surf, blit_loc[0] - 20 - text_width(font, display_text) // 2, y, item)
        else:
            ItemOptionUtils.draw_icon(surf, x, y, item)
        render_text(surf, [main_font], [display_text],
                    [color], blit_loc, align)

        uses_string_a = '--'
        uses_string_b = '--'
        if item.data.get('uses') is not None:
            uses_string_a = str(item.data['uses'])
            uses_string_b = str(item.data['starting_uses'])
        elif item.data.get('c_uses') is not None:
            uses_string_a = str(item.data['c_uses'])
            uses_string_b = str(item.data['starting_c_uses'])
        elif item.parent_item and item.parent_item.data.get('uses') is not None:
            uses_string_a = str(item.parent_item.data['uses'])
            uses_string_b = str(item.parent_item.data['starting_uses'])
        elif item.parent_item and item.parent_item.data.get('c_uses') is not None:
            uses_string_a = str(item.parent_item.data['c_uses'])
            uses_string_b = str(item.parent_item.data['starting_c_uses'])
        elif item.data.get('cooldown') is not None:
            uses_string_a = str(item.data['cooldown'])
            uses_string_b = str(item.data['starting_cooldown'])
        uses_string_a_loc = anchor_align(
            x, width, HAlignment.RIGHT, (0, 24)), y
        uses_string_b_loc = anchor_align(x, width, HAlignment.RIGHT, (0, 0)), y
        slash_loc = anchor_align(x, width, HAlignment.RIGHT, (0, 16)), y
        render_text(surf, [uses_font], [uses_string_a], [
                    uses_color], uses_string_a_loc, HAlignment.RIGHT)
        render_text(surf, [uses_font], ["/"], [], slash_loc, HAlignment.RIGHT)
        render_text(surf, [uses_font], [uses_string_b], [
                    uses_color], uses_string_b_loc, HAlignment.RIGHT)


class ItemOptionModes(Enum):
    NO_USES = 0
    USES = 1
    FULL_USES = 2
    FULL_USES_AND_REPAIR = 3
    VALUE = 4
    STOCK_AND_VALUE = 5


class BasicItemOption(BaseOption[Optional[ItemObject]]):
    def __init__(self, idx: int, item: Optional[ItemObject] = None, display_value: str | None = None,  width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: Optional[NID] = None,
                 align: HAlignment = HAlignment.LEFT, mode: ItemOptionModes = ItemOptionModes.NO_USES):
        super().__init__(idx, item, display_value, width, height, ignore)
        self._disp_value = text_funcs.translate(
            display_value or (self._value.name if self._value else "None"))
        self._align = align
        self._color = text_color
        self._font = font
        self._mode = mode

    @classmethod
    def from_nid(cls, idx, item_nid: NID, display_value: str | None = None, width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: Optional[NID] = None,
                 align: HAlignment = HAlignment.LEFT, mode: ItemOptionModes = ItemOptionModes.NO_USES):
        item_prefab = DB.items.get(item_nid, None)
        if not item_prefab:
            raise ValueError("%s is not an item" % item_nid)
        as_item = ItemObject.from_prefab(item_prefab)
        return cls(idx, as_item, display_value, width, height, ignore, font, text_color, align, mode)

    @classmethod
    def from_uid(cls, idx, item_uid: int, display_value: str | None = None, width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: Optional[NID] = None,
                 align: HAlignment = HAlignment.LEFT, mode: ItemOptionModes = ItemOptionModes.NO_USES):
        item_object = game.item_registry.get(item_uid)
        if not item_object:
            raise ValueError("%s is not a valid item uid" % item_uid)
        return cls(idx, item_object, display_value, width, height, ignore, font, text_color, align, mode)

    @classmethod
    def from_item(cls, idx, value: ItemObject, display_value: str | None = None, width: int = 0,
                  height: int = 0, ignore: bool = False, font: NID = 'text', text_color: Optional[NID] = None,
                  align: HAlignment = HAlignment.LEFT, mode: ItemOptionModes = ItemOptionModes.NO_USES):
        return cls(idx, value, display_value, width, height, ignore, font, text_color, align, mode)

    @classmethod
    def empty_option(cls, idx, display_value: str | None = "None", width: int = 0,
                     height: int = 0, ignore: bool = False, font: NID = 'text', text_color: Optional[NID] = None,
                     align: HAlignment = HAlignment.LEFT, mode: ItemOptionModes = ItemOptionModes.NO_USES):
        return cls(idx, None, display_value, width, height, ignore, font, text_color, align, mode)

    def width(self):
        return self._width or 104

    def set(self, val: Optional[ItemObject], disp_val: Optional[str] = None):
        self._value = val
        self._disp_value = text_funcs.translate(
            disp_val or (self._value.name if self._value else "None"))

    def get_color(self) -> Tuple[str, str]:
        if not self._value:
            return 'grey', 'grey'
        owner = game.get_unit(self._value.owner_nid)
        main_color = 'grey'
        uses_color = 'grey'
        if self.get_ignore():
            pass
        elif self._color:
            main_color = self._color
            if owner and not item_funcs.available(owner, self._value):
                pass
            else:
                uses_color = 'blue'
        elif self._value.droppable:
            main_color = 'green'
            uses_color = 'green'
        elif not owner or item_funcs.available(owner, self._value):
            main_color = 'white'
            uses_color = 'blue'
        return main_color, uses_color

    def get_help_box(self):
        if not self._help_box and self._value:
            if item_system.is_weapon(None, self._value) or item_system.is_spell(None, self._value):
                self._help_box = help_menu.ItemHelpDialog(self._value)
            else:
                self._help_box = help_menu.HelpDialog(self._value.desc)
        return self._help_box

    def draw(self, surf, x, y):
        main_color, uses_color = self.get_color()
        if not self._value:
            blit_loc = anchor_align(x, self.width(), self._align, (5, 5)), y
            render_text(surf, [self._font], [self._disp_value], [main_color], blit_loc, self._align)
        elif self._mode == ItemOptionModes.NO_USES:
            ItemOptionUtils.draw_without_uses(
                surf, x, y, self._value, self._font, main_color, self.width(), self._align, self._disp_value)
        elif self._mode == ItemOptionModes.USES:
            ItemOptionUtils.draw_with_uses(surf, x, y, self._value, self._font,
                                           main_color, uses_color, self.width(), self._align, self._disp_value)
        elif self._mode == ItemOptionModes.FULL_USES:
            ItemOptionUtils.draw_with_full_uses(
                surf, x, y, self._value, self._font, main_color, uses_color, self.width(), self._align, self._disp_value)


class BasicSkillOption(BaseOption[SkillObject]):
    def __init__(self, idx: int, skill: SkillObject, display_value: str | None = None,  width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: Optional[NID] = None,
                 align: HAlignment = HAlignment.LEFT):
        super().__init__(idx, skill, display_value, width, height, ignore)
        self._disp_value = text_funcs.translate(
            display_value or self._value.name)
        self._align = align
        self._color = text_color
        self._font = font

    @classmethod
    def from_nid(cls, idx, skill_nid: NID, display_value: str | None = None, width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: Optional[NID] = None,
                 align: HAlignment = HAlignment.LEFT):
        skill_prefab = DB.skills.get(skill_nid, None)
        if not skill_prefab:
            raise ValueError("%s is not a skill" % skill_nid)
        as_skill = SkillObject.from_prefab(skill_prefab)
        return cls(idx, as_skill, display_value, width, height, ignore, font, text_color, align)

    @classmethod
    def from_uid(cls, idx, skill_uid: int, display_value: str | None = None, width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: Optional[NID] = None,
                 align: HAlignment = HAlignment.LEFT):
        skill_object = game.skill_registry.get(skill_uid)
        if not skill_object:
            raise ValueError("%s is not a valid skill uid" % skill_uid)
        return cls(idx, skill_object, display_value, width, height, ignore, font, text_color, align)

    @classmethod
    def from_skill(cls, idx, value: SkillObject, display_value: str | None = None, width: int = 0,
                   height: int = 0, ignore: bool = False, font: NID = 'text', text_color: Optional[NID] = None,
                   align: HAlignment = HAlignment.LEFT):
        return cls(idx, value, display_value, width, height, ignore, font, text_color, align)

    def width(self):
        return self._width or 104

    def set(self, val: SkillObject, disp_val: Optional[str] = None):
        self._value = val
        self._disp_value = text_funcs.translate(disp_val or self._value.name)

    def get_color(self):
        if self.get_ignore():
            return 'grey'
        return 'white'

    def get_help_box(self):
        if not self._help_box:
            self._help_box = help_menu.HelpDialog(
                self._value.desc, name=self._value.name)
        return self._help_box

    def draw(self, surf, x, y):
        icon = icons.get_icon(self._value)
        display_text = self._disp_value or self._value.name
        font = self._font
        if text_width(font, display_text) > self.width() - 20:
            font = 'narrow'
        blit_loc = anchor_align(x, self.width(), self._align, (20, 5)), y
        if icon:
            if self._align == HAlignment.RIGHT:
                surf.blit(icon, (blit_loc[0] - 20 - text_width(font, display_text) // 2, y))
            elif self._align == HAlignment.CENTER:
                blit_loc = (blit_loc[0] + 10, blit_loc[1])
                surf.blit(icon, (blit_loc[0] - 20 - text_width(font, display_text) // 2 + 2, y))
            else:
                surf.blit(icon, (x + 2, y))
        color = self.get_color()
        render_text(surf, [font], [display_text], [color], blit_loc, self._align)


class BasicIconOption(BaseOption[str]):
    def __init__(self, idx: int, value: str, icon_name: str, width: int = 0,
                 height: int = 0, ignore: bool = False, font: NID = 'text', text_color: Optional[NID] = None,
                 align: HAlignment = HAlignment.LEFT):
        super().__init__(idx, value, value, width, height, ignore)
        self._disp_value = text_funcs.translate(value)
        self._icon_name = icon_name
        self._align = align
        self._color = text_color
        self._font = font

    def width(self):
        return self._width or 104

    def set(self, val: str, icon_name: Optional[str] = None):
        self._value = val
        self._disp_value = text_funcs.translate(val)
        self._icon_name = icon_name

    def get_color(self):
        if self.get_ignore():
            return 'grey'
        return 'white'

    def draw(self, surf, x, y):
        icon = icons.get_icon_by_name(self._icon_name)
        display_text = self._value
        font = self._font
        if text_width(font, display_text) > self.width() - 20:
            font = 'narrow'
        blit_loc = anchor_align(x, self.width(), self._align, (20, 5)), y
        if icon:
            if self._align == HAlignment.RIGHT:
                surf.blit(icon, (blit_loc[0] - 20 - text_width(font, display_text) // 2, y))
            elif self._align == HAlignment.CENTER:
                blit_loc = (blit_loc[0] + 10, blit_loc[1])
                surf.blit(icon, (blit_loc[0] - 20 - text_width(font, display_text) // 2 + 2, y))
            else:
                surf.blit(icon, (x + 2, y))
        color = self.get_color()
        render_text(surf, [font], [display_text], [color], blit_loc, self._align)
