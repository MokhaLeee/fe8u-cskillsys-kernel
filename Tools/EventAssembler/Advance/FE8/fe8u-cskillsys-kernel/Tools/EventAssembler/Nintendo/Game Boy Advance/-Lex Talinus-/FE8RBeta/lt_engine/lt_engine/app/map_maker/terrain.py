import os
from dataclasses import dataclass

from PyQt5.QtGui import QPixmap, QPainter

from app.constants import TILEWIDTH, TILEHEIGHT
from app.utilities.data import Data, Prefab

@dataclass
class Terrain(Prefab):
    nid: str = None
    name: str = None
    palette_path: str = None
    tileset_path: str = None

    display_tile_coord: tuple = (0, 0)

    display_pixmap: QPixmap = None
    tileset_pixmap: QPixmap = None

    autotiles: dict = None
    autotile_pixmap: QPixmap = None

    def has_autotiles(self) -> bool:
        return self.autotiles and self.autotile_pixmap

    @property
    def check_flood_fill(self) -> bool:
        return False

    def set_tileset(self):
        full_path = os.path.join(self.palette_path, self.tileset_path)
        self.tileset_pixmap = QPixmap(full_path)
        autotile_path = full_path[:-4] + '_autotiles.png'
        if os.path.exists(autotile_path):
            self.autotile_pixmap = QPixmap(autotile_path)
        self.display_pixmap = None
        self.get_display_pixmap()

    def get_display_pixmap(self) -> QPixmap:
        if not self.display_pixmap:
            pix = self.tileset_pixmap.copy(
                self.display_tile_coord[0] * TILEWIDTH, 
                self.display_tile_coord[1] * TILEHEIGHT,
                TILEWIDTH, TILEHEIGHT)
            self.display_pixmap = pix
        return self.display_pixmap
        
    def restore_attr(self, name, value):
        if name in ('tileset_path', 'display_tile_coord'):
            value = tuple(value)
        else:
            value = super().restore_attr(name, value)
        return value

    def _subsurface8(self, coord: tuple, autotile_num: int) -> QPixmap:
        if self.has_autotiles() and coord in self.autotiles:
            column = self.autotiles[coord]
            pix = self.autotile_pixmap.copy(
                column * TILEWIDTH//2, 
                autotile_num * TILEHEIGHT//2, 
                TILEWIDTH//2, TILEHEIGHT//2)
        else:
            pix = self.tileset_pixmap.copy(
                coord[0] * TILEWIDTH//2, 
                coord[1] * TILEHEIGHT//2, 
                TILEWIDTH//2, TILEHEIGHT//2)
        return pix

    def get_pixmap8(self, coord1: tuple, coord2: tuple, coord3: tuple, coord4: tuple, autotile_num: int) -> QPixmap:
        base_pixmap = QPixmap(TILEWIDTH, TILEHEIGHT)
        topleft = self._subsurface8(coord1, autotile_num)
        topright = self._subsurface8(coord2, autotile_num)
        bottomright = self._subsurface8(coord3, autotile_num)
        bottomleft = self._subsurface8(coord4, autotile_num)
        painter = QPainter()
        painter.begin(base_pixmap)
        painter.drawPixmap(0, 0, topleft)
        painter.drawPixmap(TILEWIDTH//2, 0, topright)
        painter.drawPixmap(0, TILEHEIGHT//2, bottomleft)
        painter.drawPixmap(TILEWIDTH//2, TILEHEIGHT//2, bottomright)
        painter.end()
        return base_pixmap

    def get_pixmap(self, tile_coord: tuple, autotile_num: int) -> QPixmap:
        if self.has_autotiles() and tile_coord in self.autotiles:
            column = self.autotiles[tile_coord]
            pix = self.autotile_pixmap.copy(
                column * TILEWIDTH, 
                autotile_num * TILEHEIGHT, 
                TILEWIDTH, TILEHEIGHT)
        else:
            pix = self.tileset_pixmap.copy(
                tile_coord[0] * TILEWIDTH,
                tile_coord[1] * TILEHEIGHT,
                TILEWIDTH, TILEHEIGHT)
        return pix

    def single_process(self, tilemap):
        pass

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        coord = self.display_tile_coord
        return self.get_pixmap(coord, autotile_num)

class TerrainCatalog(Data[Terrain]):
    datatype = Terrain
