from __future__ import annotations
from typing import Type
from collections.abc import Iterable
import copy
import json

from PyQt5.QtWidgets import QLineEdit, QTextEdit, QWidget, QHBoxLayout, QGridLayout, QPushButton, \
    QSizePolicy, QSplitter, QMessageBox, QApplication, QAbstractItemView
from PyQt5.QtCore import QSize, Qt, pyqtSignal
from PyQt5.QtCore import QAbstractListModel
from app.data.database.components import Component
from app.editor import timer

from app.utilities.data import Prefab
from app.extensions.custom_gui import RightClickListView
from app.editor.data_editor import SingleDatabaseEditor

from app.utilities import str_utils

import logging

if '_' not in globals():
    _ = lambda s: s

class Collection(QWidget):
    def __init__(self, deletion_criteria, collection_model: Type[DragDropCollectionModel], parent,
                 button_text="Create %s", view_type: Type[RightClickListView] = RightClickListView):
        super().__init__(parent)
        self.window = parent

        self._data = self.window._data
        self.title = self.window.title

        self.display = None

        grid = QGridLayout()
        self.setLayout(grid)

        self.view = view_type(deletion_criteria, self)
        self.view.currentChanged = self.on_item_changed

        self.model = collection_model(self._data, self)
        self.view.setModel(self.model)

        self.view.setIconSize(QSize(32, 32))

        self.setSizePolicy(QSizePolicy.Preferred, QSizePolicy.Preferred)

        self.button = QPushButton(button_text % self.title)
        self.button.clicked.connect(self.model.append)

        self.filter_field = QLineEdit()
        self.filter_field.setPlaceholderText('Filter by keyword, or by "nid"')
        self.filter_field.textChanged.connect(self.on_filter_changed)
        self.model.layoutChanged.connect(lambda _: self.on_filter_changed(self.filter_field.text()))

        grid.addWidget(self.filter_field, 0, 0, 1, 2)
        grid.addWidget(self.view, 1, 0, 1, 2)
        grid.addWidget(self.button, 2, 0, 1, 2)

        if self.window.allow_import_from_lt:
            self.import_button = QPushButton(_("Import Legacy data file..."))
            self.import_button.clicked.connect(self.window.import_data)
            grid.addWidget(self.import_button, 3, 0, 1, 2)

        if self.window.allow_import_from_csv:
            self.csv_import = QPushButton(_("Import from csv data file..."))
            self.csv_import.clicked.connect(self.window.import_csv)
            grid.addWidget(self.csv_import, 4, 0, 1, 2)

        if self.window.allow_copy_and_paste:
            self.copy_button = QPushButton(_("Copy to clipboard"))
            self.copy_button.clicked.connect(self.window.copy_data)
            grid.addWidget(self.copy_button, 5, 0)
            self.paste_button = QPushButton(_("Paste from clipboard"))
            self.paste_button.clicked.connect(self.window.paste_data)
            grid.addWidget(self.paste_button, 5, 1)

    def on_filter_changed(self, text: str):
        text = text.replace(' ', '')
        if not text:
            for i in range(self.model.rowCount()):
                self.view.setRowHidden(i, False)
            self.view.setDragEnabled(True)
            return
        try:
            for i in range(self.model.rowCount()):
                self.view.setRowHidden(i, False)
                index = self.model.index(i)

                # if quotations are used == strict mode
                match = False
                if len(text) > 2 and text.startswith('"'):
                    match_text = text[1:-1]
                    name: str = self.model.data(index, Qt.DisplayRole)
                    if match_text.lower() in name.lower():
                        match = True
                else: # search every subfield for occurrences of this string
                    item = self.model._data[index.row()]
                    item_attrs = [attr for attr in dir(item) if not callable(getattr(item, attr)) and not attr.startswith("__")]
                    for attr in item_attrs:
                        field = getattr(item, attr)
                        if isinstance(field, str):
                            if text.lower() in field.lower():
                                match = True
                                break
                        elif isinstance(field, Iterable):
                            for field_item in field:
                                if isinstance(field_item, str):
                                    if text.lower() in field_item.lower():
                                        match = True
                                        break
                        elif isinstance(field, Component) and isinstance(field.value, str):
                            if text.lower() in field.value.lower():
                                match = True
                                break
                if not match:
                    self.view.setRowHidden(i, True)

            self.view.setDragEnabled(False)
        except Exception as e:
            print(e)

    def on_item_changed(self, curr, prev):
        if self._data:
            new_data = curr.internalPointer()  # Internal pointer is way too powerful
            if not new_data:
                if self._data:
                    new_data = self._data[curr.row()]
                elif self.display:
                    self.display.setEnabled(False)
            if self.display:
                self.display.set_current(new_data)
                self.display.setEnabled(True)

    def set_display(self, disp):
        self.display = disp
        first_index = self.model.index(0)
        self.view.setCurrentIndex(first_index)

    def set_current_row(self, idx):
        index = self.model.index(idx)
        self.view.setCurrentIndex(index)

    def update_list(self):
        self.model.dataChanged.emit(self.model.index(
            0), self.model.index(self.model.rowCount()))


