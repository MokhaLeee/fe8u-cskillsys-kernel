from PyQt5.QtCore import QDateTime
from PyQt5.QtGui import QImage, QPainter, QPixmap, QColor

from app.constants import TILEWIDTH, TILEHEIGHT, AUTOTILE_FRAMES

from app.map_maker.map_prefab import MapPrefab
from app.map_maker.terrain_database import DB_terrain

def get_tilemap_pixmap(tilemap) -> QPixmap:
    return QPixmap.fromImage(draw_tilemap(tilemap))

last_autotile_num = 0

def draw_tilemap(tilemap: MapPrefab, autotile_fps=29) -> QImage:
    # import time
    # start = time.time_ns() / 1e6
    image = QImage(tilemap.width * TILEWIDTH,
                   tilemap.height * TILEHEIGHT,
                   QImage.Format_ARGB32)
    image.fill(QColor(0, 0, 0, 0))

    painter = QPainter()
    painter.begin(image)
    ms = QDateTime.currentMSecsSinceEpoch()

    if autotile_fps:
        autotile_wait = autotile_fps * 16.66
        autotile_num = int(ms / autotile_wait) % AUTOTILE_FRAMES
    else:
        autotile_num = 0

    # Process terrain
    # a = time.time_ns() / 1e6
    processed_nids = set()
    for pos in sorted(tilemap.terrain_grid):
        # Determine what terrain is in this position
        terrain_nid = tilemap.get_terrain(pos)
        if not terrain_nid:
            continue
        terrain = DB_terrain.get(terrain_nid)

        # Only process the ones that need to be updated
        if pos in tilemap.terrain_grid_to_update:
            if terrain_nid not in processed_nids:
                terrain.single_process(tilemap)
                processed_nids.add(terrain_nid)
            sprite = terrain.determine_sprite(tilemap, pos, autotile_num)
            tilemap.tile_grid[pos] = sprite

    # b = time.time_ns() / 1e6

    # Autotiles
    global last_autotile_num
    if autotile_num != last_autotile_num:
        for pos in sorted(tilemap.autotile_set):
            if pos not in tilemap.terrain_grid_to_update:
                # Determine what terrain is in this position
                terrain_nid = tilemap.get_terrain(pos)
                if not terrain_nid:
                    continue
                terrain = DB_terrain.get(terrain_nid)
                sprite = terrain.determine_sprite(tilemap, pos, autotile_num)
                tilemap.tile_grid[pos] = sprite
        last_autotile_num = autotile_num
    # c = time.time_ns() / 1e6

    # Draw the tile grid
    for pos, pix in tilemap.tile_grid.items():
        assert pix.width() == TILEWIDTH, pix.width()
        assert pix.height() == TILEHEIGHT, pix.height()
        painter.drawPixmap(pos[0] * TILEWIDTH,
                           pos[1] * TILEHEIGHT,
                           pix)
    painter.end()

    # Make sure we don't need to update it anymore
    tilemap.terrain_grid_to_update.clear()
    # end = time.time_ns() / 1e6

    # print('%.1f %.1f %.1f %.1f' % ((end - start, b - a, c - b, end - c)))

    return image

def simple_draw_tilemap(tilemap: MapPrefab) -> QImage:
    image = QImage(tilemap.width * TILEWIDTH,
                   tilemap.height * TILEHEIGHT,
                   QImage.Format_ARGB32)
    image.fill(QColor(0, 0, 0, 0))

    painter = QPainter()
    painter.begin(image)

    # Draw the tile grid
    for pos, pix in tilemap.tile_grid.items():
        assert pix.width() == TILEWIDTH, pix.width()
        assert pix.height() == TILEHEIGHT, pix.height()
        painter.drawPixmap(pos[0] * TILEWIDTH,
                           pos[1] * TILEHEIGHT,
                           pix)
    painter.end()
    return image
