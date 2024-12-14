from PyQt5.QtWidgets import QWidget, QGridLayout, QLabel, QHBoxLayout, QPushButton
from PyQt5.QtCore import Qt, pyqtSignal

from app.data.database.database import DB

from app.extensions.custom_gui import QHLine
from app.editor.multi_combo_box_list import MultiComboBoxListWithCheckbox
from app.editor.item_editor.item_model import get_pixmap

class ItemListWidget(QWidget):
    items_updated = pyqtSignal()

    def __init__(self, title, parent=None):
        super().__init__(parent)
        self.window = parent

        self.item_list = MultiComboBoxListWithCheckbox(DB.items, get_pixmap, self)
        self.item_list.item_changed.connect(self.activate)

        self.layout = QGridLayout(self)
        self.layout.setSpacing(0)
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.addWidget(self.item_list, 3, 0, 1, 2)
        self.setLayout(self.layout)

        label = QLabel(title)
        label.setAlignment(Qt.AlignBottom)
        self.layout.addWidget(label, 0, 0)

        header1 = QLabel("Item ID")
        header1.setAlignment(Qt.AlignBottom | Qt.AlignLeft)
        self.layout.addWidget(header1, 2, 0)

        header2 = QLabel("Droppable")
        header2.setAlignment(Qt.AlignBottom | Qt.AlignRight)
        self.layout.addWidget(header2, 2, 1)

        hline = QHLine()
        self.layout.addWidget(hline, 1, 0, 1, 2)

        hbox = QHBoxLayout()
        hbox.setSpacing(0)
        hbox.setContentsMargins(0, 0, 0, 0)

        add_button = QPushButton("+")
        add_button.setMaximumWidth(30)
        add_button.clicked.connect(self.add_new_item)

        remove_button = QPushButton("-")
        remove_button.setMaximumWidth(30)
        remove_button.clicked.connect(self.remove_last_item)

        hbox.addWidget(remove_button, alignment=Qt.AlignRight)
        hbox.addWidget(add_button, alignment=Qt.AlignRight)

        self.layout.addLayout(hbox, 0, 1, alignment=Qt.AlignRight)

    def set_current(self, items):
        self.item_list.set_current(items)

    def add_new_item(self):
        if DB.items:
            new_item = DB.items[0].nid
            self.item_list.add_item(new_item)
        self.activate()

    def remove_last_item(self):
        if self.item_list.length() > 0:
            self.item_list.remove_item_at_index(self.item_list.length() - 1)
        self.activate()

    def activate(self):
        self.items_updated.emit()

    def get_items(self):
        return self.item_list.index_list[:]

    def set_color(self, color_list):
        self.item_list.set_color(color_list)
