from PyQt5.QtWidgets import QWidget, QHBoxLayout, QSpinBox, QLabel, QSizePolicy
from PyQt5.QtCore import pyqtSignal

class SpinBoxXY(QWidget):
    coordsChanged = pyqtSignal(int, int)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        hbox = QHBoxLayout()
        self.setLayout(hbox)
        hbox.setContentsMargins(0, 0, 0, 0)
        hbox.setSpacing(1)

        self._x = 0
        self._y = 0

        self.x_spinbox = QSpinBox()
        self.y_spinbox = QSpinBox()
        self.x_spinbox.setMinimumWidth(40)
        self.y_spinbox.setMinimumWidth(40)
        x_label = QLabel("X:")
        y_label = QLabel("Y:")
        hbox.addWidget(x_label)
        hbox.addWidget(self.x_spinbox)
        hbox.addWidget(y_label)
        hbox.addWidget(self.y_spinbox)
        self.x_spinbox.setRange(0, 128 - 16)
        self.y_spinbox.setRange(0, 112 - 16)
        self.x_spinbox.valueChanged.connect(self.change_x)
        self.y_spinbox.valueChanged.connect(self.change_y)
        self.x_spinbox.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.y_spinbox.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        x_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        y_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.setFixedWidth(140)
        # hbox.setAlignment(Qt.AlignLeft)

    def setSingleStep(self, i):
        self.x_spinbox.setSingleStep(i)
        self.y_spinbox.setSingleStep(i)

    def setMinimum(self, i):
        self.x_spinbox.setMinimum(i)
        self.y_spinbox.setMinimum(i)

    def set_current(self, x, y):
        self.change_x(x)
        self.change_y(y)

    def change_x(self, value):
        self._x = value
        self.x_spinbox.setValue(self._x)
        self.coordsChanged.emit(self._x, self._y)

    def change_y(self, value):
        self._y = value
        self.y_spinbox.setValue(self._y)
        self.coordsChanged.emit(self._x, self._y)
