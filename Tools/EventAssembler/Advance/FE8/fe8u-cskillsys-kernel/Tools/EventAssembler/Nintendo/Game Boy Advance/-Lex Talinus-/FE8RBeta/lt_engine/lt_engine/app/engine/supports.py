from dataclasses import dataclass
from typing import Dict

from app.data.database.database import DB

from app.utilities import utils

from app.engine import action
from app.engine.game_state import game

import logging

from app.utilities.typing import NID

class SupportPair():
    """
    Keeps track of necessary values for each support pair
    """
    def __init__(self, nid):
        self.nid = nid
        self.points = 0
        self.locked_ranks = []
        self.unlocked_ranks = []
        self.points_gained_this_chapter = 0
        self.ranks_gained_this_chapter = 0

    @property
    def unit1(self):
        return self.nid.split(' | ')[0]

    @property
    def unit2(self):
        return self.nid.split(' | ')[1]

    def increment_points(self, inc):
        point_limit = DB.support_constants.value('point_limit_per_chapter')
        rank_limit = DB.support_constants.value('rank_limit_per_chapter')
        support_prefab = DB.support_pairs.get(self.nid)
        while inc > 0 and not self.locked_ranks and \
                (not point_limit or self.points_gained_this_chapter < point_limit) and \
                (not rank_limit or self.ranks_gained_this_chapter < rank_limit):
            reqs = support_prefab.requirements
            for bonus in reversed(reqs):
                if self.points < bonus.requirement and self.points + 1 >= bonus.requirement:
                    self.ranks_gained_this_chapter += 1
                    self.locked_ranks.append(bonus.support_rank)
            inc -= 1
            self.points += 1
            self.points_gained_this_chapter += 1

    def reset(self):
        self.points_gained_this_chapter = 0
        self.ranks_gained_this_chapter = 0

    def can_support(self) -> bool:
        support_prefab = DB.support_pairs.get(self.nid)
        reqs = support_prefab.requirements
        if self.locked_ranks and game.supports.check_rank_limit(self):
            for rank in self.locked_ranks:
                for bonus in reqs:
                    if bonus.support_rank == rank and not bonus.gate or game.game_vars.get(bonus.gate):
                        return True
        return False

    def save(self):
        s_dict = {}
        s_dict['nid'] = self.nid
        s_dict['points'] = self.points
        s_dict['locked_ranks'] = self.locked_ranks[:]
        s_dict['unlocked_ranks'] = self.unlocked_ranks[:]
        s_dict['points_gained_this_chapter'] = self.points_gained_this_chapter
        s_dict['ranks_gained_this_chapter'] = self.ranks_gained_this_chapter
        return s_dict

    @classmethod
    def restore(cls, s_dict):
        obj = cls(s_dict['nid'])
        obj.points = int(s_dict['points'])
        obj.locked_ranks = s_dict['locked_ranks']
        obj.unlocked_ranks = s_dict['unlocked_ranks']
        obj.points_gained_this_chapter = int(s_dict['points_gained_this_chapter'])
        obj.ranks_gained_this_chapter = int(s_dict['ranks_gained_this_chapter'])
        return obj

@dataclass
class SupportEffect():
    damage: float = 0
    resist: float = 0
    accuracy: float = 0
    avoid: float = 0
    crit: float = 0
    dodge: float = 0
    attack_speed: float = 0
    defense_speed: float = 0

    def add_effect(self, effect: list):
        self.damage += effect[0]
        self.resist += effect[1]
        self.accuracy += effect[2]
        self.avoid += effect[3]
        self.crit += effect[4]
        self.dodge += effect[5]
        self.attack_speed += effect[6]
        self.defense_speed += effect[7]


