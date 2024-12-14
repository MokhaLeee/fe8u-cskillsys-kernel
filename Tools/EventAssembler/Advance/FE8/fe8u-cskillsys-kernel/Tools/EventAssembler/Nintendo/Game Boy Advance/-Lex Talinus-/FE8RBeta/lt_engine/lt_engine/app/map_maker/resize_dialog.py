from PyQt5.QtWidgets import QVBoxLayout, \
    QDialog, QGroupBox, QFormLayout, QSpinBox
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QImage, QPainter, QPixmap, QColor, QPen

from app.editor.icon_editor.icon_view import IconView
from app.extensions.custom_gui import Dialog

class ResizeDialog(Dialog):
    def __init__(self, current, parent=None):
        super().__init__(parent)
        self.window = parent
        self.setWindowTitle("Tilemap Resize")
        self.layout = QVBoxLayout()
        self.setLayout(self.layout)

        self.current = current  # TilemapPrefab

        size_section = QGroupBox(self)
        size_section.setTitle("Size")
        size_layout = QFormLayout()
        self.width_box = QSpinBox()
        self.width_box.setValue(self.current.width)
        self.width_box.setRange(15, 255)
        self.width_box.valueChanged.connect(self.on_width_changed)
        size_layout.addRow("Width:", self.width_box)
        self.height_box = QSpinBox()
        self.height_box.setValue(self.current.height)
        self.height_box.setRange(10, 255)
        self.height_box.valueChanged.connect(self.on_height_changed)
        size_layout.addRow("Height:", self.height_box)
        size_section.setLayout(size_layout)

        offset_section = QGroupBox(self)
        offset_section.setTitle("Offset")
        offset_layout = QFormLayout()
        self.x_box = QSpinBox()
        self.x_box.setValue(0)
        self.x_box.setRange(0, 0)
        self.x_box.valueChanged.connect(self.on_offset_changed)
        # self.x_box.valueChanged.connect(self.on_x_changed)
        offset_layout.addRow("X:", self.x_box)
        self.y_box = QSpinBox()
        self.y_box.setValue(0)
        self.y_box.setRange(0, 0)
        self.y_box.valueChanged.connect(self.on_offset_changed)
        # self.y_box.valueChanged.connect(self.on_y_changed)
        offset_layout.addRow("Y:", self.y_box)
        offset_section.setLayout(offset_layout)

        self.icon_view = IconView(self)
        self.icon_view.scene.setBackgroundBrush(QColor(200, 200, 200, 255))

        self.layout.addWidget(size_section)
        self.layout.addWidget(offset_section)
        self.layout.addWidget(self.icon_view)
        self.layout.addWidget(self.buttonbox)

        self.draw_image()

    def on_width_changed(self, val):
        if val > self.current.width:
            self.x_box.setMaximum(val - self.current.width)
        elif val < self.current.width:
            self.x_box.setMinimum(val - self.current.width)
        self.draw_image()

    def on_height_changed(self, val):
        if val > self.current.height:
            self.y_box.setMaximum(val - self.current.height)
        elif val < self.current.height:
            self.y_box.setMinimum(val - self.current.height)
        self.draw_image()

    def on_offset_changed(self, val):
        self.draw_image()

    def draw_image(self):
        base_image = QImage(202, 202, QImage.Format_ARGB32)
        base_image.fill(QColor(200, 200, 200, 255))
        painter = QPainter()
        painter.begin(base_image)
        painter.setPen(QPen(Qt.black, 2, Qt.SolidLine))
        # Draw regular square around
        highest_dim = max([self.width_box.value(), self.height_box.value(),
                           self.current.width, self.current.height])
        new_offset_x = int(self.x_box.value() / highest_dim * 200)
        new_offset_y = int(self.y_box.value() / highest_dim * 200)
        new_width = int(self.width_box.value() / highest_dim * 200)
        new_height = int(self.height_box.value() / highest_dim * 200)
        painter.drawRect(1, 1, new_width, new_height)
        painter.setPen(QPen(Qt.black, 2, Qt.DashLine))
        new_width = int(self.current.width / highest_dim * 200)
        new_height = int(self.current.height / highest_dim * 200)
        painter.drawRect(1 + new_offset_x, 1 + new_offset_y, new_width, new_height)

        painter.end()

        self.icon_view.set_image(QPixmap.fromImage(base_image))
        self.icon_view.show_image()

    @classmethod
    def get_new_size(cls, tilemap_prefab, parent=None):
        dialog = cls(tilemap_prefab, parent)
        result = dialog.exec_()
        if result == QDialog.Accepted:
            new_width = int(dialog.width_box.value())
            new_height = int(dialog.height_box.value())
            x_offset = int(dialog.x_box.value())
            y_offset = int(dialog.y_box.value())
            tilemap_prefab.resize(new_width, new_height, x_offset, y_offset)
            return True
        else:
            return False
