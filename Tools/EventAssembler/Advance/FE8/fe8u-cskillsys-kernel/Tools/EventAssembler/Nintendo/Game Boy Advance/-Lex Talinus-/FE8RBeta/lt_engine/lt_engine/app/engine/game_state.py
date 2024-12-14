from __future__ import annotations
from functools import lru_cache

import random
import time
from collections import Counter
from typing import TYPE_CHECKING, Dict, List, Optional, Tuple

from app.engine.query_engine import GameQueryEngine

if TYPE_CHECKING:
    from app.engine import (ai_controller, death,
        game_board, highlight, map_view, phase,
        promotion, ui_view, banner, boundary, camera, cursor,
        initiative, records, supports, turnwheel, unit_sprite)
    from app.engine.combat.simple_combat import SimpleCombat
    from app.engine.movement import movement_system
    from app.engine.overworld.overworld_movement_manager import \
        OverworldMovementManager
    from app.engine.overworld.overworld_manager import OverworldManager
    from app.engine.objects.difficulty_mode import DifficultyModeObject
    from app.engine.objects.item import ItemObject
    from app.engine.objects.level import LevelObject
    from app.engine.objects.overworld import OverworldObject
    from app.engine.objects.party import PartyObject
    from app.engine.objects.skill import SkillObject
    from app.engine.objects.unit import UnitObject
    from app.engine.objects.region import RegionObject
    from app.engine.objects.ai_group import AIGroupObject
    from app.engine.dialog_log import DialogLog
    from app.events.event_manager import EventManager
    from app.engine.target_system import TargetSystem
    from app.engine.pathfinding.path_system import PathSystem
    from app.utilities.typing import NID, UID

from app.constants import VERSION
from app.data.database.database import DB
from app.data.database.difficulty_modes import GrowthOption, PermadeathOption
from app.events.regions import RegionType
from app.events import speak_style
from app.engine import config as cf
from app.engine import state_machine
from app.engine.roam.roam_info import RoamInfo
from app.utilities import static_random
from app.data.resources.resources import RESOURCES
from app.engine.source_type import SourceType

import logging

