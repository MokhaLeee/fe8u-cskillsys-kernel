from typing import List, Tuple

from app.constants import TILEHEIGHT, TILEWIDTH, WINHEIGHT, WINWIDTH
# from app.data.resources.resources import RESOURCES
from app.data.database.database import DB
from app.engine import action, background, banner, base_surf
from app.engine import config as cf
from app.engine import (convoy_funcs, engine, equations, gui, image_mods,
                        item_funcs, item_system, menus, text_funcs,
                        trade)
from app.engine.background import SpriteBackground
from app.engine.combat import interaction
from app.engine.fluid_scroll import FluidScroll
from app.engine.fonts import FONT
from app.engine.game_state import game
from app.engine.sound import get_sound_thread
from app.engine.sprites import SPRITES
from app.engine.state import MapState, State
from app.events import triggers


class PrepMainState(MapState):
    name = 'prep_main'
    bg = None
    menu = None

    def populate_options(self) -> Tuple[List[str], List[str], List[str]]:
        """return (options, ignore, events), which should all be the same size
        """
        # basic options
        options = ['Manage', 'Formation', 'Options', 'Save', 'Fight']
        if game.level_vars.get('_prep_pick'):
            options.insert(0, 'Pick Units')
        if cf.SETTINGS['debug']:
            options.insert(0, 'Debug')
        ignore = [False for option in options]
        # Don't manage units if there's nobody in the party!
        if not game.get_units_in_party():
            ignore[0] = True

        # initialize custom options and events
        events = [None for option in options]
        additional_options = game.game_vars.get('_prep_additional_options', [])
        additional_ignore = [not enabled for enabled in game.game_vars.get('_prep_options_enabled', [])]
        additional_events = game.game_vars.get('_prep_options_events', [])

        option_idx = options.index('Options')

        options = options[:option_idx] + additional_options + options[option_idx:]
        ignore = ignore[:option_idx] + additional_ignore + ignore[option_idx:]
        events = events[:option_idx] + additional_events + events[option_idx:]
        return options, ignore, events

    def start(self):
        prep_music = game.game_vars.get('_prep_music')
        if prep_music:
            get_sound_thread().fade_in(prep_music)
        game.cursor.hide()
        game.cursor.autocursor()
        game.boundary.hide()

        self.create_background()

        options, ignore, events_on_options = self.populate_options()
        self.events_on_option_select = events_on_options

        max_num_options = 8
        self.menu = menus.Choice(None, options, topleft='center')
        self.menu.set_limit(max_num_options)
        self.menu.set_ignore(ignore)

        # Force place any required units
        for unit in game.get_units_in_party():
            possible_position = game.get_next_formation_spot()
            if 'Required' in unit.tags and possible_position and not unit.position:
                action.ArriveOnMap(unit, possible_position).do()

        # Force reset all units
        action.do(action.ResetAll([unit for unit in game.units if not unit.dead]))

        self.fade_out = False
        self.last_update = 0

        game.events.trigger(triggers.OnPrepStart())

    def begin(self):
        prep_music = game.game_vars.get('_prep_music')
        if prep_music:
            get_sound_thread().fade_in(prep_music)

    def create_background(self):
        img = SPRITES.get('focus_fade').convert_alpha()
        self.bg = SpriteBackground(img)

    def leave(self):
        self.bg.fade_out()
        self.menu = None
        self.fade_out = True
        self.last_update = engine.get_time()

    def take_input(self, event):
        if self.fade_out:
            return
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.menu.handle_mouse()
        if 'DOWN' in directions:
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_down(first_push)
        elif 'UP' in directions:
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_up(first_push)

        elif event == 'SELECT':
            get_sound_thread().play_sfx('Select 1')
            selection = self.menu.get_current()
            if selection == 'Debug':
                game.state.change('debug')
            elif selection == 'Pick Units':
                game.memory['next_state'] = 'prep_pick_units'
                game.state.change('transition_to')
            elif selection == 'Manage':
                game.memory['next_state'] = 'prep_manage'
                game.state.change('transition_to')
            elif selection == 'Formation':
                self.bg.fade_out()
                game.memory['_prep_outline'] = self.bg
                game.state.change('prep_formation')
            elif selection == 'Options':
                game.memory['next_state'] = 'settings_menu'
                game.state.change('transition_to')
            elif selection == 'Save':
                game.memory['save_kind'] = 'prep'
                game.memory['next_state'] = 'in_chapter_save'
                game.state.change('transition_to')
            elif selection == 'Fight':
                if game.level_vars.get('_minimum_deployment', 0) > 0:
                    if sum(bool(unit.position) for unit in game.get_units_in_party()) \
                            >= min(game.level_vars['_minimum_deployment'], len(game.get_units_in_party())):
                        self.leave()
                    else:
                        get_sound_thread().play_sfx('Select 4')
                        if game.level_vars['_minimum_deployment'] == 1:
                            alert = banner.Custom("Must select at least 1 unit!")
                        else:
                            alert = banner.Custom("Must select at least %d units!" % game.level_vars['_minimum_deployment'])
                        game.alerts.append(alert)
                        game.state.change('alert')
                elif any(unit.position for unit in game.get_units_in_party()):
                    self.leave()
                else:
                    get_sound_thread().play_sfx('Select 4')
                    alert = banner.Custom("Must select at least one unit!")
                    game.alerts.append(alert)
                    game.state.change('alert')
            else:
                option_index = self.menu.get_current_index()
                if self.events_on_option_select[option_index]:
                    event_to_trigger = self.events_on_option_select[option_index]
                    valid_events = DB.events.get_by_nid_or_name(event_to_trigger, game.level.nid)
                    for event_prefab in valid_events:
                        game.events.trigger_specific_event(event_prefab.nid)

    def update(self):
        super().update()
        if self.fade_out:
            if engine.get_time() - self.last_update > 300:
                game.state.back()
        elif self.menu:
            self.menu.update()

    def draw(self, surf):
        surf = super().draw(surf)
        if not self.bg:
            self.create_background()
        if self.bg:
            self.bg.draw(surf)
        if self.menu:
            self.menu.draw(surf)
        return surf

