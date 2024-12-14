from app.engine.graphics.text.text_renderer import fix_tags
import logging
from app.engine.text_evaluator import TextEvaluator
import datetime
from app.constants import WINWIDTH, WINHEIGHT

from app.engine.sprites import SPRITES
from app.engine.fonts import FONT
from app.engine.sound import get_sound_thread
from app.engine.state import State
from app.engine import engine, background, base_surf, icons, menus
from app.engine.objects.unit import UnitObject
from app.engine.game_state import game
from app.engine.fluid_scroll import FluidScroll

class ObjectiveMenuState(State):
    name = 'objective_menu'
    bg = None
    surfaces = []

    def start(self):
        self.bg = background.create_background('settings_background')
        self.surfaces = self.get_surfaces()
        self.fluid = FluidScroll()
        self.menu.set_mode('objective_menu')

        game.state.change('transition_in')
        return 'repeat'

    def get_surfaces(self) -> list:
        surfaces = []

        name_back_surf = SPRITES.get('chapter_select_green').copy()
        # Text
        big_font = FONT['chapter-green']
        center = (name_back_surf.get_width()//2, name_back_surf.get_height()//2 - big_font.height//2)
        big_font.blit_center(game.level.name, name_back_surf, center)
        surfaces.append((name_back_surf, (24, 2)))

        # Background
        def bg_surf(x, y, pos, menu_bg=None):
            back_surf = base_surf.create_base_surf(WINWIDTH - x, y, menu_bg)
            surfaces.append((back_surf, pos))

        bg_surf(8, 24, (4, 34), 'menu_bg_white')

        # Get words
        golden_words_surf = SPRITES.get('golden_words')

        # Get Turn
        turn_surf = engine.subsurface(golden_words_surf, (0, 17, 26, 10))
        surfaces.append((turn_surf, (10, 42)))
        # Get Funds
        funds_surf = engine.subsurface(golden_words_surf, (0, 33, 32, 10))
        surfaces.append((funds_surf, (WINWIDTH//3 - 8, 42)))
        # Get PlayTime
        playtime_surf = engine.subsurface(golden_words_surf, (32, 15, 17, 13))
        surfaces.append((playtime_surf, (2*WINWIDTH//3 + 6, 39)))
        # Get G
        g_surf = engine.subsurface(golden_words_surf, (40, 47, 9, 12))
        surfaces.append((g_surf, (2*WINWIDTH//3 - 9, 40)))

        def top_surf(a, pos, game_get=None):
            count_size = FONT['text-blue'].width(str(game_get)) + 1, FONT['text-blue'].height
            count_surf = engine.create_surface(count_size, transparent=True)
            FONT['text-blue'].blit(str(game_get), count_surf, (0, 0))
            surfaces.append((count_surf, (a*WINWIDTH//3 - pos[0] - count_surf.get_width(), pos[1])))
        # TurnCountSurf
        top_surf(1, (16, 38), game.turncount)

        # MoneySurf
        top_surf(2, (12, 38), game.get_money())

        # Get win and loss conditions
        win_con = game.level.objective['win']
        text_parser = TextEvaluator(logging.getLogger(), game)
        win_lines = text_parser._evaluate_all(','+win_con).split(',')
        win_lines = [w.replace('{comma}', ',') for w in win_lines]
        win_lines = fix_tags(win_lines)

        loss_con = game.level.objective['loss']
        text_parser = TextEvaluator(logging.getLogger(), game)
        loss_lines = text_parser._evaluate_all(','+loss_con).split(',')
        loss_lines = [line.replace('{comma}', ',') for line in loss_lines]
        loss_lines = fix_tags(loss_lines)

        self.topleft = (4, 60)
        self.menu = menus.Table(None, win_lines+loss_lines, (6, 1), self.topleft)

        seed = str(game.game_vars['_random_seed'])
        seed_surf = engine.create_surface((28, 16), transparent=True)
        FONT['text_numbers'].blit_center(seed, seed_surf, (14, 0))
        surfaces.append((seed_surf, (WINWIDTH - 28, 4)))

        # Functions for Unit Count per Party
        def bg_units_surf(pos, menu_bg=None, shimmer=None):
            bgsurf = base_surf.create_base_surf(WINWIDTH - 192, 24, menu_bg)
            shimmer = SPRITES.get(shimmer)
            bgsurf.blit(shimmer, (bgsurf.get_width() - 1 - shimmer.get_width(), bgsurf.get_height() - shimmer.get_height() - 5))
            surfaces.append((bgsurf, (pos)))

        def party_golden_words_surf(x, y, w, h, pos, winw=False):
            party_surf = engine.subsurface(golden_words_surf, (x, y, w, h))
            if winw:
                surfaces.append((party_surf, (WINWIDTH - pos[0], pos[1])))
            else:
                surfaces.append((party_surf, (pos[0], pos[1])))

        def unit_count_surf(pos, get_units=None):
            count_size = FONT['text-blue'].width(str(len(get_units))) + 6, FONT['text-blue'].height
            countsurf = engine.create_surface(count_size, transparent=True)
            if len(get_units) > 0:
                FONT['text-blue'].blit(str(len(get_units)), countsurf, (5, 0))
            else:
                FONT['text-blue'].blit("--", countsurf, (0, 0))
            surfaces.append((countsurf, (WINWIDTH - pos[0] - countsurf.get_width(), pos[1])))

        # PlayerUnits
        bg_units_surf((132, 60), "menu_bg_base", "menu_shimmer1")
        party_golden_words_surf(56, 12, 40, 8, (104, 57), True)
        unit_count_surf((75, 62), game.get_player_units())

        # OtherUnits
        bg_units_surf((132, 80), "menu_bg_green", "menu_shimmer_green")
        party_golden_words_surf(56, 36, 40, 8, (104, 77), True)
        unit_count_surf((75, 82), game.get_other_units())

        # EnemyUnits
        bg_units_surf((188, 60), "menu_bg_red", "menu_shimmer_red")
        party_golden_words_surf(56, 20, 40, 8, (48, 57), True)
        unit_count_surf((16, 62), game.get_enemy1_units())

        # Enemy2Units
        bg_units_surf((188, 80), "menu_bg_purple", "menu_shimmer_purple")
        unit_count_surf((16, 82), game.get_enemy2_units())

        # Party Leader info bg
        bg_surf(136, 62, (132, 100), 'menu_bg_white')

        unit = self.determine_party_leader()

        # ChibiPortraitSurf
        chibi = engine.create_surface((96, WINHEIGHT + 24), transparent=True)
        if unit.portrait_nid:  # Can be None if unit is generic
            icons.draw_chibi(chibi, unit.portrait_nid, (7, 8))
        surfaces.append((chibi, (WINWIDTH - 44, 111)))

        # PartyLeaderSurf stats function
        def party_leader_surf(pos, font=None, game_get=None):
            size = FONT[font].width(str(game_get)) + 1, FONT[font].height
            surf = engine.create_surface(size, transparent=True)
            FONT[font].blit(str(game_get), surf, (0, 0))
            surfaces.append((surf, (WINWIDTH - pos[0] - surf.get_width(), pos[1])))

        # Party Leader Name surf
        party_leader_surf((42, 104), 'text-white', unit.name)

        # Party Leader Level Surf
        party_golden_words_surf(0, 48, 16, 24, (140, 122))
        party_leader_surf((42, 120), 'text-blue', unit.level)

        # Party Leader HP Surf
        party_golden_words_surf(16, 48, 20, 24, (140, 136))
        HitPoints_size = FONT['text-blue'].width(str(unit.get_hp())) + 20, FONT['text-blue'].height
        HitPoints_surf = engine.create_surface(HitPoints_size, transparent=True)
        FONT['text-blue'].blit(str(unit.get_hp()) + '/' + str(unit.get_max_hp()), HitPoints_surf, (0, 0))
        surfaces.append((HitPoints_surf, (WINWIDTH - 42 - HitPoints_surf.get_width(), 134)))

        return surfaces

    def determine_party_leader(self) -> UnitObject:
        if game.is_roam():
            unit = game.get_roam_unit()
        else:
            unit = game.get_unit(game.get_party().leader_nid)

        if not unit:
            # Try for a lord
            all_units = game.get_units_in_party()
            lord_units = [unit for unit in all_units if 'Lord' in unit.tags]
            if lord_units:
                unit = lord_units[0]
            elif all_units:
                unit = all_units[0]

        return unit

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        if 'DOWN' in directions:
            self.menu.move_down(first_push)
            get_sound_thread().play_sfx('Select 6')
        elif 'UP' in directions:
            self.menu.move_up(first_push)
            get_sound_thread().play_sfx('Select 6')
        if event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            # game.state.back()
            game.state.change('transition_pop')

    def update(self):
        if self.menu:
            self.menu.update()

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)

        self.menu.draw(surf)

        # Non moving surfaces
        for surface, pos in self.surfaces:
            surf.blit(surface, pos)

        # Map Sprite
        # Determine party leader
        unit = self.determine_party_leader()
        if unit:
            mapsprite = unit.sprite.create_image('passive')
            surf.blit(mapsprite, (124, 82))

        # Playtime
        time = datetime.timedelta(milliseconds=game.playtime)
        seconds = int(time.total_seconds())
        hours = min(seconds//3600, 99)
        minutes = str((seconds%3600)//60)
        if len(minutes) < 2:
            minutes = '0' + minutes
        seconds = str(seconds%60)
        if len(seconds) < 2:
            seconds = '0' + seconds

        formatted_time = ':'.join([str(hours), minutes, seconds])
        FONT['text-blue'].blit_right(formatted_time, surf, (WINWIDTH - 8, 38))

        return surf
