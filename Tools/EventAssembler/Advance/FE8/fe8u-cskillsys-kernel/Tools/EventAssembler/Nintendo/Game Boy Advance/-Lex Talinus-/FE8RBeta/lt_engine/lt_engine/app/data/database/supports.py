from dataclasses import dataclass

from app.utilities.data import Data, Prefab
from app.utilities import str_utils

from app.data.database.constants import Constant, ConstantCatalog, ConstantType

@dataclass
class SupportRank(Prefab):
    nid: str = None

    def save(self):
        return self.nid

    @classmethod
    def restore(cls, dat):
        return cls(dat)

class SupportRankCatalog(Data[SupportRank]):
    datatype = SupportRank

    def __init__(self, strs):
        super().__init__()
        for s in strs:
            self.append(SupportRank(s))

    def add_new_default(self, db):
        nid = str_utils.get_next_name("C", self.keys())
        new_tag = SupportRank(nid)
        self.append(new_tag)
        return new_tag

class SupportRankBonus(Prefab):
    def __init__(self, support_rank, effects):
        self.support_rank = support_rank

        self.damage = float(effects[0])
        self.resist = float(effects[1])
        self.accuracy = float(effects[2])
        self.avoid = float(effects[3])
        self.crit = float(effects[4])
        self.dodge = float(effects[5])
        self.attack_speed = float(effects[6])
        self.defense_speed = float(effects[7])

    @property
    def effects(self):
        return (self.damage, self.resist, self.accuracy, self.avoid, self.crit, self.dodge, self.attack_speed, self.defense_speed)

    @classmethod
    def default(cls):
        return cls(None, [0]*8)

    @classmethod
    def copy(cls, other):
        return cls(other.support_rank, other.effects)

class SupportRankBonusList(list):
    def contains(self, support_rank: str):
        return any(bonus.support_rank == support_rank for bonus in self)

    def swap_rank(self, old_rank: str, new_rank: str):
        for bonus in self:
            if bonus.support_rank == old_rank:
                bonus.support_rank = new_rank

    def add_new_default(self, db):
        new_support_rank_bonus = SupportRankBonus.default()
        if not db.support_ranks:
            db.support_ranks.append(SupportRank('default'))
        new_support_rank_bonus.support_rank = db.support_ranks[0].nid
        self.append(new_support_rank_bonus)
        return new_support_rank_bonus

    def move_index(self, old_index, new_index):
        if old_index == new_index:
            return
        obj = self.pop(old_index)
        self.insert(new_index, obj)

@dataclass(eq=False)
class Affinity(Prefab):
    nid: str = None
    name: str = None
    desc: str = None

    bonus: SupportRankBonusList = None

    icon_nid: str = None
    icon_index: tuple = (0, 0)

    def __repr__(self):
        return ("Affinity %s" % self.nid)

    def save_attr(self, name, value):
        if name == 'bonus':
            value = [bon.save() for bon in value]
        else:
            value = super().save_attr(name, value)
        return value

    def restore_attr(self, name, value):
        if name == 'bonus':
            if value:
                value = SupportRankBonusList([SupportRankBonus.restore(bon) for bon in value])
            else:
                value = SupportRankBonusList()
        else:
            value = super().restore_attr(name, value)
        return value

class AffinityCatalog(Data[Affinity]):
    datatype = Affinity

