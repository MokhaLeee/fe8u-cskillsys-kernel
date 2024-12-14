from PyQt5.QtWidgets import QGridLayout, QPushButton, QSlider, QLabel, QListView, \
    QWidget
from PyQt5.QtCore import Qt, QSize, pyqtSignal

from app.data.database.database import DB

# from app.editor.database_editor import DatabaseEditor
from app.editor.terrain_editor.terrain_model import TerrainModel

class TerrainPainterMenu(QWidget):
    alpha_updated = pyqtSignal(int)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.map_editor = parent

        grid = QGridLayout()
        self.setLayout(grid)

        self.alpha_slider = QSlider(Qt.Horizontal, self)
        self.alpha_slider.setRange(0, 255)
        self.alpha_slider.setValue(192)
        grid.addWidget(QLabel("Transparency"), 1, 0)
        grid.addWidget(self.alpha_slider, 1, 1)
        self.alpha_slider.valueChanged.connect(self.map_editor.update_view)
        self.alpha_slider.valueChanged.connect(self.alpha_changed)

        self.list_view = QListView(self)

        self.model = TerrainModel(DB.terrain, self)
        self.list_view.setModel(self.model)
        self.list_view.setIconSize(QSize(32, 32))

        grid.addWidget(self.list_view, 3, 0, 1, 2)

        self.reset_button = QPushButton('Reset Terrain')
        self.reset_button.clicked.connect(self.reset_terrain)
        # self.edit_button = QPushButton('Edit Terrain...')
        # self.edit_button.clicked.connect(self.edit_terrain)
        grid.addWidget(self.reset_button, 4, 0, 1, 2)
        # grid.addWidget(self.edit_button, 4, 1)

    def alpha_changed(self, alpha):
        self.alpha_updated.emit(alpha)

    def on_visibility_changed(self, state):
        pass

    def get_alpha(self):
        return int(self.alpha_slider.value())

    def reset_terrain(self):
        # command = commands.ResetTerrain(self.map_editor.current_level)
        # self.map_editor.undo_stack.push(command)
        self.map_editor.reset_terrain()
        self.map_editor.update_view()

    def set_current_nid(self, nid):
        idx = self.model.index(DB.terrain.index(nid))
        self.list_view.setCurrentIndex(idx)

    def get_current_nid(self):
        index = self.list_view.currentIndex()
        terrain = DB.terrain[index.row()]
        return terrain.nid
