from PyQt5.QtWidgets import QPushButton, QColorDialog
from PyQt5.QtGui import QColor
from PyQt5.QtCore import pyqtSignal

class ColorIcon(QPushButton):
    colorChanged = pyqtSignal(QColor)
    size = 64

    def __init__(self, color: QColor, parent):
        super().__init__(parent)
        self._color: QColor = None
        self.change_color(color)

        self.set_size(self.size)
        self.pressed.connect(self.onColorPicker)

    def set_size(self, size):
        self.size = size
        self.setMinimumHeight(self.size)
        self.setMaximumHeight(self.size)
        self.setMinimumWidth(self.size)
        self.setMaximumWidth(self.size)
        self.resize(self.size, self.size)

    def change_color(self, color):
        if color != self._color:
            self._color = color
            self.colorChanged.emit(color)

        if self._color:
            self.setStyleSheet("background-color: %s;" % self._color.name())
        else:
            self.setStyleSheet("")

    def color(self):
        return self._color

    def onColorPicker(self):
        dlg = QColorDialog()
        if self._color:
            dlg.setCurrentColor(self._color)
        if dlg.exec_():
            self.change_color(dlg.currentColor())

class AlphaColorIcon(ColorIcon):
    def onColorPicker(self):
        dlg = QColorDialog()
        dlg.setOption(QColorDialog.ShowAlphaChannel, True)
        if self._color:
            dlg.setCurrentColor(self._color)
        if dlg.exec_():
            self.change_color(dlg.currentColor())
