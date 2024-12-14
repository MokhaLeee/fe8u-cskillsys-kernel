from __future__ import annotations
from enum import Enum
import re
from typing import Dict, List, Optional, Set, Tuple

from app.events import event_commands
from app.events.event_version import EventVersion
from app.utilities.data import Data, Prefab
from app.utilities.typing import NID
from app.events.event_commands import EventCommand, GameVar, LevelVar
from app.utilities import str_utils

def get_event_version(script: str) -> EventVersion.EVENT:
    if not script.startswith('#pyev'):
        return EventVersion.EVENT
    # should always look like
    # #pyevXX
    # where XX is version number
    version = script[5:7]
    version = version.strip()
    try:
        return EventVersion(int(version))
    except:
        return EventVersion.EVENT

class EventPrefab(Prefab):
    def __init__(self, name):
        self.name = name
        self.trigger = None
        self.level_nid = None
        self.condition: str = "True"
        self.commands: List[event_commands.EventCommand] = []
        self.only_once = False
        self.priority: int = 20

        self._source: List[str] = []

    @property
    def nid(self):
        if not self.name:
            return None
        if self.level_nid:
            return self.level_nid + " " + self.name
        else:
            return "Global " + self.name

    @property
    def source(self):
        return '\n'.join(self._source)

    @source.setter
    def source(self, value: str):
        value = str_utils.convert_raw_text_newlines(value)
        self._source = value.split('\n')

    def version(self) -> EventVersion.EVENT:
        if not self._source:
            return EventVersion.EVENT
        return get_event_version(self._source[0])

    def save_attr(self, name, value):
        if name == 'commands':
            value = []
        elif name == '_source':
            if not self._source and self.commands:
                self._source = [str(cmd) for cmd in self.commands]
            value = self._source
        else:
            value = super().save_attr(name, value)
        return value

    def restore_attr(self, name, value):
        if name == 'priority':
            if value is None:
                value = 20
            else:
                value = super().restore_attr(name, value)
        elif name == 'commands':
            value = [event_commands.restore_command(c) for c in value]
            value = [v for v in value if v]
        else:
            value = super().restore_attr(name, value)
        return value

    @classmethod
    def default(cls):
        return cls('None')

class EventCatalog(Data[EventPrefab]):
    datatype = EventPrefab

    def __init__(self, vals: List[EventPrefab] | None = None):
        super().__init__(vals)
        self.inspector = EventInspectorEngine(self)

    def get(self, trigger_nid, level_nid):
        return [event for event in self._list if event.trigger == trigger_nid and
                (not event.level_nid or event.level_nid == level_nid)]

    def get_by_level(self, level_nid: Optional[NID]) -> List[EventPrefab]:
        return [event for event in self._list if (not event.level_nid or not level_nid or event.level_nid == level_nid)]

    def get_by_nid_or_name(self, name_or_nid: str, level_nid=None) -> List[EventPrefab]:
        level_events = self.get_by_level(level_nid)
        return [event for event in level_events if
                ((event.nid == name_or_nid) or (event.name == name_or_nid))]

    def get_from_nid(self, key, fallback=None):
        return self._dict.get(key, fallback)

    def create_new(self, db, level_nid=None):
        other_names = [d.name for d in self if d.level_nid == level_nid]
        name = str_utils.get_next_name("New Event", other_names)
        new_event = EventPrefab(name)
        new_event.level_nid = level_nid
        self.append(new_event)
        return new_event

class EventInspectorEngine():
    def __init__(self, event_db: EventCatalog):
        self.event_db = event_db
        self.parsed: Dict[NID, List[EventCommand]] = {}

    def clear_cache(self, event_nid):
        if event_nid in self.parsed:
            del self.parsed[event_nid]

    def get_commands(self, event_nid) -> List[EventCommand]:
        if event_nid not in self.parsed:
            event = self.event_db.get_by_nid_or_name(event_nid)[0]
            if event:
                # TODO(mag/rainlash) How do we find which commands are used in Python source?
                if event.version() != EventVersion.EVENT:
                    return []
                else:
                    parsed_commands = event_commands.parse_script_to_commands(event.source)
                    self.parsed[event_nid] = parsed_commands
            else:
                return []
        return self.parsed[event_nid]

    def find_all_variables_in_level(self, level_nid: Optional[NID]) -> Set[NID]:
        """returns all known user-defined symbols in level."""
        all_vars = set()
        for event in self.event_db.get_by_level(level_nid):
            if event.level_nid == level_nid:
                for command in self.get_commands(event.nid):
                    if command.nid in [GameVar.nid, LevelVar.nid]:
                        all_vars.add(command.parameters['Nid'])
        return all_vars

    def find_all_occurrences_of_symbol(self, symbol: str) -> Set[NID]:
        occurrence_dict: Set[NID] = set()
        # regex explanation: match every occurrence of symbol within brackets, but not surrounded by
        # possible interfering chars that alter the symbol
        # e.g. {v:symbol} is probably an occurrence of the symbol.
        # {eval:game.get_var("symbol")} is also an occurence of the symbol. Both match
        # but {v:symbol_version_2} will not match, and neither will {eval:game.get_var("symbol2")}
        rstr = '.*{{.*(?![^0-9A-Za-z_]){symbol}(?![0-9A-Za-z_]).*}}.*'.format(symbol=symbol)
        for event in self.event_db:
            for command in self.get_commands(event.nid):
                for parameter in command.parameters.values():
                    # Can sometimes be not a string, in which case we don't want to mess with it
                    if isinstance(parameter, str) and (parameter == symbol or re.match(rstr, parameter)):
                        occurrence_dict.add(event.nid)
                        break
                if event.nid in occurrence_dict:
                    break
        return occurrence_dict

    def find_all_calls_of_command(self, qcommand: EventCommand, level_nid='all') -> Dict[Tuple[str, int], EventCommand]:
        all_commands = {}
        for event in self.event_db:
            if level_nid == 'all' or event.level_nid == level_nid:
                for idx, command in enumerate(self.get_commands(event.nid)):
                    if command.nid == qcommand.nid:
                        all_commands[(event.nid, idx)] = command
        return all_commands
