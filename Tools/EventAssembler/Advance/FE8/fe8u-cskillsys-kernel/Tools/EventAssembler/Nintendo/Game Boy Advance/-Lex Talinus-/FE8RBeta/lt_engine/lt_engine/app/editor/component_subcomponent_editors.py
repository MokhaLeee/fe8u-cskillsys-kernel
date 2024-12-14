from __future__ import annotations

from typing import Any, Dict, List, Tuple

from PyQt5.QtWidgets import (QCheckBox, QDoubleSpinBox, QHBoxLayout, QLabel,
                             QLineEdit, QSpinBox, QWidget)

from app.data.database.components import ComponentType
from app.data.database.database import DB
from app.data.resources.resources import RESOURCES
from app.editor.component_editor_delegates import (AffinityDelegate,
                                                   BaseComponentDelegate,
                                                   ClassDelegate, ItemDelegate,
                                                   SkillDelegate, StatDelegate,
                                                   TagDelegate,
                                                   TerrainDelegate,
                                                   UnitDelegate,
                                                   WeaponTypeDelegate)
from app.editor.editor_constants import (DROP_DOWN_BUFFER, MAX_DROP_DOWN_WIDTH,
                                         MIN_DROP_DOWN_WIDTH)
from app.extensions.custom_gui import ComboBox
from app.extensions.list_widgets import AppendSingleListWidget
from app.utilities import str_utils, utils
from app.utilities.typing import override


class BaseSubcomponentEditor(QWidget):
    def __init__(self, field_name: str, option_dict: Dict[str, Any]) -> None:
        super().__init__()
        hbox = QHBoxLayout()
        hbox.setContentsMargins(0, 0, 0, 0)
        self.setLayout(hbox)
        self.field_name = field_name
        self.option_dict = option_dict

        name_label = QLabel(str_utils.snake_to_readable(field_name))
        hbox.addWidget(name_label)

        self._create_editor(hbox)
        self.resize(self.sizeHint())

    @classmethod
    def create(cls: BaseSubcomponentEditor, field_name: str, option_dict: Dict[str, Any]):
        return cls(field_name, option_dict)

    def _create_editor(self, hbox):
        raise NotImplementedError()


class BoolSubcomponentEditor(BaseSubcomponentEditor):
    @override
    def _create_editor(self, hbox):
        editor = QCheckBox(self)
        editor.setChecked(self.option_dict[self.field_name])
        editor.stateChanged.connect(self.on_value_changed)
        hbox.addWidget(editor)

    def on_value_changed(self, val):
        self.option_dict[self.field_name] = bool(val)


