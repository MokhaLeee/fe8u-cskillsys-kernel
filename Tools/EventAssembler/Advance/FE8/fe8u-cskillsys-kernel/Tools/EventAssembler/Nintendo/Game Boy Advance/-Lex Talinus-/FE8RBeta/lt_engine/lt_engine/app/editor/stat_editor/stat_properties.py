from PyQt5.QtWidgets import QWidget, QLineEdit, QMessageBox, QVBoxLayout, \
    QSpinBox, QCheckBox
from PyQt5.QtCore import Qt

from app.data.database.database import DB

from app.utilities import str_utils
from app.extensions.custom_gui import PropertyBox, ComboBox, PropertyCheckBox
from app.editor.lib.components.validated_line_edit import NidLineEdit

class StatTypeProperties(QWidget):
    def __init__(self, parent, current=None):
        super().__init__(parent)
        self.window = parent
        self._data = self.window._data

        self.current = current

        name_section = QVBoxLayout()

        self.nid_box = PropertyBox(_("Unique ID"), NidLineEdit, self)
        self.nid_box.edit.textChanged.connect(self.nid_changed)
        self.nid_box.edit.editingFinished.connect(self.nid_done_editing)
        name_section.addWidget(self.nid_box)

        self.name_box = PropertyBox(_("Display Name"), QLineEdit, self)

        self.name_box.edit.textChanged.connect(self.name_changed)
        name_section.addWidget(self.name_box)

        self.max_box = PropertyBox(_("Maximum"), QSpinBox, self)
        self.max_box.edit.setRange(0, 255)
        self.max_box.edit.setAlignment(Qt.AlignRight)
        self.max_box.edit.valueChanged.connect(self.maximum_changed)
        name_section.addWidget(self.max_box)

        self.desc_box = PropertyBox(_("Description"), QLineEdit, self)
        self.desc_box.edit.textChanged.connect(self.desc_changed)
        name_section.addWidget(self.desc_box)

        self.position_box = PropertyBox(_("Position"), ComboBox, self)
        self.position_box.edit.addItems(["hidden", "left", "right"])
        self.position_box.setToolTip("Column within Info Menu in engine")
        self.position_box.edit.currentTextChanged.connect(self.position_changed)
        name_section.addWidget(self.position_box)

        #colored growths
        self.growth_colors_box = PropertyCheckBox(_("Colored Growths"), QCheckBox, self)
        self.growth_colors_box.edit.stateChanged.connect(self.growth_colors_changed)
        self.growth_colors_box.setToolTip(_("Select individually which stat growths will change color based on their value"))
        name_section.addWidget(self.growth_colors_box)

        self.setLayout(name_section)
        name_section.setAlignment(Qt.AlignTop)

        self.clean_all_stats()

    def clean_all_stats(self):
        # makes sure all units have all stats set and nothing else
        all_stats = set([stat.nid for stat in DB.stats])
        for unit in DB.units:
            for row in unit.get_stat_lists():
                unit_stats = set(row.keys())
                missing_stats = all_stats.difference(unit_stats)
                extraneous_stats = unit_stats.difference(all_stats)
                for stat in missing_stats:
                    row[stat] = 0
                for stat in extraneous_stats:
                    row.pop(stat)
        for klass in DB.classes:
            for row in klass.get_stat_lists():
                klass_stats = set(row.keys())
                missing_stats = all_stats.difference(klass_stats)
                extraneous_stats = klass_stats.difference(all_stats)
                for stat in missing_stats:
                    row[stat] = 0
                for stat in extraneous_stats:
                    row.pop(stat)

    def nid_changed(self, text):
        if self.current.name == self.current.nid.replace('_', ' '):
            self.name_box.edit.setText(text.replace('_', ' '))
        self.current.nid = text
        self.window.update_list()

    def on_nid_changed(self, old_nid, new_nid):
        for klass in DB.classes:
            for row in klass.get_stat_lists():
                if old_nid in row:
                    row[new_nid] = row.get(old_nid, 0)
        for unit in DB.units:
            for row in unit.get_stat_lists():
                if old_nid in row:
                    row[new_nid] = row.get(old_nid, 0)

    def nid_done_editing(self):
        # Check validity of nid!
        other_nids = [d.nid for d in self._data.values() if d is not self.current]
        if self.current.nid in other_nids:
            QMessageBox.warning(self.window, 'Warning', 'Stat Type ID %s already in use' % self.current.nid)
            self.current.nid = str_utils.get_next_name(self.current.nid, other_nids)
        self.on_nid_changed(self._data.find_key(self.current), self.current.nid)
        self._data.update_nid(self.current, self.current.nid)
        self.window.update_list()
        self.clean_all_stats()

    def name_changed(self, text):
        self.current.name = text
        self.window.update_list()

    def desc_changed(self, text):
        self.current.desc = text

    def position_changed(self, val):
        self.current.position = val

    def maximum_changed(self, val):
        self.current.maximum = val

    def growth_colors_changed(self, state):
        self.current.growth_colors = bool(state)

    def set_current(self, current):
        self.current = current
        self.nid_box.edit.setText(current.nid)
        self.name_box.edit.setText(current.name)
        self.max_box.edit.setValue(current.maximum)
        self.growth_colors_box.edit.setChecked(bool(current.growth_colors))
        self.desc_box.edit.setText(current.desc)
        self.position_box.edit.setValue(current.position)
