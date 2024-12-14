from PyQt5.QtWidgets import QPushButton
from PyQt5.QtCore import QSize
from app.data.resources.resources import RESOURCES

# Custom Widgets
from app.utilities.data import Data
from app.data.database.database import DB

from app.extensions.custom_gui import PropertyBox, ComboBox

class CustomQtRoles():
    # PyQt uses ints 0-14 for its roles.
    # Therefore, for custom roles, we must use 15+
    FilterRole = 15
    UnderlyingDataRole = 16

class ObjBox(PropertyBox[ComboBox]):
    def __init__(self, title, model, database, parent=None, button=False):
        super().__init__(title, ComboBox, parent)
        self.model = model(database, parent)
        self.edit.setModel(self.model)
        if button:
            b = QPushButton('...')
            b.setMaximumWidth(40)
            self.add_button(b)

    def setValue(self, val):
        self.edit.setValue(val)

class UnitBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None, title="Unit"):
        from app.editor.unit_editor.unit_model import UnitModel
        database = DB.units
        if exclude:
            database = Data([d for d in DB.units if d is not exclude])
        super().__init__(title, UnitModel, database, parent, button)
        self.edit.setIconSize(QSize(32, 32))
        self.edit.view().setUniformItemSizes(True)

class ClassBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.class_editor.class_model import ClassModel
        database = DB.classes
        if exclude:
            database = Data([d for d in DB.classes if d is not exclude])
        super().__init__("Class", ClassModel, database, parent, button)
        self.edit.setIconSize(QSize(32, 32))
        self.edit.view().setUniformItemSizes(True)

class TeamBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.team_editor.team_model import TeamModel
        database = DB.teams
        if exclude:
            database = Data([d for d in DB.teams if d is not exclude])
        super().__init__("Team", TeamModel, database, parent, button)

class FactionBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.faction_editor.faction_model import FactionModel
        database = DB.factions
        if exclude:
            database = Data([d for d in DB.factions if d is not exclude])
        super().__init__("Faction", FactionModel, database, parent, button)

class ItemBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.item_editor.item_model import ItemModel
        database = DB.items
        if exclude:
            database = Data([d for d in DB.items if d is not exclude])
        super().__init__("Item", ItemModel, database, parent, button)

class SkillBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.skill_editor.skill_model import SkillModel
        database = DB.skills
        if exclude:
            database = Data([d for d in DB.skills if d is not exclude])
        super().__init__("Skill", SkillModel, database, parent, button)

class AIBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.ai_editor.ai_model import AIModel
        database = Data([d for d in DB.ai if not d.roam_ai])
        if exclude:
            database = Data([d for d in database if d is not exclude])
        super().__init__("Normal AI", AIModel, database, parent, button)

class RoamAIBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.ai_editor.ai_model import AIModel
        database = Data([d for d in DB.ai if d.roam_ai or d.nid == 'None'])
        if exclude:
            database = Data([d for d in database if d is not exclude])
        super().__init__("Roam AI", AIModel, database, parent, button)

class WeaponTypeBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.weapon_editor.weapon_model import WeaponModel
        database = DB.weapons
        if exclude:
            database = Data([d for d in DB.weapons if d is not exclude])
        super().__init__("Weapon Type", WeaponModel, database, parent, button)

class PartyBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.party_editor.party_model import PartyModel
        database = DB.parties
        if exclude:
            database = Data([d for d in DB.parties if d is not exclude])
        super().__init__("Party", PartyModel, database, parent, button)

class AffinityBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.support_editor.affinity_model import AffinityModel
        database = DB.affinities
        if exclude:
            database = Data([d for d in DB.affinities if d is not exclude])
        super().__init__("Affinity Type", AffinityModel, database, parent, button)

class MovementCostBox(ObjBox):
    def __init__(self, parent=None, button=False):
        from app.editor.mcost_dialog import MovementCostModel
        super().__init__("Movement Cost", MovementCostModel, DB.mcost, parent, button)

class MovementClassBox(ObjBox):
    def __init__(self, parent=None, button=False):
        from app.editor.mcost_dialog import MovementClassModel
        super().__init__("Movement Class", MovementClassModel, DB.mcost, parent, button)

class EventBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.event_editor.event_model import EventModel
        database = DB.events
        if exclude:
            database = Data([d for d in DB.events if d is not exclude])
        super().__init__("Event", EventModel, database, parent, button)

class TerrainBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.terrain_editor.terrain_model import TerrainModel
        database = DB.terrain
        if exclude:
            database = Data([d for d in DB.terrain if d is not exclude])
        super().__init__("Terrain Type", TerrainModel, database, parent, button)

class TilemapBox(ObjBox):
    def __init__(self, parent=None, button=False):
        from app.editor.tile_editor.tile_model import TileMapModel
        database = RESOURCES.tilemaps
        super().__init__("Tilemap", TileMapModel, database, parent, button)

class PaletteBox(ObjBox):
    def __init__(self, parent=None, button=False, exclude=None):
        from app.editor.combat_animation_editor.palette_model import PaletteModel
        database = RESOURCES.combat_palettes
        if exclude:
            database = Data([d for d in RESOURCES.combat_palettes if d is not exclude])
        super().__init__("Palette", PaletteModel, database, parent, button)
