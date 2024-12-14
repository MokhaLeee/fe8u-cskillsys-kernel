from app.constants import WINWIDTH, WINHEIGHT
from app.engine.sound import get_sound_thread
from app.engine.sprites import SPRITES
from app.engine.fonts import FONT
from app.engine.state import MapState

from app.engine import menus, action, base_surf, engine
from app.engine.game_state import game

class TextEntryState(MapState):
    name = 'text_entry'
    transparent = True
    keyboard_mode_sprite = SPRITES.get('focus_fade')

    def start(self):
        self.constant_id, self.header, self.character_limit, illegal_characters, self.force_entry = game.memory['text_entry']
        self.menu = menus.KeyboardMenu(self.header, self.character_limit, illegal_characters)
        self.bg_surf, self.topleft = self.create_bg_surf()
        self.keyboard_mode = False

        game.state.change('transition_in')
        return 'repeat'

    def create_bg_surf(self):
        width_of_header = FONT['text'].width(self.header) + 16
        menu_width = self.menu.get_menu_width()
        width = max(width_of_header, menu_width)
        menu_height = self.menu.get_menu_height()
        height = menu_height + FONT['text'].height
        bg_surf = base_surf.create_base_surf(width, height, 'menu_bg_base')
        topleft = (9, 42)
        return bg_surf, topleft

    def _back(self):
        if len(self.menu.name) > 0:
            self.menu.updateName('back')
            get_sound_thread().play_sfx('Select 4')
        else:
            get_sound_thread().play_sfx('Error')

    def _add(self, selection):
        if len(self.menu.name) < self.menu.character_limit and selection:
            self.menu.updateName(selection)
            get_sound_thread().play_sfx('Select 1')
        else:
            get_sound_thread().play_sfx('Error')

    def take_input(self, event):
        if self.keyboard_mode:
            for pg_event in engine.events:
                if pg_event.type == engine.KEYDOWN:
                    if pg_event.key == engine.key_map['enter']:
                        self.keyboard_mode = False
                        break
                    elif pg_event.key == engine.key_map['backspace']:
                        self._back()
                    else:
                        self._add(pg_event.unicode)
        else:
            for pg_event in engine.events:
                if pg_event.type == engine.KEYDOWN:
                    if pg_event.key == engine.key_map['enter']:
                        self.keyboard_mode = True

            first_push = self.fluid.update()
            directions = self.fluid.get_directions()

            self.menu.handle_mouse()
            if 'RIGHT' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.menu.move_right(first_push)
            elif 'LEFT' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.menu.move_left(first_push)
            if 'DOWN' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.menu.move_down(first_push)
            elif 'UP' in directions:
                get_sound_thread().play_sfx('Select 6')
                self.menu.move_up(first_push)

            if event == 'BACK':
                self._back()

            elif event == 'SELECT':
                selection = self.menu.get_current()
                if selection:
                    self._add(selection)

            elif event == 'START':
                game.memory['text_entry_menu'] = (self.constant_id, self.menu.name)
                game.state.change('text_confirm')
                get_sound_thread().play_sfx('Select 2')

            #Exits text entry without saving a value. Dev must dictate whether this is available by setting the flag in the event.
            elif event == 'AUX' and not self.force_entry:
                game.state.change('transition_pop')

    def update(self):
        if self.menu:
            self.menu.update()

    def draw(self, surf):
        surf.blit(self.bg_surf, self.topleft)
        if self.menu:
            self.menu.draw(surf)
        if self.keyboard_mode:
            surf.blit(self.keyboard_mode_sprite, (0, 0))
        return surf

class TextConfirmState(MapState):
    name = 'text_confirm'
    transparent = True

    def start(self):
        self.header = 'Finish text entry?'
        options_list = ['Yes', 'No']
        self.orientation = 'vertical'
        self.menu = menus.Choice(None, options_list, 'center', None)
        self.bg_surf, self.topleft = self.create_bg_surf()
        self.menu.topleft = (self.topleft[0], self.topleft[1] + FONT['text'].height)

    def create_bg_surf(self):
        width_of_header = FONT['text'].width(self.header) + 16
        menu_width = self.menu.get_menu_width()
        width = max(width_of_header, menu_width)
        menu_height = self.menu.get_menu_height() if self.orientation == 'vertical' else FONT['text'].height + 8
        height = menu_height + FONT['text'].height
        bg_surf = base_surf.create_base_surf(width, height, 'menu_bg_base')
        topleft = (WINWIDTH//2 - width//2, WINHEIGHT//2 - height//2)
        return bg_surf, topleft

    def take_input(self, event):
        self.menu.handle_mouse()
        if (event == 'RIGHT' and self.orientation == 'horizontal') or \
                (event == 'DOWN' and self.orientation == 'vertical'):
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_down()
        elif (event == 'LEFT' and self.orientation == 'horizontal') or \
                (event == 'UP' and self.orientation == 'vertical'):
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_up()

        elif event == 'BACK':
            get_sound_thread().play_sfx('Error')

        elif event == 'SELECT':
            get_sound_thread().play_sfx('Select 1')
            selection = self.menu.get_current()
            if selection == 'Yes':
                constant_id, text = game.memory['text_entry_menu']
                action.do(action.SetGameVar(constant_id, text))
                action.do(action.SetGameVar('last_response_entered', text))
                game.state.change('transition_double_pop')
            else:
                game.state.back()

    def update(self):
        self.menu.update()

    def draw(self, surf):
        surf.blit(self.bg_surf, self.topleft)
        FONT['text'].blit(self.header, surf, (self.topleft[0] + 4, self.topleft[1] + 4))

        # Place Menu on background
        self.menu.draw(surf)
        return surf
