from PyQt5.QtWidgets import QListView
from PyQt5.QtCore import Qt

class DeleteListView(QListView):
    def delete(self, index):
        self.model().delete(index.row())

    def keyPressEvent(self, event):
        super().keyPressEvent(event)
        if event.key() == Qt.Key_Delete:
            indices = self.selectionModel().selectedIndexes()
            for index in indices:
                self.delete(index)
