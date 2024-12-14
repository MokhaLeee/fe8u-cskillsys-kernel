from PyQt5.QtCore import Qt

from app.data.database.database import DB
from app.editor.base_database_gui import DragDropCollectionModel

class StatTypeModel(DragDropCollectionModel):
    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            stat_type = self._data[index.row()]
            text = stat_type.nid + ": " + stat_type.name
            return text
        return None

    def create_new(self):
        new_stat = self._data.create_new(DB)
        return new_stat

    def delete(self, idx):
        # check to make sure nothing else is using me!!!
        stat = self._data[idx]
        nid = stat.nid
        for klass in DB.classes:
            for stat_list in klass.get_stat_lists():
                if nid in stat_list:
                    stat_list.pop(nid)
        for unit in DB.units:
            for stat_list in unit.get_stat_lists():
                if nid in stat_list:
                    stat_list.pop(nid)
        # Delete watchers
        super().delete(idx)
