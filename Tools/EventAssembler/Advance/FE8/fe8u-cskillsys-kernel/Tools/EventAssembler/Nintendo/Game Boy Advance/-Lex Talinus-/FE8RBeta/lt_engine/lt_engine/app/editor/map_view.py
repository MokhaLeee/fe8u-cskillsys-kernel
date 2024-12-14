from enum import Enum

from app.constants import TILEHEIGHT, TILEWIDTH, WINHEIGHT, WINWIDTH
from app.data.database.database import DB
from app.data.database.levels import LevelPrefab
from app.data.resources.tiles import TileMapPrefab
from app.editor import tilemap_editor, timer
from app.editor.class_editor import class_model
from app.editor.overworld_editor.road_sprite_wrapper import RoadSpriteWrapper
from app.editor.settings import MainSettingsController
from app.data.resources.resources import RESOURCES
from app.sprites import SPRITES
from app.utilities import utils
from PyQt5.QtCore import Qt, pyqtSignal
from PyQt5.QtGui import QBrush, QColor, QPainter, QPixmap
from PyQt5.QtWidgets import QGraphicsScene, QGraphicsView

class SimpleMapView(QGraphicsView):
    min_scale = 1
    max_scale = 4
    position_clicked = pyqtSignal(int, int)
    position_right_clicked = pyqtSignal(int, int)
    position_double_clicked = pyqtSignal(int, int)
    position_double_right_clicked = pyqtSignal(int, int)
    position_moved = pyqtSignal(int, int)

    position_clicked_float = pyqtSignal(float, float)

    def __init__(self, window=None):
        super().__init__()
        self.main_editor = window
        self.settings = MainSettingsController()
        self.scene = QGraphicsScene(self)
        self.setScene(self.scene)
        self.setMouseTracking(True)

        self.setMinimumSize(WINWIDTH, WINHEIGHT)
        self.setStyleSheet("background-color:rgb(128, 128, 128);")

        self.current_level = None
        self.current_map = None
        self.pixmap = None
        self.screen_scale = 1

        self.working_image = None

        self.current_mouse_pos = None
        self.region_select = None

        self.old_middle_pos = None

    def center_on_pos(self, pos):
        self.centerOn(pos[0]*TILEWIDTH, pos[1]*TILEHEIGHT)
        self.update_view()

    def set_current_level(self, level: LevelPrefab):
        self.current_level = level
        if level:
            self.current_map = RESOURCES.tilemaps.get(level.tilemap)
        self.update_view()

    def set_current_map(self, tilemap: TileMapPrefab):
        self.current_level = None
        self.current_map = tilemap
        self.update_view()

    def clear_scene(self):
        self.scene.clear()

    def update_view(self, _=None):
        if(self.current_level and not self.current_map):
            self.current_map = RESOURCES.tilemaps.get(
                self.current_level.tilemap)
        if self.current_map:
            pixmap = tilemap_editor.get_tilemap_pixmap(self.current_map)
            self.working_image = pixmap
        else:
            self.clear_scene()
            return
        if self.current_level:
            self.paint_units(self.current_level)
        self.show_map()

    def draw_unit(self, painter, unit, position, opacity=False):
        # Draw unit map sprite
        klass_nid = unit.klass
        num = timer.get_timer().passive_counter.count
        klass = DB.classes.get(klass_nid)
        if not klass:
            klass = DB.classes[0]
        pixmap = class_model.get_map_sprite_icon(
            klass, num, False, unit.team, unit.variant)
        coord = position
        if pixmap:
            if opacity:
                painter.setOpacity(0.33)
            painter.drawImage(int(coord[0] * TILEWIDTH - 9),
                              int(coord[1] * TILEHEIGHT - 8), pixmap.toImage())
            painter.setOpacity(1.0)
        else:
            pass  # TODO: for now  # Need a fallback option... CITIZEN??

    def paint_units(self, current_level: LevelPrefab):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            drawn_units = [unit for unit in current_level.units if unit.starting_position]
            for unit in sorted(drawn_units, key=lambda unit: tuple(unit.starting_position)):
                if not unit.starting_position:
                    continue
                if unit.starting_traveler:
                    partner = current_level.units.get(unit.starting_traveler)
                    if partner:
                        pos = unit.starting_position
                        self.draw_unit(painter, partner, (pos[0] + 0.1875, pos[1] - 0.1875))
                        self.draw_unit(painter, unit, (pos[0] - 0.1875, pos[1] + 0.1875))
                    else:
                        self.draw_unit(painter, unit, unit.starting_position)
                else:
                    self.draw_unit(painter, unit, unit.starting_position)
            painter.end()

    def show_map(self):
        if self.working_image:
            self.clear_scene()
            self.scene.addPixmap(self.working_image)
            self.scene.setSceneRect(0, 0, self.working_image.width(), self.working_image.height())
            # first_item = self.items()[0]
            # self.centerOn(first_item)

    def mousePressEvent(self, event):
        super().mousePressEvent(event)
        scene_pos = self.mapToScene(event.pos())
        pos = int(scene_pos.x() / TILEWIDTH), int(scene_pos.y() / TILEHEIGHT)
        # since all items on a map are centered in their cell, the true float coordinate should be offset to account for that
        pos_float = float(scene_pos.x() / TILEWIDTH - 0.5), float(scene_pos.y() / TILEHEIGHT - 0.5)

        if self.current_map and self.current_map.check_bounds(pos):
            if(event.buttons() == Qt.RightButton):
                self.position_right_clicked.emit(*pos)
            else:
                self.position_clicked.emit(*pos)
                self.position_clicked_float.emit(*pos_float)

        if event.button() == Qt.MiddleButton:
            self.old_middle_pos = event.pos()

    def mouseDoubleClickEvent(self, event):
        super().mouseDoubleClickEvent(event)
        scene_pos = self.mapToScene(event.pos())
        pos = int(scene_pos.x() / TILEWIDTH), int(scene_pos.y() / TILEHEIGHT)

        if self.current_map and self.current_map.check_bounds(pos):
            if event.buttons() == Qt.RightButton:
                self.position_double_right_clicked.emit(*pos)
            else:
                self.position_double_clicked.emit(*pos)

    def mouseMoveEvent(self, event):
        super().mouseMoveEvent(event)
        scene_pos = self.mapToScene(event.pos())
        pos = int(scene_pos.x() / TILEWIDTH), int(scene_pos.y() / TILEHEIGHT)

        if self.current_map and self.current_map.check_bounds(pos):
            self.position_moved.emit(*pos)
            self.current_mouse_pos = pos
        else:
            self.position_moved.emit(-1, -1)
            self.current_mouse_pos = None

        if event.buttons() & Qt.MiddleButton:
            offset = self.old_middle_pos - event.pos()
            self.old_middle_pos = event.pos()

            self.verticalScrollBar().setValue(self.verticalScrollBar().value() + offset.y())
            self.horizontalScrollBar().setValue(self.horizontalScrollBar().value() + offset.x())

    def zoom_in(self):
        if self.screen_scale < self.max_scale:
            self.screen_scale += 1
            self.scale(2, 2)

    def zoom_out(self):
        if self.screen_scale > self.min_scale:
            self.screen_scale -= 1
            self.scale(0.5, 0.5)

    def wheelEvent(self, event):
        if event.angleDelta().y() > 0:
            self.zoom_in()
        elif event.angleDelta().y() < 0:
            self.zoom_out()


