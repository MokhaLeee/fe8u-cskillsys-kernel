from app.data.database.database import DB

from app.editor.base_database_gui import DatabaseTab
from app.editor.data_editor import SingleDatabaseEditor

from app.editor.lore_editor import lore_model, lore_properties

class LoreDatabase(DatabaseTab):
    @classmethod
    def create(cls, parent=None):
        data = DB.lore
        title: str = 'lore'
        right_frame = lore_properties.LoreProperties

        collection_model = lore_model.LoreModel
        dialog = cls(data, title, right_frame, (None, None, None), collection_model, parent)
        return dialog

# Testing
# Run "python -m app.editor.lore_editor.lore_tab" from main directory
if __name__ == '__main__':
    import sys
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    from app.data.resources.resources import RESOURCES
    RESOURCES.load('default.ltproj')
    DB.load('default.ltproj')
    window = SingleDatabaseEditor(LoreDatabase)
    window.show()
    app.exec_()
