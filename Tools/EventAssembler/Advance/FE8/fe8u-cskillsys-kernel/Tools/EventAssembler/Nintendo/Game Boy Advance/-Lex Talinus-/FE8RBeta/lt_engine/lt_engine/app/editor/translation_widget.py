from app.data.database.database import DB

from app.extensions.list_dialogs import MultiAttrListDialog
from app.extensions.list_models import MultiAttrListModel

class TranslationMultiModel(MultiAttrListModel):
    def create_new(self):
        return self._data.add_new_default(DB)

    def on_attr_changed(self, data, attr, old_value, new_value):
        if attr == 'nid':
            self._data.update_nid(data, new_value)

class TranslationDialog(MultiAttrListDialog):
    @classmethod
    def create(cls):
        dlg = cls(DB.translations, "Translation", ("nid", "text"),
                  TranslationMultiModel, (None, None, None), set())
        return dlg
