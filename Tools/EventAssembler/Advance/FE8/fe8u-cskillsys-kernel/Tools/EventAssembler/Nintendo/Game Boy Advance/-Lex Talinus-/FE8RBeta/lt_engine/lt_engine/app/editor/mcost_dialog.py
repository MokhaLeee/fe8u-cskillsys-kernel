from PyQt5.QtWidgets import QDialogButtonBox, QTableView, QInputDialog, QHeaderView, \
    QGridLayout, QPushButton, QLineEdit, QStyledItemDelegate, QAction, QMenu, QMessageBox, \
    QDialog, QApplication
from PyQt5.QtGui import QDoubleValidator, QFontMetrics, QBrush, QColor, QPalette
from PyQt5.QtWidgets import QStyle, QProxyStyle
from PyQt5.QtCore import QAbstractTableModel
from PyQt5.QtCore import Qt, QSize

from app.utilities.data import Data
from app.data.database.database import DB
from app.utilities import str_utils

from app.extensions.custom_gui import SimpleDialog, DeletionTab, DeletionDialog
from app.editor.custom_widgets import MovementCostBox, MovementClassBox
from app.editor.base_database_gui import CollectionModel

class McostDialog(SimpleDialog):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.setWindowTitle('Terrain Movement Cost')
        self.setMinimumSize(640, 480)

        self.model = GridModel(DB.mcost, self)
        self.view = QTableView()
        self.view.setModel(self.model)
        delegate = McostDelegate(self.view)
        self.view.setItemDelegate(delegate)

        layout = QGridLayout(self)
        layout.addWidget(self.view, 0, 0, 1, 2)
        self.setLayout(layout)

        column_header_view = ColumnHeaderView()
        row_header_view = RowHeaderView()
        self.view.setHorizontalHeader(column_header_view)
        self.view.setVerticalHeader(row_header_view)
        self.view.horizontalHeader().sectionDoubleClicked.connect(column_header_view.rename)
        self.view.verticalHeader().sectionDoubleClicked.connect(row_header_view.rename)

        self.view.resizeColumnsToContents()

        new_terrain_button = QPushButton("Add Terrain Type Row")
        new_terrain_button.clicked.connect(self.model.add_new_row)
        new_mtype_button = QPushButton("Add Movement Type Column")
        new_mtype_button.clicked.connect(self.model.add_new_col)
        self.buttonbox = QDialogButtonBox(Qt.Horizontal, self)
        self.buttonbox.addButton(new_terrain_button, QDialogButtonBox.ActionRole)
        self.buttonbox.addButton(new_mtype_button, QDialogButtonBox.ActionRole)
        layout.addWidget(self.buttonbox, 1, 0, alignment=Qt.AlignLeft)

        self.buttonbox = QDialogButtonBox(QDialogButtonBox.Ok, Qt.Horizontal, self)
        layout.addWidget(self.buttonbox, 1, 1)
        self.buttonbox.accepted.connect(self.accept)

    def accept(self):
        super().accept()

class VerticalTextHeaderStyle(QProxyStyle):
    def __init__(self, fontHeight):
        super().__init__("Fusion")
        self.half_font_height = fontHeight / 2

    def drawControl(self, element, option, painter, parent=None):
        if (element == QStyle.CE_HeaderLabel):
            header = option
            painter.save()
            painter.translate(header.rect.center().x() + self.half_font_height, header.rect.bottom())
            painter.rotate(-90)
            painter.drawText(0, 0, header.text)
            painter.restore()
        elif (element == QStyle.CE_HeaderSection):
            super().drawControl(element, option, painter, parent)
            # pass
        elif (element == QStyle.CE_HeaderEmptyArea):
            super().drawControl(element, option, painter, parent)
            # pass
        else:
            super().drawControl(element, option, painter, parent)

