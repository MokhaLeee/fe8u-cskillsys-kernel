from __future__ import annotations

from typing import Set, Tuple, Dict
from app.utilities.enums import Alignments
from app.utilities.typing import NID

from app.constants import WINWIDTH

from app.utilities.data import Prefab

class SpeakStyle(Prefab):
    def __init__(self, nid: NID, speaker: NID = None, position: Alignments | Tuple[int, int] = None,
                 width: int = None, speed: float = None, font_color: str = None,
                 font_type: str = None, background: str = None, num_lines: int = None,
                 draw_cursor: bool = None, message_tail: str = None, transparency: float = None,
                 name_tag_bg: str = None, flags: Set[str] = None):
        self.nid: NID = nid
        self.speaker: NID = speaker
        self.position: Alignments | Tuple[int, int] = position
        self.width: int = width
        self.speed: float = speed
        self.font_color: str = font_color
        self.font_type: str = font_type
        self.background: str = background
        self.num_lines: int = num_lines
        self.draw_cursor: bool = draw_cursor
        self.message_tail: str = message_tail
        self.transparency: float = transparency
        self.name_tag_bg: str = name_tag_bg
        self.flags: Set[str] = flags or set()

    def copy(self) -> SpeakStyle:
        return self.restore(self.as_dict())

    def as_dict(self) -> Dict:
        ser = self.save()
        return {param: val for param, val in ser.items() if (param != 'nid' and val is not None)}

    def update(self, other: SpeakStyle) -> SpeakStyle:
        as_dict = self.as_dict()
        other_dict = other.as_dict()
        as_dict.update(other_dict)
        flags = self.flags.union(other.flags)
        new = self.restore(as_dict)
        new.flags = flags
        return new

    @classmethod
    def default(cls):
        return cls("")

class SpeakStyleLibrary(Dict[NID, SpeakStyle]):
    def __init__(self, user_styles=None):
        # Built in speak styles for backwards compatibility
        self.update(
            {'__default': SpeakStyle('__default', speed=1, font_type='convo', background='message_bg_base', num_lines=2, draw_cursor=True,
                                     message_tail='message_bg_tail', name_tag_bg='name_tag', transparency=0.05),
             '__default_text': SpeakStyle('__default_text', speed=0.5, font_type='text', background='menu_bg_base', num_lines=0, name_tag_bg='menu_bg_base', transparency=0.05),
             '__default_help': SpeakStyle('__default_help', speed=0.5, font_type='convo', background='None', draw_cursor=False, num_lines=8, name_tag_bg='name_tag', transparency=0.05),
             'noir': SpeakStyle('noir', background='menu_bg_dark', font_color='white', message_tail='None'),
             'hint': SpeakStyle('hint', background='menu_bg_parchment', position=Alignments.CENTER, width=WINWIDTH//2 + 8, num_lines=4, message_tail='None'),
             'cinematic': SpeakStyle('cinematic', background='None', position=Alignments.CENTER, font_color='grey', num_lines=5,
                                     font_type='chapter', draw_cursor=False, message_tail='None'),
             'narration': SpeakStyle('narration', background='menu_bg_base', position=(4, 110), width=WINWIDTH - 8,
                                     font_color='white', message_tail='None'),
             'narration_top': SpeakStyle('narration_top', background='menu_bg_base', position=(4, 2), width=WINWIDTH - 8,
                                         font_color='white', message_tail='None'),
             'clear': SpeakStyle('clear', background='None', font_color='white', draw_cursor=False, message_tail='None'),
             'thought_bubble': SpeakStyle('thought_bubble', message_tail='message_bg_thought_tail', flags={'no_talk'}),
             'boss_convo_left': SpeakStyle('boss_convo_left', speed=1, font_type='convo', background='message_bg_base', num_lines=2, draw_cursor=True,
                                           message_tail='message_bg_tail', transparency=0.0, position=(72, 112), width=WINWIDTH - 80),
             'boss_convo_right': SpeakStyle('boss_convo_right', speed=1, font_type='convo', background='message_bg_base', num_lines=2, draw_cursor=True,
                                            message_tail='message_bg_tail', transparency=0.0, position=(8, 112), width=WINWIDTH - 80),
             }
        )

        if user_styles:
            self.update(user_styles)

    def save(self) -> dict:
        return {nid: style.save() for nid, style in self.items()}

    @classmethod
    def restore(cls, save_dict) -> SpeakStyleLibrary:
        self = cls()
        for nid, ser_dict in save_dict.items():
            style = SpeakStyle.restore(ser_dict)
            self[nid] = style
        return self