class DatabaseTab(QWidget):
    allow_import_from_lt = False
    allow_import_from_csv = False
    allow_copy_and_paste = False

    def __init__(self, data, title, right_frame, deletion_criteria, collection_model, parent,
                 button_text="Create %s", view_type=RightClickListView, collection_type=Collection):
        QWidget.__init__(self, parent)
        self.window = parent
        self._data = data
        self.title = title

        self.setWindowTitle(_('%s Editor') % self.title)
        self.setStyleSheet("font: 10pt;")
        self.left_frame = collection_type(
            deletion_criteria, collection_model, self, button_text=button_text, view_type=view_type)
        self.right_frame = right_frame(self)
        self.left_frame.set_display(self.right_frame)

        self.splitter = QSplitter(self)
        self.splitter.setChildrenCollapsible(False)
        self.splitter.addWidget(self.left_frame)
        self.splitter.addWidget(self.right_frame)
        self.splitter.setStyleSheet(
            "QSplitter::handle:horizontal {background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc); border: 1px solid #777; width: 13px; margin-top: 2px; margin-bottom: 2px; border-radius: 4px;}")

        self.layout = QHBoxLayout(self)
        self.setLayout(self.layout)

        self.layout.addWidget(self.splitter)

        # Check this on startup
        self.reset()

    def update_list(self):
        self.left_frame.update_list()

    def tick(self):
        pass

    def reset(self):
        """
        Whenever the tab is changed, make sure to update the tab display
        Makes sure that current is what is being displayed
        """
        if self.right_frame.current:
            self.right_frame.setEnabled(True)
            self.right_frame.set_current(self.right_frame.current)
        else:
            self.right_frame.setEnabled(False)

    def on_tab_close(self):
        pass

    @classmethod
    def edit(cls, parent=None):
        timer.get_timer().stop_for_editor()  # Don't need these while running game
        window = SingleDatabaseEditor(cls, parent)
        window.exec_()
        timer.get_timer().start_for_editor()

    def copy_data(self):
        clipboard = QApplication.clipboard()
        view = self.left_frame.view
        selected_indices = view.selectionModel().selectedIndexes()
        ser_list = []
        for index in selected_indices:
            row = index.row()
            current_obj = self._data[row]
            ser_dict = current_obj.save()
            ser_list.append(ser_dict)
        json_string = json.dumps(ser_list)
        clipboard.setText(json_string)

    def paste_data(self):
        view = self.left_frame.view
        selected_indices = view.selectionModel().selectedIndexes()
        if selected_indices:
            row = selected_indices[0].row() + 1
        else:
            row = len(self._data)

        clipboard = QApplication.clipboard()
        json_string = clipboard.text()
        try:
            ser_list = json.loads(json_string)
            for ser_dict in reversed(ser_list):
                new_obj = self._data.datatype.restore(ser_dict)
                new_obj.nid = str_utils.get_next_name(new_obj.nid, self._data.keys())
                self._data.insert(row, new_obj)
            self.update_list()
        except Exception as e:
            logging.warning("Could not read from clipboard! %s" % e)
            QMessageBox.critical(None, "Import Error", "Could not read valid json from clipboard!")

