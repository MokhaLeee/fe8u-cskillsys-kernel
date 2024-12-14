import math

from PyQt5.QtWidgets import QWidget
from PyQt5.QtCore import Qt, QRectF, pyqtSignal
from PyQt5.QtGui import QColor, QPen, QPainter, QBrush, QPixmap, QLinearGradient

from app.utilities import utils

# Inspired by sections of code from https://github.com/mortalis13/Qt-Color-Picker-Qt

def drawRoundRect(painter: QPainter, rect: QRectF, border_size: int, border_radius: int, border_color: QColor):
    pen = QPen()
    pen.setWidth(border_size)
    pen.setColor(border_color)
    pen.setJoinStyle(Qt.RoundJoin)

    painter.setPen(pen)
    painter.setBrush(Qt.NoBrush)

    new_rect = QRectF(rect.x() - border_size/2, rect.y() - border_size/2, rect.width() + border_size, rect.height() + border_size)

    if border_size % 2 == 0:
        painter.drawRoundedRect(new_rect, border_size, border_size)
    else:
        painter.drawRoundedRect(new_rect.translated(0.5, 0.5), border_radius, border_radius)

class ColorSlider(QWidget):
    border: int = 2
    border_radius: int = 10
    slider_x: int = border
    slider_y: int = border
    pointer_r: int = 5
    pointer_border: int = 2

    color: QColor = QColor(0, 0, 0)
    border_color: QColor = QColor(80, 80, 80, 200)

    pointer_color: QColor = QColor("#333")
    pointer_line_color: QColor = QColor(80, 80, 80, 200)
    pointer_border_color: QColor = QColor("#ddd")
    pointer_fill_color: QColor = QColor("#333")

    slider_x = 0
    slider_y = 0
    slider_width = 0
    slider_height = 0
    prev_slider_width = 0

    def __init__(self, parent):
        super().__init__(parent)
        self.window = parent

        self.slider_drawn = False
        self.width_changed = False

        self.pointer_x = 0
        self.pointer_y = 0

        self.slider_value = 0

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self.pointer_x = event.x()
            self.update()
            self.update_color()

    def mouseMoveEvent(self, event):
        self.slider_x = event.x()
        self.update()
        self.update_color()

    def wheelEvent(self, event):
        point = event.angleDelta()
        if point.y() > 0:
            self.inc_pointer(1)
        elif point.y() < 0:
            self.inc_pointer(-1)
        self.update_color()

    def calc_vars(self, val):
        self.slider_x = self.border
        self.slider_y = self.border
        self.slider_width = self.width() - 2 * self.border
        self.slider_height = self.height() - 2 * self.border

        self.maxRange = self.slider_width - 1
        self.min_pointer_x = self.slider_x
        self.max_pointer_x = self.min_pointer_x + self.maxRange

        if self.slider_width != self.prev_slider_width:
            self.width_changed = True
            self.slider_value = self.normalize_pointer_x(val)
        self.prev_slider_width = self.slider_width

    def inc_pointer(self, val):
        self.pointer_x += val
        self.update()

    def correct_pointer(self):
        self.pointer_x = self.slider_value + self.min_pointer_x
        self.pointer_x = utils.clamp(self.pointer_x, self.min_pointer_x, self.max_pointer_x)

    def drawPointer(self, painter: QPainter):
        self.correct_pointer()

        self.pointer_y = self.height() // 2

        pen = QPen(self.pointer_border_color, self.pointer_border)
        painter.setPen(pen)
        painter.setBrush(self.pointer_fill_color)
        painter.drawEllipse(self.pointer_x, self.pointer_y, self.pointer_r, self.pointer_r)

        pen.setColor(self.pointer_line_color)
        pen.setCapStyle(Qt.FlatCap)
        painter.setPen(pen)

        painter.drawLine(self.pointer_x, self.slider_y, self.pointer_x, self.pointer_y - self.pointer_r - self.pointer_border // 2)
        painter.drawLine(self.pointer_x, self.slider_y + self.slider_height, self.pointer_x, self.pointer_y + self.pointer_r + self.pointer_border // 2)

    def drawBorder(self, painter: QPainter):
        rect = QRectF(self.slider_x, self.slider_y, self.slider_width, self.slider_height)
        drawRoundRect(painter, rect, self.border, self.border_radius, self.border_color)

    def update_color(self):
        self.slider_value = self.pointer_x - self.min_pointer_x
        self.slider_value = utils.clamp(self.slider_value, 0, self.maxRange)

    def normalizeVal(self, val):
        return val / self.maxRange

    def normalize_pointer_x(self, val):
        return math.ceil(val / self.maxRange)

    def updatePaint(self):
        self.update()

class RGBSlider(ColorSlider):
    max_rgb = 255

    redChanged = pyqtSignal(QColor)
    greenChanged = pyqtSignal(QColor)
    blueChanged = pyqtSignal(QColor)

    def __init__(self, slider_type: str, parent):
        super().__init__(parent)
        self.window = parent

        self.slider_type = slider_type
        self.red = 0
        self.green = 0
        self.blue = 0
        self.color = QColor.fromRgbF(self.red, self.green, self.blue)

        self.color.setRgbF(self.red, self.green, self.blue)

    def paintEvent(self, event):
        painter = QPainter()
        painter.begin(self)
        painter.setRenderHint(QPainter.Antialiasing)

        if self.slider_type == 'red':
            value = self.red
        elif self.slider_type == 'green':
            value = self.green
        elif self.slider_type == 'blue':
            value = self.blue

        self.calc_vars(value)

        if not self.slider_drawn or self.width_changed:
            self.paintComponent()

        painter.drawPixmap(self.slider_x, self.slider_y, self.slider_pixmap)

        self.drawPointer(painter)
        self.drawBorder(painter)
        painter.end()

    def paintComponent(self):
        self.slider_pixmap = QPixmap(self.slider_width, self.slider_height)
        temp_painter = QPainter()
        temp_painter.begin(self.slider_pixmap)

        grad = QLinearGradient(self.slider_x, self.slider_height/2, self.slider_x + self.slider_width, self.slider_height/2)

        if self.slider_type == 'red':
            self.paint_red(grad)
        elif self.slider_type == 'green':
            self.paint_green(grad)
        elif self.slider_type == 'blue':
            self.paint_blue(grad)

        temp_painter.setPen(Qt.NoPen)
        temp_painter.setBrush(QBrush(grad))
        temp_painter.drawRect(0, 0, self.slider_width, self.slider_height)

        self.slider_drawn = True
        self.width_changed = False

    def paint_red(self, grad: QLinearGradient):
        grad.setColorAt(0, QColor.fromRgbF(0, self.green, self.blue))
        grad.setColorAt(1, QColor.fromRgbF(1, self.green, self.blue))

    def paint_green(self, grad: QLinearGradient):
        grad.setColorAt(0, QColor.fromRgbF(self.red, 0, self.blue))
        grad.setColorAt(1, QColor.fromRgbF(self.red, 1, self.blue))

    def paint_blue(self, grad: QLinearGradient):
        grad.setColorAt(0, QColor.fromRgbF(self.red, self.green, 0))
        grad.setColorAt(1, QColor.fromRgbF(self.red, self.green, 1))

    def update_color(self):
        super().update_color()

        val = utils.clamp(self.normalizeVal(self.slider_value), 0, 1)

        if self.slider_type == 'red':
            self.red = val
            self.color.setRgbF(self.red, self.green, self.blue)
            self.redChanged.emit(self.color)
        elif self.slider_type == 'green':
            self.green = val
            self.color.setRgbF(self.red, self.green, self.blue)
            self.greenChanged.emit(self.color)
        elif self.slider_type == 'blue':
            self.blue = val
            self.color.setRgbF(self.red, self.green, self.blue)
            self.blueChanged.emit(self.color)

    def set_color_component(self, color: QColor, slider_type: str):
        if self.color == color:
            return

        if slider_type == 'red':
            self.red = color.redF()
            val = self.red
        elif slider_type == 'green':
            self.green = color.greenF()
            val = self.green
        elif slider_type == 'blue':
            self.blue = color.blueF()
            val = self.blue

        self.slider_value = math.ceil(val * self.maxRange)
        self.color = QColor.fromRgbF(self.red, self.green, self.blue)
        self.update()

    def set_red(self, color: QColor):
        self.set_color_component(color, 'red')

    def set_green(self, color: QColor):
        self.set_color_component(color, 'green')

    def set_blue(self, color: QColor):
        self.set_color_component(color, 'blue')

    def updateSlider(self):
        self.color = QColor.fromRgbF(self.red, self.green, self.blue)
        self.slider_drawn = False
        self.update()

    def change_red(self, color: QColor):
        self.red = color.redF()
        self.updateSlider()

    def change_green(self, color: QColor):
        self.green = color.greenF()
        self.updateSlider()

    def change_blue(self, color: QColor):
        self.blue = color.blueF()
        self.updateSlider()


class HSVSlider(ColorSlider):
    max_hue = 360
    max_sv = 255
    ratio = 1/max_hue

    hueChanged = pyqtSignal(QColor)
    saturationChanged = pyqtSignal(QColor)
    valueChanged = pyqtSignal(QColor)

    def __init__(self, slider_type: str, parent):
        super().__init__(parent)
        self.window = parent

        self.slider_type = slider_type
        self.hue = 0
        self.sat = 1
        self.val = 1
        self.color = QColor.fromHsvF(self.hue, self.sat, self.val)

        if self.slider_type in ('saturation', 'value'):
            self.slider_value = self.max_sv

        self.color.setHsvF(self.hue, self.sat, self.val)

    def paintEvent(self, event):
        painter = QPainter()
        painter.begin(self)
        painter.setRenderHint(QPainter.Antialiasing)

        if self.slider_type == 'hue':
            value = self.hue
        elif self.slider_type == 'saturation':
            value = self.sat
        elif self.slider_type == 'value':
            value = self.val

        self.calc_vars(value)

        if not self.slider_drawn or self.width_changed:
            self.paintComponent()

        painter.drawPixmap(self.slider_x, self.slider_y, self.slider_pixmap)

        self.drawPointer(painter)
        self.drawBorder(painter)
        painter.end()

    def paintComponent(self):
        self.slider_pixmap = QPixmap(self.slider_width, self.slider_height)
        temp_painter = QPainter()
        temp_painter.begin(self.slider_pixmap)

        grad = QLinearGradient(self.slider_x, 0, self.slider_x + self.slider_width, 0)

        if self.slider_type == 'hue':
            self.paint_hue(grad)
        elif self.slider_type == 'saturation':
            self.paint_saturation(grad)
        elif self.slider_type == 'value':
            self.paint_value(grad)

        temp_painter.setPen(Qt.NoPen)
        temp_painter.setBrush(QBrush(grad))
        temp_painter.drawRect(0, 0, self.slider_width, self.slider_height)

        self.slider_drawn = True
        self.width_changed = False

    def paint_hue(self, grad: QLinearGradient):
        color = QColor()
        for hs in range(self.max_hue):
            color.setHsvF(hs / self.max_hue, self.sat, self.val)
            grad.setColorAt(hs / self.max_hue, color)

    def paint_saturation(self, grad: QLinearGradient):
        grad.setColorAt(0, QColor.fromHsvF(self.hue, 0, self.val))
        grad.setColorAt(1, QColor.fromHsvF(self.hue, 1, self.val))

    def paint_value(self, grad: QLinearGradient):
        grad.setColorAt(0, QColor.fromHsvF(self.hue, self.sat, 0))
        grad.setColorAt(1, QColor.fromHsvF(self.hue, self.sat, 1))

    def update_color(self):
        super().update_color()

        val = utils.clamp(self.normalizeVal(self.slider_value), 0, 1)

        if self.slider_type == 'hue':
            self.hue = val
            self.color.setHsvF(self.hue, self.sat, self.val)
            self.hueChanged.emit(self.color)
        elif self.slider_type == 'saturation':
            self.sat = val
            self.color.setHsvF(self.hue, self.sat, self.val)
            self.saturationChanged.emit(self.color)
        elif self.slider_type == 'value':
            self.val = val
            self.color.setHsvF(self.hue, self.sat, self.val)
            self.valueChanged.emit(self.color)

    def set_color_component(self, color: QColor, slider_type: str):
        if self.color == color:
            return

        if slider_type == 'hue':
            self.hue = color.hueF()
            val = self.hue
        elif slider_type == 'saturation':
            self.sat = color.saturationF()
            val = self.sat
        elif slider_type == 'value':
            self.val = color.valueF()
            val = self.val

        self.slider_value = math.ceil(val * self.maxRange)
        self.color = QColor.fromHsvF(self.hue, self.sat, self.val)
        self.update()

    def set_hue(self, color: QColor):
        self.set_color_component(color, 'hue')

    def set_saturation(self, color: QColor):
        self.set_color_component(color, 'saturation')

    def set_value(self, color: QColor):
        self.set_color_component(color, 'value')

    def updateSlider(self):
        self.color = QColor.fromHsvF(self.hue, self.sat, self.val)
        self.slider_drawn = False
        self.update()

    def change_hue(self, color: QColor):
        self.hue = color.hueF()
        self.updateSlider()

    def change_saturation(self, color: QColor):
        self.sat = color.saturationF()
        self.updateSlider()

    def change_value(self, color: QColor):
        self.val = color.valueF()
        self.updateSlider()