constants = ConstantCatalog([
    Constant('combat_convos', 'Allow support conversations in combat', ConstantType.BOOL, True),
    Constant('base_convos', 'Allow support conversations in base', ConstantType.BOOL, False),
    Constant("battle_buddy_system", "Allows characters to swap current support bonus partner in base", ConstantType.BOOL, False),
    Constant('break_supports_on_death', "Break supports on death (applies only to Permadeath)", ConstantType.BOOL, True),
    Constant('bonus_method', 'How are support bonuses calculated?', ["No Bonus", "Use Personal Affinity Bonus", "Use Partner's Affinity Bonus", "Use Average of Affinity Bonuses", "Use Sum of Affinity Bonuses"], "Use Average of Affinity Bonuses"),
    Constant('bonus_range', "How far do a support's effects reach (0 - can both attack same enemy, 99 - entire map)", ConstantType.INT, 3),
    Constant('growth_range', "How close to partner for support growth (0 - can both attack same enemy, 99 - entire map)", ConstantType.INT, 1),
    Constant('chapter_points', "Points gained on ending chapter with both on the map", ConstantType.INT, 0, 'growth'),
    Constant('end_turn_points', "Points gained on ending turn in growth range", ConstantType.INT, 1, 'growth'),
    Constant('combat_points', "Points gained on combat in growth range", ConstantType.INT, 0, 'growth'),
    Constant('interact_points', "Points gained on interacting with partner", ConstantType.INT, 0, 'growth'),
    Constant('pairup_points', "Points gained on combat with a pairup partner", ConstantType.INT, 0, 'growth'),
    Constant('bonus_ally_limit', "Limit to number of support bonuses that can be active at once (0 - No limit)", ConstantType.INT, 0, 'limit'),
    Constant("rank_limit", "Limit to number of support ranks allowed per character (0 - No limit)", ConstantType.INT, 5, 'limit'),
    Constant("highest_rank_limit", "Limit to number of highest rank supports allowed on one character (0 - No limit)", ConstantType.INT, 1, 'limit'),
    Constant("ally_limit", "Limit to number of allies that can be supported with per character (0 - No limit)", ConstantType.INT, 0, 'limit'),
    Constant("point_limit_per_chapter", "Limit to number of points gained in a chapter (0 - No limit)", ConstantType.INT, 0, 'limit'),
    Constant("rank_limit_per_chapter", "Limit to number of ranks gained in a chapter (0 - No limit)", ConstantType.INT, 1, 'limit')
])

class SupportRankRequirement(SupportRankBonus):
    def __init__(self, support_rank, requirement, gate, effects):
        self.support_rank = support_rank
        self.requirement: int = requirement
        self.gate: str = gate

        self.damage = float(effects[0])
        self.resist = float(effects[1])
        self.accuracy = float(effects[2])
        self.avoid = float(effects[3])
        self.crit = float(effects[4])
        self.dodge = float(effects[5])
        self.attack_speed = float(effects[6])
        self.defense_speed = float(effects[7])

    @classmethod
    def default(cls):
        return cls(None, 0, '', [0]*8)

    @classmethod
    def copy(cls, other):
        return cls(other.support_rank, other.requirement, other.gate, other.effects)

class SupportRankRequirementList(SupportRankBonusList):
    def add_new_default(self, db):
        new_support_rank_req = SupportRankRequirement.default()
        if db.support_ranks:
            new_support_rank_req.support_rank = db.support_ranks[0].nid
        else:
            new_support_rank_req.support_rank = None
        self.append(new_support_rank_req)
        return new_support_rank_req

@dataclass(eq=False)
class SupportPair(Prefab):
    unit1: str = None
    unit2: str = None
    one_way: bool = False

    requirements: SupportRankRequirementList = None

    @property
    def nid(self):
        u1 = '--'
        if self.unit1:
            u1 = self.unit1
        u2 = '--'
        if self.unit2:
            u2 = self.unit2
        return "%s | %s" % (u1, u2)

    def __repr__(self):
        return ("SupportPair %s" % self.nid)

    def save_attr(self, name, value):
        if name == 'requirements':
            value = [req.save() for req in value]
        else:
            value = super().save_attr(name, value)
        return value

    def restore_attr(self, name, value):
        if name == 'requirements':
            if value:
                value = SupportRankRequirementList([SupportRankRequirement.restore(req) for req in value])
            else:
                value = SupportRankRequirementList()
        else:
            value = super().restore_attr(name, value)
        return value

class SupportPairCatalog(Data[SupportPair]):
    datatype = SupportPair

    def get_pairs(self, unit1_nid: str, unit2_nid: str) -> list:
        pairs = []
        for prefab in self:
            if prefab.unit1 == unit1_nid and prefab.unit2 == unit2_nid:
                pairs.append(prefab)
            elif prefab.unit1 == unit2_nid and prefab.unit2 == unit1_nid:
                pairs.append(prefab)
        return pairs
