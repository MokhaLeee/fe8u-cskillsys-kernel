from typing import List, Tuple
from PyQt5 import QtCore
from app.constants import TILEHEIGHT, TILEWIDTH
from app.data.database.database import DB
from app.data.database.overworld import OverworldPrefab
from app.editor.map_view import SimpleMapView
from app.editor.overworld_editor.road_sprite_wrapper import RoadSpriteWrapper
# from app.editor.tile_editor import tile_model
from app.editor.tilemap_editor import get_tilemap_pixmap
from app.data.resources.resources import RESOURCES
from app.sprites import SPRITES
from app.utilities.typing import Point
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QColor, QImage, QPainter, QPen, QPixmap, QTransform


class WorldMapView(SimpleMapView):
    def __init__(self, window=None):
        super().__init__(window)
        self.selected = None
        self.ghost_road_points: List[Point] = None
        self.road_sprite = RoadSpriteWrapper()
        self.should_draw_ghost = False

    def set_current_level(self, overworld_nid):
        overworld = DB.overworlds.get(overworld_nid)
        if isinstance(overworld, OverworldPrefab):
            self.current_level = overworld
            self.current_map = RESOURCES.tilemaps.get(overworld.tilemap)
            self.update_view()

    def set_selected(self, sel):
        self.selected = sel
        self.update_view()

    def set_ghost_road_endpoint(self, ghost: List[Point]):
        self.ghost_road_points = ghost
        self.update_view()

    def update_view(self, _=None):
        if(self.current_level and not self.current_map):
            self.current_map = RESOURCES.tilemaps.get(
                self.current_level.tilemap)
        if self.current_map:
            pixmap = get_tilemap_pixmap(self.current_map)
            self.working_image = pixmap
        else:
            self.clear_scene()
            return
        self.paint_roads(self.current_level)
        self.paint_nodes(self.current_level)
        self.paint_selected()
        self.paint_border(self.current_level)
        self.show_map()

    def paint_border(self, current_level: OverworldPrefab):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            pixel_border_width = TILEWIDTH * current_level.border_tile_width
            # draw top and left borders
            painter.fillRect(0, 0, self.working_image.width(), pixel_border_width, QColor(160, 0, 0, 128))
            painter.fillRect(0, 0, pixel_border_width, self.working_image.height(), QColor(160, 0, 0, 128))
            # draw bottom and right borders
            painter.fillRect(0, self.working_image.height() - pixel_border_width, self.working_image.width(), pixel_border_width, QColor(160, 0, 0, 128))
            painter.fillRect(self.working_image.width() - pixel_border_width, 0, pixel_border_width, self.working_image.height(), QColor(160, 0, 0, 128))
            painter.end()

    def draw_node(self, painter, node, position, opacity=False):
        icon_nid = node.icon
        icon = RESOURCES.map_icons.get(icon_nid)
        if not icon:
            return
        coord = position
        pixmap = icon.get_pixmap()
        # to support 16x16, 32x32, and 48x48 map icons, we offset them differently
        offset_x = int((pixmap.width() / 16 - 1) * 8)
        offset_y = (pixmap.height() - 16)
        if pixmap:
            if opacity:
                painter.setOpacity(0.33)
            painter.drawImage(coord[0] * TILEWIDTH - offset_x,
                              coord[1] * TILEHEIGHT - offset_y, pixmap.toImage())
            painter.setOpacity(1.0)
        else:
            pass

    def draw_road_segment(self, painter, start_position, end_position, selected=False, transparent=False, ghost=False):
        start_x = start_position[0] * TILEWIDTH + TILEWIDTH // 2
        start_y = start_position[1] * TILEHEIGHT + TILEHEIGHT // 2
        end_x = end_position[0] * TILEWIDTH + TILEWIDTH // 2
        end_y = end_position[1] * TILEHEIGHT + TILEHEIGHT // 2

        # if this is our current working line, draw an accent to let the user know
        if selected:
            pen = QPen(Qt.yellow, 3, style=Qt.SolidLine)
            painter.setRenderHint(QPainter.Antialiasing)
            painter.setPen(pen)
            painter.drawLine(start_x, start_y, end_x, end_y)

        # only draw the ghost road with qt
        if ghost:
            # draw the road segment
            if transparent:
                pen = QPen(QColor(256, 0, 256, 80), 3, style=Qt.SolidLine)
            else:
                pen = QPen(QColor(232, 216, 136, 160), 3, style=Qt.SolidLine)
            painter.setRenderHint(QPainter.Antialiasing)
            painter.setPen(pen)
            painter.drawLine(start_x, start_y, end_x, end_y)
            pen = QPen(QColor(196, 2, 51), 2, style=Qt.DotLine)
            painter.setRenderHint(QPainter.Antialiasing)
            painter.setPen(pen)
            painter.drawLine(start_x, start_y, end_x, end_y)

    def paint_nodes(self, current_level):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            for node in current_level.overworld_nodes:
                if not node.pos:
                    continue
                if not node.icon:
                    continue
                self.draw_node(painter, node, node.pos)
            painter.end()

    def paint_roads(self, current_level):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            for path in current_level.map_paths.values():
                unpacked_path = RoadSpriteWrapper.road_to_full_points_list(path)
                for i in range(len(unpacked_path)):
                    neighbors = []
                    if i != 0:
                        neighbors.append(unpacked_path[i - 1])
                    if i < len(unpacked_path) - 1:
                        neighbors.append(unpacked_path[i + 1])
                    self.road_sprite.draw_tile(painter, unpacked_path[i], neighbors)
            painter.end()

    def paint_ghost_road(self, selected):
        if self.should_draw_ghost:
            if isinstance(selected, list) and len(selected) > 1:
                last_road_point = selected[-1]
            elif isinstance(selected, tuple):
                last_road_point = selected
            else:
                return

            painter = QPainter()
            painter.begin(self.working_image)
            if last_road_point and self.ghost_road_points:
                self.draw_road_segment(painter, last_road_point, self.ghost_road_points[0], transparent=True, ghost=True)
                if len(self.ghost_road_points) == 2 and not self.ghost_road_points[0] == self.ghost_road_points[1]:
                    self.draw_road_segment(painter, self.ghost_road_points[0], self.ghost_road_points[1], transparent=True, ghost=True)
            painter.end()

    def paint_selected(self):
        """Draws some sort of accent around the selected object (road, node).
           For the road, draws highlights.
           For the node, draws a cursor around it.
        """
        if self.working_image:
            if isinstance(self.selected, list):
                # this is a road
                self.paint_selected_road(self.selected)
                self.paint_ghost_road(self.selected)
            elif isinstance(self.selected, tuple):
                # this is a selected coord of a node
                self.paint_cursor(self.selected)
                self.paint_ghost_road(self.selected)
            else:
                # ??? None type, or something went wrong. Don't draw
                return

    def paint_selected_road(self, path):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            for i in range(len(path) - 1):
                self.draw_road_segment(painter, path[i], path[i+1], True)
            painter.end()

    def paint_cursor(self, coord):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            coord = coord
            cursor_sprite = SPRITES['cursor']
            if cursor_sprite:
                if not cursor_sprite.pixmap:
                    cursor_sprite.pixmap = QPixmap(cursor_sprite.full_path)
                cursor_image = cursor_sprite.pixmap.toImage().copy(0, 64, 32, 32)
                painter.drawImage(
                    coord[0] * TILEWIDTH - 8, coord[1] * TILEHEIGHT - 5, cursor_image)
            painter.end()

    def show_map(self):
        if self.working_image:
            self.clear_scene()
            self.scene.addPixmap(self.working_image)

    # these two are in the superclass but are useless in this context, override just in case
    def paint_units(self, current_level):
        pass

    def draw_unit(self, painter, unit, position, opacity=False):
        pass
