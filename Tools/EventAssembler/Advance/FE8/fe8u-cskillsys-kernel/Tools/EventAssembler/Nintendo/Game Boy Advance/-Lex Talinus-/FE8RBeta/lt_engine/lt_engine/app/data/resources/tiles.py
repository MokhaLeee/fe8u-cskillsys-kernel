import json
import logging
import os
from pathlib import Path
import re
import shutil
from typing import Dict, List

from app.constants import AUTOTILE_FRAMES, TILEHEIGHT, TILEWIDTH, TILEX, TILEY
from app.data.resources.base_catalog import ManifestCatalog
from app.utilities import str_utils
from app.utilities.data import Data, Prefab
from app.utilities.data_order import parse_order_keys_file, rchop


class TileMapPrefab(Prefab):
    def __init__(self, nid):
        self.nid = nid
        self.width, self.height = int(TILEX), int(TILEY)
        self.autotile_fps = 29
        self.layers = Data()
        self.layers.append(LayerGrid('base', self))

        self.pixmap = None

        self.tilesets = []  # Opened tilesets associated with this tilemap, nothing more
        self.image = None  # Icon used for drawing in resource editor

    def clear(self):
        self.width, self.height = int(TILEX), int(TILEY)
        self.layers.clear()
        self.layers.append(LayerGrid('base', self))

    def check_bounds(self, pos):
        return 0 <= pos[0] < self.width and 0 <= pos[1] < self.height

    def get_base_terrain(self, pos):
        layer = self.layers[0]
        if pos in layer.terrain_grid:
            return layer.terrain_grid[pos]
        return None

    def get_terrain(self, pos):
        for layer in reversed(self.layers):
            if layer.visible and pos in layer.terrain_grid:
                return layer.terrain_grid[pos]

    def resize(self, width, height, x_offset, y_offset):
        self.width = width
        self.height = height

        for layer in self.layers:
            # Terrain
            new_terrain_grid = {}
            for coord, terrain_nid in layer.terrain_grid.items():
                new_coord = coord[0] + x_offset, coord[1] + y_offset
                if self.check_bounds(new_coord):
                    new_terrain_grid[new_coord] = terrain_nid
            layer.terrain_grid = new_terrain_grid
            # Tile Sprites
            new_sprite_grid = {}
            for coord, tile_sprite in layer.sprite_grid.items():
                new_coord = coord[0] + x_offset, coord[1] + y_offset
                if self.check_bounds(new_coord):
                    new_sprite_grid[new_coord] = tile_sprite
            layer.sprite_grid = new_sprite_grid

    def save(self):
        s_dict = {}
        s_dict['nid'] = self.nid
        s_dict['size'] = self.width, self.height
        if self.width == 0 or self.height == 0:
            print("TileMap: Width or Height == 0!!!")
        s_dict['autotile_fps'] = self.autotile_fps
        s_dict['layers'] = [layer.save() for layer in self.layers]
        s_dict['tilesets'] = self.tilesets
        return s_dict

    @classmethod
    def restore(cls, s_dict):
        self = cls(s_dict['nid'])
        self.width, self.height = s_dict['size']
        self.autotile_fps = s_dict.get('autotile_fps', 29)
        self.tilesets = s_dict['tilesets']
        self.layers = Data([LayerGrid.restore(layer, self) for layer in s_dict['layers']])
        return self

    # Used only in tilemap editor
    def restore_edits(self, s_dict):
        self.width, self.height = s_dict['size']
        self.tilesets = s_dict['tilesets']
        self.layers = Data([LayerGrid.restore(layer, self) for layer in s_dict['layers']])
        return self

