from app import dark_theme
from app.constants import TILEHEIGHT, TILEWIDTH
from app.data.database.database import DB
from app.editor.settings import MainSettingsController
from app.editor.terrain_painter_menu import TerrainPainterMenu
from app.editor.tilemap_editor import PaintTool
from app.extensions.tiled_view import DraggableTileImageView
from app.data.resources.tiles import TileSet
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QColor, QIcon, QPainter, QPen
from PyQt5.QtWidgets import (QAction, QActionGroup, QDesktopWidget, QDialog,
                             QDialogButtonBox, QFrame, QHBoxLayout, QSplitter, QToolBar,
                             QVBoxLayout)

class TileSetEditorView(DraggableTileImageView):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.tileset: TileSet = None
        self.alpha = 128
        self.draw_gridlines = True

    def set_current(self, current: TileSet):
        self.tileset = current
        self.update_view()

    def set_alpha(self, alpha: float):
        self.alpha = alpha

    def update_view(self):
        if self.tileset:
            pixmap = self.tileset.pixmap.copy()
            self.working_image = pixmap
        else:
            self.clear_scene()
            return
        self.draw_terrain()
        self.draw_grid()
        self.show_map()

    def draw_grid(self):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            # Draw grid lines
            if self.draw_gridlines:
                painter.setPen(QPen(QColor(0, 0, 0, 128), 1, Qt.DotLine))
                for x in range(self.tileset.width):
                    painter.drawLine(x * TILEWIDTH, 0, x * TILEWIDTH, self.tileset.height * TILEHEIGHT)
                for y in range(self.tileset.height):
                    painter.drawLine(0, y * TILEHEIGHT, self.tileset.width * TILEWIDTH, y * TILEHEIGHT)
            painter.end()

    def draw_terrain(self):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            for coord, terrain_nid in self.tileset.terrain_grid.items():
                terrain = DB.terrain.get(terrain_nid)
                if terrain:
                    color = terrain.color
                    write_color = QColor(color[0], color[1], color[2])
                    write_color.setAlpha(self.alpha)
                    painter.fillRect(coord[0] * TILEWIDTH, coord[1] * TILEHEIGHT, TILEWIDTH, TILEHEIGHT, write_color)
            painter.end()

    def show_map(self):
        self.clear_scene()
        self.scene.addPixmap(self.working_image)

