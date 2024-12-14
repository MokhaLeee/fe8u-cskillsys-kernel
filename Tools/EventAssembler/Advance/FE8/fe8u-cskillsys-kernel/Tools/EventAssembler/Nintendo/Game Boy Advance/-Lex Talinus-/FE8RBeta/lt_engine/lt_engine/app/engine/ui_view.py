from app.engine.graphics.text.text_renderer import fix_tags, render_text, text_width
import logging
from app.engine.text_evaluator import TextEvaluator
import app.engine.config as cf
from app.constants import TILEX, TILEY, WINHEIGHT, WINWIDTH
from app.data.database.database import DB
from app.data.database.difficulty_modes import RNGOption
from app.engine import (base_surf, combat_calcs, engine, equations, evaluate,
                        icons, image_mods, item_funcs, item_system,
                        skill_system, text_funcs)
from app.engine.fonts import FONT
from app.engine.game_menus import menu_options
from app.engine.game_counters import ANIMATION_COUNTERS
from app.engine.game_state import game
from app.engine.sprites import SPRITES
from app.utilities import utils
from app.utilities.enums import HAlignment

from typing import List
from app.engine.combat.utils import resolve_weapon


class UIView():
    legal_states = ('free', 'prep_formation', 'prep_formation_select')
    initiative_states = ('status_endstep', 'turn_change', 'ai', 'phase_change', 'menu', 'turnwheel')
    x_positions = (0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 6, 6, 6, 5, 4, 3, 2, 1)
    y_positions = (0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0, 0, 0, 0, 0, 0)

    def __init__(self):
        self.unit_info_disp = None
        self.tile_info_disp = None
        self.obj_info_disp = None
        self.attack_info_disp = None
        self.spell_info_disp = None
        self.initiative_info_disp = None

        self.cursor_right: bool = False

        self.unit_info_offset = 0
        self.obj_info_offset = 0
        self.attack_info_offset = 0
        self.initiative_info_offset = 0

        # Tile Info Offset
        self.tile_transition_state = 'normal'
        self.tile_progress: float = 0
        self.tile_last_update: int = 0
        self.current_tile_pos = None

        self.remove_unit_info = True
        self.prev_unit_info_top = False
        self.obj_top = False

    def remove_unit_display(self):
        self.remove_unit_info = True

    def get_cursor_right(self):
        return game.cursor.position[0] > TILEX // 2 + game.camera.get_x() - 1

    def update(self):
        current_time = engine.get_time()

        # Tile info handling
        if self.cursor_right == self.get_cursor_right():
            pass  # No need to transition
        else:
            self.cursor_right = self.get_cursor_right()
            self.tile_transition_state = 'out'
            self.tile_progress = 0
            self.tile_last_update = engine.get_time()

        # Handle tile info slide in
        if self.tile_transition_state != 'normal':
            diff = current_time - self.tile_last_update
            self.tile_progress = utils.clamp(diff / utils.frames2ms(4), 0, 1)
            if self.tile_progress >= 1:
                self.tile_progress = 0
                self.tile_last_update = current_time
                if self.tile_transition_state == 'out':
                    self.tile_transition_state = 'in'
                    self.current_tile_pos = game.cursor.position
                else:
                    self.tile_transition_state = 'normal'
        else:
            self.current_tile_pos = game.cursor.position

    def draw(self, surf):
        self.update()
        # Unit info handling
        if self.remove_unit_info:
            hover = game.cursor.get_hover()
            if game.state.current() in self.legal_states and hover:
                self.remove_unit_info = False
                self.unit_info_disp = self.create_unit_info(hover)
                self.unit_info_offset = min(self.unit_info_disp.get_width(), self.unit_info_offset)
            elif self.unit_info_disp:
                self.unit_info_offset += 20
                if self.unit_info_offset >= 200:
                    self.unit_info_disp = None
        else:
            self.unit_info_offset -= 20
            self.unit_info_offset = max(0, self.unit_info_offset)

        # Objective info handling
        if game.state.current() in self.legal_states and cf.SETTINGS['show_objective']:
            self.obj_info_disp = self.create_obj_info()
            self.obj_info_offset -= 10
            self.obj_info_offset = max(0, self.obj_info_offset)
        elif self.obj_info_disp:
            self.obj_info_offset += 10
            if self.obj_info_offset >= 100:
                self.obj_info_disp = None

        if (game.state.current() in self.legal_states or game.state.current() in self.initiative_states) \
                and DB.constants.value('initiative') \
                and not game.is_roam() and game.initiative.draw_me:
            self.initiative_info_disp = self.create_initiative_info()
            self.initiative_info_offset = max(0, self.initiative_info_offset)
        elif self.initiative_info_disp:
            if self.initiative_info_offset >= 200:
                self.initiative_info_disp = None

        if DB.constants.value('initiative') and not game.initiative.draw_me:
            self.initiative_info_disp = None

        # === Final drawing
        # Should be in topleft, unless cursor is in topleft, in which case it should be in bottomleft
        if game.state.current() in self.legal_states and self.unit_info_disp:
            # If in top and not in right
            if not DB.constants.value('initiative') or not game.initiative.draw_me:
                if self.remove_unit_info:
                    if self.prev_unit_info_top:
                        surf.blit(self.unit_info_disp, (-self.unit_info_offset, 0))
                    else:
                        surf.blit(self.unit_info_disp, (-self.unit_info_offset, WINHEIGHT - self.unit_info_disp.get_height()))
                elif game.cursor.position[1] < TILEY // 2 + game.camera.get_y() and \
                        not (game.cursor.position[0] > TILEX // 2 + game.camera.get_x() - 1):
                    self.prev_unit_info_top = False
                    surf.blit(self.unit_info_disp, (-self.unit_info_offset, WINHEIGHT - self.unit_info_disp.get_height()))
                else:
                    self.prev_unit_info_top = True
                    surf.blit(self.unit_info_disp, (-self.unit_info_offset, 0))
            else:
                pass

        if game.state.current() in self.legal_states and cf.SETTINGS['show_terrain'] and \
                (game.level_vars['_fog_of_war'] != 2 or game.board.in_vision(game.cursor.position)):
            self.tile_info_disp = self.create_tile_info(self.current_tile_pos)
            if self.tile_info_disp:
                right = self.cursor_right

                # Handle transition offset
                if self.tile_transition_state == 'out':
                    right = not right  # Mirror on the way out
                    offset = self.tile_info_disp.get_width() * self.tile_progress
                elif self.tile_transition_state == 'in':
                    offset = self.tile_info_disp.get_width() * (1 - self.tile_progress)
                else:
                    offset = 0

                # Should be in bottom, no matter what. Can be in bottomleft or bottomright, depending on where cursor is
                if right:
                    surf.blit(self.tile_info_disp, (5 - offset, WINHEIGHT - self.tile_info_disp.get_height() - 3)) # Bottomleft
                else:
                    xpos = WINWIDTH - self.tile_info_disp.get_width() - 5 + offset
                    ypos = WINHEIGHT - self.tile_info_disp.get_height() - 3
                    surf.blit(self.tile_info_disp, (xpos, ypos)) # Bottomright

        if self.obj_info_disp and not self.initiative_info_disp:
            # Should be in topright, unless the cursor is in the topright
            # TopRight - I believe this has RIGHT precedence
            if game.cursor.position[1] < TILEY // 2 + game.camera.get_y() and \
                    game.cursor.position[0] > TILEX // 2 + game.camera.get_x() - 1:
                # Gotta place in bottomright, because cursor is in topright
                if self.obj_top:
                    self.obj_top = False
                    self.obj_info_offset = self.obj_info_disp.get_height()
                pos = (WINWIDTH - 4 - self.obj_info_disp.get_width(),
                       WINHEIGHT - 4 + self.obj_info_offset - self.obj_info_disp.get_height())
                surf.blit(self.obj_info_disp, pos) # Should be bottom right
            else:
                # Place in topright
                if not self.obj_top:
                    self.obj_top = True
                    self.obj_info_offset = self.obj_info_disp.get_height()
                pos = (WINWIDTH - 4 - self.obj_info_disp.get_width(), 1 - self.obj_info_offset)
                surf.blit(self.obj_info_disp, pos)

        if self.initiative_info_disp:
            surf.blit(self.initiative_info_disp, (0, 0))

        return surf

    def create_initiative_info(self):
        x_increment = 20
        y_offset = 0
        surf = engine.subsurface(SPRITES.get('bg_black').copy(), (0, 0, WINWIDTH, 40))
        surf = image_mods.make_translucent(surf, .75)

        current_unit = game.initiative.get_current_unit()
        unit_list = game.initiative.unit_line[:]
        current_idx = game.initiative.current_idx
        min_scroll, max_scroll = current_idx - 9, current_idx + 10
        min_scroll = max(min_scroll, 0)
        max_scroll = min(max_scroll, len(unit_list))
        unit_list = unit_list[min_scroll:max_scroll]

        for idx, unit_nid in enumerate(unit_list):
            unit = game.get_unit(unit_nid)
            if current_unit and unit is current_unit:
                y_offset = 10
                char_sprite = unit.sprite.create_image('active')
            else:
                y_offset = 0
                char_sprite = unit.sprite.create_image('passive')
            surf.blit(SPRITES.get('initiative_platform'), (idx * x_increment, 8 + y_offset))
            surf.blit(char_sprite, (-17 + idx * x_increment, -19 + y_offset))
        return surf

    def create_unit_info(self, unit):
        font = FONT['info-grey']
        dimensions = (112, 40)
        width, height = dimensions
        surf = SPRITES.get('unit_info_bg').copy()
        top, left = 4, 6
        if unit.generic:
            icons.draw_faction(surf, DB.factions.get(unit.faction), (left + 1, top + 4))
        else:
            portrait_nid = unit.portrait_nid
            icons.draw_chibi(surf, portrait_nid, (left + 1, top + 4))

        name = unit.name
        if unit.generic:
            short_name = DB.classes.get(unit.klass).name
            name = short_name + ' ' + str(unit.level)
        pos = (left + width//2 + 6 - font.width(name)//2, top + 4)
        font.blit(name, surf, pos)

        # Health text
        surf.blit(SPRITES.get('unit_info_hp'), (left + 34, top + height - 20))
        surf.blit(SPRITES.get('unit_info_slash'), (left + 66, top + height - 19))
        current_hp = unit.get_hp()
        max_hp = equations.parser.hitpoints(unit)
        font.blit_right(str(current_hp), surf, (left + 66, top + 16))
        font.blit_right(str(max_hp), surf, (left + 90, top + 16))

        # Health BG
        bg_surf = SPRITES.get('health_bar2_bg')
        surf.blit(bg_surf, (left + 36, top + height - 10))

        # Health Bar
        hp_ratio = utils.clamp(current_hp / float(max_hp), 0, 1)
        if hp_ratio > 0:
            hp_surf = SPRITES.get('health_bar2')
            idx = int(hp_ratio * hp_surf.get_width())
            hp_surf = engine.subsurface(hp_surf, (0, 0, idx, 2))
            surf.blit(hp_surf, (left + 37, top + height - 9))

        # Weapon Icon
        weapon = unit.get_weapon()
        icon = icons.get_icon(weapon)
        if icon:
            pos = (left + width - 20, top + height//2 - 8)
            # icon = item_system.item_icon_mod(unit, weapon, defender, icon)
            surf.blit(icon, pos)
        return surf

    def create_tile_info(self, coord):
        terrain_nid = game.get_terrain_nid(game.tilemap, coord)
        terrain = DB.terrain.get(terrain_nid)
        current_unit = game.board.get_unit(coord)
        if current_unit and 'Tile' in current_unit.tags:
            current_hp = current_unit.get_hp()
            bg_surf = SPRITES.get('tile_info_destructible_opaque').copy()
            bg_surf = image_mods.make_translucent(bg_surf, .1)
            at_icon = SPRITES.get('icon_attackable_terrain')
            bg_surf.blit(at_icon, (7, bg_surf.get_height() - 7 - at_icon.get_height()))
            cur = str(current_hp)
            render_text(bg_surf, ['small'], [cur], [None], (bg_surf.get_width() - 9, 24), HAlignment.RIGHT)
        else:
            bg_surf = SPRITES.get('tile_info_quick_opaque').copy()
            bg_surf = image_mods.make_translucent(bg_surf, .1)
            tile_def, tile_avoid = 0, 0
            if terrain.status:
                status_prefab = DB.skills.get(terrain.status)
                if status_prefab:
                    for component in status_prefab.components:
                        if component.defines('tile_def'):
                            tile_def += component.tile_def()
                        if component.defines('tile_avoid'):
                            tile_avoid += component.tile_avoid()
                else:
                    logging.error("Could not find status %s for terrain %s", terrain.status, terrain.nid)
            render_text(bg_surf, ['small'], [str(tile_def)], [None], (bg_surf.get_width() - 4, 17), HAlignment.RIGHT)
            render_text(bg_surf, ['small'], [str(tile_avoid)], [None], (bg_surf.get_width() - 4, 25), HAlignment.RIGHT)

        name = terrain.name
        width = text_width('text', name)
        height = FONT['text'].height
        pos = (bg_surf.get_width()//2 - width//2, 22 - height)
        render_text(bg_surf, ['text'], [name], [None], pos)
        return bg_surf

    def create_obj_info(self):
        obj = game.level.objective['simple']
        text_parser = TextEvaluator(logging.getLogger(), game)
        text_lines = text_parser._evaluate_all(obj).split(',')
        text_lines = [line.replace('{comma}', ',') for line in text_lines]
        text_lines = fix_tags(text_lines)
        longest_surf_width = text_funcs.get_max_width('text', text_lines)
        bg_surf = base_surf.create_base_surf(longest_surf_width + 16, 16 * len(text_lines) + 8)

        if len(text_lines) == 1:
            shimmer = SPRITES.get('menu_shimmer1')
        else:
            shimmer = SPRITES.get('menu_shimmer2')
        bg_surf.blit(shimmer, (bg_surf.get_width() - 1 - shimmer.get_width(), 4 + 16 * max(len(text_lines) - 2, 0)))
        surf = engine.create_surface((bg_surf.get_width(), bg_surf.get_height() + 3), transparent=True)
        surf.blit(bg_surf, (0, 3))
        gem = SPRITES.get('combat_gem_blue')
        surf.blit(gem, (bg_surf.get_width()//2 - gem.get_width()//2, 0))
        surf = image_mods.make_translucent(surf, .1)

        for idx, line in enumerate(text_lines):
            pos = (surf.get_width()//2 - text_width('text', line)//2, 16 * idx + 6)
            render_text(surf, ['text'], [line], [None], pos)

        return surf

    def prepare_attack_info(self):
        self.attack_info_disp = None
        self.attack_info_offset = 80

    def reset_info(self):
        self.attack_info_disp = None
        self.spell_info_disp = None

    def create_attack_info(self, attacker, weapon, defender, a_assist=None, d_assist=None):
        def blit_num(surf, num, x_pos, y_pos):
            if num is None or num == '--':
                # x_pos - 1 to center -- with general center of 2 digit numbers
                FONT['text-blue'].blit_right('--', surf, (x_pos - 1, y_pos))
                return
            if not isinstance(num, str) and num >= 100:
                surf.blit(SPRITES.get('blue_100'), (x_pos - 15, y_pos))
            else:
                FONT['text-blue'].blit_right(str(num), surf, (x_pos, y_pos))

        crit_flag = DB.constants.value('crit')
        grandmaster = game.mode.rng_choice == RNGOption.GRANDMASTER
        if grandmaster:  # Grandmaster takes precedence
            crit_flag = False

        # Choose attack info background
        prefix = 'attack_info_'
        if grandmaster:
            infix = 'grandmaster'
        elif crit_flag:
            infix = 'crit'
        else:
            infix = ''

        color = DB.teams.get(defender.team).combat_color
        final = prefix + infix + ('_' if infix else '') + color
        fallback_final = prefix + infix + ('_' if infix else '') + 'red'
        surf = SPRITES.get(final, fallback_final).copy()

        if DB.constants.value('pairup') and \
                (a_assist or d_assist) and not (attacker.traveler or defender.traveler):
            if a_assist:
                # Background boxes
                prefix = 'assist_info_'
                if grandmaster:
                    infix = 'grandmaster'
                elif crit_flag:
                    infix = 'crit'
                else:
                    infix = ''
                color = DB.teams.get(attacker.team).combat_color
                final = prefix + infix + ('_' if infix else '') + color
                fallback_final = prefix + infix + ('_' if infix else '') + 'red'
                surf.blit(SPRITES.get(final, fallback_final).copy(), (92, 35))

                mt = combat_calcs.compute_assist_damage(a_assist, defender, a_assist.get_weapon(), resolve_weapon(defender), 'attack', (0, 0))
                if grandmaster:
                    hit = utils.clamp(combat_calcs.compute_hit(a_assist, defender, a_assist.get_weapon(), resolve_weapon(defender), 'attack', (0, 0)), 0, 100)
                    blit_num(surf, int(mt * float(hit) / 100), 112, 35)
                else:
                    blit_num(surf, mt, 112, 35)
                    hit = combat_calcs.compute_hit(a_assist, defender, a_assist.get_weapon(), resolve_weapon(defender), 'attack', (0, 0))
                    blit_num(surf, hit, 112, 51)
                    # Blit crit if applicable
                    if crit_flag:
                        c = combat_calcs.compute_crit(a_assist, defender, a_assist.get_weapon(), resolve_weapon(defender), 'attack', (0, 0))
                        blit_num(surf, c, 112, 67)

            if d_assist and defender.get_weapon() and \
                    combat_calcs.can_counterattack(attacker, weapon, defender, defender.get_weapon()):
                # Background boxes
                prefix = 'assist_info_'
                if grandmaster:
                    infix = 'grandmaster'
                elif crit_flag:
                    infix = 'crit'
                else:
                    infix = ''
                color = DB.teams.get(defender.team).combat_color
                final = prefix + infix + ('_' if infix else '') + color
                fallback_final = prefix + infix + ('_' if infix else '') + 'red'
                surf.blit(SPRITES.get(final, fallback_final).copy(), (1, 35))

                mt = combat_calcs.compute_assist_damage(d_assist, attacker, d_assist.get_weapon(), weapon, 'defense', (0, 0))
                if grandmaster:
                    hit = utils.clamp(combat_calcs.compute_hit(d_assist, attacker, d_assist.get_weapon(), weapon, 'defense', (0, 0)), 0, 100)
                    blit_num(surf, int(mt * float(hit) / 100), 21, 35)
                else:
                    blit_num(surf, mt, 21, 35)
                    hit = combat_calcs.compute_hit(d_assist, attacker, d_assist.get_weapon(), weapon, 'defense', (0, 0))
                    blit_num(surf, hit, 21, 51)
                    # Blit crit if applicable
                    if crit_flag:
                        c = combat_calcs.compute_crit(d_assist, attacker, d_assist.get_weapon(), weapon, 'defense', (0, 0))
                        blit_num(surf, c, 21, 67)
        # Now make everything translucent
        surf = image_mods.make_translucent(surf, .1)

        # Name
        width = text_width('text', attacker.name)
        render_text(surf, ['text'], [attacker.name], ['white'], (67 - width//2, 3))
        # Enemy name
        y_pos = 84
        if not crit_flag:
            y_pos -= 16
        if grandmaster:
            y_pos -= 16
        position = 50 - text_width('text', defender.name)//2, y_pos
        render_text(surf, ['text'], [defender.name], ['white'], position)
        # Enemy Weapon
        if defender.get_weapon():
            width = text_width('text', defender.get_weapon().name)
            y_pos = 100
            if not crit_flag:
                y_pos -= 16
            if grandmaster:
                y_pos -= 16
            position = 56 - width//2, y_pos
            render_text(surf, ['text'], [defender.get_weapon().name], ['white'], position)
        # Self HP
        blit_num(surf, attacker.get_hp(), 88, 19)
        # Enemy HP
        blit_num(surf, defender.get_hp(), 44, 19)
        # Self MT
        mt = combat_calcs.compute_damage(attacker, defender, weapon, resolve_weapon(defender), 'attack', (0, 0))
        if grandmaster:
            hit = utils.clamp(combat_calcs.compute_hit(attacker, defender, weapon, resolve_weapon(defender), 'attack', (0, 0)), 0, 100)
            blit_num(surf, int(mt * float(hit) / 100), 88, 35)
        else:
            blit_num(surf, mt, 88, 35)
            hit = combat_calcs.compute_hit(attacker, defender, weapon, resolve_weapon(defender), 'attack', (0, 0))
            blit_num(surf, hit, 88, 51)
            # Blit crit if applicable
            if crit_flag:
                c = combat_calcs.compute_crit(attacker, defender, weapon, resolve_weapon(defender), 'attack', (0, 0))
                blit_num(surf, c, 88, 67)
        # Enemy Hit and Mt
        if defender.get_weapon() and \
                combat_calcs.can_counterattack(attacker, weapon, defender, defender.get_weapon()):
            e_mt = combat_calcs.compute_damage(defender, attacker, resolve_weapon(defender), weapon, 'defense', (0, 0))
            e_hit = combat_calcs.compute_hit(defender, attacker, resolve_weapon(defender), weapon, 'defense', (0, 0))
            if crit_flag:
                e_crit = combat_calcs.compute_crit(defender, attacker, resolve_weapon(defender), weapon, 'defense', (0, 0))
            else:
                e_crit = 0
        else:
            e_mt = '--'
            e_hit = '--'
            e_crit = '--'

        if grandmaster:
            if e_mt == '--' or e_hit == '--':
                blit_num(surf, e_mt, 44, 35)
            else:
                blit_num(surf, int(e_mt * float(e_hit) / 100), 44, 35)
        else:
            blit_num(surf, e_mt, 44, 35)
            blit_num(surf, e_hit, 44, 51)
            if crit_flag:
                blit_num(surf, e_crit, 44, 67)

        return surf

    def draw_adv_arrows(self, surf, attacker, defender, weapon, def_weapon, topleft):
        adv = combat_calcs.compute_advantage(attacker, defender, weapon, def_weapon)
        disadv = combat_calcs.compute_advantage(attacker, defender, weapon, def_weapon, False)

        up_arrow = engine.subsurface(SPRITES.get('arrow_advantage'), (ANIMATION_COUNTERS.arrow_counter.count * 7, 0, 7, 10))
        down_arrow = engine.subsurface(SPRITES.get('arrow_advantage'), (ANIMATION_COUNTERS.arrow_counter.count * 7, 10, 7, 10))

        if adv and adv.modification > 0:
            surf.blit(up_arrow, topleft)
        elif adv and adv.modification < 0:
            surf.blit(down_arrow, topleft)
        elif disadv and disadv.modification > 0:
            surf.blit(down_arrow, topleft)
        elif disadv and disadv.modification < 0:
            surf.blit(up_arrow, topleft)

    def draw_attack_info(self, surf, attacker, weapon, defender, a_assist=None, d_assist=None):
        # Turns on appropriate combat conditionals to get an accurate read
        skill_system.test_on([], attacker, weapon, defender, resolve_weapon(defender), 'attack')
        skill_system.test_on([], defender, resolve_weapon(defender), attacker, weapon, 'defense')

        def has_attacker_strike_partner() -> bool:
            return DB.constants.value('pairup') and \
                a_assist and not (attacker.traveler or defender.traveler)

        def has_defender_strike_partner() -> bool:
            return DB.constants.value('pairup') and \
                d_assist and not (attacker.traveler or defender.traveler) and \
                defender.get_weapon() and \
                combat_calcs.can_counterattack(attacker, weapon, defender, defender.get_weapon())

        if not self.attack_info_disp:
            self.attack_info_disp = self.create_attack_info(attacker, weapon, defender, a_assist, d_assist)

        crit_flag = DB.constants.value('crit')
        grandmaster = game.mode.rng_choice == RNGOption.GRANDMASTER
        if grandmaster:  # Grandmaster takes precedence
            crit_flag = False

        if game.cursor.position[0] > TILEX // 2 + game.camera.get_x() - 1:
            if has_defender_strike_partner():
                topleft = (5 - self.attack_info_offset, 4)
            else:
                topleft = (-19 - self.attack_info_offset, 4)
        else:
            if has_attacker_strike_partner():
                topleft = (WINWIDTH - 122 + self.attack_info_offset, 4)
            else:
                topleft = (WINWIDTH - 97 + self.attack_info_offset, 4)
        if self.attack_info_offset > 0:
            self.attack_info_offset -= 20

        surf.blit(self.attack_info_disp, topleft)

        # Attacker Item
        icon = icons.get_icon(weapon)
        if icon:
            icon = item_system.item_icon_mod(attacker, weapon, defender, icon)
            surf.blit(icon, (topleft[0] + 26, topleft[1] + 4))

        # Defender Item
        if defender.get_weapon():
            eweapon = defender.get_weapon()
            icon = icons.get_icon(eweapon)
            if icon:
                icon = item_system.item_icon_mod(defender, eweapon, attacker, icon)
                y_pos = topleft[1] + 83
                if not crit_flag:
                    y_pos -= 16
                if grandmaster:
                    y_pos -= 16
                surf.blit(icon, (topleft[0] + 74, y_pos))

        # Advantage arrows
        if skill_system.check_enemy(attacker, defender):
            self.draw_adv_arrows(surf, attacker, defender, weapon, resolve_weapon(defender), (topleft[0] + 37, topleft[1] + 8))

            y_pos = topleft[1] + 89
            if not crit_flag:
                y_pos -= 16
            if grandmaster:
                y_pos -= 16

            self.draw_adv_arrows(surf, defender, attacker, resolve_weapon(defender), weapon, (topleft[0] + 85, y_pos))

        # Doubling
        count = ANIMATION_COUNTERS.x2_counter.count
        x2_pos_player = (topleft[0] + 83 + self.x_positions[count], topleft[1] + 38 + self.y_positions[count])
        x2_pos_enemy = (topleft[0] + 44 + self.x_positions[count], topleft[1] + 38 + self.y_positions[count])
        x2_pos_player_partner = (topleft[0] + 107 + self.x_positions[count], topleft[1] + 38 + self.y_positions[count])
        x2_pos_enemy_partner = (topleft[0] + 20 + self.x_positions[count], topleft[1] + 38 + self.y_positions[count])

        my_num = combat_calcs.outspeed(attacker, defender, weapon, resolve_weapon(defender), "attack", (0, 0))
        my_num *= combat_calcs.compute_multiattacks(attacker, defender, weapon, "attack", (0, 0))
        if weapon.uses_options and weapon.uses_options.one_loss_per_combat():
            pass  # If you can only lose one use at a time, no need to min this
        else:
            my_num = min(my_num, weapon.data.get('uses', 100), weapon.data.get('c_uses', 100))

        if my_num != 1:
            surf.blit(SPRITES.get("x%d" % (my_num)), x2_pos_player)

        if a_assist:
            if my_num != 1 and not DB.constants.value('limit_attack_stance'):
                surf.blit(SPRITES.get("x%d" % (my_num)), x2_pos_player_partner)

        # Enemy doubling
        eweapon = defender.get_weapon()
        if eweapon and combat_calcs.can_counterattack(attacker, weapon, defender, eweapon):
            if DB.constants.value('def_double') or skill_system.def_double(defender):
                e_num = combat_calcs.outspeed(defender, attacker, eweapon, weapon, 'defense', (0, 0))
            else:
                e_num = 1
            e_num *= combat_calcs.compute_multiattacks(defender, attacker, eweapon, 'defense', (0, 0))
            e_num = min(e_num, eweapon.data.get('uses', 100))

            if e_num != 1:
                surf.blit(SPRITES.get("x%d" % (e_num)), x2_pos_enemy)

            if d_assist:
                if e_num != 1 and not DB.constants.value('limit_attack_stance'):
                    surf.blit(SPRITES.get("x%d" % (e_num)), x2_pos_enemy_partner)

        # Turns off combat conditionals
        skill_system.test_off([], defender, resolve_weapon(defender), attacker, weapon, 'defense')
        skill_system.test_off([], attacker, weapon, defender, resolve_weapon(defender), 'attack')

        return surf

    def create_spell_info(self, attacker, spell, defender):
        if defender:
            height = 2
            mt = combat_calcs.compute_damage(attacker, defender, spell, resolve_weapon(defender), 'attack', (0, 0))
            if mt is not None:
                height += 1
            hit = combat_calcs.compute_hit(attacker, defender, spell, resolve_weapon(defender), 'attack', (0, 0))
            if hit is not None:
                height += 1
            crit = combat_calcs.compute_crit(attacker, defender, spell, resolve_weapon(defender), 'attack', (0, 0))
            if DB.constants.value('crit') and crit is not None:
                height += 1

            bg_surf = SPRITES.get('spell_window' + str(height))
            bg_surf = image_mods.make_translucent(bg_surf, .1)
            width, height = bg_surf.get_width(), bg_surf.get_height()

            running_height = 8

            FONT['text'].blit(defender.name, bg_surf, (30, running_height))

            running_height += 16
            # Blit HP
            FONT['text-yellow'].blit('HP', bg_surf, (9, running_height))
            # Blit /
            FONT['text-yellow'].blit('/', bg_surf, (width - 25, running_height))
            # Blit stats['HP']
            maxhp = str(equations.parser.hitpoints(defender))
            maxhp_width = FONT['text-blue'].width(maxhp)
            FONT['text-blue'].blit(maxhp, bg_surf, (width - 5 - maxhp_width, running_height))
            # Blit currenthp
            currenthp = str(defender.get_hp())
            currenthp_width = FONT['text-blue'].width(currenthp)
            FONT['text-blue'].blit(currenthp, bg_surf, (width - 26 - currenthp_width, running_height))

            if mt is not None:
                running_height += 16
                FONT['text-yellow'].blit('Mt', bg_surf, (9, running_height))
                mt_width = FONT['text-blue'].width(str(mt))
                FONT['text-blue'].blit(str(mt), bg_surf, (width - 5 - mt_width, running_height))

            if hit is not None:
                running_height += 16
                FONT['text-yellow'].blit('Hit', bg_surf, (9, running_height))
                if hit >= 100:
                    bg_surf.blit(SPRITES.get('blue_100'), (width - 21, running_height))
                else:
                    hit_width = FONT['text-blue'].width(str(hit))
                    position = width - 5 - hit_width, running_height
                    FONT['text-blue'].blit(str(hit), bg_surf, position)

            if DB.constants.value('crit') and crit is not None:
                running_height += 16
                FONT['text-yellow'].blit('Crit', bg_surf, (9, running_height))
                if crit >= 100:
                    bg_surf.blit(SPRITES.get('blue_100'), (width - 21, running_height))
                else:
                    crit_width = FONT['text-blue'].width(str(crit))
                    position = width - 5 - crit_width, running_height
                    FONT['text-blue'].blit(str(crit), bg_surf, position)

            # Blit name
            running_height += 16
            name_width = text_width('text', spell.name)
            render_text(bg_surf, ['text'], [spell.name], ['white'], (52 - name_width//2, running_height))

            return bg_surf

        else:
            height = 24
            mt = combat_calcs.damage(attacker, spell)
            if mt is not None:
                height += 16
            real_surf = base_surf.create_base_surf((80, height), 'menu_bg_base')
            bg_surf = engine.create_surface((real_surf.get_width() + 2, real_surf.get_height() + 4), transparent=True)
            bg_surf.blit(real_surf, (2, 4))
            bg_surf.blit(SPRITES.get('menu_gem_small'), (0, 0))
            shimmer = SPRITES.get('menu_shimmer1')
            bg_surf.blit(shimmer, (bg_surf.get_width() - shimmer.get_width() - 1, bg_surf.get_height() - shimmer.get_height() - 5))
            bg_surf = image_mods.make_translucent(bg_surf, .1)
            width, height = bg_surf.get_width(), bg_surf.get_height()

            running_height = -10

            if mt is not None:
                running_height += 16
                FONT['text-yellow'].blit('Mt', bg_surf, (5, running_height))
                mt_size = FONT['text-blue'].width(str(mt))
                FONT['text-blue'].blit(str(mt), bg_surf, (width - 5 - mt_size, running_height))

            # Blit name
            running_height += 16
            icons.draw(bg_surf, spell, (4, running_height))
            name_width = text_width('text', spell.name)
            render_text(bg_surf, ['text'], [spell.name], ['white'], (52 - name_width//2, running_height))

            return bg_surf

    def prepare_spell_info(self):
        self.spell_info_disp = None

    def draw_spell_info(self, surf, attacker, spell, defender):
        # Turns on appropriate combat conditionals to get accurate stats
        skill_system.test_on([], attacker, spell, defender, resolve_weapon(defender), 'attack')

        if not self.spell_info_disp:
            self.spell_info_disp = self.create_spell_info(attacker, spell, defender)
            if self.spell_info_disp:
                return

        width = self.spell_info_disp.get_width()
        if defender:
            unit_surf = defender.sprite.create_image('passive')

        if game.cursor.position[0] > TILEX // 2 + game.camera.get_x() - 1:
            topleft = (4, 4)
            if defender:
                u_topleft = (16 - max(0, (unit_surf.get_width() - 16)//2), 12 - max(0, (unit_surf.get_width() - 16)//2))
        else:
            topleft = (WINWIDTH - 4 - width, 4)
            if defender:
                u_topleft = (WINWIDTH - width + 8 - max(0, (unit_surf.get_width() - 16)//2), 12 - max(0, (unit_surf.get_width() - 16)//2))

        surf.blit(self.spell_info_disp, topleft)
        if defender:
            surf.blit(unit_surf, u_topleft)

        icon = icons.get_icon(spell)
        if icon:
            icon = item_system.item_icon_mod(attacker, spell, defender, icon)
            surf.blit(icon, (topleft[0] + 8, topleft[1] + self.spell_info_disp.get_height() - 20))

        # Turns off combat conditionals
        skill_system.test_off([], attacker, spell, defender, resolve_weapon(defender), 'attack')

        return surf

    @staticmethod
    def draw_trade_preview(unit, surf, ignore: List[bool] = None):
        items = unit.items
        ignore = ignore or [False for _ in items]
        # Build window
        window = SPRITES.get('trade_window')
        width, height = window.get_width(), window.get_height()
        top = engine.subsurface(window, (0, 0, width, 27))
        bottom = engine.subsurface(window, (0, height - 5, width, 5))
        middle = engine.subsurface(window, (0, height//2 + 3, width, 16))
        size = (width, -2 + 27 + 5 + 16 * max(1, len(items)))
        bg_surf = engine.create_surface(size, transparent=True)
        bg_surf.blit(top, (0, 0))

        for idx, item in enumerate(items):
            bg_surf.blit(middle, (0, 27 + idx * 16))
        if not items:
            bg_surf.blit(middle, (0, 27))
        bg_surf.blit(bottom, (0, size[1] - 5))
        bg_surf = image_mods.make_translucent(bg_surf, .1)

        for idx, item in enumerate(items):
            item_option = menu_options.ItemOption(idx, item)
            item_option.ignore = ignore[idx]
            item_option.draw(bg_surf, 5, 27 + idx * 16 - 2)
        if not items:
            FONT['text-grey'].blit('Nothing', bg_surf, (25, 27 - 2))

        unit_sprite = unit.sprite.create_image('passive')
        FONT['text'].blit(unit.name, bg_surf, (32, 8))

        if game.cursor.position[0] > TILEX//2 + game.camera.get_x() - 1:
            topleft = (0, 0)
        else:
            topleft = (WINWIDTH - 4 - window.get_width(), 0)
        surf.blit(bg_surf, topleft)

        surf.blit(unit_sprite, (topleft[0] - 12, topleft[1] - 16))

        return surf


class ItemDescriptionPanel():
    """
    The panel that shows up in the weapon selection state
    opposite the selection menu
    """

    def __init__(self, unit, item):
        self.unit = unit
        self.item = item
        self.surf = None

    def set_item(self, item):
        self.item = item
        self.surf = None

    def create_surf(self):
        width, height = 96, 56
        sub_bg_surf = base_surf.create_base_surf(width, height, 'menu_bg_base')
        bg_surf = engine.create_surface((width + 2, height + 4), transparent=True)
        bg_surf.blit(sub_bg_surf, (2, 4))
        bg_surf.blit(SPRITES.get('menu_gem_small'), (0, 0))
        bg_surf = image_mods.make_translucent(bg_surf, .1)

        weapon = item_system.is_weapon(self.unit, self.item)
        available = item_funcs.available(self.unit, self.item)

        if weapon and available:
            top = 4
            left = 2
            affin_width = FONT['text'].width('Affin')
            FONT['text'].blit('Affin', bg_surf, (left + width//2 - 16//2 - affin_width//2, top + 4))
            FONT['text'].blit('Atk', bg_surf, (5 + left, top + 20))
            FONT['text'].blit('Hit', bg_surf, (5 + left, top + 36))
            if DB.constants.value('crit'):
                FONT['text'].blit('Crit', bg_surf, (width//2 + 5 + left, top + 20))
            else:
                FONT['text'].blit('AS', bg_surf, (width//2 + 5 + left, top + 20))
            FONT['text'].blit('Avo', bg_surf, (width//2 + 5 + left, top + 36))

            damage = combat_calcs.damage(self.unit, self.item)
            accuracy = combat_calcs.accuracy(self.unit, self.item)
            crit = combat_calcs.crit_accuracy(self.unit, self.item)
            if crit is None:
                crit = '--'
            avoid = combat_calcs.avoid(self.unit, self.item)
            attack_speed = combat_calcs.attack_speed(self.unit, self.item)

            FONT['text-blue'].blit_right(str(damage), bg_surf, (left + width//2 - 3, top + 20))
            FONT['text-blue'].blit_right(str(accuracy), bg_surf, (left + width//2 - 3, top + 36))
            if DB.constants.value('crit'):
                FONT['text-blue'].blit_right(str(crit), bg_surf, (left + width - 10, top + 20))
            else:
                FONT['text-blue'].blit_right(str(attack_speed), bg_surf, (left + width - 10, top + 20))
            FONT['text-blue'].blit_right(str(avoid), bg_surf, (left + width - 10, top + 36))

            weapon_type = item_system.weapon_type(self.unit, self.item)
            if weapon_type:
                icons.draw_weapon(bg_surf, weapon_type, (left + width//2 - 16//2 + affin_width//2 + 8, top + 4))
            else:
                FONT['text-blue'].blit('--', bg_surf, (left + width//2 - 16//2 + affin_width + 8, top + 4))

        else:
            if self.item.desc:
                desc = self.item.desc
            elif not available:
                desc = "Cannot wield."
            else:
                desc = ""

            desc = desc.replace('{br}', '\n')
            lines = text_funcs.line_wrap('text', desc, width - 8)
            new_lines = []
            for line in lines:
                new_lines += line.split('\n')
            lines = fix_tags(new_lines)
            for idx, line in enumerate(lines):
                render_text(bg_surf, ['text'], [line], [None], (4 + 2, 8 + idx * 16))

        return bg_surf

    def draw(self, surf):
        if not self.item:
            return surf
        if not self.surf:
            self.surf = self.create_surf()

        cursor_left = False
        if game.cursor.position[0] > TILEX // 2 + game.camera.get_x():
            topleft = (WINWIDTH - 8 - self.surf.get_width(), WINHEIGHT - 8 - self.surf.get_height())
        else:
            cursor_left = True
            topleft = (8, WINHEIGHT - 8 - self.surf.get_height())

        portrait, _ = icons.get_portrait(self.unit)
        if portrait:
            if cursor_left:
                portrait = engine.flip_horiz(portrait)
            surf.blit(portrait, (topleft[0] + 2, topleft[1] - 76))

        surf.blit(self.surf, topleft)
        return surf