class McostDeletionDialog(DeletionDialog):
    @staticmethod
    def get_swap(tabs, swap_box, parent=None):
        dialog = DeletionDialog(tabs, swap_box, parent)
        result = dialog.exec_()
        if result == QDialog.Accepted:
            text = dialog.swap_box.edit.currentText()
            return text, True
        else:
            return None, False

class ColumnHeaderView(QHeaderView):
    def __init__(self, parent=None):
        super().__init__(Qt.Horizontal, parent)
        self._metrics = QFontMetrics(self.font())
        self._descent = self._metrics.descent()
        self._margin = 10

        custom_style = VerticalTextHeaderStyle(self.font().pixelSize() + 1)
        self.setStyle(custom_style)

        self.setContextMenuPolicy(Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.customHeaderMenuRequested)

        self.setSectionsClickable(True)
        self.sectionClicked.connect(self.select_column)

    def sizeHint(self):
        return QSize(0, self._get_text_width() + 2 * self._margin)

    def _get_text_width(self):
        return max([self._metrics.width(self._get_data(i)) for i in range(self.model().columnCount())])

    def _get_data(self, index):
        return self.model().headerData(index, self.orientation())

    def select_column(self, col_idx):
        self.parent().selectColumn(col_idx)

    def insert(self, idx):
        self.parent().model().insert_col(idx)

    def delete(self, idx):
        if self.parent().model().columnCount() > 1:
            column_name = DB.mcost.column_headers[idx]
            affected_classes = [klass for klass in DB.classes if klass.movement_group == column_name]
            if affected_classes:
                from app.editor.class_editor.class_model import ClassModel
                model = ClassModel
                msg = "Deleting column <b>%s</b> would modify these classes." % column_name
                deletion_tab = DeletionTab(affected_classes, model, msg, "Classes")
                swap, ok = McostDeletionDialog.get_swap([deletion_tab], MovementClassBox(self))
                if ok:
                    # Actually make the change
                    for klass in affected_classes:
                        klass.movement_group = swap
                else:
                    return  # User cancelled swap
            self.parent().model().delete_col(idx)
        else:
            QMessageBox.critical(self.parent(), 'Error', 'Cannot delete when only one column left!')

    def rename(self, idx):
        old_column_name = DB.mcost.column_headers[idx]
        self.parent().model().change_col_header(idx)
        new_column_name = DB.mcost.column_headers[idx]
        affected_classes = [klass for klass in DB.classes if klass.movement_group == old_column_name]
        for klass in affected_classes:
            klass.movement_group = new_column_name

    def cut(self, idx):
        self.parent().model().copy_col(idx)
        self.parent().model().mark_col(idx)

    def copy(self, idx):
        self.parent().model().copy_col(idx)

    def paste(self, idx):
        self.parent().model().paste_col(idx)
        self.parent().model().delete_marked_col()

    def customHeaderMenuRequested(self, pos):
        col_idx = self.logicalIndexAt(pos)
        self.parent().selectColumn(col_idx)

        insert_action = QAction("Insert", self, triggered=lambda: self.insert(col_idx))
        delete_action = QAction("Delete", self, triggered=lambda: self.delete(col_idx))
        rename_action = QAction("Rename", self, triggered=lambda: self.rename(col_idx))
        cut_action = QAction("Cut", self, shortcut="Ctrl+X", triggered=lambda: self.cut(col_idx))
        copy_action = QAction("Copy", self, shortcut="Ctrl+C", triggered=lambda: self.copy(col_idx))
        paste_action = QAction("Paste", self, shortcut="Ctrl+V", triggered=lambda: self.paste(col_idx))

        menu = QMenu(self)
        menu.addAction(cut_action)
        menu.addAction(copy_action)
        menu.addAction(paste_action)
        menu.addSeparator()
        menu.addAction(insert_action)
        menu.addAction(delete_action)
        menu.addAction(rename_action)

        menu.popup(self.viewport().mapToGlobal(pos))

