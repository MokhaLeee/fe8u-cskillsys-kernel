from PyQt5.QtWidgets import QWidget, QLineEdit, QMessageBox, QVBoxLayout, QTextEdit
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFontMetrics

from app.extensions.custom_gui import PropertyBox
from app.editor.lib.components.validated_line_edit import NidLineEdit

from app.utilities import str_utils

class LoreProperties(QWidget):
    def __init__(self, parent, current=None):
        super().__init__(parent)
        self.window = parent
        self._data = self.window._data

        self.current = current

        name_section = QVBoxLayout()

        self.nid_box = PropertyBox("Unique ID", NidLineEdit, self)
        self.nid_box.edit.textChanged.connect(self.nid_changed)
        self.nid_box.edit.editingFinished.connect(self.nid_done_editing)
        name_section.addWidget(self.nid_box)

        self.short_name_box = PropertyBox("Short Name", QLineEdit, self)

        self.short_name_box.edit.textChanged.connect(self.short_name_changed)
        name_section.addWidget(self.short_name_box)

        self.long_name_box = PropertyBox("Long Name", QLineEdit, self)

        self.long_name_box.edit.textChanged.connect(self.long_name_changed)
        name_section.addWidget(self.long_name_box)

        self.category_box = PropertyBox("Category", QLineEdit, self)

        self.category_box.edit.textChanged.connect(self.category_changed)
        name_section.addWidget(self.category_box)

        self.desc_box = PropertyBox("Description Text", QTextEdit, self)
        font_height = QFontMetrics(self.desc_box.edit.font())
        self.desc_box.edit.setFixedHeight(font_height.lineSpacing() * 20 + 20)
        self.desc_box.edit.textChanged.connect(self.desc_changed)

        total_section = QVBoxLayout()
        self.setLayout(total_section)
        total_section.addLayout(name_section)
        total_section.addWidget(self.desc_box)

        total_section.setAlignment(Qt.AlignTop)

    def nid_changed(self, text):
        # Also change name if they are identical
        if self.current.name == self.current.nid.replace('_', ' '):
            self.short_name_box.edit.setText(text.replace('_', ' '))
        if self.current.title == self.current.nid.replace('_', ' '):
            self.long_name_box.edit.setText(text.replace('_', ' '))
        self.current.nid = text
        self.window.update_list()

    def nid_done_editing(self):
        # Check validity of nid!
        other_nids = [d.nid for d in self._data.values() if d is not self.current]
        if self.current.nid in other_nids:
            QMessageBox.warning(self.window, 'Warning', 'Lore ID %s already in use' % self.current.nid)
            self.current.nid = str_utils.get_next_name(self.current.nid, other_nids)
        self.window.left_frame.model.on_nid_changed(self._data.find_key(self.current), self.current.nid)
        self._data.update_nid(self.current, self.current.nid)
        self.window.update_list()

    def short_name_changed(self, text):
        self.current.name = text

    def long_name_changed(self, text):
        self.current.title = text

    def category_changed(self, category):
        self.current.category = category
        self.window.update_list()

    def desc_changed(self, text=None):
        self.current.text = self.desc_box.edit.toPlainText()

    def set_current(self, current):
        self.current = current
        self.nid_box.edit.setText(current.nid)
        self.short_name_box.edit.setText(current.name)
        self.long_name_box.edit.setText(current.title)
        self.category_box.edit.setText(current.category)
        self.desc_box.edit.setText(current.text)