class GlobalModeLevelMapView(SimpleMapView):
    def __init__(self, window=None):
        super().__init__(window)
        self.overworld_flag: bool = False
        self.road_sprite = RoadSpriteWrapper()

    def set_current_level(self, nid, overworld=False):
        self.overworld_flag = overworld
        super().set_current_level(nid)

    def paint_units(self):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            if self.current_level:
                drawn_units = [unit for unit in self.current_level.units if unit.starting_position]
                for unit in sorted(drawn_units, key=lambda unit: tuple(unit.starting_position)):
                    if not unit.starting_position:
                        continue
                    if unit.generic or unit.nid in DB.units.keys():
                        if unit.starting_traveler:
                            partner = self.current_level.units.get(unit.starting_traveler)
                            if partner:
                                pos = unit.starting_position
                                self.draw_unit(painter, partner, (pos[0] + 0.1875, pos[1] - 0.1875))
                                self.draw_unit(painter, unit, (pos[0] - 0.1875, pos[1] + 0.1875))
                            else:
                                self.draw_unit(painter, unit, unit.starting_position)
                        else:
                            self.draw_unit(painter, unit, unit.starting_position)

    def update_view(self, _=None):
        if self.current_level and not self.current_map:
            self.current_map = RESOURCES.tilemaps.get(
                self.current_level.tilemap)
        if self.current_map:
            pixmap = tilemap_editor.get_tilemap_pixmap(self.current_map)
            self.working_image = pixmap
        else:
            self.clear_scene()
            return
        if not self.overworld_flag:
            self.paint_units()
        if self.overworld_flag:
            self.paint_roads(self.current_level)
            self.paint_nodes(self.current_level)
        self.show_map()

    def draw_node(self, painter, node, position, opacity=False):
        icon_nid = node.icon
        icon = RESOURCES.map_icons.get(icon_nid)
        if not icon:
            return
        coord = position
        pixmap = icon.get_pixmap()
        # to support 16x16, 32x32, and 48x48 map icons, we offset them differently
        offset_x = int((pixmap.width() / 16 - 1) * 8)
        offset_y = (pixmap.height() - 16)
        if pixmap:
            if opacity:
                painter.setOpacity(0.33)
            painter.drawImage(coord[0] * TILEWIDTH - offset_x,
                              coord[1] * TILEHEIGHT - offset_y, pixmap.toImage())
            painter.setOpacity(1.0)
        else:
            pass

    def paint_nodes(self, current_level):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            for node in current_level.overworld_nodes:
                if not node.pos:
                    continue
                self.draw_node(painter, node, node.pos)
            painter.end()

    def paint_roads(self, current_level):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            for path in current_level.map_paths.values():
                unpacked_path = RoadSpriteWrapper.road_to_full_points_list(path)
                for i in range(len(unpacked_path)):
                    neighbors = []
                    if i != 0:
                        neighbors.append(unpacked_path[i - 1])
                    if i < len(unpacked_path) - 1:
                        neighbors.append(unpacked_path[i + 1])
                    self.road_sprite.draw_tile(painter, unpacked_path[i], neighbors)
            painter.end()

    def mouseMoveEvent(self, event):
        super().mouseMoveEvent(event)
        scene_pos = self.mapToScene(event.pos())
        pos = int(scene_pos.x() / TILEWIDTH), int(scene_pos.y() / TILEHEIGHT)

        if not self.overworld_flag and self.current_map and self.current_map.check_bounds(pos):
            self.current_mouse_pos = pos
            self.main_editor.set_position_bar(pos)
            terrain_nid = self.current_map.get_base_terrain(pos)
            terrain = DB.terrain.get(terrain_nid)
            if terrain:
                self.main_editor.set_message(
                    "%s: %s" % (terrain.nid, terrain.name))
            else:
                self.main_editor.set_message(None)
        else:
            self.current_mouse_pos = None
            self.main_editor.set_position_bar(None)
            self.main_editor.set_message(None)


