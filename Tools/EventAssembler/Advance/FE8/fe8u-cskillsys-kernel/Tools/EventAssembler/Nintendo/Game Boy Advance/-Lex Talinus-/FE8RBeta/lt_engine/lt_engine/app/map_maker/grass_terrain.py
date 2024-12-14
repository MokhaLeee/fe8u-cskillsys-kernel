from PyQt5.QtGui import QPixmap, QPainter

from app.constants import TILEWIDTH, TILEHEIGHT
from app.map_maker.terrain import Terrain
from app.map_maker.wang_terrain import WangCorner2Terrain
from app.map_maker.utilities import random_choice


class GrassTerrain(Terrain):
    terrain_like = ('Grass', 'Light Grass')
    cliff_data = [(13, 9), (13, 10), (14, 9), (14, 10)]  # Topright, Bottomright, Bottomleft, Topleft
    limit = 48

    def get_display_pixmap(self) -> QPixmap:
        if not self.display_pixmap:
            main_pix = QPixmap(16, 16)
            painter = QPainter()
            painter.begin(main_pix)
            painter.drawPixmap(0, 0, self.tileset_pixmap.copy(0, 0, TILEWIDTH//2, TILEHEIGHT//2))
            painter.drawPixmap(0, TILEHEIGHT//2, self.tileset_pixmap.copy(0, 1 * TILEHEIGHT//2, TILEWIDTH//2, TILEHEIGHT//2))
            painter.drawPixmap(TILEWIDTH//2, 0, self.tileset_pixmap.copy(0, 2 * TILEHEIGHT//2, TILEWIDTH//2, TILEHEIGHT//2))
            painter.drawPixmap(TILEWIDTH//2, TILEHEIGHT//2, self.tileset_pixmap.copy(0, 3 * TILEHEIGHT//2, TILEWIDTH//2, TILEHEIGHT//2))
            painter.end()
            self.display_pixmap = main_pix
        return self.display_pixmap

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        new_coord1 = random_choice([(0, k) for k in range(self.limit)], pos)
        new_coord2 = random_choice([(0, k) for k in range(self.limit)], pos, offset=1)
        new_coord3 = random_choice([(0, k) for k in range(self.limit)], pos, offset=2)
        new_coord4 = random_choice([(0, k) for k in range(self.limit)], pos, offset=3)

        # Handle cliffs
        north, east, south, west = tilemap.get_cardinal_terrain(pos)
        northeast, southeast, southwest, northwest = tilemap.get_diagonal_terrain(pos)
        if north and north == 'Cliff' and east and east == 'Cliff' and not (northeast and northeast == 'Cliff'):
            new_coord2 = self.cliff_data[1]
        elif north and north == 'Cliff' and west and west == 'Cliff' and not (northwest and northwest == 'Cliff'):
            new_coord1 = self.cliff_data[3]
        elif south and south == 'Cliff' and east and east == 'Cliff' and not (southeast and southeast == 'Cliff'):
            new_coord3 = self.cliff_data[0]
        elif south and south == 'Cliff' and west and west == 'Cliff' and not (southwest and southwest == 'Cliff'):
            new_coord4 = self.cliff_data[2]
        # Handle seacliffs
        elif north and north == 'Sea' and east and east == 'Sea' and northeast and northeast == 'Sea':
            new_coord2 = self.cliff_data[1]
        elif north and north == 'Sea' and west and west == 'Sea' and northwest and northwest == 'Sea':
            new_coord1 = self.cliff_data[3]
        elif south and south == 'Sea' and east and east == 'Sea' and southeast and southeast == 'Sea':
            new_coord3 = self.cliff_data[0]
        elif south and south == 'Sea' and west and west == 'Sea' and southwest and southwest == 'Sea':
            new_coord4 = self.cliff_data[2]

        pix = self.get_pixmap8(new_coord1, new_coord2, new_coord3, new_coord4, autotile_num)
        return pix

class LightGrassTerrain(WangCorner2Terrain):
    terrain_like = ('Light Grass', )
    corner_chance = 0.8
    edge_chance = 0.6
    cliff_data = [(13, 9), (13, 10), (14, 9), (14, 10)]  # Topright, Bottomright, Bottomleft, Topleft
    vertices: dict = {}

    def get_display_pixmap(self):
        if not self.display_pixmap:
            main_pix = QPixmap(16, 16)
            painter = QPainter()
            painter.begin(main_pix)
            painter.drawPixmap(0, 0, self.tileset_pixmap.copy(15 * TILEWIDTH//2, 0, TILEWIDTH//2, TILEHEIGHT//2))
            painter.drawPixmap(0, TILEHEIGHT//2, self.tileset_pixmap.copy(15 * TILEWIDTH//2, 1 * TILEHEIGHT//2, TILEWIDTH//2, TILEHEIGHT//2))
            painter.drawPixmap(TILEWIDTH//2, 0, self.tileset_pixmap.copy(15 * TILEWIDTH//2, 2 * TILEHEIGHT//2, TILEWIDTH//2, TILEHEIGHT//2))
            painter.drawPixmap(TILEWIDTH//2, TILEHEIGHT//2, self.tileset_pixmap.copy(15 * TILEWIDTH//2, 3 * TILEHEIGHT//2, TILEWIDTH//2, TILEHEIGHT//2))
            painter.end()
            self.display_pixmap = main_pix
        return self.display_pixmap

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

        # Randomly determine some to remove
        if self.vertices[center][0] == 3 and self.vertices[center][1] < self.edge_chance:
            center_edge = False
        if self.vertices[center][0] == 2 and self.vertices[center][1] < self.corner_chance:
            center_edge = False
        if self.vertices[left][0] in (2, 3) and self.vertices[left][1] < self.edge_chance:
            left_edge = False
        if self.vertices[right][0] in (2, 3) and self.vertices[right][1] < self.edge_chance:
            right_edge = False
        if self.vertices[top][0] in (2, 3) and self.vertices[top][1] < self.edge_chance:
            top_edge = False
        if self.vertices[bottom][0] in (2, 3) and self.vertices[bottom][1] < self.edge_chance:
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
            
    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        index1, index2, index3, index4 = self._determine_index(tilemap, pos)

        new_coord1 = random_choice([(index1, k) for k in range(self.limits[index1])], pos)
        new_coord2 = random_choice([(index2, k) for k in range(self.limits[index2])], pos, offset=1)
        new_coord3 = random_choice([(index3, k) for k in range(self.limits[index3])], pos, offset=2)
        new_coord4 = random_choice([(index4, k) for k in range(self.limits[index4])], pos, offset=3)

        # Handle cliffs
        north, east, south, west = tilemap.get_cardinal_terrain(pos)
        northeast, southeast, southwest, northwest = tilemap.get_diagonal_terrain(pos)
        if north and north == 'Cliff' and east and east == 'Cliff' and not (northeast and northeast == 'Cliff'):
            new_coord2 = self.cliff_data[1]
        elif north and north == 'Cliff' and west and west == 'Cliff' and not (northwest and northwest == 'Cliff'):
            new_coord1 = self.cliff_data[3]
        elif south and south == 'Cliff' and east and east == 'Cliff' and not (southeast and southeast == 'Cliff'):
            new_coord3 = self.cliff_data[0]
        elif south and south == 'Cliff' and west and west == 'Cliff' and not (southwest and southwest == 'Cliff'):
            new_coord4 = self.cliff_data[2]
        # Handle seacliffs
        elif north and north == 'Sea' and east and east == 'Sea' and northeast and northeast == 'Sea':
            new_coord2 = self.cliff_data[1]
        elif north and north == 'Sea' and west and west == 'Sea' and northwest and northwest == 'Sea':
            new_coord1 = self.cliff_data[3]
        elif south and south == 'Sea' and east and east == 'Sea' and southeast and southeast == 'Sea':
            new_coord3 = self.cliff_data[0]
        elif south and south == 'Sea' and west and west == 'Sea' and southwest and southwest == 'Sea':
            new_coord4 = self.cliff_data[2]

        pix = self.get_pixmap8(new_coord1, new_coord2, new_coord3, new_coord4, autotile_num)
        return pix
