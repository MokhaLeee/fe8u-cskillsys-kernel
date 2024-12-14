from dataclasses import dataclass

from app.utilities.data import Data, Prefab
from app.utilities import str_utils

# === Can get bonuses to combat statistics based on weapon_type and weapon_rank
class CombatBonus(Prefab):
    modification = 1

    def __init__(self, weapon_type, weapon_rank, effects):
        self.weapon_type = weapon_type
        self.weapon_rank = weapon_rank

        self.damage = int(effects[0])
        self.resist = int(effects[1])
        self.accuracy = int(effects[2])
        self.avoid = int(effects[3])
        self.crit = int(effects[4])
        self.dodge = int(effects[5])
        self.attack_speed = int(effects[6])
        self.defense_speed = int(effects[7])

    @property
    def effects(self):
        return (self.damage, self.resist, self.accuracy, self.avoid, self.crit, self.dodge, self.attack_speed, self.defense_speed)

    @classmethod
    def default(cls):
        return cls(None, None, [0]*8)

    @classmethod
    def copy(cls, other):
        return cls(other.weapon_type, other.weapon_rank, other.effects)

    def modify(self, w_mod):
        self.modification *= w_mod

        self.damage *= w_mod
        self.resist *= w_mod
        self.accuracy *= w_mod
        self.avoid *= w_mod
        self.crit *= w_mod
        self.dodge *= w_mod
        self.attack_speed *= w_mod
        self.defense_speed *= w_mod

class CombatBonusList(list):
    def contains(self, weapon_type: str):
        return any(bonus.weapon_type == weapon_type for bonus in self)

    def swap_type(self, old_weapon_type: str, new_weapon_type: str):
        for bonus in self:
            if bonus.weapon_type == old_weapon_type:
                bonus.weapon_type = new_weapon_type

    def swap_rank(self, old_rank: str, new_rank: str):
        for bonus in self:
            if bonus.weapon_rank == old_rank:
                bonus.weapon_rank = new_rank

    def add_new_default(self, db):
        new_combat_bonus = CombatBonus.default()
        new_combat_bonus.weapon_type = db.weapons[0].nid
        new_combat_bonus.weapon_rank = "All"
        self.append(new_combat_bonus)
        return new_combat_bonus

    def move_index(self, old_index, new_index):
        if old_index == new_index:
            return
        obj = self.pop(old_index)
        self.insert(new_index, obj)

# === WEAPON RANK ===
@dataclass
class WeaponRank(Prefab):
    rank: str = None
    requirement: int = 1

    @property
    def nid(self):
        return self.rank

    @nid.setter
    def nid(self, value):
        self.rank = value

    def __repr__(self):
        return "WeaponRank %s: %d" % \
            (self.rank, self.requirement)

class RankCatalog(Data[WeaponRank]):
    datatype = WeaponRank

    def get_rank_from_wexp(self, wexp) -> WeaponRank:
        ranks = sorted(self._list, key=lambda x: x.requirement)
        correct_rank = None
        for rank in ranks:
            if wexp >= rank.requirement:
                correct_rank = rank
        return correct_rank

    def get_next_rank_from_wexp(self, wexp) -> WeaponRank:
        ranks = sorted(self._list, key=lambda x: x.requirement)
        correct_rank = None
        for rank in ranks:
            if wexp < rank.requirement:
                correct_rank = rank
                return correct_rank
        return correct_rank

    def get_highest_rank(self) -> WeaponRank:
        ranks = sorted(self._list, key=lambda x: x.requirement)
        return ranks[-1]


# === WEAPON TYPE ===
@dataclass(eq=False)
class WeaponType(Prefab):
    nid: str = None
    name: str = None

    force_melee_anim: bool = False

    rank_bonus: CombatBonusList = None
    advantage: CombatBonusList = None
    disadvantage: CombatBonusList = None

    icon_nid: str = None
    icon_index: tuple = (0, 0)

    def __repr__(self):
        return ("WeaponType %s" % self.nid)

    def save_attr(self, name, value):
        if name in ('rank_bonus', 'advantage', 'disadvantage'):
            value = [adv.save() for adv in value]
        else:
            value = super().save_attr(name, value)
        return value

    def restore_attr(self, name, value):
        if name in ('rank_bonus', 'advantage', 'disadvantage'):
            if value:
                value = CombatBonusList([CombatBonus.restore(adv) for adv in value])
            else:
                value = CombatBonusList()
        else:
            value = super().restore_attr(name, value)
        return value

class WeaponCatalog(Data[WeaponType]):
    datatype = WeaponType

    def default(self, db):
        return WexpGain(False, 0)

    def create_new(self, db):
        nids = [d.nid for d in self]
        nid = name = str_utils.get_next_name("New Weapon Type", nids)
        new_weapon = WeaponType(
            nid, name, False, CombatBonusList(),
            CombatBonusList(), CombatBonusList())
        self.append(new_weapon)
        return new_weapon

# === WEAPON EXPERIENCE GAINED ===
class WexpGain():
    def __init__(self, usable: bool, wexp_gain: int, cap: int = None):
        self.usable = usable  # Used only by Klass Objects
        self.wexp_gain = wexp_gain
        self.cap = cap  # Used only by Klass Objects -- when None, default to highest weapon rank

    def save(self):
        return (self.usable, self.wexp_gain, self.cap)
    
    @classmethod
    def restore(cls, s_tuple):
        return cls(*s_tuple)