class EditMode(Enum):
    NONE = 0
    REGIONS = 1
    GROUPS = 2
    UNITS = 3


class NewMapView(SimpleMapView):
    def __init__(self, window=None):
        super().__init__(window)
        self.edit_mode = EditMode.UNITS

    def set_mode(self, edit_mode):
        self.edit_mode = edit_mode

    def check_position(self, level_prefab, pos):
        for unit in level_prefab.units:
            if unit.starting_position and \
                    unit.starting_position[0] == pos[0] and \
                    unit.starting_position[1] == pos[1]:
                return unit
        return None

    def update_view(self, _=None):
        if(self.current_level):
            self.current_map = RESOURCES.tilemaps.get(
                self.current_level.tilemap)
        if self.current_map:
            pixmap = tilemap_editor.get_tilemap_pixmap(self.current_map)
            self.working_image = pixmap
        else:
            self.clear_scene()
            return
        if self.edit_mode == EditMode.NONE:
            self.paint_units(in_unit_tab=False)
        elif self.edit_mode == EditMode.UNITS:
            self.paint_units()
        elif self.edit_mode == EditMode.REGIONS:
            self.paint_regions()
            self.paint_units(in_unit_tab=False)
        elif self.edit_mode == EditMode.GROUPS:
            self.paint_groups()
        else:
            self.paint_units(in_unit_tab=False)
        self.show_map()

    def paint_units(self, in_unit_tab=True):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            drawn_units = [unit for unit in self.current_level.units if unit.starting_position]
            for unit in sorted(drawn_units, key=lambda unit: tuple(unit.starting_position)):
                if not unit.starting_position:
                    continue
                if unit.generic or unit.nid in DB.units.keys():
                    if unit.starting_traveler:
                        partner = self.current_level.units.get(unit.starting_traveler)
                        if partner:
                            pos = unit.starting_position
                            self.draw_unit(painter, partner, (pos[0] + 0.1875, pos[1] - 0.1875))
                            self.draw_unit(painter, unit, (pos[0] - 0.1875, pos[1] + 0.1875))
                        else:
                            self.draw_unit(painter, unit, unit.starting_position)
                    else:
                        self.draw_unit(painter, unit, unit.starting_position)
            # Highlight current unit with cursor
            current_unit = self.main_editor.unit_painter_menu.get_current()
            if in_unit_tab and current_unit:
                if current_unit.starting_position:
                    coord = current_unit.starting_position
                    cursor_sprite = SPRITES['cursor']
                    if cursor_sprite:
                        if not cursor_sprite.pixmap:
                            cursor_sprite.pixmap = QPixmap(cursor_sprite.full_path)
                        cursor_image = cursor_sprite.pixmap.toImage().copy(0, 64, 32, 32)
                        painter.drawImage(
                            coord[0] * TILEWIDTH - 8, coord[1] * TILEHEIGHT - 5, cursor_image)
                if self.current_mouse_pos:
                    coord = self.current_mouse_pos
                    self.draw_unit(painter, current_unit, coord, opacity=True)
            painter.end()

    def paint_groups(self):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            for group in self.main_editor.current_level.unit_groups:
                for unit_nid in group.units:
                    position = group.positions.get(unit_nid)
                    if not position:
                        continue
                    unit = self.main_editor.current_level.units.get(unit_nid)
                    if unit:
                        self.draw_unit(painter, unit, position, opacity=True)
            # Draw current group
            current_group = self.main_editor.group_painter_menu.get_current()
            if current_group:
                for unit_nid in current_group.units:
                    position = current_group.positions.get(unit_nid)
                    if not position:
                        continue
                    # With full opacity
                    unit = self.main_editor.current_level.units.get(unit_nid)
                    if unit:
                        self.draw_unit(painter, unit, position)

                # Highlight current unit with cursor
                current_unit = self.main_editor.group_painter_menu.get_current_unit()
                if current_unit and current_unit.nid in current_group.positions:
                    coord = current_group.positions.get(current_unit.nid)
                    cursor_sprite = SPRITES['cursor']
                    if coord and cursor_sprite:
                        if not cursor_sprite.pixmap:
                            cursor_sprite.pixmap = QPixmap(
                                cursor_sprite.full_path)
                        cursor_image = cursor_sprite.pixmap.toImage().copy(0, 64, 32, 32)
                        painter.drawImage(
                            coord[0] * TILEWIDTH - 8, coord[1] * TILEHEIGHT - 5, cursor_image)
            painter.end()

    def paint_regions(self):
        if self.working_image:
            painter = QPainter()
            painter.begin(self.working_image)
            for region in self.current_level.regions:
                if not region.position:
                    continue
                x, y = region.position
                width, height = region.size
                color = utils.hash_to_color(utils.strhash(region.nid))
                pixmap = QPixmap(width * TILEWIDTH, height * TILEHEIGHT)
                pixmap.fill(QColor(*color))
                if region.area == 1:
                    painter.setOpacity(0.75)
                else:
                    # Makes bigger regions less opaque
                    o = 20 - min(20, region.area)
                    painter.setOpacity(0.25 + o * .025)
                painter.drawImage(x * TILEWIDTH, y *
                                  TILEHEIGHT, pixmap.toImage())
            current_region = self.main_editor.region_painter_menu.get_current()
            if current_region:
                if current_region.position:
                    x, y = current_region.position
                    width, height = current_region.size
                    painter.setBrush(Qt.NoBrush)
                    painter.setPen(Qt.yellow)
                    painter.setOpacity(0.75)
                    painter.drawRect(x * TILEWIDTH, y * TILEHEIGHT,
                                     width * TILEWIDTH, height * TILEHEIGHT)
                elif self.region_select and self.current_mouse_pos:
                    left = min(self.region_select[0],
                               self.current_mouse_pos[0])
                    right = max(
                        self.region_select[0], self.current_mouse_pos[0])
                    top = min(self.region_select[1], self.current_mouse_pos[1])
                    bottom = max(
                        self.region_select[1], self.current_mouse_pos[1])
                    width = right - left + 1
                    height = bottom - top + 1
                    color = utils.hash_to_color(
                        utils.strhash(current_region.nid))
                    # painter.setBrush(Qt.DiagCrossPattern)
                    # painter.setPen()
                    painter.setOpacity(0.75)
                    painter.fillRect(left * TILEWIDTH, top * TILEHEIGHT, width * TILEWIDTH,
                                     height * TILEHEIGHT, QBrush(QColor(*color), Qt.DiagCrossPattern))
            painter.end()

    def mousePressEvent(self, event):
        super().mousePressEvent(event)
        scene_pos = self.mapToScene(event.pos())
        pos = int(scene_pos.x() / TILEWIDTH), int(scene_pos.y() / TILEHEIGHT)

        if self.current_map and self.current_map.check_bounds(pos):
            # Units
            if self.edit_mode == EditMode.UNITS:
                if event.button() == self.settings.get_place_button(Qt.RightButton):
                    current_unit = self.main_editor.unit_painter_menu.get_current()
                    if current_unit:
                        under_unit = self.check_position(
                            self.current_level, pos)
                        if under_unit:
                            under_unit.starting_position = None
                        if under_unit is current_unit:
                            message = "Removed unit %s from map" % (
                                current_unit.nid)
                            self.main_editor.set_message(message)
                        elif current_unit.starting_position:
                            current_unit.starting_position = pos
                            message = "Moved unit %s to (%d, %d)" % (
                                current_unit.nid, pos[0], pos[1])
                            self.main_editor.set_message(message)
                        else:
                            current_unit.starting_position = pos
                            message = "Placed unit %s at (%d, %d)" % (
                                current_unit.nid, pos[0], pos[1])
                            # Remove from pairup
                            for unit in self.current_level.units:
                                if unit.starting_traveler == current_unit.nid:
                                    unit.starting_traveler = None
                            self.main_editor.set_message(message)
                        self.update_view()
                elif event.button() == self.settings.get_select_button(Qt.LeftButton):
                    under_unit = self.check_position(self.current_level, pos)
                    if under_unit:
                        idx = self.current_level.units.index(under_unit.nid)
                        self.main_editor.unit_painter_menu.select(idx)
                    else:
                        self.main_editor.unit_painter_menu.deselect()
            # Groups
            elif self.edit_mode == EditMode.GROUPS:
                if event.button() == self.settings.get_place_button(Qt.RightButton):
                    current_group = self.main_editor.group_painter_menu.get_current()
                    current_unit = self.main_editor.group_painter_menu.get_current_unit()
                    if current_unit:
                        if current_group.positions.get(current_unit.nid) == pos:
                            del current_group.positions[current_unit.nid]
                            message = "Removing unit %s from map" % (
                                current_unit.nid)
                        else:
                            current_group.positions[current_unit.nid] = pos
                            message = "Group %s unit %s's position to (%d, %d)" % (
                                current_group.nid, current_unit.nid, pos[0], pos[1])
                        self.main_editor.set_message(message)
                        self.update_view()
                elif event.button() == self.settings.get_select_button(Qt.LeftButton):
                    current_group = self.main_editor.group_painter_menu.get_current()
                    under_unit_nid = None
                    if current_group:
                        for unit_nid, position in current_group.positions.items():
                            if pos == position:
                                under_unit_nid = unit_nid
                                break
                    for group in self.main_editor.current_level.unit_groups:
                        if under_unit_nid:
                            break
                        for unit_nid, position in group.positions.items():
                            if pos == position:
                                current_group = group
                                under_unit_nid = unit_nid
                                break
                    if under_unit_nid:
                        self.main_editor.group_painter_menu.select(current_group, under_unit_nid)
                    else:
                        self.main_editor.group_painter_menu.deselect()
            elif self.edit_mode == EditMode.REGIONS:
                if event.button() == self.settings.get_place_button(Qt.RightButton):
                    current_region = self.main_editor.region_painter_menu.get_current()
                    if current_region:
                        # Remove position for current region if it has one
                        current_region.position = None
                        self.region_select = pos

    def mouseMoveEvent(self, event):
        super().mouseMoveEvent(event)
        scene_pos = self.mapToScene(event.pos())
        pos = int(scene_pos.x() / TILEWIDTH), int(scene_pos.y() / TILEHEIGHT)

        if self.current_map and self.current_map.check_bounds(pos):
            self.current_mouse_pos = pos
            self.main_editor.set_position_bar(pos)
            under_unit = self.check_position(self.current_level, pos)
            current_group = self.main_editor.group_painter_menu.get_current()
            group_unit_nid = None
            if current_group:
                for unit_nid in current_group.units:
                    my_pos = current_group.positions.get(unit_nid)
                    if my_pos and my_pos[0] == pos[0] and my_pos[1] == pos[1]:
                        group_unit_nid = unit_nid
                        break

            if self.edit_mode == EditMode.REGIONS:
                current_region = None
                for region in self.current_level.regions:
                    if region.position and region.contains(pos):
                        current_region = region
                        break
                if current_region:
                    self.main_editor.set_message(
                        "Region ID: %s" % current_region.nid)
                else:
                    self.main_editor.set_message(None)
            elif self.edit_mode == EditMode.UNITS and under_unit:
                self.main_editor.set_message("Unit: %s" % under_unit.nid)
            elif self.edit_mode == EditMode.GROUPS and group_unit_nid:
                self.main_editor.set_message("Unit: %s" % group_unit_nid)
            else:
                terrain_nid = self.current_map.get_base_terrain(pos)
                terrain = DB.terrain.get(terrain_nid)
                if terrain:
                    self.main_editor.set_message(
                        "%s: %s" % (terrain.nid, terrain.name))
                else:
                    self.current_mouse_pos = None
                    self.main_editor.set_message(None)
        else:
            self.main_editor.set_position_bar(None)
            self.main_editor.set_message(None)

    def mouseReleaseEvent(self, event):
        scene_pos = self.mapToScene(event.pos())
        pos = int(scene_pos.x() / TILEWIDTH), int(scene_pos.y() / TILEHEIGHT)

        if self.current_map and self.current_map.check_bounds(pos):
            if self.region_select and self.edit_mode == EditMode.REGIONS:
                if event.button() == self.settings.get_place_button(Qt.RightButton):
                    current_region = self.main_editor.region_painter_menu.get_current()
                    if current_region:
                        prev_pos = self.region_select
                        top = min(prev_pos[1], pos[1])
                        left = min(prev_pos[0], pos[0])
                        right = max(prev_pos[0], pos[0])
                        bottom = max(prev_pos[1], pos[1])
                        width = right - left
                        height = bottom - top
                        current_region.position = (left, top)
                        current_region.size = [width + 1, height + 1]
                    self.region_select = None

    def keyPressEvent(self, event):
        super().keyPressEvent(event)
        if self.edit_mode == EditMode.UNITS:
            if event.key() == Qt.Key_Delete:
                unit_painter_menu = self.main_editor.unit_painter_menu
                indices = unit_painter_menu.view.selectionModel().selectedIndexes()
                for index in indices:
                    unit_painter_menu.model.delete(index.row())
