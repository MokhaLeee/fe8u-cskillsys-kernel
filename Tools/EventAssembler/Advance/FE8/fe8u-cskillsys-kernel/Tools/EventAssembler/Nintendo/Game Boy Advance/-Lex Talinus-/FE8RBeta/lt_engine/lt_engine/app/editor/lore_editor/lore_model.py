from PyQt5.QtCore import Qt

from app.data.database.database import DB
from app.editor.base_database_gui import DragDropCollectionModel

class LoreModel(DragDropCollectionModel):
    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            lore = self._data[index.row()]
            text = lore.nid + ': ' + lore.category
            return text
        return None

    def create_new(self):
        new_lore = DB.lore.create_new(DB)
        return new_lore

    def on_nid_changed(self, old_value, new_value):
        pass