class TileSet(Prefab):
    def __init__(self, nid, full_path=None):
        self.nid = nid
        self.width, self.height = 0, 0
        self.terrain_grid = {}
        self.full_path = full_path

        self.pixmap = None
        self.subpixmaps = {}

        # For autotile handling
        self.autotiles = {}  # Key: Position Tuple, Value: column number
        self.autotile_full_path = None
        self.autotile_pixmap = None

        self.image = None
        self.autotile_image = None

    def check_bounds(self, pos):
        return 0 <= pos[0] < self.width and 0 <= pos[1] < self.height

    def set_autotile_pixmap(self, pixmap):
        self.autotile_pixmap = pixmap

    def set_pixmap(self, pixmap):
        self.pixmap = pixmap
        self.width = self.pixmap.width() // TILEWIDTH
        self.height = self.pixmap.height() // TILEHEIGHT
        # Subsurface
        self.subpixmaps.clear()
        for x in range(self.width):
            for y in range(self.height):
                p = self.pixmap.copy(x * TILEWIDTH, y * TILEHEIGHT, TILEWIDTH, TILEHEIGHT)
                self.subpixmaps[(x, y)] = p

    def get_pixmap(self, pos, ms=0, autotile_fps=29):
        if autotile_fps and pos in self.autotiles and self.autotile_pixmap:
            column = self.autotiles[pos]
            autotile_wait = int(autotile_fps * 16.66)
            num = (ms // autotile_wait) % AUTOTILE_FRAMES
            p = self.autotile_pixmap.copy(column * TILEWIDTH, num * TILEHEIGHT, TILEWIDTH, TILEHEIGHT)
            return p
        elif pos in self.subpixmaps:
            return self.subpixmaps[pos]
        return None

    def set_full_path(self, full_path):
        self.full_path = full_path

    def set_autotile_full_path(self, full_path):
        self.autotile_full_path = full_path

    def save(self):
        s_dict = {}
        s_dict['nid'] = self.nid
        s_dict['terrain_grid'] = {}
        for coord, terrain_nid in self.terrain_grid.items():
            str_coord = "%d,%d" % (coord[0], coord[1])
            s_dict['terrain_grid'][str_coord] = terrain_nid
        s_dict['autotiles'] = {}
        for coord, column in self.autotiles.items():
            str_coord = "%d,%d" % (coord[0], coord[1])
            s_dict['autotiles'][str_coord] = column
        return s_dict

    @classmethod
    def restore(cls, s_dict):
        self = cls(s_dict['nid'])
        for str_coord, terrain_nid in s_dict['terrain_grid'].items():
            coord = tuple(str_utils.intify(str_coord))
            self.terrain_grid[coord] = terrain_nid
        for str_coord, column in s_dict.get('autotiles', {}).items():
            coord = tuple(str_utils.intify(str_coord))
            self.autotiles[coord] = column
        return self

class LayerGrid(Prefab):
    def __init__(self, nid: str, parent):
        self.nid: str = nid
        self.parent = parent
        self.visible: bool = True
        self.foreground: bool = False
        self.terrain_grid = {}
        self.sprite_grid = {}

    def set(self, coord, tile, tile_sprite):
        self.terrain_grid[coord] = tile
        self.sprite_grid[coord] = tile_sprite

    def get_terrain(self, coord):
        return self.terrain_grid.get(coord)

    def get_sprite(self, coord):
        return self.sprite_grid.get(coord)

    def set_sprite(self, self_coord, tileset_nid, tileset_coord):
        tile_sprite = TileSprite(tileset_nid, tileset_coord, self)
        self.sprite_grid[self_coord] = tile_sprite

    def erase_sprite(self, coord):
        if coord in self.sprite_grid:
            del self.sprite_grid[coord]

    def erase_terrain(self, coord):
        if coord in self.terrain_grid:
            del self.terrain_grid[coord]

    def save(self):
        s_dict = {}
        s_dict['nid'] = self.nid
        s_dict['visible'] = self.visible
        s_dict['foreground'] = self.foreground
        s_dict['terrain_grid'] = {}
        for coord, terrain_nid in self.terrain_grid.items():
            str_coord = "%d,%d" % (coord[0], coord[1])
            s_dict['terrain_grid'][str_coord] = terrain_nid
        s_dict['sprite_grid'] = {}
        for coord, tile_sprite in self.sprite_grid.items():
            str_coord = "%d,%d" % (coord[0], coord[1])
            s_dict['sprite_grid'][str_coord] = tile_sprite.save()
        return s_dict

    @classmethod
    def restore(cls, s_dict, parent):
        self = cls(s_dict['nid'], parent)
        self.visible = s_dict['visible']
        self.foreground = s_dict.get('foreground', False)
        for str_coord, terrain_nid in s_dict['terrain_grid'].items():
            coord = tuple(int(_) for _ in str_coord.split(','))
            self.terrain_grid[coord] = terrain_nid
        for str_coord, data in s_dict['sprite_grid'].items():
            coord = tuple(int(_) for _ in str_coord.split(','))
            self.sprite_grid[coord] = TileSprite.restore(*data, self)
        return self

class TileSprite(Prefab):
    def __init__(self, tileset_nid, tileset_position, parent):
        self.parent = parent
        self.tileset_nid: str = tileset_nid
        self.tileset_position = tileset_position

    def save(self):
        return self.tileset_nid, self.tileset_position

    @classmethod
    def restore(cls, tileset_nid, tileset_position, parent):
        new_tile_sprite = cls(tileset_nid, tuple(tileset_position), parent)
        return new_tile_sprite

class TileSetCatalog(ManifestCatalog[TileSet]):
    manifest = 'tileset.json'
    title = 'tilesets'
    datatype = TileSet

    def load(self, loc):
        tileset_dict = self.read_manifest(os.path.join(loc, self.manifest))
        for s_dict in tileset_dict:
            new_tileset = TileSet.restore(s_dict)
            new_tileset.set_full_path(os.path.join(loc, new_tileset.nid + '.png'))
            if new_tileset.autotiles:
                new_tileset.set_autotile_full_path(os.path.join(loc, new_tileset.nid + '_autotiles.png'))
            self.append(new_tileset)

    def save(self, loc):
        for tileset in self:
            # Regular sprite
            new_full_path = os.path.join(loc, tileset.nid + '.png')
            if os.path.abspath(tileset.full_path) != os.path.abspath(new_full_path):
                shutil.copy(tileset.full_path, new_full_path)
                tileset.set_full_path(new_full_path)
            # Autotile sprite
            if tileset.autotiles and tileset.autotile_full_path:
                new_full_path = os.path.join(loc, tileset.nid + '_autotiles.png')
                if os.path.abspath(tileset.autotile_full_path) != os.path.abspath(new_full_path):
                    shutil.copy(tileset.autotile_full_path, new_full_path)
                    tileset.set_autotile_full_path(new_full_path)
        self.dump(loc)

    def valid_files(self) -> set:
        valid_filenames = {datum.nid + '.png' for datum in self}
        valid_filenames |= {datum.nid + '_autotiles.png' for datum in self}
        return valid_filenames

class TileMapCatalog(ManifestCatalog[TileMapPrefab]):
    manifest = 'tilemap.json'
    title = 'tilemaps'
    multi_loc = 'tilemap_data'

    def load(self, loc):
        single_loc = os.path.join(loc, self.manifest)
        multi_loc = os.path.join(loc, self.multi_loc)
        if not os.path.exists(multi_loc):  # old tilemap.json
            if not os.path.exists(single_loc):
                return
            tilemap_dict = self.read_manifest(single_loc)
            for s_dict in tilemap_dict:
                new_tilemap = TileMapPrefab.restore(s_dict)
                self.append(new_tilemap)
        else:   # new distributed saving
            data_fnames = os.listdir(multi_loc)
            save_data = []
            for fname in data_fnames:
                if not fname.endswith('.json'):
                    continue
                save_loc = os.path.join(multi_loc, fname)
                with open(save_loc) as load_file:
                    for data in json.load(load_file):
                        data['fname'] = os.path.basename(fname)
                        save_data.append(data)
            if '.orderkeys' in data_fnames:  # using order key file
                ordering = parse_order_keys_file(Path(multi_loc, '.orderkeys'))
                save_data = sorted(save_data, key=lambda data: ordering.index(rchop(data['fname'], '.json')) if rchop(data['fname'], '.json') in ordering else 99999)
            else:  # using order keys per object
                save_data = sorted(save_data, key=lambda obj: obj['_orderkey'])
            for s_dict in save_data:
                new_tilemap = TileMapPrefab.restore(s_dict)
                self.append(new_tilemap)

    def dump(self, loc):
        saves = [datum.save() for datum in self]
        save_dir = os.path.join(loc, self.multi_loc)
        if os.path.exists(save_dir):
            shutil.rmtree(save_dir)
        os.mkdir(save_dir)
        orderkeys: List[str] = []
        for idx, save in enumerate(saves):
            # ordering
            nid = save['nid']
            nid = re.sub(r'[\\/*?:"<>|]', "", nid)
            nid = nid.replace(' ', '_')
            fname = nid + '.json'
            orderkeys.append(fname)
            save_loc = os.path.join(save_dir, nid + '.json')
            with open(save_loc, 'w') as serialize_file:
                json.dump([save], serialize_file, indent=4)
        with open(os.path.join(save_dir, '.orderkeys'), 'w') as orderkey_file:
            json.dump(orderkeys, orderkey_file, indent=4)

    def save(self, loc):
        # No need to finagle with full paths
        # Because TileMaps are don't have any connection to any actual file.
        # They are all references to TileSets
        import time
        start = time.time_ns()/1e6
        self.dump(loc)
        end = time.time_ns()/1e6
        print("Time Taken: %s ms" % (end - start))
