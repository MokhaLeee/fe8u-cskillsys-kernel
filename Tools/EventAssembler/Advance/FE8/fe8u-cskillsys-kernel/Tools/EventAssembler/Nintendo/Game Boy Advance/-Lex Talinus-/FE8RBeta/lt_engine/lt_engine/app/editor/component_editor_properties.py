import functools
from enum import Enum
from typing import (Callable, Dict, Generic, List, Optional, Tuple, Type,
                    TypeVar)

from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFontMetrics, QIcon
from PyQt5.QtWidgets import (QAction, QGridLayout, QHBoxLayout, QLabel,
                             QLineEdit, QMessageBox, QPushButton, QSizePolicy,
                             QSpacerItem, QTextEdit, QToolBar, QToolButton,
                             QVBoxLayout, QWidget, QWidgetAction)
from app import dark_theme

from app.data.database.components import Component
from app.editor import component_database
from app.editor.component_search_dialog import ShowComponentSearchDialog
from app.editor.icons import PushableIcon16
from app.editor.lib.components.validated_line_edit import NidLineEdit
from app.editor.settings import MainSettingsController
from app.extensions.custom_gui import PropertyBox, QHLine
from app.extensions.qhelpmenu import QHelpMenu
from app.utilities.data import Data
from app.utilities.typing import NID, Protocol

class HasComponents(Protocol):
    nid: NID
    name: str
    desc: str
    components: Data[Component]
    icon_nid: Optional[NID]
    icon_index: Tuple[int, int]

T = TypeVar('T', bound=HasComponents)

