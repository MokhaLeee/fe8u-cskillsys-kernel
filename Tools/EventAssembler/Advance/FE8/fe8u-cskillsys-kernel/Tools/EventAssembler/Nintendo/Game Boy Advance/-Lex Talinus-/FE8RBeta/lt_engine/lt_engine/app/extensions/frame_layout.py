from PyQt5.QtWidgets import QFrame, QWidget, QVBoxLayout, QHBoxLayout, QSizePolicy, QLabel
from PyQt5.QtGui import QPainter, QColor
from PyQt5.QtCore import QPoint, QPointF, pyqtSignal, Qt

class FrameLayout(QWidget):
    clicked = pyqtSignal()

    def __init__(self, parent=None, title=None):
        QFrame.__init__(self, parent=parent)

        self._is_collapsed = True
        self._title_frame = None
        self._content, self._content_layout = (None, None)

        self._main_v_layout = QVBoxLayout(self)
        self._main_v_layout.setContentsMargins(0, 0, 0, 0)
        self._main_v_layout.addWidget(self.initTitleFrame(title, self._is_collapsed))
        self._main_v_layout.addWidget(self.initContent(self._is_collapsed))

        self.initCollapsable()

    def initTitleFrame(self, title, collapsed):
        self._title_frame = self.TitleFrame(title=title, collapsed=collapsed)

        return self._title_frame

    def initContent(self, collapsed):
        self._content = QWidget()
        self._content_layout = QVBoxLayout()
        self._content_layout.setContentsMargins(0, 0, 0, 0)

        self._content.setLayout(self._content_layout)
        self._content.setVisible(not collapsed)

        return self._content

    def addWidget(self, widget):
        self._content_layout.addWidget(widget)
        self._content.resize(self._content.sizeHint())

    def initCollapsable(self):
        self._title_frame.clicked.connect(self.toggleCollapsed)
        self._title_frame.clicked.connect(self.clicked)

    def toggleCollapsed(self):
        self._content.setVisible(self._is_collapsed)
        self._is_collapsed = not self._is_collapsed
        self._title_frame._arrow.setArrow(int(self._is_collapsed))
        self.resize(self.sizeHint())

    def enabled(self):
        return not self._is_collapsed

    ############################
    #           TITLE          #
    ############################
    class TitleFrame(QFrame):
        clicked = pyqtSignal()

        def __init__(self, parent=None, title="", collapsed=False):
            QFrame.__init__(self, parent=parent)

            self.setMinimumHeight(24)
            self.move(QPoint(24, 0))
            self.setStyleSheet("border:1px solid rgb(41, 41, 41); ")
            self.setFrameShadow(QFrame.Sunken)
            self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Maximum)

            self._hlayout = QHBoxLayout(self)
            self._hlayout.setContentsMargins(0, 0, 0, 0)
            self._hlayout.setSpacing(0)

            self._arrow = None
            self._title = None

            self._hlayout.addWidget(self.initArrow(collapsed))
            self._hlayout.addWidget(self.initTitle(title))

        def initArrow(self, collapsed):
            self._arrow = FrameLayout.Arrow(collapsed=collapsed)
            self._arrow.setStyleSheet("border:0px")

            return self._arrow

        def initTitle(self, title=None):
            self._title = QLabel(title)
            self._title.setMinimumHeight(24)
            self._title.move(QPoint(24, 0))
            self._title.setStyleSheet("border:0px")

            return self._title

        def mousePressEvent(self, event):
            self.clicked.emit()

            return super(FrameLayout.TitleFrame, self).mousePressEvent(event)

    #############################
    #           ARROW           #
    #############################
    class Arrow(QFrame):
        def __init__(self, parent=None, collapsed=False):
            QFrame.__init__(self, parent=parent)

            self.setMaximumSize(24, 24)

            # horizontal == 0
            self._arrow_horizontal = (QPointF(7.0, 8.0), QPointF(17.0, 8.0), QPointF(12.0, 13.0))
            # vertical == 1
            self._arrow_vertical = (QPointF(8.0, 7.0), QPointF(13.0, 12.0), QPointF(8.0, 17.0))
            # arrow
            self._arrow = None
            self.setArrow(int(collapsed))

        def setArrow(self, arrow_dir):
            if arrow_dir:
                self._arrow = self._arrow_vertical
            else:
                self._arrow = self._arrow_horizontal

        def paintEvent(self, event):
            painter = QPainter()
            painter.begin(self)
            painter.setBrush(QColor(192, 192, 192))
            painter.setPen(QColor(64, 64, 64))
            painter.drawPolygon(*self._arrow)
            painter.end()