class TileSetEditor(QDialog):
    def __init__(self, parent=None, current: TileSet = None):
        super().__init__(parent)
        self.window = parent
        self.setWindowTitle("Tileset Editor")
        self.setWindowFlag(Qt.WindowContextHelpButtonHint, False)

        self.settings = MainSettingsController()

        desktop = QDesktopWidget()
        main_screen_size = desktop.availableGeometry(desktop.primaryScreen())
        default_size = int(main_screen_size.width()*0.7), int(main_screen_size.height()*0.7)
        self.resize(*default_size)

        self.current: TileSet = current
        self.save()
        self.current_tool = PaintTool.NoTool
        self.tool_active = False

        self.terrain_painter_menu = TerrainPainterMenu(self)
        self.terrain_painter_menu.alpha_updated.connect(self.on_terrain_alpha_change)
        self.view = TileSetEditorView(self)
        self.view.set_current(current)
        self.view.set_alpha(self.terrain_painter_menu.get_alpha())
        self.view.left_clicked.connect(self.handle_left_click)
        self.view.left_released.connect(self.handle_mouse_release)
        self.view.mouse_moved.connect(self.handle_mouse_move)

        self.create_actions()
        self.create_toolbar()

        self.main_splitter = QSplitter(self)
        self.main_splitter.setChildrenCollapsible(False)

        view_frame = QFrame()
        view_layout = QVBoxLayout()
        toolbar_layout = QHBoxLayout()
        toolbar_layout.addWidget(self.toolbar)
        view_layout.addLayout(toolbar_layout)
        view_layout.addWidget(self.view)
        view_frame.setLayout(view_layout)


        self.main_splitter.addWidget(self.terrain_painter_menu)
        self.main_splitter.addWidget(view_frame)

        self.layout = QVBoxLayout()
        self.setLayout(self.layout)
        self.layout.addWidget(self.main_splitter)

        self.buttonbox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel, Qt.Horizontal, self)
        self.layout.addWidget(self.buttonbox)
        self.buttonbox.accepted.connect(self.accept)
        self.buttonbox.rejected.connect(self.reject)

        self.check_brush()

        # Restore Geometry
        geometry = self.settings.component_controller.get_geometry(self._type())
        if geometry:
            self.restoreGeometry(geometry)
        state = self.settings.component_controller.get_state(self._type())
        if state:
            self.main_splitter.restoreState(state)

        self.view.update_view()

    def on_terrain_alpha_change(self, alpha: int):
        self.view.set_alpha(alpha)

    def reset_terrain(self):
        self.current.terrain_grid.clear()

    def handle_left_click(self, x, y):
        tile_pos = (x, y)
        if self.current_tool == PaintTool.Brush:
            if self.current.check_bounds(tile_pos):
                current_nid = self.terrain_painter_menu.get_current_nid()
                self.current.terrain_grid[tile_pos] = current_nid
        elif self.current_tool == PaintTool.Erase:
            if self.current.check_bounds(tile_pos):
                self.current.terrain_grid[tile_pos] = None
        self.tool_active = True

    def handle_mouse_release(self):
        self.tool_active = False

    def handle_mouse_move(self, x, y):
        tile_pos = (x, y)
        if self.tool_active:
            if self.current_tool == PaintTool.Brush:
                if self.current.check_bounds(tile_pos):
                    current_nid = self.terrain_painter_menu.get_current_nid()
                    self.current.terrain_grid[tile_pos] = current_nid
            elif self.current_tool == PaintTool.Erase:
                if self.current.check_bounds(tile_pos):
                    self.current.terrain_grid[tile_pos] = None

    def create_actions(self):
        theme = dark_theme.get_theme()
        icon_folder = theme.icon_dir()


        paint_group = QActionGroup(self)
        self.brush_action = QAction(QIcon(f"{icon_folder}/brush.png"), "&Brush", self, shortcut="B", triggered=self.set_brush)
        self.brush_action.setCheckable(True)
        paint_group.addAction(self.brush_action)
        self.erase_action = QAction(QIcon(f"{icon_folder}/eraser.png"), "&Erase", self, shortcut="E", triggered=self.set_erase)
        self.erase_action.setCheckable(True)
        paint_group.addAction(self.erase_action)

        self.show_gridlines_action = QAction(QIcon(f"{icon_folder}/gridlines.png"), "Show GridLines", self, triggered=self.gridline_toggle)
        self.show_gridlines_action.setCheckable(True)
        self.show_gridlines_action.setChecked(True)

    def check_brush(self):
        self.brush_action.setChecked(True)
        self.set_brush(True)

    def set_brush(self, val):
        self.current_tool = PaintTool.Brush

    def set_erase(self, val):
        self.current_tool = PaintTool.Erase

    def create_toolbar(self):
        self.toolbar = QToolBar(self)
        self.toolbar.addAction(self.brush_action)
        self.toolbar.addAction(self.erase_action)
        self.toolbar.addAction(self.show_gridlines_action)

    def set_current(self, current: TileSet):
        self.current = current
        self.view.set_current(current)
        self.view.update_view()

    def gridline_toggle(self, val):
        self.view.draw_gridlines = val

    def update_view(self):
        self.view.update_view()

    def accept(self):
        self.save_geometry()
        super().accept()

    def reject(self):
        self.restore()
        self.save_geometry()
        super().reject()

    def closeEvent(self, event):
        self.save_geometry()
        super().closeEvent(event)

    def save(self):
        self.saved_data = self.current.terrain_grid.copy()

    def restore(self):
        self.current.terrain_grid = self.saved_data

    def _type(self):
        return 'tileset_editor'

    def save_geometry(self):
        self.settings.component_controller.set_geometry(self._type(), self.saveGeometry())
        self.settings.component_controller.set_state(self._type(), self.main_splitter.saveState())
