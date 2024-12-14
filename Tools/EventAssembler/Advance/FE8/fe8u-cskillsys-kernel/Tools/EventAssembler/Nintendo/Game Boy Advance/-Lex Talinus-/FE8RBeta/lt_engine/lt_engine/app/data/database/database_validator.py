from __future__ import annotations
from app.data.database.components import ComponentType as CType
from app.data.database.database_types import DatabaseType as DType
from app.data.database.weapons import WexpGain
from dataclasses import dataclass
import logging
from app.data.database.database import Database
from app.data.resources.resource_types import ResourceType as RType
from app.data.resources.resources import Resources
from app.utilities.data import Data
from app.utilities.typing import NID
import re
from typing import Any, Callable, Dict, List, Set

@dataclass
class ValidationError():
    category: str
    owner: str
    where: str
    what: str

    def __str__(self) -> str:
        return "Error in %s:%s:%s - %s" % self.category, self.owner, self.where, self.what

    @classmethod
    def does_not_exist(self, nid):
        return "%s does not exist" % nid

def _nid_in_data(data: Data):
    return lambda nid: nid in data.keys()

class DatabaseValidatorEngine():
    def __init__(self, db: Database, resources: Resources):
        self.db = db
        self.resources = resources
        self._vmap: Dict[CType | RType | DType, Callable[[NID], bool]] = {
            CType.WeaponType: _nid_in_data(db.weapons),
            CType.WeaponRank: _nid_in_data(db.weapon_ranks),
            CType.Unit: _nid_in_data(db.units),
            CType.Class: _nid_in_data(db.classes),
            CType.Tag: _nid_in_data(db.tags),
            CType.Item: _nid_in_data(db.items),
            CType.Skill: _nid_in_data(db.skills),
            CType.Stat: _nid_in_data(db.stats),
            CType.MapAnimation: _nid_in_data(resources.animations),
            CType.Equation: _nid_in_data(db.equations),
            CType.MovementType: lambda mtype: mtype in db.mcost.get_unit_types(),
            CType.Sound: _nid_in_data(resources.sfx),
            CType.AI: _nid_in_data(db.ai),
            CType.Music: _nid_in_data(resources.music),
            CType.CombatAnimation: _nid_in_data(resources.combat_anims),
            CType.EffectAnimation: _nid_in_data(resources.combat_effects),
            CType.Affinity: _nid_in_data(db.affinities),
            CType.Terrain: _nid_in_data(db.terrain),
            CType.Event: _nid_in_data(db.events),

            RType.ICONS16: _nid_in_data(resources.icons16),
            RType.ICONS32: _nid_in_data(resources.icons32),
            RType.ICONS80: _nid_in_data(resources.icons80),
            RType.PORTRAITS: _nid_in_data(resources.portraits),
            RType.ANIMATIONS: _nid_in_data(resources.animations),
            RType.PANORAMAS: _nid_in_data(resources.panoramas),
            RType.MAP_ICONS: _nid_in_data(resources.map_icons),
            RType.MAP_SPRITES: _nid_in_data(resources.map_sprites),
            RType.COMBAT_PALETTES: _nid_in_data(resources.combat_palettes),
            RType.COMBAT_ANIMS: _nid_in_data(resources.combat_anims),
            RType.COMBAT_EFFECTS: _nid_in_data(resources.combat_effects),
            RType.TILESETS: _nid_in_data(resources.tilesets),
            RType.TILEMAPS: _nid_in_data(resources.tilemaps),
            RType.MUSIC: _nid_in_data(resources.music),
            RType.SFX: _nid_in_data(resources.sfx),

            DType.CONSTANTS: _nid_in_data(db.constants),
            DType.TEAMS: _nid_in_data(db.teams),
            DType.STATS: _nid_in_data(db.stats),
            DType.EQUATIONS: _nid_in_data(db.equations),
            DType.MCOST: _nid_in_data(db.mcost),
            DType.TERRAIN: _nid_in_data(db.terrain),
            DType.MINIMAP: _nid_in_data(db.minimap),
            DType.WEAPON_RANKS: _nid_in_data(db.weapon_ranks),
            DType.WEAPONS: _nid_in_data(db.weapons),
            DType.FACTIONS: _nid_in_data(db.factions),
            DType.ITEMS: _nid_in_data(db.items),
            DType.SKILLS: _nid_in_data(db.skills),
            DType.TAGS: _nid_in_data(db.tags),
            DType.GAME_VAR_SLOTS: _nid_in_data(db.game_var_slots),
            DType.CLASSES: _nid_in_data(db.classes),
            DType.SUPPORT_CONSTANTS: _nid_in_data(db.support_constants),
            DType.SUPPORT_RANKS: _nid_in_data(db.support_ranks),
            DType.AFFINITIES: _nid_in_data(db.affinities),
            DType.UNITS: _nid_in_data(db.units),
            DType.SUPPORT_PAIRS: _nid_in_data(db.support_pairs),
            DType.PARTIES: _nid_in_data(db.parties),
            DType.AI: _nid_in_data(db.ai),
            DType.DIFFICULTY_MODES: _nid_in_data(db.difficulty_modes),
            DType.OVERWORLDS: _nid_in_data(db.overworlds),
            DType.LEVELS: _nid_in_data(db.levels),
            DType.EVENTS: _nid_in_data(db.events),
            DType.TRANSLATIONS: _nid_in_data(db.translations),
            DType.LORE: _nid_in_data(db.lore),
            DType.RAW_DATA: _nid_in_data(db.raw_data),
        }

        # native types, don't really need to check these
        for ctype in (CType.Bool, CType.Int, CType.Float, CType.String,
                      CType.Color3, CType.Color4, CType.StringDict, CType.MultipleOptions):
            def _trivial_type(val):
                return True
            self._vmap[ctype] = _trivial_type

        # we shouldn't be validating these types. instead we should parse their elements and validate separately
        for ctype in (CType.List, CType.Dict, CType.FloatDict,
                      CType.MultipleChoice, CType.NewMultipleOptions):
            def should_not_be_validating(nid):
                raise ValueError('%s should not be validated. Validate contents instead' % ctype)
            self._vmap[ctype] = should_not_be_validating

    def validate(self, dtype: CType | RType | DType, value: Any):
        return self._vmap[dtype](value)

    def validate_units(self):
        errors: List[ValidationError] = []
        for unit in self.db.units:
            if not self.validate_class(unit.klass):
                errors.append(ValidationError("Units", unit.nid, "klass", ValidationError.does_not_exist(unit.klass)))
        return errors

    def fill_and_trim(self, data_dict: dict, expected_keys: set, real_keys: set, default_value_factory: Callable):
        missing_keys = expected_keys - real_keys
        extraneous_keys = real_keys - expected_keys
        for key in missing_keys:
            data_dict[key] = default_value_factory()
        for key in extraneous_keys:
            del data_dict[key]

    def repair_units(self):
        all_weapon_types = set(self.db.weapons.keys())
        all_stats = set(self.db.stats.keys())
        for unit in self.db.units:
            # make sure each unit has an entry for every weapon type, and no extraneous ones
            self.fill_and_trim(unit.wexp_gain, all_weapon_types, set(unit.wexp_gain.keys()), lambda: WexpGain(False, 0, 0))
            # make sure each unit has an entry for every stat, and no extraneous ones
            self.fill_and_trim(unit.bases, all_stats, set(unit.bases.keys()), lambda: 0)
            self.fill_and_trim(unit.growths, all_stats, set(unit.growths.keys()), lambda: 0)

    def repair_klasses(self):
        all_weapon_types = set(self.db.weapons.keys())
        all_stats = set(self.db.stats.keys())
        for klass in self.db.classes:
            # make sure each klass has an entry for every weapon type, and no extraneous ones
            self.fill_and_trim(klass.wexp_gain, all_weapon_types, set(klass.wexp_gain.keys()), lambda: WexpGain(False, 0, 0))
            # make sure each klass has an entry for every stat in all dicts
            self.fill_and_trim(klass.bases, all_stats, set(klass.bases.keys()), lambda: 0)
            self.fill_and_trim(klass.growths, all_stats, set(klass.growths.keys()), lambda: 0)
            self.fill_and_trim(klass.growth_bonus, all_stats, set(klass.growth_bonus.keys()), lambda: 0)
            self.fill_and_trim(klass.promotion, all_stats, set(klass.promotion.keys()), lambda: 0)
            self.fill_and_trim(klass.max_stats, all_stats, set(klass.max_stats.keys()), lambda: 0)

    def repair_levels(self):
        for level in self.db.levels:
            all_level_units = set(level.units.keys())
            travelers: Dict[NID, NID] = {}
            for unit in level.units:
                # fix phantom travelers
                unit_traveler = unit.starting_traveler
                if unit_traveler and unit_traveler == unit.nid:
                    logging.error("Unit %s is its own traveler", unit.nid)
                    unit.starting_traveler = None
                    continue
                if unit_traveler and unit_traveler in travelers:
                    logging.error("Found traveler %s originally on unit %s, found duplicate on unit %s in level %s",
                                  unit_traveler, travelers[unit_traveler], unit.nid, level.nid)
                    unit.starting_traveler = None
                    continue
                if unit_traveler and unit_traveler not in all_level_units:
                    logging.error("Found traveler %s on unit %s in level %s that does not exist",
                                  unit_traveler, unit.nid, level.nid)
                    unit.starting_traveler = None
                    continue
                if unit_traveler and level.units.get(unit_traveler) and level.units.get(unit_traveler).starting_position:
                    logging.error("Traveler %s on unit %s is already on map", unit_traveler, unit.nid)
                    unit.starting_traveler = None
                    continue
                travelers[unit_traveler] = unit.nid

    def repair(self):
        """Only obvious repairs should be done here.
        """
        self.repair_units()
        self.repair_klasses()
        self.repair_levels()