class PrepPickUnitsState(State):
    name = 'prep_pick_units'

    def start(self):
        self.fluid = FluidScroll()
        player_units = game.get_units_in_party()
        stuck_units = [unit for unit in player_units if unit.position and not game.check_for_region(unit.position, 'formation')]
        unstuck_units = [unit for unit in player_units if unit not in stuck_units]

        self.units = stuck_units + sorted(unstuck_units, key=lambda unit: bool(unit.position), reverse=True)
        self.menu = menus.Table(None, self.units, (6, 2), (110, 24))
        self.menu.set_mode('position')

        self.bg = background.create_background('rune_background')
        game.memory['prep_bg'] = self.bg

        game.state.change('transition_in')
        return 'repeat'

    def order_party(self):
        '''Run on exiting the prep menu. Saves the order for future levels with the party.
        Saved order is unique to current party - will not effect other parties'''
        party = game.parties[game.current_party]
        party.party_prep_manage_sort_order = [u.nid for u in sorted(self.units, key=lambda unit: bool(unit.position), reverse=True)]

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.menu.handle_mouse()
        if 'DOWN' in directions:
            get_sound_thread().play_sfx('Select 5')
            self.menu.move_down(first_push)
        elif 'UP' in directions:
            get_sound_thread().play_sfx('Select 5')
            self.menu.move_up(first_push)
        elif 'LEFT' in directions:
            get_sound_thread().play_sfx('Select 5')
            self.menu.move_left(first_push)
        elif 'RIGHT' in directions:
            get_sound_thread().play_sfx('Select 5')
            self.menu.move_right(first_push)

        if event == 'SELECT':
            unit = self.menu.get_current()
            if not unit:
                get_sound_thread().play_sfx('Error')
                return
            if unit.position and not game.check_for_region(unit.position, 'formation'):
                get_sound_thread().play_sfx('Select 4')  # Locked/Lord character
            elif unit.position and 'Required' in unit.tags:
                get_sound_thread().play_sfx('Select 4')  # Required unit, can't be removed
            elif unit.position:
                get_sound_thread().play_sfx('Select 1')
                action.LeaveMap(unit).do()
            else:
                possible_position = game.get_next_formation_spot()
                is_fatigued = False
                if DB.constants.value('fatigue') and game.game_vars.get('_fatigue') == 1:
                    if unit.get_fatigue() >= unit.get_max_fatigue():
                        is_fatigued = True
                if 'Blacklist' in unit.tags:  # Blacklisted unit can't be added
                    is_fatigued = True
                num_slots = game.level_vars.get('_prep_slots')
                if num_slots is None:
                    num_slots = len(game.get_all_formation_spots())
                on_map = [unit for unit in game.units if unit.position and unit in game.get_units_in_party() 
                            and game.check_for_region(unit.position, 'formation')]
                if possible_position and not is_fatigued and len(on_map) < num_slots:
                    get_sound_thread().play_sfx('Select 1')
                    action.do(action.ArriveOnMap(unit, possible_position))
                    action.do(action.Reset(unit))
                elif is_fatigued:
                    get_sound_thread().play_sfx('Select 4')

        elif event == 'BACK':
            self.order_party()
            get_sound_thread().play_sfx('Select 4')
            game.state.change('transition_pop')

        elif event == 'INFO':
            get_sound_thread().play_sfx('Select 1')
            game.memory['scroll_units'] = game.get_units_in_party()
            game.memory['current_unit'] = self.menu.get_current()
            game.memory['next_state'] = 'info_menu'
            game.state.change('transition_to')

    def update(self):
        self.menu.update()

    def draw_pick_units_card(self, surf):
        bg_surf = base_surf.create_base_surf(132, 24, 'menu_bg_white')
        player_units = game.get_units_in_party()
        on_map = [unit for unit in game.units if unit.position and unit in player_units and game.check_for_region(unit.position, 'formation')]
        num_slots = game.level_vars.get('_prep_slots')
        if num_slots is None:
            num_slots = len(game.get_all_formation_spots())
        num_on_map = len(on_map)
        pick_s = ['Pick ', str(num_slots - num_on_map), ' units  ', str(num_on_map), '/', str(num_slots)]
        pick_f = ['text', 'text-blue', 'text', 'text-blue', 'text', 'text-blue']
        left_justify = 8
        for word, font in zip(pick_s, pick_f):
            FONT[font].blit(word, bg_surf, (left_justify, 4))
            left_justify += FONT[font].width(word)
        surf.blit(bg_surf, (110, 4))

    def draw_fatigue_card(self, surf):
        # Useful for telling at a glance which units are fatigued
        bg_surf = base_surf.create_base_surf(132, 24)
        topleft = (110, 128 + 4)
        unit = self.menu.get_current()
        if 'Blacklist' in unit.tags:
            text = text_funcs.translate('Away')
        elif unit.get_fatigue() >= unit.get_max_fatigue():
            text = text_funcs.translate('Fatigued')
        else:
            text = text_funcs.translate('Ready!')
        FONT['text'].blit_center(text, bg_surf, (66, 4))
        surf.blit(bg_surf, topleft)

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        if self.menu.get_current():
            menus.draw_unit_items(surf, (4, 44), self.menu.get_current(), include_top=True)

        self.draw_pick_units_card(surf)
        if DB.constants.value('fatigue') and game.game_vars.get('_fatigue'):
            self.draw_fatigue_card(surf)

        self.menu.draw(surf)
        return surf

