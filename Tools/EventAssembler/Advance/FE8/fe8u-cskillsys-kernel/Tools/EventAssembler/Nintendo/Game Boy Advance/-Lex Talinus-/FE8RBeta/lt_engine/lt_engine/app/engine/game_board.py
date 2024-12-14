from __future__ import annotations

from app.engine.objects.unit import UnitObject
from typing import Dict, List, Optional, Set, Tuple, TYPE_CHECKING

from app.data.database.database import DB
from app.engine import line_of_sight
from app.engine.pathfinding.node import Node
from app.engine.game_state import game
from app.utilities.grid import Grid, BoundedGrid
from app.utilities.typing import NID, Pos, UID

if TYPE_CHECKING:
    from app.engine.objects.skill import SkillObject

class GameBoard(object):
    def __init__(self, tilemap):
        self.width: int = tilemap.width
        self.height: int = tilemap.height
        self.bounds: Tuple[int, int, int, int] = (0, 0, self.width - 1, self.height - 1)
        self.mcost_grids: Dict[NID, Grid[Node]] = {}

        self.reset_tile_grids(tilemap)

        # Keeps track of what team occupies which tile
        self.team_grid: Grid[List[NID]] = self.initialize_list_grid()
        # Keeps track of which unit occupies which tile
        self.unit_grid: Grid[List[UnitObject]] = self.initialize_list_grid()

        # Fog of War -- one for each team
        self.fog_of_war_grids = {}
        for team in DB.teams:
            self.fog_of_war_grids[team.nid] = self.init_set_grid()
        self.fow_vantage_point = {}  # Unit: Position where the unit is that's looking
        self.fog_regions = self.init_set_grid()
        self.fog_region_set = set()  # Set of Fog region nids so we can tell how many fog regions exist at all times
        self.vision_regions = self.init_set_grid()

        # For Auras
        self.aura_grid = self.init_set_grid()
        # Key: Aura Skill Uid, Value: Set of positions
        self.known_auras = {}

        # For opacity
        self.opacity_grid = self.init_opacity_grid(tilemap)

    def set_bounds(self, min_x: int, min_y: int, max_x: int, max_y: int):
        self.bounds = (min_x, min_y, max_x, max_y)

    def check_bounds(self, pos: Pos) -> bool:
        return self.bounds[0] <= pos[0] <= self.bounds[2] and self.bounds[1] <= pos[1] <= self.bounds[3]

    def get_all_positions_in_bounds(self) -> Set[Pos]:
        return {(x, y) for x in range(self.bounds[0], self.bounds[2] + 1) for y in range(self.bounds[1], self.bounds[3] + 1)}

    def reset_tile_grids(self, tilemap):
        # For each movement type
        mtype_grid: Grid[NID] = Grid((self.width, self.height))
        for x in range(self.width):
            for y in range(self.height):
                terrain_nid = game.get_terrain_nid(tilemap, (x, y))
                terrain = DB.terrain.get(terrain_nid)
                if not terrain:
                    terrain = DB.terrain[0]
                mtype_grid.append(terrain.mtype)
        for mode in DB.mcost.unit_types:
            self.mcost_grids[mode] = self.init_movement_grid(mode, tilemap, mtype_grid)
        self.opacity_grid = self.init_opacity_grid(tilemap)

    def reset_pos(self, tilemap, pos: Pos):
        terrain_nid = game.get_terrain_nid(tilemap, pos)
        terrain = DB.terrain.get(terrain_nid)
        if not terrain:
            terrain = DB.terrain[0]
        mtype = terrain.mtype

        # Movement reset
        for movement_group in DB.mcost.unit_types:
            mcost_grid = self.mcost_grids[movement_group]        
            if mtype:
                tile_cost = DB.mcost.get_mcost(movement_group, mtype)
            else:
                tile_cost = 1
            mcost_grid.insert(pos, Node(*pos, tile_cost < 99, tile_cost))

        # Opacity reset
        if terrain:
            self.opacity_grid.insert(pos, terrain.opaque)
        else:
            self.opacity_grid.insert(pos, False)

    # For movement
    def init_movement_grid(self, movement_group: NID, tilemap, mtype_grid: Grid[NID]) -> Grid[Node]:
        grid = Grid[Node]((self.width, self.height))
        for x in range(self.width):
            for y in range(self.height):
                mtype = mtype_grid.get((x, y))
                if mtype:
                    tile_cost = DB.mcost.get_mcost(movement_group, mtype)
                else:
                    tile_cost = 1
                grid.append(Node(x, y, tile_cost < 99, tile_cost))

        return grid

    def get_movement_grid(self, movement_group: NID) -> BoundedGrid[Node]:
        return self.mcost_grids[movement_group].apply_bounds(self.bounds)

    def initialize_list_grid(self) -> Grid[List]:
        grid = Grid[List[NID]]((self.width, self.height))
        for x in range(self.width):
            for y in range(self.height):
                grid.append([])
        return grid

    def set_unit(self, pos: Pos, unit: UnitObject):
        if unit not in self.unit_grid.get(pos):
            self.unit_grid.get(pos).append(unit)
            self.team_grid.get(pos).append(unit.team)

    def remove_unit(self, pos: Pos, unit: UnitObject):
        if unit in self.unit_grid.get(pos):
            self.unit_grid.get(pos).remove(unit)
            self.team_grid.get(pos).remove(unit.team)

    def get_unit(self, pos: Pos) -> Optional[UnitObject]:
        if not pos:
            return None
        if self.unit_grid.get(pos):
            return self.unit_grid.get(pos)[0]
        return None

    def get_units(self, pos: Pos) -> List[UnitObject]:
        if not pos:
            return []
        return self.unit_grid.get(pos)

    def get_team(self, pos: Pos) -> NID:
        if not pos:
            return None
        if self.team_grid.get(pos):
            return self.team_grid.get(pos)[0]
        return None

    def is_tile(self, pos: Pos) -> bool:
        """Returns whether a Tile tagged unit is on the given position"""
        return self.get_unit(pos) and 'Tile' in self.get_unit(pos).tags

    def can_move_through(self, team: NID, pos: Pos) -> bool:
        unit_team = self.get_team(pos)
        if not unit_team or team in DB.teams.get_allies(unit_team):
            return True
        if team == 'player' or DB.constants.value('ai_fog_of_war'):
            if not self.in_vision(pos, team):
                return True  # Can always move through what you can't see
        return False

    def can_move_through_ally_block(self, team: NID, pos: Pos) -> bool:
        unit_team = self.get_team(pos)
        if not unit_team:
            return True
        if team == 'player' or DB.constants.value('ai_fog_of_war'):
            if not self.in_vision(pos, team):
                return True
        return False

    # === Fog of War ===
    def update_fow(self, pos: Optional[Pos], unit: UnitObject, sight_range: int):
        """Modifies the state of the fog of war game board to reflect the unit moving to the pos"""
        grid: Grid[List[UnitObject]] = self.fog_of_war_grids[unit.team]
        # Remove the old vision
        self.fow_vantage_point[unit.nid] = None
        for cell in grid.cells():
            cell.discard(unit.nid)
        # Add new vision
        if pos:
            self.fow_vantage_point[unit.nid] = pos
            positions = game.target_system.find_manhattan_spheres(range(sight_range + 1), *pos)
            positions = {pos for pos in positions if 0 <= pos[0] < self.width and 0 <= pos[1] < self.height}
            for position in positions:
                grid.get(position).add(unit.nid)

    def add_fog_region(self, region):
        if region.position:
            self.fog_region_set.add(region.nid)
            fog_range = int(region.sub_nid) if region.sub_nid else 0
            positions = set()
            for pos in region.get_all_positions():
                positions |= game.target_system.find_manhattan_spheres(range(fog_range + 1), pos[0], pos[1])
            positions = {pos for pos in positions if 0 <= pos[0] < self.width and 0 <= pos[1] < self.height}
            for position in positions:
                self.fog_regions.get(position).add(region.nid)

    def remove_fog_region(self, region):
        self.fog_region_set.discard(region.nid)
        for cell in self.fog_regions.cells():
            cell.discard(region.nid)

    def add_vision_region(self, region):
        if region.position:
            vision_range = int(region.sub_nid) if region.sub_nid else 0
            positions = set()
            for pos in region.get_all_positions():
                positions |= game.target_system.find_manhattan_spheres(range(vision_range + 1), pos[0], pos[1])
            positions = {pos for pos in positions if 0 <= pos[0] < self.width and 0 <= pos[1] < self.height}
            for position in positions:
                self.vision_regions.get(position).add(region.nid)

    def remove_vision_region(self, region):
        for cell in self.vision_regions.cells():
            cell.discard(region.nid)

    def in_vision(self, pos: Tuple[int, int], team: NID = 'player') -> bool:
        # Anybody can see things in vision regions no matter what
        # So don't use vision regions with fog line of sight
        if self.vision_regions.get(pos):
            return True

        if game.level_vars.get('_fog_of_war') or self.fog_regions.get(pos):
            if team == 'player':
                # Right now, line of sight doesn't interact at all with vision regions
                # Since I'm not sure how we'd handle cases where a vision region is obscured by an opaque tile
                if DB.constants.value('fog_los'):
                    fog_of_war_radius = game.level_vars.get('_fog_of_war_radius', 0)
                    valid: bool = False  # Can we see the pos?
                    # We can if any of our allies can see the pos.
                    for team_nid in DB.teams.get_allies(team):
                        valid |= line_of_sight.simple_check(pos, team_nid, fog_of_war_radius)
                    if not valid:
                        return False
                
                for team_nid in DB.teams.get_allies(team):
                    team_grid = self.fog_of_war_grids[team_nid]
                    if team_grid.get(pos):
                        return True
            else:
                if DB.constants.value('fog_los'):
                    fog_of_war_radius = self.get_fog_of_war_radius(team)
                    valid = line_of_sight.simple_check(pos, team, fog_of_war_radius)
                    if not valid:
                        return False
                grid = self.fog_of_war_grids[team]
                if grid.get(pos):
                    return True
            return False
        else:
            return True

    def get_fog_of_war_radius(self, team: str) -> int:
        ai_fog_of_war_radius = game.level_vars.get('_ai_fog_of_war_radius', game.level_vars.get('_fog_of_war_radius', 0))
        player_team_allies = DB.teams.allies
        if team == 'player':
            fog_of_war_radius = game.level_vars.get('_fog_of_war_radius', 0)
        elif team in player_team_allies:
            fog_of_war_radius = game.level_vars.get('_other_fog_of_war_radius', ai_fog_of_war_radius)
        else:
            fog_of_war_radius = ai_fog_of_war_radius
        return fog_of_war_radius

    def check_fog_of_war(self, unit: UnitObject, pos: Pos) -> bool:
        """Returns True if unit can see position for targeting.

        Units can always see their own position and the position of Tile-tagged units."""
        return (pos == unit.position or
                self.in_vision(pos, unit.team) or
                self.is_tile(pos))

    # === Line of Sight ===
    def init_opacity_grid(self, tilemap) -> Grid[bool]:
        grid = Grid[bool]((self.width, self.height))
        for x in range(self.width):
            for y in range(self.height):
                terrain_nid = game.get_terrain_nid(tilemap, (x, y))
                t = DB.terrain.get(terrain_nid)
                if t:
                    grid.append(t.opaque)
                else:
                    grid.append(False)
        return grid

    def get_opacity(self, pos: Pos) -> bool:
        if not pos:
            return False
        return self.opacity_grid.get(pos)

    # === Auras ===
    def init_set_grid(self) -> Grid[set]:
        grid = Grid[set]((self.width, self.height))
        for x in range(self.width):
            for y in range(self.height):
                grid.append(set())
        return grid

    def reset_aura(self, child_skill: SkillObject):
        if child_skill.uid in self.known_auras:
            self.known_auras[child_skill.uid].clear()

    def add_aura(self, pos: Pos, child_skill: SkillObject, target: str):
        # Target is one of ('Ally', 'Enemy', 'Unit')
        self.aura_grid.get(pos).add((child_skill.uid, target))
        if child_skill.uid not in self.known_auras:
            self.known_auras[child_skill.uid] = set()
        self.known_auras[child_skill.uid].add(pos)

    def remove_aura(self, pos: Pos, child_skill: SkillObject):
        for aura_data in list(self.aura_grid.get(pos)):
            if aura_data[0] == child_skill.uid:
                self.aura_grid.get(pos).discard(aura_data)
        if child_skill.uid in self.known_auras:
            self.known_auras[child_skill.uid].discard(pos)

    def get_auras(self, pos: Pos) -> Set[Tuple[UID, str]]:
        return self.aura_grid.get(pos)

    def get_aura_positions(self, child_skill: SkillObject) -> Set[Pos]:
        return self.known_auras.get(child_skill.uid, set())
