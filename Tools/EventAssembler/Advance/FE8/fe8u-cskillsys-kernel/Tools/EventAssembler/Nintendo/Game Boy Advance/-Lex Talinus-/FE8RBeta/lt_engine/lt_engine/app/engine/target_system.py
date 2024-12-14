from __future__ import annotations
from typing import FrozenSet, TYPE_CHECKING, List, Optional, Set, Tuple
from functools import lru_cache

from app.data.database.database import DB
from app.engine import (combat_calcs, item_funcs, item_system,
                        line_of_sight, skill_system)
from app.engine.movement import movement_funcs
from app.engine.game_state import GameState
from app.utilities import utils
from app.utilities.typing import Pos
from app.engine.combat.utils import resolve_weapon


if TYPE_CHECKING:
    from app.engine.objects.unit import UnitObject
    from app.engine.objects.item import ItemObject

class TargetSystem():
    def __init__(self, game: GameState = None):
        if game:
            self.game = game
        else:
            from app.engine.game_state import game
            self.game = game

    # Consider making these sections faster
    def get_shell(self, valid_moves: Set[Pos], potential_range: Set[int],
                  bounds: Tuple[int, int, int, int], manhattan_restriction: Optional[Set[Pos]] = None) -> Set[Pos]:
        """Finds positions in a shell of radius {potential_range} from each of the positions in {valid_moves}.

        Args:
            valid_moves (Set[Pos]): Set of positions to find shells from.
            potential_range (Set[int]): Radii of the shells.
            bounds (Tuple[int, int, int, int]): Left bound, Upper bound, Right bound, Lower bound (inclusive) of legal positions.
            manhattan_restriction (Optional[Set[Pos]]): If present, set of valid positions for the shell to be in.

        Returns:
            The set of positions in the shell within {bounds} and that fall within the {manhattan_restriction}
        """
        valid_attacks = set()
        if manhattan_restriction:
            for valid_move in valid_moves:
                valid_attacks |= self.restricted_manhattan_spheres(potential_range, valid_move[0], valid_move[1], manhattan_restriction)
        else:
            for valid_move in valid_moves:
                valid_attacks |= self.find_manhattan_spheres(potential_range, valid_move[0], valid_move[1])
        return {pos for pos in valid_attacks if bounds[0] <= pos[0] <= bounds[2] and bounds[1] <= pos[1] <= bounds[3]}

    def restricted_manhattan_spheres(self, rng: Set[int], x: int, y: int, manhattan_restriction: Set[Pos]) -> Set[Pos]:
        sphere = self._cached_base_manhattan_spheres(frozenset(rng))
        sphere = {(a + x, b + y) for (a, b) in sphere if (a, b) in manhattan_restriction}
        return sphere

    def find_manhattan_spheres(self, rng: Set[int], x: int, y: int) -> Set[Pos]:
        """Returns the set of positions at radius {rng} from position {x, y}"""
        sphere = self._cached_base_manhattan_spheres(frozenset(rng))
        sphere = {(a + x, b + y) for (a, b) in sphere}
        return sphere

    @lru_cache(1024)
    def _cached_base_manhattan_spheres(self, rng: FrozenSet[int]) -> Set[Pos]:
        _range = range  # For speed
        _abs = abs  # For speed
        sphere = set()
        for r in rng:
            for i in _range(-r, r+1):
                magn = _abs(i)
                dx = i
                dy = r - magn
                sphere.add((dx, dy))
                sphere.add((dx, -dy))
        return sphere

    def get_nearest_open_tile(self, unit: UnitObject, position: Pos) -> Optional[Pos]:
        """Given a unit and their position, determines the nearest tile without a unit on it.

        The nearest tile must be weakly traversable by the unit and not have a unit on it or in the process of moving to it.
        If all tiles within 10 tiles of the starting point do not meet the requirements, returns None

        Args:
            unit (UnitObject): This unit's movement capabilities are used for determining valid tiles.
            position (Pos): Where to start looking for a nearby open tile.

        Returns:
            The nearest tile without a unit on it.
        """
        r = 0
        _abs = abs
        while r < 10:
            for x in range(-r, r + 1):
                magn = _abs(x)
                n1 = position[0] + x, position[1] + r - magn
                n2 = position[0] + x, position[1] - r + magn
                if movement_funcs.check_weakly_traversable(unit, n1) and not self.game.board.get_unit(n1) and not self.game.movement.check_if_occupied_in_future(n1):
                    return n1
                elif movement_funcs.check_weakly_traversable(unit, n2) and not self.game.board.get_unit(n2) and not self.game.movement.check_if_occupied_in_future(n2):
                    return n2
            r += 1
        return None

    def distance_to_closest_enemy(self, unit: UnitObject, pos: Optional[Pos] = None) -> int:
        """Returns the distance in tiles to the closest enemy.

        If no enemies exist at all, returns -1.
        """
        if pos is None:
            pos = unit.position
        enemy_list = [u for u in self.game.units if u.position and skill_system.check_enemy(u, unit)]
        if not enemy_list:
            return -1  # No enemies
        dist_list = [utils.calculate_distance(enemy.position, pos) for enemy in enemy_list]
        return min(dist_list)

    def get_adjacent_positions(self, pos: Pos) -> List[Pos]:
        """Returns a list of adjacent positions to the given position that are within the game board bounds"""
        x, y = pos
        adjs = ((x, y - 1), (x - 1, y), (x + 1, y), (x, y + 1))
        return [a for a in adjs if self.game.board.check_bounds(a)]

    def get_adj_units(self, unit: UnitObject) -> List[UnitObject]:
        """Returns a list of adjacent units to the unit that are within game board bounds. Does not include the unit itself."""
        adj_positions = self.get_adjacent_positions(unit.position)
        adj_units = [self.game.board.get_unit(pos) for pos in adj_positions]
        adj_units = [u for u in adj_units if u]
        return adj_units

    def get_adj_allies(self, unit: UnitObject) -> List[UnitObject]:
        """
        Returns a list of adjacent allies to the unit that are within game board bounds. Does not include the unit itself."""
        adj_units = self.get_adj_units(unit)
        adj_allies = [u for u in adj_units if skill_system.check_ally(unit, u)]
        return adj_allies

    def apply_fog_of_war(self, unit: UnitObject, item: ItemObject) -> bool:
        """Returns whether fog of war applies to this unit and item combination"""
        return (unit.team == 'player' or DB.constants.value('ai_fog_of_war')) and not item_system.ignore_fog_of_war(unit, item)

    def _filter_splash_through_fog_of_war(self, unit, main_target_pos: Optional[Pos],
                                          splash_positions: List[Pos]
                                          ) -> Tuple[Optional[Pos], List[Pos]]:
        """Returns only the main target pos and the splash positions that can be seen in fog of war."""
        # Handle main target position first
        if main_target_pos:
            main_target_pos = main_target_pos if self.game.board.check_fog_of_war(unit, main_target_pos) else None
        splash_positions = [splash_pos for splash_pos in splash_positions if splash_pos and self.game.board.check_fog_of_war(unit, splash_pos)]
        return main_target_pos, splash_positions

    def get_target_from_position(self, unit: UnitObject, item: ItemObject, target_pos: Pos) -> Tuple[Optional[Pos], List[Pos]]:
        """Returns the targeting tuple for the target_pos from its current position with the item"""
        main_target_pos, splash_positions = item_system.splash(unit, item, target_pos)
        if self.apply_fog_of_war(unit, item):
            main_target_pos, splash_positions = self._filter_splash_through_fog_of_war(unit, main_target_pos, splash_positions)
        return main_target_pos, splash_positions

    def check_target_from_position(self, unit: UnitObject, item: ItemObject, target_pos: Pos) -> bool:
        """Returns whether the unit can target the target_pos from its current position with the item"""
        main_target_pos, splash_positions = self.get_target_from_position(unit, item, target_pos)
        return item_system.target_restrict(unit, item, main_target_pos, splash_positions)

    def get_attackable_positions(self, unit: UnitObject, item: Optional[ItemObject] = None, force: bool = False) -> Set[Pos]:
        """Returns all positions the unit could attack given the item's range.

        Takes into account the unit's current position, whether the unit has attacked already, the item's range,
        line of sight, any positional restrictions, and game board bounds. Does not attempt to determine if an
        enemy is actually in the location or if the item would actually target that position,
        (ie. can't heal a full health unit, can't damage an empty tile).

        Args:
            unit (UnitObject): The unit to get attackable positions for.
            item (Optional[ItemObject]): What item to check. If not supplied, use the unit's currently equipped weapon.
            force (bool): Ignore whether the unit has already attacked. Defaults to False.

        Returns:
            All attackable positions
        """
        if not force and unit.has_attacked:
            return set()
        if not item:
            item = unit.get_weapon()
        if not item:
            return set()
        no_attack_after_move = item_system.no_attack_after_move(unit, item) or skill_system.no_attack_after_move(unit)
        if not force and no_attack_after_move and unit.has_moved_any_distance:
            return set()

        item_range = item_funcs.get_range(unit, item)
        if not item_range:
            return set()
        max_range = max(item_range)

        if max_range >= 99:
            attacks = self.game.board.get_all_positions_in_bounds()
        else:
            manhattan_restriction = item_system.range_restrict(unit, item)
            attacks = self.get_shell({unit.position}, item_range, self.game.board.bounds, manhattan_restriction)

        # Filter away those that aren't in line of sight
        if DB.constants.value('line_of_sight') and not item_system.ignore_line_of_sight(unit, item):
            attacks = set(line_of_sight.line_of_sight({unit.position}, attacks, max_range))

        return attacks

    def _get_all_attackable_positions(self, unit: UnitObject, valid_moves: List[Pos],
                                      items: List[ItemObject], force=False) -> Set[Pos]:
        """Returns all positions that the unit can attack at given a list of valid moves and a list of items available.

        Takes into account the item's range, any positional restrictions, game board bounds, and line of sight.
        Does not attempt to determine if an enemy is actually in the location or
        if the item would actually target that position, (ie. can't heal a full health unit, can't damage an empty tile).

        Args:
            unit (UnitObject): The unit to get all attackable positions for.
            valid_moves (List[Pos]): All possible moves the unit could use this turn.
            items (List[ItemObject]): Items to check.
            force (bool): Ignore whether the unit has already attacked. Defaults to False.

        Returns:
            All attackable positions
        """
        attacks: Set[Pos] = set()

        for item in items:
            no_attack_after_move = item_system.no_attack_after_move(unit, item) or skill_system.no_attack_after_move(unit)
            if not force:
                if unit.has_attacked:
                    continue
                if no_attack_after_move and unit.has_moved_any_distance:
                    continue

            item_range = item_funcs.get_range(unit, item)
            if not item_range:  # Possible if you have a weapon with say range 2-3 but your maximum range is limited to 1
                continue
            max_range = max(item_range)

            if no_attack_after_move:
                moves = {unit.position}
            else:
                moves = valid_moves

            if max_range >= 99:
                item_attacks = self.game.board.get_all_positions_in_bounds()
            else:
                manhattan_restriction = item_system.range_restrict(unit, item)
                item_attacks = self.get_shell(moves, item_range, self.game.board.bounds, manhattan_restriction)

            if DB.constants.value('line_of_sight') and not item_system.ignore_line_of_sight(unit, item):
                item_attacks = set(line_of_sight.line_of_sight(moves, item_attacks, max_range))

            attacks |= item_attacks

        return attacks

    def get_all_attackable_positions_weapons(self, unit: UnitObject, valid_moves: List[Pos], force=False) -> Set[Pos]:
        """Returns all positions that the unit can attack with their WEAPONS and weapon-like abilities given a list of valid moves to attack from

        Takes into account the item's range, any positional restrictions, and game board bounds.
        Does not attempt to determine if an enemy is actually in the location or
        if the item would actually target that position, (ie. can't heal a full health unit, can't damage an empty tile).

        Args:
            unit (UnitObject): The unit to get all attackable positions for.
            valid_moves (List[Pos]): All possible moves the unit could use this turn.
            force (bool): Ignore whether the unit has already attacked. Defaults to False.

        Returns:
            All attackable positions
        """
        return self._get_all_attackable_positions(unit, valid_moves, self._get_all_weapons(unit, show_abilities=True), force)

    def get_all_attackable_positions_spells(self, unit: UnitObject, valid_moves: List[Pos], force=False) -> Set[Pos]:
        """Returns all positions that the unit can attack with their SPELLS and spell-like abilities given a list of valid moves to attack from

        Takes into account the item's range, any positional restrictions, and game board bounds.
        Does not attempt to determine if an enemy is actually in the location or
        if the item would actually target that position, (ie. can't heal a full health unit, can't damage an empty tile).

        Args:
            unit (UnitObject): The unit to get all attackable positions for.
            valid_moves (List[Pos]): All possible moves the unit could use this turn.
            force (bool): Ignore whether the unit has already attacked. Defaults to False.

        Returns:
            All attackable positions
        """
        return self._get_all_attackable_positions(unit, valid_moves, self._get_all_spells(unit, show_abilities=True), force)

    def get_possible_attack_positions(self, unit: UnitObject, target: Pos, moves: Set[Pos],
                                      item: ItemObject) -> List[Pos]:
        """
        Given a unit, a target, an item, and the set of the unit's possible moves,
        determines where the unit is able to move so that the unit can strike the
        target with the item.

        Args:
            unit (UnitObject): The attacking unit.
            target (Pos): the position of the target.
            moves (Set[Pos]): the set of tiles the unit can reach this turn.
            item (ItemObject): What item the unit is using to attack with.

        Returns:
            the subset of moves that the unit can attack the target from with the item.
        """
        item_range = item_funcs.get_range(unit, item)
        restriction = item_system.range_restrict(unit, item)
        valid_moves: Set[Pos] = set()
        for move in moves:
            possible_strike_locations = \
                self.get_shell({move}, item_range, self.game.board.bounds, restriction)
            if target in possible_strike_locations:
                valid_moves.add(move)

        # Filter away possible attacks that aren't in line of sight
        if DB.constants.value('line_of_sight') and not item_system.ignore_line_of_sight(unit, item):
            valid_moves = line_of_sight.line_of_sight({target}, valid_moves, max(item_range, default=0))
        return list(valid_moves)

    # === Finding valid targets ===
    def targets_in_range(self, unit: UnitObject, item: ItemObject) -> Set[Pos]:
        """Given a unit and an item, finds a set of positions that are within range of the item
        and count as possible targets
        """
        possible_targets = item_system.valid_targets(unit, item)
        item_range = item_funcs.get_range(unit, item)
        return {t for t in possible_targets if utils.calculate_distance(unit.position, t) in item_range}

    def get_valid_targets(self, unit: UnitObject, item: Optional[ItemObject] = None) -> Set[Pos]:
        """Returns valid targets the unit could attack from their current position using the item.

        Considers fog of war as well as targeting restrictions in addition to the usual
        item's range, line of sight, any positional restrictions, and game board bounds.

        Args:
            unit (UnitObject): The unit to get valid targets for.
            item (Optional[ItemObject]): What item to check. If not supplied, use the unit's currently equipped weapon.

        Returns:
            Valid targets
        """
        if not item:
            item = unit.get_weapon()
        if not item:
            return set()

        # Check sequence item targeting
        if item.sequence_item:
            all_targets = set()
            for subitem in item.subitems:
                valid_targets = self.get_valid_targets(unit, subitem)
                if not valid_targets:
                    return set()
                all_targets |= valid_targets
            if not item_system.allow_same_target(unit, item) and \
                    len(all_targets) < sum(1 if item_system.allow_less_than_max_targets(unit, si) else item_system.num_targets(unit, si) for si in item.subitems):
                return set()

        # Handle regular item targeting
        all_targets = item_system.valid_targets(unit, item)
        attackable_positions = self.get_attackable_positions(unit, item)
        all_targets &= attackable_positions

        valid_targets = set()
        for position in all_targets:
            main_target_pos, splash_positions = self.get_target_from_position(unit, item, position)
            # If there are no valid targets at all, then this cannot be a valid position to target
            if not main_target_pos and not splash_positions:
                continue
            if item_system.target_restrict(unit, item, main_target_pos, splash_positions):
                valid_targets.add(position)

        # Make sure we have enough targets to satisfy the item
        if not item_system.allow_same_target(unit, item) and \
                not item_system.allow_less_than_max_targets(unit, item) and \
                len(valid_targets) < item_system.num_targets(unit, item):
            return set()
        return valid_targets

    def get_valid_targets_recursive_with_availability_check(self, unit: UnitObject, item: ItemObject) -> Set[Pos]:
        """Returns valid targets the unit could attack given the item. Checks subitems of the item
        if it's a multi-item as well.

        Args:
            unit (UnitObject): The unit to get valid targets for.
            item (Optional[ItemObject]): What item to check.

        Returns:
            Valid targets
        """
        if item.multi_item:
            items = [subitem for subitem in item_funcs.get_all_items_from_multi_item(unit, item) if item_funcs.available(unit, subitem)]
        else:
            items = [item] if item_funcs.available(unit, item) else []
        valid_targets: Set[Pos] = set()
        for subitem in items:
            valid_targets |= self.get_valid_targets(unit, subitem)
        return valid_targets

    def get_ai_targets(self, unit: UnitObject, valid_moves: List[Pos],
                       items: List[ItemObject]) -> Set[Pos]:
        """Returns all valid targets of the ai from any of their valid moves with the input items.

        Only used by ai_controller.py

        Considers item's range, any positional restrictions, and game board bounds.

        Does NOT consider fog of war or targeting restrictions. Handled later during the AI utility determination

        Does NOT fully consider line of sight. Handle line of sight checks later on in
        processing if so desired.
        For instance, the AI controller does so while iterating over (move, item, target) triples
        Reason: Figuring out line of sight here would be:
            1. Not efficient: we would have to iterate over every pair of (move, target) to find the good ones.
            2. Not useful: You are going to have to do this check again later since just because
                a target is valid, doesn't mean that every (move, target) pair is valid.

        Args:
            unit (UnitObject): The unit to get all attackable positions for.
            valid_moves (List[Pos]): All possible moves the unit could use this turn.
            items (List[ItemObject]): Items to check.

        Returns:
            All valid attacks
        """
        all_valid_targets: Set[Pos] = set()
        for item in items:
            # AI does not handle sequence or multi-target items
            if item.sequence_item and item.subitems:
                pass  # 
            else:
                possible_targets = item_system.valid_targets(unit, item)
                attackable_positions = self._get_all_attackable_positions(unit, valid_moves, [item])
                all_valid_targets |= (possible_targets & attackable_positions)

        return all_valid_targets

    # === Item Filtering ===
    def get_weapons(self, unit: UnitObject) -> List[ItemObject]:
        # Explicitly does not consider extra abilities since it's used within the Attack menu
        return [item for item in unit.items if item_funcs.is_weapon_recursive(unit, item) and item_funcs.available(unit, item)]

    def _get_all_weapons(self, unit: UnitObject, show_abilities: bool = False) -> List[ItemObject]:
        if DB.constants.value("show_abilities") and show_abilities:
            items = item_funcs.get_all_items_and_abilities(unit)
        else:
            items = item_funcs.get_all_items(unit)
        return [item for item in items if item_system.is_weapon(unit, item) and item_funcs.available(unit, item)]

    def get_all_weapon_targets(self, unit: UnitObject) -> Set[Pos]:
        weapons: List[ItemObject] = self._get_all_weapons(unit)
        targets = set()
        for weapon in weapons:
            targets |= self.get_valid_targets_recursive_with_availability_check(unit, weapon)
        return targets

    def get_spells(self, unit: UnitObject) -> List[ItemObject]:
        # Explicitly does not consider extra abilities since it's used within the Spell menu
        return [item for item in unit.items if item_funcs.is_spell_recursive(unit, item) and item_funcs.available(unit, item)]

    def _get_all_spells(self, unit: UnitObject, show_abilities: bool = False) -> List[ItemObject]:
        if DB.constants.value("show_abilities") and show_abilities:
            items = item_funcs.get_all_items_and_abilities(unit)
        else:
            items = item_funcs.get_all_items(unit)
        return [item for item in items if item_system.is_spell(unit, item) and item_funcs.available(unit, item)]

    def get_all_spell_targets(self, unit: UnitObject) -> Set[Pos]:
        spells: List[ItemObject] = self._get_all_spells(unit)
        targets = set()
        for spell in spells:
            targets |= self.get_valid_targets_recursive_with_availability_check(unit, spell)
        return targets

    # === PAIRUP ===
    def find_strike_partners(self, attacker, defender, item):
        """Finds and returns a tuple of strike partners for the specified units
        First item in tuple is attacker partner, second is target partner
        Returns a tuple of None if no valid partner"""
        if not DB.constants.value('pairup'):
            return None, None
        if not attacker or not defender:
            return None, None
        if skill_system.check_ally(attacker, defender):  # If targeting an ally
            return None, None
        if attacker.traveler or defender.traveler:  # Dual guard cancels
            return None, None
        if not item_system.is_weapon(attacker, item):  # If you're healing someone else
            return None, None
        if attacker.team == defender.team:  # If you are the same team. Catches components who define their own check_ally function
            return None, None

        attacker_partner = None
        defender_partner = None
        attacker_adj_allies = self.get_adj_allies(attacker)
        attacker_adj_allies = [ally for ally in attacker_adj_allies if ally.get_weapon() and not item_system.cannot_dual_strike(ally, ally.get_weapon())]
        defender_adj_allies = self.get_adj_allies(defender)
        defender_adj_allies = [ally for ally in defender_adj_allies if ally.get_weapon() and not item_system.cannot_dual_strike(ally, ally.get_weapon())]
        attacker_partner = self.strike_partner_formula(attacker_adj_allies, attacker, defender, 'attack', (0, 0))
        defender_partner = self.strike_partner_formula(defender_adj_allies, defender, attacker, 'defense', (0, 0))

        if item_system.cannot_dual_strike(attacker, item):
            attacker_partner = None
        if defender.get_weapon() and item_system.cannot_dual_strike(defender, defender.get_weapon()):
            defender_partner = None
        if DB.constants.value('player_pairup_only'):
            if attacker.team != 'player':
                attacker_partner = None
            if defender.team != 'player':
                defender_partner = None

        if attacker_partner is defender_partner:
            # If both attacker and defender have the same partner something is weird
            return None, None
        return attacker_partner, defender_partner

    def strike_partner_formula(self, allies: list, attacker, defender, mode, attack_info):
        """This is the formula for the best choice to make when autoselecting strike partners"""
        if not allies:
            return None
        damage = [combat_calcs.compute_assist_damage(ally, defender, ally.get_weapon(), resolve_weapon(defender), mode, attack_info) for ally in allies]
        accuracy = [utils.clamp(combat_calcs.compute_hit(ally, defender, ally.get_weapon(), resolve_weapon(defender), mode, attack_info)/100., 0, 1) for ally in allies]
        score = [dam * acc for dam, acc in zip(damage, accuracy)]
        max_score = max(score)
        max_index = score.index(max_score)
        return allies[max_index]
