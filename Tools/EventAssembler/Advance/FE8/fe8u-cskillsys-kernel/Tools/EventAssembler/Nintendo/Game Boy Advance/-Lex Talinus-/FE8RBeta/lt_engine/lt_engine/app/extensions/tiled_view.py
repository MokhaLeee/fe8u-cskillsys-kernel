from app.constants import TILEHEIGHT, TILEWIDTH, WINHEIGHT, WINWIDTH
from app.editor import timer
from PyQt5.QtCore import Qt, pyqtSignal
from PyQt5.QtWidgets import (QGraphicsScene, QGraphicsView)

class DraggableTileImageView(QGraphicsView):
    min_scale = 0
    max_scale = 5

    left_clicked = pyqtSignal(int, int)
    left_released = pyqtSignal()
    middle_clicked = pyqtSignal(int, int)
    mouse_moved = pyqtSignal(int, int)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent
        self.scene = QGraphicsScene(self)
        self.setScene(self.scene)
        self.setMouseTracking(True)
        self.setMinimumSize(WINWIDTH, WINHEIGHT)
        self.setStyleSheet("background-color:rgb(128, 128, 128);")
        self.screen_scale = 1
        self.drag_origin = None
        timer.get_timer().tick_elapsed.connect(self.tick)

    def update_view(self):
        """Redraw the view in its entirety."""
        raise NotImplementedError("Subclasses should implement UpdateView")

    def tick(self):
        self.update_view()

    def clear_scene(self):
        self.scene.clear()

    def mousePressEvent(self, event):
        scene_pos = self.mapToScene(event.pos())
        tile_pos = int(scene_pos.x() // TILEWIDTH), \
            int(scene_pos.y() // TILEHEIGHT)
        if event.button() == Qt.LeftButton:
            self.left_clicked.emit(*tile_pos)
        elif event.button() == Qt.MiddleButton:
            self.drag_origin = event.pos()
            self.middle_clicked.emit(*tile_pos)

    def mouseMoveEvent(self, event):
        scene_pos = self.mapToScene(event.pos())
        tile_pos = int(scene_pos.x() // TILEWIDTH), \
            int(scene_pos.y() // TILEHEIGHT)
        self.mouse_moved.emit(*tile_pos)
        if self.drag_origin:
            offset = self.drag_origin - event.pos()
            self.drag_origin = event.pos()
            self.verticalScrollBar().setValue(self.verticalScrollBar().value() + offset.y())
            self.horizontalScrollBar().setValue(self.horizontalScrollBar().value() + offset.x())

    def mouseReleaseEvent(self, event):
        if event.button() == Qt.LeftButton:
            self.left_released.emit()
        elif event.button() == Qt.MiddleButton:
            self.drag_origin = None

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