class SupportController():
    def __init__(self):
        self.support_pairs: Dict[NID, SupportPair] = {}

    def get(self, unit1_nid: str, unit2_nid: str) -> SupportPair:
        nid = "%s | %s" % (unit1_nid, unit2_nid)
        if nid in self.support_pairs:
            return self.support_pairs.get(nid)
        # Try the other way round
        nid = "%s | %s" % (unit2_nid, unit1_nid)
        return self.support_pairs.get(nid)

    def create_pair(self, nid) -> SupportPair:
        prefab = DB.support_pairs.get(nid)
        if not prefab:
            return
        if prefab.nid in self.support_pairs:
            return self.support_pairs[prefab.nid]

        new_support_pair = SupportPair(nid)
        self.support_pairs[nid] = new_support_pair
        return new_support_pair

    def save(self):
        return [support_pair.save() for support_pair in self.support_pairs.values()]

    @classmethod
    def restore(cls, s_list):
        self = cls()
        for support_pair_dat in s_list:
            support_pair = SupportPair.restore(support_pair_dat)
            self.support_pairs[support_pair.nid] = support_pair
        return self

    def get_pairs(self, unit_nid: str) -> list:
        pairs = []
        for prefab in DB.support_pairs:
            if prefab.unit1 == unit_nid or prefab.unit2 == unit_nid:
                if prefab.nid not in self.support_pairs:
                    self.create_pair(prefab.nid)
                pairs.append(self.support_pairs[prefab.nid])
        return pairs

    def get_bonus_pairs(self, unit_nid: str) -> list:
        """
        Only gets the pairs that could conceivably give out a support bonus
        """
        pairs = []
        for key, pair in self.support_pairs.items():
            prefab = DB.support_pairs.get(key)
            if not prefab:  # In case you somehow deleted a pair!
                logging.warning("Support Pair with key %s no longer exists in database! Skipping..." % key)
                continue
            if prefab.unit1 == unit_nid or (prefab.unit2 == unit_nid and not prefab.one_way):
                pairs.append(pair)
        return pairs

    def check_rank_limit(self, support_pair: SupportPair) -> bool:
        """
        Return False if either of the units is already at their limit
        """
        rank_limit = DB.support_constants.value('rank_limit')
        highest_rank_limit = DB.support_constants.value('highest_rank_limit')
        rank1 = game.supports.get_num_ranks(support_pair.unit1)
        rank2 = game.supports.get_num_ranks(support_pair.unit2)
        highest_rank1 = game.supports.get_num_highest_ranks(support_pair.unit1)
        highest_rank2 = game.supports.get_num_highest_ranks(support_pair.unit2)
        if rank_limit and (rank1 >= rank_limit or rank2 >= rank_limit):
            return False
        if highest_rank_limit and (highest_rank1 >= highest_rank_limit or highest_rank2 >= highest_rank_limit):
            return False
        return True

    # Next three functions used for limits
    def get_num_ranks(self, unit_nid: str) -> int:
        total = 0
        pairs = self.get_pairs(unit_nid)
        for pair in pairs:
            total += len(pair.unlocked_ranks)
        return total

    def get_num_highest_ranks(self, unit_nid: str) -> int:
        highest_rank = DB.support_ranks[-1]
        return len([pair for pair in self.get_pairs(unit_nid) if highest_rank in pair.unlocked_ranks])

    def get_num_allies(self, unit_nid: str) -> int:
        return len([pair for pair in self.get_pairs(unit_nid) if pair.unlocked_ranks])

    def check_bonus_range(self, unit1, unit2) -> bool:
        return self.check_range(unit1, unit2, 'bonus_range')

    def check_growth_range(self, unit1, unit2) -> bool:
        return self.check_range(unit1, unit2, 'growth_range')

    def check_range(self, unit1, unit2, constant) -> bool:
        if not unit1.position or not unit2.position:
            return False
        r = DB.support_constants.value(constant)
        if r == 99:
            return True
        elif r == 0:
            return False
        else:
            dist = utils.calculate_distance(unit1.position, unit2.position)
            return dist <= r

    def get_specific_bonus(self, unit1, unit2, highest_rank):
        for pair in DB.support_pairs:
            if (pair.unit1 == unit1.nid and pair.unit2 == unit2.nid) or (pair.unit1 == unit2.nid and pair.unit2 == unit1.nid):
                for support_rank_req in pair.requirements:
                    if support_rank_req.support_rank == highest_rank:
                        return support_rank_req
        return None

    def get_bonus(self, unit1, unit2, highest_rank) -> SupportEffect:
        # Get bonus to stats from affinity
        aff1_nid = unit1.affinity
        aff2_nid = unit2.affinity
        aff1 = DB.affinities.get(aff1_nid)
        aff2 = DB.affinities.get(aff2_nid)

        # Check affinities exist before proceeding
        if not aff1:
            logging.error("%s is not a valid affinity", aff1_nid)
            return SupportEffect()
        if not aff2:
            logging.error("%s is not a valid affinity", aff2_nid)
            return SupportEffect()

        aff1_bonus = None
        aff2_bonus = None
        for support_rank_bonus in aff1.bonus:
            if support_rank_bonus.support_rank == highest_rank:
                aff1_bonus = support_rank_bonus
                break
        for support_rank_bonus in aff2.bonus:
            if support_rank_bonus.support_rank == highest_rank:
                aff2_bonus = support_rank_bonus
                break

        # Get bonus to stats from specific pairing
        specific_bonus = self.get_specific_bonus(unit1, unit2, highest_rank)

        # Build final bonus
        final_bonus = SupportEffect()

        method = DB.support_constants.value('bonus_method')
        if method == 'Use Personal Affinity Bonus':
            if aff1_bonus:
                final_bonus.add_effect(aff1_bonus.effects)
        elif method == "Use Partner's Affinity Bonus":
            if aff2_bonus:
                final_bonus.add_effect(aff2_bonus.effects)
        elif method == 'Use Average of Affinity Bonuses':
            if aff1_bonus and aff2_bonus:
                final_bonus.add_effect([_/2. for _ in aff1_bonus.effects])
                final_bonus.add_effect([_/2. for _ in aff2_bonus.effects])
        elif method == 'Use Sum of Affinity Bonuses':
            if aff1_bonus and aff2_bonus:
                final_bonus.add_effect(aff1_bonus.effects)
                final_bonus.add_effect(aff2_bonus.effects)

        if specific_bonus:
            final_bonus.add_effect(specific_bonus.effects)

        return final_bonus

    def lock_on_death(self, unit):
        """
        Locks all support ranks when one of the units in a pair dies
        """
        pairs = self.get_pairs(unit.nid)
        for pair in pairs:
            action.do(action.LockAllSupportRanks(pair.nid))

