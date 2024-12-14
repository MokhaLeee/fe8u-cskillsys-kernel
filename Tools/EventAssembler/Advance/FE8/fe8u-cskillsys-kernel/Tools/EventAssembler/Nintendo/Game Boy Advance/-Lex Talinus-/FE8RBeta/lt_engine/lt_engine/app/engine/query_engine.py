from __future__ import annotations

import logging
from typing import TYPE_CHECKING, Any, List, Optional, Tuple

if TYPE_CHECKING:
    from app.data.database.klass import Klass
    from app.engine.game_state import GameState
    from app.engine.objects.item import ItemObject
    from app.engine.objects.region import RegionObject
    from app.engine.objects.skill import SkillObject
    from app.engine.objects.unit import UnitObject
    from app.utilities.typing import NID

from app.data.database.database import DB
from app.utilities import utils

class QueryType():
    UNIT = 'Units'
    SKILL = 'Skills'
    ITEM = 'Items'
    MAP = 'Map Functions'
    ACHIEVEMENT = 'Achievements'
    VARIABLES = 'VARIABLES'
    CLASS = 'Class'

def categorize(tag):
    def deco(func):
        func.tag = tag
        return func
    return deco

class GameQueryEngine():
    def __init__(self, logger: logging.Logger, game: GameState) -> None:
        self.logger = logger
        self.game = game
        query_funcs = [funcname for funcname in dir(self) if not funcname.startswith('_')]
        self.func_dict = {funcname: getattr(self, funcname) for funcname in query_funcs}

    def _resolve_to_nid(self, obj_or_nid) -> NID:
        try:
            return obj_or_nid.uid
        except:
            try:
                return obj_or_nid.nid
            except:
                return obj_or_nid

    def _resolve_to_unit(self, unit_or_nid) -> Optional[UnitObject]:
        nid = self._resolve_to_nid(unit_or_nid)
        return self.game.get_unit(nid)

    def _resolve_to_region(self, region_or_nid) -> Optional[RegionObject]:
        nid = self._resolve_to_nid(region_or_nid)
        return self.game.get_region(nid)

    def _resolve_pos(self, has_pos_or_is_pos) -> Optional[Tuple[int, int]]:
        try:
            # possibly a unit?
            a_unit = self._resolve_to_unit(has_pos_or_is_pos)
            if a_unit:
                return a_unit.position
            else:
                return has_pos_or_is_pos
        except:
            return has_pos_or_is_pos

    @categorize(QueryType.ITEM)
    def get_item(self, unit, item) -> Optional[ItemObject]:
        """Returns a item object by nid.

        Args:
            unit: unit to check
            item: item to check

        Returns:
            Optional[ItemObject]: Item if exists on unit, otherwise None
        """
        item = self._resolve_to_nid(item)
        found_items = []
        if unit == 'convoy':
            found_items = [it for it in self.game.get_convoy_inventory() if it.uid == item or it.nid == item]
        else:
            unit = self._resolve_to_unit(unit)
            if unit:
                found_items = [it for it in unit.items if it.uid == item or it.nid == item]
        if found_items:
            return found_items[0]
        return None

    @categorize(QueryType.ITEM)
    def get_subitem(self, unit, parent_item, child_item) -> Optional[ItemObject]:
        """Returns a item object by nid.

        Args:
            unit: unit to check
            parent_item: parent item (multi-item) to check
            child_item: child item (subitem) to check

        Returns:
            Optional[ItemObject]: Item if exists on unit, otherwise None
        """
        parent_item = self._resolve_to_nid(parent_item)
        child_item = self._resolve_to_nid(child_item)
        found_items = []
        if unit == 'convoy':
            found_items = []
            possible_parent_items = [it for it in self.game.get_convoy_inventory() if it.uid == parent_item or it.nid == parent_item]
            for item in possible_parent_items:
                found_items += [it for it in item.subitems if it.uid == child_item or it.nid == child_item]
        else:
            unit = self._resolve_to_unit(unit)
            if unit:
                found_items = []
                possible_parent_items = [it for it in unit.items if it.uid == parent_item or it.nid == parent_item]
                for item in possible_parent_items:
                    found_items += [it for it in item.subitems if it.uid == child_item or it.nid == child_item]
        if found_items:
            return found_items[0]
        return None

    @categorize(QueryType.ITEM)
    def has_item(self, item, nid=None, team=None, tag=None, party=None) -> bool:
        """Check if any unit matching criteria has item.

Example usage:

* `has_item("Iron Sword", team="player")` will check if any player unit is holding an iron sword
* `has_item("Sacred Stone", party='Eirika')` will check if Eirika's party has the item "Sacred Stone"

        Args:
            item: item to check
            nid (optional): use to check specific unit nid
            team (optional): used to match for team.
            tag (optional): used to match for tag.
            party (optional): used to match for party

        Returns:
            bool: True if unit has item, else False
        """
        all_units = self.game.get_all_units(False) if not party else self.game.get_all_units_in_party(party)
        convoy: List[ItemObject] = []
        item = self._resolve_to_nid(item)
        if not item:
            return False
        if not nid or nid == 'convoy':
            if nid == 'convoy' or team == 'player':
                convoy = self.game.get_convoy_inventory()
            elif party:
                convoy = self.game.get_convoy_inventory(self.game.get_party(party))
        if convoy and any([citem.nid == item or citem.uid == item for citem in convoy]):
            return True
        for unit in all_units:
            if nid and not nid == unit.nid:
                continue
            if team and not team == unit.team:
                continue
            if tag and not tag in unit.tags:
                continue
            if bool(self.get_item(unit, item)):
                return True
        return False

    @categorize(QueryType.SKILL)
    def get_skill(self, unit, skill) -> Optional[SkillObject]:
        """Returns a skill object by nid.

        Args:
            unit: unit in question
            skill: nid of skill

        Returns:
            Optional[SkillObject] | None: Skill, if exists on unit, else None.
        """
        unit = self._resolve_to_unit(unit)
        skill = self._resolve_to_nid(skill)
        if unit:
            for sk in reversed(unit.all_skills):
                if sk.nid == skill:
                    return sk
        return None

    @categorize(QueryType.SKILL)
    def has_skill(self, unit, skill) -> bool:
        """checks if unit has skill

        Args:
            unit: unit to check
            skill: skill to check

        Returns:
            bool: True if unit has skill, else false
        """
        return bool(self.get_skill(unit, skill))

    @categorize(QueryType.CLASS)
    def get_klass(self, unit) -> Optional[Klass]:
        """Returns the klass prefab of the unit.

        Args:
            unit: unit in question

        Returns:
            Klass object if the unit exists and has a valid klass, otherwise None
        """
        unit = self._resolve_to_unit(unit)
        if unit:
            klass = DB.classes.get(unit.klass)
            return klass
        return None
    # Gives get_klass an alternate name
    get_class = get_klass

    @categorize(QueryType.MAP)
    def get_closest_allies(self, position, num: int = 1) -> List[Tuple[UnitObject, int]]:
        """Return a list containing the closest player units and their distances.

        Args:
            position: position or unit
            num (int, optional): How many allies to search for. Defaults to 1.

        Returns:
            List[Tuple[UnitObject, int]]: Returns `num` pairs of `(unit, distance)` to the position.
            Will return fewer if there are fewer player units than `num`.
        """
        position = self._resolve_pos(position)
        if position:
            return sorted([(unit, utils.calculate_distance(unit.position, position)) for unit in self.game.get_player_units()],
                            key=lambda pair: pair[1])[:num]
        return []

    @categorize(QueryType.MAP)
    def get_units_within_distance(self, position, dist: int = 1, nid=None, team=None, tag=None, party=None) -> List[Tuple[UnitObject, int]]:
        """Return a list containing all units within `dist` distance to the specific position
        that match specific criteria

        Args:
            position: position or unit
            dist (int, optional): How far to search. Defaults to 1.
            nid (optional): use to check specific unit nid
            team (optional): used to match for team.
            tag (optional): used to match for tag.
            party (optional): used to match for party

        Returns:
            List[Tuple[UnitObject, int]]: Returns all pairs of `(unit, distance)`
            within the specified `dist` that match criteria.
        """
        position = self._resolve_pos(position)
        res = []
        for unit in self.game.get_all_units():
            if tag and not tag in unit.tags:
                continue
            if nid and not unit.nid == nid:
                continue
            if team and not unit.team == team:
                continue
            if party and not unit.party == party:
                continue
            if position:
                distance = utils.calculate_distance(unit.position, position)
                if distance <= dist:
                    res.append(unit)
        return res

    @categorize(QueryType.MAP)
    def get_allies_within_distance(self, position, dist: int = 1) -> List[Tuple[UnitObject, int]]:
        """Return a list containing all player units within `dist` distance to the specific position.

        Args:
            position: position or unit
            dist (int, optional): How far to search. Defaults to 1.

        Returns:
            List[Tuple[UnitObject, int]]: Returns all pairs of `(unit, distance)`
            within the specified `dist`.
        """
        return self.get_units_within_distance(position, dist, team='player')

    @categorize(QueryType.MAP)
    def get_units_in_area(self, position_corner_1: Tuple[int, int], position_corner_2: Tuple[int, int]) -> List[UnitObject]:
        """Returns a list of units within a rectangular area.

        Args:
            position_corner_1 (Tuple[int, int]): (x, y) coordinates for one corner of the area
            position_corner_2 (Tuple[int, int]): (x, y) coordinates for the opposite corner

        Returns:
            List[UnitObject]: Returns all units with positions with values between those
            specified by the corners (inclusive), or an empty list if no units exist in that area
        """
        x1, y1 = position_corner_1
        x2, y2 = position_corner_2
        if x1 > x2:
            x1, x2 = x2, x1
        if y1 > y2:
            y1, y2 = y2, y1
        target_units = []
        for unit in self.game.get_all_units():
            ux, uy = unit.position
            if x1 <= ux <= x2 and y1 <= uy <= y2:
                target_units.append(unit)
        return target_units

    @categorize(QueryType.SKILL)
    def get_debuff_count(self, unit) -> int:
        """Checks how many negative skills the unit has.

        Args:
            unit: Unit in question

        Returns:
            int: Number of unique negative skills on the unit
        """
        unit = self._resolve_to_unit(unit)
        if unit:
            return len([skill for skill in unit.skills if skill.negative])
        return 0

    @categorize(QueryType.MAP)
    def get_units_in_region(self, region, nid=None, team=None, tag=None) -> List[UnitObject]:
        """returns all units matching the criteria in the given region

Example usage:
* `get_units_in_region('NorthReinforcements', team='player')` will return all player units in the region
* `get_units_in_region('NorthReinforcements', nid='Eirika')` will return Eirika if Eirika is in the region
* `get_units_in_region('NorthReinforcements')` will return all units in the region

        Args:
            region: region in question
            nid (optional): used to match for NID
            team (optional): used to match for team.
            tag (optional): used to match for tag.

        Returns:
            List[UnitObject]: all units matching the criteria in the region
        """
        region = self._resolve_to_region(region)
        if not region:
            return []
        all_units = []
        for unit in self.game.get_all_units():
            if nid and nid != unit.nid:
                continue
            if team and team != unit.team:
                continue
            if tag and tag not in unit.tags:
                continue
            if region.contains(unit.position):
                all_units.append(unit)
        return all_units

    @categorize(QueryType.MAP)
    def any_unit_in_region(self, region, nid=None, team=None, tag=None) -> bool:
        """checks if any unit matching the criteria is in the region

Example usage:
* `any_unit_in_region('NorthReinforcements', team='player')` will check if any player unit is in the region
* `any_unit_in_region('NorthReinforcements', nid='Eirika')` will check if Eirika is in the region
* `any_unit_in_region('NorthReinforcements')` will check if ANY unit is in the region

        Args:
            region: region in question
            nid (optional): used to match for NID
            team (optional): used to match for team.
            tag (optional): used to match for tag.

        Returns:
            bool: if any unit matching criteria is in the region
        """
        return bool(self.get_units_in_region(region, nid, team, tag))

    @categorize(QueryType.UNIT)
    def is_dead(self, unit) -> bool:
        """checks if unit is dead

        Args:
            unit: unit to check

        Returns:
            bool: if the unit has died
        """
        unit = self._resolve_to_unit(unit)
        if unit:
            return self.game.check_dead(unit.nid)
        return False

    @categorize(QueryType.UNIT)
    def u(self, unit) -> Optional[UnitObject]:
        """Shorthand for game.get_unit. Fetches the unit object.

        Args:
            unit: unit nid

        Returns:
            Optional[UnitObject]: the actual unit object, if exists, else None
        """
        return self._resolve_to_unit(unit)

    @categorize(QueryType.UNIT)
    def get_support_rank(self, unit1, unit2) -> Optional[NID]:
        """Returns the most recently obtained support rank between two units.

        Args:
            unit1: unit in the support pair
            unit2: the other unit in the support pair

        Returns:
            Rank nid: if the two units have achieved a support rank.
            none: if the support pair is invalid or no rank has been obtained
        """
        unit1 = self._resolve_to_nid(unit1)
        unit2 = self._resolve_to_nid(unit2)

        support_pair = self.game.supports.get(unit1, unit2)
        if support_pair and support_pair.unlocked_ranks:
            most_recent_rank = support_pair.unlocked_ranks[-1]
            return most_recent_rank
        else: # no support exists, or no support is unlocked
            return None

    @categorize(QueryType.MAP)
    def get_terrain(self, pos) -> Optional[NID]:
        """Returns the terrain at position, or, if unit is provided,
        the terrain underneath the unit.

        Args:
            pos: Position tuple or unit

        Returns:
            Optional[NID]: the nid of the region, or None if the position is invalid
        """
        pos = self._resolve_pos(pos)
        return self.game.get_terrain_nid(self.game.tilemap, pos)

    @categorize(QueryType.VARIABLES)
    def v(self, varname, fallback=None) -> Any:
        """shorthand for game.level_vars.get and game.game_vars.get. Fetches the variable
        if game.level_vars and game.game_vars share an identical name,
        game.level_vars takes priority

        Args:
            varname: name of the variable
            fallback: fallback value, if any. Defaults to None

        Returns:
            Any: the value of the variable
        """
        var = self.game.level_vars.get(varname, None)
        if var is None:
            var = self.game.game_vars.get(varname, fallback)
        return var

    @categorize(QueryType.ACHIEVEMENT)
    def has_achievement(self, nid) -> bool:
        """Checks if an achievement is completed

        Args:
            nid: nid to check for completion

        Returns:
            bool: if the achievement exists
        """
        from app.engine.achievements import ACHIEVEMENTS
        return ACHIEVEMENTS.check_achievement(nid)
