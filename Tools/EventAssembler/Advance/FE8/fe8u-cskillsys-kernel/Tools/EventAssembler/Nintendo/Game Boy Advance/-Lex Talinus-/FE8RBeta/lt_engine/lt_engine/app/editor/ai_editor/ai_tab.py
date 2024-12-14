from app.data.database.database import DB

from app.editor.base_database_gui import DatabaseTab
from app.editor.data_editor import SingleDatabaseEditor

from app.editor.ai_editor import ai_model, ai_properties

class AIDatabase(DatabaseTab):
    allow_copy_and_paste = True

    @classmethod
    def create(cls, parent=None):
        data = DB.ai
        title = "AI"
        right_frame = ai_properties.AIProperties

        def deletion_func(model, index):
            return model._data[index.row()].nid != "None"

        collection_model = ai_model.AIModel
        dialog = cls(data, title, right_frame, (deletion_func, None, None), collection_model, parent)
        return dialog

# Testing
# Run "python -m app.editor.ai_editor.ai_tab" from main directory
if __name__ == '__main__':
    import sys
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    from app.data.resources.resources import RESOURCES
    RESOURCES.load('default.ltproj')
    DB.load('default.ltproj')
    window = SingleDatabaseEditor(AIDatabase)
    window.show()
    app.exec_()
