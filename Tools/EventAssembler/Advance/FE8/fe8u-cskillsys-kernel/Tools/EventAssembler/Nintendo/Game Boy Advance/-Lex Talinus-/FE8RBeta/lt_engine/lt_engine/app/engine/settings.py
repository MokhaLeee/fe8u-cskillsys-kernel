from app.constants import WINWIDTH, WINHEIGHT

from app.engine import config as cf
from app.engine.sprites import SPRITES
from app.engine.fonts import FONT
from app.engine.sound import get_sound_thread
from app.engine.input_manager import get_input_manager
from app.engine.state import State
from app.engine import engine, background, banner, menus, settings_menu, base_surf, text_funcs
from app.engine.game_state import game
from app.engine.fluid_scroll import FluidScroll

controls = {'key_SELECT': engine.subsurface(SPRITES.get('buttons'), (0, 66, 14, 13)),
            'key_BACK': engine.subsurface(SPRITES.get('buttons'), (0, 82, 14, 13)),
            'key_INFO': engine.subsurface(SPRITES.get('buttons'), (1, 149, 16, 9)),
            'key_AUX': engine.subsurface(SPRITES.get('buttons'), (1, 133, 16, 9)),
            'key_START': engine.subsurface(SPRITES.get('buttons'), (0, 165, 33, 9)),
            'key_LEFT': engine.subsurface(SPRITES.get('buttons'), (1, 4, 13, 12)),
            'key_RIGHT': engine.subsurface(SPRITES.get('buttons'), (1, 19, 13, 12)),
            'key_DOWN': engine.subsurface(SPRITES.get('buttons'), (1, 34, 12, 13)),
            'key_UP': engine.subsurface(SPRITES.get('buttons'), (1, 50, 12, 13))}
control_order = ('key_SELECT', 'key_BACK', 'key_INFO', 'key_AUX', 'key_LEFT', 'key_RIGHT', 'key_UP', 'key_DOWN', 'key_START')

config = [('animation', ['Always', 'Your Turn', 'Combat Only', 'Never'], 0),
          ('screen_size', [1, 2, 3, 4, 5, 6], 18),
          ('display_fps', bool, 2),
          ('battle_bg', bool, 17),
          ('unit_speed', list(reversed(range(15, 180, 15))), 1),
          ('text_speed', cf.text_speed_options, 2),
          # ('cursor_speed', list(reversed(range(32, 160, 16))), 8),
          ('mouse', bool, 19),
          ('show_terrain', bool, 7),
          ('show_objective', bool, 6),
          ('autocursor', bool, 13),
          ('hp_map_team', ['All', 'Ally', 'Enemy'], 10),
          ('hp_map_cull', ['None', 'Wounded', 'All'], 10),
          ('music_volume', [x/10.0 for x in range(0, 11, 1)], 15),
          ('sound_volume', [x/10.0 for x in range(0, 11, 1)], 16),
          ('talk_boop', bool, 16),
          ('show_bounds', bool, 7),
          ('grid_opacity', [int(255 * x / 10.0) for x in range(11)], 7),
          ('autoend_turn', bool, 14),
          ('confirm_end', bool, 14),
          ('display_hints', bool, 3)]

config_icons = [engine.subsurface(SPRITES.get('settings_icons'), (0, c[2] * 16, 16, 16)) for c in config]

