from app.data.database.database import DB

from app.editor.base_database_gui import DatabaseTab
from app.editor.data_editor import SingleDatabaseEditor

from app.editor.stat_editor import stat_model, stat_properties

class StatTypeDatabase(DatabaseTab):
    @classmethod
    def create(cls, parent=None):
        data = DB.stats
        title: str = "Stat Types"
        right_frame = stat_properties.StatTypeProperties

        collection_model = stat_model.StatTypeModel
        return cls(data, title, right_frame, None, collection_model, parent)

# Testing
# Run "python -m app.editor.stat_editor.stat_tab" from main directory
if __name__ == '__main__':
    import sys
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    from app.data.resources.resources import RESOURCES
    RESOURCES.load('default.ltproj')
    DB.load('default.ltproj')
    window = SingleDatabaseEditor(StatTypeDatabase)
    window.show()
    app.exec_()
