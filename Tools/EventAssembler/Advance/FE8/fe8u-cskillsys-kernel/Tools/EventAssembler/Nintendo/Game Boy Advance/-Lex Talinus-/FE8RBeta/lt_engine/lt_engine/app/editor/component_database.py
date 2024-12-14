from __future__ import annotations
from functools import partial
from typing import Any, Dict

from PyQt5.QtCore import Qt, QSize
from PyQt5.QtGui import QColor, QIcon, QPalette
from PyQt5.QtWidgets import (QApplication, QDoubleSpinBox, QHBoxLayout,
                             QItemDelegate, QLabel, QLineEdit, QListWidgetItem,
                             QSpinBox, QToolButton, QVBoxLayout, QWidget)

from app.data.database.components import ComponentType
from app.data.database.database import DB
from app.data.resources.resources import RESOURCES
from app.editor.component_editor_delegates import (AffinityDelegate,
                                                   ClassDelegate, ItemDelegate,
                                                   SkillDelegate, StatDelegate,
                                                   TagDelegate,
                                                   TerrainDelegate,
                                                   UnitDelegate,
                                                   WeaponTypeDelegate)
from app.editor.component_subcomponent_editors import get_editor_widget
from app.editor.editor_constants import (DROP_DOWN_BUFFER, MAX_DROP_DOWN_WIDTH,
                                         MIN_DROP_DOWN_WIDTH)
from app.extensions import list_models
from app.extensions.color_icon import AlphaColorIcon, ColorIcon
from app.extensions.custom_gui import ComboBox
from app.extensions.key_value_delegate import (FixedKeyMutableValueDelegate,
                                               KeyValueDoubleListModel)
from app.extensions.list_widgets import (AppendMultiListWidget,
                                         AppendSingleListWidget,
                                         BasicMultiListWidget)
from app.extensions.frame_layout import FrameLayout
from app.extensions.widget_list import WidgetList
from app.utilities import utils


class ComponentList(WidgetList):
    def __init__(self, parent):
        super().__init__(parent)
        self.order_swapped.connect(self.rerender)
        palette = QApplication.palette()
        self.bg_color = palette.color(QPalette.Base)
        self.highlight_color = palette.color(QPalette.AlternateBase)

    def add_component(self, component: BoolItemComponent):
        item = QListWidgetItem()
        item.setSizeHint(component.sizeHint())
        self.addItem(item)
        self.setItemWidget(item, component)
        self.index_list.append(component.data.nid)
        if len(self.index_list) % 2 == 0:
            item.setBackground(self.highlight_color)
        else:
            item.setBackground(self.bg_color)
        return item

    def remove_component(self, component):
        if component.data.nid in self.index_list:
            idx = self.index_list.index(component.data.nid)
            self.index_list.remove(component.data.nid)
            return self.takeItem(idx)
        return None

    def updateGeometry(self):
        for idx in range(self.count()):
            item = self.item(idx)
            component = self.itemWidget(item)
            item.setSizeHint(component.sizeHint())
        super().updateGeometry()

    def rerender(self, start, row):
        for idx in range(self.count()):
            item = self.item(idx)
            if idx % 2 == 0:
                item.setBackground(self.highlight_color)
            else:
                item.setBackground(self.bg_color)


class BoolItemComponent(QWidget):
    delegate = None

    def __init__(self, data, parent, delegate=None):
        super().__init__(parent)
        self._data = data
        self.window = parent
        self.delegate = delegate

        hbox = QHBoxLayout()
        hbox.setContentsMargins(0, 0, 0, 0)
        self.setLayout(hbox)

        self.setToolTip(self._data.desc)

        name_label = QLabel(self._data.name)
        hbox.addWidget(name_label)

        self.create_editor(hbox)

        x_button = QToolButton(self)
        x_button.setIcon(QIcon("icons/icons/x.png"))
        x_button.setStyleSheet(
            "QToolButton { border: 0px solid #575757; background-color: palette(base); }")
        x_button.clicked.connect(partial(self.window.remove_component, self))
        hbox.addWidget(x_button, Qt.AlignRight)

    def create_editor(self, hbox):
        pass

    def on_value_changed(self, val):
        self._data.value = val

    @property
    def data(self):
        return self._data


class IntItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = QSpinBox(self)
        self.set_format()
        if self._data.value is None:
            self._data.value = 0
        self.editor.setValue(self._data.value)
        self.editor.valueChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self._data.value = int(val)

    def set_format(self):
        self.editor.setMaximumWidth(60)
        self.editor.setRange(-1000, 10000)


