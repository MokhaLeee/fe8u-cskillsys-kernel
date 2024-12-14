from PyQt5.QtGui import QPixmap

import app.utilities as utils
from app.map_maker.utilities import random_choice, edge_random, flood_fill, find_bounds
from app.map_maker.terrain import Terrain, TerrainCatalog
from app.map_maker.wang_terrain import WangCorner2Terrain, WangEdge2Terrain, WangEdge2Terrain16
from app.map_maker.building_terrain import CastleTerrain, HouseTerrain, RuinsTerrain
from app.map_maker.cliff_terrain import CliffTerrain
from app.map_maker.sea_terrain import SeaTerrain
from app.map_maker.grass_terrain import GrassTerrain, LightGrassTerrain
from app.map_maker.mountain_terrain import MountainTerrain


class RandomTerrain(Terrain):
    data = []

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        new_coord1 = random_choice([(p[0]*2, p[1]*2) for p in self.data], pos)
        new_coord2 = random_choice([(p[0]*2 + 1, p[1]*2) for p in self.data], pos, offset=1)
        new_coord3 = random_choice([(p[0]*2 + 1, p[1]*2 + 1) for p in self.data], pos, offset=2)
        new_coord4 = random_choice([(p[0]*2, p[1]*2 + 1) for p in self.data], pos, offset=3)

        pix = self.get_pixmap8(new_coord1, new_coord2, new_coord3, new_coord4, autotile_num)
        return pix

class SandTerrain(WangCorner2Terrain):
    terrain_like = ('Sand', 'Road', 'Sea', 'BridgeV', 'BridgeH')
    corner_chance = 0.6
    edge_chance = 0.4
    vertices: dict = {}

    def single_process(self, tilemap):
        # For each vertex, assign a random value
        # Then go through each vertex and determine if corner, edge, or neither
        # Check values for each vertex to decide if it should be removed
        # Save data somewhere
        positions: set = tilemap.get_all_terrain(self.nid)
        self.vertices.clear()
        for pos in positions:
            self.determine_vertex(tilemap, pos)

    def _determine_index(self, tilemap, pos: tuple) -> tuple:
        center, left, right, top, bottom, topleft, topright, bottomleft, bottomright = self._pos_to_vertices(pos)
        center_edge = True
        left_edge = bool(self.vertices[left][0])
        right_edge = bool(self.vertices[right][0])
        top_edge = bool(self.vertices[top][0])
        bottom_edge = bool(self.vertices[bottom][0])
        topleft_edge = bool(self.vertices[topleft][0])
        topright_edge = bool(self.vertices[topright][0])
        bottomleft_edge = bool(self.vertices[bottomleft][0])
        bottomright_edge = bool(self.vertices[bottomright][0])

        # If adjacent to non-sand, don't remove
        north, east, south, west = tilemap.get_cardinal_terrain(pos)
        force_north_edge = north in self.terrain_like and north != 'Sand'
        force_south_edge = south in self.terrain_like and south != 'Sand'
        force_east_edge = east in self.terrain_like and east != 'Sand'
        force_west_edge = west in self.terrain_like and west != 'Sand'

        # Randomly determine some to remove
        if self.vertices[center][0] == 3 and self.vertices[center][1] < self.edge_chance:
            center_edge = False
        if self.vertices[center][0] == 2 and self.vertices[center][1] < self.corner_chance:
            center_edge = False
        if not force_west_edge and self.vertices[left][0] in (2, 3) and self.vertices[left][1] < self.edge_chance:
            left_edge = False
        if not force_east_edge and self.vertices[right][0] in (2, 3) and self.vertices[right][1] < self.edge_chance:
            right_edge = False
        if not force_north_edge and self.vertices[top][0] in (2, 3) and self.vertices[top][1] < self.edge_chance:
            top_edge = False
        if not force_south_edge and self.vertices[bottom][0] in (2, 3) and self.vertices[bottom][1] < self.edge_chance:
            bottom_edge = False

        index1 = 1 * top_edge + \
            2 * center_edge + \
            4 * left_edge + \
            8 * topleft_edge
        index2 = 1 * topright_edge + \
            2 * right_edge + \
            4 * center_edge + \
            8 * top_edge
        index3 = 1 * right_edge + \
            2 * bottomright_edge + \
            4 * bottom_edge + \
            8 * center_edge
        index4 = 1 * center_edge + \
            2 * bottom_edge + \
            4 * bottomleft_edge + \
            8 * left_edge
        return index1, index2, index3, index4

