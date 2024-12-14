from PyQt5.QtWidgets import QFileDialog, QMessageBox
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap, QImage, QPainter, QIcon, QColor

import os

from app.constants import TILEWIDTH, TILEHEIGHT
from app.data.resources.resources import RESOURCES
from app.data.resources.tiles import TileSet, TileMapPrefab

from app.utilities.data import Data
from app.data.database.database import DB

from app.editor.base_database_gui import ResourceCollectionModel
from app.extensions.custom_gui import DeletionTab, DeletionDialog
from app.editor.tilemap_editor import MapEditor
from app.editor.settings import MainSettingsController

from app.utilities import str_utils

class TileSetModel(ResourceCollectionModel):
    def __init__(self, data, window):
        super().__init__(data, window)
        for tileset in self._data:
            tileset.set_pixmap(QPixmap(tileset.full_path))

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole or role == Qt.EditRole:
            tileset = self._data[index.row()]
            text = tileset.nid
            return text
        elif role == Qt.DecorationRole:
            tileset = self._data[index.row()]
            pixmap = tileset.pixmap
            pix = pixmap.scaled(96, 96)
            # pix = pixmap
            return QIcon(pix)
        return None

    def create_new(self):
        settings = MainSettingsController()
        starting_path = settings.get_last_open_path()
        fns, ok = QFileDialog.getOpenFileNames(self.window, "Choose %s", starting_path, "PNG Files (*.png);;All Files(*)")
        new_tileset = None
        if ok:
            for fn in fns:
                if fn.endswith('.png'):
                    nid = os.path.split(fn)[-1][:-4]
                    pix = QPixmap(fn)
                    nid = str_utils.get_next_name(nid, RESOURCES.tilesets.keys())
                    if pix.width() % TILEWIDTH != 0:
                        QMessageBox.critical(self.window, 'Error', "Image width must be exactly divisible by %d pixels!" % TILEWIDTH)
                        continue
                    elif pix.height() % TILEHEIGHT != 0:
                        QMessageBox.critical(self.window, 'Error', "Image height must be exactly divisible by %d pixels!" % TILEHEIGHT)
                        continue
                    new_tileset = TileSet(nid, fn)
                    new_tileset.set_pixmap(pix)
                    RESOURCES.tilesets.append(new_tileset)
                else:
                    QMessageBox.critical(self.window, "File Type Error!", "Tileset must be PNG format!")
            parent_dir = os.path.split(fns[-1])[0]
            settings.set_last_open_path(parent_dir)
        return new_tileset

    def delete(self, idx):
        # Check to see what is using me?
        res = self._data[idx]
        nid = res.nid
        affected_tilemaps = [tilemap for tilemap in RESOURCES.tilemaps if nid in tilemap.tilesets]
        if affected_tilemaps:
            model = TileMapModel
            msg = "Deleting Tileset <b>%s</b> would affect these tilemaps." % nid
            deletion_tab = DeletionTab(affected_tilemaps, model, msg, "Tilemaps")
            ok = DeletionDialog.inform([deletion_tab], self.window)
            if ok:
                self.delete_tileset_from_tilemaps(nid)
            else:
                return
        super().delete(idx)

    def delete_tileset_from_tilemaps(self, tileset_nid):
        # What uses tilesets
        # Tilemaps use tilesets
        for tilemap in RESOURCES.tilemaps:
            if tileset_nid in tilemap.tilesets:
                tilemap.tilesets.remove(tileset_nid)
            for layer in tilemap.layers:
                for coord, tile_sprite in list(layer.sprite_grid.items()):
                    if tile_sprite.tileset_nid == tileset_nid:
                        # Delete all places that tileset is used
                        del layer.sprite_grid[coord]

    def on_nid_changed(self, old_nid, new_nid):
        # What uses tilesets
        # Tilemaps use tilesets
        for tilemap in RESOURCES.tilemaps:
            for idx, nid in enumerate(tilemap.tilesets):
                if nid == old_nid:
                    tilemap.tilesets[idx] = new_nid
            for layer in tilemap.layers:
                for coord, tile_sprite in layer.sprite_grid.items():
                    if tile_sprite.tileset_nid == old_nid:
                        tile_sprite.tileset_nid = new_nid

def create_tilemap_pixmap(tilemap):
    base_layer = tilemap.layers.get('base')
    image = QImage(tilemap.width * TILEWIDTH,
                   tilemap.height * TILEHEIGHT,
                   QImage.Format_ARGB32)
    image.fill(QColor(0, 0, 0, 255))

    painter = QPainter()
    painter.begin(image)
    for coord, tile_sprite in base_layer.sprite_grid.items():
        tileset = RESOURCES.tilesets.get(tile_sprite.tileset_nid)
        if not tileset:
            continue
        if not tileset.pixmap:
            tileset.set_pixmap(QPixmap(tileset.full_path))
        pix = tileset.get_pixmap(tile_sprite.tileset_position)
        if pix:
            painter.drawImage(coord[0] * TILEWIDTH,
                              coord[1] * TILEHEIGHT,
                              pix.toImage())
    painter.end()
    tilemap.pixmap = QPixmap.fromImage(image)
    return tilemap.pixmap

class TileMapModel(ResourceCollectionModel):
    def __init__(self, data, window):
        super().__init__(data, window)
        for tilemap in self._data:
            create_tilemap_pixmap(tilemap)

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole or role == Qt.EditRole:
            tilemap = self._data[index.row()]
            text = tilemap.nid
            return text
        elif role == Qt.DecorationRole:
            tilemap = self._data[index.row()]
            pixmap = tilemap.pixmap
            if pixmap:
                pix = pixmap.scaled(96, 96)
                return QIcon(pix)
        return None

    def create_new(self):
        new_nid = str_utils.get_next_name('New Tilemap', self._data.keys())
        new_tilemap = TileMapPrefab(new_nid)
        map_editor = MapEditor(self.window, new_tilemap)
        map_editor.exec_()
        create_tilemap_pixmap(new_tilemap)
        RESOURCES.tilemaps.append(new_tilemap)
        self.layoutChanged.emit()

    def delete(self, idx):
        # Check to see what is using me?
        res = self._data[idx]
        nid = res.nid
        affected_levels = [level for level in DB.levels if level.tilemap == nid]
        if affected_levels:
            from app.editor.global_editor.level_menu import LevelModel
            model = LevelModel
            msg = "Deleting Tilemap <b>%s</b> would affect these levels." % nid
            deletion_tab = DeletionTab(affected_levels, model, msg, "Levels")
            ok = DeletionDialog.inform([deletion_tab], self.window)
            if ok:
                pass
            else:
                return
        super().delete(idx)

    def on_nid_changed(self, old_nid, new_nid):
        # What uses tilemaps
        # Levels use tilemaps
        for level in DB.levels:
            if level.tilemap == old_nid:
                level.tilemap = new_nid

    def duplicate(self, tilemap):
        idx = self._data.index(tilemap.nid)
        new_nid = str_utils.get_next_name(tilemap.nid, self._data.keys())
        # Duplicate by serializing and then deserializing
        ser_tilemap = tilemap.save()
        new_tilemap = TileMapPrefab.restore(ser_tilemap)
        new_tilemap.nid = new_nid
        self._data.insert(idx + 1, new_tilemap)
        create_tilemap_pixmap(new_tilemap)
        self.layoutChanged.emit()
