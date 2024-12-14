from PyQt5.QtGui import QPixmap, QIcon
from PyQt5.QtCore import Qt

from app.data.resources.resources import RESOURCES
from app.data.database.database import DB
from app.data.database.level_units import UniqueUnit

from app.extensions.custom_gui import DeletionTab, DeletionDialog

from app.editor.base_database_gui import DragDropCollectionModel
import app.editor.utilities as editor_utilities

from app.utilities.typing import NID

def get_chibi(unit):
    res = RESOURCES.portraits.get(unit.portrait_nid)
    if not res:
        return None
    if not res.pixmap:
        res.pixmap = QPixmap(res.full_path)
    pixmap = res.pixmap.copy(res.pixmap.width() - 32, 16, 32, 32)
    pixmap = QPixmap.fromImage(editor_utilities.convert_colorkey(pixmap.toImage()))
    return pixmap

def check_delete(nid: NID, window) -> bool:
    affected_ais = [ai for ai in DB.ai if ai.has_unit_spec("ID", nid)]
    affected_levels = [level for level in DB.levels if any(isinstance(unit, UniqueUnit) and unit.nid == nid for unit in level.units)]

    deletion_tabs = []
    if affected_ais:
        from app.editor.ai_editor.ai_model import AIModel
        model = AIModel
        msg = "Deleting Unit <b>%s</b> would affect these AIs" % nid
        deletion_tabs.append(DeletionTab(affected_ais, model, msg, "AIs"))
    if affected_levels:
        from app.editor.global_editor.level_menu import LevelModel
        model = LevelModel
        msg = "Deleting Unit <b>%s</b> would affect these levels" % nid
        deletion_tabs.append(DeletionTab(affected_levels, model, msg, "Levels"))

    if deletion_tabs:
        ok = DeletionDialog.inform(deletion_tabs, window)
        return ok
    return True

def on_delete(old_nid: NID):
    new_nid = None
    for unit in DB.units:
        if unit.nid != old_nid:
            new_nid = unit.nid
            break
    for ai in DB.ai:
        ai.change_unit_spec("ID", old_nid, new_nid)
    for level in DB.levels:
        if old_nid in level.units.keys():
            level.units.remove_key(old_nid)
        for unit_group in level.unit_groups:
            unit_group.remove(old_nid)
        for unit in level.units:
            if unit.starting_traveler == old_nid:
                unit.starting_traveler = None
    for party in DB.parties:
        if party.leader == old_nid:
            party.leader = new_nid

def on_nid_changed(old_nid: NID, new_nid: NID):
    for ai in DB.ai:
        ai.change_unit_spec("ID", old_nid, new_nid)
    for level in DB.levels:
        for unit in level.units:
            if isinstance(unit, UniqueUnit) and unit.nid == old_nid:
                level.units.update_nid(unit, new_nid)
        for unit_group in level.unit_groups:
            unit_group.swap(old_nid, new_nid)
        for unit in level.units:
            if old_nid and unit.starting_traveler == old_nid:
                unit.starting_traveler = new_nid
    for party in DB.parties:
        if party.leader == old_nid:
            party.leader = new_nid

class UnitModel(DragDropCollectionModel):
    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            unit = self._data[index.row()]
            if not unit:
                return "None"
            text = unit.nid
            return text
        elif role == Qt.DecorationRole:
            unit = self._data[index.row()]
            if not unit:
                return None
            # Get chibi image
            pixmap = get_chibi(unit)
            if pixmap:
                return QIcon(pixmap)
            else:
                return None
        return None

    def delete(self, idx):
        # check to make sure nothing else is using me!!!
        unit = self._data[idx]
        nid = unit.nid
        ok = check_delete(nid)
        if ok:
            on_delete(nid)
        else:
            return
        super().delete(idx)

    def on_delete(self, old_nid):
        on_delete(old_nid)

    def on_nid_changed(self, old_nid, new_nid):
        on_nid_changed(old_nid, new_nid)

    def create_new(self):
        new_unit = self._data.create_new(DB)
        return new_unit
