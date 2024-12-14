from __future__ import annotations

import re

from typing import List, Optional, Tuple

from app.data.resources.resources import RESOURCES
from app.data.database.database import DB

from app.engine import engine, icons
from app.engine.graphics.dialog.dialog_log_ui import DialogLogUI
from app.engine.state import State
from app.engine.fluid_scroll import FluidScroll
from app.engine.game_state import game


class DialogLogState(State):
    name = 'dialog_log'
    transparent = True

    def start(self):
        self.fluid = FluidScroll(speed=16)

    def begin(self):
        game.dialog_log.ui.scroll_to_bottom() # Set scroll to bottom.

    def take_input(self, event):
        self.fluid.update()
        directions = self.fluid.get_directions()

        if 'UP' in directions:
            game.dialog_log.ui.scroll_up()
        elif 'DOWN' in directions:
            game.dialog_log.ui.scroll_down()
        
        if event == 'INFO' or event == 'BACK':
            game.state.back()

    def draw(self, surf):
        surf = game.dialog_log.ui.draw(surf)
        return surf

class DialogLog():
    def __init__(self):
        self.ui = DialogLogUI()
        self.entries: List[Tuple[str, str]] = []

    def append(self, dialog_tuple: Tuple[str, str]):
        speaker, text = dialog_tuple
        text = DialogLog.clean_speak_text(text)
        self.ui.add_entry(speaker, self.get_chibi(speaker), text)
        self.entries.append((speaker, text))

    def pop(self):
        self.ui.pop_entry()
        self.entries.pop()

    def clear(self):
        while self.entries:
            self.pop()
        self.entries.clear()

    def get_chibi(self, name: str) -> Optional[engine.Surface]:
        unit = game.get_unit(name)
        if unit:
            name = unit.nid
        if unit and unit.portrait_nid:
            portrait = RESOURCES.portraits.get(unit.portrait_nid)
        elif name in DB.units.keys():
            portrait = RESOURCES.portraits.get(DB.units.get(name).portrait_nid)
        else:
            portrait = RESOURCES.portraits.get(name)

        # Get chibi
        chibi = None
        if portrait:
            chibi = icons.get_chibi(portrait)
            
        return chibi

    def save(self) -> List[Tuple[str, str]]:
        return self.entries

    def load(self, entries: List[Tuple[str, str]]):
        for entry in entries:
            self.append(entry)

    @staticmethod
    def clean_speak_text(s: str) -> str:
        """
        Returns a copy of the "speak" command text without any commands
        besides {br}, which is kept to force new-lines where appropraite

        >>> s = 'This is a test| with{w}{br} commands.'
        >>> clean_speak_text(s)
        >>> 'This is a test{br} with{br} commands.'
        """
        # x = re.sub(r'({\w*})|(\|)|(;)/', ' ', s)
        s = s.strip()
        s = s.replace('{semicolon}', ';')
        s = s.replace('|', '{br}')
        s = s.replace('{br}', '<br>')  # So we don't remove it
        s = re.sub(r'(\{[^\{]*?\})', '', s)  # remove all commands
        s = s.replace('<br>', '{br}')
        # Get rid of extra spaces beyond length of 1
        return re.sub(r' +', ' ', s)

    @classmethod
    def restore(cls, entries: List[Tuple[str, str]]):
        d = cls()
        d.load(entries)
        return d
