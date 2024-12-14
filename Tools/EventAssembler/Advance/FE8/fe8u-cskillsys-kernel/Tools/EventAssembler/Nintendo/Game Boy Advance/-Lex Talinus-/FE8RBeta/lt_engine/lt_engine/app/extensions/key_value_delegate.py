from PyQt5.QtWidgets import QLineEdit, QItemDelegate

from app.extensions.list_models import DoubleListModel


class FixedKeyMutableValueDelegate(QItemDelegate):
    key_column = 0
    value_column = 1

    def createEditor(self, parent, option, index):
        if index.column() == self.key_column:
            editor = QLineEdit(parent)
            editor.setDisabled(True)
            return editor
        elif index.column() == self.value_column:
            editor = QLineEdit(parent)
            return editor
        else:
            return super().createEditor(parent, option, index)
class KeyValueDelegate(QItemDelegate):
    key_column = 0
    value_column = 1

    def createEditor(self, parent, option, index):
        if index.column() == self.key_column:
            editor = QLineEdit(parent)
            return editor
        elif index.column() == self.value_column:
            editor = QLineEdit(parent)
            return editor
        else:
            return super().createEditor(parent, option, index)

class KeyValueDoubleListModel(DoubleListModel):
    """
    Handles a simple list of 2-tuples/lists where
    both values are strings that can be edited
    """
    def create_new(self):
        new_key = "New Key"
        new_value = "New Value"
        self._data.append([new_key, new_value])
