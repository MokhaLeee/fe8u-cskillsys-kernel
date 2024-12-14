from __future__ import annotations
from ast import Tuple
from dataclasses import dataclass, field
from typing import Any, Dict, List, Optional, Set

from app.data.database.components import Component, ComponentType
from app.data.database.database import Database
from app.data.database.database_types import DatabaseType as DType
from app.data.database.database_validator import DatabaseValidatorEngine
from app.data.database.item_components import ItemTags
from app.data.database.level_units import GenericUnit, UniqueUnit
from app.data.database.levels import OBJECTIVE_KEYS, LevelPrefab
from app.data.database.skill_components import SkillTags
from app.data.resources.resource_types import ResourceType
from app.data.resources.resources import Resources
from app.data.resources.tiles import TileMapPrefab
from app.data.validation.utils import LTType
import app.data.validation.validation_errors as ltdb
from app.events.event_version import EventVersion
from app.events.python_eventing.analyzer import PyEventAnalyzer
from app.events import event_commands
from app.events.regions import Region
from app.utilities.typing import NID

@dataclass
class ValidationResult():
    errors: List = field(default_factory=list)
    warnings: List = field(default_factory=list)

    def __add__(self, other: ValidationResult):
        return ValidationResult(self.errors + other.errors, self.warnings + other.warnings)