class ForestTerrain(Terrain):
    forest_like = ('Forest', 'Thicket')
    
    @property
    def check_flood_fill(self):
        return True

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        north, east, south, west = tilemap.get_cardinal_terrain(pos)
        blob_positions = flood_fill(tilemap, pos)
        _, _, _, _, blob_width, blob_height, center_x, center_y = \
            find_bounds(tilemap, blob_positions)
        my_radius_width = abs(pos[0] + 0.5 - center_x)
        my_radius_height = abs(pos[1] + 0.5 - center_y)
        depth_w = (blob_width / 2) - my_radius_width
        depth_h = (blob_height / 2) - my_radius_height
        chance_w = utils.lerp(1, 0, depth_w/4)
        chance_h = utils.lerp(1, 0, depth_h/4)
        chance_to_lose_adjacent_edges = utils.clamp(max(chance_w, chance_h), 0, 1)

        north_edge = bool(north and north not in self.forest_like)  # Whether we don't border a forest
        if not north_edge and north and north != 'Thicket':  # We border a forest
            north_edge = (edge_random((pos[0], pos[1] - 1), pos) < chance_to_lose_adjacent_edges)
        east_edge = bool(east and east not in self.forest_like)
        if not east_edge and east and east != 'Thicket':  # We border a forest
            east_edge = (edge_random(pos, (pos[0] + 1, pos[1])) < chance_to_lose_adjacent_edges)
        south_edge = bool(south and south not in self.forest_like)
        if not south_edge and south and south != 'Thicket':  # We border a forest
            south_edge = (edge_random(pos, (pos[0], pos[1] + 1)) < chance_to_lose_adjacent_edges)
        west_edge = bool(west and west not in self.forest_like)
        if not west_edge and west and west != 'Thicket':  # We border a forest
            west_edge = (edge_random((pos[0] - 1, pos[1]), pos) < chance_to_lose_adjacent_edges)
        
        total_index = \
            north_edge + 2 * east_edge + 4 * south_edge + 8 * west_edge
        index1 = north_edge + 8 * west_edge
        index2 = north_edge + 2 * east_edge
        index3 = 4 * south_edge + 2 * east_edge
        index4 = 4 * south_edge + 8 * west_edge
        if total_index == 15 and random_choice([1, 2, 3], pos) != 3:  # When by itself
            new_coord1 = (14, 0)
            new_coord2 = (15, 0)
            new_coord3 = (15, 1)
            new_coord4 = (14, 1)
        else:
            new_coord1 = (index1, {0: 0, 1: 0, 8: 0, 9: 0}[index1])
            new_coord2 = (index2, {0: 1, 1: 1, 2: 0, 3: 0}[index2])
            new_coord3 = (index3, {0: 3, 2: 1, 4: 1, 6: 0}[index3])
            new_coord4 = (index4, {0: 2, 4: 0, 8: 1, 12: 0}[index4])
        
        pix = self.get_pixmap8(new_coord1, new_coord2, new_coord3, new_coord4, autotile_num)
        return pix

class HillTerrain(Terrain): 
    data = {'main': (12, 21), 'pair1': (13, 20), 'pair2': (14, 20), 'alter1': (13, 21)}
    
    @property
    def check_flood_fill(self):
        return True

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        _, east, _, west = tilemap.get_cardinal_terrain(pos)
        _, far_east, _, _ = tilemap.get_cardinal_terrain((pos[0] + 1, pos[1]))
        _, _, _, far_west = tilemap.get_cardinal_terrain((pos[0] - 1, pos[1]))
        if east != self.nid and west != self.nid:
            choice = random_choice([1, 2, 3, 4, 5, 6], pos)
            if choice <= 3:
                coord = self.data['main']
            elif choice in (4, 5):
                coord = self.data['alter1']
            else:
                coord = self.data['pair2']
        elif west != self.nid:
            coord = self.data['main']
        elif east != self.nid:
            if far_west == self.nid:
                coord = self.data['pair2']
            else:
                coord = self.data['pair1']
        else:
            coord = self.data['pair1']

        new_coord1 = (coord[0]*2, coord[1]*2)
        new_coord2 = (coord[0]*2 + 1, coord[1]*2)
        new_coord3 = (coord[0]*2 + 1, coord[1]*2 + 1)
        new_coord4 = (coord[0]*2, coord[1]*2 + 1)

        pix = self.get_pixmap8(new_coord1, new_coord2, new_coord3, new_coord4, autotile_num)
        return pix

