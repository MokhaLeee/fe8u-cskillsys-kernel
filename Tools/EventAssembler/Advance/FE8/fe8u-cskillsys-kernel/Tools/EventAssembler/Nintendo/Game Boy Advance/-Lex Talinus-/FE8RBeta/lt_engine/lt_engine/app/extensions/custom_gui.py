from __future__ import annotations
from typing import Any, Generic, List, Optional, Tuple, Type, TypeVar

from dataclasses import dataclass

from PyQt5.QtCore import QAbstractItemModel, QItemSelectionModel, QSize, Qt
from PyQt5.QtWidgets import (QAbstractItemView, QAction, QApplication,
                             QComboBox, QDialog, QDialogButtonBox, QFrame,
                             QHBoxLayout, QItemDelegate, QLabel, QLineEdit,
                             QListView, QMenu, QPushButton, QSizePolicy,
                             QSpinBox, QTableView, QTreeView, QVBoxLayout,
                             QWidget, QTabWidget)


class SimpleDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowFlag(Qt.WindowContextHelpButtonHint, False)

    @classmethod
    def edit(cls, parent):
        dialog = cls(parent)
        dialog.exec_()

class Dialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowFlag(Qt.WindowContextHelpButtonHint, False)

        self.buttonbox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel, Qt.Horizontal, self)
        self.buttonbox.accepted.connect(self.accept)
        self.buttonbox.rejected.connect(self.reject)

@dataclass
class DeletionTab:
    """
    Contains all the information needed to render a single tab in the 
    DeletionDialog below
    """
    affected_items: List[Any]  # What prefabs will be affected by the deletion
    model: QAbstractItemModel  # How do we display the prefabs to the user
    msg: str  # Informs the user about what will happen if they proceed
    label: str  # What kind of things are being deleted?

class DeletionDialog(Dialog):
    def __init__(self, tabs: List[DeletionTab], swap_box: Optional[ComboBox], parent=None):
        super().__init__(parent)
        self.setWindowTitle("Deletion Warning")
        self.layout = QVBoxLayout()
        self.setLayout(self.layout)

        self.tab_bar = QTabWidget(self)
        for tab in tabs:
            model = tab.model(tab.affected_items, parent)
            view = QListView(self)
            view.setModel(model)
            view.setSelectionMode(0)  # No selection
            view.setIconSize(QSize(32, 32))

            layout = QVBoxLayout()
            layout.addWidget(QLabel(tab.msg))
            layout.addWidget(view)

            frame = QFrame(self)
            frame.setLayout(layout)

            self.tab_bar.addTab(frame, tab.label)

        self.layout.addWidget(self.tab_bar)
        self.swap_box = swap_box
        if self.swap_box:
            self.layout.addWidget(QLabel("Swap these references to:"))
            self.layout.addWidget(self.swap_box)
        self.layout.addWidget(self.buttonbox)

    @staticmethod
    def get_swap(tabs: List[DeletionTab], swap_box: ComboBox, 
                 parent=None) -> Tuple[Optional[Any], bool]:
        dialog = DeletionDialog(tabs, swap_box, parent)
        result = dialog.exec_()
        if result == QDialog.Accepted:
            idx = dialog.swap_box.edit.currentIndex()
            return dialog.swap_box.model._data[idx], True
        else:
            return None, False

    @staticmethod
    def get_simple_swap(tabs: List[DeletionTab], swap_box: ComboBox, 
                        parent=None) -> Tuple[Optional[int], bool]:
        dialog = DeletionDialog(tabs, swap_box, parent)
        result = dialog.exec_()
        if result == QDialog.Accepted:
            idx = dialog.swap_box.edit.currentIndex()
            return idx, True
        else:
            return None, False

    @staticmethod
    def inform(tabs: List[DeletionTab], parent=None) -> bool:
        dialog = DeletionDialog(tabs, None, parent)
        result = dialog.exec_()
        if result == QDialog.Accepted:
            return True
        else:
            return False

class QHLine(QFrame):
    def __init__(self):
        super().__init__()
        self.setFrameShape(QFrame.HLine)
        self.setFrameShadow(QFrame.Plain)
        self.setLineWidth(1)

class QVLine(QFrame):
    def __init__(self):
        super().__init__()
        self.setFrameShape(QFrame.VLine)
        self.setFrameShadow(QFrame.Plain)
        self.setLineWidth(1)

