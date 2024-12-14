from PyQt5.QtWidgets import QVBoxLayout, QDialog, QLineEdit, QDialogButtonBox
from app.extensions.custom_gui import PropertyBox, Dialog

class NewGameDialog(Dialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("New Game")
        self.window = parent

        layout = QVBoxLayout()
        self.setLayout(layout)

        self.game_box = PropertyBox('Unique Game ID', QLineEdit, self)
        self.game_box.edit.setPlaceholderText("Unique Game ID")
        self.game_box.edit.textChanged.connect(self.text_changed)
        layout.addWidget(self.game_box)

        self.game_title_box = PropertyBox("Game Title", QLineEdit, self)
        self.game_title_box.edit.setPlaceholderText("Game Title")
        self.game_title_box.edit.textChanged.connect(self.text_changed)
        layout.addWidget(self.game_title_box)

        layout.addWidget(self.buttonbox)
        self.buttonbox.button(QDialogButtonBox.Ok).setEnabled(False)

    def text_changed(self, text):
        if (self.game_title_box.edit.text() or self.game_box.edit.text()):
            self.buttonbox.button(QDialogButtonBox.Ok).setEnabled(True)
        else:
            self.buttonbox.button(QDialogButtonBox.Ok).setEnabled(False)

    @classmethod
    def get(cls, parent=None):
        dialog = cls(parent)
        result = dialog.exec_()
        if result == QDialog.Accepted:
            return (dialog.game_box.edit.text(), dialog.game_title_box.edit.text())
        else:
            return None
