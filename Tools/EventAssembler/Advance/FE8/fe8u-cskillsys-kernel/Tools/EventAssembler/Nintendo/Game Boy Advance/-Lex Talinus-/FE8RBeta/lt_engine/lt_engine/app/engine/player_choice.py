import logging
from typing import List, Tuple

from app.data.database.database import DB
from app.engine import action
from app.engine.game_menus.menu_components.generic_menu.cursor_hand import CursorDrawMode
from app.engine.game_menus.menu_components.generic_menu.grid_choice import GridChoiceMenu
from app.engine.game_state import game
from app.engine.sound import get_sound_thread
from app.engine.state import MapState
from app.utilities.enums import Orientation
from app.utilities.typing import NID

from app.engine import item_funcs, help_menu, item_system
from app.constants import WINWIDTH, WINHEIGHT


class PlayerChoiceState(MapState):
    name = 'player_choice'
    transparent = True

    def start(self):
        self.nid, self.header, options_list, self.row_width, self.orientation, \
            self.data_type, self.should_persist, self.alignment, self.bg, self.event_on_choose, \
            self.size, self.no_cursor, self.arrows, self.scroll_bar, self.text_align, self.backable, self.event_context = \
            game.memory['player_choice']
        self.is_callable = False
        self.data = options_list
        if callable(options_list):
            self._resolved_data = options_list()
            self.is_callable = True
        else:
            self._resolved_data = options_list

        if not self.size:
            if self.orientation == Orientation.HORIZONTAL:
                ncols = len(self._resolved_data)
                self.size = (ncols, 1)
            else:
                nrows = len(self._resolved_data)
                self.size = (1, nrows)

        values, display_values = self.process_data(self._resolved_data)
        self.menu = GridChoiceMenu(values, display_values, self.header, self.data_type, self.size,
                                   self.row_width, self.alignment, self.orientation,
                                   self.bg, self.text_align)

        if self.scroll_bar is not None:
            self.menu.set_scrollbar(self.scroll_bar)
        if self.arrows is not None:
            self.menu.set_arrows(self.arrows)

        self.made_choice = False

        # For now putting info stuff here because innards of UIF are too arcane.
        self.info_flag = False
        self.create_help_boxes(values)

    def process_data(self, data: List[str]) -> Tuple[List[str], List[str]]:
        data = list(map(str, data))
        values = []
        display_values = []
        for datum in data:
            spl = datum.split("|")
            if len(spl) == 1:
                values.append(spl[0])
                display_values.append(None)
            else:
                spl = [spl[0], ''.join(spl[1:])]
                values.append(spl[0])
                display_values.append(spl[1])
        return values, display_values

    def create_help_boxes(self, options_list: List[NID]):
        self.help_boxes = []
        if self.data_type == 'type_base_item':
            items = item_funcs.create_items(None, options_list)
            for item in items:
                if item_system.is_weapon(None, item) or item_system.is_spell(None, item):
                    self.help_boxes.append(help_menu.ItemHelpDialog(item))
                else:
                    self.help_boxes.append(help_menu.HelpDialog(item.desc))
        elif self.data_type == 'type_skill':
            for skill_nid in options_list:
                skill = DB.skills.get(skill_nid)
                self.help_boxes.append(help_menu.HelpDialog(skill.desc))

    def choose(self, selection):
        action.do(action.SetGameVar(self.nid, selection))
        action.do(action.SetGameVar('_last_choice', selection))
        self.made_choice = True

    def unchoose(self):
        self.made_choice = False

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()
        if ('RIGHT' in directions and (self.orientation == Orientation.HORIZONTAL or self.size[0] > 1)):
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_right(first_push)
        elif ('DOWN' in directions and (self.orientation == Orientation.VERTICAL or self.size[1] > 1)):
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_down(first_push)
        elif ('LEFT' in directions and (self.orientation == Orientation.HORIZONTAL or self.size[0] > 1)):
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_left(first_push)
        elif ('UP' in directions and (self.orientation == Orientation.VERTICAL or self.size[1] > 1)):
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_up(first_push)

        if event == 'BACK':
            if self.should_persist or self.backable:
                if self.backable:
                    action.do(action.SetGameVar(self.nid, "BACK"))
                # this is the only way to exit a persistent state
                game.state.back()
            else:
                get_sound_thread().play_sfx('Error')

        elif event == 'SELECT':
            get_sound_thread().play_sfx('Select 1')
            selection = self.menu.get_selected()
            self.choose(selection)
            if self.event_on_choose:
                valid_events = DB.events.get_by_nid_or_name(
                    self.event_on_choose, game.level.nid)
                for event_prefab in valid_events:
                    game.events.trigger_specific_event(
                        event_prefab.nid, **self.event_context)
                    game.memory[self.nid + '_unchoice'] = self.unchoose
                if not valid_events:
                    logging.error(
                        "Couldn't find any valid events matching name %s" % self.event_on_choose)
            return 'repeat'

        elif event == 'INFO':
            if self.data_type == 'type_unit':
                unit = game.get_unit(self.menu.get_selected())
                if not unit:
                    get_sound_thread().play_sfx('Error')
                else:
                    all_units = [game.get_unit(unid) for unid in self._resolved_data if game.get_unit(unid)]
                    game.memory['scroll_units'] = all_units
                    game.memory['current_unit'] = unit
                    game.memory['next_state'] = 'info_menu'
                    game.state.change('transition_to')
            else:
                if self.info_flag:
                    get_sound_thread().play_sfx('Info Out')
                    self.info_flag = False
                elif self.help_boxes:
                    get_sound_thread().play_sfx('Info In')
                    self.info_flag = True

        selection = self.menu.get_selected()
        game.game_vars[self.nid + '_choice_hover'] = selection

    def update(self):
        if self.is_callable:
            data = self.data()
            if data != self._resolved_data:
                self._resolved_data = data
                values, display_values = self.process_data(self._resolved_data)
                self.menu.set_data(values, display_values)
                self.create_help_boxes(values)

        self.menu.update()
        if self.made_choice and not self.should_persist:
            game.state.back()
            return 'repeat'

    def draw(self, surf):
        if not self.started:
            return
        draw_mode = CursorDrawMode.NO_DRAW
        if not self.no_cursor:
            draw_mode = CursorDrawMode.DRAW if game.state.current_state(
            ) == self else CursorDrawMode.DRAW_STATIC
        self.menu.set_cursor_mode(draw_mode)
        self.menu.draw(surf)

        if self.info_flag:
            idx = self.menu.get_selected_idx()
            help_box = self.help_boxes[idx]
            if not help_box:
                pass
            else:
                topleft = self.menu.get_topleft_of_idx(idx)
                if topleft[0] < WINWIDTH // 2:
                    help_box.draw(surf, (topleft[0] - 4, topleft[1] + 16))
                else:
                    width, _ = self.menu._get_pixel_size()
                    help_box.draw(surf, (topleft[0] + width, topleft[1] + 16), right=True)

        return surf
