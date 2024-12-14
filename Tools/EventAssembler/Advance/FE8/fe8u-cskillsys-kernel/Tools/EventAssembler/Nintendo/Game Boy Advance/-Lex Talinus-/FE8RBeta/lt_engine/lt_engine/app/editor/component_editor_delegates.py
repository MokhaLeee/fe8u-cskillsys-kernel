from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (QDoubleSpinBox, QItemDelegate, QLineEdit,
                             QSpinBox, QWidget)

from app.data.database.database import DB
from app.extensions.custom_gui import ComboBox
from app.utilities.data import Data


class BaseComponentDelegate(QItemDelegate):
    data: Data
    name: str
    is_float = False
    is_string = False

    def createEditor(self, parent, option, index):
        if index.column() == 0:
            editor = ComboBox(parent)
            for obj in self.data:
                name = obj.nid
                if hasattr(obj, 'name'):
                    name = "%s (%s)" % (obj.name, obj.nid)
                editor.addItem(name, obj.nid)
            return editor
        elif index.column() == 1:  # Integer value column
            if self.is_string:
                editor = QLineEdit(parent)
            elif self.is_float:
                editor = QDoubleSpinBox(parent)
                editor.setRange(0, 10)
            else:
                editor = QSpinBox(parent)
                editor.setRange(-1000, 1000)
            return editor
        else:
            return super().createEditor(parent, option, index)

    def setModelData(self, editor: QWidget, model, index) -> None:
        if index.column() == 0: # combobox
            model.setData(index, editor.itemData(editor.currentIndex()), Qt.ItemDataRole)
        else:
            super().setModelData(editor, model, index)

class UnitDelegate(BaseComponentDelegate):
    data = DB.units
    name = "Unit"

class ClassDelegate(BaseComponentDelegate):
    data = DB.classes
    name = "Class"

class AffinityDelegate(BaseComponentDelegate):
    data = DB.affinities
    name = "Affinity"

class TagDelegate(BaseComponentDelegate):
    data = DB.tags
    name = "Tag"

class ItemDelegate(BaseComponentDelegate):
    data = DB.items
    name = "Item"

class StatDelegate(BaseComponentDelegate):
    data = DB.stats
    name = "Stat"

class WeaponTypeDelegate(BaseComponentDelegate):
    data = DB.weapons
    name = "Weapon Type"

class SkillDelegate(BaseComponentDelegate):
    data = DB.skills
    name = "Skill"

class TerrainDelegate(BaseComponentDelegate):
    data = DB.terrain
    name = "Terrain"
