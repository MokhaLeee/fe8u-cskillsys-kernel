from PyQt5.QtCore import Qt

from app.data.database.database import DB

from app.extensions.custom_gui import DeletionTab, DeletionDialog
from app.editor.custom_widgets import AIBox
from app.editor.base_database_gui import DragDropCollectionModel

class AIModel(DragDropCollectionModel):
    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            ai = self._data[index.row()]
            text = ai.nid
            return text
        return None

    def delete(self, idx):
        # check to make sure nothing else is using me!!!
        ai = self._data[idx]
        nid = ai.nid
        affected_levels = [level for level in DB.levels if any(unit.ai == nid for unit in level.units)]
        if affected_levels:
            from app.editor.global_editor.level_menu import LevelModel
            model = LevelModel
            msg = "Deleting AI <b>%s</b> would affect units in these levels" % nid
            deletion_tab = DeletionTab(affected_levels, model, msg, "Levels")
            swap, ok = DeletionDialog.get_swap([deletion_tab], AIBox(self.window, exclude=ai), self.window)
            if ok:
                self.on_nid_changed(nid, swap.nid)
            else:
                return
        # Delete watchers
        super().delete(idx)

    def on_nid_changed(self, old_nid, new_nid):
        for level in DB.levels:
            for unit in level.units:
                if unit.ai == old_nid:
                    unit.ai = new_nid

    def create_new(self):
        new_ai = self._data.create_new(DB)
        return new_ai