class RiverTerrain(WangEdge2Terrain):
    terrain_like = ('River', 'Sea', 'BridgeH', 'BridgeV')

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        north, east, south, west = tilemap.get_cardinal_terrain(pos)
        northeast, southeast, southwest, northwest = tilemap.get_diagonal_terrain(pos)
        north_edge = bool(not north or north in self.terrain_like)
        south_edge = bool(not south or south in self.terrain_like)
        east_edge = bool(not east or east in self.terrain_like)
        west_edge = bool(not west or west in self.terrain_like)
        northeast_edge = bool(not northeast or northeast in self.terrain_like)
        southeast_edge = bool(not southeast or southeast in self.terrain_like)
        southwest_edge = bool(not southwest or southwest in self.terrain_like)
        northwest_edge = bool(not northwest or northwest in self.terrain_like)
        if random_choice([1, 2], pos) == 1:
            use_top = True
        else:
            use_top = False
        # Handle true diagonals
        # Topleft
        if north_edge and west_edge and not south_edge and not east_edge:
            if use_top:
                new_coord2 = (9, 0)
                new_coord4 = (9, 1)
                new_coord3 = (8, 0)
                if northwest_edge:
                    new_coord1 = (2, 7)
                else:
                    new_coord1 = (2, 5)
            else:
                new_coord2 = (9, 2)
                new_coord4 = (9, 3)
                new_coord3 = (8, 1)
                if northwest_edge:
                    new_coord1 = (2, 6)
                else:
                    new_coord1 = (2, 4)
        # Topright
        elif north_edge and east_edge and not south_edge and not west_edge:
            if use_top:
                new_coord1 = (3, 0)
                new_coord3 = (3, 1)
                new_coord4 = (1, 0)
                if northeast_edge:
                    new_coord2 = (4, 6)
                else:
                    new_coord2 = (4, 4)
            else:
                new_coord1 = (3, 2)
                new_coord3 = (3, 3)
                new_coord4 = (1, 1)
                if northeast_edge:
                    new_coord2 = (4, 7)
                else:
                    new_coord2 = (4, 5)
        # Bottomleft
        elif south_edge and west_edge and not north_edge and not east_edge:
            if use_top:
                new_coord1 = (12, 0)
                new_coord3 = (12, 1)
                new_coord2 = (4, 1)
                if southwest_edge:
                    new_coord4 = (1, 7)
                else:
                    new_coord4 = (1, 5)
            else:
                new_coord1 = (12, 2)
                new_coord3 = (12, 3)
                new_coord2 = (4, 0)
                if southwest_edge:
                    new_coord4 = (1, 6)
                else:
                    new_coord4 = (1, 4)
        # Bottomright
        elif south_edge and east_edge and not north_edge and not west_edge:
            if use_top:
                new_coord2 = (6, 1)
                new_coord4 = (6, 0)
                new_coord1 = (2, 0)
                if southeast_edge:
                    new_coord3 = (8, 6)
                else:
                    new_coord3 = (8, 4)
            else:
                new_coord2 = (6, 2)
                new_coord4 = (6, 3)
                new_coord1 = (2, 1)
                if southeast_edge:
                    new_coord3 = (8, 7)
                else:
                    new_coord3 = (8, 5)
        # Waterfall -- TODO check the chirality of the cliff
        elif south_edge and north_edge and west and west.startswith('Cliff') and east and east.startswith('Cliff'):
            new_coord1 = (0, 8)
            new_coord2 = (1, 8)
            new_coord3 = (1, 9)
            new_coord4 = (0, 9)
        else:
            index1 = 6 + 1 * north_edge + 8 * west_edge
            index2 = 12 + 1 * north_edge + 2 * east_edge
            index3 = 9 + 4 * south_edge + 2 * east_edge
            index4 = 3 + 4 * south_edge + 8 * west_edge
            new_coord1 = random_choice([(index1, k) for k in range(self.limits[index1])], pos)
            new_coord2 = random_choice([(index2, k) for k in range(self.limits[index2])], pos, offset=1)
            new_coord3 = random_choice([(index3, k) for k in range(self.limits[index3])], pos, offset=2)
            new_coord4 = random_choice([(index4, k) for k in range(self.limits[index4])], pos, offset=3)
            # Handle using the same set for vertical edges
            if index1 == 7:
                new_coord1 = (index1, random_choice([0, 1, 2, 3, 4], pos)*2)
            if index4 == 7:
                new_coord4 = (index4, random_choice([0, 1, 2, 3, 4], pos)*2 + 1)
            if index2 == 13:
                new_coord2 = (index2, random_choice([0, 1, 2, 3, 4], pos)*2)
            if index3 == 13:
                new_coord3 = (index3, random_choice([0, 1, 2, 3, 4], pos)*2 + 1)

        pix = self.get_pixmap8(new_coord1, new_coord2, new_coord3, new_coord4, autotile_num)
        return pix
                