class DBChecker():
    def __init__(self, db: Database, resources: Resources) -> None:
        self.db = db
        self.resources = resources
        self.dve = DatabaseValidatorEngine(db, resources)

    def _validate_level_unit(self, unit: UniqueUnit | GenericUnit, level: LevelPrefab, level_units: Set[NID], travelers: Dict[NID, NID], l_err: ltdb.Error) -> ValidationResult:
        res = ValidationResult()
        u_err = l_err.field("Units").prop(DType.UNITS, unit.nid)

        res.errors += self._val_or_err(unit.ai, DType.AI, u_err.field("AI"))
        res.errors += self._val_or_err(unit.roam_ai, DType.AI, u_err.field("Roam AI"))

        tilemap_prefab = self.resources.tilemaps.get(level.tilemap)
        if unit.starting_position and tilemap_prefab and not tilemap_prefab.check_bounds(unit.starting_position):
            res.errors.append(u_err.msg("Starting Position %s is out of bounds" % unit.starting_position))

        if travelers.get(unit.nid):
            traveler = travelers.get(unit.nid)
            if traveler not in level_units:
                res.errors.append(u_err.msg("Traveler %s is not loaded into level" % traveler))
            elif traveler == unit.nid:
                res.errors.append(u_err.msg("Unit is its own traveler"))
            elif level.units.get(traveler).starting_position:
                res.errors.append(u_err.msg("Traveler %s already has position on map" % traveler))
            elif traveler in [v for k, v in travelers.items() if k != unit.nid]:
                res.errors.append(u_err.msg("Multiple units have identical traveler %s" % traveler))

        return res

    def _validate_generic_unit(self, unit: GenericUnit, level: LevelPrefab, level_units: Set[NID], travelers: Dict[NID, NID], l_err: ltdb.Error) -> ValidationResult:
        res = ValidationResult()
        u_err = l_err.field("Units").prop(DType.UNITS, unit.nid)
        res.errors += self._val_or_err(unit.get_items(), DType.ITEMS, u_err.field("Items"))
        res.errors += self._val_or_err(unit.get_skills(), DType.SKILLS, u_err.field("Skills"))
        res += self._validate_level_unit(unit, level, level_units, travelers, l_err)
        return res

    def _validate_region(self, region: Region, level_tilemap: Optional[TileMapPrefab], l_err: ltdb.Error) -> ValidationResult:
        res = ValidationResult()
        r_err = l_err.prop("Region", region.nid)
        r_warn = r_err.to_warn()
        if not level_tilemap:
            res.warnings.append(r_warn.msg("Region exists but no tilemap exists in level"))
            return res
        # @TODO(mag): determine if all regions should have positions/it's warning-worthy
        # if not region.position: append "Region has no position"
        if region.position:
            rx, ry = region.position
            rxb, ryb = rx + region.size[0], ry + region.size[1]
            if rx < 0 or ry < 0 or rxb > level_tilemap.width or ryb > level_tilemap.height:
                res.warnings.append(r_warn.msg("Region bounds exceed tilemap size or region not in tilemap"))
        return res

    def validate_levels(self) -> ValidationResult:
        res = ValidationResult()
        for level in self.db.levels:
            l_err = ltdb.Error(DType.LEVELS, level.nid)
            l_warn = l_err.to_warn()

            res.errors += self._val_or_err(level.tilemap, ResourceType.TILEMAPS, l_err.field("tilemap"))
            res.errors += self._val_or_err(level.bg_tilemap, ResourceType.TILEMAPS, l_err.field("bg_tilemap"))
            res.errors += self._val_or_err(level.party, DType.PARTIES, l_err.field("party"))

            # music
            for music_key in self.db.music_keys:
                track = level.music.get(music_key)
                res.errors += self._val_or_err(track, ResourceType.MUSIC, l_err.field("Music:%s" % music_key))

            # objective, warning only
            res.warnings += [l_warn.tmplt.null_f("objective:%s" % objtv) for objtv in OBJECTIVE_KEYS if not level.objective.get(objtv)]

            # roam
            if level.roam:
                res.errors += self._val_or_err(level.roam_unit, DType.UNITS, l_err.field("roam_unit"), False)

            all_level_units = set(level.units.keys())
            travelers: Dict[NID, NID] = {unit.nid: unit.starting_traveler for unit in level.units}
            for unit in level.units:
                if isinstance(unit, UniqueUnit):
                    res += self._validate_level_unit(unit, level, all_level_units, travelers, l_err)
                elif isinstance(unit, GenericUnit):
                    res += self._validate_generic_unit(unit, level, all_level_units, travelers, l_err)

            for region in level.regions:
                res += self._validate_region(region, self.resources.tilemaps.get(level.tilemap), l_err)

            # level.groups: if any warnings or errors pop up, we can add them here; can't think of any right now
            # level.ai_groups: ditto to the above
        return res

    # in short, iterates through every component on the item, and verifies that
    # the component's value matches its type.
    def validate_items_and_skills(self) -> ValidationResult:
        def _val_comp_t(ctype: ComponentType | Tuple[ComponentType, ComponentType], cval, c_err: ltdb.Error) -> List[ltdb.Error]:
            types_and_values = []
            if isinstance(ctype, tuple):
                container = ctype[0]
                value_type = ctype[1]
                if container == ComponentType.List:
                    # list of nids
                    types_and_values = [(value_type, value) for value in cval]
                elif container in (ComponentType.Dict, ComponentType.FloatDict):
                    # see elsewhere for usages: always dict of 'nid: int'
                    types_and_values = [(value_type, key) for key, _ in cval]
                elif container == ComponentType.MultipleChoice:
                    # short-circuit. we don't use DB validation for this
                    if not cval in value_type:
                        return [c_err.msg("Choice %s is not any of [%s]" % (cval, ','.join(value_type)))]
            else:
                types_and_values = [(ctype, cval)]
            errors = []
            for cctype, cvalue in types_and_values:
                errors += self._val_or_err(cvalue, cctype, c_err)
            return errors

        def _val_comp(component: Component, s_err: ltdb.Error) -> ValidationResult:
            c_err = s_err.prop("Component", component.nid)
            c_warn = c_err.to_warn()
            errors: List[ltdb.Error] = []
            warnings: List[ltdb.Warning] = []
            if not component.expose:
                return ValidationResult()
            if component.expose == ComponentType.NewMultipleOptions:
                for option in component.options.keys():
                    errors += _val_comp_t(component.options[option], component.value[option], c_err)
            else:
                errors += _val_comp_t(component.expose, component.value, c_err)
            if component.tag in (SkillTags.DEPRECATED, ItemTags.DEPRECATED):
                warnings.append(c_warn.msg("Deprecated Component. Consider using a newer version of this component. Engine updates may cause this to break."))
            return ValidationResult(errors, warnings)

        res = ValidationResult()
        for skill in self.db.skills:
            s_err = ltdb.Error(DType.SKILLS, skill.nid)
            for component in skill.components:
                res += _val_comp(component, s_err)
        for item in self.db.items:
            i_err = ltdb.Error(DType.ITEMS, item.nid)
            for component in item.components:
                res += _val_comp(component, i_err)
        return res

    def validate_units(self) -> ValidationResult:
        res = ValidationResult()
        for unit in self.db.units:
            u_err = ltdb.Error(DType.UNITS, unit.nid)
            res.errors += self._val_or_err(unit.get_items(), DType.ITEMS, u_err.field("Starting Items"))
            res.errors += self._val_or_err(unit.get_skills(), DType.SKILLS, u_err.field("Learned Skills"))
            res.errors += self._val_or_err(unit.klass, DType.CLASSES, u_err.field("Class"))
            res.errors += self._val_or_err(unit.alternate_classes, DType.CLASSES, u_err.field("Alternate Classes"))
        return res

    def validate_events(self) -> ValidationResult:
        alz = PyEventAnalyzer(self.db.events)
        all_errors = []
        for event in self.db.events:
            if event.version() != EventVersion.EVENT:
                all_errors += alz.verify_event(event.nid, event.source)
        return ValidationResult(all_errors)

    def _val_or_err(self, nids: List[NID] | NID, dtype: LTType, parent_error: ltdb.Error, optional=True) -> List[ltdb.Error]:
        if not isinstance(nids, list):
            nids = [nids]
        if optional:
            nids = [nid for nid in nids if nid]
        return [parent_error.tmplt.not_type(dtype, nid) for nid in nids if not self.dve.validate(dtype, nid)]

    def validate_for_errors(self) -> List:
        all_errors = self.validate_events().errors
        all_errors += self.validate_items_and_skills().errors
        all_errors += self.validate_levels().errors
        all_errors += self.validate_units().errors
        return all_errors