class ComboBox(QComboBox):
    def setValue(self, text):
        i = self.findText(text)
        if i >= 0:
            self.setCurrentIndex(i)

class LineSearch(QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        layout = QHBoxLayout()
        self.line_edit = QLineEdit(self)
        self.line_edit.setReadOnly(True)
        self.search_button = QPushButton('...', self)
        self.search_button.setMaximumWidth(40)
        layout.setSpacing(0)
        layout.addWidget(self.line_edit)
        layout.addWidget(self.search_button)
        self.setLayout(layout)

T = TypeVar('T')

class PropertyBox(QWidget, Generic[T]):
    def __init__(self, label, widget: Type[T], parent=None, horiz_layout=False):
        super().__init__(parent)
        self.window = parent

        self.setSizePolicy(QSizePolicy.Preferred, QSizePolicy.Fixed)

        if horiz_layout:
            _layout = QHBoxLayout()
        else:
            _layout = QVBoxLayout()
        self.setLayout(_layout)
        _layout.setSpacing(0)
        _layout.setContentsMargins(0, 0, 0, 0)

        self.label = QLabel(label, self)
        if horiz_layout:
            self.label.setAlignment(Qt.AlignLeft)
        else:
            self.label.setAlignment(Qt.AlignBottom)
        size_policy = QSizePolicy.Preferred
        self.label.setSizePolicy(size_policy, QSizePolicy.Fixed)
        self.edit: T = widget(self)
        self.edit.setSizePolicy(size_policy, QSizePolicy.Fixed)
        self.bottom_section = QHBoxLayout()
        self.bottom_section.addWidget(self.edit)

        _layout.addWidget(self.label)
        _layout.addLayout(self.bottom_section)

    def add_button(self, button):
        self.button = button
        self.bottom_section.addWidget(self.button)

class PropertyCheckBox(QWidget):
    def __init__(self, label, widget, parent=None):
        super().__init__(parent)

        _layout = QHBoxLayout()
        self.setLayout(_layout)
        _layout.setSpacing(0)
        _layout.setContentsMargins(0, 0, 0, 0)

        # self.label = QLabel(label, self)
        # self.label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.edit = widget(label, self)
        self.edit.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

        _layout.addWidget(self.edit)
        # _layout.addWidget(self.label)
        # _layout.setAlignment(self.label, Qt.AlignLeft)

class RightClickView():
    def __init__(self, action_funcs=None, parent=None):
        super().__init__(parent)
        self.window = parent

        self.setDragEnabled(True)
        self.setAcceptDrops(True)
        self.setDropIndicatorShown(True)
        self.setDragDropMode(4)  # QAbstractItemView.InternalMove
        self.setDragDropOverwriteMode(False)

        if action_funcs:
            self.can_delete, self.can_duplicate, self.can_rename = action_funcs
        else:
            self.can_delete, self.can_duplicate, self.can_rename = None, None, None

        self.setContextMenuPolicy(Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.customMenuRequested)

    def customMenuRequested(self, pos):
        index = self.indexAt(pos)
        menu = QMenu(self)

        new_action = QAction("New", self, triggered=lambda: self.new(index))
        menu.addAction(new_action)
        # Check to see if we're actually selecting something
        if index.isValid():
            duplicate_action = QAction("Duplicate", self, triggered=lambda: self.duplicate(index))
            menu.addAction(duplicate_action)
            delete_action = QAction("Delete", self, triggered=lambda: self.delete(index))
            menu.addAction(delete_action)
            if self.can_duplicate and not self.can_duplicate(self.model(), index):
                duplicate_action.setEnabled(False)
            if self.can_delete and not self.can_delete(self.model(), index):
                delete_action.setEnabled(False)

        menu.popup(self.viewport().mapToGlobal(pos))

    def new(self, index):
        idx = index.row()
        self.model().new(idx)
        new_index = self.model().index(idx + 1)
        self.setCurrentIndex(new_index)

    def duplicate(self, index):
        idx = index.row()
        self.model().duplicate(idx)
        new_index = self.model().index(idx + 1)
        self.setCurrentIndex(new_index)

    def delete(self, index):
        idx = index.row()
        self.model().delete(idx)
        new_index = self.model().index(min(idx, self.model().rowCount() - 1))
        self.setCurrentIndex(new_index)

    def delete_all(self):
        self.model().delete_all()
        new_index = self.model().index(0)
        self.setCurrentIndex(new_index)

    def keyPressEvent(self, event):
        super().keyPressEvent(event)
        if event.key() == Qt.Key_Delete:
            indices = self.selectionModel().selectedIndexes()
            for index in indices:
                if not self.can_delete or self.can_delete(self.model(), index):
                    self.delete(index)
                    break  # Only ever delete one
        elif event.key() == Qt.Key_D and (QApplication.keyboardModifiers() & Qt.ControlModifier):
            indices = self.selectionModel().selectedIndexes()
            for index in indices:
                if not self.can_duplicate or self.can_duplicate(self.model(), index):
                    self.duplicate(index)
                    break  # Only ever duplicate one

    def mousePressEvent(self, event):
        index = self.indexAt(event.pos())
        is_selected = self.selectionModel().isSelected(index)
        super().mousePressEvent(event)
        if not index.isValid() or is_selected:
            self.clearSelection()
            self.selectionModel().setCurrentIndex(index, QItemSelectionModel.Select)

class RightClickTreeView(RightClickView, QTreeView):
    pass

class RightClickListView(RightClickView, QListView):
    pass

class RightClickTableView(RightClickView, QTableView):
    pass

class ResourceView(RightClickView):
    def check_index(self, index):
        return True

    def customMenuRequested(self, pos):
        index = self.indexAt(pos)
        if not self.check_index(index):
            return

        menu = QMenu(self)
        new_action = QAction("New", self, triggered=lambda: self.new(index))
        menu.addAction(new_action)

        # Check to see if we're actually selecting something
        if index.isValid():
            rename_action = QAction("Rename", self, triggered=lambda: self.edit(index))
            menu.addAction(rename_action)
            delete_action = QAction("Delete", self, triggered=lambda: self.delete(index))
            menu.addAction(delete_action)
            if self.can_rename and not self.can_rename(self.model(), index):
                rename_action.setEnabled(False)
            if self.can_delete and not self.can_delete(self.model(), index):
                delete_action.setEnabled(False)

        menu.popup(self.viewport().mapToGlobal(pos))

class ResourceListView(ResourceView, QListView):
    pass

class TableView(RightClickTableView):
    def __init__(self, action_funcs=None, parent=None):
        super().__init__(action_funcs, parent)
        self.setSelectionMode(QAbstractItemView.SingleSelection)

        self.setDragEnabled(False)
        self.setAcceptDrops(False)
        self.setDropIndicatorShown(False)
        self.setDragDropMode(0)

    def check_index(self, index):
        return True

    def delete(self, index):
        indices = self.selectionModel().selectedIndexes()
        orig_rows = [index.row() for index in indices]
        rows = []
        for i, index in enumerate(indices):
            if orig_rows[i] in rows:
                continue  # Already deleted
            self.model().delete(index)
            rows.append(orig_rows[i])

    # def delete(self, index):
    #     self.model().delete(index)

    def duplicate(self, index):
        new_index = self.model().duplicate(index)
        if new_index:
            self.setCurrentIndex(new_index)

    def new(self, index):
        new_index = self.model().new(index)
        if new_index:
            self.setCurrentIndex(new_index)

class ResourceTableView(ResourceView, TableView):
    pass

class MultiselectTableView(ResourceTableView):
    def __init__(self, action_funcs=None, parent=None):
        super().__init__(action_funcs, parent)
        self.setSelectionMode(QAbstractItemView.ExtendedSelection)

class ResourceMultiselectListView(ResourceListView):
    def __init__(self, action_funcs=None, parent=None):
        super().__init__(action_funcs, parent)
        self.setSelectionMode(QAbstractItemView.ExtendedSelection)

class ResourceTreeView(ResourceView, QTreeView):
    def check_index(self, index):
        item = index.internalPointer()
        if item.parent_image:
            return False
        return True

    def delete(self, index):
        self.model().delete(index)

    # def keyPressEvent(self, event):
    #     RightClickView.keyPressEvent(self, event)

    def mousePressEvent(self, event):  # Try using the old version of this
        QTreeView.mousePressEvent(self, event)

class IntDelegate(QItemDelegate):
    def __init__(self, parent, int_columns):
        super().__init__(parent)
        self.int_columns = int_columns

    def createEditor(self, parent, option, index):
        if index.column() in self.int_columns:
            editor = QSpinBox(parent)
            editor.setAlignment(Qt.AlignRight)
            editor.setRange(-1023, 1023)
            return editor
        else:
            return super().createEditor(parent, option, index)
