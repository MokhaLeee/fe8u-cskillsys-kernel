
import logging
from typing import List, Tuple

from app.constants import WINHEIGHT, WINWIDTH
from app.data.database.database import DB
from app.data.resources.resources import RESOURCES
from app.engine import (background, combat_calcs, engine, equations, gui,
                        help_menu, icons, image_mods, item_funcs, item_system,
                        skill_system, text_funcs, unit_funcs)
from app.engine.fluid_scroll import FluidScroll
from app.engine.game_menus.icon_options import BasicItemOption, ItemOptionModes
from app.engine.game_state import game
from app.engine.graphics.ingame_ui.build_groove import build_groove
from app.engine.graphics.text.text_renderer import render_text, text_width
from app.engine.info_menu.info_graph import InfoGraph, info_states
from app.engine.info_menu.info_menu_portrait import InfoMenuPortrait
from app.engine.input_manager import get_input_manager
from app.engine.objects.unit import UnitObject
from app.engine.sound import get_sound_thread
from app.engine.sprites import SPRITES
from app.engine.state import State
from app.engine.text_evaluator import TextEvaluator
from app.utilities import utils
from app.utilities.enums import HAlignment


class InfoMenuState(State):
    name = 'info_menu'
    in_level = False
    show_map = False

    def create_background(self):
        panorama = RESOURCES.panoramas.get('info_menu_background')
        if not panorama:
            panorama = RESOURCES.panoramas.get('default_background')
        if panorama:
            self.bg = background.PanoramaBackground(panorama)
        else:
            self.bg = None

    def start(self):
        self.mouse_indicator = gui.MouseIndicator()
        self.create_background()

        # Unit to be displayed
        self.unit: UnitObject = game.memory.get('current_unit')
        self.scroll_units = game.memory.get('scroll_units')
        if self.scroll_units is None:
            self.scroll_units = [unit for unit in game.units if not unit.dead and unit.team == self.unit.team and unit.party == self.unit.party]
            if self.unit.position:
                self.scroll_units = [unit for unit in self.scroll_units if unit.position and game.board.in_vision(unit.position)]
        self.scroll_units = [unit for unit in self.scroll_units if 'Tile' not in unit.tags]
        game.memory['scroll_units'] = None

        self.state = game.memory.get('info_menu_state', info_states[0])
        if self.state == 'notes' and not (DB.constants.value('unit_notes') and self.unit.notes):
            self.state = 'personal_data'
        self.growth_flag = False

        self.fluid = FluidScroll(200, 1)

        self.build_arrows()

        self.logo = None
        self.switch_logo(self.state)

        self.info_graph = InfoGraph()
        self.info_flag = False
        self.info_graph.set_current_state(self.state)
        self.reset_surfs()

        # For transitions between states
        self.rescuer = None  # Keeps track of the rescuer if we are looking at the traveler
        self.next_unit = None
        self.next_state = None
        self.scroll_offset_x = 0
        self.scroll_offset_y = 0
        self.transition = None
        self.transition_counter = 0
        self.transparency = 0

        game.state.change('transition_in')
        return 'repeat'

    def reset_surfs(self):
        self.info_graph.clear()
        self.portrait_surf = None
        self.current_portrait = None

        self.personal_data_surf: engine.Surface = None
        self.growths_surf: engine.Surface = None
        self.wexp_surf: engine.Surface = None
        self.equipment_surf: engine.Surface = None
        self.support_surf: engine.Surface = None
        self.skill_surf: engine.Surface = None
        self.class_skill_surf: engine.Surface = None
        self.fatigue_surf: engine.Surface = None
        self.notes_surf: engine.Surface = None

    def build_arrows(self):
        self.left_arrow = gui.ScrollArrow('left', (103, 3))
        self.right_arrow = gui.ScrollArrow('right', (217, 3), 0.5)

    def switch_logo(self, name):
        if name == 'personal_data':
            image = SPRITES.get('info_title_personal_data')
        elif name == 'equipment':
            image = SPRITES.get('info_title_items')
        elif name == 'support_skills':
            image = SPRITES.get('info_title_weapon')
        elif name == 'notes':
            image = SPRITES.get('info_title_notes')
        else:
            return
        if self.logo:
            self.logo.switch_image(image)
        else:
            self.logo = gui.Logo(image, (164, 10))

    def back(self):
        get_sound_thread().play_sfx('Select 4')
        game.memory['info_menu_state'] = self.state
        game.memory['current_unit'] = self.unit
        if self.unit.position and not game.is_roam():
            # Move camera to the new character unless it's a free roam, in which case we just stay on the free roamer
            game.cursor.set_pos(self.unit.position)
        game.state.change('transition_pop')

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.handle_mouse()
        if self.info_flag:
            if event == 'INFO' or event == 'BACK':
                get_sound_thread().play_sfx('Info Out')
                self.info_graph.set_transition_out()
                self.info_flag = False
                return

            if 'RIGHT' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.info_graph.move_right()
            elif 'LEFT' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.info_graph.move_left()
            elif 'UP' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.info_graph.move_up()
            elif 'DOWN' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.info_graph.move_down()

        elif not self.transition:  # Only takes input when not transitioning

            if event == 'INFO':
                get_sound_thread().play_sfx('Info In')
                self.info_graph.set_transition_in()
                self.info_flag = True
                return
            elif event == 'AUX':
                if self.state == 'personal_data' and self.unit.team == 'player' and DB.constants.value('growth_info'):
                    get_sound_thread().play_sfx('Select 3')
                    self.growth_flag = not self.growth_flag
                    if self.growth_flag:
                        self.info_graph.set_current_state('growths')
                    else:
                        self.info_graph.set_current_state('personal_data')
            elif event == 'BACK':
                if self.rescuer:
                    self.move_up()
                else:
                    self.back()
                    return
            elif event == 'SELECT':
                mouse_position = get_input_manager().get_mouse_position()
                if mouse_position:
                    mouse_x, mouse_y = mouse_position
                    if mouse_x <= 16:
                        self.move_left()
                    elif mouse_x >= WINWIDTH - 16:
                        self.move_right()
                    elif mouse_y <= 16:
                        self.move_up()
                    elif mouse_y >= WINHEIGHT - 16:
                        self.move_down()
                if not self.transition:  # Some of the above move commands could cause transition
                    if self.unit.traveler:
                        self.move_traveler()

            if 'RIGHT' in directions:
                self.move_right()
            elif 'LEFT' in directions:
                self.move_left()
            elif 'DOWN' in directions:
                self.move_down()
            elif 'UP' in directions:
                self.move_up()

    def move_left(self):
        if len(info_states) > 1:
            get_sound_thread().play_sfx('Status_Page_Change')
            index = info_states.index(self.state)
            new_index = (index - 1) % len(info_states)
            self.next_state = info_states[new_index]
            if self.next_state == 'notes' and not (DB.constants.value('unit_notes') and self.unit.notes):
                new_index = (new_index - 1) % len(info_states)
                self.next_state = info_states[new_index]
            self.transition = 'LEFT'
            self.left_arrow.pulse()
            self.switch_logo(self.next_state)

    def move_right(self):
        if len(info_states) > 1:
            get_sound_thread().play_sfx('Status_Page_Change')
            index = info_states.index(self.state)
            new_index = (index + 1) % len(info_states)
            self.next_state = info_states[new_index]
            if self.next_state == 'notes' and not (DB.constants.value('unit_notes') and self.unit.notes):
                new_index = (new_index + 1) % len(info_states)
                self.next_state = info_states[new_index]
            self.transition = 'RIGHT'
            self.right_arrow.pulse()
            self.switch_logo(self.next_state)

    def move_down(self):
        get_sound_thread().play_sfx('Status_Character')
        if self.rescuer:
            new_index = self.scroll_units.index(self.rescuer)
            self.rescuer = None
        elif len(self.scroll_units) > 1:
            index = self.scroll_units.index(self.unit)
            new_index = (index + 1) % len(self.scroll_units)
        else:
            return
        self.next_unit = self.scroll_units[new_index]
        if self.state == 'notes' and not (DB.constants.value('unit_notes') and self.next_unit.notes):
            self.state = 'personal_data'
            self.switch_logo('personal_data')
        self.transition = 'DOWN'

    def move_up(self):
        get_sound_thread().play_sfx('Status_Character')
        if self.rescuer:
            new_index = self.scroll_units.index(self.rescuer)
            self.rescuer = None
        elif len(self.scroll_units) > 1:
            index = self.scroll_units.index(self.unit)
            new_index = (index - 1) % len(self.scroll_units)
        else:
            return
        self.next_unit = self.scroll_units[new_index]
        if self.state == 'notes' and not (DB.constants.value('unit_notes') and self.next_unit.notes):
            self.state = 'personal_data'
            self.switch_logo('personal_data')
        self.transition = 'UP'

    def move_traveler(self):
        get_sound_thread().play_sfx('Status_Character')
        self.rescuer = self.unit
        self.next_unit = game.get_unit(self.unit.traveler)
        if self.state == 'notes' and not (DB.constants.value('unit_notes') and self.next_unit.notes):
            self.state = 'personal_data'
            self.switch_logo('personal_data')
        self.transition = 'DOWN'

    def handle_mouse(self):
        mouse_position = get_input_manager().get_mouse_position()
        if not mouse_position:
            return
        if self.info_flag:
            self.info_graph.handle_mouse(mouse_position)

    def update(self):
        # Up and Down
        if self.next_unit:
            self.transition_counter += 1
            # Transition in
            if self.next_unit == self.unit:
                if self.transition_counter == 1:
                    self.transparency = .75
                    self.scroll_offset_y = -80 if self.transition == 'DOWN' else 80
                elif self.transition_counter == 2:
                    self.transparency = .6
                    self.scroll_offset_y = -32 if self.transition == 'DOWN' else 32
                elif self.transition_counter == 3:
                    self.transparency = .48
                    self.scroll_offset_y = -16 if self.transition == 'DOWN' else 16
                elif self.transition_counter == 4:
                    self.transparency = .15
                    self.scroll_offset_y = -4 if self.transition == 'DOWN' else 4
                elif self.transition_counter == 5:
                    self.scroll_offset_y = 0
                else:
                    self.transition = None
                    self.transparency = 0
                    self.next_unit = None
                    self.transition_counter = 0
            # Transition out
            else:
                if self.transition_counter == 1:
                    self.transparency = .15
                elif self.transition_counter == 2:
                    self.transparency = .48
                elif self.transition_counter == 3:
                    self.transparency = .6
                    self.scroll_offset_y = 8 if self.transition == 'DOWN' else -8
                elif self.transition_counter == 4:
                    self.transparency = .75
                    self.scroll_offset_y = 16 if self.transition == 'DOWN' else -16
                elif self.transition_counter < 8: # (5, 6, 7, 8):  # Pause for a bit
                    self.transparency = 1.
                    self.scroll_offset_y = 160 if self.transition == 'DOWN' else -160
                else:
                    self.unit = self.next_unit  # Now transition in
                    self.reset_surfs()
                    self.transition_counter = 0

        # Left and Right
        elif self.next_state is not None:
            self.transition_counter += 1
            # Transition in
            if self.next_state == self.state:
                idxs = (104, 72, 56, 40, 24, 8)
                counter = self.transition_counter - 1
                if 0 <= counter < len(idxs):
                    self.scroll_offset_x = idxs[counter] if self.transition == 'RIGHT' else -idxs[counter]
                else:
                    self.transition = None
                    self.scroll_offset_x = 0
                    self.next_state = None
                    self.transition_counter = 0
            else:
                idxs = (-32, -56, -80, -96, -112)
                counter = self.transition_counter - 1
                if 0 <= counter < len(idxs):
                    self.scroll_offset_x = idxs[counter] if self.transition == 'RIGHT' else -idxs[counter]
                else:
                    self.scroll_offset_x = -140 if self.transition == 'RIGHT' else 140
                    self.state = self.next_state
                    self.info_graph.set_current_state(self.state)
                    self.transition_counter = 0

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        else:
            # info menu shouldn't be transparent
            surf.blit(SPRITES.get('bg_black'), (0, 0))

        # Image flashy thing at the top of the InfoMenu
        num_frames = 8
        # 8 frames long, 8 different frames
        blend_perc = abs(num_frames - ((engine.get_time()/134) % (num_frames * 2))) / float(num_frames)
        sprite = SPRITES.get('info_menu_flash')
        im = image_mods.make_translucent_blend(sprite, 128. * blend_perc)
        surf.blit(im, (98, 0), None, engine.BLEND_RGB_ADD)

        self.draw_portrait(surf)
        self.draw_slide(surf)

        if self.info_graph.current_bb:
            self.info_graph.draw(surf)

        if not self.transition:
            self.mouse_indicator.draw(surf)

        return surf

    def draw_portrait(self, surf):
        # Only create if we don't have one in memory
        if not self.portrait_surf:
            self.portrait_surf = self.create_portrait_section()
        portrait_surf = self.portrait_surf.copy()

        # If no portrait for this unit, either create one or default to class card using icons.get_portrait
        if not self.current_portrait:
            portrait = RESOURCES.portraits.get(self.unit.portrait_nid)
            if portrait:
                self.current_portrait = InfoMenuPortrait(portrait, DB.constants.value('info_menu_blink'))
            else:
                im, offset = icons.get_portrait(self.unit)
        # We do have a portrait, so update...
        if self.current_portrait:
            self.current_portrait.update()
            im = self.current_portrait.create_image()
            offset = self.current_portrait.portrait.info_offset
        # Draw portrait onto the portrait surf
        if im:
            x_pos = (im.get_width() - 80)//2
            im_surf = engine.subsurface(im, (x_pos, offset, 80, 72))
            portrait_surf.blit(im_surf, (8, 8))

        # Stick it on the surface
        if self.transparency:
            im = image_mods.make_translucent(portrait_surf, self.transparency)
            surf.blit(im, (0, self.scroll_offset_y))
        else:
            surf.blit(portrait_surf, (0, self.scroll_offset_y))

        # Blit the unit's active/focus map sprite
        if not self.transparency:
            active_sprite = self.unit.sprite.create_image('active')
            x_pos = 81 - active_sprite.get_width()//2
            y_pos = WINHEIGHT - 61
            surf.blit(active_sprite, (x_pos, y_pos + self.scroll_offset_y))

    def growth_colors(self, value):
        color = 'yellow'
        if value >= 0 and value <= 20:
            color = 'red-orange'
        elif value > 20 and value <= 30:
            color = 'light-red'
        elif value > 30 and value <= 40:
            color = 'pink-orange'
        elif value > 40 and value <= 50:
            color = 'light-orange'
        elif value > 50 and value <= 60:
            color = 'corn-yellow'
        elif value > 60 and value <= 70:
            color = 'light-green'
        elif value > 70 and value <= 80:
            color = 'olive-green'
        elif value > 80 and value <= 90:
            color = 'soft-green'
        else:  # > 90
            color = 'yellow-green'
        return color

    def create_portrait_section(self):
        surf = engine.create_surface((96, WINHEIGHT), transparent=True)
        surf.blit(SPRITES.get('info_unit'), (8, 122))

        render_text(surf, ['text'], [self.unit.name], ['white'], (48, 80), HAlignment.CENTER)
        self.info_graph.register((24, 80, 52, 24), self.unit.desc, 'all')
        class_obj = DB.classes.get(self.unit.klass)
        render_text(surf, ['text'], [class_obj.name], ['white'], (8, 104))
        self.info_graph.register((8, 104, 72, 16), class_obj.desc, 'all')
        render_text(surf, ['text'], [str(self.unit.level)], ['blue'], (39, 120), HAlignment.RIGHT)
        self.info_graph.register((8, 120, 30, 16), 'Level_desc', 'all')
        render_text(surf, ['text'], [str(self.unit.exp)], ['blue'], (63, 120), HAlignment.RIGHT)
        self.info_graph.register((38, 120, 30, 16), 'Exp_desc', 'all')
        render_text(surf, ['text'], [str(self.unit.get_hp())], ['blue'], (39, 136), HAlignment.RIGHT)
        self.info_graph.register((8, 136, 72, 16), 'HP_desc', 'all')
        max_hp = equations.parser.hitpoints(self.unit)
        render_text(surf, ['text'], [str(max_hp)], ['blue'], (63, 136), HAlignment.RIGHT)
        # Blit the white status platform
        surf.blit(SPRITES.get('status_platform'), (66, 131))
        # Blit affinity
        affinity = DB.affinities.get(self.unit.affinity)
        if affinity:
            icons.draw_item(surf, affinity, (78, 81))
            self.info_graph.register((76, 80, 16, 16), affinity.desc, 'all')
        return surf

    def draw_top_arrows(self, surf):
        self.left_arrow.draw(surf)
        self.right_arrow.draw(surf)

    def draw_slide(self, surf):
        top_surf = engine.create_surface((WINWIDTH, WINHEIGHT), transparent=True)
        main_surf = engine.copy_surface(top_surf)

        # Blit title of menu
        top_surf.blit(SPRITES.get('info_title_background'), (112, 8))
        if self.logo:
            self.logo.update()
            self.logo.draw(top_surf)
        # Blit page numbers
        if DB.constants.value('unit_notes') and self.unit.notes:
            num_states = len(info_states)
        else:
            num_states = len(info_states) - 1
        page = str(info_states.index(self.state) + 1) + '/' + str(num_states)
        render_text(top_surf, ['small'], [page], [], (235, 12), HAlignment.RIGHT)

        if num_states > 1:
            self.draw_top_arrows(top_surf)

        if self.state == 'personal_data':
            if self.growth_flag:
                if not self.growths_surf:
                    self.growths_surf = self.create_personal_data_surf(growths=True)
                self.draw_growths_surf(main_surf)
            else:
                if not self.personal_data_surf:
                    self.personal_data_surf = self.create_personal_data_surf()
                self.draw_personal_data_surf(main_surf)
            if not self.class_skill_surf:
                self.class_skill_surf = self.create_class_skill_surf()
            self.draw_class_skill_surf(main_surf)
            if DB.constants.value('fatigue') and self.unit.team == 'player' and \
                    game.game_vars.get('_fatigue'):
                if not self.fatigue_surf:
                    self.fatigue_surf = self.create_fatigue_surf()
                self.draw_fatigue_surf(main_surf)

        elif self.state == 'equipment':
            if not self.equipment_surf:
                self.equipment_surf = self.create_equipment_surf()
            self.draw_equipment_surf(main_surf)

        elif self.state == 'support_skills':
            main_surf.blit(SPRITES.get('status_logo'), (100, WINHEIGHT - 42))
            if not self.skill_surf:
                self.skill_surf = self.create_skill_surf()
            self.draw_skill_surf(main_surf)
            if not self.wexp_surf:
                self.wexp_surf = self.create_wexp_surf()
            self.draw_wexp_surf(main_surf)
            if not self.support_surf:
                self.support_surf = self.create_support_surf()
            self.draw_support_surf(main_surf)

        elif self.state == 'notes':
            if not self.notes_surf:
                self.notes_surf = self.create_notes_surf()
            self.draw_notes_surf(main_surf)

        # Now put it in the right place
        offset_x = max(96, 96 - self.scroll_offset_x)
        main_surf = engine.subsurface(main_surf, (offset_x, 0, main_surf.get_width() - offset_x, WINHEIGHT))
        surf.blit(main_surf, (max(96, 96 + self.scroll_offset_x), self.scroll_offset_y))
        if self.transparency:
            top_surf = image_mods.make_translucent(top_surf, self.transparency)
        surf.blit(top_surf, (0, self.scroll_offset_y))

    def create_personal_data_surf(self, growths=False):
        if growths:
            state = 'growths'
        else:
            state = 'personal_data'

        menu_size = WINWIDTH - 96, WINHEIGHT
        surf = engine.create_surface(menu_size, transparent=True)

        left_stats = [stat.nid for stat in DB.stats if stat.position == 'left']
        if len(left_stats) >= 7:
            self._extra_stat_row = True
            # If we have 7 or more left stats, use 7 rows
            right_stats = left_stats[7:]
        else:  # Otherwise, just use the 6 rows
            self._extra_stat_row = False
            right_stats = left_stats[6:]
        right_stats += [stat.nid for stat in DB.stats if stat.position == 'right']
        # Make sure we only display up to 6 or 7 on each
        if self._extra_stat_row:
            left_stats = left_stats[:7]
            right_stats = right_stats[:7]
        else:
            left_stats = left_stats[:6]
            right_stats = right_stats[:6]

        for idx, stat_nid in enumerate(left_stats):
            curr_stat = DB.stats.get(stat_nid)
            # Value
            if growths:
                icons.draw_growth(surf, stat_nid, self.unit, (47, 16 * idx + 24))
            else:
                highest_stat = curr_stat.maximum
                max_stat = self.unit.get_stat_cap(stat_nid)
                if max_stat > 0:
                    total_length = int(max_stat / highest_stat * 42)
                    frac = utils.clamp(self.unit.stats.get(stat_nid) / max_stat, 0, 1)
                    build_groove(surf, (27, 16 * idx + 32), total_length, frac)
                icons.draw_stat(surf, stat_nid, self.unit, (47, 16 * idx + 24))

            # Name
            name = curr_stat.name
            color = 'yellow'
            if DB.stats.get(stat_nid).growth_colors and self.unit.team == 'player':
                color = self.growth_colors(unit_funcs.growth_rate(self.unit, stat_nid))
            render_text(surf, ['text'], [name], [color], (8, 16 * idx + 24))
            base_value = self.unit.stats.get(stat_nid, 0)
            subtle_stat_bonus = self.unit.subtle_stat_bonus(stat_nid)
            base_value += subtle_stat_bonus
            contribution = self.unit.stat_contribution(stat_nid)
            contribution['Base Value'] = base_value
            desc_text = curr_stat.desc
            help_box = help_menu.StatDialog(desc_text or ('%s_desc' % stat_nid), contribution)
            self.info_graph.register((96 + 8, 16 * idx + 24, 64, 16), help_box, state, first=(idx == 0))

        for idx, stat_nid in enumerate(right_stats):
            curr_stat = DB.stats.get(stat_nid)
            if growths:
                icons.draw_growth(surf, stat_nid, self.unit, (111, 16 * idx + 24))
            else:
                icons.draw_stat(surf, stat_nid, self.unit, (111, 16 * idx + 24))

            # Name
            name = curr_stat.name
            color = 'yellow'
            if DB.stats.get(stat_nid).growth_colors and self.unit.team == 'player':
                color = self.growth_colors(unit_funcs.growth_rate(self.unit, stat_nid))
            render_text(surf, ['text'], [name], [color], (72, 16 * idx + 24))
            base_value = self.unit.stats.get(stat_nid, 0)
            contribution = self.unit.stat_contribution(stat_nid)
            contribution['Base Value'] = base_value
            desc_text = curr_stat.desc
            help_box = help_menu.StatDialog(desc_text or ('%s_desc' % stat_nid), contribution)
            self.info_graph.register((96 + 72, 16 * idx + 24, 64, 16), help_box, state)

        other_stats = ['RAT']
        if DB.constants.value('talk_display'):
            other_stats.insert(0, 'TALK')
        if DB.constants.value('pairup') and DB.constants.value('attack_stance_only'):
            pass
        else:
            other_stats.insert(0, 'AID')
            other_stats.insert(0, 'TRV')
        if self.unit.get_max_mana() > 0:
            other_stats.insert(0, 'MANA')
        if DB.constants.value('pairup') and not DB.constants.value('attack_stance_only'):
            other_stats.insert(2, 'GAUGE')

        other_stats = other_stats[:6 - len(right_stats)]

        for idx, stat in enumerate(other_stats):
            true_idx = idx + len(right_stats)

            if stat == 'TRV':
                if self.unit.traveler:
                    trav = game.get_unit(self.unit.traveler)
                    render_text(surf, ['text'], [trav.name], ['blue'], (96, 16 * true_idx + 24))
                else:
                    render_text(surf, ['text'], ['--'], ['blue'], (96, 16 * true_idx + 24))
                render_text(surf, ['text'], [text_funcs.translate('Trv')], ['yellow'], (72, 16 * true_idx + 24))
                self.info_graph.register((96 + 72, 16 * true_idx + 24, 64, 16), 'Trv_desc', state)

            elif stat == 'AID':
                if growths:
                    icons.draw_growth(surf, 'HP', self.unit, (111, 16 * true_idx + 24))
                    color = 'yellow'
                    if DB.stats.get('HP').growth_colors and self.unit.team == 'player':
                        color = self.growth_colors(unit_funcs.growth_rate(self.unit, 'HP'))
                    render_text(surf, ['text'], [text_funcs.translate('HP')], [color], (72, 16 * true_idx + 24))
                    self.info_graph.register((96 + 72, 16 * true_idx + 24, 64, 16), 'HP_desc', state)
                else:
                    aid = equations.parser.rescue_aid(self.unit)
                    render_text(surf, ['text'], [str(aid)], ['blue'], (111, 16 * true_idx + 24), HAlignment.RIGHT)

                    # Mount Symbols
                    if 'Dragon' in self.unit.tags:
                        aid_surf = engine.subsurface(SPRITES.get('aid_icons'), (0, 48, 16, 16))
                    elif 'Flying' in self.unit.tags:
                        aid_surf = engine.subsurface(SPRITES.get('aid_icons'), (0, 32, 16, 16))
                    elif 'Mounted' in self.unit.tags:
                        aid_surf = engine.subsurface(SPRITES.get('aid_icons'), (0, 16, 16, 16))
                    else:
                        aid_surf = engine.subsurface(SPRITES.get('aid_icons'), (0, 0, 16, 16))
                    surf.blit(aid_surf, (112, 16 * true_idx + 24))
                    render_text(surf, ['text'], [text_funcs.translate('Aid')], ['yellow'], (72, 16 * true_idx + 24))
                    self.info_graph.register((96 + 72, 16 * true_idx + 24, 64, 16), 'Aid_desc', state)

            elif stat == 'RAT':
                rat = str(equations.parser.rating(self.unit))
                render_text(surf, ['text'], [rat], ['blue'], (111, 16 * true_idx + 24), HAlignment.RIGHT)
                render_text(surf, ['text'], [text_funcs.translate('Rat')], ['yellow'], (72, 16 * true_idx + 24))
                self.info_graph.register((96 + 72, 16 * true_idx + 24, 64, 16), 'Rating_desc', state)

            elif stat == 'MANA':
                mana = str(self.unit.current_mana)
                render_text(surf, ['text'], [mana], ['blue'], (111, 16 * true_idx + 24), HAlignment.RIGHT)
                render_text(surf, ['text'], [text_funcs.translate('MANA')], ['yellow'], (72, 16 * true_idx + 24))
                self.info_graph.register((96 + 72, 16 * true_idx + 24, 64, 16), 'MANA_desc', state)

            elif stat == 'GAUGE':
                gge = str(self.unit.get_guard_gauge()) + '/' + str(self.unit.get_max_guard_gauge())
                render_text(surf, ['text'], [gge], ['blue'], (111, 16 * true_idx + 24), HAlignment.RIGHT)
                render_text(surf, ['text'], [text_funcs.translate('GAUGE')], ['yellow'], (72, 16 * true_idx + 24))
                self.info_graph.register((96 + 72, 16 * true_idx + 24, 64, 16), 'GAUGE_desc', state)

            elif stat == 'TALK':
                if (len([talk for talk in game.talk_options if talk[0] == self.unit.nid]) != 0):
                    talkee = [talk for talk in game.talk_options if talk[0] == self.unit.nid][0][1]
                    render_text(surf, ['text'], [game.get_unit(talkee).name], ['blue'], (96, 16 * true_idx + 24))
                else:
                    render_text(surf, ['text'], ['--'], ['blue'], (98, 16 * true_idx + 24))
                render_text(surf, ['text'], [text_funcs.translate('Talk')], ['yellow'], (72, 16 * true_idx + 24))
                self.info_graph.register((96 + 72, 16 * true_idx + 24, 64, 16), 'Talk_desc', state)

            if DB.constants.value('lead'):
                render_text(surf, ['text'], [text_funcs.translate('Lead')], ['yellow'], (72, 120))
                self.info_graph.register((96 + 72, 120, 64, 16), 'Lead_desc', state)

                if growths:
                    icons.draw_growth(surf, 'LEAD', self.unit, (111, 120))
                else:
                    icons.draw_stat(surf, 'LEAD', self.unit, (111, 120))
                    lead_surf = engine.subsurface(SPRITES.get('lead_star'), (0, 16, 16, 16))
                    surf.blit(lead_surf, (111, 120))

        return surf

    def draw_personal_data_surf(self, surf):
        surf.blit(self.personal_data_surf, (96, 0))

    def draw_growths_surf(self, surf):
        surf.blit(self.growths_surf, (96, 0))

    def create_wexp_surf(self):
        wexp_to_draw: List[Tuple[str, int]] = []
        for weapon, wexp in self.unit.wexp.items():
            if wexp > 0 and weapon in unit_funcs.usable_wtypes(self.unit):
                wexp_to_draw.append((weapon, wexp))
        width = (WINWIDTH - 102) // 2
        height = 16 * 2 + 4

        surf = engine.create_surface((WINWIDTH - 96, height), transparent=True)
        if not wexp_to_draw:
            return surf
        counter = 0
        for y in range(0, 32, 16):
            for x in range(0, 2):
                weapon, value = wexp_to_draw[counter]
                weapon_rank = DB.weapon_ranks.get_rank_from_wexp(value)
                next_weapon_rank = DB.weapon_ranks.get_next_rank_from_wexp(value)
                if not weapon_rank:
                    perc = 0
                elif not next_weapon_rank:
                    perc = 1
                else:
                    perc = (value - weapon_rank.requirement) / (next_weapon_rank.requirement - weapon_rank.requirement)
                offset = 8 + x * width

                icons.draw_weapon(surf, weapon, (offset, 4 + y))

                # Build groove
                build_groove(surf, (offset + 18, 10 + y), width - 24, perc)
                # Add text
                pos = (offset + 7 + width//2, 4 + y)
                render_text(surf, ['text'], [weapon_rank.nid], ['blue'], pos, HAlignment.CENTER)
                self.info_graph.register((96 + pos[0] - width//2 - 8, 24 + pos[1], width, 16), "%s mastery level: %d" % (DB.weapons.get(weapon).name, value), 'support_skills', first=(counter==0))
                counter += 1
                if counter >= len(wexp_to_draw):
                    break
            if counter >= len(wexp_to_draw):
                break

        return surf

    def draw_wexp_surf(self, surf):
        surf.blit(self.wexp_surf, (96, 24))

    def create_equipment_surf(self):
        def create_item_option(idx, item):
            return BasicItemOption.from_item(idx, item, width=120, mode=ItemOptionModes.FULL_USES, text_color=item_system.text_color(None, item))

        surf = engine.create_surface((WINWIDTH - 96, WINHEIGHT), transparent=True)

        weapon = self.unit.get_weapon()
        accessory = self.unit.get_accessory()

        # Blit items
        for idx, item in enumerate(self.unit.nonaccessories):
            if item.multi_item and any(subitem is weapon for subitem in item.subitems):
                surf.blit(SPRITES.get('equipment_highlight'), (8, idx * 16 + 24 + 8))
                for subitem in item.subitems:
                    if subitem is weapon:
                        item_option = create_item_option(idx, subitem)
                        break
                else:  # Shouldn't happen
                    item_option = create_item_option(idx, item)
            else:
                if item is weapon:
                    surf.blit(SPRITES.get('equipment_highlight'), (8, idx * 16 + 24 + 8))
                item_option = create_item_option(idx, item)
            item_option.draw(surf, 8, idx * 16 + 24)
            self.info_graph.register((96 + 8, idx * 16 + 24, 120, 16), item_option.get_help_box(), 'equipment', first=(idx == 0))

        # Blit accessories
        for idx, item in enumerate(self.unit.accessories):
            aidx = item_funcs.get_num_items(self.unit) + idx
            y_pos = aidx * 16 + 24
            if item.multi_item and any(subitem is accessory for subitem in item.subitems):
                surf.blit(SPRITES.get('equipment_highlight'), (8, y_pos + 8))
                for subitem in item.subitems:
                    if subitem is accessory:
                        item_option = create_item_option(aidx, subitem)
                        break
                else:  # Shouldn't happen
                    item_option = create_item_option(aidx, item)
            else:
                if item is accessory:
                    surf.blit(SPRITES.get('equipment_highlight'), (8, y_pos + 8))
                item_option = create_item_option(aidx, item)
            item_option.draw(surf, 8, y_pos)
            first = (idx == 0 and not self.unit.nonaccessories)
            self.info_graph.register((96 + 8, y_pos, 120, 16), item_option.get_help_box(), 'equipment', first=first)

        # Battle stats
        battle_surf = SPRITES.get('battle_info')
        top, left = 104, 12
        surf.blit(battle_surf, (left, top))
        # Populate battle info
        surf.blit(SPRITES.get('equipment_logo'), (14, top + 4))
        render_text(surf, ['text'], [text_funcs.translate('Rng')], ['yellow'], (78, top))
        self.info_graph.register((96 + 78, top, 56, 16), 'Rng_desc', 'equipment')
        render_text(surf, ['text'], [text_funcs.translate('Atk')], ['yellow'], (22, top + 16))
        self.info_graph.register((96 + 14, top + 16, 64, 16), 'Atk_desc', 'equipment')
        render_text(surf, ['text'], [text_funcs.translate('Hit')], ['yellow'], (22, top + 32))
        self.info_graph.register((96 + 14, top + 32, 64, 16), 'Hit_desc', 'equipment')
        if DB.constants.value('crit'):
            render_text(surf, ['text'], [text_funcs.translate('Crit')], ['yellow'], (78, top + 16))
            self.info_graph.register((96 + 78, top + 16, 56, 16), 'Crit_desc', 'equipment')
        else:
            render_text(surf, ['text'], [text_funcs.translate('AS')], ['yellow'], (78, top + 16))
            self.info_graph.register((96 + 78, top + 16, 56, 16), 'AS_desc', 'equipment')
        render_text(surf, ['text'], [text_funcs.translate('Avoid')], ['yellow'], (78, top + 32))
        self.info_graph.register((96 + 78, top + 32, 56, 16), 'Avoid_desc', 'equipment')

        if weapon:
            rng = item_funcs.get_range_string(self.unit, weapon)
            dam = str(combat_calcs.damage(self.unit, weapon))
            acc = str(combat_calcs.accuracy(self.unit, weapon))
            crt = combat_calcs.crit_accuracy(self.unit, weapon)
            if crt is None:
                crt = '--'
            else:
                crt = str(crt)
        else:
            rng, dam, acc, crt = '--', '--', '--', '--'

        avo = str(combat_calcs.avoid(self.unit, weapon))
        attack_speed = str(combat_calcs.attack_speed(self.unit, weapon))
        render_text(surf, ['text'], [rng], ['blue'], (127, top), HAlignment.RIGHT)
        render_text(surf, ['text'], [dam], ['blue'], (71, top + 16), HAlignment.RIGHT)
        render_text(surf, ['text'], [acc], ['blue'], (71, top + 32), HAlignment.RIGHT)
        if DB.constants.value('crit'):
            render_text(surf, ['text'], [crt], ['blue'], (127, top + 16), HAlignment.RIGHT)
        else:
            render_text(surf, ['text'], [attack_speed], ['blue'], (127, top + 16), HAlignment.RIGHT)
        render_text(surf, ['text'], [avo], ['blue'], (127, top + 32), HAlignment.RIGHT)

        return surf

    def draw_equipment_surf(self, surf):
        surf.blit(self.equipment_surf, (96, 0))

    def create_skill_surf(self):
        surf = engine.create_surface((WINWIDTH - 96, 24), transparent=True)
        skills = [skill for skill in self.unit.skills if not (skill.class_skill or skill_system.hidden(skill, self.unit))]
        # stacked skills appear multiple times, but should be drawn only once
        skill_counter = {}
        unique_skills = []
        for skill in skills:
            if skill.nid not in skill_counter:
                skill_counter[skill.nid] = 1
                unique_skills.append(skill)
            else:
                skill_counter[skill.nid] += 1
        for idx, skill in enumerate(unique_skills[:6]):
            left_pos = idx * 24
            icons.draw_skill(surf, skill, (left_pos + 8, 4), compact=False, grey=skill_system.is_grey(skill, self.unit))
            if skill_counter[skill.nid] > 1:
                text = str(skill_counter[skill.nid])
                render_text(surf, ['small'], [text], ['white'], (left_pos + 20 - 4 * len(text), 6))
            if skill.data.get('total_charge'):
                charge = ' %d / %d' % (skill.data['charge'], skill.data['total_charge'])
            else:
                charge = ''
            self.info_graph.register((96 + left_pos + 8, WINHEIGHT - 28, 16, 16), help_menu.HelpDialog(skill.desc, name=skill.name + charge), 'support_skills')

        return surf

    def draw_skill_surf(self, surf):
        surf.blit(self.skill_surf, (96, WINHEIGHT - 32))

    def create_class_skill_surf(self):
        surf = engine.create_surface((WINWIDTH - 96, 24), transparent=True)
        class_skills = [skill for skill in self.unit.skills if skill.class_skill and not skill_system.hidden(skill, self.unit)]

        # stacked skills appear multiple times, but should be drawn only once
        skill_counter = {}
        unique_skills = list()
        for skill in class_skills:
            if skill.nid not in skill_counter:
                skill_counter[skill.nid] = 1
                unique_skills.append(skill)
            else:
                skill_counter[skill.nid] += 1
        for idx, skill in enumerate(unique_skills[:6]):
            left_pos = idx * 24
            icons.draw_skill(surf, skill, (left_pos + 8, 8), compact=False, grey=skill_system.is_grey(skill, self.unit))
            if skill_counter[skill.nid] > 1:
                text = str(skill_counter[skill.nid])
                render_text(surf, ['small'], [text], ['white'], (left_pos + 20 - 4 * len(text), 6))
            if skill.data.get('total_charge'):
                charge = ' %d / %d' % (skill.data['charge'], skill.data['total_charge'])
            else:
                charge = ''
            if self._extra_stat_row:
                self.info_graph.register((96 + left_pos + 8, WINHEIGHT - 22, 16, 16), help_menu.HelpDialog(skill.desc, name=skill.name + charge), 'personal_data')
                self.info_graph.register((96 + left_pos + 8, WINHEIGHT - 22, 16, 16), help_menu.HelpDialog(skill.desc, name=skill.name + charge), 'growths')
            else:
                self.info_graph.register((96 + left_pos + 8, WINHEIGHT - 32, 16, 16), help_menu.HelpDialog(skill.desc, name=skill.name + charge), 'personal_data')
                self.info_graph.register((96 + left_pos + 8, WINHEIGHT - 32, 16, 16), help_menu.HelpDialog(skill.desc, name=skill.name + charge), 'growths')

        return surf

    def draw_class_skill_surf(self, surf):
        if self._extra_stat_row:
            surf.blit(self.class_skill_surf, (96, WINHEIGHT - 26))
        else:
            surf.blit(self.class_skill_surf, (96, WINHEIGHT - 36))

    def create_support_surf(self):
        surf = engine.create_surface((WINWIDTH - 96, WINHEIGHT), transparent=True)
        width = (WINWIDTH - 102) // 2

        if game.game_vars.get('_supports'):
            pairs = game.supports.get_pairs(self.unit.nid)
            pairs = [pair for pair in pairs if pair.unlocked_ranks]
        else:
            pairs = []

        pairs = pairs[:6] # max six supports displayed

        top = self.wexp_surf.get_height() + 24
        for idx, pair in enumerate(pairs):
            x, y = (idx) % 2, idx // 2
            other_unit = None
            if pair.unit1 == self.unit.nid:
                other_unit = game.get_unit(pair.unit2)
            elif pair.unit2 == self.unit.nid:
                other_unit = game.get_unit(pair.unit1)
            if not other_unit:
                continue
            affinity = DB.affinities.get(other_unit.affinity)
            if affinity:
                icons.draw_item(surf, affinity, (x * width + 8, y * 16 + top))
                self.info_graph.register((96 + x * width + 8, y * 16 + top, WINWIDTH - 120, 16), affinity.desc, 'support_skills')
            render_text(surf, ['narrow'], [other_unit.name], [], (x * width + 22, y * 16 + top))
            highest_rank = pair.unlocked_ranks[-1]
            render_text(surf, ['text'], [highest_rank], ['yellow'], (x * width + surf.get_width()/2 - 2, y * 16 + top), HAlignment.RIGHT)
        return surf

    def draw_support_surf(self, surf):
        surf.blit(self.support_surf, (96, 0))

    def create_fatigue_surf(self):
        surf = engine.create_surface((WINWIDTH - 96, WINHEIGHT), transparent=True)
        max_fatigue = max(1, self.unit.get_max_fatigue())
        fatigue = self.unit.get_fatigue()
        build_groove(surf, (27, WINHEIGHT - 9), 88, utils.clamp(fatigue / max_fatigue, 0, 1))
        x_pos = 27 + 88 // 2
        text = str(fatigue) + '/' + str(max_fatigue)
        x_pos -= text_width('text', text)//2
        render_text(surf, ['text'], [text], ['blue'], (x_pos, WINHEIGHT - 17))
        if fatigue >= max_fatigue:
            render_text(surf, ['text'], [str(fatigue)], ['red'], (x_pos, WINHEIGHT - 17))
        render_text(surf, ['text'], [text_funcs.translate('Ftg')], ['yellow'], (8, WINHEIGHT - 17))

        return surf

    def draw_fatigue_surf(self, surf):
        surf.blit(self.fatigue_surf, (96, 0))

    def create_notes_surf(self):
        # Menu background
        menu_surf = engine.create_surface((WINWIDTH - 96, WINHEIGHT), transparent=True)

        text_parser = TextEvaluator(logging.getLogger(), game)
        my_notes = self.unit.notes

        if my_notes:
            total_height = 24
            help_offset = 0
            for idx, note in enumerate(my_notes):
                category = note[0]
                entries = note[1].split(',')
                render_text(menu_surf, ['text'], [category], ['blue'], (10, total_height))
                for entry in entries:
                    category_length = text_width('text', category)
                    left_pos = 64 if category_length <= 64 else (category_length + 8)
                    render_text(menu_surf, ['text'], [text_parser._evaluate_all(entry)], [], (left_pos, total_height))
                    total_height += 16
                self.info_graph.register((96, 16 * help_offset + 24, 64, 16), '%s_desc' % category, 'notes', first=(idx == 0))
                help_offset += len(entries)

        return menu_surf

    def draw_notes_surf(self, surf):
        surf.blit(self.notes_surf, (96, 0))
