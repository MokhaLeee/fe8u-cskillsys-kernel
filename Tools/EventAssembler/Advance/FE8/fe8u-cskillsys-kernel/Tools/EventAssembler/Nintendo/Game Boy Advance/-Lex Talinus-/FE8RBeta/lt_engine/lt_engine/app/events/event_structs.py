from dataclasses import dataclass, field
from enum import Enum
from typing import List

from app.utilities.typing import NID

class ParseMode(Enum):
    COMMAND = 1
    ARGS = 2
    FLAGS = 3
    EOL = 4     # basically only happens when there's a hard stop char, such as comment #, that finishes the command

EOL = '__EOL__'

@dataclass
class EventCommandTokens():
    tokens: List[str]       # parsed tokens
    source: str             # source string
    token_idx: List[int]    # position of token in source string
    start_idx: int = 0      # position of entire command in source string. may differ from above due to prefixes

    _command_idx: int = field(default=0, init=False)   # idx of token indicating command
    _arg_idx: int = field(default=1, init=False)       # idx of first arg
    _flag_idx: int = field(default=99, init=False)     # idx of first flag

    def command(self) -> NID:
        if self.tokens:
            return self.tokens[0]

    def args(self) -> List[str]:
        return self.tokens[self._arg_idx:self._flag_idx]

    def flags(self) -> List[str]:
        return self.tokens[self._flag_idx:]

    def mode(self) -> ParseMode:
        if self._arg_idx >= len(self.tokens):
            return ParseMode.COMMAND
        elif self._flag_idx >= len(self.tokens):
            return ParseMode.ARGS
        elif self.tokens[-1] == EOL:
            return ParseMode.EOL
        else:
            return ParseMode.FLAGS
