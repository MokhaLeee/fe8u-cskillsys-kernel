from PyQt5.QtGui import QPixmap, QPainter, qRgb

from app.constants import TILEWIDTH, TILEHEIGHT
from app.map_maker.terrain import Terrain
from app.map_maker.utilities import random_choice, random_random

class WangEdge2Terrain(Terrain):
    terrain_like = ()

    def set_tileset(self):
        super().set_tileset()
        self.limits = {k: self._find_limit(k) for k in range(16)}

    def _find_limit(self, idx: int) -> int:
        bg_color = qRgb(0, 0, 0)
        img = self.tileset_pixmap.toImage()
        x = idx * TILEWIDTH//2
        for y in range(0, img.height(), TILEHEIGHT//2):
            current_color = img.pixel(x, y)
            if current_color == bg_color:
                return y // (TILEHEIGHT//2)
        return (img.height() // (TILEHEIGHT//2))

    def get_display_pixmap(self):
        if not self.display_pixmap:
            main_pix = QPixmap(16, 16)
            painter = QPainter()
            painter.begin(main_pix)
            painter.drawPixmap(0, 0, self.tileset_pixmap.copy(TILEWIDTH//2 * 15, 0 * TILEHEIGHT//2, TILEWIDTH//2, TILEHEIGHT//2))
            painter.drawPixmap(0, TILEHEIGHT//2, self.tileset_pixmap.copy(TILEWIDTH//2 * 15, 1 * TILEHEIGHT//2, TILEWIDTH//2, TILEHEIGHT//2))
            painter.drawPixmap(TILEWIDTH//2, 0, self.tileset_pixmap.copy(TILEWIDTH//2 * 15, 2 * TILEHEIGHT//2, TILEWIDTH//2, TILEHEIGHT//2))
            painter.drawPixmap(TILEWIDTH//2, TILEHEIGHT//2, self.tileset_pixmap.copy(TILEWIDTH//2 * 15, 3 * TILEHEIGHT//2, TILEWIDTH//2, TILEHEIGHT//2))
            painter.end()
            self.display_pixmap = main_pix
        return self.display_pixmap

    def _determine_index(self, tilemap, pos: tuple) -> tuple:
        north, east, south, west = tilemap.get_cardinal_terrain(pos)
        north_edge = bool(not north or north in self.terrain_like)
        south_edge = bool(not south or south in self.terrain_like)
        east_edge = bool(not east or east in self.terrain_like)
        west_edge = bool(not west or west in self.terrain_like)
        index1 = 6 + 1 * north_edge + 8 * west_edge
        index2 = 12 + 1 * north_edge + 2 * east_edge
        index3 = 9 + 4 * south_edge + 2 * east_edge
        index4 = 3 + 4 * south_edge + 8 * west_edge
        return index1, index2, index3, index4

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        index1, index2, index3, index4 = self._determine_index(tilemap, pos)  
        new_coord1 = random_choice([(index1, k) for k in range(self.limits[index1])], pos)
        new_coord2 = random_choice([(index2, k) for k in range(self.limits[index2])], pos, offset=1)      
        new_coord3 = random_choice([(index3, k) for k in range(self.limits[index3])], pos, offset=2)      
        new_coord4 = random_choice([(index4, k) for k in range(self.limits[index4])], pos, offset=3)

        pix = self.get_pixmap8(new_coord1, new_coord2, new_coord3, new_coord4, autotile_num)
        return pix

class WangCorner2Terrain(WangEdge2Terrain):
    terrain_like = ()

    def _determine_index(self, tilemap, pos: tuple) -> tuple:
        north, east, south, west = tilemap.get_cardinal_terrain(pos)
        northeast, southeast, southwest, northwest = tilemap.get_diagonal_terrain(pos)
        north_edge = bool(not north or north in self.terrain_like)
        south_edge = bool(not south or south in self.terrain_like)
        east_edge = bool(not east or east in self.terrain_like)
        west_edge = bool(not west or west in self.terrain_like)
        index1 = 1 * north_edge + \
            2 * True + \
            4 * west_edge + \
            8 * (bool(not northwest or northwest in self.terrain_like) and north_edge and west_edge)
        index2 = 1 * (bool(not northeast or northeast in self.terrain_like) and north_edge and east_edge) + \
            2 * east_edge + \
            4 * True + \
            8 * north_edge
        index3 = 1 * east_edge + \
            2 * (bool(not southeast or southeast in self.terrain_like) and south_edge and east_edge) + \
            4 * south_edge + \
            8 * True
        index4 = 1 * True + \
            2 * south_edge + \
            4 * (bool(not southwest or southwest in self.terrain_like) and south_edge and west_edge) + \
            8 * west_edge
        return index1, index2, index3, index4

    def _pos_to_vertices(self, pos) -> tuple:
        center_vertex_pos = pos[0]*2 + 1, pos[1]*2 + 1
        left_vertex_pos = pos[0]*2, pos[1]*2 + 1
        right_vertex_pos = pos[0]*2 + 2, pos[1]*2 + 1
        top_vertex_pos = pos[0]*2 + 1, pos[1]*2
        bottom_vertex_pos = pos[0]*2 + 1, pos[1]*2 + 2
        topleft_vertex_pos = pos[0]*2, pos[1]*2
        topright_vertex_pos = pos[0]*2 + 2, pos[1]*2
        bottomleft_vertex_pos = pos[0]*2, pos[1]*2 + 2
        bottomright_vertex_pos = pos[0]*2 + 2, pos[1]*2 + 2
        return center_vertex_pos, left_vertex_pos, right_vertex_pos, \
            top_vertex_pos, bottom_vertex_pos, topleft_vertex_pos, \
            topright_vertex_pos, bottomleft_vertex_pos, bottomright_vertex_pos

    def get_edges(self, tilemap, pos):
        north, east, south, west = tilemap.get_cardinal_terrain(pos)
        north_edge = bool(not north or north in self.terrain_like)
        south_edge = bool(not south or south in self.terrain_like)
        east_edge = bool(not east or east in self.terrain_like)
        west_edge = bool(not west or west in self.terrain_like)
        northeast, southeast, southwest, northwest = tilemap.get_diagonal_terrain(pos)
        northeast_edge = bool(not northeast or northeast in self.terrain_like)
        southeast_edge = bool(not southeast or southeast in self.terrain_like)
        southwest_edge = bool(not southwest or southwest in self.terrain_like)
        northwest_edge = bool(not northwest or northwest in self.terrain_like)
        return north_edge, south_edge, east_edge, west_edge, northeast_edge, northwest_edge, southeast_edge, southwest_edge

    def determine_vertex(self, tilemap, pos):
        north_edge, south_edge, east_edge, west_edge, northeast_edge, northwest_edge, southeast_edge, southwest_edge = self.get_edges(tilemap, pos)
        # 0 is patch
        # 1 is end
        # 2 is corner (unless north and south or east and west, then end)
        # 3 is edge
        # 4 is center
        center_vertex_type = sum((north_edge, south_edge, east_edge, west_edge))
        if center_vertex_type == 2 and ((north_edge and south_edge) or (east_edge and west_edge)):
            center_vertex_type = 1
        # if not north: 0
        # if north: 0
        # if north and ((east and northeast) or (west and northwest)): edge
        # if north and both: center
        left_vertex_type = west_edge + (south_edge and southwest_edge) + (north_edge and northwest_edge)
        if left_vertex_type == 3:
            left_vertex_type = 4
        elif left_vertex_type == 2 and west_edge:
            left_vertex_type = 3
        else:
            left_vertex_type = west_edge
        right_vertex_type = east_edge + (south_edge and southeast_edge) + (north_edge and northeast_edge)
        if right_vertex_type == 3:
            right_vertex_type = 4
        elif right_vertex_type == 2 and east_edge:
            right_vertex_type = 3
        else:
            right_vertex_type = east_edge
        top_vertex_type = north_edge + (west_edge and northwest_edge) + (east_edge and northeast_edge)
        if top_vertex_type == 3:
            top_vertex_type = 4
        elif top_vertex_type == 2 and north_edge:
            top_vertex_type = 3
        else:
            top_vertex_type = north_edge
        bottom_vertex_type = south_edge + (west_edge and southwest_edge) + (east_edge and southeast_edge)
        if bottom_vertex_type == 3:
            bottom_vertex_type = 4
        elif bottom_vertex_type == 2 and south_edge:
            bottom_vertex_type = 3
        else:
            bottom_vertex_type = south_edge
        # 0 is not possible
        # 1 is empty
        # 2 is empty
        # 3 is empty
        # 4 is center
        topleft_vertex_type = 4 if (1 + sum((north_edge, west_edge, northwest_edge))) == 4 else 0
        bottomleft_vertex_type = 4 if (1 + sum((south_edge, west_edge, southwest_edge))) == 4 else 0
        topright_vertex_type = 4 if (1 + sum((north_edge, east_edge, northeast_edge))) == 4 else 0
        bottomright_vertex_type = 4 if (1 + sum((south_edge, east_edge, southeast_edge))) == 4 else 0

        center, left, right, top, bottom, topleft, topright, bottomleft, bottomright = self._pos_to_vertices(pos)

        self.vertices[center] = (center_vertex_type, random_random(center))
        self.vertices[left] = (left_vertex_type, random_random(left))
        self.vertices[right] = (right_vertex_type, random_random(right))
        self.vertices[top] = (top_vertex_type, random_random(top))
        self.vertices[bottom] = (bottom_vertex_type, random_random(bottom))
        self.vertices[topleft] = (topleft_vertex_type, random_random(topleft))
        self.vertices[topright] = (topright_vertex_type, random_random(topright))
        self.vertices[bottomleft] = (bottomleft_vertex_type, random_random(bottomleft))
        self.vertices[bottomright] = (bottomright_vertex_type, random_random(bottomright))

class WangEdge2Terrain16(WangEdge2Terrain):
    terrain_like = ()

    def _find_limit(self, idx: int) -> int:
        bg_color = qRgb(0, 0, 0)
        img = self.tileset_pixmap.toImage()
        x = idx * TILEWIDTH
        for y in range(0, img.height(), TILEHEIGHT):
            current_color = img.pixel(x, y)
            if current_color == bg_color:
                return y // TILEHEIGHT
        return (img.height() // TILEHEIGHT)

    def get_display_pixmap(self):
        if not self.display_pixmap:
            main_pix = QPixmap(16, 16)
            painter = QPainter()
            painter.begin(main_pix)
            painter.drawPixmap(0, 0, self.tileset_pixmap.copy(0, 0, TILEWIDTH, TILEHEIGHT))
            painter.end()
            self.display_pixmap = main_pix
        return self.display_pixmap

    def _determine_index(self, tilemap, pos: tuple) -> tuple:
        north, east, south, west = tilemap.get_cardinal_terrain(pos)
        north_edge = bool(not north or north in self.terrain_like)
        south_edge = bool(not south or south in self.terrain_like)
        east_edge = bool(not east or east in self.terrain_like)
        west_edge = bool(not west or west in self.terrain_like)
        index = 1 * north_edge + 2 * east_edge + 4 * south_edge + 8 * west_edge
        return index

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        index = self._determine_index(tilemap, pos)
        coord = random_choice([(index, k) for k in range(self.limits[index])], pos)
        return self.get_pixmap(coord, autotile_num)