original_palette = 'app/map_maker/palettes/westmarch'

Grass = GrassTerrain('Grass', 'Grass', original_palette, 'grass.png')
LightGrass = LightGrassTerrain('Light Grass', 'Light Grass', original_palette, 'grass.png')

Road = WangEdge2Terrain('Road', 'Road', original_palette, 'road.png')
Road.terrain_like = ('Sand', 'Road', 'BridgeH', 'BridgeV')

River = RiverTerrain('River', 'River', original_palette, 'river.png')
River.autotiles = \
    {(0, 8): 0, (0, 9): 1, (1, 6): 2, (1, 7): 3, (1, 8): 4, (1, 9): 5, (2, 6): 6, (2, 7): 7, (2, 8): 8, (2, 9): 9, (3, 0): 10, (3, 1): 11, (3, 2): 12,
     (3, 3): 13, (3, 8): 14, (3, 9): 15, (4, 6): 16, (4, 7): 17, (4, 8): 18, (4, 9): 19, (5, 8): 20, (5, 9): 21, (7, 0): 22, (7, 3): 23, (7, 4): 13, (7, 6): 24, 
     (7, 7): 25, (7, 8): 26, (7, 9): 27, (8, 6): 28, (8, 7): 29, (9, 0): 30, (9, 1): 31, (11, 1): 32, (11, 2): 33, (12, 0): 34, (12, 1): 35, (13, 6): 36, 
     (13, 7): 37, (13, 8): 38, (15, 0): 39, (15, 1): 39, (15, 2): 40, (15, 3): 40, (15, 4): 41, (15, 5): 41, (15, 6): 42, (15, 7): 42, (15, 8): 43, (15, 9): 43, 
     (1, 4): 44, (1, 5): 45, (2, 4): 46, (2, 5): 47, (4, 4): 48, (4, 5): 49, (8, 4): 50, (8, 5): 51}

Sand = SandTerrain('Sand', 'Sand', original_palette, 'sand.png')

Forest = ForestTerrain('Forest', 'Forest', original_palette, 'forest.png', (7, 0))

Thicket = RandomTerrain('Thicket', 'Thicket', original_palette, 'main.png', (17, 22))
Thicket.data = [(17, 22), (18, 22), (19, 22), (17, 23), (18, 23), (19, 23), (18, 24), (19, 24), (18, 25)]

Hill = HillTerrain('Hill', 'Hill', original_palette, 'main.png', (13, 21))

Mountain = MountainTerrain('Mountain', 'Mountain', original_palette, 'main.png', (8, 23))
Mountain.initial_process()

Cliff = CliffTerrain('Cliff', 'Cliff', original_palette, 'cliff.png', (15, 0))

