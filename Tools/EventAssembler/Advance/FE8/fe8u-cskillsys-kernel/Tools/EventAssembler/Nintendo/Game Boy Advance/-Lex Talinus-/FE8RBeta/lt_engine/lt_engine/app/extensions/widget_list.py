from PyQt5.QtWidgets import QListWidget
from PyQt5.QtCore import pyqtSignal

import logging

class WidgetList(QListWidget):
    order_swapped = pyqtSignal(int, int)
    
    def __init__(self, parent):
        super().__init__(parent)
        self.window = parent
        self.index_list = []

        self.setDragEnabled(True)
        self.setAcceptDrops(True)
        self.setDragDropMode(4)  # Internal Move

        self.model().rowsMoved.connect(self.row_moved)

    def clear(self):
        super().clear()
        self.index_list.clear()

    def row_moved(self, parent, start, end, destination, row):
        elem = self.index_list.pop(start)
        logging.debug("WidgetList: %s moved from %s to %s with row %s", elem, start, end, row)
        if row > start:
            self.index_list.insert(row - 1, elem)
            self.order_swapped.emit(start, row - 1)
        else:
            self.index_list.insert(row, elem)
            self.order_swapped.emit(start, row)

    def dragMoveEvent(self, event):
        """
        Solves bug affecting a minority of Qt versions
        https://stackoverflow.com/a/74271152
        """
        target = self.row(self.itemAt(event.pos()))
        current = self.currentRow()
        if target == current + 1 or current == self.count() - 1 and target == -1:
            event.ignore()
        else:
            super().dragMoveEvent(event)
