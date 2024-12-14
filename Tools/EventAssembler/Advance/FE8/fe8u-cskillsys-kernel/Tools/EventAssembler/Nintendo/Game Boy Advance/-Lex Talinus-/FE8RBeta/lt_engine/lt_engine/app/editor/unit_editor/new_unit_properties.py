from app.editor.lib.components.database_delegate import UnitFieldDelegate
from app.extensions.key_value_delegate import KeyValueDelegate, KeyValueDoubleListModel
from app.data.database.database import DB
from app.editor.custom_widgets import AffinityBox, ClassBox
from app.editor.component_editor_properties import T
from app.editor.icons import UnitPortrait
from app.editor.item_list_widget import ItemListWidget
from app.editor.learned_skill_delegate import LearnedSkillDelegate
from app.editor.stat_widget import (StatAverageDialog, StatListWidget,
                                    UnitStatAveragesModel)
from app.editor.tag_widget import TagDialog
from app.editor.weapon_editor import weapon_model
from app.editor.lib.components.validated_line_edit import NidLineEdit
from app.extensions.custom_gui import ComboBox, PropertyBox, QHLine
from app.extensions.list_models import ReverseDoubleListModel, VirtualListModel
from app.extensions.list_widgets import (AppendMultiListWidget,
                                         BasicSingleListWidget)
from app.extensions.multi_select_combo_box import MultiSelectComboBox
from app.utilities import str_utils
from app.utilities.typing import NID

from PyQt5.QtCore import QItemSelection, QItemSelectionModel, Qt
from PyQt5.QtGui import QFontMetrics, QIcon
from PyQt5.QtWidgets import (QDialog, QGridLayout, QHBoxLayout, QLineEdit,
                             QPushButton, QSpinBox, QSplitter,
                             QStyledItemDelegate, QTableView, QTextEdit,
                             QVBoxLayout, QWidget)

from typing import (Callable, Optional)

class WexpModel(VirtualListModel):
    def __init__(self, columns, data, parent=None):
        super().__init__(parent)
        self.window = parent
        self._columns = self._headers = columns
        self._data: dict = data

    def rowCount(self, parent=None):
        return 1

    def columnCount(self, parent=None):
        return len(self._headers)

    def set_new_data(self, wexp_gain: dict):
        self._data: dict = wexp_gain
        self.layoutChanged.emit()

    def update_column_header(self, columns):
        self._columns = self._headers = columns

    def headerData(self, idx, orientation, role=Qt.DisplayRole):
        if role == Qt.DisplayRole and orientation == Qt.Horizontal:
            # return self._columns[idx].nid
            return None
        elif role == Qt.DecorationRole and orientation == Qt.Horizontal:
            weapon = self._columns[idx]
            pixmap = weapon_model.get_pixmap(weapon)
            if pixmap:
                return QIcon(pixmap)
        return None

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole or role == Qt.EditRole:
            weapon = self._columns[index.column()]
            wexp_gain = self._data.get(weapon.nid)
            if wexp_gain:
                return wexp_gain.wexp_gain
            else:
                return 0
        elif role == Qt.TextAlignmentRole:
            return Qt.AlignRight + Qt.AlignVCenter

    def setData(self, index, value, role):
        if not index.isValid():
            return False
        weapon = self._columns[index.column()]
        wexp_gain = self._data.get(weapon.nid)
        if not wexp_gain:
            self._data[weapon.nid] = DB.weapons.default(DB)
            wexp_gain = self._data[weapon.nid]
        if value in DB.weapon_ranks.keys():
            value = DB.weapon_ranks.get(value).requirement
        elif str_utils.is_int(value):
            value = int(value)
        else:
            value = 0
        wexp_gain.wexp_gain = value
        self.dataChanged.emit(index, index)
        return True

    def flags(self, index):
        basic_flags = Qt.ItemIsEnabled | Qt.ItemIsSelectable | Qt.ItemNeverHasChildren | Qt.ItemIsEditable
        return basic_flags

