from app.data.database import raw_data
from app.data.database.database import DB
from app.editor.base_database_gui import DragDropCollectionModel
from app.utilities import str_utils
from PyQt5.QtCore import Qt


class RawDataModel(DragDropCollectionModel):
    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            rdata = self._data[index.row()]
            text = rdata.nid + ": (" + rdata.dtype + ")"
            return text
        return None

    def create_new(self):
        nids = [d.nid for d in self._data]
        nid = str_utils.get_next_generic_nid("New Raw", nids)
        new_data = raw_data.RawDataPrefab(nid, "str")
        DB.raw_data.append(new_data)
        return new_data

    def on_nid_changed(self, old_value, new_value):
        pass