Sea = SeaTerrain('Sea', 'Sea', original_palette, 'sea.png', (15, 0))
Sea.autotiles = \
    {(0, 4): 0, (0, 5): 1, (0, 6): 2, (0, 7): 2, (0, 8): 3, (0, 9): 3, (0, 10): 3, (0, 11): 2, (0, 12): 4, (0, 13): 2, (0, 14): 5, (0, 15): 3, (0, 16): 2,
     (0, 17): 4, (1, 0): 6, (1, 1): 7, (1, 4): 2, (1, 5): 3, (1, 6): 2, (1, 7): 2, (1, 8): 3, (1, 9): 3, (1, 10): 2, (1, 11): 3, (1, 12): 3, (1, 13): 3,
     (1, 14): 3, (1, 15): 3, (1, 16): 4, (1, 17): 2, (2, 1): 8, (2, 18): 9, (3, 0): 10, (3, 18): 11, (5, 18): 12, (6, 0): 13, (6, 2): 14, (6, 6): 15, 
     (6, 18): 9, (7, 1): 16, (7, 2): 17, (7, 3): 18, (7, 4): 19, (7, 6): 19, (7, 7): 20, (7, 18): 2, (7, 19): 21, (8, 1): 22, (9, 1): 23, (10, 0): 24, 
     (10, 1): 25, (10, 18): 26, (10, 19): 27, (10, 20): 28, (10, 21): 29, (11, 0): 30, (11, 1): 6, (12, 1): 31, (12, 3): 32, (12, 5): 33, (12, 7): 34, 
     (12, 19): 35, (13, 1): 36, (13, 2): 37, (13, 4): 38, (13, 5): 39, (13, 6): 40, (13, 7): 41, (13, 18): 42, (13, 19): 43, (14, 1): 44, (14, 2): 24, 
     (14, 3): 25, (14, 4): 45, (14, 5): 46, (14, 7): 25, (14, 18): 26, (14, 19): 27, (14, 20): 28, (14, 21): 29, (15, 2): 2, (15, 3): 43, (15, 4): 2, 
     (15, 5): 43, (15, 6): 2, (15, 7): 43, (15, 18): 2, (15, 19): 43, (15, 20): 2, (15, 21): 43, (16, 18): 47, (16, 19): 48, (18, 1): 49, (18, 3): 50, 
     (18, 4): 0, (18, 6): 0, (18, 7): 51, (18, 18): 0, (18, 19): 48, (19, 0): 52, (19, 6): 53, (19, 18): 11, (20, 18): 54, (20, 19): 55, (20, 20): 56, 
     (21, 18): 57, (21, 19): 58, (21, 20): 59, (22, 1): 60, (22, 2): 0, (22, 3): 61, (22, 5): 61, (22, 7): 60, (22, 10): 62, (22, 11): 53, (22, 18): 0, 
     (22, 19): 63, (22, 20): 0, (22, 21): 55, (23, 0): 2, (23, 1): 64, (23, 3): 64, (23, 4): 2, (23, 5): 65, (23, 7): 65, (23, 18): 2, (23, 19): 58, 
     (23, 20): 2, (23, 21): 58, (24, 1): 66, (24, 2): 67, (24, 4): 68, (24, 5): 69, (24, 6): 70, (24, 7): 71, (24, 18): 47, (24, 19): 62, (25, 1): 72, 
     (25, 3): 73, (25, 5): 74, (25, 7): 75, (25, 19): 76, (26, 2): 0, (26, 3): 62, (26, 4): 0, (26, 5): 62, (26, 6): 0, (26, 7): 62, (26, 18): 0, (26, 19): 62, 
     (26, 20): 0, (26, 21): 62, (27, 0): 10, (27, 2): 30, (27, 3): 6, (27, 4): 7, (27, 5): 77, (27, 6): 53, (27, 7): 68, (27, 18): 78, (27, 19): 79, 
     (27, 20): 80, (27, 21): 81, (28, 1): 82, (28, 18): 54, (28, 19): 62, (28, 20): 83, (28, 21): 62, (28, 22): 76, (28, 23): 43, (29, 18): 57, (29, 19): 43, 
     (29, 20): 57, (29, 21): 43, (29, 22): 35, (29, 23): 62, (30, 0): 0, (30, 1): 62, (30, 2): 0, (30, 3): 62, (30, 4): 0, (30, 5): 62, (30, 6): 0, (30, 7): 1, 
     (30, 8): 0, (30, 9): 62, (31, 0): 2, (31, 1): 43, (31, 2): 2, (31, 3): 43, (31, 4): 2, (31, 5): 3, (31, 6): 2, (31, 7): 43, (31, 8): 2, (31, 9): 43}

BridgeH = WangEdge2Terrain16('BridgeH', 'Bridge', original_palette, 'bridge_h.png', (0, 0))
BridgeH.terrain_like = ('BridgeH')
BridgeV = WangEdge2Terrain16('BridgeV', 'Bridge', original_palette, 'bridge_v.png', (0, 0))
BridgeV.terrain_like = ('BridgeV')

Castle = CastleTerrain('Castle', 'Castle', original_palette, 'main.png', (4, 27))
House = HouseTerrain('House', 'House', original_palette, 'main.png', (4, 25))
Ruins = RuinsTerrain('Ruins', 'Ruins', original_palette, 'main.png', (3, 28))

d = [Grass, LightGrass, Sand, Road, Forest, Thicket, Cliff, Hill, Mountain, River, Sea, BridgeH, BridgeV, House, Castle, Ruins]
DB_terrain = TerrainCatalog(d)
