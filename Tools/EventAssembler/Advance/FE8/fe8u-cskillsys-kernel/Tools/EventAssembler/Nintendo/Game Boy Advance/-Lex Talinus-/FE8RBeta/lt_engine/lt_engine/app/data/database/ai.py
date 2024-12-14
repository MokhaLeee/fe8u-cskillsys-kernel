from app.utilities.data import Data, Prefab
from app.utilities import str_utils

AI_ActionTypes = ['None', 'Attack', 'Support', 'Steal', 'Interact', 'Move_to', 'Move_away_from', 'Wait']
AI_TargetTypes = ['None', 'Enemy', 'Ally', 'Unit', 'Position', 'Event', 'Time']
unit_spec = ['All', 'Class', 'Tag', 'Name', 'Team', 'Faction', 'Party', 'ID']
# View Range
# (Don't look | Movement*2 + Maximum Item Range | Entire Map | Custom Range (Integer))

class AIPrefab(Prefab):
    def __init__(self, nid, priority, offense_bias=2):
        self.nid = nid
        self.behaviours = [AIBehaviour.DoNothing(), AIBehaviour.DoNothing(), AIBehaviour.DoNothing()]
        self.priority: int = priority
        self.offense_bias: float = offense_bias
        self.roam_ai = False

    def add_behaviour(self, behaviour):
        self.behaviours.append(behaviour)

    def add_default(self):
        self.behaviours.append(AIBehaviour.DoNothing())

    def pop_behaviour(self):
        self.behaviours.pop()

    def set_behaviour(self, idx, behaviour):
        self.behaviours[idx] = behaviour

    def save_attr(self, name, value):
        if name == 'behaviours':
            value = [b.save() for b in value]
        else:
            value = super().save_attr(name, value)
        return value

    def restore_attr(self, name, value):
        if name == 'behaviours':
            value = [AIBehaviour.restore(b) for b in value]
        elif name == 'offense_bias':
            if value is None:
                value = 2.
        else:
            value = super().restore_attr(name, value)
        return value

    @classmethod
    def default(cls):
        return cls(None, 0)

    def has_unit_spec(self, spec_type, spec_nid) -> bool:
        for behaviour in self.behaviours:
            if behaviour.has_unit_spec(spec_type, spec_nid):
                return True
        return False

    def change_unit_spec(self, spec_type, old_nid, new_nid):
        for behaviour in self.behaviours:
            behaviour.change_unit_spec(spec_type, old_nid, new_nid)

    def guard_ai(self) -> bool:
        # Determines whether this AI will ever move
        # Used in game.boundary for graphics
        if all(behaviour.action == "None" for behaviour in self.behaviours):
            return False
        for behaviour in self.behaviours:
            if behaviour.action == "None":
                continue
            elif not behaviour.guard_ai():
                return False
        return True

class AIBehaviour(Prefab):
    def __init__(self, action: str, target, view_range: int, target_spec=None, speed=100, desired_proximity=0, condition=''):
        self.action: str = action
        self.target = target
        self.target_spec = target_spec
        self.view_range: int = view_range
        self.invert_targeting: bool = False
        self.roam_speed = speed
        self.desired_proximity = desired_proximity
        self.condition: str = condition

    @classmethod
    def DoNothing(cls):
        return cls('None', 'None', 0)

    @classmethod
    def default(cls):
        return cls.DoNothing()

    def has_unit_spec(self, spec_type, spec_nid):
        if self.target in ('Enemy', 'Ally', 'Unit'):
            if self.target_spec and self.target_spec[0] == spec_type and self.target_spec[1] == spec_nid:
                return True
        return False

    def change_unit_spec(self, spec_type, old_nid, new_nid):
        if self.target in ('Enemy', 'Ally', 'Unit'):
            if self.target_spec and self.target_spec[0] == spec_type and self.target_spec[1] == old_nid:
                self.target_spec[1] = new_nid

    def guard_ai(self):
        return self.view_range == -1

class AICatalog(Data[AIPrefab]):
    datatype = AIPrefab

    def create_new(self, db):
        nids = [d.nid for d in self]
        nid = str_utils.get_next_name("New AI", nids)
        new_ai = AIPrefab(nid, 20)
        self.append(new_ai)
        return new_ai
