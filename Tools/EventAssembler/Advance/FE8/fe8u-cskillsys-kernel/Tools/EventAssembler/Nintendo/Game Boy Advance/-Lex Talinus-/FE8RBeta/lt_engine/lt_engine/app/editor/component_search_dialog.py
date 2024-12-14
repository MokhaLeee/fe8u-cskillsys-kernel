from PyQt5.QtCore import QSize, QSortFilterProxyModel, Qt
from PyQt5.QtGui import QFontMetrics, QPalette
from PyQt5.QtWidgets import (QApplication, QDialog, QFrame, QHBoxLayout,
                             QLineEdit, QListView, QSplitter, QStyle,
                             QStyledItemDelegate, QTextEdit, QVBoxLayout)

from app.editor.base_database_gui import CollectionModel

class ShowComponentSearchDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowFlag(Qt.WindowContextHelpButtonHint, False)
        self.setWindowTitle("Search Components")
        self.window = parent

        get_components = self.window.get_components()
        get_tags = self.window.get_tags()
        self.components = get_components
        self.categories = [category.value for category in get_tags]
        self._data = []
        for category in self.categories:
            # Ignore hidden category
            if category in {'hidden', 'deprecated'}:
                continue
            self._data.append(category)
            components = [component() for component in self.components if component.tag.value == category]
            self._data += components

        # Get text information about component
        self.model = ComponentModel(self._data, self.categories, self)
        self.proxy_model = QSortFilterProxyModel()
        self.proxy_model.setSourceModel(self.model)
        self.view = QListView(self)
        self.view.setMinimumSize(256, 360)
        self.view.setModel(self.proxy_model)
        self.view.doubleClicked.connect(self.on_double_click)

        self.delegate = ComponentDelegate(self._data, self)
        self.view.setItemDelegate(self.delegate)

        self.search_box = QLineEdit(self)
        self.search_box.setPlaceholderText("Enter search term here...")
        self.search_box.textChanged.connect(self.search)

        self.desc_box = QTextEdit(self)
        self.desc_box.setReadOnly(True)
        self.view.selectionModel().selectionChanged.connect(self.on_item_changed)

        left_layout = QVBoxLayout()
        left_layout.addWidget(self.search_box)
        left_layout.addWidget(self.view)
        left_frame = QFrame(self)
        left_frame.setLayout(left_layout)

        splitter = QSplitter(self)
        splitter.setOrientation(Qt.Horizontal)
        splitter.setChildrenCollapsible(False)
        splitter.setStyleSheet("QSplitter::handle:horizontal {background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc); border: 1px solid #777; width: 13px; margin-top: 2px; margin-bottom: 2px; border-radius: 4px;}")
        splitter.addWidget(left_frame)
        splitter.addWidget(self.desc_box)

        main_layout = QHBoxLayout()
        main_layout.addWidget(splitter)
        self.setLayout(main_layout)

    def search(self, text):
        self.proxy_model.setFilterRegularExpression(text.lower())

    def on_double_click(self, index):
        # Add the component to the item/skill
        index = self.proxy_model.mapToSource(index)
        idx = index.row()
        component = self._data[idx]
        if component not in self.categories:
            self.window.add_component(self.components.get(component.nid))

    def on_item_changed(self, curr, prev):
        # Show new information about the item you're selecting
        if curr.indexes():
            index = curr.indexes()[0]
            index = self.proxy_model.mapToSource(index)
            idx = index.row()
            component = self._data[idx]
            if component not in self.categories:
                # component name
                text = '**%s**\n' % component.class_name()

                # component desc
                if component.desc:
                    text += " --- \n\n"
                text += component.desc
                self.desc_box.setMarkdown(text)
            else:
                self.desc_box.setMarkdown(component + ' Section')
        else:
            self.desc_box.setMarkdown('')


class ComponentModel(CollectionModel):
    def __init__(self, data, categories, window):
        super().__init__(data, window)
        self.categories = categories

    def get_text(self, component) -> str:
        full_text = component.class_name()
        return full_text

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            component = self._data[index.row()]
            if component in self.categories:
                category = component
                # We want to include the header if any of it's children are counted
                return '-'.join([self.get_text(component).lower() for component in self._data if component not in self.categories and component.tag == category])
            else:
                return self.get_text(component).lower()

class ComponentDelegate(QStyledItemDelegate):
    def __init__(self, data, parent=None):
        super().__init__(parent=None)
        self.window = parent
        self._data = data

    def sizeHint(self, option, index):
        index = self.window.proxy_model.mapToSource(index)
        component = self._data[index.row()]
        if hasattr(component, 'nid'):
            return QSize(0, 24)
        else:
            return QSize(0, 32)

    def paint(self, painter, option, index):
        index = self.window.proxy_model.mapToSource(index)
        component = self._data[index.row()]
        rect = option.rect
        left = rect.left()
        right = rect.right()
        top = rect.top()
        bottom = rect.bottom()
        if option.state & QStyle.State_Selected:
            palette = QApplication.palette()
            color = palette.color(QPalette.Highlight)
            painter.fillRect(rect, color)
        font = painter.font()
        if hasattr(component, 'nid'):
            font.setBold(True)
            font_height = QFontMetrics(font).lineSpacing()
            painter.setFont(font)
            painter.drawText(left, top + font_height, component.class_name())
        else:
            prev_size = font.pointSize()
            font.setPointSize(prev_size + 4)
            font_height = QFontMetrics(font).lineSpacing()
            painter.setFont(font)
            painter.drawText(left, top + font_height, component)
            font.setPointSize(prev_size)
            painter.setFont(font)
            painter.drawLine(left, int(top + 1.25 * font_height), right, int(top + 1.25 * font_height))
