from app.utilities.data import Prefab

from app.utilities.typing import NID

class AIGroupObject(Prefab):
    def __init__(self, nid: NID, trigger_threshold: int = 1, active: bool = False):
        self.nid = nid
        self.trigger_threshold = trigger_threshold
        self.active: bool = active
        # Cleared at the end of an AI phase
        self.triggered = set()

    def trigger(self, unit_nid: NID, num_units_in_group: int) -> bool:
        """
        # Returns whether this trigger has finally triggered the number of units required for the ai group to fire
        """
        self.triggered.add(unit_nid)
        # If we have less units than the trigger threshold, our threshold is every unit in our group
        threshold = min(self.trigger_threshold, num_units_in_group)
        return len(self.triggered) >= threshold

    def clear(self):
        self.triggered.clear()

    @classmethod
    def from_prefab(cls, prefab):
        return cls(prefab.nid, prefab.trigger_threshold)

    def save(self):
        return {'nid': self.nid,
                'trigger_threshold': self.trigger_threshold,
                'active': self.active,
                }

    @classmethod
    def restore(cls, s_dict):
        ai_group = cls(s_dict['nid'], s_dict['trigger_threshold'], s_dict['active'])
        return ai_group