class RowHeaderView(QHeaderView):
    def __init__(self, parent=None):
        super().__init__(Qt.Vertical, parent)

        self.setContextMenuPolicy(Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.customHeaderMenuRequested)

        self.setSectionsClickable(True)
        self.sectionClicked.connect(self.select_row)

    def select_row(self, idx):
        self.parent().selectRow(idx)

    def insert(self, idx):
        self.parent().model().insert_row(idx)

    def delete(self, idx):
        if self.parent().model().rowCount() > 1:
            row_name = DB.mcost.row_headers[idx]
            affected_terrain = [terrain for terrain in DB.terrain if terrain.mtype == row_name]
            if affected_terrain:
                from app.editor.terrain_editor.terrain_model import TerrainModel
                model = TerrainModel
                msg = "Deleting row <b>%s</b> would modify these terrains." % row_name
                deletion_tab = DeletionTab(affected_terrain, model, msg, "Terrain")
                swap, ok = McostDeletionDialog.get_swap([deletion_tab], MovementCostBox(self))
                if ok:
                    for terrain in affected_terrain:
                        terrain.mtype = swap
                else:
                    return # User cancelled swap
            self.parent().model().delete_row(idx)
        else:
            QMessageBox.critical(self.parent(), 'Error', 'Cannot delete when only one row left!')

    def rename(self, idx):
        old_row_name = DB.mcost.row_headers[idx]
        self.parent().model().change_row_header(idx)
        new_row_name = DB.mcost.row_headers[idx]
        affected_terrain = [terrain for terrain in DB.terrain if terrain.mtype == old_row_name]
        for terrain in affected_terrain:
            terrain.mtype = new_row_name

    def cut(self, idx):
        self.parent().model().copy_row(idx)
        self.parent().model().mark_row(idx)

    def copy(self, idx):
        self.parent().model().copy_row(idx)

    def paste(self, idx):
        self.parent().model().paste_row(idx)
        self.parent().model().delete_marked_row()

    def customHeaderMenuRequested(self, pos):
        row_idx = self.logicalIndexAt(pos)
        self.parent().selectRow(row_idx)

        insert_action = QAction("Insert", self, triggered=lambda: self.insert(row_idx))
        delete_action = QAction("Delete", self, triggered=lambda: self.delete(row_idx))
        rename_action = QAction("Rename", self, triggered=lambda: self.rename(row_idx))
        cut_action = QAction("Cut", self, shortcut="Ctrl+X", triggered=lambda: self.cut(row_idx))
        copy_action = QAction("Copy", self, shortcut="Ctrl+C", triggered=lambda: self.copy(row_idx))
        paste_action = QAction("Paste", self, shortcut="Ctrl+V", triggered=lambda: self.paste(row_idx))

        menu = QMenu(self)
        menu.addAction(cut_action)
        menu.addAction(copy_action)
        menu.addAction(paste_action)
        menu.addSeparator()
        menu.addAction(insert_action)
        menu.addAction(delete_action)
        menu.addAction(rename_action)

        menu.popup(self.viewport().mapToGlobal(pos))

class McostDelegate(QStyledItemDelegate):
    def createEditor(self, parent, option, index):
        editor = QLineEdit(parent)
        validator = QDoubleValidator(1, 99, 2)
        validator.setNotation(QDoubleValidator.StandardNotation)
        editor.setValidator(validator)
        return editor