def _handle_info():
    if game.cursor.get_hover():
        get_sound_thread().play_sfx('Select 1')
        game.memory['next_state'] = 'info_menu'
        game.memory['current_unit'] = game.cursor.get_hover()
        game.state.change('transition_to')
    else:
        get_sound_thread().play_sfx('Select 3')
        game.boundary.toggle_all_enemy_attacks()

class PrepFormationState(MapState):
    name = 'prep_formation'

    def start(self):
        game.highlight.show_formation(game.get_all_formation_spots())

    def begin(self):
        game.cursor.show()
        game.boundary.show()

    def take_input(self, event):
        game.cursor.take_input()

        if event == 'INFO':
            _handle_info()

        elif event == 'AUX':
            pass

        elif event == 'SELECT':
            cur_unit = game.cursor.get_hover()
            if cur_unit:
                if game.check_for_region(game.cursor.position, 'formation'):
                    get_sound_thread().play_sfx('Select 3')
                    game.state.change('prep_formation_select')
                else:
                    get_sound_thread().play_sfx('Select 2')
                    player_team_enemies = DB.teams.enemies
                    if cur_unit.team in player_team_enemies:
                        get_sound_thread().play_sfx('Select 3')
                        game.boundary.toggle_unit(cur_unit)
                    else:
                        get_sound_thread().play_sfx('Error')

        elif event == 'BACK':
            get_sound_thread().play_sfx('Select 1')
            if game.memory.get('_prep_outline'):
                game.memory['_prep_outline'].fade_in()
            game.state.back()

        elif event == 'START':
            get_sound_thread().play_sfx('Select 5')
            if DB.constants.value('initiative'):
                game.initiative.toggle_draw()
            else:
                game.state.change('minimap')

    def update(self):
        super().update()
        game.highlight.handle_hover()

    def draw(self, surf):
        surf = super().draw(surf)
        if game.memory.get('_prep_outline'):
            game.memory['_prep_outline'].draw(surf)
        return surf

    def finish(self):
        game.ui_view.remove_unit_display()
        game.cursor.hide()
        game.highlight.hide_formation()
        game.highlight.remove_highlights()

class PrepFormationSelectState(MapState):
    name = 'prep_formation_select'
    marker = SPRITES.get('menu_hand_rotated')
    marker_offset = [0, 1, 2, 3, 4, 5, 4, 3, 2, 1]

    def start(self):
        game.cursor.formation_show()
        self.last_update = engine.get_time()
        self.counter = 0
        self.unit = game.cursor.get_hover()

    def take_input(self, event):
        game.cursor.take_input()

        if event == 'SELECT':
            if game.check_for_region(game.cursor.position, 'formation'):
                get_sound_thread().play_sfx('FormationSelect')
                cur_unit = game.cursor.get_hover()
                if cur_unit and (cur_unit.team != 'player' or cur_unit is self.unit):
                    pass
                elif cur_unit:
                    action.PickUnitUp(cur_unit).do()
                    action.PickUnitUp(self.unit).do()
                    cur_unit.position, self.unit.position = self.unit.position, cur_unit.position
                    action.PutUnitDown(cur_unit).do()
                    action.PutUnitDown(self.unit).do()
                    action.UpdateFogOfWar(cur_unit).do()
                    action.UpdateFogOfWar(self.unit).do()
                else:
                    action.PickUnitUp(self.unit).do()
                    self.unit.position = game.cursor.position
                    action.PutUnitDown(self.unit).do()
                    action.UpdateFogOfWar(self.unit).do()
                game.state.back()
                game.ui_view.remove_unit_display()
                game.highlight.remove_highlights()
            else:
                get_sound_thread().play_sfx('Error')

        elif event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            game.state.back()

        elif event == 'AUX':
            game.cursor.set_pos(self.unit.position)

        elif event == 'INFO':
            _handle_info()

    def draw(self, surf):
        surf = super().draw(surf)

        # Draw static hand
        if self.unit:
            pos = self.unit.position
            x = (pos[0] - game.camera.get_x()) * TILEWIDTH + 2
            y = (pos[1] - game.camera.get_y() - 1) * TILEHEIGHT
            surf.blit(self.marker, (x, y))

        if game.check_for_region(game.cursor.position, 'formation'):
            pos = game.cursor.position
            while engine.get_time() - 50 > self.last_update:
                self.last_update += 50
                self.counter = self.counter % len(self.marker_offset)
            x = (pos[0] - game.camera.get_x()) * TILEWIDTH + 2
            y = (pos[1] - game.camera.get_y() - 1) * TILEHEIGHT + self.marker_offset[self.counter]
            surf.blit(self.marker, (x, y))

        return surf

def draw_funds(surf):
    # Draw R: Info display
    helper = engine.get_key_name(cf.SETTINGS['key_INFO']).upper()
    FONT['text-yellow'].blit(helper, surf, (123, 143))
    FONT['text'].blit(': Info', surf, (123 + FONT['text-blue'].width(helper), 143))
    # Draw Funds display
    surf.blit(SPRITES.get('funds_display'), (168, 137))
    money = str(game.get_money())
    FONT['text-blue'].blit_right(money, surf, (219, 141))

