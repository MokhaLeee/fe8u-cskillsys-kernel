from app.constants import WINWIDTH, WINHEIGHT
from app.data.database.database import DB

from app.engine.sprites import SPRITES
from app.engine.fonts import FONT
from app.engine.game_menus import menu_options
from app.engine import menus, gui, base_surf, image_mods, text_funcs

from app.engine.game_state import game

class RecordOption(menu_options.BasicOption):
    def __init__(self, idx, text):
        self.idx = idx
        self.level, self.turncount, self.mvp = text
        self.help_box = None
        self.font = 'text'
        self.color = None
        self.ignore = False

    def get(self):
        return self.level, self.turncount, self.mvp

    def width(self):
        return 224

    def height(self):
        return 16

    def draw(self, surf, x, y):
        level_prefab = DB.levels.get(self.level)
        if level_prefab:
            level_name = level_prefab.name
        else:
            level_name = "???"
        main_font = FONT[self.font]
        width = main_font.width(level_name)
        if width > 96:
            FONT['narrow'].blit(level_name, surf, (x + 4, y))
        else:
            main_font.blit(level_name, surf, (x + 4, y))
        main_font.blit_right(str(self.turncount), surf, (x + 120, y), color='blue')
        unit_prefab = DB.units.get(self.mvp)
        if unit_prefab:
            unit_name = unit_prefab.name
        else:
            unit_name = "???"
        width = main_font.width(unit_name)
        if width > 72:
            FONT['narrow'].blit_right(unit_name, surf, (x + 196, y))
        else:
            main_font.blit_right(unit_name, surf, (x + 196, y))

class UnitRecordOption(RecordOption):
    def __init__(self, idx, text):
        self.idx = idx
        self.level, self.kills, self.damage, self.healing = text
        self.help_box = None
        self.font = 'text'
        self.color = None
        self.ignore = False

    def get(self):
        return self.level, self.kills, self.damage, self.healing

    def draw(self, surf, x, y):
        level_prefab = DB.levels.get(self.level)
        if level_prefab:
            level_name = level_prefab.name
        else:
            level_name = "???"
        FONT['text'].blit(level_name, surf, (x + 4, y))
        FONT['text-blue'].blit_right(str(self.kills), surf, (x + 114, y))
        FONT['text-blue'].blit_right(str(self.damage), surf, (x + 164, y))
        FONT['text-blue'].blit_right(str(self.healing), surf, (x + 210, y))

class LevelRecordOption(RecordOption):
    def __init__(self, idx, text):
        self.idx = idx
        self.unit_nid, self.kills, self.damage, self.healing = text
        self.help_box = None
        self.font = 'text'
        self.color = None
        self.ignore = False

    def get(self):
        return self.unit_nid, self.kills, self.damage, self.healing

    def draw(self, surf, x, y):
        unit_prefab = DB.units.get(self.unit_nid)
        if unit_prefab:
            unit_name = unit_prefab.name
        else:
            unit_name = "???"
        FONT['text-yellow'].blit(str(self.idx + 1), surf, (x + 4, y))
        FONT['text'].blit(unit_name, surf, (x + 34, y))
        FONT['text-blue'].blit_right(str(self.kills), surf, (x + 114, y))
        FONT['text-blue'].blit_right(str(self.damage), surf, (x + 164, y))
        FONT['text-blue'].blit_right(str(self.healing), surf, (x + 210, y))

