from app.data.database.database import DB
from app.editor.base_database_gui import DatabaseTab
from app.editor.data_editor import SingleDatabaseEditor
from app.editor.raw_editor import raw_data_model, raw_data_properties


class RawDataDatabase(DatabaseTab):
    @classmethod
    def create(cls, parent=None):
        data = DB.raw_data
        title: str = 'Raw Data'
        right_frame = raw_data_properties.RawDataProperties

        collection_model = raw_data_model.RawDataModel
        dialog = cls(data, title, right_frame, (None, None, None), collection_model, parent)
        return dialog

# Testing
# Run "python -m app.editor.raw_editor.raw_data_tab" from main directory
if __name__ == '__main__':
    import sys

    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    from app.data.resources.resources import RESOURCES
    RESOURCES.load('default.ltproj')
    DB.load('default.ltproj')
    window = SingleDatabaseEditor(RawDataDatabase)
    window.show()
    app.exec_()