class GridModel(QAbstractTableModel):
    def __init__(self, data, parent):
        super().__init__(parent)
        self._data = data

        # For cut, copy, and paste
        self.copied_row = None
        self.copied_col = None
        self.marked_row = None
        self.marked_col = None

    def add_new_row(self):
        new_row_name = str_utils.get_next_name('New', self._data.row_headers)
        self._data.add_row(new_row_name)
        self.layoutChanged.emit()

    def add_new_col(self):
        new_col_name = str_utils.get_next_name('New', self._data.column_headers)
        self._data.add_column(new_col_name)
        self.layoutChanged.emit()

    def headerData(self, idx, orientation, role=Qt.DisplayRole):
        if role != Qt.DisplayRole:
            return None
        if orientation == Qt.Vertical:  # Row
            return self._data.terrain_types[idx]
        elif orientation == Qt.Horizontal:  # Column
            return self._data.unit_types[idx]
        return None

    def insert_col(self, idx):
        new_col_name = str_utils.get_next_name('New', self._data.column_headers)
        self._data.insert_column(new_col_name, idx)
        self.layoutChanged.emit()

    def insert_row(self, idx):
        new_row_name = str_utils.get_next_name('New', self._data.row_headers)
        self._data.insert_row(new_row_name, idx)
        self.layoutChanged.emit()

    def delete_col(self, idx):
        self._data.delete_column(idx)
        self.layoutChanged.emit()

    def delete_row(self, idx):
        self._data.delete_row(idx)
        self.layoutChanged.emit()

    def change_col_header(self, idx):
        old_header = self._data.column_headers[idx]
        new_header, ok = QInputDialog.getText(self.parent(), 'Change Movement Type', 'Header:', QLineEdit.Normal, old_header)
        if ok:
            self._data.column_headers[idx] = str_utils.get_next_name(new_header, self._data.column_headers)

    def change_row_header(self, idx):
        old_header = self._data.row_headers[idx]
        new_header, ok = QInputDialog.getText(self.parent(), 'Change Terrain Type', 'Header:', QLineEdit.Normal, old_header)
        if ok:
            self._data.row_headers[idx] = str_utils.get_next_name(new_header, self._data.row_headers)

    def copy_row(self, idx):
        self.copied_row = self._data.get_row(idx)

    def copy_col(self, idx):
        self.copied_col = self._data.get_column(idx)

    def paste_row(self, idx):
        if self.copied_row:
            self._data.set_row(idx, self.copied_row)
        self.copied_row = None

    def paste_col(self, idx):
        if self.copied_col:
            self._data.set_column(idx, self.copied_col)
        self.copied_col = None

    def mark_row(self, idx):
        self.marked_row = idx

    def mark_col(self, idx):
        self.marked_col = idx

    def delete_marked_row(self):
        if self.marked_row:
            self.delete_row(self.marked_row)
        self.marked_row = None

    def delete_marked_col(self):
        if self.marked_col:
            self.delete_col(self.marked_col)
        self.marked_col = None

    def rowCount(self, parent=None):
        return self._data.height()

    def columnCount(self, parent=None):
        return self._data.width()

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            return self._data.get((index.column(), index.row()))
        elif role == Qt.TextAlignmentRole:
            return Qt.AlignRight + Qt.AlignVCenter
        elif role == Qt.ForegroundRole:
            if index.row() == self.marked_row:
                return QBrush(QColor("gray"))
            elif index.column() == self.marked_col:
                return QBrush(QColor("gray"))
            else:
                palette = QApplication.palette()
                return QBrush(palette.color(QPalette.Text))
        return None

    def setData(self, index, value, role):
        if not index.isValid():
            return False
        self._data.set((index.column(), index.row()), float(value))
        self.dataChanged.emit(index, index)
        return True

    # Determines how each item behaves
    def flags(self, index):
        return Qt.ItemIsEditable | Qt.ItemIsEnabled | Qt.ItemIsSelectable | Qt.ItemNeverHasChildren

class MovementCostModel(CollectionModel):
    def rowCount(self, parent=None):
        return len(self._data.terrain_types)

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            mtype = self._data.terrain_types[index.row()]
            return mtype
        return None

class MovementClassModel(CollectionModel):
    def rowCount(self, parent=None):
        return len(self._data.unit_types)

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            mtype = self._data.unit_types[index.row()]
            return mtype
        return None

# Testing
# Run "python -m app.editor.mcost_dialog" from main directory
if __name__ == '__main__':
    import sys
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    DB.load('default.ltproj')
    window = McostDialog()
    window.show()
    app.exec_()
