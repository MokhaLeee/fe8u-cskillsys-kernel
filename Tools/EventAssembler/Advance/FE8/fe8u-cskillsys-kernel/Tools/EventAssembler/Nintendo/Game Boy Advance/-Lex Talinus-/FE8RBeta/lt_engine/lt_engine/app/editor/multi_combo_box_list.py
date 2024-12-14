from functools import partial

from PyQt5.QtWidgets import QListWidget, QListWidgetItem, QWidget, QHBoxLayout, QCheckBox
from PyQt5.QtCore import pyqtSignal, Qt
from PyQt5.QtGui import QIcon, QPalette

from app.extensions.custom_gui import ComboBox

class MultiComboBoxList(QListWidget):
    item_changed = pyqtSignal()

    def __init__(self, data, pixmap_func, parent):
        super().__init__(parent)
        self.window = parent
        self._data = data
        self.pixmap_func = pixmap_func
        self.index_list = []
        self.combo_box_list = []

    def length(self):
        return len(self.index_list)

    def add_item(self, item_nid):
        new_box = QListWidgetItem()
        combo_box = ComboBox(self)
        for i in self._data:
            if self.pixmap_func:
                pix = self.pixmap_func(i)
                if pix:
                    combo_box.addItem(QIcon(pix), i.nid)
                else:
                    combo_box.addItem(i.nid)
            else:
                combo_box.addItem(i.nid)
        combo_box.setValue(item_nid)
        self.addItem(new_box)
        self.setItemWidget(new_box, combo_box)
        corrected_item_nid = combo_box.currentText()
        self.index_list.append(corrected_item_nid)
        self.combo_box_list.append(combo_box)
        idx = len(self.combo_box_list) - 1
        combo_box.currentIndexChanged.connect(partial(self.on_item_change, idx))
        return corrected_item_nid

    def remove_item(self, item_nid):
        if item_nid in self.index_list:
            idx = self.index_list.index(item_nid)
            self.index_list.pop(idx)
            self.combo_box_list.pop(idx)
            return self.takeItem(idx)
        return None

    def remove_item_at_index(self, idx):
        if len(self.index_list) > idx:
            self.index_list.pop(idx)
            self.combo_box_list.pop(idx)
            return self.takeItem(idx)
        return None

    def clear(self):
        super().clear()
        self.index_list.clear()
        self.combo_box_list.clear()

    def set_current(self, items):
        self.clear()
        for i in items:
            self.add_item(i)
        self.item_changed.emit()

    def on_item_change(self, index):
        combo_box = self.combo_box_list[index]
        item_nid = combo_box.currentText()
        self.index_list[index] = item_nid
        self.item_changed.emit()

    def set_color(self, color_list):
        for idx, box in enumerate(self.combo_box_list):
            palette = box.palette()
            palette.setColor(QPalette.Text, color_list[idx])
            box.setPalette(palette)

class ItemBox(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        self.combo_box = ComboBox(self)
        self.check_box = QCheckBox(self)
        self.check_box.setMaximumWidth(50)

        layout = QHBoxLayout()
        self.setLayout(layout)
        # layout.setSpacing(0)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.combo_box, 1)
        layout.addWidget(self.check_box)

class MultiComboBoxListWithCheckbox(MultiComboBoxList):
    def add_item(self, item_nid, droppable=False):
        new_box = QListWidgetItem()

        item_box = ItemBox(self)
        combo_box = item_box.combo_box
        for i in self._data:  # DB.items
            if self.pixmap_func:
                pix = self.pixmap_func(i)
                if pix:
                    combo_box.addItem(QIcon(pix), i.nid)
                else:
                    combo_box.addItem(i.nid)
            else:
                combo_box.addItem(i.nid)

        combo_box.setValue(item_nid)
        self.addItem(new_box)
        self.setItemWidget(new_box, item_box)

        corrected_item_nid = combo_box.currentText()
        self.index_list.append([corrected_item_nid, droppable])
        self.combo_box_list.append(item_box)

        idx = len(self.combo_box_list) - 1
        combo_box.currentIndexChanged.connect(partial(self.on_item_change, idx))
        check_box = item_box.check_box
        check_box.setChecked(droppable)
        check_box.toggled.connect(partial(self.on_item_change, idx))

        return corrected_item_nid

    def remove_item(self, item_nid):
        nid_list = [i[0] for i in self.index_list]
        if item_nid in nid_list:
            idx = nid_list.index(item_nid)
            self.index_list.pop(idx)
            self.combo_box_list.pop(idx)
            return self.takeItem(idx)
        return None

    def set_current(self, items):
        self.clear()
        for (item_nid, droppable) in items:
            self.add_item(item_nid, droppable)
        self.item_changed.emit()

    def on_item_change(self, idx):
        item_box = self.combo_box_list[idx]
        item_nid = item_box.combo_box.currentText()
        droppable = bool(item_box.check_box.isChecked())
        self.index_list[idx] = [item_nid, droppable]
        self.item_changed.emit()

    def set_color(self, color_list):
        for idx, box in enumerate(self.combo_box_list):
            combo_box = box.combo_box
            palette = combo_box.palette()
            palette.setColor(QPalette.Text, color_list[idx])
            combo_box.setPalette(palette)