class HorizWeaponListWidget(BasicSingleListWidget):
    def __init__(self, data, title, dlgate, parent=None):
        QWidget.__init__(self, parent)
        self.initiate(data, parent)

        self.model = WexpModel(DB.weapons, data, self)
        self.view = QTableView(self)
        self.view.setModel(self.model)
        self.view.setFixedHeight(60)
        delegate = dlgate(self.view)
        self.view.setItemDelegate(delegate)

        self.placement(data, title)

        for col in range(len(DB.weapons)):
            self.view.resizeColumnToContents(col)
            self.view.setColumnWidth(col, 20)

class HorizWeaponListDelegate(QStyledItemDelegate):
    def createEditor(self, parent, option, idnex):
        editor = ComboBox(parent)
        editor.setEditable(True)
        editor.addItem('0')
        for rank in DB.weapon_ranks:
            editor.addItem(rank.rank)
        return editor

class NewUnitProperties(QWidget):
    title = "Unit"
    
    def __init__(self, parent, current: Optional[T] = None,
                 attempt_change_nid: Optional[Callable[[NID, NID], bool]] = None,
                 on_icon_change: Optional[Callable] = None):
        super().__init__(parent)

        self.current: Optional[T] = current
        self.cached_nid: Optional[NID] = self.current.nid if self.current else None
        self.attempt_change_nid = attempt_change_nid
        self.on_icon_change = on_icon_change

        top_section = QHBoxLayout()

        main_section = QGridLayout()

        self.icon_edit = UnitPortrait(self)
        self.icon_edit.sourceChanged.connect(self.on_icon_changed)
        main_section.addWidget(self.icon_edit, 0, 0, 2, 1, Qt.AlignHCenter)

        self.nid_box = PropertyBox(_("Unique ID"), NidLineEdit, self)
        self.nid_box.edit.textChanged.connect(self.nid_changed)
        self.nid_box.edit.editingFinished.connect(self.nid_done_editing)
        main_section.addWidget(self.nid_box, 0, 1)

        self.name_box = PropertyBox(_("Display Name"), QLineEdit, self)

        self.name_box.edit.textChanged.connect(self.name_changed)
        main_section.addWidget(self.name_box, 0, 2)

        self.variant_box = PropertyBox("Animation Variant", QLineEdit, self)
        self.variant_box.edit.textChanged.connect(self.variant_changed)
        self.variant_box.edit.setPlaceholderText("No Variant")
        main_section.addWidget(self.variant_box, 1, 2)

        self.desc_box = PropertyBox(_("Description"), QTextEdit, self)
        self.desc_box.edit.textChanged.connect(self.desc_changed)
        font_height = QFontMetrics(self.desc_box.edit.font())
        self.desc_box.edit.setFixedHeight(font_height.lineSpacing() * 3 + 20)
        main_section.addWidget(self.desc_box, 2, 0, 1, 3)

        self.level_box = PropertyBox(_("Level"), QSpinBox, self)
        self.level_box.edit.setRange(1, 255)
        self.level_box.edit.setAlignment(Qt.AlignRight)
        self.level_box.edit.valueChanged.connect(self.level_changed)
        main_section.addWidget(self.level_box, 1, 1)

        self.class_box = ClassBox(self)
        self.class_box.edit.currentIndexChanged.connect(self.class_changed)
        main_section.addWidget(self.class_box, 3, 0)

        tag_section = QHBoxLayout()

        self.tag_box = PropertyBox(_("Personal Tags"), MultiSelectComboBox, self)
        self.tag_box.edit.setPlaceholderText(_("No tag"))
        self.tag_box.edit.addItems(DB.tags.keys())
        self.tag_box.edit.updated.connect(self.tags_changed)
        tag_section.addWidget(self.tag_box)

        self.tag_box.add_button(QPushButton('...'))
        self.tag_box.button.setMaximumWidth(40)
        self.tag_box.button.clicked.connect(self.access_tags)

        main_section.addLayout(tag_section, 3, 1, 1, 2)

        self.unit_stat_widget = StatListWidget(self.current, _("Stats"), reset_button=True, parent=self)
        self.unit_stat_widget.button.clicked.connect(self.display_averages)
        self.unit_stat_widget.reset_button.clicked.connect(self.reset_stats)
        self.unit_stat_widget.model.dataChanged.connect(self.stat_list_model_data_changed)
        self.unit_stat_widget.view.setFixedHeight(130)
        self.averages_dialog = None
        # Changing of stats done automatically by using model view framework within

        attrs = ("level", "skill_nid")
        self.personal_skill_widget = AppendMultiListWidget([], _("Personal Skills"), attrs, LearnedSkillDelegate, self, model=ReverseDoubleListModel)
        self.personal_skill_widget.view.setMaximumHeight(120)

        noteAttrs = ("Category", "Entries")
        self.unit_notes_widget = AppendMultiListWidget([], "Unit Notes", noteAttrs, KeyValueDelegate, self, model=KeyValueDoubleListModel)
        self.unit_notes_widget.view.setMaximumHeight(120)
        if not DB.constants.value('unit_notes'):
            self.unit_notes_widget.hide()

        fieldAttrs = ("Field", "Value")
        self.unit_fields_widget = AppendMultiListWidget([], "Unit Fields", fieldAttrs, UnitFieldDelegate, self, model=KeyValueDoubleListModel)
        self.unit_fields_widget.view.setMaximumHeight(120)

        default_weapons = {weapon_nid: DB.weapons.default(DB) for weapon_nid in DB.weapons.keys()}
        self.wexp_gain_widget = HorizWeaponListWidget(
            default_weapons, "Starting Weapon Experience", HorizWeaponListDelegate, self)

        item_section = QHBoxLayout()
        self.item_widget = ItemListWidget(_("Starting Items"), self)
        self.item_widget.items_updated.connect(self.items_changed)
        item_section.addWidget(self.item_widget)

        self.alternate_class_box = PropertyBox("Alternate Classes", MultiSelectComboBox, self)
        self.alternate_class_box.edit.setPlaceholderText("Class Change Options...")
        self.alternate_class_box.edit.addItems(DB.classes.keys())
        self.alternate_class_box.edit.updated.connect(self.alternate_class_changed)

        self.affinity_box = AffinityBox(self)
        self.affinity_box.edit.currentIndexChanged.connect(self.affinity_changed)

        total_section = QVBoxLayout()
        total_section.addLayout(top_section)
        total_section.addLayout(main_section)
        total_section.addWidget(QHLine())
        total_section.addWidget(self.unit_stat_widget)
        total_section.addWidget(self.wexp_gain_widget)
        total_widget = QWidget()
        total_widget.setLayout(total_section)

        right_section = QVBoxLayout()
        right_section.addLayout(item_section)
        right_section.addWidget(QHLine())
        right_section.addWidget(self.personal_skill_widget)
        right_section.addWidget(self.unit_notes_widget)
        right_section.addWidget(self.unit_fields_widget)
        right_section.addWidget(QHLine())
        right_section.addWidget(self.alternate_class_box)
        right_section.addWidget(self.affinity_box)
        right_widget = QWidget()
        right_widget.setLayout(right_section)

        self.splitter = QSplitter(self)
        self.splitter.setChildrenCollapsible(False)
        self.splitter.addWidget(total_widget)
        self.splitter.addWidget(right_widget)
        self.splitter.setStyleSheet("QSplitter::handle:horizontal {background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc); border: 1px solid #777; width: 13px; margin-top: 2px; margin-bottom: 2px; border-radius: 4px;}")

        final_section = QHBoxLayout()
        self.setLayout(final_section)
        final_section.addWidget(self.splitter)

        self.set_current(self.current)

    def on_icon_changed(self, nid):
        if self.current:
            self.current.portrait_nid = nid
            if self.on_icon_change:
                self.on_icon_change()

    def nid_changed(self, text):
        if self.current:
            # Also change name if they are identical
            if self.current.name == self.current.nid.replace('_', ' '):
                self.name_box.edit.setText(text.replace('_', ' '))
            self.current.nid = text

    def nid_done_editing(self):
        if self.current and self.cached_nid:
            self.nid_box.edit.blockSignals(True)  # message box causes focus loss which double triggers nid_done_editing
            # Check validity of nid!
            if self.attempt_change_nid and self.attempt_change_nid(self.cached_nid, self.current.nid):
                self.cached_nid = self.current.nid
            else:
                self.current.nid = self.cached_nid
                self.nid_box.edit.setText(self.cached_nid)
            self.nid_box.edit.blockSignals(False)

    def name_changed(self, text):
        self.current.name = text

    def desc_changed(self, text=None):
        self.current.desc = self.desc_box.edit.toPlainText()

    def level_changed(self, val):
        self.current.level = int(val)
        if self.averages_dialog:
            self.averages_dialog.update()

    def class_changed(self, index):
        self.current.klass = self.class_box.edit.currentText()
        if self.averages_dialog:
            self.averages_dialog.update()

    def tags_changed(self):
        self.current.tags = self.tag_box.edit.currentText()

    def reset_stats(self):
        model = self.unit_stat_widget.model
        view = self.unit_stat_widget.view
        selected_indexes = view.selectionModel().selectedIndexes()
        my_klass = DB.classes.get(self.current.klass)

        if not selected_indexes:
            # Select all
            topLeft = model.index(0, 0)
            bottomRight = model.index(model.rowCount() - 1, model.columnCount() - 1)
            selection = QItemSelection(topLeft, bottomRight)
            view.selectionModel().select(selection, QItemSelectionModel.Select)
            selected_indexes = view.selectionModel().selectedIndexes()

        for index in selected_indexes:
            stat_nid = DB.stats[index.column()].nid
            if index.row() == 0:
                class_value = my_klass.bases.get(stat_nid, 0)
            elif index.row() == 1:
                class_value = my_klass.growths.get(stat_nid, 0)
            else:
                class_value = 0
            model.setData(index, class_value, Qt.EditRole)

    def display_averages(self):
        # Modeless dialog
        if not self.averages_dialog:
            self.averages_dialog = StatAverageDialog(self.current, "Unit", UnitStatAveragesModel, self)
        self.averages_dialog.show()
        self.averages_dialog.raise_()
        self.averages_dialog.activateWindow()

    def close_averages(self):
        if self.averages_dialog:
            self.averages_dialog.done(0)
            self.averages_dialog = None

    def stat_list_model_data_changed(self, index1, index2):
        if self.averages_dialog:
            self.averages_dialog.update()

    def items_changed(self):
        self.current.starting_items = self.item_widget.get_items()

    def access_tags(self):
        dlg = TagDialog.create(self)
        result = dlg.exec_()
        if result == QDialog.Accepted:
            self.tag_box.edit.clear()
            self.tag_box.edit.addItems(DB.tags.keys())
            self.tag_box.edit.setCurrentTexts(self.current.tags)
        else:
            pass

    def variant_changed(self, text):
        self.current.variant = text

    def alternate_class_changed(self):
        self.current.alternate_classes = self.alternate_class_box.edit.currentText()

    def affinity_changed(self, index):
        self.current.affinity = self.affinity_box.edit.currentText()

    def set_current(self, current):
        if not current:
            self.setEnabled(False)
        else:
            self.setEnabled(True)
            self.current = current
            self.cached_nid = current.nid
            self.nid_box.edit.setText(current.nid)
            self.name_box.edit.setText(current.name)
            self.desc_box.edit.setText(current.desc)
            self.level_box.edit.setValue(int(current.level))
            self.class_box.edit.setValue(current.klass)
            tags = current.tags[:]
            self.tag_box.edit.clear()
            self.tag_box.edit.addItems(DB.tags.keys())
            self.tag_box.edit.setCurrentTexts(tags)

            self.unit_stat_widget.update_stats()
            self.unit_stat_widget.set_new_obj(current)
            if self.averages_dialog:
                self.averages_dialog.set_current(current)

            self.personal_skill_widget.set_current(current.learned_skills)
            self.unit_notes_widget.set_current(current.unit_notes)
            self.unit_fields_widget.set_current(current.fields)
            self.wexp_gain_widget.set_current(current.wexp_gain)
            self.item_widget.set_current(current.starting_items)

            if current.variant:
                self.variant_box.edit.setText(current.variant)
            else:
                self.variant_box.edit.clear()

            self.alternate_class_box.edit.clear()
            self.alternate_class_box.edit.addItems(DB.classes.keys())
            if current.alternate_classes:
                alternate_classes = current.alternate_classes[:]
                self.alternate_class_box.edit.setCurrentTexts(alternate_classes)
            if current.affinity:
                self.affinity_box.edit.setValue(current.affinity)
            else:
                self.affinity_box.edit.setValue("None")

            self.icon_edit.set_current(current.portrait_nid)

    def hideEvent(self, event):
        self.close_averages()
