from PyQt5.QtWidgets import QWidget, QGridLayout, QLabel, QHBoxLayout, QPushButton
from PyQt5.QtCore import Qt, pyqtSignal

from app.data.database.database import DB

from app.extensions.custom_gui import QHLine
from app.editor.multi_combo_box_list import MultiComboBoxList
from app.editor.skill_editor.skill_model import get_pixmap

class SkillListWidget(QWidget):
    skills_updated = pyqtSignal()

    def __init__(self, title, parent=None):
        super().__init__(parent)
        self.window = parent

        self.skill_list = MultiComboBoxList(DB.skills, get_pixmap, self)
        self.skill_list.item_changed.connect(self.activate)

        self.layout = QGridLayout(self)
        self.layout.setSpacing(0)
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.addWidget(self.skill_list, 3, 0, 1, 2)
        self.setLayout(self.layout)

        label = QLabel(title)
        label.setAlignment(Qt.AlignBottom)
        self.layout.addWidget(label, 0, 0)

        header1 = QLabel("Skill ID")
        header1.setAlignment(Qt.AlignBottom | Qt.AlignLeft)
        self.layout.addWidget(header1, 2, 0)

        hline = QHLine()
        self.layout.addWidget(hline, 1, 0, 1, 2)

        hbox = QHBoxLayout()
        hbox.setSpacing(0)
        hbox.setContentsMargins(0, 0, 0, 0)

        add_button = QPushButton("+")
        add_button.setMaximumWidth(30)
        add_button.clicked.connect(self.add_new_skill)

        remove_button = QPushButton("-")
        remove_button.setMaximumWidth(30)
        remove_button.clicked.connect(self.remove_last_item)

        hbox.addWidget(remove_button, alignment=Qt.AlignRight)
        hbox.addWidget(add_button, alignment=Qt.AlignRight)

        self.layout.addLayout(hbox, 0, 1, alignment=Qt.AlignRight)

    def set_current(self, skills):
        self.skill_list.set_current(skills)

    def add_new_skill(self):
        if DB.skills:
            new_skill = DB.skills[0].nid
            self.skill_list.add_item(new_skill)
        self.activate()

    def remove_last_item(self):
        if self.skill_list.length() > 0:
            self.skill_list.remove_item_at_index(self.skill_list.length() - 1)
        self.activate()

    def activate(self):
        self.skills_updated.emit()

    def get_skills(self):
        return self.skill_list.index_list[:]

    def set_color(self, color_list):
        self.skill_list.set_color(color_list)
