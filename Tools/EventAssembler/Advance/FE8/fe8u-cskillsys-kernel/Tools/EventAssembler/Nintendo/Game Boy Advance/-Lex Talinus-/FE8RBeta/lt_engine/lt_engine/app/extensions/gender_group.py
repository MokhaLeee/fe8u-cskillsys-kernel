from PyQt5.QtWidgets import QWidget, QHBoxLayout, QRadioButton
from PyQt5.QtCore import pyqtSignal

class GenderGroup(QWidget):
    toggled = pyqtSignal(bool)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        self.male = QRadioButton('M', self)
        self.male.toggled.connect(self.male_toggled)
        self.female = QRadioButton('F', self)

        layout = QHBoxLayout()
        self.setLayout(layout)
        layout.setSpacing(0)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.male)
        layout.addWidget(self.female)

    def male_toggled(self, checked):
        self.toggled.emit(checked)

    def setValue(self, gender):
        if gender < 5:
            self.male.setChecked(True)
        else:
            self.female.setChecked(True)