class GameState():
    def __init__(self):
        # define all GameState properties
        self.memory: Dict = {}

        self.state: state_machine.StateMachine = state_machine.StateMachine()

        self.alerts: List[banner.Banner] = []

        self.current_mode: DifficultyModeObject = None

        # global variable stores
        self.game_vars: Counter = None
        self.level_vars: Counter = None
        self.playtime: int = 0
        self.current_save_slot: int = None

        # global registries
        self.unit_registry: Dict[NID, UnitObject] = {}
        self.item_registry: Dict[UID, ItemObject] = {}
        self.skill_registry: Dict[UID, SkillObject] = {}
        self.terrain_status_registry: Dict[Tuple[int, int, NID], UID] = {}
        self.region_registry: Dict[NID, RegionObject] = {}
        self.overworld_registry: Dict[NID, OverworldObject] = {}
        self.parties: Dict[NID, PartyObject] = {}
        self.unlocked_lore: List[NID] = []
        self.dialog_log: DialogLog = None
        self.already_triggered_events: List[NID] = []
        self.market_items: Dict[NID, int] = {}  # Any negative number is infinite stock

        # global controllers
        self.supports: supports.SupportController = None
        self.records: records.Recordkeeper = None
        self.speak_styles: speak_style.SpeakStyleLibrary = None
        self.query_engine: GameQueryEngine = GameQueryEngine(logging.Logger('query_engine'), self)

        # 'current' state information, typically varies by level
        self.current_level: LevelObject = None
        self._current_party: NID = None
        self.turncount: int = 0
        self.roam_info: RoamInfo = RoamInfo()
        self.talk_options: List[Tuple[NID, NID]] = []
        self.base_convos: Dict[NID, bool] = {}
        self.action_log: turnwheel.ActionLog = None
        self.events: EventManager = None
        self.map_sprite_registry: Dict[NID, unit_sprite.MapSprite] = {}

        # current-level controllers and game objects
        self.board: game_board.GameBoard = None
        self.cursor: cursor.BaseCursor = None
        self.camera: camera.Camera = None
        self.boundary: boundary.BoundaryInterface = None
        self.phase: phase.PhaseController = None
        self.highlight: highlight.HighlightController = None
        self.initiative: initiative.InitiativeTracker = None
        self.map_view: map_view.MapView = None
        self.movement: movement_system.MovementSystem | OverworldMovementManager = None
        self.death: death.DeathManager = None
        self.ui_view: ui_view.UIView = None
        self.combat_instance: List[SimpleCombat] = []
        self.exp_instance: List[Tuple[UnitObject, int, promotion.PromotionState, str]] = []
        self.mana_instance: List[Tuple[UnitObject, int]] = []
        self.ai: ai_controller.AIController = None
        self.overworld_controller: OverworldManager = None

        self.target_system: TargetSystem = None
        self.path_system: PathSystem = None

        self.clear()

    def on_alter_game_state(self):
        from app.engine import skill_system
        skill_system.reset_cache()

    def is_displaying_overworld(self) -> bool:
        from app.engine.overworld.overworld_map_view import OverworldMapView
        return isinstance(self.map_view, OverworldMapView)

    def clear(self):
        self.game_vars = Counter()
        self.memory = {}

        self.state = state_machine.StateMachine()

        self.playtime = 0

        self.alerts = []
        self.cursor = None
        self.camera = None
        self.boundary = None
        self.movement = None

        self.current_save_slot = None
        self.current_level = None
        self.roam_info.clear()

        self.speak_styles = speak_style.SpeakStyleLibrary()

    def load_states(self, starting_states):
        self.state.load_states(starting_states)

    # Start a new game
    # When the player clicks "New Game"
    def build_new(self):
        from app.engine import records, supports
        logging.info("Building New Game")
        self.playtime = 0

        self.unit_registry = {}
        self.item_registry = {}
        self.skill_registry = {}
        self.terrain_status_registry = {}
        self.region_registry = {}

        self.current_mode = self.default_mode()

        self.parties = {}
        self.current_party = None
        self.current_level = None
        self.roam_info.clear()
        self.game_vars.clear()

        # Set up random seed
        if cf.SETTINGS['random_seed'] >= 0:
            random_seed = int(cf.SETTINGS['random_seed'])
        else:
            random_seed = random.randint(0, 1023)
        static_random.set_seed(random_seed)
        self.game_vars['_random_seed'] = random_seed

        # initialize all parties
        from app.engine.objects.party import PartyObject
        for party_prefab in DB.parties.values():
            nid, name, leader = party_prefab.nid, party_prefab.name, party_prefab.leader
            self.parties[nid] = PartyObject(nid, name, leader)

        # Initialize all overworlds and enter them into the registry
        from app.engine.objects.overworld import OverworldObject
        for overworld in DB.overworlds.values():
            self.overworld_registry[overworld.nid] = OverworldObject.from_prefab(overworld, self.parties, self.unit_registry)
        self.supports = supports.SupportController()
        self.records = records.Recordkeeper()
        self.market_items = {}
        self.unlocked_lore = []
        from app.engine.dialog_log import DialogLog
        self.dialog_log = DialogLog()
        self.already_triggered_events = []
        self.sweep()
        self.generic()

    def sweep(self):
        """
        Cleans up variables that need to be reset at the end of each level
        """
        from app.engine import turnwheel
        from app.events import event_manager
        from app.engine.dialog_log import DialogLog

        self.level_vars = Counter()
        self.turncount = 0
        self.talk_options = []
        self.base_convos = {}
        self.action_log = turnwheel.ActionLog()
        self.events = event_manager.EventManager()
        if not self.dialog_log:
            self.dialog_log = DialogLog()
        self.dialog_log.clear()

    def generic(self):
        """
        Done on loading a level, whether from overworld, last level, save_state, etc.
        """
        from app.engine import (ai_controller, camera, death, highlight,
                                map_view, phase, ui_view)
        from app.engine.movement import movement_system
        from app.engine.target_system import TargetSystem
        from app.engine.pathfinding.path_system import PathSystem

        # Systems
        self.camera = camera.Camera(self)
        self.phase = phase.PhaseController()
        self.highlight = highlight.HighlightController()
        self.map_view = map_view.MapView()
        self.movement = movement_system.MovementSystem(self.cursor, self.camera)
        self.death = death.DeathManager()
        self.ui_view = ui_view.UIView()
        self.combat_instance = []
        self.exp_instance = []
        self.mana_instance = []
        self.ai = ai_controller.AIController()
        self.target_system = TargetSystem()
        self.path_system = PathSystem()

        self.alerts.clear()

        # Build registries
        self.map_sprite_registry = {}

        self.get_region_under_pos.cache_clear()

    def level_setup(self):
        from app.engine.initiative import InitiativeTracker
        from app.engine import action

        # Build party object for new parties
        if self.current_party not in self.parties:
            self.build_party(self.current_party)

        # Assign every unit the levels party if they don't already have one
        for unit in self.current_level.units:
            if not unit.party:
                unit.party = self.current_party
        self.set_up_game_board(self.current_level.tilemap)

        for region in self.current_level.regions:
            self.register_region(region)

        # The fog and vision regions affect the game board
        for region in self.current_level.regions:
            if region.region_type == RegionType.FOG:
                action.AddFogRegion(region).execute()
            elif region.region_type == RegionType.VISION:
                action.AddVisionRegion(region).execute()

        for unit in self.current_level.units:
            self.full_register(unit)
        for unit in self.current_level.units:
            # Only let unit's that have a VALID position spawn onto the map
            if unit.position:
                if self.current_level.tilemap.check_bounds(unit.position):
                    self.arrive(unit)
                else:
                    logging.warning("Unit %s's position not on map. Removing...", unit.nid)
                    unit.position = None

        # Handle initiative
        if DB.constants.value('initiative'):
            self.initiative = InitiativeTracker()
            self.initiative.start(self.get_all_units())

    def start_level(self, level_nid, with_party=None):
        """
        Done at the beginning of a new level to start the level up
        """
        self.boundary = None
        self.generic()
        logging.debug("Starting Level %s", level_nid)

        from app.engine.level_cursor import LevelCursor
        from app.engine.objects.level import LevelObject
        from app.engine.objects.tilemap import TileMapObject

        level_nid = str(level_nid)
        level_prefab = DB.levels.get(level_nid)
        tilemap_nid = level_prefab.tilemap
        tilemap_prefab = RESOURCES.tilemaps.get(tilemap_nid)
        tilemap = TileMapObject.from_prefab(tilemap_prefab)
        bg_tilemap = TileMapObject.from_prefab(RESOURCES.tilemaps.get(level_prefab.bg_tilemap)) if level_prefab.bg_tilemap else None
        self.cursor = LevelCursor(self)
        self.current_level = LevelObject.from_prefab(level_prefab, tilemap, bg_tilemap, self.unit_registry, self.current_mode)
        if with_party:
            self.current_party = with_party
        else:
            self.current_party = self.current_level.party

        self.roam_info = RoamInfo(level_prefab.roam, level_prefab.roam_unit)

        self.level_setup()

    def build_level_from_scratch(self, level_nid, tilemap):
        """
        Using the tilemap: TileMapObject to build a level
        """
        self.boundary = None
        self.generic()
        logging.debug("Building Level %s", level_nid)

        from app.engine.level_cursor import LevelCursor
        from app.engine.objects.level import LevelObject

        self.cursor = LevelCursor(self)
        party = DB.parties.keys()[0]
        self.current_level = LevelObject.from_scratch(level_nid, tilemap, None, party, self.unit_registry, self.current_mode)
        self.current_party = self.current_level.party
        self.roam_info.clear()

        self.level_setup()

    def full_register(self, unit):
        self.register_unit(unit)
        for item in unit.items:
            self.register_item(item)
        for skill in unit.all_skills:
            self.register_skill(skill)

    def set_up_game_board(self, tilemap, bounds=None):
        from app.engine import boundary, game_board
        self.board = game_board.GameBoard(tilemap)
        if bounds:
            self.board.set_bounds(*bounds)
        self.boundary = boundary.BoundaryInterface(tilemap.width, tilemap.height)

    def save(self):
        s_dict = {'units': [unit.save() for unit in self.unit_registry.values()],
                  'items': [item.save() for item in self.item_registry.values()],
                  'skills': [skill.save() for skill in self.skill_registry.values()],
                  'terrain_status_registry': self.terrain_status_registry,
                  'regions': [region.save() for region in self.region_registry.values()],
                  'level': self.current_level.save() if self.current_level else None,
                  'overworlds': [overworld.save() for overworld in self.overworld_registry.values()],
                  'turncount': self.turncount,
                  'playtime': self.playtime,
                  'game_vars': self.game_vars,
                  'level_vars': self.level_vars,
                  'current_mode': self.current_mode.save(),
                  'parties': [party.save() for party in self.parties.values()],
                  'current_party': self.current_party,
                  'state': self.state.save(),
                  'action_log': self.action_log.save(),
                  'events': self.events.save(),
                  'supports': self.supports.save(),
                  'records': self.records.save(),
                  'speak_styles': self.speak_styles.save(),
                  'market_items': self.market_items,  # Item nids
                  'unlocked_lore': self.unlocked_lore,
                  'dialog_log': self.dialog_log.save(),
                  'already_triggered_events': self.already_triggered_events,
                  'talk_options': self.talk_options,
                  'base_convos': self.base_convos,
                  'current_random_state': static_random.get_combat_random_state(),
                  'bounds': self.board.bounds if self.board else None,
                  'roam_info': self.roam_info,
                  }
        meta_dict = {'playtime': self.playtime,
                     'realtime': time.time(),
                     'version': VERSION,
                     'title': DB.constants.value('title'),
                     'mode': self.current_mode.nid,
                     }
        if self.current_level:
            meta_dict['level_nid'] = self.current_level.nid
            meta_dict['level_title'] = self.current_level.name
        elif self.game_vars.get('_next_level_nid') is not None:
            fake_level = DB.levels.get(self.game_vars.get('_next_level_nid'))
            meta_dict['level_nid'] = fake_level.nid
            meta_dict['level_title'] = fake_level.name
        else:
            meta_dict['level_nid'] = None
            meta_dict['level_title'] = 'Overworld'

        return s_dict, meta_dict

    def load(self, s_dict):
        from app.engine import action, aura_funcs, records, save, supports, turnwheel, dialog_log
        from app.engine.objects.difficulty_mode import DifficultyModeObject
        from app.engine.objects.item import ItemObject
        from app.engine.objects.level import LevelObject
        from app.engine.objects.overworld import OverworldObject
        from app.engine.objects.party import PartyObject
        from app.engine.objects.skill import SkillObject
        from app.engine.objects.unit import UnitObject
        from app.engine.objects.region import RegionObject
        from app.events import event_manager, speak_style

        logging.info("Loading Game...")
        self.game_vars = Counter(s_dict.get('game_vars', {}))
        static_random.set_seed(self.game_vars.get('_random_seed', 0))
        self.level_vars = Counter(s_dict.get('level_vars', {}))
        mode_dict = s_dict.get('current_mode')
        if mode_dict:
            self.current_mode = DifficultyModeObject.restore(mode_dict)
        else:
            self.current_mode = self.default_mode()
        self.playtime = float(s_dict['playtime'])
        self.current_party = s_dict['current_party']
        self.turncount = int(s_dict['turncount'])

        self.state.load_states(s_dict['state'][0], s_dict['state'][1])

        self.item_registry = {item['uid']: ItemObject.restore(item) for item in s_dict['items']}
        self.skill_registry = {skill['uid']: SkillObject.restore(skill) for skill in s_dict['skills']}
        save.set_next_uids(self)
        self.terrain_status_registry = s_dict.get('terrain_status_registry', {})
        self.unit_registry = {unit['nid']: UnitObject.restore(unit, self) for unit in s_dict['units']}
        self.region_registry = {region['nid']: RegionObject.restore(region) for region in s_dict.get('regions', [])}

        # Handle subitems
        for item in self.item_registry.values():
            for subitem_uid in item.subitem_uids:
                subitem = self.item_registry.get(subitem_uid)
                item.subitems.append(subitem)
                for component in subitem.components:
                    component.item = item
                subitem.parent_item = item
        # Handle subskill
        for skill in self.skill_registry.values():
            if skill.subskill_uid is not None:
                subskill = self.skill_registry.get(skill.subskill_uid)
                skill.subskill = subskill
                subskill.parent_skill = skill

        self.parties = {party_data['nid']: PartyObject.restore(party_data) for party_data in s_dict['parties']}
        self.market_items = s_dict.get('market_items', {})
        self.unlocked_lore = s_dict.get('unlocked_lore', [])
        self.dialog_log = dialog_log.DialogLog.restore(s_dict.get('dialog_log', []))

        self.already_triggered_events = s_dict.get('already_triggered_events', [])
        self.talk_options = s_dict.get('talk_options', [])
        self.base_convos = s_dict.get('base_convos', {})

        # load all overworlds, or initialize them
        if 'overworlds' in s_dict:
            for overworld in s_dict['overworlds']:
                overworld_obj = OverworldObject.restore(overworld, self)
                self.overworld_registry[overworld_obj.nid] = overworld_obj
        for overworld in DB.overworlds.values():
            if overworld.nid not in self.overworld_registry:
                self.overworld_registry[overworld.nid] = OverworldObject.from_prefab(overworld, self.parties, self.unit_registry)

        self.action_log = turnwheel.ActionLog.restore(s_dict['action_log'])
        if s_dict.get('supports'):
            self.supports = supports.SupportController.restore(s_dict['supports'])
        else:
            self.supports = supports.SupportController()
        if s_dict.get('records'):
            self.records = records.Recordkeeper.restore(s_dict['records'])
        else:
            self.records = records.Recordkeeper()
        if s_dict.get('speak_styles'):
            self.speak_styles = speak_style.SpeakStyleLibrary.restore(s_dict['speak_styles'])
        else:
            self.speak_styles = speak_style.SpeakStyleLibrary()

        if 'current_random_state' in s_dict:
            static_random.set_combat_random_state(s_dict['current_random_state'])

        self.roam_info = s_dict.get('roam_info', RoamInfo())

        if s_dict['level']:
            logging.info("Loading Level...")
            self.current_level = LevelObject.restore(s_dict['level'], self)
            self.set_up_game_board(self.current_level.tilemap, s_dict.get('bounds'))

            self.generic()
            from app.engine.level_cursor import LevelCursor
            self.cursor = LevelCursor(self)

            # The fog and vision regions affect the game board
            for region in self.current_level.regions:
                if region.region_type == RegionType.FOG:
                    action.AddFogRegion(region).execute()
                elif region.region_type == RegionType.VISION:
                    action.AddVisionRegion(region).execute()

            # Now have units actually arrive on map
            for unit in self.units:
                if unit.position:
                    self.board.set_unit(unit.position, unit)
                    for skill in unit.all_skills:
                        if skill.aura:
                            aura_funcs.repopulate_aura(unit, skill, self)
                    self.boundary.arrive(unit)
                    action.UpdateFogOfWar(unit).execute()

            self.cursor.autocursor(True)

        self.events = event_manager.EventManager.restore(s_dict.get('events'))

    def clean_up(self, full: bool = True):
        '''
        A `full` cleanup does everything associated with cleaning up
        a chapter in preparation for the next.
        A non-full cleanup does not
            - Remove any units from the field
            - Remove all non-persistent units from memory
            - Remove all now unused items and skills from memory
            - Remove any regions or terrain statuses
            - Reset level vars
            - Reset talk options or base convos
            - Actually remove the level
        '''

        from app.engine import (action, item_funcs, item_system, skill_system,
                                supports)

        supports.increment_end_chapter_supports()

        self.game_vars['_current_turnwheel_uses'] = \
            self.game_vars.get('_max_turnwheel_uses', -1)

        if full:
            for unit in self.unit_registry.values():
                self.leave(unit)
        for unit in self.unit_registry.values():
            # Unit cleanup
            unit.is_dying = False
            if unit.traveler:
                droppee = self.get_unit(unit.traveler)
                if full:
                    action.RemoveSkill(unit, 'Rescue', source=unit.traveler, source_type=SourceType.TRAVELER).execute()
                    unit.traveler = None
                else:
                    pos = self.target_system.get_nearest_open_tile(droppee, unit.position)
                    action.Drop(unit, droppee, pos).execute()
                skill_system.on_separate(droppee, unit)
            unit.set_hp(1000)  # Set to full health
            unit.set_guard_gauge(0) # Remove all guard gauge
            if DB.constants.value('reset_mana'):
                unit.set_mana(1000)  # Set to full mana
            if full:
                unit.position = None
            unit.sprite.change_state('normal')
            unit.sprite.reset()
            unit.reset()

        for item in list(self.item_registry.values()):
            unit = None
            if item.owner_nid:
                unit = self.get_unit(item.owner_nid)
            item_system.on_end_chapter(unit, item)

        for skill in list(self.skill_registry.values()):
            unit = None
            if skill.owner_nid:
                unit = self.get_unit(skill.owner_nid)
                if unit:
                    skill_system.on_end_chapter(unit, skill)
                else:
                    logging.error("Unable to find owner %s in unit_registry", skill.owner_nid)

        if full:
            self.terrain_status_registry.clear()
            self.region_registry.clear()

            # Remove all non-persistent units
            self.unit_registry = {k: v for (k, v) in self.unit_registry.items() if v.persistent}

            # Remove any skill that's not on a unit and does not have a parent_skill
            for k, v in list(self.skill_registry.items()):
                if v.parent_skill:
                    if v.parent_skill.owner_nid:
                        if v.parent_skill.owner_nid not in self.unit_registry:
                            del self.skill_registry[k]
                    else:
                        del self.skill_registry[k]
                elif v.owner_nid:  # Remove skills from units that no longer exist
                    if v.owner_nid not in self.unit_registry:
                        del self.skill_registry[k]
                else:
                    del self.skill_registry[k]

            # Remove any item that's not on a unit or in the convoy
            for k, v in list(self.item_registry.items()):
                if v.owner_nid:  # Remove items from units that no longer exist
                    if v.owner_nid not in self.unit_registry:
                        del self.item_registry[k]
                else:
                    for party in self.parties.values():
                        if v in party.convoy or (v.parent_item and v.parent_item in party.convoy):
                            break
                    else:  # No party ever found
                        del self.item_registry[k]
        # End If

        # Handle player death
        for unit in self.unit_registry.values():
            if unit.dead:
                action.do(action.ChangeFatigue(unit, -unit.get_fatigue()))  # Reset fatigue
            if unit.dead and unit.team == 'player':
                if not game.current_mode.permadeath:
                    unit.dead = False  # Resurrect unit
                elif DB.constants.value('convoy_on_death'):
                    for item in item_funcs.get_all_tradeable_items(unit):
                        unit.remove_item(item)
                        # Put the item in the unit's party's convoy
                        self.parties[unit.party].convoy.append(item)

        # Remove unnecessary information between levels
        if full:
            self.sweep()
            self.current_level = None
            self.roam_info.clear()
        else:
            self.turncount = 1
            self.action_log.set_first_free_action()
        self.on_alter_game_state()

    @property
    def level(self):
        return self.current_level

    @property
    def level_nid(self):
        if self.is_displaying_overworld():
            return self.overworld_controller.next_level
        elif self.level:
            return self.level.nid

    @property
    def current_party(self):
        if self.is_displaying_overworld() and self.overworld_controller.selected_entity.nid:
            self._current_party = self.overworld_controller.selected_entity.nid
        return self._current_party

    @current_party.setter
    def current_party(self, party_nid: NID):
        self._current_party = party_nid
        if self.overworld_controller and party_nid:
            self.overworld_controller.select_entity(self._current_party)

    @property
    def tilemap(self):
        if self.is_displaying_overworld():
            return self.overworld_controller.tilemap
        elif self.current_level:
            return self.current_level.tilemap
        else:
            return None

    @property
    def bg_tilemap(self):
        if self.current_level and self.current_level.bg_tilemap:
            return self.current_level.bg_tilemap
        return None

    @property
    def mode(self):
        return DB.difficulty_modes.get(self.current_mode.nid)

    def default_mode(self):
        from app.engine.objects.difficulty_mode import DifficultyModeObject
        first_mode = DB.difficulty_modes[0]
        if first_mode.permadeath_choice == PermadeathOption.PLAYER_CHOICE:
            permadeath = False
        else:
            permadeath = first_mode.permadeath_choice == PermadeathOption.CLASSIC
        if first_mode.growths_choice == GrowthOption.PLAYER_CHOICE:
            growths = GrowthOption.FIXED
        else:
            growths = first_mode.growths_choice
        return DifficultyModeObject(first_mode.nid, permadeath, growths)

    @property
    def party(self):
        return self.parties[self.current_party]

    def get_party(self, party_nid: str = None):
        if not party_nid:
            party_nid = self.current_party
        if party_nid not in self.parties:
            self.build_party(party_nid)
        return self.parties[party_nid]

    def build_party(self, party_nid):
        from app.engine.objects.party import PartyObject
        party_prefab = DB.parties.get(party_nid)
        if not party_prefab:
            party_prefab = DB.parties[0]
        nid, name, leader = party_prefab.nid, party_prefab.name, party_prefab.leader
        self.parties[self.current_party] = PartyObject(nid, name, leader)

    @property
    def units(self):
        return list(self.unit_registry.values())

    @property
    def regions(self):
        return list(self.region_registry.values())

    def register_unit(self, unit):
        logging.debug("Registering unit %s as %s", unit, unit.nid)
        self.unit_registry[unit.nid] = unit

    def unregister_unit(self, unit):
        logging.debug("Unregistering unit %s as %s", unit, unit.nid)
        del self.unit_registry[unit.nid]

    def register_item(self, item):
        logging.debug("Registering item %s as %s", item, item.uid)
        self.item_registry[item.uid] = item
        # For multi-items
        for subitem in item.subitems:
            self.register_item(subitem)

    def unregister_item(self, item):
        logging.debug("Unregistering item %s as %s", item, item.uid)
        del self.item_registry[item.uid]
        # For multi-items
        for subitem in item.subitems:
            self.unregister_item(subitem)

    def register_skill(self, skill):
        logging.debug("Registering skill %s as %s", skill, skill.uid)
        self.skill_registry[skill.uid] = skill
        # For aura skills
        if skill.subskill:
            self.skill_registry[skill.subskill.uid] = skill.subskill

    def unregister_skill(self, skill):
        logging.debug("Unregistering skill %s as %s", skill, skill.uid)
        del self.skill_registry[skill.uid]
        # For aura skills
        if skill.subskill:
            del self.skill_registry[skill.subskill.uid]

    def register_terrain_status(self, key: Tuple[int, int, NID], skill_uid: UID):
        logging.debug("Registering terrain status %s", skill_uid)
        self.terrain_status_registry[key] = skill_uid

    def register_region(self, region: RegionObject):
        logging.debug("Registering region %s", region.nid)
        self.region_registry[region.nid] = region

    def get_data(self, raw_data_nid):
        if str(raw_data_nid) in DB.raw_data.keys():
            return DB.raw_data.get(str(raw_data_nid))
        return None

    def get_unit(self, unit_nid):
        """
        Can get units not just in the current level
        Could be used to get units in overworld, base,
        etc.
        """
        unit = self.unit_registry.get(unit_nid)
        return unit

    def get_klass(self, unit_nid):
        unit = self.unit_registry.get(unit_nid)
        if not unit:
            return None
        klass = DB.classes.get(unit.klass)
        return klass

    def get_convoy_inventory(self, party=None):
        if not party:
            party = self.party
        return party.convoy

    def get_item(self, item_uid):
        item = self.item_registry.get(item_uid)
        return item

    def get_skill(self, skill_uid):
        skill = self.skill_registry.get(skill_uid)
        return skill

    def get_terrain_status(self, key: Tuple[int, int, NID]) -> UID:
        skill_uid = self.terrain_status_registry.get(key)
        return skill_uid

    def get_region(self, region_nid):
        region = self.region_registry.get(region_nid)
        return region

    @lru_cache(128)
    def get_region_under_pos(self, pos: Tuple[int, int], region_type: RegionType = None) -> Optional[RegionObject]:
        """
        if region_type arguments is None, all region types are accepted and available to be returned
        """
        if pos and self.level:
            for region in self.level.regions.values():
                if (not region_type or region.region_type == region_type) and region.contains(pos):
                    return region

    def get_ai_group(self, ai_group_nid: NID) -> Optional[AIGroupObject]:
        if self.level:
            return self.level.ai_groups.get(ai_group_nid)
        return None

    def ai_group_active(self, ai_group_nid: NID) -> bool:
        group = self.get_ai_group(ai_group_nid)
        if group:
            return group.active
        return False

    def get_units_in_ai_group(self, ai_group_nid: NID) -> List[UnitObject]:
        return [unit for unit in self.get_all_units() if unit.ai_group == ai_group_nid]

    def get_all_units(self, only_on_field=True) -> List[UnitObject]:
        if only_on_field:
            return [unit for unit in self.units if unit.position and not unit.dead and not unit.is_dying and 'Tile' not in unit.tags]
        else:
            return self.units

    def get_player_units(self, only_on_field=True) -> List[UnitObject]:
        return [unit for unit in self.get_all_units(only_on_field) if unit.team == 'player']

    def get_enemy_units(self, only_on_field=True) -> List[UnitObject]:
        return [unit for unit in self.get_all_units(only_on_field) if unit.team in DB.teams.enemies]

    def get_enemy1_units(self, only_on_field=True) -> List[UnitObject]:
        return [unit for unit in self.get_all_units(only_on_field) if unit.team == 'enemy']

    def get_enemy2_units(self, only_on_field=True) -> List[UnitObject]:
        return [unit for unit in self.get_all_units(only_on_field) if unit.team == 'enemy2']

    def get_other_units(self, only_on_field=True) -> List[UnitObject]:
        return [unit for unit in self.get_all_units(only_on_field) if unit.team == 'other']

    def get_team_units(self, team: str, only_on_field=True) -> List[UnitObject]:
        return [unit for unit in self.get_all_units(only_on_field) if unit.team == team]

    def get_travelers(self) -> List[UnitObject]:
        return [self.get_unit(unit.traveler) for unit in self.get_all_units() if unit.traveler]

    def get_player_units_and_travelers(self) -> List[UnitObject]:
        return self.get_player_units() + [unit for unit in self.get_travelers() if unit.team == 'player']

    def get_rescuer(self, unit):
        for rescuer in self.units:
            if rescuer.traveler == unit.nid:
                return rescuer
        return None

    def get_rescuers_position(self, unit):
        for rescuer in self.units:
            if rescuer.traveler == unit.nid:
                return rescuer.position
        return None

    def get_all_units_in_party(self, party=None) -> List[UnitObject]:
        if party is None:
            party = self.current_party
        party_units = [unit for unit in self.units if unit.team == 'player' and unit.persistent and unit.party == party]
        return party_units

    def get_units_in_party(self, party=None) -> List[UnitObject]:
        if party is None:
            party = self.current_party
        party_order = self.parties[party].party_prep_manage_sort_order
        party_units = [unit for unit in game.get_all_units_in_party(party) if not unit.dead]
        party_units = sorted(party_units, key=lambda unit: party_order.index(unit.nid) if unit.nid in party_order else 999999)
        return party_units

    def get_all_player_units(self) -> List[UnitObject]:
        """
        # Return all units who are currently player team and persistent
        """
        return [unit for unit in self.units if unit.team == 'player' and unit.persistent]

    # For working with roaming
    def is_roam(self) -> bool:
        return self.roam_info.roam

    def set_roam(self, b: bool):
        self.roam_info.roam = b

    def get_roam_unit(self) -> UnitObject:
        if self.roam_info.roam_unit_nid:
            return game.get_unit(self.roam_info.roam_unit_nid)
        return None

    def set_roam_unit(self, unit: UnitObject):
        self.roam_info.roam_unit_nid = unit.nid

    def clear_roam_unit(self):
        self.roam_info.roam_unit_nid = None

    def check_dead(self, nid):
        unit = self.get_unit(nid)
        if unit and (unit.dead or unit.is_dying):
            return True
        return False

    def check_alive(self, nid):
        unit = self.get_unit(nid)
        if unit and not (unit.dead or unit.is_dying):
            return True
        return False

    def leave(self, unit, test=False):
        """
        # Removes a unit from the map
        # This function should always be called BEFORE changing the unit's position
        # Handles:
        # 1. removing the unit from the boundary manager
        # 2. Removes any auras from the unit's skill list, since they will no longer be on the map
        # 3. Removes any of the unit's own auras from the map
        # 4. Removes any status/skills that the terrain or regions on the map are giving
        # the unit
        #
        # If "test" is True, some of these are skipped, such as removing the unit from
        # the boundary manager and registering these actions with the action_log
        # Set "test" to True when you are just testing what would happen by moving
        # to a position (generally used for AI)
        """
        from app.engine import action, aura_funcs
        if unit.position:
            logging.debug("Leave %s %s", unit.nid, unit.position)
            # Auras
            for aura_data in game.board.get_auras(unit.position):
                child_aura_uid, target = aura_data
                child_skill = self.get_skill(child_aura_uid)
                aura_funcs.remove_aura(unit, child_skill, test)
            if not test:
                for skill in unit.all_skills:
                    if skill.aura:
                        aura_funcs.release_aura(unit, skill, self)
                self.boundary.unregister_unit_auras(unit)
            # Status Regions
            for region in game.level.regions:
                if region.region_type == RegionType.STATUS and region.contains(unit.position):
                    skill_uid = self.get_terrain_status((*region.position, region.sub_nid))
                    skill_obj = self.get_skill(skill_uid)
                    if skill_obj and skill_obj in unit.all_skills:
                        if test:
                            unit.remove_skill(skill_obj, source=region.nid, source_type=SourceType.REGION)
                        else:
                            act = action.RemoveSkill(unit, skill_obj, source=region.nid, source_type=SourceType.REGION)
                            action.do(act)
            self.remove_terrain_skills(unit, test)
            # Boundary
            if not test:
                self.boundary.leave(unit)
            # Board
            if not test:
                self.board.remove_unit(unit.position, unit)

    def remove_terrain_skills(self, unit, test=False):
        from app.engine import action
        # Tiles and terrain regions
        terrain_nid = self.get_terrain_nid(self.tilemap, unit.position)
        terrain = DB.terrain.get(terrain_nid)
        terrain_key = (*unit.position, terrain.status)

        skill_uid = self.get_terrain_status(terrain_key)
        skill_obj = self.get_skill(skill_uid)
        if skill_obj and skill_obj in unit.all_skills:
            if test:
                unit.remove_skill(skill_obj, source=unit.position, source_type=SourceType.TERRAIN)
            else:
                act = action.RemoveSkill(unit, skill_obj, source=unit.position, source_type=SourceType.TERRAIN)
                action.do(act)

    def arrive(self, unit, test=False):
        """
        # Adds a unit to the map
        # This function should always be called AFTER changing the unit's position
        # Handles:
        # 1. adding the unit to the boundary manager
        # 2. adding any auras from that the unit should be affected by to the the unit's skill list
        # 3. Adding any of the unit's own auras to other units
        # 4. Adding any status/skills that the terrain or regions on the map are giving
        #
        # If "test" is True, some of these are skipped, such as adding the unit to
        # the boundary manager and registering these actions with the action_log
        # Set "test" to True when you are just testing what would happen by moving
        # to a position (generally used for AI)
        """
        from app.engine import aura_funcs, skill_system
        if unit.position:
            logging.debug("Arrive %s %s", unit.nid, unit.position)
            if not test:
                self.board.set_unit(unit.position, unit)
            # Tiles and Terrain Regions
            if not skill_system.ignore_terrain(unit):
                self.add_terrain_status(unit, test)
            # Status Regions
            if not skill_system.ignore_region_status(unit):
                for region in game.level.regions:
                    if region.region_type == RegionType.STATUS and region.contains(unit.position):
                        self.add_region_status(unit, region, test)
            # Auras
            aura_funcs.pull_auras(unit, self, test)
            if not test:
                for skill in unit.all_skills:
                    if skill.aura:
                        aura_funcs.propagate_aura(unit, skill, self)
                self.boundary.register_unit_auras(unit)
            # Boundary
            if not test:
                self.boundary.arrive(unit)

    def add_terrain_status(self, unit, test):
        from app.engine import action, item_funcs

        terrain_nid = self.get_terrain_nid(self.tilemap, unit.position)
        terrain = DB.terrain.get(terrain_nid)
        terrain_key = (*unit.position, terrain.status)

        skill_uid = self.get_terrain_status(terrain_key)
        skill_obj = self.get_skill(skill_uid)

        if not skill_obj:
            if terrain and terrain.status:
                skill_obj = item_funcs.create_skill(unit, terrain.status)
                if skill_obj:
                    game.register_skill(skill_obj)
                    self.register_terrain_status(terrain_key, skill_obj.uid)
                else:
                    logging.error("Could not create skill with nid: %s" % terrain.status)

        if skill_obj:
            # Only bother adding if not already present
            if skill_obj not in unit.all_skills:
                if test:
                    # Don't need to use action for test
                    unit.add_skill(skill_obj, source=unit.position, source_type=SourceType.TERRAIN)
                else:
                    act = action.AddSkill(unit, skill_obj, source=unit.position, source_type=SourceType.TERRAIN)
                    action.do(act)

    def add_region_status(self, unit: UnitObject, region: RegionObject, test: bool):
        from app.engine import action, item_funcs
        terrain_key = (*region.position, region.sub_nid)
        skill_uid = self.get_terrain_status(terrain_key)
        skill_obj = self.get_skill(skill_uid)

        if not skill_obj:
            skill_obj = item_funcs.create_skill(unit, region.sub_nid)
            self.register_skill(skill_obj)
            self.register_terrain_status(terrain_key, skill_obj.uid)

        if skill_obj:
            # Only bother adding if not already present
            if skill_obj not in unit.all_skills:
                if test:
                    # Don't need to use action for test
                    unit.add_skill(skill_obj, source=region.nid, source_type=SourceType.REGION)
                else:
                    act = action.AddSkill(unit, skill_obj, source=region.nid, source_type=SourceType.REGION)
                    action.do(act)
                    return act

    def check_for_region(self, position, region_type: RegionType, sub_nid=None):
        if not position:
            return None
        for region in game.level.regions:
            if region.region_type == region_type and region.contains(position):
                if not sub_nid or region.sub_nid == sub_nid:
                    return region
        return None

    def get_terrain_nid(self, tilemap, position) -> NID:
        terrain_region = self.get_region_under_pos(position, RegionType.TERRAIN)
        if terrain_region:
            terrain_nid = terrain_region.sub_nid
        else:
            terrain_nid = tilemap.get_terrain(position)
        return terrain_nid

    def get_all_formation_spots(self) -> list:
        legal_spots = set()
        for region in game.level.regions:
            if region.region_type == RegionType.FORMATION:
                for x in range(region.size[0]):
                    for y in range(region.size[1]):
                        legal_spots.add((region.position[0] + x, region.position[1] + y))
        return legal_spots

    def get_open_formation_spots(self) -> list:
        all_formation_spots = self.get_all_formation_spots()
        return sorted({pos for pos in all_formation_spots if not self.board.get_unit(pos)})

    def get_next_formation_spot(self) -> tuple:
        legal_spots = self.get_open_formation_spots()
        if legal_spots:
            return legal_spots[0]
        return None

    def get_money(self):
        return self.parties[self.current_party].money

    def set_money(self, val):
        self.parties[self.current_party].money = val

    def get_bexp(self):
        return self.parties[self.current_party].bexp

    def inc_bexp(self, amount):
        self.parties[self.current_party].bexp += amount

    def set_bexp(self, amount):
        self.parties[self.current_party].bexp = amount

    # Random funcs
    def get_random(self, a: int, b: int) -> int:
        """
        Canonical method for getting a random integer from within an event
        without screwing up the turnwheel
        Inclusive between a and b
        """
        from app.engine import action
        old = static_random.get_other_random_state()
        result = static_random.get_other(a, b)
        new = static_random.get_other_random_state()
        action.do(action.RecordOtherRandomState(old, new))
        return result

    def get_random_float(self) -> float:
        """
        Canonical method for getting a random float (0, 1]
        without screwing up the turnwheel
        """
        from app.engine import action
        old = static_random.get_other_random_state()
        result = static_random.get_random_float()
        new = static_random.get_other_random_state()
        action.do(action.RecordOtherRandomState(old, new))
        return result

    def get_random_choice(self, choices, seed=None):
        """
        Canonical method for getting a random choice from an iterable
        without screwing up the turnwheel
        """
        from app.engine import action
        if seed is not None:
            r = static_random.get_generator(int(seed))
            idx = r.randint(0, len(choices) - 1)
        else:
            old = static_random.get_other_random_state()
            idx = static_random.get_other(0, len(choices) - 1)
            new = static_random.get_other_random_state()
            action.do(action.RecordOtherRandomState(old, new))
        return list(choices)[idx]

    def get_random_weighted_choice(self, choices: List, weights: List[float]):
        from app.engine import action
        old = static_random.get_other_random_state()
        idx = static_random.weighted_choice(weights, static_random.r.other_random)
        new = static_random.get_other_random_state()
        action.do(action.RecordOtherRandomState(old, new))
        return choices[idx]

game = GameState()

def start_game():
    global game
    if not game:
        game = GameState()
    else:
        game.clear()  # Need to use old game if called twice in a row
    game.load_states(['title_start'])
    return game

def start_level(level_nid):
    global game
    logging.info("Start Level %s" % level_nid)
    if not game:
        game = GameState()
    else:
        game.clear()  # Need to use old game if called twice in a row
    game.load_states(['start_level_asset_loading'])
    game.build_new()
    game.start_level(level_nid)
    return game

def load_level(level_nid, save_loc):
    global game
    logging.info("Load Level %s" % level_nid)
    if not game:
        game = GameState()
    else:
        game.clear()
    import pickle

    from app.engine import save
    with open(save_loc, 'rb') as fp:
        s_dict = pickle.load(fp)
    game.load_states(['start_level_asset_loading'])
    game.build_new()
    game.load(s_dict)
    save.set_next_uids(game)
    game.start_level(level_nid)
    return game
