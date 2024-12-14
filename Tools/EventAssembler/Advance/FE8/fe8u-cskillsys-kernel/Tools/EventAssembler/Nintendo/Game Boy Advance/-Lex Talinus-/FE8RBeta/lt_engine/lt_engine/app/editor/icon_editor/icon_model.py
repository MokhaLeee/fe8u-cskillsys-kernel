from app.editor.custom_widgets import CustomQtRoles
import os

from PyQt5.QtWidgets import QFileDialog, QMessageBox
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap, QIcon

from app.utilities import str_utils
from app.data.resources.icons import Icon, IconSheet
from app.data.resources.map_icons import MapIcon, MapIconCatalog
from app.data.resources.resources import RESOURCES
from app.utilities.data import Data
from app.data.database.database import DB
from app.editor.base_database_gui import ResourceCollectionModel
from app.extensions.custom_gui import DeletionTab, DeletionDialog

from app.editor.settings import MainSettingsController
from app.editor.icon_editor import icon_view

class IconModel(ResourceCollectionModel):
    def __init__(self, data, window):
        super().__init__(data, window)
        self.sub_data = Data()
        for icon in self._data:
            new_icons = icon_view.icon_slice(icon, self.width, self.height)
            for i in new_icons:
                self.sub_data.append(i)

    def rowCount(self, parent=None):
        return len(self.sub_data)

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole or role == Qt.EditRole:
            item = self.sub_data[index.row()]
            text = item.nid
            return text
        elif role == Qt.DecorationRole:
            item = self.sub_data[index.row()]
            if item.pixmap:
                pixmap = item.pixmap.scaled(max(self.width, 32), max(self.height, 32))
                return QIcon(pixmap)
        elif role == CustomQtRoles.FilterRole:
            item = self.sub_data[index.row()]
            text = item.parent_nid
            return text
        return None

    def setData(self, index, value, role):
        if not index.isValid():
            return False
        if role == Qt.EditRole:
            item: Icon = self.sub_data[index.row()]
            item.nid = value
            self._data.get(item.parent_nid).set_alias(item.nid, item.icon_index)
        return True

    def flags(self, index):
        if index.isValid():
            return Qt.ItemIsEnabled | Qt.ItemIsSelectable | Qt.ItemIsEditable
        else:
            return Qt.NoItemFlags

    def create_new(self):
        raise NotImplementedError

    def append(self):
        self.create_new()
        view = self.window.view
        # self.dataChanged.emit(self.index(0), self.index(self.rowCount()))
        self.layoutChanged.emit()
        last_index = self.index(self.rowCount() - 1)
        view.setCurrentIndex(last_index)
        return last_index

    def new(self, index):
        item = self.sub_data[index]
        # idx = self._data.index(item.nid)
        # self.create_new()
        # self._data.move_index(len(self._data) - 1, idx + 1)
        # self.layoutChanged.emit()

    def on_nid_changed(self, old_nid, new_nid):
        pass

    def do_delete(self, nid):
        self.layoutAboutToBeChanged.emit()
        for i in self._data:
            if i.nid == nid:
                self._data.delete(i)
        for i in self.sub_data._list[:]:
            if i.nid == nid or i.parent_nid == nid:
                self.sub_data.delete(i)
        self.layoutChanged.emit()

