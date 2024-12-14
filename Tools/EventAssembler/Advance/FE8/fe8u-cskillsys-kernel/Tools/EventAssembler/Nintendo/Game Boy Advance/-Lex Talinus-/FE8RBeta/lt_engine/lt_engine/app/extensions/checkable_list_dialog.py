# from PyQt5.QtWidgets import QDialog, QTreeView, QDialogButtonBox, QVBoxLayout
from PyQt5.QtCore import Qt, QAbstractItemModel, QModelIndex

# from app.engine import item_component

# class CheckableListDialog(QDialog):
#     def __init__(self, data, title, parent=None):
#         super().__init__(parent)

#         self.window = parent
#         self._data = data
#         self.current_components = self.window.current.components

#         self.setWindowTitle(title)
#         self.setWindowFlag(Qt.WindowContextHelpButtonHint, False)

#         self.model = ComponentModel(data, self.current_components, self)

#         self.view = QTreeView()
#         self.view.setModel(self.model)
#         self.view.header().hide()
#         self.view.clicked.connect(self.on_click)

#         layout = QVBoxLayout(self)
#         layout.addWidget(self.view)

#         self.buttonbox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel, Qt.Horizontal, self)
#         layout.addWidget(self.buttonbox)
#         self.buttonbox.accepted.connect(self.accept)
#         self.buttonbox.rejected.connect(self.reject)

#     def get_checked(self):
#         components = item_component.get_item_components()
#         # remove components that were already present
#         checked = self.model.checked - set(self.model.already_present.keys())
#         # sort based off position in item_components
#         sorted_components = sorted(checked, key=lambda x: [c.nid for c in components].index(x))
#         return sorted_components

#     def on_click(self, index):
#         # Only if clickable
#         if bool(self.model.flags(index) & Qt.ItemIsEnabled):
#             nid_clicked = self.model._data[index.row()].nid
#             if nid_clicked in self.model.checked:
#                 self.model.checked.discard(nid_clicked)
#             else:
#                 self.model.checked.add(nid_clicked)
#             self.model.dataChanged.emit(index, index)

class ComponentModel(QAbstractItemModel):
    def __init__(self, data, already_present, parent=None):
        super().__init__(parent)
        self.window = parent
        self._data = data
        self.already_present = already_present
        self.checked = set(self.already_present.keys())

    def headerData(self, idx, orientation, role=Qt.DisplayRole):
        return None

    def index(self, row, column, parent_index=QModelIndex()):
        if self.hasIndex(row, column, parent_index):
            return self.createIndex(row, column)
        return QModelIndex()

    def parent(self, index):
        return QModelIndex()

    def rowCount(self, parent=None):
        return len(self._data)

    def columnCount(self, parent=None):
        return 1

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.EditRole:
            return self._data[index.row()]
        data = self._data[index.row()]
        if role == Qt.DisplayRole:
            return data.class_name()
        elif role == Qt.CheckStateRole:
            value = Qt.Checked if data.nid in self.checked else Qt.Unchecked
            return value
        return None

    def setData(self, index, value, role):
        if not index.isValid():
            return False
        if role == Qt.CheckStateRole:
            data = self._data[index.row()]
            if value == Qt.Checked:
                self.checked.add(data.nid)
            else:
                self.checked.discard(data.nid)
            self.dataChanged.emit(index, index)
        return True

    def flags(self, index):
        basic_flags = Qt.ItemNeverHasChildren
        data = self._data[index.row()]
        true_components = set(self.already_present.keys()) | self.checked
        if data.nid in self.already_present.keys():
            pass
        elif not data.requires or all(r in true_components for r in data.requires):
            basic_flags |= Qt.ItemIsEnabled | Qt.ItemIsSelectable
            # basic_flags |= Qt.ItemIsUserCheckable
        return basic_flags
