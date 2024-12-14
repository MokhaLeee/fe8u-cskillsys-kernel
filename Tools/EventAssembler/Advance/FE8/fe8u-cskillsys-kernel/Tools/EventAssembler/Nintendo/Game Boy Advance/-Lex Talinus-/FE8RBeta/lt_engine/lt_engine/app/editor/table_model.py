import copy

from PyQt5.QtCore import Qt, QAbstractTableModel
from PyQt5.QtCore import QSortFilterProxyModel

from app.utilities.data import Prefab
from app.utilities import str_utils

class TableModel(QAbstractTableModel):
    rows = ['nid']

    def __init__(self, data, parent):
        super().__init__(parent)
        self.window = parent
        self._data = data

    def rowCount(self, parent=None):
        return len(self._data)

    def columnCount(self, parent=None):
        return len(self.rows)

    def on_attr_changed(self, data, attr, old_value, new_value):
        if attr == 'nid':
            self._data.update_nid(data, new_value)

    def setData(self, index, value, role):
        if not index.isValid():
            return False
        d = self._data[index.row()]
        attr = self.rows[index.column()]
        current_value = getattr(d, attr)
        self.on_attr_changed(d, attr, current_value, value)
        setattr(d, attr, value)
        self.dataChanged.emit(index, index)
        return True

    def flags(self, index):
        return Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable | Qt.ItemNeverHasChildren

    def delete(self, index):
        if not index.isValid():
            return False
        idx = index.row()
        self.layoutAboutToBeChanged.emit()
        self._data.pop(idx)
        self.layoutChanged.emit()

    def update(self):
        self.layoutChanged.emit()

    def append(self):
        self.layoutAboutToBeChanged.emit()
        if self.create_new():
            self.layoutChanged.emit()
            last_index = self.index(self.rowCount() - 1, 0)
            return last_index
        else:
            self.layoutChanged.emit()
        return None

    def new(self, index):
        if not index.isValid():
            return False
        if self.create_new():
            idx = index.row()
            self.layoutAboutToBeChanged.emit()
            self._data.move_index(len(self._data) - 1, idx + 1)
            self.layoutChanged.emit()
            new_index = self.index(idx + 1, 0)
            return new_index
        return None

    def duplicate(self, index):
        if not index.isValid():
            return False
        idx = index.row()
        obj = self._data[idx]
        new_nid = str_utils.get_next_name(obj.nid, self._data.keys())
        if isinstance(obj, Prefab):
            serialized_obj = obj.save()
            new_obj = self._data.datatype.restore(serialized_obj)
        else:
            new_obj = copy.copy(obj)
        new_obj.nid = new_nid
        self.layoutAboutToBeChanged.emit()
        self._data.insert(idx + 1, new_obj)
        self.layoutChanged.emit()
        new_index = self.index(idx + 1, 0)
        return new_index

    def modify(self, indices):
        raise NotImplementedError

    def create_new(self):
        raise NotImplementedError

class ProxyModel(QSortFilterProxyModel):
    def delete(self, index):
        index = self.mapToSource(index)
        self.layoutAboutToBeChanged.emit()
        self.sourceModel().delete(index)
        self.layoutChanged.emit()

    def modify(self, indices):
        indices = [self.mapToSource(index) for index in indices]
        self.sourceModel().modify(indices)

    def new(self, index):
        index = self.mapToSource(index)
        self.layoutAboutToBeChanged.emit()
        new_index = self.sourceModel().new(index)
        self.layoutChanged.emit()
        if new_index:
            return self.mapFromSource(new_index)
        return None

    def duplicate(self, index):
        index = self.mapToSource(index)
        self.layoutAboutToBeChanged.emit()
        new_index = self.sourceModel().duplicate(index)
        self.layoutChanged.emit()
        if new_index:
            new_index = self.mapFromSource(new_index)
            return new_index
        return None
