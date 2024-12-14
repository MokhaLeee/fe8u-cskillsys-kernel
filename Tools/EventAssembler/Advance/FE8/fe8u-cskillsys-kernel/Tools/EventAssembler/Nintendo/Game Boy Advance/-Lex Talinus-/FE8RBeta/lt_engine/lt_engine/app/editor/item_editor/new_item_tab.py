from __future__ import annotations

import os
from typing import Optional

from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QFileDialog

import app.engine.item_component_access as ICA
from app.data.database import item_components, skill_components
from app.data.database.items import ItemCatalog, ItemPrefab
from app.data.database.components import swap_values, ComponentType
from app.editor import timer
from app.editor.data_editor import SingleDatabaseEditor
from app.editor.item_editor import item_import, item_model
from app.editor.component_editor_properties import NewComponentProperties
from app.editor.settings.main_settings_controller import MainSettingsController
from app.editor.new_editor_tab import NewEditorTab
from app.extensions.custom_gui import DeletionTab, DeletionDialog
from app.editor.custom_widgets import ItemBox

from app.utilities.typing import NID


class NewItemProperties(NewComponentProperties[ItemPrefab]):
    title = "Item"
    get_components = staticmethod(ICA.get_item_components)
    get_templates = staticmethod(ICA.get_templates)
    get_tags = staticmethod(ICA.get_item_tags)


class NewItemDatabase(NewEditorTab):
    catalog_type = ItemCatalog
    properties_type = NewItemProperties

    @classmethod
    def edit(cls, parent=None):
        timer.get_timer().stop_for_editor()  # Don't need these while running game
        window = SingleDatabaseEditor(NewItemDatabase, parent)
        window.exec_()
        timer.get_timer().start_for_editor()

    @property
    def data(self):
        return self._db.items

    def get_icon(self, item_nid) -> Optional[QIcon]:
        pix = item_model.get_pixmap(self.data.get(item_nid))
        if pix:
            return QIcon(pix.scaled(32, 32))
        return None

    def _on_delete(self, nid: NID) -> bool:
        """
        Returns whether the user wants to proceed with deletion
        """
        item = self.data.get(nid)
        affected_units = [unit for unit in self._db.units if nid in unit.get_items()]
        affected_levels = [level for level in self._db.levels if any(nid in unit.get_items() for unit in level.units)]
        affected_items = item_components.get_items_using(ComponentType.Item, nid, self._db)
        affected_skills = skill_components.get_skills_using(ComponentType.Item, nid, self._db)

        deletion_tabs = []
        if affected_units:
            from app.editor.unit_editor.unit_model import UnitModel
            model = UnitModel
            msg = "Deleting Item <b>%s</b> would affect these units." % nid
            deletion_tabs.append(DeletionTab(affected_units, model, msg, "Units"))
        if affected_levels:
            from app.editor.global_editor.level_menu import LevelModel
            model = LevelModel
            msg = "Deleting Item <b>%s</b> would affect units in these levels." % nid
            deletion_tabs.append(DeletionTab(affected_levels, model, msg, "Levels"))
        if affected_items:
            from app.editor.item_editor.item_model import ItemModel
            model = ItemModel
            msg = "Deleting Item <b>%s</b> would affect these items" % nid
            deletion_tabs.append(DeletionTab(affected_items, model, msg, "Items"))
        if affected_skills:
            from app.editor.skill_editor.skill_model import SkillModel
            model = SkillModel
            msg = "Deleting Item <b>%s</b> would affect these skills" % nid
            deletion_tabs.append(DeletionTab(affected_skills, model, msg, "Skills"))

        if deletion_tabs:
            swap, ok = DeletionDialog.get_swap(deletion_tabs, ItemBox(self, exclude=item), self)
            if ok:
                self._on_nid_changed(nid, swap.nid)
            else:
                return False
        return True

    def _on_nid_changed(self, old_nid: NID, new_nid: NID) -> None:
        for unit in self._db.units:
            unit.replace_item_nid(old_nid, new_nid)
        for level in self._db.levels:
            for unit in level.units:
                unit.replace_item_nid(old_nid, new_nid)
        swap_values(self._db.items.values(), ComponentType.Item, old_nid, new_nid)
        swap_values(self._db.skills.values(), ComponentType.Item, old_nid, new_nid)

    def import_xml(self):
        settings = MainSettingsController()
        starting_path = settings.get_last_open_path()
        fn, ok = QFileDialog.getOpenFileName(self, _("Import items from items.xml"), starting_path, "Items XML (items.xml);;All Files(*)")
        if ok and fn.endswith('items.xml'):
            parent_dir = os.path.split(fn)[0]
            settings.set_last_open_path(parent_dir)
            new_items = item_import.get_from_xml(parent_dir, fn)
            for item in new_items:
                self.data.append(item)
            self.reset()

    def import_csv(self):
        settings = MainSettingsController()
        starting_path = settings.get_last_open_path()
        fn, ok = QFileDialog.getOpenFileName(self, _("Import items from csv"), starting_path, "items csv (*.csv);;All Files(*)")
        if ok and fn:
            parent_dir = os.path.split(fn)[0]
            settings.set_last_open_path(parent_dir)
            item_import.update_db_from_csv(self._db, fn)
            self.reset()

# Testing
# Run "python -m app.editor.item_editor.new_item_tab" from main directory
if __name__ == '__main__':
    import sys
    from app.data.database.database import DB
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    from app.data.resources.resources import RESOURCES
    DB.load('default.ltproj')
    RESOURCES.load('default.ltproj')
    window = NewItemDatabase(None, DB)
    window.show()
    app.exec_()