class SettingsMenuState(State):
    name = 'settings_menu'
    in_level = False
    header_width = 112

    def start(self):
        self.fluid = FluidScroll(128)
        self.bg = background.create_background('settings_background')
        # top_menu_left, top_menu_right, config, controls, get_input
        self.state = 'top_menu_left'

        control_options = control_order
        control_icons = [controls[c] for c in control_options]
        self.controls_menu = settings_menu.Controls(None, control_options, 'menu_bg_base', control_icons)
        self.controls_menu.takes_input = False

        config_options = [(c[0], c[1]) for c in config]
        self.config_menu = settings_menu.Config(None, config_options, 'menu_bg_base', config_icons)
        self.config_menu.takes_input = False

        self.top_cursor = menus.Cursor()

        game.state.change('transition_in')
        return 'repeat'

    @property
    def current_menu(self):
        if self.state in ('top_menu_left', 'config'):
            return self.config_menu
        else:
            return self.controls_menu

    def handle_mouse(self):
        mouse_position = get_input_manager().get_mouse_position()
        if mouse_position:
            mouse_x, mouse_y = mouse_position
            top_left_rect = (4, 4, self.header_width, 24)
            top_right_rect = (WINWIDTH//2 + 4, 4, self.header_width, 24)
            # Test left rect
            x, y, width, height = top_left_rect
            if x <= mouse_x <= x + width and y <= mouse_y <= y + height:
                self.current_menu.takes_input = False
                self.state = 'top_menu_left'
                return
            # Test right rect
            x, y, width, height = top_right_rect
            if x <= mouse_x <= x + width and y <= mouse_y <= y + height:
                self.current_menu.takes_input = False
                self.state = 'top_menu_right'
                return
            current_idxs, current_option_rects = self.current_menu.get_rects()
            for idx, option_rect in zip(current_idxs, current_option_rects):
                x, y, width, height = option_rect
                if x <= mouse_x <= x + width and y <= mouse_y <= y + height:
                    if self.state in ('top_menu_left', 'config'):
                        self.state = 'config'
                    else:
                        self.state = 'controls'
                    self.current_menu.takes_input = True
                    self.current_menu.move_to(idx)
                    return

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        if self.state == 'get_input':
            if event == 'BACK':
                get_sound_thread().play_sfx('Select 4')
                self.state = 'controls'
                get_input_manager().set_change_keymap(False)
            elif event == 'NEW':
                get_sound_thread().play_sfx('Select 1')
                self.state = 'controls'
                selection = self.current_menu.get_current()
                cf.SETTINGS[selection] = get_input_manager().unavailable_button
                get_input_manager().set_change_keymap(False)
                get_input_manager().update_key_map()
            elif event:
                get_sound_thread().play_sfx('Select 4')
                self.state = 'controls'
                get_input_manager().set_change_keymap(False)
                text = 'Invalid Choice!'
                game.alerts.append(banner.Custom(text))
                game.state.change('alert')

        elif self.state in ('top_menu_left', 'top_menu_right'):
            self.handle_mouse()
            if event == 'DOWN' or event == 'SELECT':
                get_sound_thread().play_sfx('Select 6')
                if self.state == 'top_menu_left':
                    self.state = 'config'
                else:
                    self.state = 'controls'
                self.current_menu.takes_input = True
            elif event == 'LEFT':
                if self.state == 'top_menu_right':
                    get_sound_thread().play_sfx('Select 6')
                    self.state = 'top_menu_left'
            elif event == 'RIGHT':
                if self.state == 'top_menu_left':
                    get_sound_thread().play_sfx('Select 6')
                    self.state = 'top_menu_right'
            elif event == 'BACK':
                self.back()

        else:
            self.handle_mouse()
            if 'DOWN' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.current_menu.move_down(first_push)
            elif 'UP' in directions:
                get_sound_thread().play_sfx('Select 6')
                if self.current_menu.get_current_index() <= 0:
                    self.current_menu.takes_input = False
                    if self.state == 'config':
                        self.state = 'top_menu_left'
                    else:
                        self.state = 'top_menu_right'
                else:
                    self.current_menu.move_up(first_push)
            elif 'LEFT' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.current_menu.move_left()
                if self.current_menu.get_current_option().name in ('music_volume', 'sound_volume'):
                    self.update_sound()
            elif 'RIGHT' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.current_menu.move_right()
                if self.current_menu.get_current_option().name in ('music_volume', 'sound_volume'):
                    self.update_sound()

            elif event == 'BACK':
                self.back()

            elif event == 'SELECT':
                if self.state == 'controls':
                    get_sound_thread().play_sfx('Select 1')
                    self.state = 'get_input'
                    get_input_manager().set_change_keymap(True)
                elif self.state == 'config':
                    get_sound_thread().play_sfx('Select 6')
                    self.current_menu.move_next()
                    if self.current_menu.get_current_option().name in ('music_volume', 'sound_volume'):
                        self.update_sound()

    def back(self):
        get_sound_thread().play_sfx('Select 4')
        cf.save_settings()
        self.update_sound()
        # if game.cursor is not None:
        #     game.cursor.fluid.update_speed(cf.SETTINGS['cursor_speed'])
        game.state.change('transition_pop')

    def update_sound(self):
        get_sound_thread().set_music_volume(cf.SETTINGS['music_volume'])
        get_sound_thread().set_sfx_volume(cf.SETTINGS['sound_volume'])

    def update(self):
        self.current_menu.update()
        self.top_cursor.update()

    def draw_top_menu(self, surf):
        bg = base_surf.create_base_surf(self.header_width, 24, 'menu_bg_clear')
        offset = (WINWIDTH // 2 - self.header_width) // 2
        surf.blit(bg, (offset, 4))
        surf.blit(bg, (WINWIDTH//2 + offset, 4))
        if self.current_menu is self.config_menu:
            FONT['text-yellow'].blit_center('Config', surf, (offset + self.header_width//2, 8))
            FONT['text-grey'].blit_center('Controls', surf, (WINWIDTH//2 + offset + self.header_width//2, 8))
            if self.state in ('top_menu_left', 'top_menu_right'):
                self.top_cursor.draw(surf, self.header_width//2 - 16, 8)
        else:
            FONT['text-grey'].blit_center('Config', surf, (offset + self.header_width/2, 8))
            FONT['text-yellow'].blit_center('Controls', surf, (WINWIDTH//2 + offset + self.header_width//2, 8))
            if self.state in ('top_menu_left', 'top_menu_right'):
                self.top_cursor.draw(surf, WINWIDTH//2 + 2 + self.header_width//2 - 16, 8)

    def draw_info_banner(self, surf):
        height = 16
        bg = base_surf.create_base_surf(WINWIDTH + 16, height, 'menu_bg_clear')
        surf.blit(bg, (-8, WINHEIGHT - height))
        if self.state == 'top_menu_left':
            text = 'config_desc'
        elif self.state == 'top_menu_left':
            text = 'controls_desc'
        elif self.state == 'config':
            idx = self.config_menu.get_current_index()
            text = config[idx][0] + '_desc'
        elif self.state == 'get_input':
            text = 'get_input_desc'
        else:
            text = 'keymap_desc'
        text = text_funcs.translate(text)
        FONT['text'].blit_center(text, surf, (WINWIDTH//2, WINHEIGHT - height))

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        else:
            # settings menu shouldn't be transparent
            surf.blit(SPRITES.get('bg_black'), (0, 0))

        self.draw_top_menu(surf)
        if self.state == 'get_input':
            self.current_menu.draw(surf, True)
        else:
            self.current_menu.draw(surf)
        self.draw_info_banner(surf)

        return surf

    def finish(self):
        # Just to make sure!
        get_input_manager().set_change_keymap(False)
