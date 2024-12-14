from PyQt5.QtCore import Qt

from app.utilities.data import Data
from app.data.database.database import DB

from app.extensions.custom_gui import DeletionTab, DeletionDialog

from app.editor.custom_widgets import TeamBox
from app.editor.base_database_gui import DragDropCollectionModel

class TeamModel(DragDropCollectionModel):
    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            team = self._data[index.row()]
            text = team.nid
            return text
        return None

    def create_new(self):
        new_team = DB.teams.create_new(DB)
        return new_team

    def delete(self, idx):
        team = self._data[idx]
        nid = team.nid
        affected_ais = [ai for ai in DB.ai if ai.has_unit_spec("Team", nid)]
        affected_levels = [level for level in DB.levels if any(unit.team == nid for unit in level.units)]

        deletion_tabs = []
        if affected_ais:
            from app.editor.ai_editor.ai_model import AIModel
            model = AIModel
            msg = "Deleting Team <b>%s</b> would affect these objects" % nid
            deletion_tabs.append(DeletionTab(affected_ais, model, msg, "AIs"))
        if affected_levels:
            from app.editor.global_editor.level_menu import LevelModel
            model = LevelModel
            msg = "Deleting Team <b>%s</b> would affect units in these levels" % nid
            deletion_tabs.append(DeletionTab(affected_levels, model, msg, "Levels"))

        if deletion_tabs:
            swap, ok = DeletionDialog.get_swap(deletion_tabs, TeamBox(self.window, exclude=team), self.window)
            if ok:
                self.on_nid_changed(nid, swap.nid)
            else:
                return
        super().delete(idx)

    def on_nid_changed(self, old_nid, new_nid):
        for ai in DB.ai:
            ai.change_unit_spec("Team", old_nid, new_nid)
        for level in DB.levels:
            for unit in level.units:
                if unit.team == old_nid:
                    unit.team = new_nid
