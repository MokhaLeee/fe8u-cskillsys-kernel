from typing import Callable
from PyQt5 import QtGui
from PyQt5.QtWidgets import QAction, QWidget, QGridLayout, QLabel, QPushButton, QTreeView, QMenu
from PyQt5.QtCore import Qt

from app.extensions.custom_gui import RightClickTreeView
from app.extensions.list_models import SingleListModel, DefaultMultiAttrListModel

MIN_NID_COLUMN_WIDTH = 80

class BasicSingleListWidget(QWidget):
    def __init__(self, data, title, dlgate, parent=None):
        super().__init__(parent)
        self.initiate(data, parent)
        self.title = title

        self.model = SingleListModel(self.current, title, self)
        self.view = QTreeView(self)
        self.view.setModel(self.model)
        delegate = dlgate(self.view)
        self.view.setItemDelegate(delegate)
        self.view.resizeColumnToContents(0)
        self.view.setColumnWidth(0, max(MIN_NID_COLUMN_WIDTH, self.view.columnWidth(0)))

        self.placement(data, title)

    def initiate(self, data, parent):
        self.window = parent
        self.current = data

    def placement(self, data, title):
        self.layout = QGridLayout(self)
        self.layout.setSpacing(0)
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.addWidget(self.view, 1, 0, 1, 2)
        self.setLayout(self.layout)

        label = QLabel(title)
        label.setAlignment(Qt.AlignBottom)
        self.layout.addWidget(label, 0, 0)

    def set_current(self, data):
        self.current = data
        self.model.set_new_data(self.current)
        for col in range(self.model.columnCount()):
            self.view.resizeColumnToContents(col)
            self.view.setColumnWidth(0, max(MIN_NID_COLUMN_WIDTH, self.view.columnWidth(0)))

class AppendSingleListWidget(BasicSingleListWidget):
    def __init__(self, data, title, dlgate, parent=None):
        QWidget.__init__(self, parent)
        self.initiate(data, parent)
        self.title = title

        self.model = SingleListModel(self.current, title, self)
        self.view = RightClickTreeView(parent=self)
        self.view.setModel(self.model)
        delegate = dlgate(self.view)
        self.view.setItemDelegate(delegate)
        self.view.resizeColumnToContents(0)
        self.view.setColumnWidth(0, max(MIN_NID_COLUMN_WIDTH, self.view.columnWidth(0)))

        self.placement(data, title)

        add_button = QPushButton("+")
        add_button.setMaximumWidth(30)
        add_button.clicked.connect(self.model.append)
        self.layout.addWidget(add_button, 0, 1, alignment=Qt.AlignRight)

class BasicMultiListWidget(BasicSingleListWidget):
    def __init__(self, data, title, attrs, dlgate, parent=None, model=DefaultMultiAttrListModel):
        QWidget.__init__(self, parent)
        self.initiate(data, parent)
        self.title = title

        self.model = model(self.current, attrs, parent=self)
        self.view = QTreeView(self)
        self.view.setModel(self.model)
        delegate = dlgate(self.view)
        self.view.setItemDelegate(delegate)
        for col in range(len(attrs)):
            self.view.resizeColumnToContents(col)
            self.view.setColumnWidth(0, max(MIN_NID_COLUMN_WIDTH, self.view.columnWidth(0)))

        self.placement(data, title)

class MultiDictWidget(BasicSingleListWidget):
    def __init__(self, data, title, attrs, dlgate, parent=None, model=DefaultMultiAttrListModel):
        QWidget.__init__(self, parent)
        self.initiate(data, parent)
        self.title = title

        self.model = model(self.current, attrs, parent=self)
        self.view = QTreeView(self)
        self.view.setModel(self.model)
        delegate = dlgate(self.view)
        self.view.setItemDelegate(delegate)
        for col in range(len(attrs)):
            self.view.resizeColumnToContents(col)
            self.view.setColumnWidth(0, max(MIN_NID_COLUMN_WIDTH, self.view.columnWidth(0)))

        self.placement(data, title)

class AppendMultiListWidget(BasicSingleListWidget):
    def __init__(self, data, title, attrs, dlgate, parent=None, model=DefaultMultiAttrListModel):
        QWidget.__init__(self, parent)
        self.initiate(data, parent)
        self.title = title

        self.model = model(self.current, attrs, parent=self)

        def duplicate_func(model, index):
            return False

        action_funcs = (None, duplicate_func, duplicate_func)

        self.view = RightClickTreeView(action_funcs, self)
        self.view.setModel(self.model)
        delegate = dlgate(self.view)
        self.view.setItemDelegate(delegate)

        self.placement(data, title)

        add_button = QPushButton("+")
        add_button.setMaximumWidth(30)
        add_button.clicked.connect(self.model.append)
        self.layout.addWidget(add_button, 0, 1, alignment=Qt.AlignRight)

class MutableAppendMultiListWidget(AppendMultiListWidget):
    def __init__(self, data, title, attrs, dlgate, parent=None, model=DefaultMultiAttrListModel,
                 rename_column_action: Callable[[QtGui.QContextMenuEvent], None] = None,
                 delete_column_action: Callable[[QtGui.QContextMenuEvent], None] = None):
        super().__init__(data, title, attrs, dlgate, parent=parent, model=model)
        self.rename_column_action = rename_column_action
        self.delete_column_action = delete_column_action

    def rename_slot(self, col_num):
        if self.rename_column_action:
            self.rename_column_action(col_num)

    def delete_slot(self, col_num):
        if self.delete_column_action:
            self.delete_column_action(col_num)

    def contextMenuEvent(self, event: QtGui.QContextMenuEvent) -> None:
        self.menu = QMenu(self)
        col_clicked = self.view.columnAt(event.pos().x())

        rename_action = QAction('Rename', self)
        rename_action.triggered.connect(lambda: self.rename_slot(col_clicked))
        self.menu.addAction(rename_action)

        delete_action = QAction('Delete', self)
        delete_action.triggered.connect(lambda: self.delete_slot(col_clicked))
        self.menu.addAction(delete_action)

        self.menu.popup(QtGui.QCursor.pos())