class HitItemComponent(IntItemComponent):
    def set_format(self):
        self.editor.setMaximumWidth(60)
        self.editor.setRange(-1000, 1000)
        self.editor.setSingleStep(5)


class ValueItemComponent(IntItemComponent):
    def set_format(self):
        self.editor.setMaximumWidth(70)
        self.editor.setRange(0, 99999)


class FloatItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = QDoubleSpinBox(self)
        self.editor.setMaximumWidth(60)
        self.editor.setRange(-99, 99)
        self.editor.setSingleStep(.1)
        if self._data.value is None:
            self._data.value = 0
        self.editor.setValue(self._data.value)
        self.editor.valueChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self._data.value = float(val)


class StringItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = QLineEdit(self)
        self.editor.setMaximumWidth(640)
        if not self._data.value:
            self._data.value = ''
        self.editor.setText(str(self._data.value))
        self.editor.textChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, text):
        self._data.value = text


class DropDownItemComponent(BoolItemComponent):
    def __init__(self, data, parent, options):
        self.options = options if options else ['N/A']
        super().__init__(data, parent)

    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        self.editor.setMaximumWidth(320)
        self.editor.addItems(self.options)
        if not self._data.value:
            self._data.value = self.options[0]
        if self._data.value in self.options:
            self.editor.setCurrentIndex(self.options.index(self._data.value))
        else:
            self.editor.setCurrentIndex(0)
        self.editor.currentIndexChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self, val):
        self._data.value = self.options[val]


class DeprecatedOptionsItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        if not self._data.value:
            self._data.value = []
        title = self._data.name
        self.editor = BasicMultiListWidget(self._data.value, title, (
            "Setting", "Value", "Desc"), self.KeyValueDescDelegate, self, model=KeyValueDoubleListModel)
        self.editor.view.setColumnWidth(0, 150)
        self.editor.view.setColumnWidth(1, 50)
        self.editor.view.setColumnWidth(2, 150)
        self.editor.view.setMaximumHeight(75)
        self.editor.model.nid_column = 0
        hbox.addWidget(self.editor)

    class KeyValueDescDelegate(FixedKeyMutableValueDelegate):
        desc_column = 2

        def createEditor(self, parent, option, index):
            if index.column() == self.desc_column:
                editor = QLineEdit(parent)
                editor.setDisabled(True)
                return editor
            else:
                return super().createEditor(parent, option, index)


class BetterOptionsItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        options: Dict[str, ComponentType] = self._data.options
        self.collapsible_frame_layout = FrameLayout(self, "Component Options")
        self.editors_widget = QWidget(self)
        vbox = QVBoxLayout(self.editors_widget)
        # backwards compatability update
        value = self._data.__class__().value.copy()
        value.update(self._data.value)
        self._data.value = value
        for field_name, component_type in options.items():
            editor = get_editor_widget(
                field_name, component_type, self._data.value)
            vbox.addWidget(editor)
        self.editors_widget.resize(self.editors_widget.sizeHint())
        self.collapsible_frame_layout.addWidget(self.editors_widget)
        self.collapsible_frame_layout.clicked.connect(self.updateGeometry)
        self.orig_height = self.height()
        hbox.addWidget(self.collapsible_frame_layout)

    def sizeHint(self):
        if self.collapsible_frame_layout.enabled():
            h = self.collapsible_frame_layout.sizeHint().height()
            return QSize(self.width(), h)
        else:
            return QSize(self.width(), self.orig_height)

    def updateGeometry(self):
        size = self.sizeHint()
        self.setFixedSize(size)
        super().updateGeometry()
        self.window.component_list.updateGeometry()


class WeaponTypeItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        for weapon_type in DB.weapons.values():
            self.editor.addItem(weapon_type.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and DB.weapons:
            self._data.value = DB.weapons[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class WeaponRankItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        for weapon_rank in DB.weapon_ranks.values():
            self.editor.addItem(weapon_rank.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and DB.weapon_ranks:
            self._data.value = DB.weapon_ranks[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class EquationItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        self.editor.addItems(DB.equations.keys())
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and DB.equations:
            self._data.value = DB.equations[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self):
        val = self.editor.currentText()
        self._data.value = val


class Color3ItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        if not self._data.value:
            self._data.value = (0, 0, 0)
        color = self._data.value
        self.color = ColorIcon(QColor(*color), self)
        self.color.set_size(32)
        self.color.colorChanged.connect(self.on_value_changed)
        hbox.addWidget(self.color)

    def on_value_changed(self, val):
        color = self.color.color()
        r, g, b = color.red(), color.green(), color.blue()
        self._data.value = (r, g, b)


class Color4ItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        if not self._data.value:
            self._data.value = (0, 0, 0, 0)
        color = self._data.value
        self.color = AlphaColorIcon(QColor(*color), self)
        self.color.set_size(32)
        self.color.colorChanged.connect(self.on_value_changed)
        hbox.addWidget(self.color)

    def on_value_changed(self, val):
        color = self.color.color()
        r, g, b, a = color.red(), color.green(), color.blue(), color.alpha()
        self._data.value = (r, g, b, a)


class ClassItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        for klass in DB.classes.values():
            self.editor.addItem(klass.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and DB.classes:
            self._data.value = DB.classes[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class AffinityItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        for affinity in DB.affinities.values():
            self.editor.addItem(affinity.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and DB.affinities:
            self._data.value = DB.affinities[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class ItemItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        for item in DB.items.values():
            self.editor.addItem(item.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and DB.items:
            self._data.value = DB.items[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class SkillItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        for skill in DB.skills.values():
            self.editor.addItem(skill.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and DB.skills:
            self._data.value = DB.skills[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class MusicItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        for music in RESOURCES.music.values():
            self.editor.addItem(music.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and RESOURCES.music:
            self._data.value = RESOURCES.music[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class SoundItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        for sound in RESOURCES.sfx.values():
            self.editor.addItem(sound.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and RESOURCES.sfx:
            self._data.value = RESOURCES.sfx[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class MapAnimationItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        for map_anim in RESOURCES.animations.values():
            self.editor.addItem(map_anim.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and RESOURCES.animations:
            self._data.value = RESOURCES.animations[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class EffectAnimationItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        for effect_anim in RESOURCES.combat_effects.values():
            self.editor.addItem(effect_anim.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and RESOURCES.combat_effects:
            self._data.value = RESOURCES.combat_effects[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class AIItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        self.editor.addItems(DB.ai.keys())
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and DB.ai:
            self._data.value = DB.ai[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self):
        val = self.editor.currentText()
        self._data.value = val


class MovementTypeItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        self.editor.addItems(DB.mcost.unit_types)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and DB.mcost.unit_types:
            self._data.value = DB.mcost.unit_types[0]
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self):
        val = self.editor.currentText()
        self._data.value = val


class StatItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        self.editor.addItems(DB.stats.keys())
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and DB.stats:
            self._data.value = DB.stats[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)

    def on_value_changed(self):
        val = self.editor.currentText()
        self._data.value = val


class EventItemComponent(BoolItemComponent):
    def create_editor(self, hbox):
        self.editor = ComboBox(self)
        # Only use global events
        valid_events = [event for event in DB.events.values()
                        if not event.level_nid]
        for event in valid_events:
            self.editor.addItem(event.nid)
        width = utils.clamp(self.editor.minimumSizeHint().width(
        ) + DROP_DOWN_BUFFER, MIN_DROP_DOWN_WIDTH, MAX_DROP_DOWN_WIDTH)
        self.editor.setMaximumWidth(width)
        if not self._data.value and valid_events:
            self._data.value = valid_events[0].nid
        self.editor.setValue(self._data.value)
        self.editor.currentTextChanged.connect(self.on_value_changed)
        hbox.addWidget(self.editor)


class ListItemComponent(BoolItemComponent):
    delegate = None

    def create_editor(self, hbox):
        if not self._data.value:
            self._data.value = []
        self.editor = AppendSingleListWidget(
            self._data.value, self._data.name, self.delegate, self)
        self.editor.view.setColumnWidth(0, 100)
        self.editor.view.setMaximumHeight(75)
        self.editor.model.nid_column = 0

        hbox.addWidget(self.editor)


class DictItemComponent(BoolItemComponent):
    delegate = None

    def create_editor(self, hbox):
        if not self._data.value:
            self._data.value = []
        title = self._data.name
        self.modify_delegate()
        self.editor = AppendMultiListWidget(self._data.value, title, (
            self.delegate.name, "Value"), self.delegate, self, model=list_models.DoubleListModel)
        self.editor.view.setColumnWidth(0, 100)
        self.editor.view.setMaximumHeight(75)
        self.editor.model.nid_column = 0

        hbox.addWidget(self.editor)

    def modify_delegate(self):
        self.delegate.is_float = False
        self.delegate.is_string = False


class FloatDictItemComponent(DictItemComponent):
    def modify_delegate(self):
        self.delegate.is_float = True
        self.delegate.is_string = False


class StringDictItemComponent(DictItemComponent):
    def modify_delegate(self):
        self.delegate.is_float = False
        self.delegate.is_string = True


def get_display_widget(component, parent):
    if not component.expose:
        c = BoolItemComponent(component, parent)
    elif component.expose == ComponentType.Int:
        if component.nid == 'hit':
            c = HitItemComponent(component, parent)
        elif component.nid == 'value':
            c = ValueItemComponent(component, parent)
        else:
            c = IntItemComponent(component, parent)
    elif component.expose == ComponentType.Float:
        c = FloatItemComponent(component, parent)
    elif component.expose == ComponentType.String:
        c = StringItemComponent(component, parent)
    elif component.expose == ComponentType.WeaponType:
        c = WeaponTypeItemComponent(component, parent)
    elif component.expose == ComponentType.WeaponRank:
        c = WeaponRankItemComponent(component, parent)
    elif component.expose == ComponentType.Color3:
        c = Color3ItemComponent(component, parent)
    elif component.expose == ComponentType.Color4:
        c = Color4ItemComponent(component, parent)
    elif component.expose == ComponentType.Class:
        c = ClassItemComponent(component, parent)
    elif component.expose == ComponentType.Affinity:
        c = AffinityItemComponent(component, parent)
    elif component.expose == ComponentType.Item:
        c = ItemItemComponent(component, parent)
    elif component.expose == ComponentType.Skill:
        c = SkillItemComponent(component, parent)
    elif component.expose == ComponentType.MapAnimation:
        c = MapAnimationItemComponent(component, parent)
    elif component.expose == ComponentType.EffectAnimation:
        c = EffectAnimationItemComponent(component, parent)
    elif component.expose == ComponentType.Equation:
        c = EquationItemComponent(component, parent)
    elif component.expose == ComponentType.Music:
        c = MusicItemComponent(component, parent)
    elif component.expose == ComponentType.Sound:
        c = SoundItemComponent(component, parent)
    elif component.expose == ComponentType.AI:
        c = AIItemComponent(component, parent)
    elif component.expose == ComponentType.Stat:
        c = StatItemComponent(component, parent)
    elif component.expose == ComponentType.Event:
        c = EventItemComponent(component, parent)
    elif component.expose == ComponentType.MovementType:
        c = MovementTypeItemComponent(component, parent)
    elif component.expose == ComponentType.MultipleOptions:
        c = DeprecatedOptionsItemComponent(component, parent)
    elif component.expose == ComponentType.NewMultipleOptions:
        c = BetterOptionsItemComponent(component, parent)

    elif isinstance(component.expose, tuple):
        delegate = None
        if component.expose[1] == ComponentType.Unit:
            delegate = UnitDelegate
        elif component.expose[1] == ComponentType.Class:
            delegate = ClassDelegate
        elif component.expose[1] == ComponentType.Affinity:
            delegate = AffinityDelegate
        elif component.expose[1] == ComponentType.Tag:
            delegate = TagDelegate
        elif component.expose[1] == ComponentType.Item:
            delegate = ItemDelegate
        elif component.expose[1] == ComponentType.Stat:
            delegate = StatDelegate
        elif component.expose[1] == ComponentType.WeaponType:
            delegate = WeaponTypeDelegate
        elif component.expose[1] == ComponentType.Skill:
            delegate = SkillDelegate
        elif component.expose[1] == ComponentType.Terrain:
            delegate = TerrainDelegate

        if component.expose[0] == ComponentType.List:
            c = ListItemComponent(component, parent, delegate)
        elif component.expose[0] == ComponentType.Dict:
            c = DictItemComponent(component, parent, delegate)
        elif component.expose[0] == ComponentType.FloatDict:
            c = FloatDictItemComponent(component, parent, delegate)
        elif component.expose[0] == ComponentType.StringDict:
            c = StringDictItemComponent(component, parent, delegate)
        elif component.expose[0] == ComponentType.MultipleChoice:
            c = DropDownItemComponent(component, parent, component.expose[1])

    else:  # TODO
        c = BoolItemComponent(component, parent)
    return c
