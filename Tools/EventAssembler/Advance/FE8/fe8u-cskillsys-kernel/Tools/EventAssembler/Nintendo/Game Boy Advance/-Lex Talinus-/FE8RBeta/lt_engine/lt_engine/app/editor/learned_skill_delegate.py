from PyQt5.QtWidgets import QSpinBox, QItemDelegate
from PyQt5.QtGui import QIcon

from app.data.database.database import DB

from app.extensions.custom_gui import ComboBox
from app.editor.skill_editor import skill_model

class LearnedSkillDelegate(QItemDelegate):
    int_column = 0
    skill_column = 1

    def createEditor(self, parent, option, index):
        if index.column() == self.int_column:
            editor = QSpinBox(parent)
            editor.setRange(1, 255)
            return editor
        elif index.column() == self.skill_column:
            editor = ComboBox(parent)
            # editor.addItem("None")
            for skill in DB.skills:
                pixmap = skill_model.get_pixmap(skill)
                if pixmap:
                    editor.addItem(QIcon(pixmap), skill.nid)
                else:
                    editor.addItem(skill.nid)
            return editor
        else:
            return super().createEditor(parent, option, index)
