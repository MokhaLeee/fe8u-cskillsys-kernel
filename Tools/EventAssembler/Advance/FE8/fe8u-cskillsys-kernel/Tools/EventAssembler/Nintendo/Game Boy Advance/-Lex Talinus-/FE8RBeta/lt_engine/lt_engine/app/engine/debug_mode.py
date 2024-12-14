from __future__ import annotations

from typing import List

from app.constants import WINHEIGHT, WINWIDTH
from app.engine import config, engine
from app.engine.fonts import FONT
from app.engine.game_state import game
from app.engine.graphics.text.text_renderer import render_text
from app.engine.input_manager import get_input_manager
from app.engine.objects.unit import UnitObject
from app.engine.sound import get_sound_thread
from app.engine.sprites import SPRITES
from app.engine.state import MapState
from app.events import event_commands
from app.events.triggers import GenericTrigger
from app.utilities.enums import HAlignment
from app.utilities.typing import NID


class DebugState(MapState):
    num_back: int = 4
    backspace_time: int = 80  # ms
    current_command: str = ''
    commands = config.get_debug_commands()
    bg = SPRITES.get('debug_bg').convert_alpha()
    quit_commands: List[str] = ['q', 'exit', '']

    def begin(self):
        game.cursor.show()
        self.current_command = ''
        self.buffer_count = 0
        self.backspace_down = 0

        self.quit_commands += engine.get_key_name(get_input_manager().key_map['BACK'])

    def take_input(self, event):
        current_time = engine.get_time()
        game.cursor.take_input()

        for event in engine.events:
            if event.type == engine.KEYDOWN:
                if event.key == engine.key_map['enter']:
                    self.parse_command(self.current_command)
                    if self.current_command not in self.quit_commands:
                        self.commands.append(self.current_command)
                    self.current_command = ''
                    self.buffer_count = 0
                elif event.key == engine.key_map['backspace']:
                    self.current_command = self.current_command[:-1]
                    self.backspace_down = current_time
                elif event.key == engine.key_map['pageup'] and self.commands:
                    self.buffer_count += 1
                    if self.buffer_count >= len(self.commands):
                        self.buffer_count = 0
                    self.current_command = self.commands[-self.buffer_count]
                else:
                    self.current_command += event.unicode

            elif event.type == engine.KEYUP:
                if event.key == engine.key_map['backspace']:
                    self.backspace_down = 0

        if self.backspace_down and current_time - self.backspace_down > self.backspace_time:
            self.current_command = self.current_command[:-1]
            self.backspace_down = current_time

    def parse_command(self, command):
        if command in self.quit_commands:
            get_sound_thread().play_sfx('Select 4')
            game.state.back()
            return

        event_command, error_loc = event_commands.parse_text_to_command(command)
        if not event_command:
            return
        game.events._add_event_from_script('debug_console', str(command), GenericTrigger(unit1=game.cursor.get_hover(), position=game.cursor.position))

    def draw(self, surf):
        surf = super().draw(surf)
        self.draw_bg(surf)
        self.draw_hover_info(surf)
        for idx, command in enumerate(reversed(self.commands[-self.num_back:])):
            FONT['text'].blit(command, surf, (0, WINHEIGHT - idx * 16 - 32))
        FONT['text'].blit(self.current_command, surf, (0, WINHEIGHT - 16))
        return surf

    def draw_bg(self, surf):
        surf.blit(self.bg, (0, 0 - (4 * 16)))
        surf.blit(self.bg, (0, WINHEIGHT - (5 * 16)))

    def draw_hover_info(self, surf):
        if game.is_displaying_overworld():
            return
        unit: UnitObject = game.cursor.get_hover()
        unit_position_info = [""]
        colors: List[NID | None] = []
        if unit:
            unit_position_info = [unit.nid, ': ', str(unit.position)]
            colors = ['white', 'white', 'blue']
        else:
            unit_position_info = [str(game.cursor.position)]
            colors = ['blue']
        render_text(surf, ['text'], unit_position_info, colors, topleft=(WINWIDTH, 0), align=HAlignment.RIGHT)

    def end(self):
        game.cursor.hide()
        config.save_debug_commands(self.commands[-20:])
