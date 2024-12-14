from PyQt5.QtWidgets import QWidget, QLabel, QLineEdit, \
    QMessageBox, QSpinBox, QHBoxLayout, QGroupBox, QRadioButton, \
    QVBoxLayout, QComboBox, QStackedWidget, QDoubleSpinBox, QCheckBox, \
    QGridLayout, QListWidget, QListWidgetItem, QPushButton
from PyQt5.QtCore import Qt

import app.data.database.ai as ai
from app.data.database.database import DB

from app.extensions.custom_gui import PropertyBox, ComboBox, PropertyCheckBox
from app.editor.custom_widgets import ClassBox, UnitBox, TeamBox, FactionBox, PartyBox
from app.editor.lib.components.validated_line_edit import NidLineEdit
from app.utilities import str_utils

# Target Specifications
class NullSpecification(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        self.layout = QHBoxLayout()

        self.setLayout(self.layout)

    def set_current(self, target_spec):
        pass

class UnitSpecification(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        self.layout = QGridLayout()
        self.box1 = ComboBox(self)
        for spec in ai.unit_spec:
            self.box1.addItem(spec)
        self.box1.activated.connect(self.unit_spec_changed)

        self.box2 = QStackedWidget(self)
        all_box = ComboBox(self)
        all_box.setEnabled(False)
        self.box2.addWidget(all_box)
        class_box = ClassBox(self)
        class_box.edit.activated.connect(self.sub_spec_changed)
        self.box2.addWidget(class_box)
        tag_box = ComboBox(self)
        tag_box.addItems([tag.nid for tag in DB.tags])
        tag_box.activated.connect(self.sub_spec_changed)
        self.box2.addWidget(tag_box)
        name_box = ComboBox(self)
        name_box.addItems([unit.name for unit in DB.units])
        name_box.activated.connect(self.sub_spec_changed)
        self.box2.addWidget(name_box)
        team_box = TeamBox(self)
        team_box.edit.activated.connect(self.sub_spec_changed)
        self.box2.addWidget(team_box)
        faction_box = FactionBox(self)
        faction_box.edit.activated.connect(self.sub_spec_changed)
        self.box2.addWidget(faction_box)
        party_box = PartyBox(self)
        party_box.edit.activated.connect(self.sub_spec_changed)
        self.box2.addWidget(party_box)
        unit_box = UnitBox(self)
        unit_box.edit.activated.connect(self.sub_spec_changed)
        self.box2.addWidget(unit_box)

        self.except_check_box = QCheckBox(self)
        self.except_check_box.setText("Except")
        self.except_check_box.toggled.connect(self.check_box_toggled)
        self.except_check_box.setEnabled(False)

        self.layout.addWidget(self.except_check_box, 0, 0)
        self.layout.addWidget(self.box1, 1, 0)
        self.layout.addWidget(self.box2, 0, 1, 2, 1)

        self.setLayout(self.layout)

    def unit_spec_changed(self, recurse=True):
        unit_spec = self.box1.currentText()
        self.box2.setEnabled(True)
        self.except_check_box.setEnabled(True)
        if unit_spec == "Class":
            self.box2.setCurrentIndex(1)
            if recurse:
                self.sub_spec_changed()
        elif unit_spec == "Tag":
            self.box2.setCurrentIndex(2)
            if recurse:
                self.sub_spec_changed()
        elif unit_spec == "Name":
            self.box2.setCurrentIndex(3)
            if recurse:
                self.sub_spec_changed()
        elif unit_spec == "Team":
            self.box2.setCurrentIndex(4)
            if recurse:
                self.sub_spec_changed()
        elif unit_spec == "Faction":
            self.box2.setCurrentIndex(5)
            if recurse:
                self.sub_spec_changed()
        elif unit_spec == "Party":
            self.box2.setCurrentIndex(6)
            if recurse:
                self.sub_spec_changed()
        elif unit_spec == "ID":
            self.box2.setCurrentIndex(7)
            if recurse:
                self.sub_spec_changed()
        else:
            self.box2.setCurrentIndex(0)
            self.box2.setEnabled(False)
            self.except_check_box.setEnabled(False)
            self.window.current.target_spec = None

    def sub_spec_changed(self):
        unit_spec = self.box1.currentText()
        if self.box2.currentIndex() in (0, 2, 3):
            sub_spec = self.box2.currentWidget().currentText()
        else:
            sub_spec = self.box2.currentWidget().edit.currentText()
        self.window.current.target_spec = (unit_spec, sub_spec)

    def check_box_toggled(self, checked):
        self.window.current.invert_targeting = bool(checked)

    def set_current(self, target_spec):
        self.except_check_box.setChecked(bool(self.window.current.invert_targeting))
        try:
            self.box1.setValue(str(target_spec[0]))
            self.unit_spec_changed(recurse=False)
            self.box2.currentWidget().setValue(target_spec[1])
        except Exception:  # Target spec is not compatible
            self.box1.setValue("All")
            self.unit_spec_changed(recurse=False)
            self.except_check_box.setEnabled(False)

class EventSpecification(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        self.layout = QHBoxLayout()
        self.box = QLineEdit(self)
        self.box.setPlaceholderText("Event Region Type")
        self.box.textChanged.connect(self.spec_changed)

        self.layout.addWidget(self.box)
        self.setLayout(self.layout)

    def spec_changed(self, text):
        event = self.box.text()
        self.window.current.target_spec = event

    def set_current(self, target_spec):
        try:
            self.box.setText(target_spec)
        except: # spec isn't compatible
            pass

class WaitSpecification(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        self.layout = QHBoxLayout()
        self.box = QSpinBox()
        self.box.setMinimumWidth(40)
        self.box.setRange(0, 10000000)
        self.box.valueChanged.connect(self.spec_changed)

        self.layout.addWidget(self.box)
        self.setLayout(self.layout)

    def spec_changed(self, text):
        time = self.box.value()
        self.window.current.target_spec = time

    def set_current(self, target_spec):
        try:
            self.box.setValue(target_spec)
        except: # spec isn't compatible
            pass

class PositionSpecification(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        self.layout = QVBoxLayout()
        self.starting = QRadioButton("Starting", self)
        self.starting.toggled.connect(self.starting_toggled)
        self.custom = QRadioButton("Custom", self)

        bottom = QHBoxLayout()
        bottom.addWidget(self.custom)
        self.x_spinbox = QSpinBox()
        self.y_spinbox = QSpinBox()
        self.x_spinbox.setMinimumWidth(40)
        self.y_spinbox.setMinimumWidth(40)
        self.x_spinbox.setRange(0, 255)
        self.y_spinbox.setRange(0, 255)
        self.x_spinbox.setEnabled(False)
        self.y_spinbox.setEnabled(False)
        self.x_spinbox.valueChanged.connect(self.change_spinbox)
        self.y_spinbox.valueChanged.connect(self.change_spinbox)
        bottom.addWidget(self.x_spinbox)
        bottom.addWidget(self.y_spinbox)

        self.layout.addWidget(self.starting)
        self.layout.addLayout(bottom)

        self.setLayout(self.layout)

    def starting_toggled(self, checked):
        if checked:
            self.x_spinbox.setEnabled(False)
            self.y_spinbox.setEnabled(False)
            self.window.current.target_spec = "Starting"
        else:
            self.x_spinbox.setEnabled(True)
            self.y_spinbox.setEnabled(True)
            x, y = int(self.x_spinbox.value()), int(self.y_spinbox.value())
            self.window.current.target_spec = (x, y)

    def change_spinbox(self, value):
        x, y = int(self.x_spinbox.value()), int(self.y_spinbox.value())
        self.window.current.target_spec = (x, y)

    def set_current(self, target_spec):
        if target_spec == "Starting":
            self.window.current.target_spec = "Starting"
            self.starting.setChecked(True)
        elif target_spec:
            self.starting.setChecked(False)
            self.x_spinbox.setValue(int(target_spec[0]))
            self.y_spinbox.setValue(int(target_spec[1]))
        else:
            self.starting.setChecked(False)
            self.x_spinbox.setValue(0)
            self.y_spinbox.setValue(0)

class BehaviourBox(QGroupBox):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.window = parent

        self.current = None

        self.layout = QHBoxLayout()

        self.action = ComboBox(self)
        for action in ai.AI_ActionTypes:
            self.action.addItem(action.replace('_', ' '))
        self.action.currentIndexChanged.connect(self.action_changed)

        self.target = ComboBox(self)
        for target in ai.AI_TargetTypes:
            self.target.addItem(target)
        self.target.currentIndexChanged.connect(self.target_changed)
        self.target.setEnabled(False)

        self.target_spec = QStackedWidget(self)
        for target in ai.AI_TargetTypes:
            if target == "None":
                target_spec = NullSpecification(self)
            elif target in ("Unit", "Ally", "Enemy"):
                target_spec = UnitSpecification(self)
            elif target == "Position":
                target_spec = PositionSpecification(self)
            elif target == "Event":
                target_spec = EventSpecification(self)
            elif target == "Time":
                target_spec = WaitSpecification(self)
            self.target_spec.addWidget(target_spec)

        self.view_range = ComboBox(self)
        self.view_range.setInsertPolicy(QComboBox.NoInsert)
        self.view_range.addItem("Max Item Range")
        self.view_range.addItem("Movement + Max Item Range")
        self.view_range.addItem("Movement*2 + Max Item Range")
        self.view_range.addItem("Entire Map")
        self.view_range.addItem("Custom Integer")
        self.view_range.currentIndexChanged.connect(self.check_view_range)

        self.custom_view_range = QSpinBox(self)
        self.custom_view_range.setMaximum(255)
        self.custom_view_range.editingFinished.connect(self.check_view_range)

        self.speed_box = PropertyBox("Roam Speed", QSpinBox, self)
        self.speed_box.setToolTip("How fast the unit moves in Free Roam. Divided by 100 and multiplies the default speed")
        self.speed_box.edit.setRange(0, 1000)
        self.speed_box.edit.setAlignment(Qt.AlignRight)
        self.speed_box.edit.valueChanged.connect(self.set_roam_speed)

        self.proximity_box = PropertyBox("Desired Proximity", QSpinBox, self)
        self.proximity_box.setToolTip("When within this distance of a target, move to the next behaviour.\nSet to -1 to never change behaviour.")
        self.proximity_box.edit.setRange(-1, 255)
        self.proximity_box.edit.setAlignment(Qt.AlignRight)
        self.proximity_box.edit.valueChanged.connect(self.set_desired_proximity)

        self.condition_box = PropertyBox("Condition", QLineEdit, self)
        self.condition_box.setToolTip("If Condition is false, behaviour is skipped.")
        self.condition_box.edit.setMaximumWidth(200)
        self.condition_box.edit.textChanged.connect(self.set_condition)

        self.within_label = QLabel(" within ")

        left_layout = QGridLayout()
        left_layout.addWidget(self.action, 0, 0)
        left_layout.addWidget(self.target, 0, 1)
        left_layout.addWidget(self.condition_box, 1, 0, 1, 2)

        self.layout.addLayout(left_layout)
        self.layout.addWidget(self.target_spec)
        self.layout.addWidget(self.speed_box)
        self.layout.addWidget(self.proximity_box)

        self.show_range()

        self.show_roam_info(False)

        self.custom_view_range.hide()
        self.setLayout(self.layout)

    def show_range(self):
        if isinstance(self.target_spec.currentWidget(), WaitSpecification):
            self.within_label.setParent(None)
            self.view_range.setParent(None)
            self.custom_view_range.setParent(None)
            self.layout.removeWidget(self.within_label)
            self.layout.removeWidget(self.view_range)
            self.layout.removeWidget(self.custom_view_range)
        else:
            self.layout.addWidget(self.within_label)
            self.layout.addWidget(self.view_range)
            self.layout.addWidget(self.custom_view_range)

    def set_roam_speed(self, val):
        self.current.roam_speed = int(val)

    def set_desired_proximity(self, val):
        self.current.desired_proximity = int(val)

    def set_condition(self, val: str):
        self.current.condition = val

    def show_roam_info(self, enable: bool):
        if enable and self.target_spec and not isinstance(self.target_spec.currentWidget(), WaitSpecification):
            self.speed_box.show()
            self.proximity_box.show()
        else:
            self.speed_box.hide()
            self.proximity_box.hide()

    def action_changed(self, index):
        action = self.action.currentText().replace(' ', '_')
        self.current.action = action

        if self.current.action in ('Move_to', 'Move_away_from'):
            self.target.setEnabled(True)
            if self.current.target != "Time":
                self.target.setValue(self.current.target)
            else:
                self.target.setValue("None")
            target_spec = self.target_spec.currentWidget()
            target_spec.set_current(self.current.target_spec)
        elif self.current.action == 'None':
            self.target.setEnabled(False)
            self.target.setValue('None')
        elif self.current.action in ('Attack', 'Steal'):
            self.target.setEnabled(False)
            self.target.setValue('Enemy')
            target_spec = self.target_spec.currentWidget()
            target_spec.set_current(self.current.target_spec)
        elif self.current.action == 'Support':
            self.target.setEnabled(False)
            self.target.setValue('Ally')
            target_spec = self.target_spec.currentWidget()
            target_spec.set_current(self.current.target_spec)
        elif self.current.action == 'Interact':
            self.target.setEnabled(False)
            self.target.setValue('Event')
            target_spec = self.target_spec.currentWidget()
            target_spec.set_current(self.current.target_spec)
        elif self.current.action == 'Wait':
            self.target.setEnabled(False)
            self.target.setValue('Time')
            target_spec = self.target_spec.currentWidget()
            target_spec.set_current(self.current.target_spec)

        self.show_range()

    def target_changed(self, index):
        target = self.target.currentText()
        self.current.target = target
        # Swap the specification
        idx = ai.AI_TargetTypes.index(target)
        self.target_spec.setCurrentIndex(idx)

    def check_view_range(self):
        cur_val = self.view_range.currentText()
        if cur_val == 'Custom Integer' and self.view_range.isVisible():
            self.custom_view_range.show()
            self.current.view_range = int(self.custom_view_range.value())
        else:
            self.custom_view_range.hide()
            self.current.view_range = -1 * (self.view_range.currentIndex() + 1)

    def set_current(self, behaviour):
        self.current = behaviour
        action = behaviour.action.replace('_', ' ')
        self.action.setValue(action)
        self.action_changed(None)

        if self.speed_box:
            self.speed_box.edit.setValue(behaviour.roam_speed)
        if self.proximity_box:
            self.proximity_box.edit.setValue(behaviour.desired_proximity)

        if behaviour.view_range < 0:
            correct_index = -behaviour.view_range - 1
            self.view_range.setCurrentIndex(correct_index)
        else:
            self.custom_view_range.setValue(int(behaviour.view_range))
            self.view_range.setCurrentIndex(4)

        if behaviour.condition:
            self.condition_box.edit.setText(behaviour.condition)
        else:
            self.condition_box.edit.setText("")

class AIProperties(QWidget):
    def __init__(self, parent, current=None):
        super().__init__(parent)
        self.window = parent
        self.model = self.window.left_frame.model
        self._data = self.window._data
        self.database_editor = self.window.window

        self.current = current

        self.top_section = QHBoxLayout()

        self.nid_box = PropertyBox("Unique ID", NidLineEdit, self)
        self.nid_box.edit.textChanged.connect(self.nid_changed)
        self.nid_box.edit.editingFinished.connect(self.nid_done_editing)
        self.top_section.addWidget(self.nid_box)

        self.priority_box = PropertyBox("Priority", QSpinBox, self)
        self.priority_box.setToolTip("Higher priority AIs move first")
        self.priority_box.edit.setRange(0, 255)
        self.priority_box.edit.setAlignment(Qt.AlignRight)
        self.priority_box.edit.valueChanged.connect(self.priority_changed)
        self.top_section.addWidget(self.priority_box)

        self.offense_bias_box = PropertyBox("Offense Bias", QDoubleSpinBox, self)
        self.offense_bias_box.setToolTip("Higher offense AIs weigh damage dealt over their own survival")
        self.offense_bias_box.edit.setRange(0.01, 100)
        self.offense_bias_box.edit.setSingleStep(0.2)
        self.offense_bias_box.edit.setAlignment(Qt.AlignRight)
        self.offense_bias_box.edit.valueChanged.connect(self.offense_bias_changed)
        self.top_section.addWidget(self.offense_bias_box)

        self.roam_ai_box = PropertyCheckBox("Roam AI", QCheckBox, self)
        self.roam_ai_box.edit.stateChanged.connect(self.roam_ai)
        self.roam_ai_box.setToolTip("Is this AI for Free Roam or normal gameplay?")
        self.top_section.addWidget(self.roam_ai_box)

        self.add_button = QPushButton("Add Behaviour", self)
        self.add_button.clicked.connect(self.add_behaviour)
        self.top_section.addWidget(self.add_button)

        self.remove_button = QPushButton("Remove Behaviour", self)
        self.remove_button.clicked.connect(self.pop_behaviour)
        self.top_section.addWidget(self.remove_button)

        main_section = QVBoxLayout()

        self.behaviour_boxes = []
        self.behaviour_list = QListWidget()

        main_section.addWidget(self.behaviour_list)

        total_section = QVBoxLayout()
        total_section.addLayout(self.top_section)
        total_section.addLayout(main_section)
        self.setLayout(total_section)

        self.construct_roam_info()

    def construct_roam_info(self):
        if self.current:
            for b in self.behaviour_boxes:
                b.show_roam_info(self.current.roam_ai)

    def nid_changed(self, text):
        self.current.nid = text
        self.window.update_list()

    def nid_done_editing(self):
        # Check validity of nid!
        other_nids = [d.nid for d in self._data.values() if d is not self.current]
        if self.current.nid in other_nids:
            QMessageBox.warning(self.window, 'Warning', 'AI ID %s already in use' % self.current.nid)
            self.current.nid = str_utils.get_next_name(self.current.nid, other_nids)
        self.model.on_nid_changed(self._data.find_key(self.current), self.current.nid)
        self._data.update_nid(self.current, self.current.nid)
        self.window.update_list()

    def priority_changed(self, val):
        self.current.priority = int(val)

    def offense_bias_changed(self, val):
        self.current.offense_bias = float(val)

    def pop_behaviour(self, checked=False, set_current=True):
        idx = len(self.behaviour_boxes)
        self.behaviour_list.takeItem(idx - 1)
        self.behaviour_boxes.pop()

        if self.current and set_current:
            self.current.pop_behaviour()

    def roam_ai(self, state):
        self.current.roam_ai = bool(state)
        self.construct_roam_info()

    def add_behaviour(self, checked=False, set_current=True):
        item = QListWidgetItem(self.behaviour_list)
        self.behaviour_list.addItem(item)

        idx = len(self.behaviour_boxes) + 1

        behaviour_box = BehaviourBox(self)
        behaviour_box.setTitle("Behaviour %d" % idx)
        item.setSizeHint(behaviour_box.minimumSizeHint())

        self.behaviour_list.setItemWidget(item, behaviour_box)
        self.behaviour_boxes.append(behaviour_box)

        self.construct_roam_info()

        if self.current and set_current:
            self.current.add_default()
            behaviour_box.set_current(self.current.behaviours[-1])

        self.construct_roam_info()

    def set_current(self, current):
        self.current = current
        self.nid_box.edit.setText(current.nid)
        self.priority_box.edit.setValue(current.priority)
        self.offense_bias_box.edit.setValue(current.offense_bias)
        self.roam_ai_box.edit.setChecked(bool(current.roam_ai))
        num_behaviours = len(current.behaviours)
        # Remove all after num behaviours
        while len(self.behaviour_boxes) > num_behaviours:
            self.pop_behaviour(set_current=False)
        while len(self.behaviour_boxes) < num_behaviours:
            self.add_behaviour(set_current=False)
        # Now set the rest to this
        for idx, behaviour in enumerate(current.behaviours):
            self.behaviour_boxes[idx].set_current(behaviour)

        self.construct_roam_info()
