from PyQt5.QtWidgets import QPushButton, QDialog, QDialogButtonBox, QGridLayout
from PyQt5.QtCore import Qt

from app.extensions.custom_gui import RightClickListView, RightClickTreeView, IntDelegate, RightClickTableView
from app.extensions.list_models import SingleListModel, DefaultMultiAttrListModel

# === LIST DIALOGS ===========================================================
class SingleListDialog(QDialog):
    def __init__(self, data, title, parent=None):
        super().__init__(parent)
        self.initiate(data, title, parent)

        self.model = SingleListModel(self._data, title, self)
        self.view = RightClickListView(parent=self)
        self.view.setModel(self.model)

        self.placement(title)

    def initiate(self, data, title, parent):
        self.window = parent
        self._data = data
        self.title = title

        self.setWindowTitle("%s Editor" % title)
        self.setStyleSheet("font: 10pt;")
        self.setWindowFlag(Qt.WindowContextHelpButtonHint, False)

    def placement(self, title):
        layout = QGridLayout(self)
        layout.addWidget(self.view, 0, 0, 1, 2)
        self.setLayout(layout)

        self.add_button = QPushButton("Add %s" % title)
        self.add_button.clicked.connect(self.model.append)
        layout.addWidget(self.add_button, 1, 0, alignment=Qt.AlignLeft)

        self.buttonbox = QDialogButtonBox(QDialogButtonBox.Ok, Qt.Horizontal, self)
        layout.addWidget(self.buttonbox, 1, 1)
        self.buttonbox.accepted.connect(self.accept)

    def accept(self):
        super().accept()

class MultiAttrListDialog(SingleListDialog):
    def __init__(self, data, title, attrs, model=DefaultMultiAttrListModel, deletion_criteria=None, locked: set = None, parent=None):
        QDialog.__init__(self, parent)
        self.initiate(data, title, parent)

        self.model = model(self._data, attrs, self)
        if locked:
            self.model.edit_locked = locked
        self.view = RightClickTreeView(deletion_criteria, self)
        self.view.setModel(self.model)
        self.view.setAutoScroll(False)
        int_columns = [i for i, attr in enumerate(attrs) if len(self._data) and type(getattr(self._data[0], attr)) == int]
        delegate = IntDelegate(self.view, int_columns)
        self.view.setItemDelegate(delegate)
        for col in range(len(attrs)):
            self.view.resizeColumnToContents(col)
        # self.view.resize(600, 400)
        self.resize(600, 400)

        self.placement(title)