class RecordsDisplay(menus.Choice):
    """
    For each level, display turncount and mvp
    Level 1 - Turncount - MVP
    Level 2 - Turncount - MVP
    """

    option_type = RecordOption

    def __init__(self):
        options = self.get_options()
        super().__init__(None, options, ('center', 28))
        self.shimmer = 2
        self.gem = False
        self.set_limit(6)
        self.set_hard_limit(True)
        self.y_offset = 16  # Make room for header

        self.top_banner = self.create_top_banner()

        self.left_arrow = gui.ScrollArrow('left', (2, 7))
        self.right_arrow = gui.ScrollArrow('right', (self.get_menu_width() - 8, 7), 0.5)

    def get_options(self):
        levels = game.records.get_levels()[:-1]
        turncounts = game.records.get_turncounts(levels)
        mvps = [game.records.get_mvp(level_nid) for level_nid in levels]

        return [(l, t, m) for (l, t, m) in zip(levels, turncounts, mvps)]

    def create_options(self, options, info_descs=None):
        self.options.clear()
        for idx, option in enumerate(options):
            option = self.option_type(idx, option)
            self.options.append(option)

        if self.hard_limit:
            for num in range(self.limit - len(options)):
                option = menu_options.EmptyOption(len(options) + num)
                option._width = 224
                self.options.append(option)

    def create_top_banner(self):
        levels = game.records.get_levels()[:-1]
        total_turns = sum(game.records.get_turncounts(levels))
        total_turns = str(total_turns)
        overall_mvp = game.records.get_mvp()

        bg = base_surf.create_base_surf(self.get_menu_width(), 24, 'menu_bg_white')
        bg = image_mods.make_translucent(bg, 0.25)
        FONT['text-yellow'].blit(text_funcs.translate('Total Turns'), bg, (4, 4))

        FONT['text-blue'].blit_right(total_turns, bg, (92, 4))
        FONT['text-yellow'].blit(text_funcs.translate('Overall MVP'), bg, (100, 4))
        unit = DB.units.get(overall_mvp)
        if unit:
            FONT['text'].blit_right(unit.name, bg, (216, 4))
        else:
            FONT['text'].blit_right('--', bg, (216, 4))
        return bg

    def draw(self, surf, offset=None):
        if not offset:
            offset = (0, 0)
        surf.blit(self.top_banner, (offset[0] + WINWIDTH//2 - self.top_banner.get_width()//2, offset[1] + 4))
        super().vert_draw(surf, offset)
        FONT['text-yellow'].blit(text_funcs.translate('record_header'), surf, (offset[0] + 12, offset[1] + 32))
        return surf

class UnitStats(RecordsDisplay):
    """
    For a unit, get it's stats on each level
    Level 1 - Kills - Damage - Healing
    Level 2 - Kills - Damage - Healing
    """
    option_type = UnitRecordOption

    def __init__(self, unit_nid):
        self.unit_nid = unit_nid
        super().__init__()

    def get_options(self):
        # Ignore the last level
        levels = game.records.get_levels()[:-1]
        kills = [game.records.get_kills(self.unit_nid, level) for level in levels]
        damage = [game.records.get_damage(self.unit_nid, level) for level in levels]
        healing = [game.records.get_heal(self.unit_nid, level) for level in levels]

        return [(l, k, d, h) for (l, k, d, h) in zip(levels, kills, damage, healing)]

    def create_top_banner(self):
        bg = SPRITES.get('purple_background').convert_alpha().copy()
        unit_prefab = DB.units.get(self.unit_nid)
        if unit_prefab:
            unit_name = unit_prefab.name
        else:
            unit_name = "???"
        FONT['chapter-grey'].blit_center(unit_name, bg, (bg.get_width()//2, 5))
        return bg

    def draw(self, surf, offset=None):
        if not offset:
            offset = (0, 0)
        surf.blit(self.top_banner, (offset[0] + WINWIDTH//2 - self.top_banner.get_width()//2, offset[1] + 4))
        super().vert_draw(surf, offset)
        FONT['text-yellow'].blit(text_funcs.translate('mvp_record_header'), surf, (offset[0] + 12, offset[1] + 32))
        return surf

class MVPDisplay(RecordsDisplay):
    """
    For each unit, display stats
    Rank - Unit5 - Kills - Damage - Healing
    Rank - Unit2 - Kills - Damage - Healing
    """
    option_type = LevelRecordOption

    def get_options(self):
        units = [unit.nid for unit in game.get_all_player_units()]
        units = list(sorted(units, key=lambda x: game.records.determine_score(x), reverse=True))
        kills = [game.records.get_kills(unit_nid) for unit_nid in units]
        damage = [game.records.get_damage(unit_nid) for unit_nid in units]
        healing = [game.records.get_heal(unit_nid) for unit_nid in units]

        return [(u, k, d, h) for (u, k, d, h) in zip(units, kills, damage, healing)]

    def draw(self, surf, offset=None):
        if not offset:
            offset = (0, 0)
        surf.blit(self.top_banner, (offset[0] + WINWIDTH//2 - self.top_banner.get_width()//2, offset[1] + 4))
        super().vert_draw(surf, offset)
        FONT['text-yellow'].blit(text_funcs.translate('unit_record_header'), surf, (offset[0] + 12, offset[1] + 32))
        return surf


class ChapterStats(RecordsDisplay):
    """
    For a given level, display each unit in mvp order
    Rank - Unit5 - Kills - Damage - Healing
    Rank - Unit2 - Kills - Damage - Healing
    """
    option_type = LevelRecordOption

    def __init__(self, level_nid):
        self.level_nid = level_nid
        super().__init__()

    def get_options(self):
        # For all player units
        units = [unit.nid for unit in game.get_all_player_units()]
        units = list(sorted(units, key=lambda x: game.records.determine_score(x, self.level_nid), reverse=True))
        kills = [game.records.get_kills(unit_nid, self.level_nid) for unit_nid in units]
        damage = [game.records.get_damage(unit_nid, self.level_nid) for unit_nid in units]
        healing = [game.records.get_heal(unit_nid, self.level_nid) for unit_nid in units]

        # Only those units that actually participated
        participated = [(u, k, d, h) for (u, k, d, h) in zip(units, kills, damage, healing) if (k > 0 or d > 0 or h > 0)]
        if not participated:
            # In case we can't find any of the units or nobody participated
            participated = [(game.get_all_units_in_party()[0].nid, 0, 0, 0)]
        return participated

    def create_top_banner(self):
        bg = SPRITES.get('purple_background').convert_alpha().copy()
        level_prefab = DB.levels.get(self.level_nid)
        if level_prefab:
            level_name = level_prefab.name
        else:
            level_name = "???"
        FONT['chapter-grey'].blit_center(level_name, bg, (bg.get_width()//2, 4))
        return bg

    def draw(self, surf, offset=None):
        if not offset:
            offset = (0, 0)
        surf.blit(self.top_banner, (offset[0] + WINWIDTH//2 - self.top_banner.get_width()//2, offset[1] + 4))
        super().vert_draw(surf, offset)
        FONT['text-yellow'].blit(text_funcs.translate('unit_record_header'), surf, (offset[0] + 12, offset[1] + 32))
        return surf
