from PyQt5.QtWidgets import QGridLayout, QListView, \
    QWidget
from PyQt5.QtCore import QSize

from app.map_maker.terrain_database import DB_terrain

from app.map_maker.map_terrain_model import MapTerrainModel

class TerrainPainterMenu(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.map_editor = parent

        for terrain in DB_terrain:
            terrain.set_tileset()

        grid = QGridLayout()
        self.setLayout(grid)

        self.list_view = QListView(self)

        self.model = MapTerrainModel(DB_terrain, self)
        self.list_view.setModel(self.model)
        self.list_view.setIconSize(QSize(32, 32))
        self.list_view.setMaximumWidth(300)

        grid.addWidget(self.list_view, 3, 0, 1, 2)

    def on_visibility_changed(self, state):
        pass

    def reset(self):
        self.model.layoutChanged.emit()

    def set_current_nid(self, nid):
        idx = self.model.index(DB_terrain.index(nid))
        self.list_view.setCurrentIndex(idx)

    def get_current_nid(self):
        index = self.list_view.currentIndex()
        terrain = DB_terrain[index.row()]
        return terrain.nid

# Testing
# Run "python -m app.map_maker.terrain_painter_menu" from main directory
if __name__ == "__main__":
    import sys
    from PyQt5.QtWidgets import QApplication
    ap = QApplication(sys.argv)
    terrain_painter = TerrainPainterMenu()
    terrain_painter.show()
    ap.exec_()