class PrepManageState(State):
    name = 'prep_manage'

    def start(self):
        self.fluid = FluidScroll()

        units = game.get_units_in_party()
        self.units = sorted(units, key=lambda unit: bool(unit.position), reverse=True)
        self.menu = menus.Table(None, self.units, (4, 3), (6, 0))
        if self.name.startswith('base'):
            self.menu.set_mode('unit')
        else:
            self.menu.set_mode('prep_manage')  # Gray out undeployed units in prep

        # Display
        self.quick_disp = self.create_quick_disp()

        if self.name.startswith('base') and game.memory['base_bg']:
            self.bg = game.memory['base_bg']
        else:
            self.bg = background.create_background('rune_background')
        game.memory['prep_bg'] = self.bg
        game.memory['manage_menu'] = self.menu

        game.state.change('transition_in')
        return 'repeat'

    def begin(self):
        # If come back from info menu
        unit = game.memory.get('current_unit')
        if unit and unit in self.units:
            idx = self.units.index(unit)
            self.menu.move_to(idx)
        game.memory['current_unit'] = None

    def create_quick_disp(self):
        sprite = SPRITES.get('buttons')
        buttons = [sprite.subsurface(0, 66, 14, 13), sprite.subsurface(0, 165, 33, 9)]
        font = FONT['text']
        commands = ['Manage', 'Optimize All']
        commands = [text_funcs.translate(c) for c in commands]
        size = (49 + max(font.width(c) for c in commands), 40)
        bg_surf = base_surf.create_base_surf(size[0], size[1], 'menu_bg_brown')
        bg_surf = image_mods.make_translucent(bg_surf, 0.1)
        bg_surf.blit(buttons[0], (20 - buttons[0].get_width()//2, 18 - buttons[0].get_height()))
        bg_surf.blit(buttons[1], (20 - buttons[1].get_width()//2, 32 - buttons[1].get_height()))
        for idx, command in enumerate(commands):
            font.blit(command, bg_surf, (38, idx * 16 + 3))
        return bg_surf

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.menu.handle_mouse()
        if 'DOWN' in directions:
            if self.menu.move_down(first_push):
                get_sound_thread().play_sfx('Select 5')
        elif 'UP' in directions:
            if self.menu.move_up(first_push):
                get_sound_thread().play_sfx('Select 5')
        elif 'LEFT' in directions:
            if self.menu.move_left(first_push):
                get_sound_thread().play_sfx('Select 5')
        elif 'RIGHT' in directions:
            if self.menu.move_right(first_push):
                get_sound_thread().play_sfx('Select 5')

        if event == 'SELECT':
            unit = self.menu.get_current()
            game.memory['current_unit'] = unit
            if self.name == 'base_manage':
                game.state.change('base_manage_select')
            else:
                game.state.change('prep_manage_select')
            get_sound_thread().play_sfx('Select 1')
        elif event == 'BACK':
            game.state.change('transition_pop')
            get_sound_thread().play_sfx('Select 4')
        elif event == 'INFO':
            get_sound_thread().play_sfx('Select 1')
            game.memory['scroll_units'] = game.get_units_in_party()
            game.memory['current_unit'] = self.menu.get_current()
            game.memory['next_state'] = 'info_menu'
            game.state.change('transition_to')
        elif event == 'START':
            get_sound_thread().play_sfx('Select 1')
            # convoy_funcs.optimize_all()
            game.state.change('optimize_all_choice')

    def update(self):
        self.menu.update()

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        self.menu.draw(surf)
        menus.draw_unit_items(surf, (6, 72), self.menu.get_current(), include_face=True, shimmer=2)
        surf.blit(self.quick_disp, (WINWIDTH//2 + 10, WINHEIGHT//2 + 9))
        draw_funds(surf)
        return surf

class OptimizeAllChoiceState(State):
    name = 'optimize_all_choice'
    transparent = True
    bg_surf = None

    def start(self):
        options = ['Yes', 'No']
        self.menu = menus.Choice(None, options, 'center', None)  # Clear background
        self.menu.set_horizontal(True)

        width = sum(option.width() + 8 for option in self.menu.options) + 16
        owidth = FONT['text'].width('Optimize All?') + 8
        self.bg_surf = base_surf.create_base_surf(max(width, owidth), 40)
        FONT['text'].blit('Optimize All?', self.bg_surf, (self.bg_surf.get_width()//2 - owidth//2 + 4, 4))

    def take_input(self, event):
        self.menu.handle_mouse()
        if event == 'RIGHT':
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_down()
        elif event == 'LEFT':
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_up()

        elif event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            game.state.back()

        elif event == 'SELECT':
            selection = self.menu.get_current()
            if selection == 'Yes':
                get_sound_thread().play_sfx('Select 1')
                convoy_funcs.optimize_all()
            else:
                get_sound_thread().play_sfx('Select 4')
            game.state.back()

    def update(self):
        self.menu.update()

    def draw(self, surf):
        if self.bg_surf:
            surf.blit(self.bg_surf, (WINWIDTH//2 - self.bg_surf.get_width()//2, WINHEIGHT//2 - 28))
        surf = self.menu.draw(surf)
        return surf

class PrepManageSelectState(State):
    name = 'prep_manage_select'

    def start(self):
        self.fluid = FluidScroll()

        self.bg = game.memory['prep_bg']
        self.menu = game.memory['manage_menu']
        self.unit = game.memory['current_unit']
        self.current_index = self.menu.current_index

        options = ['Trade', 'Restock', 'Give all', 'Optimize', 'Use', 'Market']
        # Replace Optimize with Repair when the repair shop is available
        if DB.constants.value('repair_shop'):
            options[3] = 'Repair'
        # Replace Use with Items when the convoy is available
        if game.game_vars.get('_convoy'):
            options[4] = 'Items'
        ignore = self.get_ignore()
        self.select_menu = menus.Table(self.unit, options, (3, 2), (120, 80))
        self.select_menu.set_ignore(ignore)

    def get_ignore(self) -> list:
        ignore = [False, True, True, True, True, True]
        if game.game_vars.get('_convoy'):
            # Turn Optimize and Items on
            ignore = [False, True, True, False, False, True]
            tradeable_items = item_funcs.get_all_tradeable_items(self.unit)
            if tradeable_items:
                ignore[2] = False  # Give all
            if any(convoy_funcs.can_restock(item) for item in tradeable_items):
                ignore[1] = False  # Restock
        else:  # Handle Use
            if any((item_funcs.can_be_used_in_base(self.unit, item) for item in self.unit.items)):
                ignore[4] = False
        if self.name == 'base_manage_select':
            if game.game_vars.get('_base_market') and game.market_items:
                ignore[5] = False
        else:
            if game.game_vars.get('_prep_market') and game.market_items:
                ignore[5] = False
        if DB.constants.value('repair_shop'):
            ignore[3] = not item_funcs.has_repair(self.unit)
        return ignore

    def begin(self):
        ignore = self.get_ignore()
        self.select_menu.set_ignore(ignore)
        self.menu.move_to(self.current_index)

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.select_menu.handle_mouse()
        if 'DOWN' in directions:
            if self.select_menu.move_down(first_push):
                get_sound_thread().play_sfx('Select 6')
        elif 'UP' in directions:
            if self.select_menu.move_up(first_push):
                get_sound_thread().play_sfx('Select 6')
        elif 'RIGHT' in directions:
            if self.select_menu.move_right(first_push):
                get_sound_thread().play_sfx('Select 6')
        elif 'LEFT' in directions:
            if self.select_menu.move_left(first_push):
                get_sound_thread().play_sfx('Select 6')

        if event == 'SELECT':
            get_sound_thread().play_sfx('Select 1')
            choice = self.select_menu.get_current()
            if choice == 'Trade':
                game.state.change('prep_trade_select')
            elif choice == 'Give all':
                tradeable_items = item_funcs.get_all_tradeable_items(self.unit)
                for item in tradeable_items:
                    convoy_funcs.store_item(item, self.unit)
            elif choice == 'Items':
                if self.name.startswith('base'):
                    game.memory['next_state'] = 'base_items'
                else:
                    game.memory['next_state'] = 'prep_items'
                game.state.change('transition_to')
            elif choice == 'Restock':
                game.state.change('prep_restock')
            elif choice == 'Optimize':
                convoy_funcs.optimize(self.unit)
            elif choice == 'Market':
                game.memory['next_state'] = 'prep_market'
                game.state.change('transition_to')
            elif choice == 'Repair':
                game.memory['next_state'] = 'repair_shop'
                game.state.change('transition_to')
            elif choice == 'Use':
                game.state.change('prep_use')

        elif event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            game.state.back()

    def update(self):
        self.menu.update()
        self.select_menu.update()

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        self.menu.draw(surf)
        menus.draw_unit_items(surf, (6, 72), self.unit, include_face=True, include_top=True, shimmer=2)
        self.select_menu.draw(surf)
        draw_funds(surf)
        return surf

class PrepTradeSelectState(State):
    name = 'prep_trade_select'

    def start(self):
        self.fluid = FluidScroll()

        self.menu = game.memory['manage_menu']
        self.bg = game.memory['prep_bg']
        self.unit = game.memory['current_unit']
        self.menu.set_fake_cursor(self.menu.current_index)

        if game.state.from_transition():
            game.state.change('transition_in')
            return 'repeat'

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.menu.handle_mouse()
        if 'DOWN' in directions:
            if self.menu.move_down(first_push):
                get_sound_thread().play_sfx('Select 5')
        elif 'UP' in directions:
            if self.menu.move_up(first_push):
                get_sound_thread().play_sfx('Select 5')
        elif 'LEFT' in directions:
            if self.menu.move_left(first_push):
                get_sound_thread().play_sfx('Select 5')
        elif 'RIGHT' in directions:
            if self.menu.move_right(first_push):
                get_sound_thread().play_sfx('Select 5')

        if event == 'SELECT':
            unit2 = self.menu.get_current()
            game.memory['unit1'] = self.unit
            game.memory['unit2'] = unit2
            game.memory['next_state'] = 'prep_trade'
            game.state.change('transition_to')

        elif event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            game.state.change('transition_pop')

        elif event == 'INFO':
            get_sound_thread().play_sfx('Select 1')
            game.memory['scroll_units'] = game.get_units_in_party()
            game.memory['current_unit'] = self.menu.get_current()
            game.memory['next_state'] = 'info_menu'
            game.state.change('transition_to')

    def update(self):
        self.menu.update()

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        menus.draw_unit_items(surf, (6, 72), self.unit, include_face=True, shimmer=2)
        menus.draw_unit_items(surf, (126, 72), self.menu.get_current(), include_face=True, right=False, shimmer=2)

        self.menu.draw(surf)

        return surf

    def finish(self):
        self.menu.set_fake_cursor(None)

class PrepItemsState(State):
    name = 'prep_items'

    trade_name_surf = SPRITES.get('trade_name')

    def start(self):
        self.fluid = FluidScroll()

        self.bg = game.memory.get('prep_bg')
        if not self.bg:
            self.bg = background.create_background('rune_background')
        self.unit = game.memory['current_unit']
        include_other_units_items = game.memory.get('include_other_units', False) or (self.name != 'supply_items')
        game.memory['include_other_units'] = False  # Reset
        self.menu = menus.Convoy(self.unit, (WINWIDTH - 116, 40), include_other_units_items)

        self.state = 'free'
        self.sub_menu = None

        self._proceed_with_targets_item = False

        game.state.change('transition_in')
        return 'repeat'

    def begin(self):
        if self._proceed_with_targets_item:
            self.state = 'free'
            self._proceed_with_targets_item = False
            if game.memory.get('item') and game.memory.get('item').data.get('target_item'):
                item = game.memory.get('item')
                action.do(action.HasTraded(self.unit))
                interaction.start_combat(self.unit, None, item)
                return 'repeat'

        self.menu.update_options()
        if self.name.startswith('base'):
            base_music = game.game_vars.get('_base_music')
            if base_music:
                get_sound_thread().fade_in(base_music)

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        if self.state in ('free', 'trade_convoy', 'trade_inventory'):
            self.menu.handle_mouse()
            if 'DOWN' in directions:
                if self.menu.move_down(first_push):
                    get_sound_thread().play_sfx('Select 6')
            elif 'UP' in directions:
                if self.menu.move_up(first_push):
                    get_sound_thread().play_sfx('Select 6')
            elif 'LEFT' in directions:
                if self.menu.move_left(first_push):
                    get_sound_thread().play_sfx('TradeRight')
            elif 'RIGHT' in directions:
                if self.menu.move_right(first_push):
                    get_sound_thread().play_sfx('TradeRight')
        elif self.sub_menu:
            self.sub_menu.handle_mouse()
            if 'DOWN' in directions:
                if self.sub_menu.move_down(first_push):
                    get_sound_thread().play_sfx('Select 6')
            elif 'UP' in directions:
                if self.sub_menu.move_up(first_push):
                    get_sound_thread().play_sfx('Select 6')

        if event == 'SELECT':
            get_sound_thread().play_sfx('Select 1')
            if self.state == 'free':
                current = self.menu.get_current()
                context = self.menu.get_context()
                if context == 'inventory':
                    if current:
                        self.state = 'owner_item'
                        options = []
                        if not item_system.locked(self.unit, current):
                            options.append('Store')
                            options.append('Trade')
                        if self.name != 'supply_items' and \
                                item_funcs.can_be_used_in_base(self.unit, current):
                            options.append('Use')
                        if convoy_funcs.can_restock(current):
                            options.append('Restock')
                        if not options:
                            options.append('Nothing')
                        top = self.menu.get_current_index() * 16 + 68 - 8 * len(options)
                        left = 96
                        top = min(top, WINHEIGHT - 4 - 16 * len(options))
                        self.sub_menu = menus.Choice(current, options, (left, top))
                    else:
                        self.menu.move_to_convoy()
                elif context == 'convoy':
                    if current:
                        if self.name != 'supply_items' and \
                                item_funcs.can_be_used_in_base(self.unit, current):
                            self.state = 'convoy_item'
                            topleft = (80, (self.menu.get_current_index() - self.menu.get_scrolled_index()) * 16 + 36)
                            if item_funcs.inventory_full(self.unit, current):
                                options = ['Trade', 'Use']
                            else:
                                options = ['Take', 'Use']
                            self.sub_menu = menus.Choice(current, options, topleft)
                        else:
                            action.do(action.HasTraded(self.unit))
                            if item_funcs.inventory_full(self.unit, current):
                                self.state = 'trade_inventory'
                                self.menu.move_to_inventory()
                            else:
                                if current.owner_nid:
                                    unit = game.get_unit(current.owner_nid)
                                    convoy_funcs.give_item(current, unit, self.unit)
                                else:
                                    convoy_funcs.take_item(current, self.unit)
                                self.menu.update_options()
                    else:
                        pass  # Nothing happens

            elif self.state == 'owner_item':
                current = self.sub_menu.get_current()
                item = self.menu.get_current()
                if current == 'Store':
                    action.do(action.HasTraded(self.unit))
                    convoy_funcs.store_item(item, self.unit)
                    self.menu.update_options()
                    self.menu.move_to_item_type(item)
                    self.state = 'free'
                elif current == 'Trade':
                    self.state = 'trade_convoy'
                    self.menu.move_to_convoy()
                    self.menu.update_options()
                elif current == 'Use':
                    if item_system.targets_items(self.unit, item):
                        game.memory['target'] = self.unit
                        game.memory['item'] = item
                        self._proceed_with_targets_item = True
                        game.state.change('item_targeting')
                    else:
                        action.do(action.HasTraded(self.unit))
                        interaction.start_combat(self.unit, None, item)
                        self.state = 'free'
                elif current == 'Restock':
                    action.do(action.HasTraded(self.unit))
                    convoy_funcs.restock(item)
                    self.menu.update_options()
                    self.state = 'free'
                elif current == 'Nothing':
                    self.state = 'free'
                self.sub_menu = None

            elif self.state == 'convoy_item':
                current = self.sub_menu.get_current()
                item = self.menu.get_current()
                if current == 'Take':
                    action.do(action.HasTraded(self.unit))
                    if item.owner_nid:
                        unit = game.get_unit(item.owner_nid)
                        convoy_funcs.give_item(item, unit, self.unit)
                    else:
                        convoy_funcs.take_item(item, self.unit)
                    self.state = 'free'
                elif current == 'Trade':
                    self.state = 'trade_inventory'
                    self.menu.move_to_inventory()
                elif current == 'Use':
                    if item_system.targets_items(self.unit, item):
                        game.memory['target'] = self.unit
                        game.memory['item'] = item
                        self._proceed_with_targets_item = True
                        game.state.change('item_targeting')
                    else:
                        action.do(action.HasTraded(self.unit))
                        interaction.start_combat(self.unit, None, item)
                        self.state = 'free'
                elif current == 'Nothing':
                    self.state = 'free'
                self.sub_menu = None
                self.menu.update_options()

            elif self.state == 'trade_convoy':
                unit_item = self.menu.get_inventory_current()
                convoy_item = self.menu.get_convoy_current()
                if trade.check_trade(unit_item, self.unit, convoy_item, None):
                    action.do(action.HasTraded(self.unit))
                    convoy_funcs.trade_items(convoy_item, unit_item, self.unit)
                    self.menu.unlock()
                    self.menu.update_options()
                    self.state = 'free'
                else:
                    get_sound_thread().play_sfx('Error')

            elif self.state == 'trade_inventory':
                convoy_item = self.menu.get_convoy_current()
                unit_item = self.menu.get_inventory_current()
                if trade.check_trade(unit_item, self.unit, convoy_item, None):
                    action.do(action.HasTraded(self.unit))
                    convoy_funcs.trade_items(convoy_item, unit_item, self.unit)
                    self.menu.unlock()
                    self.menu.update_options()
                    self.state = 'free'
                else:
                    get_sound_thread().play_sfx('Error')

        elif event == 'BACK':
            if self.menu.info_flag:
                self.menu.toggle_info()
                get_sound_thread().play_sfx('Info Out')
            elif self.state == 'free':
                get_sound_thread().play_sfx('Select 4')
                game.state.change('transition_pop')
            elif self.state == 'owner_item':
                self.sub_menu = None
                self.state = 'free'
            elif self.state == 'convoy_item':
                self.sub_menu = None
                self.state = 'free'
            elif self.state == 'trade_convoy':
                self.menu.move_to_inventory()
                self.menu.unlock()
                self.menu.update_options()
                self.state = 'free'
            elif self.state == 'trade_inventory':
                self.menu.move_to_convoy()
                self.menu.unlock()
                self.menu.update_options()
                self.state = 'free'

        elif event == 'INFO':
            if self.state in ('free', 'trade_convoy', 'trade_inventory'):
                self.menu.toggle_info()
                if self.menu.info_flag:
                    get_sound_thread().play_sfx('Info In')
                else:
                    get_sound_thread().play_sfx('Info Out')

    def update(self):
        self.menu.update()
        if self.sub_menu:
            self.sub_menu.update()

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        self.menu.draw(surf)
        if self.sub_menu:
            self.sub_menu.draw(surf)
        if self.menu.info_flag:
            self.menu.draw_info(surf)
        return surf

class PrepRestockState(State):
    name = 'prep_restock'

    def start(self):
        self.fluid = FluidScroll()

        self.bg = game.memory['prep_bg']
        self.unit = game.memory['current_unit']
        self.unit_menu = game.memory['manage_menu']

        topleft = (6, 72)
        self.menu = menus.Inventory(self.unit, self.unit.items, topleft)
        # ignore = [not convoy_funcs.can_restock(item) for item in self.unit.items]
        ignore = [not convoy_funcs.can_restock(option.get()) if option.get() else True for option in self.menu.options]
        self.menu.set_ignore(ignore)

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.menu.handle_mouse()
        if 'DOWN' in directions:
            get_sound_thread().play_sfx('Select 5')
            self.menu.move_down(first_push)
        elif 'UP' in directions:
            get_sound_thread().play_sfx('Select 5')
            self.menu.move_up(first_push)

        if event == 'SELECT':
            get_sound_thread().play_sfx('Select 1')
            item = self.menu.get_current()
            convoy_funcs.restock(item)
            true_ignore = [not convoy_funcs.can_restock(item) for item in self.unit.items]
            ignore = [not convoy_funcs.can_restock(option.get()) if option.get() else True for option in self.menu.options]
            if all(true_ignore):
                self.menu.set_ignore(ignore)
                game.state.back()
            else:
                self.menu.set_ignore(ignore)

        elif event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            game.state.back()

        elif event == 'INFO':
            self.menu.toggle_info()
            if self.menu.info_flag:
                get_sound_thread().play_sfx('Info In')
            else:
                get_sound_thread().play_sfx('Info Out')

    def update(self):
        self.menu.update()
        self.unit_menu.update()

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        self.unit_menu.draw(surf)
        self.menu.draw(surf)
        return surf


class PrepUseState(State):
    name = 'prep_use'

    def start(self):
        self.fluid = FluidScroll()

        self.bg = game.memory['prep_bg']
        self.unit = game.memory['current_unit']
        self.unit_menu = game.memory['manage_menu']

        topleft = (6, 72)
        self.menu = menus.Inventory(self.unit, self.unit.items, topleft)

    def begin(self):
        self.menu.update_options(self.unit.items)
        ignore = self.get_ignore()
        self.menu.set_ignore(ignore)
        if all(ignore):
            game.state.back()

    def get_ignore(self) -> List[bool]:
        items = [option.get() for option in self.menu.options]
        ignore = [not item_funcs.can_be_used_in_base(self.unit, item)
                  for item in items]
        return ignore

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.menu.handle_mouse()
        if 'DOWN' in directions:
            get_sound_thread().play_sfx('Select 5')
            self.menu.move_down(first_push)
        elif 'UP' in directions:
            get_sound_thread().play_sfx('Select 5')
            self.menu.move_up(first_push)

        if event == 'SELECT':
            get_sound_thread().play_sfx('Select 1')
            item = self.menu.get_current()
            # Actually Use item
            if item_system.targets_items(self.unit, item):
                game.memory['target'] = self.unit
                game.memory['item'] = item
                self._proceed_with_targets_item = True
                game.state.change('item_targeting')
            else:
                action.do(action.HasTraded(self.unit))
                interaction.start_combat(self.unit, None, item)

        elif event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            game.state.back()

        elif event == 'INFO':
            self.menu.toggle_info()
            if self.menu.info_flag:
                get_sound_thread().play_sfx('Info In')
            else:
                get_sound_thread().play_sfx('Info Out')

    def update(self):
        self.menu.update()
        self.unit_menu.update()

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        self.unit_menu.draw(surf)
        self.menu.draw(surf)
        return surf


class PrepMarketState(State):
    name = 'prep_market'

    def start(self):
        self.fluid = FluidScroll()

        self.bg = game.memory['prep_bg']
        self.unit = game.memory['current_unit']

        self.sell_menu = menus.Market(self.unit, None, (WINWIDTH - 164, 40), disp_value='sell')
        market_items = game.market_items.keys()
        market_items = item_funcs.create_items(self.unit, market_items)
        show_stock = any(stock >= 0 for stock in game.market_items.values())
        self.buy_menu = menus.Market(self.unit, market_items, (WINWIDTH - 164, 40), disp_value='buy', show_stock=show_stock)
        self.display_menu = self.buy_menu
        self.sell_menu.set_takes_input(False)
        self.buy_menu.set_takes_input(False)

        self.state = 'free'
        options = ["Buy", "Sell"]
        self.choice_menu = menus.Choice(self.unit, options, (20, 24), 'menu_bg_brown')
        self.choice_menu.gem = False
        self.menu = self.choice_menu

        self.money_counter_disp = gui.PopUpDisplay((66, WINHEIGHT - 40))

        game.state.change('transition_in')
        return 'repeat'

    def update_options(self):
        self.buy_menu.update_options()
        self.sell_menu.update_options()

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.menu.handle_mouse()
        if 'DOWN' in directions:
            if self.menu.move_down(first_push):
                get_sound_thread().play_sfx('Select 6')
            if self.state == 'free':
                current = self.menu.get_current()
                if current == 'Buy':
                    self.display_menu = self.buy_menu
                else:
                    self.display_menu = self.sell_menu
        elif 'UP' in directions:
            if self.menu.move_up(first_push):
                get_sound_thread().play_sfx('Select 6')
            if self.state == 'free':
                current = self.menu.get_current()
                if current == 'Buy':
                    self.display_menu = self.buy_menu
                else:
                    self.display_menu = self.sell_menu
        elif 'LEFT' in directions:
            get_sound_thread().play_sfx('TradeRight')
            self.display_menu.move_left(first_push)
        elif 'RIGHT' in directions:
            get_sound_thread().play_sfx('TradeRight')
            self.display_menu.move_right(first_push)

        if event == 'SELECT':
            if self.state == 'buy':
                item = self.menu.get_current()
                if item:
                    value = item_funcs.buy_price(self.unit, item)
                    if game.get_money() - value >= 0 and self.menu.get_stock() != 0:
                        get_sound_thread().play_sfx('GoldExchange')
                        game.set_money(game.get_money() - value)
                        action.do(action.UpdateRecords('money', (game.current_party, -value)))
                        self.money_counter_disp.start(-value)
                        self.menu.decrement_stock()
                        game.market_items[item.nid] -= 1
                        new_item = item_funcs.create_item(self.unit, item.nid)
                        game.register_item(new_item)
                        if not item_funcs.inventory_full(self.unit, new_item):
                            action.GiveItem(self.unit, new_item).do()
                        else:
                            action.PutItemInConvoy(new_item).do()
                        self.update_options()
                    elif self.menu.get_stock() == 0:
                        # Market is out of stock
                        get_sound_thread().play_sfx('Select 4')
                    else:
                        # You don't have enough money
                        get_sound_thread().play_sfx('Select 4')
                else:
                    # You didn't choose anything to buy
                    get_sound_thread().play_sfx('Select 4')

            elif self.state == 'sell':
                item = self.menu.get_current()
                if item:
                    value = item_funcs.sell_price(self.unit, item)
                    if value:
                        get_sound_thread().play_sfx('GoldExchange')
                        game.set_money(game.get_money() + value)
                        action.do(action.UpdateRecords('money', (game.current_party, value)))
                        self.money_counter_disp.start(value)
                        if item.owner_nid:
                            owner = game.get_unit(item.owner_nid)
                            action.RemoveItem(owner, item).do()
                        else:
                            action.RemoveItemFromConvoy(item).do()
                        self.update_options()
                    else:
                        # No value, can't be sold
                        get_sound_thread().play_sfx('Select 4')
                else:
                    # You didn't choose anything to sell
                    get_sound_thread().play_sfx('Select 4')

            elif self.state == 'free':
                current = self.menu.get_current()
                if current == 'Buy':
                    self.menu = self.buy_menu
                    self.state = 'buy'
                    self.display_menu = self.buy_menu
                else:
                    self.menu = self.sell_menu
                    self.state = 'sell'
                    self.display_menu = self.sell_menu
                self.menu.set_takes_input(True)

        elif event == 'BACK':
            if self.state == 'buy' or self.state == 'sell':
                if self.menu.info_flag:
                    self.menu.toggle_info()
                    get_sound_thread().play_sfx('Info Out')
                else:
                    get_sound_thread().play_sfx('Select 4')
                    self.state = 'free'
                    self.menu.set_takes_input(False)
                    self.menu = self.choice_menu
            else:
                get_sound_thread().play_sfx('Select 4')
                game.state.change('transition_pop')

        elif event == 'INFO':
            if self.state == 'buy' or self.state == 'sell':
                self.menu.toggle_info()
                if self.menu.info_flag:
                    get_sound_thread().play_sfx('Info In')
                else:
                    get_sound_thread().play_sfx('Info Out')

    def update(self):
        self.menu.update()

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        self.choice_menu.draw(surf)
        self.display_menu.draw(surf)
        # Money
        surf.blit(SPRITES.get('funds_display'), (10, WINHEIGHT - 24))
        money = str(game.get_money())
        FONT['text-blue'].blit_right(money, surf, (61, WINHEIGHT - 20))
        self.money_counter_disp.draw(surf)

        if self.display_menu.info_flag:
            self.display_menu.draw_info(surf)

        return surf
