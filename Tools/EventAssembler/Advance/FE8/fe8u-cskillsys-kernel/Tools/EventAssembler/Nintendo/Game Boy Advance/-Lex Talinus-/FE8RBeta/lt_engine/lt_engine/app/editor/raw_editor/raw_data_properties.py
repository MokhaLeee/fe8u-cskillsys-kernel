from app.editor.lib.components.validated_line_edit import NidLineEdit
from app.editor.lib.components.database_delegate import DBNamesDelegate
from typing import List

from app.data.database import raw_data
from app.extensions.custom_gui import ComboBox, PropertyBox
from app.extensions.generic_attr_object_delegate import (
    GenericObjectDelegate, GenericObjectListModel)
from app.extensions.key_value_delegate import (KeyValueDelegate,
                                               KeyValueDoubleListModel)
from app.extensions.list_widgets import AppendMultiListWidget, MutableAppendMultiListWidget
from app.utilities import str_utils
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFontMetrics
from PyQt5.QtWidgets import (QInputDialog, QLineEdit, QMessageBox,
                             QPushButton, QTextEdit, QVBoxLayout, QWidget)


class RawDataProperties(QWidget):
    def __init__(self, parent, current=None):
        super().__init__(parent)
        self.window = parent
        self._data = self.window._data

        self.current: raw_data.RawDataPrefab = current

        right_section = QVBoxLayout()

        self.nid_box = PropertyBox("Raw ID", NidLineEdit, self)
        self.nid_box.edit.textChanged.connect(self.nid_changed)
        self.nid_box.edit.editingFinished.connect(self.nid_done_editing)
        right_section.addWidget(self.nid_box)

        self.type_box = PropertyBox("Type", ComboBox, self)
        self.type_box.edit.addItems(raw_data.valid_types)
        self.type_box.edit.currentIndexChanged.connect(self.type_changed)
        right_section.addWidget(self.type_box)

        self.str_box = PropertyBox("Raw Data String", QTextEdit, self)
        font_height = QFontMetrics(self.str_box.edit.font())
        self.str_box.edit.setFixedHeight(font_height.lineSpacing() * 20 + 20)
        self.str_box.edit.textChanged.connect(self.data_changed)
        self.str_box.hide()

        self.kv_box = AppendMultiListWidget([], "Raw Data List", ("key", "value"), DBNamesDelegate, self, model=KeyValueDoubleListModel)
        self.kv_box.hide()

        self.add_column_button = QPushButton("Add Column...")
        self.add_column_button.clicked.connect(self.add_column_dialog)
        self.add_column_button.hide()

        self.sheet_box = MutableAppendMultiListWidget([], "Raw Data Sheet", tuple(self.current.oattrs) if self.current else (), DBNamesDelegate, self, model=GenericObjectListModel,
                                                      rename_column_action=self.rename_column_dialog, delete_column_action=self.remove_column)
        self.sheet_box.hide()

        total_section = QVBoxLayout()
        self.setLayout(total_section)
        total_section.addLayout(right_section)
        total_section.addWidget(self.str_box)
        total_section.addWidget(self.kv_box)
        total_section.addWidget(self.add_column_button)
        total_section.addWidget(self.sheet_box)

        total_section.setAlignment(Qt.AlignTop)

    def nid_changed(self, text):
        self.current.nid = text
        self.window.update_list()

    def nid_done_editing(self):
        # Check validity of nid!
        other_nids = [d.nid for d in self._data.values() if d is not self.current]
        if self.current.nid in other_nids:
            QMessageBox.warning(self.window, 'Warning', 'NID %s already in use' % self.current.nid)
            self.current.nid = str_utils.get_next_name(self.current.nid, other_nids)
        self.window.left_frame.model.on_nid_changed(self._data.find_key(self.current), self.current.nid)
        self._data.update_nid(self.current, self.current.nid)
        self.window.update_list()

    def type_changed(self):
        self.current.dtype = self.type_box.edit.currentText()
        self.str_box.hide()
        self.kv_box.hide()
        self.add_column_button.hide()
        self.sheet_box.hide()
        if self.current.dtype == "str":
            self.str_box.show()
        elif self.current.dtype == "kv":
            self.kv_box.show()
        elif self.current.dtype == "list":
            self.add_column_button.show()
            self.sheet_box.show()

    def data_changed(self, text=None):
        self.current.svalue = self.str_box.edit.toPlainText()

    def set_current(self, current):
        self.current = current
        self.nid_box.edit.setText(current.nid)
        self.type_box.edit.setValue(current.dtype)
        self.str_box.edit.setText(current.svalue)
        self.kv_box.set_current(current.kvvalue)
        self.rerender_sheet_widget()
        self.type_changed()

    def rerender_sheet_widget(self):
        self.sheet_box.setParent(None)
        self.sheet_box = MutableAppendMultiListWidget([], "Raw Data", tuple(self.current.oattrs), DBNamesDelegate, self, model=GenericObjectListModel,
                                                      rename_column_action=self.rename_column_dialog, delete_column_action=self.remove_column)
        self.layout().addWidget(self.sheet_box)
        self.sheet_box.set_current(self.current.lovalue)

    def add_column(self, col_name):
        attr = str(col_name)
        if attr in self.current.oattrs:
            return
        self.current.oattrs.append(attr)
        for o in self.current.lovalue:
            try:
                getattr(o, attr)
            except:
                setattr(o, attr, "")
        self.rerender_sheet_widget()

    def remove_column(self, col_idx):
        if (col_idx >= len(self.current.oattrs)
            or col_idx == 0): # can't delete nid column
            return
        attr = self.current.oattrs[col_idx]
        self.current.oattrs.remove(attr)
        self.rerender_sheet_widget()

    def rename_column(self, col_idx, text):
        new_attr = text
        old_attr = self.current.oattrs[col_idx]
        self.current.oattrs[col_idx] = text
        for o in self.current.lovalue:
            try:
                old_val = getattr(o, old_attr)
                setattr(o, new_attr, old_val)
            except:
                setattr(o, new_attr, "")
        self.rerender_sheet_widget()

    def rename_column_dialog(self, col_idx):
        if (col_idx >= len(self.current.oattrs)
            or col_idx == 0): # can't rename nid column
            return
        text, ok = QInputDialog.getText(self, 'Rename Column', 'Column Name:')
        if ok:
            self.rename_column(col_idx, text)

    def add_column_dialog(self):
        text, ok = QInputDialog.getText(self, 'New Column', 'New Column Name:')
        if ok:
            self.add_column(text)

    def get_attrs(self, obj) -> List[str]:
        attributes = dir(obj)
        return [a for a in attributes if not(a.startswith('__') and a.endswith('__'))]
