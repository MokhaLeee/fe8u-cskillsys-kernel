from typing import Dict, List
from app.data.database.database import DB
from app.data.database.levels import LevelCatalog
from app.events.event_commands import EventCommand, Speak
from app.events.event_prefab import EventCatalog
from app.utilities.str_utils import SHIFT_NEWLINE
from app.utilities.typing import NID
from collections import defaultdict
def dump_script(event_db: EventCatalog, level_db: LevelCatalog=None) -> Dict[NID, Dict[NID, str]]:
    """Dumps the entire script in text form.

    Returns:
        Dict[NID, str]: a map of event names to scripts
    """
    if not event_db._list:
        return ""
    if level_db:
        level_order = level_db.keys()
    else:
        level_order = ['all']
    inspector = DB.events.inspector
    ret = {}
    for level in level_order:
        level_event_ret = {}
        level_event_dict: Dict[NID, List[Speak]] = defaultdict(list)
        all_events_in_level = inspector.find_all_calls_of_command(Speak(), level)
        all_speak_calls_in_level = sorted(all_events_in_level.keys())
        for event_nid, event_line in all_speak_calls_in_level:
            level_event_dict[event_nid].append(all_events_in_level[(event_nid, event_line)])
        for event_nid, event_commands in level_event_dict.items():
            event_str = ""
            event_str += "Event: %s\n\n" % event_nid
            for line in event_commands:
                speaker = line.parameters['Speaker']
                text = line.parameters['Text']
                if not speaker:
                    speaker = line.parameters.get('StyleNid', "")
                if text.startswith(SHIFT_NEWLINE):
                    text = text[1:]
                text = text.replace("{w}", '').replace("|", "\n\t").replace("{br}", "\n\t").replace(SHIFT_NEWLINE, '\n\t').replace("{clear}", "")
                event_str += "%s: %s\n" % (speaker, text)
            escaped_event_nid_filename = ''.join([c for c in event_nid if c.isalnum()])
            level_event_ret[escaped_event_nid_filename] = event_str
        ret[level] = level_event_ret
    return ret
