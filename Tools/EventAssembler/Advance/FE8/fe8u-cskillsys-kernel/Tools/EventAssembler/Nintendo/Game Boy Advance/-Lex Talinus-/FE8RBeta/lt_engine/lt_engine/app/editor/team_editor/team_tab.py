from app.data.database.database import DB

from app.editor.base_database_gui import DatabaseTab
from app.editor.data_editor import SingleDatabaseEditor

from app.editor.team_editor import team_model, team_properties

class TeamDatabase(DatabaseTab):
    @classmethod
    def create(cls, parent=None):
        data = DB.teams
        title: str = 'Team'
        right_frame = team_properties.TeamProperties

        def deletion_func(model, index):
            # Cannot delete the player team
            return model._data[index.row()].nid != 'player'

        collection_model = team_model.TeamModel
        dialog = cls(data, title, right_frame, (deletion_func, None, deletion_func), collection_model, parent)
        return dialog

# Testing
# Run "python -m app.editor.team_editor.team_tab" from main directory
if __name__ == '__main__':
    import sys
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    from app.data.resources.resources import RESOURCES
    RESOURCES.load('default.ltproj')
    DB.load('default.ltproj')
    window = SingleDatabaseEditor(TeamDatabase)
    window.show()
    app.exec_()
