from PyQt5.QtGui import QPixmap, QPainter, qRgb

from app.constants import TILEWIDTH, TILEHEIGHT
from app.map_maker.utilities import random_choice, random_random, edge_random, get_random_seed
from app.map_maker.wang_terrain import WangEdge2Terrain
from app.utilities import utils

class SeaTerrain(WangEdge2Terrain):
    terrain_like = ('Sea', 'River', 'BridgeH', 'BridgeV')
    serration_chance = 0.6
    sand_start_px = 144
    sand_limits = {}

    @property
    def check_flood_fill(self):
        return True

    def set_tileset(self):
        super().set_tileset()
        self.limits = {k: self._find_limit(k) for k in range(16)}
        self.sand_limits = {k: self._find_limit(k, self.sand_start_px) for k in range(16)}

    def _find_limit(self, idx: int, offset: int = 0) -> int:
        bg_color = qRgb(0, 0, 0)
        img = self.tileset_pixmap.toImage()
        x = idx * TILEWIDTH
        for y in range(offset, img.height(), TILEHEIGHT):
            current_color = img.pixel(x, y)
            if current_color == bg_color:
                return (y - offset) // TILEHEIGHT
        return (img.height() // TILEHEIGHT)

    def get_display_pixmap(self):
        if not self.display_pixmap:
            main_pix = QPixmap(16, 16)
            painter = QPainter()
            painter.begin(main_pix)
            painter.drawPixmap(0, 0, self.tileset_pixmap.copy(TILEWIDTH * 15, 0, TILEWIDTH, TILEHEIGHT))
            painter.end()
            self.display_pixmap = main_pix
        return self.display_pixmap

    def _determine_index(self, tilemap, pos: tuple) -> int:
        north, east, south, west = tilemap.get_cardinal_terrain(pos)
        north_edge = bool(not north or north in self.terrain_like)
        south_edge = bool(not south or south in self.terrain_like)
        east_edge = bool(not east or east in self.terrain_like)
        west_edge = bool(not west or west in self.terrain_like)
        index = 1 * north_edge + 2 * east_edge + 4 * south_edge + 8 * west_edge
        return index

    def _near_sand(self, tilemap, pos: tuple) -> bool:
        return any(x == 'Sand' for x in tilemap.get_cardinal_terrain(pos))

    def _modify_index(self, index: int, tilemap, pos: tuple) -> int:
        # For randomly serrating the straight edges of the sea
        odd = bool((pos[0] + pos[1] + get_random_seed()) % 2)
        even = not odd
        # Left
        if index == 13:
            if odd and self._determine_index(tilemap, (pos[0], pos[1] - 1)) == 13 and edge_random((pos[0], pos[1] - 1), pos) < self.serration_chance:
                index = 12
            elif even and self._determine_index(tilemap, (pos[0], pos[1] + 1)) == 13 and edge_random(pos, (pos[0], pos[1] + 1)) < self.serration_chance:
                index = 9
        # Right
        elif index == 7:
            if odd and self._determine_index(tilemap, (pos[0], pos[1] - 1)) == 7 and edge_random((pos[0], pos[1] - 1), pos) < self.serration_chance:
                index = 6
            elif even and self._determine_index(tilemap, (pos[0], pos[1] + 1)) == 7 and edge_random(pos, (pos[0], pos[1] + 1)) < self.serration_chance:
                index = 3
        # Top
        elif index == 11:
            if odd and self._determine_index(tilemap, (pos[0] - 1, pos[1])) == 11 and edge_random((pos[0] - 1, pos[1]), pos) < self.serration_chance:
                index = 3
            elif even and self._determine_index(tilemap, (pos[0] + 1, pos[1])) == 11 and edge_random(pos, (pos[0] + 1, pos[1])) < self.serration_chance:
                index = 9
        # Bottom
        elif index == 14:
            if odd and self._determine_index(tilemap, (pos[0] - 1, pos[1])) == 14 and edge_random((pos[0] - 1, pos[1]), pos) < self.serration_chance:
                index = 6
            elif even and self._determine_index(tilemap, (pos[0] + 1, pos[1])) == 14 and edge_random(pos, (pos[0] + 1, pos[1])) < self.serration_chance:
                index = 12

        return index

    def _distance_to_closest(self, tilemap, pos: tuple) -> float:
        min_distance = 99
        for other_pos in tilemap.terrain_grid.keys():
            if tilemap.get_terrain(other_pos) not in ('Sea', 'BridgeV', 'BridgeH'):
                distance = utils.distance(pos, other_pos)
                if distance < min_distance:
                    min_distance = distance
        return min_distance

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        index = self._determine_index(tilemap, pos)
        index = self._modify_index(index, tilemap, pos)
        if index == 15:
            dist = self._distance_to_closest(tilemap, pos)
            if dist > (2 + 2*random_random(pos)): # Open Sea (adds random number between 0 and 1 for rng)
                # Measure distance to closest non sea, bridge terrain
                new_coords = [(0, k) for k in range(2, 9)]
            else:
                new_coords = [(index, k) for k in range(self.limits[index])]
        else:
            if self._near_sand(tilemap, pos):
                new_coords = [(index, k + self.sand_start_px//TILEHEIGHT) for k in range(self.sand_limits[index])]
            else:
                new_coords = [(index, k) for k in range(self.limits[index])]

        coord = random_choice(new_coords, pos)
        # Even though this should be able to use the 16x16
        # pixmaps, the sea autotiles are not set up to use 
        # 16x16, but 8x8, so we need to do this
        coord1 = (coord[0] * 2, coord[1] * 2)
        coord2 = (coord[0] * 2 + 1, coord[1] * 2)
        coord3 = (coord[0] * 2 + 1, coord[1] * 2 + 1)
        coord4 = (coord[0] * 2, coord[1] * 2 + 1)
        pix = self.get_pixmap8(coord1, coord2, coord3, coord4, autotile_num)
        return pix