class SkillSubcomponentEditor(BaseSubcomponentEditor):
    @override
    def _create_editor(self, hbox):
        self.editor = ComboBox(self)
        for skill in DB.skills.values():
            self.editor.addItem(skill.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self.option_dict.get(self.field_name):
            self.option_dict[self.field_name] = DB.skills[0].nid
        self.editor.setValue(self.option_dict[self.field_name])
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self.option_dict[self.field_name] = val


class IntSubcomponentEditor(BaseSubcomponentEditor):
    @override
    def _create_editor(self, hbox):
        self.editor = QSpinBox(self)
        self.set_format()
        if self.option_dict.get(self.field_name) is None:
            self.option_dict[self.field_name] = 0
        self.editor.setValue(self.option_dict[self.field_name])
        self.editor.valueChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self.option_dict[self.field_name] = int(val)

    def set_format(self):
        self.editor.setMaximumWidth(60)
        self.editor.setRange(-1000, 10000)


class StringSubcomponentEditor(BaseSubcomponentEditor):
    @override
    def _create_editor(self, hbox):
        self.editor = QLineEdit(self)
        self.editor.setMaximumWidth(640)
        if not self.option_dict.get(self.field_name):
            self.option_dict[self.field_name] = ''
        self.editor.setText(str(self.option_dict[self.field_name]))
        self.editor.textChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self.option_dict[self.field_name] = str(val)


class FloatSubcomponentEditor(BaseSubcomponentEditor):
    @override
    def _create_editor(self, hbox):
        self.editor = QDoubleSpinBox(self)
        self.editor.setMaximumWidth(60)
        self.editor.setRange(-99, 99)
        self.editor.setSingleStep(.1)
        if self.option_dict.get(self.field_name) is None:
            self.option_dict[self.field_name] = 0
        self.editor.setValue(self.option_dict[self.field_name])
        self.editor.valueChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self.option_dict[self.field_name] = float(val)


class EventSubcomponentEditor(BaseSubcomponentEditor):
    @override
    def _create_editor(self, hbox):
        self.editor = ComboBox(self)
        # Only use global events
        valid_events = [event for event in DB.events.values()
                        if not event.level_nid]
        for event in valid_events:
            self.editor.addItem(event.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self.option_dict.get(self.field_name) and valid_events:
            self.option_dict[self.field_name] = valid_events[0].nid
        self.editor.setValue(self.option_dict[self.field_name])
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self.option_dict[self.field_name] = val


class ItemSubcomponentEditor(BaseSubcomponentEditor):
    @override
    def _create_editor(self, hbox):
        self.editor = ComboBox(self)
        for item in DB.items.values():
            self.editor.addItem(item.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self.option_dict.get(self.field_name):
            self.option_dict[self.field_name] = DB.items[0].nid
        self.editor.setValue(self.option_dict[self.field_name])
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self.option_dict[self.field_name] = val


class AffinitySubcomponentEditor(BaseSubcomponentEditor):
    @override
    def _create_editor(self, hbox):
        self.editor = ComboBox(self)
        for affinity in DB.affinities.values():
            self.editor.addItem(affinity.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self.option_dict.get(self.field_name):
            self.option_dict[self.field_name] = DB.affinities[0].nid
        self.editor.setValue(self.option_dict[self.field_name])
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self.option_dict[self.field_name] = val


class SoundSubcomponentEditor(BaseSubcomponentEditor):
    @override
    def _create_editor(self, hbox):
        self.editor = ComboBox(self)
        for sound in RESOURCES.sfx.values():
            self.editor.addItem(sound.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self.option_dict.get(self.field_name):
            self.option_dict[self.field_name] = RESOURCES.sfx[0].nid
        self.editor.setValue(self.option_dict[self.field_name])
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self.option_dict[self.field_name] = val


class BaseContainerSubcomponentEditor(BaseSubcomponentEditor):
    def __init__(self, field_name: str, option_dict: Dict[str, Any], delegate: BaseComponentDelegate) -> None:
        self.delegate = delegate
        super().__init__(field_name, option_dict)

    @classmethod
    def create(cls: BaseSubcomponentEditor, field_name: str, option_dict: Dict[str, Any], delegate: BaseComponentDelegate):
        return cls(field_name, option_dict, delegate)


class ListSubcomponentEditor(BaseContainerSubcomponentEditor):
    @override
    def _create_editor(self, hbox):
        if not self.option_dict[self.field_name]:
            self.option_dict[self.field_name] = []
        self.editor = AppendSingleListWidget(
            self.option_dict[self.field_name], str_utils.snake_to_readable(self.field_name), self.delegate, self)
        self.editor.view.setColumnWidth(0, 100)
        self.editor.view.setMinimumHeight(100)
        self.editor.view.setMaximumHeight(150)
        self.editor.model.nid_column = 0
        hbox.addWidget(self.editor)


DELEGATE_MAP: Dict[ComponentType, BaseComponentDelegate] = {
    ComponentType.Unit: UnitDelegate,
    ComponentType.Class: ClassDelegate,
    ComponentType.Affinity: AffinityDelegate,
    ComponentType.Tag: TagDelegate,
    ComponentType.Item: ItemDelegate,
    ComponentType.Stat: StatDelegate,
    ComponentType.WeaponType: WeaponTypeDelegate,
    ComponentType.Skill: SkillDelegate,
    ComponentType.Terrain: TerrainDelegate,
}

EDITOR_MAP: Dict[ComponentType, BaseSubcomponentEditor] = {
    ComponentType.Bool: BoolSubcomponentEditor,
    ComponentType.Skill: SkillSubcomponentEditor,
    ComponentType.Int: IntSubcomponentEditor,
    ComponentType.String: StringSubcomponentEditor,
    ComponentType.Float: FloatSubcomponentEditor,
    ComponentType.Item: ItemSubcomponentEditor,
    ComponentType.Event: EventSubcomponentEditor,
    ComponentType.Sound: SoundSubcomponentEditor,
    ComponentType.Affinity: AffinitySubcomponentEditor
}

CONTAINER_EDITOR_MAP: Dict[ComponentType, BaseContainerSubcomponentEditor] = {
    ComponentType.List: ListSubcomponentEditor
}


class MultipleChoiceSubcomponentEditor(BaseSubcomponentEditor):
    def __init__(self, field_name: str, option_dict: Dict[str, Any], choices: List[str]) -> None:
        self.choices = choices
        super().__init__(field_name, option_dict)

    @override
    def _create_editor(self, hbox):
        self.editor = ComboBox(self)
        for choice in self.choices:
            self.editor.addItem(choice)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self.option_dict.get(self.field_name):
            self.option_dict[self.field_name] = self.choices[0]
        self.editor.setValue(self.option_dict[self.field_name])
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self.option_dict[self.field_name] = val


def get_editor_widget(field_name: str, ctype: ComponentType | Tuple[ComponentType, ComponentType | list], option_dict: Dict[str, Any]):
    if ctype in EDITOR_MAP:
        return EDITOR_MAP[ctype].create(field_name, option_dict)
    elif isinstance(ctype, tuple):  # tuple
        container_type, stype = ctype
        if container_type == ComponentType.MultipleChoice:
            if not isinstance(stype, tuple):
                raise ValueError("Multiple choice has no list arg")
            choices = stype
            return MultipleChoiceSubcomponentEditor(field_name, option_dict, choices)
        elif container_type in CONTAINER_EDITOR_MAP:
            if not isinstance(stype, ComponentType):
                raise ValueError("Container has no subtype")
            delegate = DELEGATE_MAP[stype]
            return CONTAINER_EDITOR_MAP[container_type].create(field_name, option_dict, delegate)
    raise ValueError("Component type not valid")
