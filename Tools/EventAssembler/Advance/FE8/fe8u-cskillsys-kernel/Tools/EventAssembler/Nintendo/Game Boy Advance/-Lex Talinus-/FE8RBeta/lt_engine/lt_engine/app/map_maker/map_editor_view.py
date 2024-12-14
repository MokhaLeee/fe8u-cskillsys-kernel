from enum import IntEnum

from PyQt5.QtWidgets import QGraphicsView, QGraphicsScene
from PyQt5.QtCore import Qt, QRect
from PyQt5.QtGui import QPainter, QPixmap, QColor, QPen

from app.constants import TILEWIDTH, TILEHEIGHT, WINWIDTH, WINHEIGHT

from app.map_maker.draw_tilemap import draw_tilemap
from app.map_maker.terrain_database import DB_terrain
import app.map_maker.utilities as map_utils
from app.editor import timer

class PaintTool(IntEnum):
    NoTool = 0
    Brush = 1
    Fill = 2
    Erase = 3
    CliffMarker = 4

class MapEditorView(QGraphicsView):
    min_scale = 1
    max_scale = 6

    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        self.scene = QGraphicsScene(self)
        self.setScene(self.scene)
        self.setMouseTracking(True)

        self.setMinimumSize(WINWIDTH, WINHEIGHT)
        self.setStyleSheet("background-color:rgb(128, 128, 128);")

        self.screen_scale = 1

        self.tilemap = None

        self.current_mouse_position = (0, 0)
        self.old_middle_pos = None

        self.left_selecting = False
        self.right_selecting = False
        self.right_selection = {}  # Dictionary of tile_sprites

        self.draw_autotiles = True
        self.draw_gridlines = True

        timer.get_timer().tick_elapsed.connect(self.tick)

    def tick(self):
        if self.tilemap:
            self.update_view()

    def set_current(self, current):
        self.tilemap = current
        self.update_view()

    def clear_scene(self):
        self.scene.clear()

    def update_view(self):
        if self.tilemap:
            pixmap = QPixmap.fromImage(self.get_map_image())
            self.working_image = pixmap
        else:
            return
        self.show_map()

    def get_map_image(self):
        if self.draw_autotiles:
            image = draw_tilemap(self.tilemap, autotile_fps=self.tilemap.autotile_fps)
        else:
            image = draw_tilemap(self.tilemap, autotile_fps=0)

        painter = QPainter()
        painter.begin(image)
        # Draw grid lines
        if self.draw_gridlines:
            painter.setPen(QPen(QColor(0, 0, 0, 128), 1, Qt.DotLine))
            for x in range(self.tilemap.width):
                painter.drawLine(x * TILEWIDTH, 0, x * TILEWIDTH, self.tilemap.height * TILEHEIGHT)
            for y in range(self.tilemap.height):
                painter.drawLine(0, y * TILEHEIGHT, self.tilemap.width * TILEWIDTH, y * TILEHEIGHT)

        # Draw cliff markers
        painter.setPen(QPen(QColor(255, 0, 0, 128), 2))
        for cliff_marker in self.tilemap.cliff_markers:
            painter.drawPoint(cliff_marker[0] * TILEWIDTH, cliff_marker[1] * TILEHEIGHT)

        # Draw cursor...
        if self.window.current_tool == PaintTool.CliffMarker:
            painter.setPen(QPen(QColor(128, 0, 128, 255), 3))
            mouse_pos = self.current_mouse_position
            painter.drawPoint(mouse_pos[0] * TILEWIDTH, mouse_pos[1] * TILEHEIGHT)
        elif self.right_selecting:
            # Currently holding down right click and selecting area
            self.draw_selection(painter)
        elif self.right_selection:
            # Currently drawing with a right click held down area
            self.draw_right_cursor(painter)
        elif self.window.current_tool in (PaintTool.Erase, PaintTool.NoTool):
            self.draw_simple_cursor(painter)
        else:
            # Currently drawing with a terrain
            self.draw_normal_cursor(painter)

        painter.end()
        return image
        
    def show_map(self):
        self.clear_scene()
        self.scene.addPixmap(self.working_image)

    def draw_simple_cursor(self, painter):
        mouse_pos = self.current_mouse_position
        # Fill with blue
        rect = QRect(mouse_pos[0] * TILEWIDTH, mouse_pos[1] * TILEHEIGHT, TILEWIDTH, TILEHEIGHT)
        painter.fillRect(rect, QColor(0, 255, 255, 96))

    def draw_normal_cursor(self, painter):
        mouse_pos = self.current_mouse_position
        current_selection = self.window.terrain_painter_menu.get_current_nid()
        terrain = DB_terrain.get(current_selection)
        if not terrain:
            return
        if not terrain.display_pixmap:
            terrain.tileset_pixmap = QPixmap(terrain.tileset_path)
            terrain.display_pixmap = terrain.tileset_pixmap.copy(*terrain.display_tile_coord, 16, 16)
        im = terrain.display_pixmap.toImage()
        painter.drawImage(mouse_pos[0] * TILEWIDTH,
                          mouse_pos[1] * TILEHEIGHT,
                          im)
        # Fill with blue
        rect = QRect(mouse_pos[0] * TILEWIDTH, mouse_pos[1] * TILEHEIGHT, TILEWIDTH, TILEHEIGHT)
        painter.fillRect(rect, QColor(0, 255, 255, 96))

    def draw_right_cursor(self, painter):
        mouse_pos = self.current_mouse_position
        for coord, (true_coord, terrain_nid) in self.right_selection.items():
            terrain = DB_terrain.get(terrain_nid)
            if not terrain:
                return
            if not terrain.display_pixmap:
                terrain.tileset_pixmap = QPixmap(terrain.tileset_path)
                terrain.display_pixmap = terrain.tileset_pixmap.copy(*terrain.display_tile_coord, 16, 16)
            true_coord = mouse_pos[0] + coord[0], mouse_pos[1] + coord[1]
            painter.drawImage(true_coord[0] * TILEWIDTH,
                              true_coord[1] * TILEHEIGHT,
                              terrain.display_pixmap.toImage())
            # Fill with blue
            rect = QRect(true_coord[0] * TILEWIDTH, true_coord[1] * TILEHEIGHT, TILEWIDTH, TILEHEIGHT)
            painter.fillRect(rect, QColor(0, 255, 255, 96))

    def draw_selection(self, painter):
        starting_pos = self.right_selecting
        if not starting_pos:
            return
        for coord, (true_coord, terrain_nid) in self.right_selection.items():
            color = QColor(0, 255, 255, 128)
            rect = QRect(true_coord[0] * TILEWIDTH, true_coord[1] * TILEHEIGHT, TILEWIDTH, TILEHEIGHT)
            painter.fillRect(rect, color)

    def create_right_selection(self):
        self.right_selection.clear()
        left = min(self.right_selecting[0], self.current_mouse_position[0])
        width = max(self.right_selecting[0], self.current_mouse_position[0]) - left + 1
        top = min(self.right_selecting[1], self.current_mouse_position[1])
        height = max(self.right_selecting[1], self.current_mouse_position[1]) - top + 1
        for x in range(width):
            for y in range(height):
                i, j = x + left, y + top
                self.right_selection[(x, y)] = ((i, j), self.tilemap.get_terrain((i, j)))

    def paint_terrain(self, tile_pos):
        if self.right_selection:
            for coord, (true_coord, terrain_nid) in self.right_selection.items():
                true_pos = tile_pos[0] + coord[0], tile_pos[1] + coord[1]
                if self.tilemap.check_bounds(true_pos):
                    terrain = DB_terrain.get(terrain_nid)
                    old_terrain = DB_terrain.get(self.tilemap.get_terrain(true_pos))
                    self.tilemap.set(true_pos, old_terrain, terrain)
        elif self.tilemap.check_bounds(tile_pos):
            current_nid = self.window.terrain_painter_menu.get_current_nid()
            terrain = DB_terrain.get(current_nid)
            old_terrain = DB_terrain.get(self.tilemap.get_terrain(tile_pos))
            self.tilemap.set(tile_pos, old_terrain, terrain)

    def erase_terrain(self, tile_pos):
        if self.tilemap.check_bounds(tile_pos):
            terrain = DB_terrain.get(self.tilemap.get_terrain(tile_pos))
            self.tilemap.erase_terrain(tile_pos, terrain)

    def flood_fill_terrain(self, tile_pos):
        if not self.tilemap.check_bounds(tile_pos):
            return

        coords_to_replace = map_utils.flood_fill(self.tilemap, tile_pos)

        if self.right_selection:
            # Only handles the topleft tile
            coords = list(self.right_selection.keys())
            topleft = min(coords)
            w = max(coord[0] for coord in coords) - topleft[0] + 1
            h = max(coord[1] for coord in coords) - topleft[1] + 1

            # Do the deed
            for x in range(self.tilemap.width):
                for y in range(self.tilemap.height):
                    if (x, y) in coords_to_replace:
                        new_coord_x = (x % w) + topleft[0]
                        new_coord_y = (y % h) + topleft[1]
                        if (new_coord_x, new_coord_y) in coords:
                            old_terrain = DB_terrain.get(self.tilemap.get_terrain((x, y)))
                            current_nid = self.right_selection[(new_coord_x, new_coord_y)][1]
                            new_terrain = DB_terrain.get(current_nid)
                            self.tilemap.set((x, y), old_terrain, new_terrain)
        else:
            terrain_nid = self.window.terrain_painter_menu.get_current_nid()
            for pos in coords_to_replace:
                old_terrain = DB_terrain.get(self.tilemap.get_terrain(pos))
                new_terrain = DB_terrain.get(terrain_nid)
                self.tilemap.set(pos, old_terrain, new_terrain)

    def mousePressEvent(self, event):
        scene_pos = self.mapToScene(event.pos())
        tile_pos = int(scene_pos.x() // TILEWIDTH), \
            int(scene_pos.y() // TILEHEIGHT)

        # Determined by Paint Tool selected
        if event.button() == Qt.LeftButton:
            # Put a single terrain on location
            if self.window.current_tool == PaintTool.Brush:
                self.paint_terrain(tile_pos)
                self.left_selecting = True
            # Erase the terrain on location
            elif self.window.current_tool == PaintTool.Erase:
                self.erase_terrain(tile_pos)
                self.left_selecting = True
            # Flood-fill terrain on location
            elif self.window.current_tool == PaintTool.Fill:
                self.flood_fill_terrain(tile_pos)
            # Add new cliff marker on location
            elif self.window.current_tool == PaintTool.CliffMarker:
                self.window.cliff_marker_widget.add_new_marker(tile_pos)
        # Select the current terrain under cursor
        elif event.button() == Qt.RightButton and self.tilemap.check_bounds(tile_pos):
            current_nid = self.tilemap.get_terrain(tile_pos)
            if current_nid:
                self.window.terrain_painter_menu.set_current_nid(current_nid)
        # Move about the map
        elif event.button() == Qt.MiddleButton:
            self.old_middle_pos = event.pos()

    def mouseMoveEvent(self, event):
        scene_pos = self.mapToScene(event.pos())
        tile_pos = int(scene_pos.x() // TILEWIDTH), \
            int(scene_pos.y() // TILEHEIGHT)

        self.current_mouse_position = tile_pos

        self.window.set_position_bar(tile_pos)
        terrain_nid = self.tilemap.get_terrain(tile_pos)
        if terrain_nid:
            self.window.set_message("%s" % terrain_nid)
        else:
            self.window.set_message(None)

        # If holding down mouse, paint or erase
        if self.left_selecting and self.tilemap.check_bounds(tile_pos):
            if self.window.current_tool == PaintTool.Brush:
                self.paint_terrain(tile_pos)
            elif self.window.current_tool == PaintTool.Erase:
                self.erase_terrain(tile_pos)
        # Calculate what coordinates have been selected under me
        elif self.right_selecting:
            self.create_right_selection()
        # Move about the map
        elif event.buttons() & Qt.MiddleButton:
            offset = self.old_middle_pos - event.pos()
            self.old_middle_pos = event.pos()

            self.verticalScrollBar().setValue(self.verticalScrollBar().value() + offset.y())
            self.horizontalScrollBar().setValue(self.horizontalScrollBar().value() + offset.x())

    def mouseReleaseEvent(self, event):
        scene_pos = self.mapToScene(event.pos())
        tile_pos = int(scene_pos.x() // TILEWIDTH), \
            int(scene_pos.y() // TILEHEIGHT)

        if self.window.current_tool == PaintTool.Brush:
            if event.button() == Qt.LeftButton:
                self.left_selecting = False
            elif event.button() == Qt.RightButton:
                if self.right_selecting:
                    self.create_right_selection()
                    self.right_selecting = False
        elif self.window.current_tool == PaintTool.Erase:
            if event.button() == Qt.LeftButton:
                self.left_selecting = False

    def zoom_in(self):
        if self.screen_scale < self.max_scale:
            self.screen_scale += 1
            self.scale(2, 2)

    def zoom_out(self):
        if self.screen_scale > self.min_scale:
            self.screen_scale -= 1
            self.scale(0.5, 0.5)

    def wheelEvent(self, event):
        if event.angleDelta().y() > 0:
            self.zoom_in()
        elif event.angleDelta().y() < 0:
            self.zoom_out()