class Icon16Model(IconModel):
    database = RESOURCES.icons16
    width, height = 16, 16

    def create_new(self):
        settings = MainSettingsController()
        starting_path = settings.get_last_open_path()
        fns, ok = QFileDialog.getOpenFileNames(self.window, "Choose %s", starting_path, "PNG Files (*.png);;All Files(*)")
        icon = None
        if ok:
            for fn in fns:
                if fn.endswith('.png'):
                    nid = os.path.split(fn)[-1][:-4]  # Get rid of .png ending
                    pix = QPixmap(fn)
                    if pix.width() % self.width == 0 and pix.height() % self.height == 0:
                        nid = str_utils.get_next_name(nid, [d.nid for d in self.database])
                        icon = IconSheet(nid, fn)
                        icon.pixmap = pix
                        self._data.append(icon)
                        new_icons = icon_view.icon_slice(icon, self.width, self.height)
                        for i in new_icons:
                            self.sub_data.append(i)
                    else:
                        QMessageBox.critical(self.window, "File Size Error!", "Icon width and height must be exactly divisible by %dx%d pixels!" % (self.width, self.height))
                else:
                    QMessageBox.critical(self.window, "File Type Error!", "Icon must be PNG format!")
            parent_dir = os.path.split(fns[-1])[0]
            settings.set_last_open_path(parent_dir)
            self.window.update_list()
        return icon

    def delete(self, idx):
        icon = self.sub_data[idx]
        if icon.parent_nid:
            # Are you sure you want to delete all ?
            nid = icon.parent_nid
        else:
            nid = icon.nid
        # What uses 16x16 icons
        # Items, Skills, Weapons, Affinities
        affected_items = [item for item in DB.items if item.icon_nid == nid]
        affected_skills = [skill for skill in DB.skills if skill.icon_nid == nid]
        affected_weapons = [weapon for weapon in DB.weapons if weapon.icon_nid == nid]
        affected_affinities = [affinity for affinity in DB.affinities if affinity.icon_nid == nid]
        deletion_tabs = []
        if affected_items:
            from app.editor.item_editor.item_model import ItemModel
            model = ItemModel
            msg = "Deleting Icon <b>%s</b> would affect these items." % nid
            deletion_tabs.append(DeletionTab(affected_items, model, msg, "Items"))
        if affected_skills:
            from app.editor.skill_editor.skill_model import SkillModel
            model = SkillModel
            msg = "Deleting Icon <b>%s</b> would affect these skills." % nid
            deletion_tabs.append(DeletionTab(affected_skills, model, msg, "Skills"))
        if affected_weapons:
            from app.editor.weapon_editor.weapon_model import WeaponModel
            model = WeaponModel
            msg = "Deleting Icon <b>%s</b> would affect these weapons." % nid
            deletion_tabs.append(DeletionTab(affected_weapons, model, msg, "Weapons"))
        if affected_affinities:
            from app.editor.support_editor.affinity_model import AffinityModel
            model = AffinityModel
            msg = "Deleting Icon <b>%s</b> would affect these affinities." % nid
            deletion_tabs.append(DeletionTab(affected_affinities, model, msg, "Affinities"))

        if deletion_tabs:
            ok = DeletionDialog.inform(deletion_tabs, self.window)
            if ok:
                pass
            else:
                return

        self.do_delete(nid)

    def on_nid_changed(self, old_nid, new_nid):
        # What uses 16x16 icons
        # Items, Skills, Weapons, Affinities
        for item in DB.items:
            if item.icon_nid == old_nid:
                item.icon_nid = new_nid
        for skill in DB.skills:
            if skill.icon_nid == old_nid:
                skill.icon_nid = new_nid
        for weapon in DB.weapons:
            if weapon.icon_nid == old_nid:
                weapon.icon_nid = new_nid
        for affinity in DB.affinities:
            if affinity.icon_nid == old_nid:
                affinity.icon_nid = new_nid

class Icon32Model(Icon16Model):
    database = RESOURCES.icons32
    width, height = 32, 32

    def delete(self, idx):
        icon = self.sub_data[idx]
        if icon.parent_nid:
            # Are you sure you want to delete all ?
            nid = icon.parent_nid
        else:
            nid = icon.nid
        # What uses 32x32 icons
        # Factions
        affected_factions = [faction for faction in DB.factions if faction.icon_nid == nid]
        if affected_factions:
            from app.editor.faction_editor.faction_model import FactionModel
            model = FactionModel
            msg = "Deleting Icon <b>%s</b> would affect these factions." % nid
            deletion_tab = DeletionTab(affected_factions, model, msg, "Factions")
            ok = DeletionDialog.inform([deletion_tab], self.window)
            if ok:
                pass
            else:
                return

        self.do_delete(nid)

    def on_nid_changed(self, old_nid, new_nid):
        # What uses 32x32 icons
        # Factions
        for faction in DB.factions:
            if faction.icon_nid == old_nid:
                faction.icon_nid = new_nid

