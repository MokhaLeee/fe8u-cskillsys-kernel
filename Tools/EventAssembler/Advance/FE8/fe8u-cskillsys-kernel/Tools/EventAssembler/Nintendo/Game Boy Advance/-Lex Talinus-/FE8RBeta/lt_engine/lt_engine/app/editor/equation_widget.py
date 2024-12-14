from PyQt5.QtWidgets import QStyle
from PyQt5.QtCore import Qt

from app.utilities import str_utils
from app.data.database.database import DB

from app.extensions.custom_gui import DeletionTab, DeletionDialog, PropertyBox, ComboBox
from app.extensions.list_dialogs import MultiAttrListDialog
from app.extensions.list_models import MultiAttrListModel

from app.data.database import equations, item_components, skill_components
from app.data.database.components import ComponentType, swap_values

import logging

class EquationMultiModel(MultiAttrListModel):
    descs = {
        "AVOID": "Chance to avoid an enemy's attack",
        "CONSTITUTION": "Offsets weight speed reduction from items",
        "CRIT_ADD": "Flat damage bonus on crits",
        "CRIT_AVOID": "Reduces enemy's crit chance",
        "CRIT_HIT": "Crit chance",
        "CRIT_MULT": "Multiplicative damage bonus on crits, after def/res",
        "DAMAGE": "Base damage with physical weapons",
        "DEFENSE": "Base defense",
        "HEAL": "Base heal formula",
        "HIT": "Base hit rate",
        "HITPOINTS": "Total HP",
        "MAGIC_DAMAGE": "Base damage with magical weapons",
        "MAGIC_DEFENSE": "Base magical defense",
        "MOVEMENT": "Total movement",
        "RATING": "Overall unit rating",
        "RESCUE_AID": "Maximum weight that can be rescued",
        "RESCUE_WEIGHT": "Rescue weight",
        "SPEED_TO_DOUBLE": "Doubling threshold",
        "STEAL_ATK": "Steal ability",
        "STEAL_DEF": "Steal resistance",
        "THRACIA_CRIT": "Multiplicative damage bonus on crits, before def/res",
    }

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.EditRole:
            data = self._data[index.row()]
            attr = self._headers[index.column()]
            return getattr(data, attr)
        if index.column() == 1 and role == Qt.DecorationRole:
            equation = self._data[index.row()]
            good = self.test_equation(equation)
            if good:
                icon = self.window.style().standardIcon(QStyle.SP_DialogApplyButton)
            else:
                icon = self.window.style().standardIcon(QStyle.SP_DialogCancelButton)
            return icon
        elif role == Qt.DisplayRole or role == Qt.EditRole:
            data = self._data[index.row()]
            attr = self._headers[index.column()]
            return getattr(data, attr)
        elif role == Qt.ToolTipRole:
            data = self._data[index.row()]
            if data.nid in self.descs:
                return self.descs[data.nid]
        return None

    def test_equation(self, equation) -> bool:
        try:
            from app.engine import equations as parse
            from app.engine.objects import unit
            test_unit = unit.UnitObject.from_prefab(DB.units[0])
            test_unit.stats = {k: v for (k, v) in test_unit.bases.items()}
            test_unit.stat_bonus = lambda x: 0
            parser = parse.Parser()
            result = parser.get(equation.nid, test_unit)
            result = parser.get_expression(equation.expression, test_unit)
            return True
        except Exception as e:
            logging.exception(e)
            logging.error("TestEquation Error: %s" % e)
            return False

    def delete(self, idx):
        element = self._data[idx]
        affected_items = item_components.get_items_using(ComponentType.Equation, element.nid, DB)
        affected_skills = skill_components.get_skills_using(ComponentType.Equation, element.nid, DB)
        deletion_tabs = []
        if affected_items:
            from app.editor.item_editor.item_model import ItemModel
            model = ItemModel
            msg = "Deleting Equation <b>%s</b> would affect these items" % element.nid
            deletion_tabs.append(DeletionTab(affected_items, model, msg, "Items"))
        if affected_skills:
            from app.editor.skill_editor.skill_model import SkillModel
            model = SkillModel
            msg = "Deleting Equation <b>%s</b> would affect these skills" % element.nid
            deletion_tabs.append(DeletionTab(affected_skills, model, msg, "Skills"))

        if deletion_tabs:
            combo_box = PropertyBox("Equation", ComboBox, self.window)
            objs = [eq for eq in DB.equations if eq.nid != element.nid]
            combo_box.edit.addItems([eq.nid for eq in objs])
            obj_idx, ok = DeletionDialog.get_simple_swap(deletion_tabs, combo_box)
            if ok:
                swap = objs[obj_idx]
                swap_values(affected_items, ComponentType.Equation, element.nid, swap.nid)
                swap_values(affected_skills, ComponentType.Equation, element.nid, swap.nid)
            else:
                return
        super().delete(idx)

    def create_new(self):
        nids = [d.nid for d in self._data]
        nid = str_utils.get_next_name("EQUATION", nids)
        new_equation = equations.Equation(nid)
        DB.equations.append(new_equation)
        return new_equation

    def on_attr_changed(self, data, attr, old_value, new_value):
        if attr == 'nid':
            self._data.update_nid(data, new_value)
            affected_items = item_components.get_items_using(ComponentType.Equation, old_value, DB)
            swap_values(affected_items, ComponentType.Equation, old_value, new_value)
            affected_skills = skill_components.get_skills_using(ComponentType.Equation, old_value, DB)
            swap_values(affected_skills, ComponentType.Equation, old_value, new_value)

class EquationDialog(MultiAttrListDialog):
    locked_vars = {"HIT", "AVOID", "CRIT_HIT", "CRIT_AVOID",
                   "DAMAGE", "DEFENSE", "MAGIC_DAMAGE", "MAGIC_DEFENSE",
                   "HITPOINTS", "MOVEMENT", "THRACIA_CRIT", "CRIT_ADD", "CRIT_MULT",
                   "SPEED_TO_DOUBLE", "STEAL_ATK", "STEAL_DEF",
                   "HEAL", "RESCUE_AID", "RESCUE_WEIGHT", "RATING", "CONSTITUTION"}

    @classmethod
    def create(cls):
        def deletion_func(model, index):
            return model._data[index.row()].nid not in cls.locked_vars

        dlg = cls(DB.equations, "Equation", ("nid", "expression"),
                  EquationMultiModel, (deletion_func, None, deletion_func), cls.locked_vars)
        return dlg

    def accept(self):
        super().accept()
        from app.engine import equations
        equations.clear()

# Testing
# Run "python -m app.editor.equation_widget"
if __name__ == '__main__':
    import sys
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    DB.load('default.ltproj')
    window = EquationDialog.create()
    window.show()
    app.exec_()