class NewComponentProperties(QWidget, Generic[T]):
    title = None
    get_components: Callable[[], Data[Type[Component]]]
    get_templates = None
    get_tags: Callable[[], List[Enum]]

    def __init__(self, parent, current: Optional[T] = None,
                 attempt_change_nid: Optional[Callable[[NID, NID], bool]] = None,
                 on_icon_change: Optional[Callable] = None):
        super().__init__(parent)
        self.attempt_change_nid = attempt_change_nid
        self.on_icon_change = on_icon_change

        self.current: Optional[T] = current
        self.cached_nid: Optional[NID] = self.current.nid if self.current else None

        top_section = QHBoxLayout()

        self.icon_edit = PushableIcon16(self)
        self.icon_edit.sourceChanged.connect(self.on_icon_changed)
        top_section.addWidget(self.icon_edit)

        horiz_spacer = QSpacerItem(40, 10, QSizePolicy.Fixed, QSizePolicy.Fixed)
        top_section.addSpacerItem(horiz_spacer)

        name_section = QVBoxLayout()

        self.nid_box = PropertyBox(_("Unique ID"), NidLineEdit, self)
        self.nid_box.edit.textChanged.connect(self.nid_changed)
        self.nid_box.edit.editingFinished.connect(self.nid_done_editing)
        name_section.addWidget(self.nid_box)

        self.name_box = PropertyBox(_("Display Name"), QLineEdit, self)
        self.name_box.setMinimumWidth(250)
        self.name_box.edit.textChanged.connect(self.name_changed)
        name_section.addWidget(self.name_box)

        top_section.addLayout(name_section)

        main_section = QGridLayout()

        self.desc_box = PropertyBox(_("Description"), QTextEdit, self)
        self.desc_box.edit.textChanged.connect(self.desc_changed)
        font_height = QFontMetrics(self.desc_box.edit.font())
        self.desc_box.edit.setFixedHeight(font_height.lineSpacing() * 3 + 20)
        main_section.addWidget(self.desc_box, 0, 0, 1, 3)

        component_section = QGridLayout()
        component_label = QLabel(_("Components"))
        component_label.setAlignment(Qt.AlignBottom)
        component_section.addWidget(component_label, 0, 0, Qt.AlignBottom)

        # Create actions
        self.property_actions = {}
        for component in self.get_components():
            new_func = functools.partial(self.add_component, component)
            new_action = QAction(QIcon(), component.class_name(), self, triggered=new_func)
            new_action.setToolTip(component.desc)
            self.property_actions[component.nid] = new_action

        # Create toolbar
        self.toolbar = QToolBar(self)
        self.menus = {}

        theme = dark_theme.get_theme()
        icon_folder = theme.icon_dir()

        # Add component search to the top component bar
        self.show_components_dialog = None
        self.show_components_button = QPushButton("Search Components")
        self.show_components_button.clicked.connect(self.show_components)
        self.toolbar.addWidget(self.show_components_button)
        for component in self.get_components():
            if component.tag.value not in ('hidden', 'deprecated'):
                if component.tag.value not in self.menus:
                    new_menu = QHelpMenu(self)
                    self.menus[component.tag.value] = new_menu
                    toolbutton = QToolButton(self)
                    toolbutton.setIcon(QIcon(f"{icon_folder}/component_%s.png" % component.tag.value))
                    toolbutton.setMenu(new_menu)
                    toolbutton.setPopupMode(QToolButton.InstantPopup)
                    toolbutton_action = QWidgetAction(self)
                    toolbutton_action.setDefaultWidget(toolbutton)
                    self.toolbar.addAction(toolbutton_action)
                menu = self.menus[component.tag.value]
                menu.addAction(self.property_actions.get(component.nid))

        # Template action
        for template_key, template_value in self.get_templates():
            new_func = functools.partial(self.add_template, template_value)
            template_action = QAction(QIcon(), template_key, self, triggered=new_func)
            self.property_actions[template_key] = template_action

        if self.get_templates():
            template_menu = QHelpMenu(self)
            self.menus['templates'] = template_menu
            toolbutton = QToolButton(self)
            toolbutton.setIcon(QIcon(f"{icon_folder}/component_template.png"))
            toolbutton.setMenu(template_menu)
            toolbutton.setPopupMode(QToolButton.InstantPopup)
            toolbutton_action = QWidgetAction(self)
            toolbutton_action.setDefaultWidget(toolbutton)
            self.toolbar.addAction(toolbutton_action)

        for template_key, template_value in self.get_templates():
            menu = self.menus['templates']
            menu.addAction(self.property_actions.get(template_key))

        component_section.addWidget(self.toolbar, 1, 0, 1, 2)

        self.component_list = component_database.ComponentList(self)
        component_section.addWidget(self.component_list, 2, 0, 1, 2)
        self.component_list.order_swapped.connect(self.component_moved)

        total_section = QVBoxLayout()
        self.setLayout(total_section)
        total_section.addLayout(top_section)
        total_section.addLayout(main_section)
        h_line = QHLine()
        total_section.addWidget(h_line)
        total_section.addLayout(component_section)

        self.set_current(self.current)

    def on_icon_changed(self, nid, x, y):
        if self.current:
            self.current.icon_nid = nid
            self.current.icon_index = (x, y)
            if self.on_icon_change:
                self.on_icon_change()

    def show_components(self):
        # Modeless dialog
        if not self.show_components_dialog:
            self.show_components_dialog = ShowComponentSearchDialog(self)
        self.show_components_dialog.show()
        self.show_components_dialog.raise_()

    def close_components(self):
        if self.show_components_dialog:
            self.show_components_dialog.done(0)
            self.show_components_dialog = None

    def nid_changed(self, text):
        if self.current:
            # Also change name if they are identical
            if self.current.name == self.current.nid.replace('_', ' '):
                self.name_box.edit.setText(text.replace('_', ' '))
            self.current.nid = text

    def nid_done_editing(self):
        if self.current and self.cached_nid:
            self.nid_box.edit.blockSignals(True) # message box causes focus loss which double triggers nid_done_editing
            # Check validity of nid!
            if self.attempt_change_nid and self.attempt_change_nid(self.cached_nid, self.current.nid):
                self.cached_nid = self.current.nid
            else:
                self.current.nid = self.cached_nid
                self.nid_box.edit.setText(self.cached_nid)
            self.nid_box.edit.blockSignals(False)

    def name_changed(self, text):
        if self.current:
            self.current.name = text

    def desc_changed(self, text=None):
        if self.current:
            self.current.desc = self.desc_box.edit.toPlainText()

    def add_template(self, component_list):
        all_components = self.get_components()
        for component_nid in component_list:
            self.add_component(all_components.get(component_nid))

    def add_component(self, component_class):
        if self.current:
            component = component_class(component_class.value)
            if component.nid in self.current.components.keys():
                QMessageBox.warning(self, 'Warning', '%s component already present' % component.class_name())
            else:
                self.current.components.append(component)
                self.add_component_widget(component)
                # Add other components that this should be paired with
                for pair in component.paired_with:
                    if pair not in self.current.components.keys():
                        self.add_component(self.get_components().get(pair))

    def add_component_widget(self, component):
        c = component_database.get_display_widget(component, self)
        self.component_list.add_component(c)

    def remove_component(self, component_widget):
        if self.current:
            data = component_widget._data
            self.component_list.remove_component(component_widget)
            self.current.components.delete(data)
            # Remove all paired components
            for pair in data.paired_with:
                if pair in self.current.components.keys():
                    idx = self.component_list.index_list.index(pair)
                    item = self.component_list.item(idx)
                    component_widget = self.component_list.itemWidget(item)
                    self.remove_component(component_widget)

    def component_moved(self, start, end):
        if self.current:
            self.current.components.move_index(start, end)

    def set_current(self, current: Optional[T]):
        if not current:
            self.setEnabled(False)
        else:
            self.setEnabled(True)
            self.current = current
            self.cached_nid = current.nid
            self.nid_box.edit.setText(current.nid)
            self.name_box.edit.setText(current.name)
            self.desc_box.edit.setText(current.desc)
            self.icon_edit.change_icon(current.icon_nid, current.icon_index)
            self.component_list.clear()
            for component in current.components.values():
                self.add_component_widget(component)