class Icon80Model(Icon16Model):
    database = RESOURCES.icons80
    width, height = 80, 72

    def delete(self, idx):
        icon = self.sub_data[idx]
        if icon.parent_nid:
            # Are you sure you want to delete all ?
            nid = icon.parent_nid
        else:
            nid = icon.nid
        # What uses 80x72 icons
        # Classes
        affected_classes = [klass for klass in DB.classes if klass.icon_nid == nid]
        if affected_classes:
            from app.editor.class_editor.class_model import ClassModel
            model = ClassModel
            msg = "Deleting Icon <b>%s</b> would affect these classes." % nid
            deletion_tab = DeletionTab(affected_classes, model, msg, "Classes")
            ok = DeletionDialog.inform([deletion_tab], self.window)
            if ok:
                pass
            else:
                return

        self.do_delete(nid)

    def on_nid_changed(self, old_nid, new_nid):
        # What uses 80x72 icons
        # Classes
        for klass in DB.classes:
            if klass.icon_nid == old_nid:
                klass.icon_nid = new_nid

class MapIconModel(Icon16Model):
    database = RESOURCES.map_icons
    width, height = 48, 48

    def __init__(self, data, window):
        super().__init__(data, window)
        self.sub_data = self._data

    def flags(self, index):
        if index.isValid():
            return Qt.ItemIsEnabled | Qt.ItemIsSelectable
        else:
            return Qt.NoItemFlags

    def create_new(self):
        settings = MainSettingsController()
        starting_path = settings.get_last_open_path()
        fns, ok = QFileDialog.getOpenFileNames(self.window, "Choose %s", starting_path, "PNG Files (*.png);;All Files(*)")
        icon = None
        if ok:
            for fn in fns:
                if fn.endswith('.png'):
                    nid = os.path.split(fn)[-1][:-4]  # Get rid of .png ending
                    pix = QPixmap(fn)
                    if pix.width() % 16 == 0 and pix.height() % 16 == 0 and pix.width() == pix.height():
                        ratio = pix.width() / 16
                        if ratio > 3:
                            QMessageBox.critical(self.window, "File Size Error!", "Map icon width and height for file " + fn + " must be either 16x16, 32x32, or 48x48")
                            continue
                        nid = str_utils.get_next_name(nid, [d.nid for d in self.database])
                        icon = MapIcon(nid, os.path.basename(fn))
                        icon.full_path = fn
                        icon.pixmap = pix
                        self._data.append(icon)
                    else:
                        QMessageBox.critical(self.window, "File Size Error!", "Icon width and height must be exactly divisible by %dx%d pixels!" % (self.width, self.height))
                else:
                    QMessageBox.critical(self.window, "File Type Error!", "Icon must be PNG format!")
            parent_dir = os.path.split(fns[-1])[0]
            settings.set_last_open_path(parent_dir)
            self.window.update_list()
        return icon

    def delete(self, idx):
        icon = self.sub_data[idx]
        nid = icon.nid
        # What uses map icons
        # overworld map nodes
        affected_nodes = []
        affected_node_names = []
        for overworld in DB.overworlds:
            for node in overworld.overworld_nodes:
                if node.icon == nid:
                    affected_nodes.append(node)
                    affected_node_names.append(node.name)
        if affected_nodes:
            affected_node_string = ', '.join(affected_node_names)
            confirmation = QMessageBox.question(self.window, 'Confirmation', 'Deleting this map icon will affect the following nodes: ' + affected_node_string, QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
            if confirmation == QMessageBox.Yes:
                for node in affected_nodes:
                    node.icon = MapIconCatalog.DEFAULT()
                self.do_delete(nid)

    def on_nid_changed(self, old_nid, new_nid):
        # What uses map icons
        # overworld map nodes
        for overworld in DB.overworlds:
            for node in overworld.overworld_nodes:
                if node.nid == old_nid:
                    node.icon = new_nid

    def do_delete(self, nid):
        self.layoutAboutToBeChanged.emit()
        for i in self._data:
            if i.nid == nid:
                self._data.delete(i)
        for i in self.sub_data._list[:]:
            if i.nid == nid:
                self.sub_data.delete(i)
        self.layoutChanged.emit()