class CollectionModel(QAbstractListModel):
    allow_delete_last_obj = False

    def __init__(self, data, window):
        super().__init__(window)
        self._data = data
        self.window = window

    def rowCount(self, parent=None):
        return len(self._data)

    def data(self, index, role):
        raise NotImplementedError

    def delete(self, idx):
        # special case for 1-length data
        if not self.allow_delete_last_obj and len(self._data) == 1:
            QMessageBox.critical(None, "Deletion Error", "Can not delete last object of a kind!")
            return

        new_index = 0
        # If deleting the element at the bottom of the list
        if idx == len(self._data) - 1:
            new_index = self.index(idx - 1)
        else:
            new_index = self.index(idx + 1)
        self.window.view.setCurrentIndex(new_index)
        self._data.pop(idx)
        new_index = self.index(min(idx, len(self._data) - 1))
        self.window.view.setCurrentIndex(new_index)
        self.layoutChanged.emit()
        return new_index

    def update(self):
        # self.dataChanged.emit(self.index(0), self.index(self.rowCount()))
        self.layoutChanged.emit()

    def move_to_bottom(self):
        view = self.window.view
        self.dataChanged.emit(self.index(0), self.index(self.rowCount()))
        self.layoutChanged.emit()
        last_index = self.index(self.rowCount() - 1)
        view.setCurrentIndex(last_index)
        view.scrollTo(last_index, QAbstractItemView.EnsureVisible)
        return last_index

    def create_new(self):
        raise NotImplementedError

    def append(self):
        new_item = self.create_new()
        if not new_item:
            return
        return self.move_to_bottom()

    def new(self, idx):
        new_item = self.create_new()
        if not new_item:
            return
        view = self.window.view
        self._data.move_index(len(self._data) - 1, idx + 1)
        self.layoutChanged.emit()
        new_index = self.index(idx + 1)
        view.setCurrentIndex(new_index)
        return new_index

    def duplicate(self, idx):
        view = self.window.view
        obj = self._data[idx]
        new_nid = str_utils.get_next_name(obj.nid, self._data.keys())
        if isinstance(obj, Prefab):
            serialized_obj = obj.save()
            new_obj = self._data.datatype.restore(serialized_obj)
        else:
            new_obj = copy.copy(obj)
        new_obj.nid = new_nid
        self._data.insert(idx + 1, new_obj)
        self.layoutChanged.emit()
        new_index = self.index(idx + 1)
        view.setCurrentIndex(new_index)
        return new_index

class DragDropCollectionModel(CollectionModel):
    drop_to = None
    most_recent_dragdrop = None
    drag_drop_finished = pyqtSignal()

    def supportedDropActions(self):
        return Qt.MoveAction

    def supportedDragActions(self):
        return Qt.MoveAction

    def insertRows(self, row, count, parent):
        if count < 1 or row < 0 or row > self.rowCount() or parent.isValid():
            return False
        self.drop_to = row
        self.layoutChanged.emit()
        return True

    def do_drag_drop(self, index):
        if self.drop_to is None:
            return False
        if index < self.drop_to:
            self._data.move_index(index, self.drop_to - 1)
            return index, self.drop_to - 1
        else:
            self._data.move_index(index, self.drop_to)
            return index, self.drop_to

    def removeRows(self, row, count, parent):
        if count < 1 or row < 0 or (row + count) > self.rowCount() or parent.isValid():
            return False
        self.most_recent_dragdrop = self.do_drag_drop(row)
        self.layoutChanged.emit()
        self.drop_to = None

        if self.most_recent_dragdrop:
            old, new = self.most_recent_dragdrop
            view = self.window.view
            new_index = self.index(new)
            view.setCurrentIndex(new_index)
        self.drag_drop_finished.emit()
        return True

    def flags(self, index):
        if not index.isValid() or index.row() >= len(self._data) or index.model() is not self:
            return Qt.ItemIsDropEnabled
        else:
            return Qt.ItemIsDragEnabled | super().flags(index)


class ResourceCollectionModel(DragDropCollectionModel):
    def setData(self, index, value, role):
        if not index.isValid():
            return False
        if role == Qt.EditRole and self.drop_to is None:
            if value:
                item = self._data[index.row()]
                old_nid = item.nid
                nids = [d.nid for d in self._data if d is not item]
                nid = str_utils.get_next_name(value, nids)
                self._data.update_nid(item, nid)
                self.on_nid_changed(old_nid, nid)
        return True

    def flags(self, index):
        flags = super().flags(index)
        if not index.isValid():
            return flags
        return flags | Qt.ItemIsEditable

    def on_nid_changed(self, old_nid, new_nid):
        pass