def increment_end_chapter_supports():
    if not game.game_vars.get('_supports'):
        return
    inc = DB.support_constants.value('chapter_points')
    if inc:
        units = [unit for unit in game.units if unit.position and not unit.generic]
        for unit in units[:]:
            if unit.traveler and not game.get_unit(unit.traveler).generic:
                units.append(game.get_unit(unit.traveler))

        unit_nids = {unit.nid for unit in units}
        for support_prefab in DB.support_pairs:
            if support_prefab.unit1 in unit_nids and support_prefab.unit2 in unit_nids:
                action.do(action.IncrementSupportPoints(support_prefab.nid, inc))

    # Reset max number of support points and rank that can be gotten in one chapter
    for pair in game.supports.support_pairs.values():
        pair.reset()

def increment_unit_end_turn_supports(unit):
    if not game.game_vars.get('_supports'):
        return
    if not unit.position:
        return
    inc = DB.support_constants.value('end_turn_points')
    if inc:
        dist = DB.support_constants.value('growth_range')
        units = [u for u in game.units if u.position and not u.generic and u.team == unit.team and u is not unit]
        unit_nids = {unit.nid for unit in units}
        for support_prefab in DB.support_pairs:
            if (support_prefab.unit1 in unit_nids and support_prefab.unit2 == unit.nid) or \
               (support_prefab.unit2 in unit_nids and support_prefab.unit1 == unit.nid):
                unit1 = game.get_unit(support_prefab.unit1)
                unit2 = game.get_unit(support_prefab.unit2)
                if dist == 99 or utils.calculate_distance(unit1.position, unit2.position) <= dist:
                    action.do(action.IncrementSupportPoints(support_prefab.nid, inc))

def increment_team_end_turn_supports(team='player'):
    if not game.game_vars.get('_supports'):
        return
    inc = DB.support_constants.value('end_turn_points')
    if inc:
        dist = DB.support_constants.value('growth_range')
        units = [unit for unit in game.units if unit.position and not unit.generic and unit.team == team]
        unit_nids = {unit.nid for unit in units}
        for support_prefab in DB.support_pairs:
            if support_prefab.unit1 in unit_nids and support_prefab.unit2 in unit_nids:
                unit1 = game.get_unit(support_prefab.unit1)
                unit2 = game.get_unit(support_prefab.unit2)
                if dist == 99 or utils.calculate_distance(unit1.position, unit2.position) <= dist:
                    action.do(action.IncrementSupportPoints(support_prefab.nid, inc))

def increment_end_combat_supports(combatant, target=None) -> list:
    """
    Returns a list of tuples, containing the units that gained support points together
    """
    if not game.game_vars.get('_supports'):
        return []
    if not combatant.position:
        return []
    inc = DB.support_constants.value('combat_points')
    pairs = []
    if inc:
        dist = DB.support_constants.value('growth_range')
        units = [unit for unit in game.units if unit.position and not unit.generic and unit.team == combatant.team and unit is not combatant]
        unit_nids = {unit.nid for unit in units}
        for support_prefab in DB.support_pairs:
            other_unit = None
            if support_prefab.unit1 == combatant.nid and support_prefab.unit2 in unit_nids:
                other_unit = game.get_unit(support_prefab.unit2)
            elif support_prefab.unit2 == combatant.nid and support_prefab.unit1 in unit_nids:
                other_unit = game.get_unit(support_prefab.unit1)
            if not other_unit:
                continue
            assert other_unit.position is not None
            if dist == 0 and target:
                if target.position in game.target_system.get_attackable_positions(other_unit, force=True):
                    action.do(action.IncrementSupportPoints(support_prefab.nid, inc))
                    pairs.append((combatant, other_unit))
            elif dist == 99 or utils.calculate_distance(combatant.position, other_unit.position) <= dist:
                action.do(action.IncrementSupportPoints(support_prefab.nid, inc))
                pairs.append((combatant, other_unit))
    return pairs

def increment_supports(combatant, partner, constant: str) -> bool:
    """
    Returns whether the combatant and partner are successful in incrementing their support
    """
    if not game.game_vars.get('_supports'):
        return False
    inc = DB.support_constants.value(constant)
    success: bool = False
    if inc:
        for support_prefab in DB.support_pairs:
            if (support_prefab.unit1 == combatant.nid and support_prefab.unit2 == partner.nid) or \
                    (support_prefab.unit2 == combatant.nid and support_prefab.unit1 == partner.nid):
                action.do(action.IncrementSupportPoints(support_prefab.nid, inc))
                success = True
    return success

def increment_interact_supports(combatant, target) -> bool:
    return increment_supports(combatant, target, 'interact_points')

def increment_pairup_supports(combatant, partner) -> bool:
    return increment_supports(combatant, partner, 'pairup_points')
