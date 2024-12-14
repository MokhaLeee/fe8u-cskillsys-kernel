from PyQt5.QtWidgets import QWidget, QLineEdit, QMessageBox, QHBoxLayout, QVBoxLayout, \
    QSpacerItem, QSizePolicy, QPushButton
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap

from app.data.resources.resources import RESOURCES
from app.data.database.database import DB

from app.editor.combat_animation_editor.palette_model import PaletteModel
from app.editor.icon_editor.icon_view import IconView
from app.editor.lib.components.validated_line_edit import NidLineEdit
from app.editor.map_sprite_editor import map_sprite_model
from app.editor.sound_editor import sound_tab
from app.extensions.custom_gui import ComboBox, PropertyBox
from app.extensions.multi_select_combo_box import MultiSelectComboBox

from app.utilities import str_utils

class TeamProperties(QWidget):
    def __init__(self, parent, current=None):
        super().__init__(parent)
        self.window = parent
        self._data = self.window._data

        self.current = current

        top_section = QHBoxLayout()

        self.frame_view = IconView(self)
        top_section.addWidget(self.frame_view)

        horiz_spacer = QSpacerItem(40, 10, QSizePolicy.Fixed, QSizePolicy.Fixed)
        top_section.addSpacerItem(horiz_spacer)

        top_right_section = QVBoxLayout()

        self.nid_box = PropertyBox("Unique ID", NidLineEdit, self)
        self.nid_box.edit.textChanged.connect(self.nid_changed)
        self.nid_box.edit.editingFinished.connect(self.nid_done_editing)
        top_right_section.addWidget(self.nid_box)

        self.palette_box = PropertyBox("Map Sprite Palette", ComboBox, self)
        model = PaletteModel(RESOURCES.combat_palettes, self)
        self.palette_box.edit.setModel(model)
        self.palette_box.edit.view().setUniformItemSizes(True)
        self.palette_box.edit.activated.connect(self.palette_changed)
        top_right_section.addWidget(self.palette_box)

        top_section.addLayout(top_right_section)

        mid_section = QHBoxLayout()

        self.combat_palette_box = PropertyBox("Combat Palette Variant", QLineEdit, self)
        self.combat_palette_box.edit.editingFinished.connect(self.combat_variant_palette_changed)
        mid_section.addWidget(self.combat_palette_box)

        self.color_box = PropertyBox("Combat UI Color", QLineEdit, self)
        self.color_box.edit.editingFinished.connect(self.color_changed)
        mid_section.addWidget(self.color_box)

        self.sfx_box = PropertyBox("Phase Change Sound Effect", QLineEdit, self)
        self.sfx_box.edit.setReadOnly(True)
        self.sfx_box.add_button(QPushButton('...'))
        self.sfx_box.button.setMaximumWidth(40)
        self.sfx_box.button.clicked.connect(self.change_phase_change_sound_effect)
        self.sfx_box.delete_button = QPushButton('X')
        self.sfx_box.bottom_section.addWidget(self.sfx_box.delete_button)
        self.sfx_box.delete_button.setMaximumWidth(30)
        self.sfx_box.delete_button.clicked.connect(self.delete_phase_change_sound_effect)
        mid_section.addWidget(self.sfx_box)

        self.allies_box = PropertyBox("Allies", MultiSelectComboBox, self)
        self.allies_box.edit.addItems(DB.teams.keys())
        self.allies_box.edit.updated.connect(self.allies_changed)

        total_section = QVBoxLayout()
        self.setLayout(total_section)
        total_section.addLayout(top_section)
        total_section.addLayout(mid_section)
        total_section.addWidget(self.allies_box)

        total_section.setAlignment(Qt.AlignTop)

        if current:
            self.set_current(current)

    def nid_changed(self, text):
        self.current.nid = text
        self.window.update_list()

    def nid_done_editing(self):
        # Check validity of nid!
        other_nids = [d.nid for d in self._data.values() if d is not self.current]
        if self.current.nid in other_nids:
            QMessageBox.warning(self.window, 'Warning', 'Team ID %s already in use' % self.current.nid)
            self.current.nid = str_utils.get_next_name(self.current.nid, other_nids)
        self.window.left_frame.model.on_nid_changed(self._data.find_key(self.current), self.current.nid)
        self._data.update_nid(self.current, self.current.nid)
        self.draw_frame()
        self.window.update_list()

    def palette_changed(self):
        self.current.map_sprite_palette = self.palette_box.edit.currentText()
        self.draw_frame()
        self.window.update_list()

    def combat_variant_palette_changed(self):
        self.current.combat_variant_palette = self.combat_palette_box.edit.text()

    def color_changed(self):
        self.current.combat_color = self.color_box.edit.text()

    def change_phase_change_sound_effect(self):
        res, ok = sound_tab.get_sfx()
        if ok and res:
            nid = res[0].nid
            self.current.phase_change_sound_effect = nid
            self.sfx_box.edit.setText(nid)

    def delete_phase_change_sound_effect(self):
        self.current.phase_change_sound_effect = None
        self.sfx_box.edit.setText('')

    def allies_changed(self):
        DB.teams.set_allies(self.current.nid, self.allies_box.edit.currentText())

    def set_current(self, current):
        self.current = current
        self.nid_box.edit.setText(current.nid)
        self.nid_box.edit.setDisabled(current.nid == 'player')
        self.palette_box.edit.setValue(current.map_sprite_palette)
        self.combat_palette_box.edit.setText(current.combat_variant_palette)
        self.color_box.edit.setText(current.combat_color)
        self.sfx_box.edit.setText(current.phase_change_sound_effect or "")

        allies = DB.teams.get_allies(current.nid)[:] # Must make a copy
        self.allies_box.edit.clear()
        self.allies_box.edit.addItems([k for k in DB.teams.keys() if k != current.nid])
        self.allies_box.edit.setCurrentTexts(allies)

        if self.current:
            self.draw_frame()

    def draw_frame(self):
        first_map_sprite = RESOURCES.map_sprites[0]
        if not first_map_sprite.standing_pixmap:
            first_map_sprite.standing_pixmap = QPixmap(first_map_sprite.stand_full_path)
        pixmap = first_map_sprite.standing_pixmap
        pix = map_sprite_model.get_basic_icon(pixmap, 0, team=self.current.nid)
        self.frame_view.set_image(pix)
        self.frame_view.show_image()
