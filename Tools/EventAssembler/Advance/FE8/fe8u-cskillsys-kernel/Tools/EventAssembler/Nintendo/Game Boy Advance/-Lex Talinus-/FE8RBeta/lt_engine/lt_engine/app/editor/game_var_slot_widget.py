from app.data.database.varslot import VarSlot, VarSlotCatalog
from app.data.database.database import DB

from app.extensions.list_dialogs import MultiAttrListDialog
from app.extensions.list_models import MultiAttrListModel

class GameVarSlotMultiModel(MultiAttrListModel):
    locked_columns = [2]

    def delete(self, idx):
        DB.game_var_slots.remove_key(self._data[idx].nid)
        super().delete(idx)

    def create_new(self):
        new_slot = self._data.add_new_default(DB)
        DB.game_var_slots.append(VarSlot(new_slot.nid, new_slot.desc))
        new_slot.occurrences = '[]'
        return new_slot

    def duplicate(self, idx):
        idx = super().duplicate(idx)
        copy = self._data[idx.row()]
        copy.occurrences = '[]'
        return idx

    def on_attr_changed(self, data, attr, old_value, new_value):
        if attr == 'nid':
            original = DB.game_var_slots.get(data.nid)
            DB.game_var_slots.update_nid(original, new_value)
            original = self._data.get(data.nid)
            self._data.update_nid(original, new_value)
        elif attr == 'desc':
            DB.game_var_slots.get(data.nid).desc = new_value
            self._data.get(data.nid).desc = new_value

class GameVarSlotDialog(MultiAttrListDialog):
    @classmethod
    def create(cls, parent=None):
        def deletion_func(model, index):
            return True
        # copying because we're augmenting the data
        copy = VarSlotCatalog()
        for var in DB.game_var_slots:
            copied_slot = VarSlot(var.nid, var.desc)
            occurrences = DB.events.inspector.find_all_occurrences_of_symbol(var.nid)
            copied_slot.occurrences = '[' + '], ['.join(list(occurrences)) + ']'
            copy.append(copied_slot)
        dlg = cls(copy, "Game Var Slots", ("nid", "desc", "occurrences"), GameVarSlotMultiModel, (deletion_func, None, deletion_func), [], parent)
        return dlg

# Testing
# Run "python -m app.editor.tag_widget"
if __name__ == '__main__':
    import sys
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    DB.load('default.ltproj')
    window = GameVarSlotDialog.create()
    window.show()
    app.exec_()
