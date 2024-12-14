from PyQt5.QtWidgets import QSpinBox, QItemDelegate

from app.extensions.custom_gui import ComboBox

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
            editor.addItem("None")
            # for status_effect in DB.status_effects:
                # editor.addItem(status_effect.nid)
            return editor
        else:
            return super().createEditor(parent, option, index)
