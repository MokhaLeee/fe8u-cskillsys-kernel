import math

from app.data.database.database import DB

from app.engine.sprites import SPRITES
from app.engine.fonts import FONT
from app.engine import engine, image_mods, icons, help_menu, text_funcs, item_system, item_funcs
from app.engine.game_state import game

from app.engine.graphics.text.text_renderer import render_text, text_width, rendered_text_width
from app.utilities.enums import HAlignment

class EmptyOption():
    def __init__(self, idx):
        self.idx = idx
        self.help_box = None
        self.font = 'text'
        self.color = None
        self.ignore = True
        self._width = 104

    def get(self):
        return None

    def set_text(self):
        pass

    def width(self):
        return self._width

    def height(self):
        return 16

    def draw(self, surf, x, y):
        pass

    def draw_highlight(self, surf, x, y, menu_width):
        highlight_surf = SPRITES.get('menu_highlight')
        width = highlight_surf.get_width()
        for slot in range((menu_width - 10)//width):
            left = x + 5 + slot*width
            top = y + 9
            surf.blit(highlight_surf, (left, top))
        return surf

class BasicOption():
    def __init__(self, idx, text):
        self.idx = idx
        self.text = text
        self.display_text = text_funcs.translate(text)
        self.help_box = None
        self.font = 'text'
        self.color = None
        self.ignore = False

    def get(self):
        return self.text

    def set_text(self, text):
        self.text = text
        self.display_text = text_funcs.translate(text)

    def width(self):
        return text_width(self.font, self.display_text) + 24

    def height(self):
        return 16

    def get_color(self):
        if self.ignore:
            return 'grey'
        return self.color

    def draw(self, surf, x, y):
        # font = FONT[self.font]
        render_text(surf, [self.font], [self.display_text], [self.get_color()], (x + 5, y))
        # font.blit(self.display_text, surf, (x + 5, y), self.get_color())

    def draw_highlight(self, surf, x, y, menu_width):
        highlight_surf = SPRITES.get('menu_highlight')
        width = highlight_surf.get_width()
        for slot in range((menu_width - 10)//width):
            left = x + 5 + slot*width
            top = y + 9
            surf.blit(highlight_surf, (left, top))
        self.draw(surf, x, y)
        return surf

class AchievementOption(BasicOption):
    def __init__(self, idx, achievement):
        self.idx = idx
        self.achievement = achievement
        self.help_box = None
        self.font = 'text'
        self.color = None
        self.ignore = False
        self.mode = None

    def height(self):
        return 32

    def width(self):
        return 220

    def get(self):
        return self.achievement

    def set_text(self, text):
        pass

    def draw(self, surf, x, y):
        x_offset = 5
        title_font = self.font
        desc_font = self.font

        # Render Title
        if self.achievement.get_hidden():
            front_half = "Hidden - "
        else:
            front_half = self.achievement.name + " - "
        if rendered_text_width([title_font], [front_half + "Complete"]) > 220:
            title_font = 'narrow'
        front_color = 'yellow' if self.achievement.get_complete() else 'grey'
        render_text(surf, [title_font], [front_half], [front_color], (x + x_offset, y))

        # Render Description
        offset = rendered_text_width([title_font], [front_half])
        if self.achievement.get_complete():
            render_text(surf, [title_font], ["Complete"], ['green'], (x + x_offset + offset, y))
        else:
            render_text(surf, [title_font], ["Locked"], ['red'], (x + x_offset + offset, y))
        if self.achievement.get_hidden():
            desc = "???"
        else:
            desc = self.achievement.desc
        if rendered_text_width([desc_font], [desc]) > 220:
            desc_font = 'narrow'
        render_text(surf, [desc_font], [desc], [self.get_color()], (x + x_offset, y + 13))

    def get_color(self):
        if self.achievement.get_complete():
            return 'white'
        return 'grey'

class NullOption(BasicOption):
    def __init__(self, idx):
        super().__init__(idx, "Nothing")
        self.ignore = True
        self._width = 0

    def width(self):
        if self._width:
            return self._width
        else:
            return super().width()

    def get(self):
        return None

class HorizOption(BasicOption):
    def width(self):
        return text_width(self.font, self.display_text)

class SingleCharacterOption(BasicOption):
    def width(self):
        return 8

class TitleOption():
    def __init__(self, idx, text, option_bg_name):
        self.idx = idx
        self.text = text
        self.display_text = text_funcs.translate(text)
        self.option_bg_name = option_bg_name

        self.font = 'chapter'
        self.color = 'grey'
        self.ignore = False

    def get(self):
        return self.text

    def set_text(self, text):
        self.text = text
        self.display_text = text_funcs.translate(text)

    def width(self):
        return SPRITES.get(self.option_bg_name).get_width()

    def height(self):
        return SPRITES.get(self.option_bg_name).get_height()

    def draw_text(self, surf, x, y):
        font = FONT[self.font]

        text = self.display_text
        text_size = font.size(text)
        position = (x - text_size[0]//2, y - text_size[1]//2)

        # Handle outline
        t = math.sin(math.radians((engine.get_time()//10) % 180))
        color_transition = image_mods.blend_colors((192, 248, 248), (56, 48, 40), t)
        outline_surf = engine.create_surface((text_size[0] + 4, text_size[1] + 2), transparent=True)
        render_text(outline_surf, [self.font], [text], [self.color], (1, 0))
        render_text(outline_surf, [self.font], [text], [self.color], (0, 1))
        render_text(outline_surf, [self.font], [text], [self.color], (1, 2))
        render_text(outline_surf, [self.font], [text], [self.color], (2, 1))
        outline_surf = image_mods.change_color(outline_surf, color_transition)

        surf.blit(outline_surf, (position[0] - 1, position[1] - 1))
        render_text(surf, [self.font], [text], [self.color], position)

    def draw(self, surf, x, y):
        left = x - self.width()//2
        surf.blit(SPRITES.get(self.option_bg_name), (left, y))

        self.draw_text(surf, left + self.width()//2, y + self.height()//2 + 1)

    def draw_highlight(self, surf, x, y):
        left = x - self.width()//2
        surf.blit(SPRITES.get(self.option_bg_name + '_highlight'), (left, y))

        self.draw_text(surf, left + self.width()//2, y + self.height()//2 + 1)

class ChapterSelectOption(TitleOption):
    def __init__(self, idx, text, option_bg_name, bg_color):
        self.idx = idx
        self.text = text
        self.display_text = text_funcs.translate(text)
        self.option_bg_prefix = option_bg_name
        self.set_bg_color(bg_color)

        self.font = 'chapter'
        self.color = 'grey'
        self.ignore = False

    def set_bg_color(self, color):
        self.bg_color = color
        self.option_bg_name = self.option_bg_prefix + '_' + self.bg_color

    def draw_flicker(self, surf, x, y):
        left = x - self.width()//2
        surf.blit(SPRITES.get(self.option_bg_name + '_flicker'), (left, y))

        self.draw_text(surf, left + self.width()//2, y + self.height()//2 + 1)

class ModeOption(TitleOption):
    def __init__(self, idx, text):
        self.idx = idx
        self.text = text
        self.display_text = text_funcs.translate(text)

        self.font = 'chapter'
        self.color = 'grey'
        self.ignore = False
        self.option_bg_name = 'mode_bg'

    def draw_text(self, surf, x, y):
        if self.ignore:
            color = 'black'
        else:
            color = 'grey'

        font = FONT[self.font]

        text = self.display_text
        text_size = font.size(text)
        position = (x - text_size[0]//2, y - text_size[1]//2)

        # Handle outline
        render_text(surf, [self.font], [text], [color], position)

class ItemOption(BasicOption):
    def __init__(self, idx, item):
        self.idx = idx
        self.item = item
        self.help_box = None
        self.font = 'text'
        self.color = item_system.text_color(None, item)
        self.ignore = False

    def get(self):
        return self.item

    def set_text(self, text):
        pass

    def set_item(self, item):
        self.item = item

    def width(self):
        return 104

    def height(self):
        return 16

    def get_color(self):
        owner = game.get_unit(self.item.owner_nid)
        main_color = 'grey'
        uses_color = 'grey'
        if self.ignore:
            pass
        elif self.color:
            main_color = self.color
            if owner and not item_funcs.available(owner, self.item):
                pass
            else:
                uses_color = 'blue'
        elif self.item.droppable:
            main_color = 'green'
            uses_color = 'green'
        elif not owner or item_funcs.available(owner, self.item):
            main_color = None
            uses_color = 'blue'
        return main_color, uses_color

    def get_help_box(self):
        owner = game.get_unit(self.item.owner_nid)
        if item_system.is_weapon(owner, self.item) or item_system.is_spell(owner, self.item):
            return help_menu.ItemHelpDialog(self.item)
        else:
            return help_menu.HelpDialog(self.item.desc)

    def draw(self, surf, x, y):
        icon = icons.get_icon(self.item)
        if icon:
            surf.blit(icon, (x + 2, y))
        main_color, uses_color = self.get_color()
        main_font = self.font
        if text_width(main_font, self.item.name) > 60:
            main_font = 'narrow'
        uses_font = 'text'
        render_text(surf, [main_font], [self.item.name], [main_color], (x + 20, y))
        uses_string = '--'
        if self.item.uses:
            uses_string = str(self.item.data['uses'])
        elif self.item.parent_item and self.item.parent_item.uses and self.item.parent_item.data['uses']:
            uses_string = str(self.item.parent_item.data['uses'])
        elif self.item.c_uses:
            uses_string = str(self.item.data['c_uses'])
        elif self.item.parent_item and self.item.parent_item.c_uses and self.item.parent_item.data['c_uses']:
            uses_string = str(self.item.parent_item.data['c_uses'])
        elif self.item.cooldown:
            uses_string = str(self.item.data['cooldown'])
        left = x + 99
        render_text(surf, [uses_font], [uses_string], [uses_color], (left, y), HAlignment.RIGHT)

class ConvoyItemOption(ItemOption):
    def __init__(self, idx, item, owner):
        super().__init__(idx, item)
        self.owner = owner

    def width(self):
        return 112

    def get_color(self):
        main_color = 'grey'
        uses_color = 'grey'
        if self.ignore:
            pass
        elif self.color:
            main_color = self.color
            uses_color = 'blue'
        elif item_funcs.available(self.owner, self.item):
            main_color = None
            uses_color = 'blue'
        return main_color, uses_color

class FullItemOption(ItemOption):
    def width(self):
        return 108

    def draw(self, surf, x, y):
        icon = icons.get_icon(self.item)
        if icon:
            surf.blit(icon, (x + 2, y))
        main_color, uses_color = self.get_color()
        main_font = self.font
        width = text_width(main_font, self.item.name)
        if width > 60:
            main_font = 'narrow'
        uses_font = 'text'
        render_text(surf, [main_font], [self.item.name], [main_color], (x + 20, y))

        uses_string_a = '--'
        uses_string_b = '--'
        if self.item.data.get('uses') is not None:
            uses_string_a = str(self.item.data['uses'])
            uses_string_b = str(self.item.data['starting_uses'])
        elif self.item.data.get('c_uses') is not None:
            uses_string_a = str(self.item.data['c_uses'])
            uses_string_b = str(self.item.data['starting_c_uses'])
        elif self.item.parent_item and self.item.parent_item.data.get('uses') is not None:
            uses_string_a = str(self.item.parent_item.data['uses'])
            uses_string_b = str(self.item.parent_item.data['starting_uses'])
        elif self.item.parent_item and self.item.parent_item.data.get('c_uses') is not None:
            uses_string_a = str(self.item.parent_item.data['c_uses'])
            uses_string_b = str(self.item.parent_item.data['starting_c_uses'])
        elif self.item.data.get('cooldown') is not None:
            uses_string_a = str(self.item.data['cooldown'])
            uses_string_b = str(self.item.data['starting_cooldown'])
        if not (uses_string_a == '--' and uses_string_b == '--'):
            render_text(surf, [uses_font], [uses_string_a], [uses_color], (x + 96, y), HAlignment.RIGHT)
            render_text(surf, [uses_font], ["/"], [], (x + 98, y))
            render_text(surf, [uses_font], [uses_string_b], [uses_color], (x + 120, y), HAlignment.RIGHT)

class ValueItemOption(ItemOption):
    def __init__(self, idx, item, disp_value):
        super().__init__(idx, item)
        self.disp_value = disp_value

    def width(self):
        return 152

    def draw(self, surf, x, y):
        icon = icons.get_icon(self.item)
        if icon:
            surf.blit(icon, (x + 2, y))
        main_color, uses_color = self.get_color()
        main_font = self.font
        width = text_width(main_font, self.item.name)
        if width > 60:
            main_font = 'narrow'
        uses_font = 'text'
        render_text(surf, [main_font], [self.item.name], [main_color], (x + 20, y))

        uses_string = '--'
        if self.item.data.get('uses') is not None:
            uses_string = str(self.item.data['uses'])
        elif self.item.parent_item and self.item.parent_item.data.get('uses') is not None:
            uses_string = str(self.item.parent_item.data['uses'])
        elif self.item.c_uses is not None:
            uses_string = str(self.item.data['c_uses'])
        elif self.item.parent_item and self.item.parent_item.data.get('c_uses') is not None:
            uses_string = str(self.item.parent_item.data['c_uses'])
        elif self.item.cooldown is not None:
            uses_string = str(self.item.data['cooldown'])
        render_text(surf, [uses_font], [uses_string], [uses_color], (x + 100, y), HAlignment.RIGHT)

        value_color = 'grey'
        value_string = '--'
        owner = game.get_unit(self.item.owner_nid)
        if self.disp_value == 'buy':
            value = item_funcs.buy_price(owner, self.item)
            if value:
                value_string = str(value)
                if value <= game.get_money():
                    value_color = 'blue'
            else:
                value_string = '--'
        elif self.disp_value == 'sell':
            value = item_funcs.sell_price(owner, self.item)
            if value:
                value_string = str(value)
                value_color = 'blue'
            else:
                value_string = '--'
        render_text(surf, [uses_font], [value_string], [value_color], (x + self.width() - 6, y), HAlignment.RIGHT)

class RepairValueItemOption(ValueItemOption):
    def draw(self, surf, x, y):
        icon = icons.get_icon(self.item)
        if icon:
            surf.blit(icon, (x + 2, y))
        main_color, uses_color = self.get_color()
        main_font = self.font
        width = text_width(main_font, self.item.name)
        if width > 60:
            main_font = 'narrow'
        uses_font = 'text'
        render_text(surf, [main_font], [self.item.name], [main_color], (x + 20, y))

        uses_string = '--'
        if self.item.data.get('uses') is not None:
            uses_string = str(self.item.data['uses'])
        render_text(surf, [uses_font], [uses_string], [uses_color], (x + 100, y), HAlignment.RIGHT)

        value_color = 'grey'
        value_string = '--'
        owner = game.get_unit(self.item.owner_nid)
        value = item_funcs.repair_price(owner, self.item)
        if value:
            value_string = str(value)
            if value < game.get_money():
                value_color = 'blue'
        render_text(surf, [uses_font], [value_string], [value_color], (x + self.width() - 10, y), HAlignment.RIGHT)

class StockValueItemOption(ValueItemOption):
    def __init__(self, idx, item, disp_value, stock):
        super().__init__(idx, item, disp_value)
        self.stock = stock

    def width(self):
        return 168

    def draw(self, surf, x, y):
        super().draw(surf, x, y)

        main_color, uses_color = self.get_color()
        main_font = self.font

        stock_string = '--'
        if self.stock >= 0:
            stock_string = str(self.stock)
        render_text(surf, [main_font], [stock_string], [main_color], (x + 128, y), HAlignment.RIGHT)

class UnitOption(BasicOption):
    def __init__(self, idx, unit):
        self.idx = idx
        self.unit = unit
        self.help_box = None
        self.font = 'text'
        self.color = None
        self.ignore = False
        self.mode = None

    def get(self):
        return self.unit

    def set_text(self, text):
        pass

    def set_unit(self, unit):
        self.unit = unit

    def set_mode(self, mode):
        self.mode = mode

    def width(self):
        return 64

    def height(self):
        return 16

    def get_color(self):
        color = None
        if self.ignore:
            color = 'grey'
        elif self.color:
            color = self.color
        elif self.mode in ('position', 'prep_manage'):
            if 'Blacklist' in self.unit.tags:
                color = 'red'
            elif DB.constants.value('fatigue') and game.game_vars.get('_fatigue') and \
                    self.unit.get_fatigue() >= self.unit.get_max_fatigue():
                color = 'red'
            elif not self.unit.position:
                color = 'grey'
            elif self.unit.position and (not game.check_for_region(self.unit.position, 'formation') or 'Required' in self.unit.tags):
                color = 'green'
            else:
                color = None
        return color

    def get_help_box(self):
        return None

    def draw_map_sprite(self, surf, x, y, highlight=False):
        map_sprite = self.unit.sprite.create_image('passive')
        if self.mode == 'position' and not self.unit.position:
            map_sprite = self.unit.sprite.create_image('gray')
        elif highlight:
            map_sprite = self.unit.sprite.create_image('active')
        surf.blit(map_sprite, (x - 20, y - 24 - 1))

    def draw_text(self, surf, x, y):
        color = self.get_color()
        font = self.font
        if text_width(font, self.unit.name) > 44:
            font = 'narrow'
        render_text(surf, [font], [self.unit.name], [color], (x + 20, y))

    def draw(self, surf, x, y):
        self.draw_map_sprite(surf, x, y)
        self.draw_text(surf, x, y)

    def draw_highlight(self, surf, x, y, menu_width=None):
        # Draw actual highlight surf
        highlight_surf = SPRITES.get('menu_highlight')
        width = highlight_surf.get_width()
        for slot in range((self.width() - 10)//width):
            left = x + 5 + slot*width
            top = y + 9
            surf.blit(highlight_surf, (left, top))

        self.draw_map_sprite(surf, x, y, highlight=True)
        self.draw_text(surf, x, y)

class LoreOption(BasicOption):
    def __init__(self, idx, lore):
        self.idx = idx
        self.lore = lore
        self.text = lore.name
        self.display_text = text_funcs.translate(self.text)
        self.help_box = None
        self.font = 'text'
        self.color = None
        self.ignore = False

    def get(self):
        return self.lore

    def set_text(self, text):
        pass

    def set_lore(self, lore):
        self.lore = lore
        self.text = lore.name
        self.display_text = text_funcs.translate(self.text)

    def width(self):
        return 84

    def height(self):
        return 16

    def get_color(self):
        if self.ignore:
            return 'yellow'
        return self.color

    def draw(self, surf, x, y):
        main_color = self.get_color()
        main_font = self.font
        if self.ignore:
            s = self.lore.category
        else:
            s = self.display_text
        width = text_width(main_font, s)
        if width > 78:
            main_font = 'narrow'
        render_text(surf, [main_font], s, [main_color], (x + 6, y))
