from __future__ import annotations

from dataclasses import dataclass, field
from typing import TYPE_CHECKING, Dict, List, Optional, Tuple

from app.data.database.database import DB
from app.data.database.difficulty_modes import GrowthOption
from app.data.database.level_units import GenericUnit, UniqueUnit
from app.data.database.units import UnitPrefab
from app.engine import (combat_calcs, equations, item_funcs, item_system,
                        skill_system, unit_funcs)
from app.engine.objects.difficulty_mode import DifficultyModeObject
from app.engine.objects.item import ItemObject
from app.engine.objects.skill import SkillObject
from app.engine.source_type import SourceType
from app.utilities import utils
from app.utilities.data import Prefab
from app.utilities.typing import NID
from typing import Union

if TYPE_CHECKING:
    from app.engine.unit_sound import UnitSound
    from app.engine.unit_sprite import UnitSprite

import logging

@dataclass
class UnitSkill():
    skill_obj: SkillObject
    source: Union[str, tuple, int]
    source_type: tuple

    def __init__(self, skill_obj, source = None, source_type = SourceType.DEFAULT):
        self.skill_obj = skill_obj
        self.source = source
        self.source_type = source_type

    def get(self):
        return self.skill_obj

# Main unit object used by engine
@dataclass
class UnitObject(Prefab):
    nid: NID
    prefab_nid: NID = None
    generic: bool = False
    persistent: bool = True
    ai: str = None
    ai_group: NID = None
    roam_ai: str = None
    faction: NID = None
    team: NID = "player"
    portrait_nid: NID = None
    affinity: NID = None
    notes: List[Tuple[str, str]] = field(default_factory=list)
    _fields: Dict[str, str] = field(default_factory=dict)
    klass: NID = None
    variant: str = None

    name: str = None
    desc: str = None
    _tags: List[str] = field(default_factory=list)
    party: NID = None
    level: int = 1
    exp: int = 0
    stats: Dict[NID, int] = field(default_factory=dict)
    growths: Dict[NID, int] = field(default_factory=dict)
    growth_points: Dict[NID, int] = field(default_factory=dict)
    stat_cap_modifiers: Dict[NID, int] = field(default_factory=dict)
    wexp: Dict[NID, int] = field(default_factory=dict)

    position: Tuple[int, int] = None
    starting_position: Tuple[int, int] = None
    previous_position: Tuple[int, int] = None
    current_hp: int = 0
    current_mana: int = 0
    current_fatigue: int = 0
    _movement_left: int = 0
    current_guard_gauge: int = 0

    traveler: NID = None
    strike_partner: UnitObject = None
    lead_unit: bool = False
    built_guard: bool = False

    dead: bool = False
    is_dying: bool = False
    _finished: bool = False
    _has_attacked: bool = False
    _has_traded: bool = False
    _has_moved: bool = False

    items: List[ItemObject] = field(default_factory=list)
    equipped_weapon: ItemObject = None
    equipped_accessory: ItemObject = None

    _skills: List[UnitSkill] = field(default_factory=list)
    _visible_skills_cache: List[SkillObject] = field(default_factory=list)

    has_rescued: bool = False
    has_taken: bool = False
    has_given: bool = False
    has_dropped: bool = False

    has_run_ai: bool = False

    _sprite = None
    _sound = None
    _battle_anim = None
    current_move = None

    # If the attribute is not found
    def __getattr__(self, attr):
        if attr.startswith('__') and attr.endswith('__'):
            return super().__getattr__(attr)
        elif self.nid:
            prefab = DB.units.get(self.prefab_nid)
            if prefab:
                return getattr(prefab, attr)
        # not in prefab, so...
        raise AttributeError('UnitObject has no attribute %s' % attr)

    @classmethod
    def from_prefab(cls, prefab: UniqueUnit | GenericUnit | UnitPrefab, current_mode: DifficultyModeObject = None, new_nid=None):
        new_nid = new_nid or prefab.nid
        self = cls(new_nid)
        is_level_unit = not isinstance(prefab, UnitPrefab)
        self.nid = new_nid
        self.prefab_nid = prefab.nid
        if not is_level_unit: # initing a non-level unit
            self.generic = False
            self.persistent = True
            self.ai = None
            self.roam_ai = None
            self.ai_group = None
            self.faction = None
            self.team = 'player'
        else:
            self.generic = prefab.generic
            self.persistent = not prefab.generic
            self.ai = prefab.ai
            self.roam_ai = prefab.roam_ai
            self.ai_group = prefab.ai_group
            self.faction = prefab.faction
            self.team = prefab.team
        self.portrait_nid = prefab.portrait_nid if not self.generic else None
        self.affinity = prefab.affinity if not self.generic else None
        self.notes = [(n[0], n[1]) for n in prefab.unit_notes] if not self.generic else []
        self._fields = {key: value for (key, value) in prefab.fields} if not self.generic else {}
        self.klass = prefab.klass
        self.variant = prefab.variant

        self.name = prefab.name
        self.desc = prefab.desc
        self._tags = [tag for tag in prefab.tags] if not self.generic else []
        self.party = None

        if is_level_unit:
            if prefab.starting_position:
                self.position = self.previous_position = tuple(prefab.starting_position)
            else:
                self.position = self.previous_position = None
        self.starting_position = self.position
        self.level = prefab.level

        self.exp = 0

        if self.generic:
            klass_obj = DB.classes.get(self.klass)
            bases = klass_obj.bases
            growths = klass_obj.growths
            self.stats = {stat_nid: bases.get(stat_nid, 0) for stat_nid in DB.stats.keys()}
            self.growths = {stat_nid: growths.get(stat_nid, 0) for stat_nid in DB.stats.keys()}
            # Generics have defualt stat cap modifiers of 0
            self.stat_cap_modifiers = {stat_nid: 0 for stat_nid in DB.stats.keys()}
            weapon_gain = klass_obj.wexp_gain
            self.wexp = {
                weapon_nid:
                utils.clamp(weapon_gain.get(weapon_nid, DB.weapons.default(DB)).wexp_gain, 0, unit_funcs.get_weapon_cap(self, weapon_nid))
                for weapon_nid in DB.weapons.keys()
            }
        else:
            bases = prefab.bases
            growths = prefab.growths
            stat_cap_modifiers = prefab.stat_cap_modifiers
            self.stats = {stat_nid: bases.get(stat_nid, 0) for stat_nid in DB.stats.keys()}
            self.growths = {stat_nid: growths.get(stat_nid, 0) for stat_nid in DB.stats.keys()}
            if DB.constants.value('unit_stats_as_bonus'):
                klass_obj = DB.classes.get(self.klass)
                self.stats = {stat_nid: self.stats[stat_nid] + klass_obj.bases.get(stat_nid, 0) for stat_nid in DB.stats.keys()}
                self.growths = {stat_nid: self.growths[stat_nid] + klass_obj.growths.get(stat_nid, 0) for stat_nid in DB.stats.keys()}
            self.stat_cap_modifiers = {stat_nid: stat_cap_modifiers.get(stat_nid, 0) for stat_nid in DB.stats.keys()}
            weapon_gain = prefab.wexp_gain
            self.wexp = {
                weapon_nid:
                utils.clamp(weapon_gain.get(weapon_nid, DB.weapons.default(DB)).wexp_gain, 0, unit_funcs.get_weapon_cap(self, weapon_nid))
                for weapon_nid in DB.weapons.keys()
            }

        # status bools
        self.dead = False
        self.is_dying = False
        self._finished = False
        self._has_attacked = False
        self._has_traded = False
        self._has_moved = False

        self.has_rescued = False
        self.has_taken = False
        self.has_given = False
        self.has_dropped = False

        self.has_run_ai = False

        self._sprite = None
        self._sound = None
        self._battle_anim = None

        self.current_move = None

        if is_level_unit:
            method = unit_funcs.get_leveling_method(self)
        else:
            method = GrowthOption.FIXED

        if method == GrowthOption.FIXED:
            self.growth_points = {k: 50 for k in self.stats.keys()}
        else:
            self.growth_points = {k: 0 for k in self.stats.keys()}

        self.traveler = prefab.starting_traveler if is_level_unit else None  # Always a nid of a unit
        self.strike_partner = None
        self.lead_unit = False
        self.built_guard = False

        self.current_hp = self.get_max_hp()
        self.current_mana = self.get_max_mana()
        self.current_fatigue = 0
        self._movement_left = equations.parser.movement(self)
        self.current_guard_gauge = 0

        # Handle items
        if is_level_unit:
            items = item_funcs.create_items(self, prefab.starting_items)
            for item in items:
                self.add_item(item)

            if self.generic:
                self.calculate_needed_wexp_from_items()

            # Handle skills
            all_skills = []
            global_skills = unit_funcs.get_global_skills(self)
            for s in global_skills:
                all_skills.append(UnitSkill(s, 'game', SourceType.GLOBAL))
            personal_skills = unit_funcs.get_personal_skills(self, prefab)
            for s in personal_skills:
                all_skills.append(UnitSkill(s, self.nid, SourceType.PERSONAL))
            class_skills = unit_funcs.get_starting_skills(self)
            for s in class_skills:
                all_skills.append(UnitSkill(s, self.klass, SourceType.KLASS))
            if self.generic:
                generic_skills = item_funcs.create_skills(self, prefab.starting_skills)
                for s in generic_skills:
                    all_skills.append(UnitSkill(s, self.nid, SourceType.PERSONAL))
            for s in all_skills:
                skill_system.before_add(self, s.get())
                self._skills.append(s)

        klass = DB.classes.get(self.klass)
        if klass.tier == 0:
            num_levels = self.level - 1
        else:
            num_levels = self.get_internal_level() - 1

        # Difficulty mode stat bonuses
        if current_mode:
            mode = DB.difficulty_modes.get(current_mode.nid)
            if klass.tier >= 2:
                prev_levels = num_levels - (self.level - 1)
                num_levels = self.level + int(prev_levels * mode.promoted_autolevels_fraction)
            stat_bonus = mode.get_base_bonus(self, DB)
            bonus = {nid: 0 for nid in DB.stats.keys()}
            for nid in DB.stats.keys():
                max_stat = klass.max_stats.get(nid, 30) + self.stat_cap_modifiers.get(nid, 0)
                bonus[nid] = utils.clamp(stat_bonus.get(nid, 0), -self.stats.get(nid, 0), max_stat - self.stats.get(nid, 0))
            if any(v != 0 for v in bonus.values()):
                unit_funcs.apply_stat_changes(self, bonus)

            if self.generic:
                unit_funcs.auto_level(self, 1, num_levels)
            # Existing units would have leveled up different with bonus growths
            elif DB.constants.value('backpropagate_difficulty_growths'):
                difficulty_growth_bonus = mode.get_growth_bonus(self, DB)
                if difficulty_growth_bonus:
                    unit_funcs.difficulty_auto_level(self, 1, num_levels)

            difficulty_autolevels = mode.get_difficulty_autolevels(self, DB)
            # Handle the ones that you can change in events
            if self.team in DB.teams.enemies:
                difficulty_autolevels += current_mode.enemy_autolevels
                difficulty_autolevels += current_mode.enemy_truelevels
            if 'Boss' in self.tags:
                difficulty_autolevels += current_mode.boss_autolevels
                difficulty_autolevels += current_mode.boss_truelevels

            if difficulty_autolevels > 0:
                unit_funcs.auto_level(self, 1, difficulty_autolevels)

            if self.team in DB.teams.enemies:
                self.level += current_mode.enemy_truelevels
            if 'Boss' in self.tags:
                self.level += current_mode.boss_truelevels

        # equip items and skill after initialization
        for s in self._skills:
            skill_system.after_add(self, s.get())
        self._visible_skills_cache.clear()

        # -- Equipped Items
        self.autoequip()

        # Reset these so max hp can be changed by skills and items
        self.current_hp = self.get_max_hp()
        self.current_mana = self.get_max_mana()

        return self

    def get_max_hp(self):
        return equations.parser.hitpoints(self)

    def get_hp(self):
        return self.current_hp

    def set_hp(self, val):
        self.current_hp = int(utils.clamp(val, 0, equations.parser.hitpoints(self)))

    def get_max_mana(self):
        return equations.parser.get_mana(self)

    def get_mana(self) -> int:
        return self.current_mana

    def set_mana(self, val):
        self.current_mana = int(utils.clamp(val, 0, equations.parser.get_mana(self)))

    def get_max_fatigue(self):
        return equations.parser.max_fatigue(self)

    def get_fatigue(self):
        return self.current_fatigue

    def set_fatigue(self, val):
        self.current_fatigue = int(max(val, 0))

    def get_guard_gauge(self):
        return self.current_guard_gauge

    def get_max_guard_gauge(self):
        return equations.parser.get_max_guard(self)

    def set_guard_gauge(self, val):
        self.current_guard_gauge = int(utils.clamp(val, 0, self.get_max_guard_gauge()))

    def get_gauge_inc(self):
        return equations.parser.get_gauge_inc(self)

    def get_field(self, key: str, default: str = None) -> str:
        if key in self._fields:
            return self._fields[key]
        my_klass = DB.classes.get(self.klass, None)
        if my_klass:
            klass_property_dict = dict(my_klass.fields)
            if key in klass_property_dict:
                return klass_property_dict[key]
        return default

    def set_field(self, key: str, value: str):
        self._fields[key] = value

    def get_exp(self) -> int:
        return self.exp

    def set_exp(self, val: int) -> int:
        self.exp = int(utils.clamp(val, 0, 100))

    def add_skill(self, skill, source=None, source_type=SourceType.DEFAULT, test=False):
        """
        # Adds skill to the UnitSkill list while checking if the skill already exists/stack is full
        # If so, removes the oldest displaceable skill and returns it
        # If no existing skill is displaceable AND the new skill is displaceable, returns the new skill back
        # Only actually adds the new skill on test=False
        """
        popped_skill = None
        stack_value = skill.stack.value if skill.stack else 1
        # Checks if we already have the max allowable number of the skill
        if item_funcs.num_stacks(self, skill.nid) >= stack_value:
            # Gets all skills of the same ID that can be displaced
            displaceable_skills = [s.skill_obj for s in self._skills if s.skill_obj.nid == skill.nid and s.source_type.displaceable]
            if len(displaceable_skills) == 0 and source_type.displaceable:
                popped_skill = skill
            # Returns back the input skill only if it can't be added
            if len(displaceable_skills) > 0:
                popped_skill = displaceable_skills[0]
        if not test:
            self._skills.append(UnitSkill(skill, source, source_type))
            self._visible_skills_cache.clear()
        return popped_skill

    def remove_skill(self, skill, source, source_type=SourceType.DEFAULT, test=False):
        """
        # Removes the given skill and returns it along with its source and source type
        # If the given skill cannot be removed, returns nothing
        # Only actually removes the skill on test=False
        """
        removed_skill_info = None
        to_remove = None
        for s in self._skills:
            same_source = s.source == source and s.source_type == source_type
            if s.skill_obj.uid == skill.uid and \
            (s.source_type.removable or same_source):
                removed_skill_info = (s.source, s.source_type)
                to_remove = s
        if not test and to_remove:
            self._skills.remove(to_remove)
            self._visible_skills_cache.clear()
        return removed_skill_info

    @property
    def all_skills(self):
        return [s.get() for s in self._skills]

    @property
    def skills(self):
        """
        # Returns a list of the actionable skills
        # that aren't being shadowed by other more recently added skills
        # Has a cache that is reset when a skill is added or removed from _skills
        """
        if self._visible_skills_cache:
            return self._visible_skills_cache

        skills = []
        skill_nids = set()
        # reversed so that more recently added skills take priority
        for skill in reversed([s.get() for s in self._skills]):
            if skill.stack:
                if sum([s.nid == skill.nid for s in skills]) >= skill.stack.value:
                    pass
                else:
                    skills.append(skill)
                    skill_nids.add(skill.nid)
            elif skill.nid in skill_nids:
                # already shadowed by a later skill
                pass
            else:
                skills.append(skill)
                skill_nids.add(skill.nid)
        skills = list(reversed(skills)) # Reverse back to correct direction
        self._visible_skills_cache = skills
        return skills

    def stat_bonus(self, stat_nid: NID) -> int:
        bonus = skill_system.stat_change(self, stat_nid)
        weapon = self.equipped_weapon
        if weapon:
            bonus += item_system.stat_change(self, weapon, stat_nid)
        accessory = self.equipped_accessory
        if accessory:
            bonus += item_system.stat_change(self, accessory, stat_nid)
        return bonus

    def subtle_stat_bonus(self, stat_nid: NID) -> int:
        bonus = skill_system.subtle_stat_change(self, stat_nid)
        return bonus

    def stat_contribution(self, stat_nid: NID) -> list:
        contribution = skill_system.stat_change_contribution(self, stat_nid)
        weapon = self.equipped_weapon
        if weapon:
            contribution.update(item_system.stat_change_contribution(self, weapon, stat_nid))
        accessory = self.equipped_accessory
        if accessory:
            contribution.update(item_system.stat_change_contribution(self, accessory, stat_nid))
        return contribution

    def get_stat(self, stat_nid: NID) -> int:
        return self.stats.get(stat_nid, 0) + self.stat_bonus(stat_nid)

    def growth_bonus(self, stat_nid: NID) -> int:
        return skill_system.growth_change(self, stat_nid)

    def get_growth(self, stat_nid: NID) -> int:
        return self.growths.get(stat_nid, 0) + self.growth_bonus(stat_nid)

    def get_stat_cap(self, stat_nid: NID) -> int:
        return DB.classes.get(self.klass).max_stats.get(stat_nid, 30) + self.stat_cap_modifiers.get(stat_nid, 0)

    def get_damage_with_current_weapon(self) -> int:
        if self.get_weapon():
            return combat_calcs.damage(self, self.get_weapon())
        else:
            return 0

    def get_accuracy_with_current_weapon(self) -> int:
        if self.get_weapon():
            return combat_calcs.accuracy(self, self.get_weapon())
        else:
            return 0

    def get_avoid_with_current_weapon(self) -> int:
        return combat_calcs.avoid(self, self.get_weapon())

    @property
    def sprite(self):
        if not self._sprite:
            from app.engine import unit_sprite
            self._sprite = unit_sprite.UnitSprite(self)
        return self._sprite

    def reset_sprite(self):
        self._sprite = None
        self._sound = None
        self._battle_anim = None

    @property
    def battle_anim(self):
        return None

    @property
    def sound(self):
        if not self._sound:
            from app.engine import unit_sound
            self._sound = unit_sound.UnitSound(self)
        return self._sound

    @property
    def tags(self):
        return set(self._tags) | set(DB.classes.get(self.klass).tags) | skill_system.additional_tags(self)

    def get_ai(self):
        return skill_system.change_ai(self)

    def get_roam_ai(self):
        return skill_system.change_roam_ai(self)

    @property
    def accessories(self):
        return [item for item in self.items if item_system.is_accessory(self, item)]

    @property
    def nonaccessories(self):
        return [item for item in self.items if not item_system.is_accessory(self, item)]

    @property
    def movement_left(self):
        if not self.has_moved:
            return equations.parser.movement(self)
        else:
            return self._movement_left

    @movement_left.setter
    def movement_left(self, val):
        self._movement_left = val

    def consume_movement(self, val):
        self._movement_left -= val

    def calculate_needed_wexp_from_items(self):
        for item in item_funcs.get_all_items(self):
            weapon_rank_required = item_system.weapon_rank(self, item)
            if weapon_rank_required:
                weapon_type = item_system.weapon_type(self, item)
                requirement = DB.weapon_ranks.get(weapon_rank_required).requirement
                self.wexp[weapon_type] = max(self.wexp[weapon_type], requirement)

    def can_unlock(self, region) -> bool:
        return unit_funcs.can_unlock(self, region)

    def get_skill(self, nid: NID):
        skills = [skill for skill in reversed(self.all_skills) if skill.nid == nid or skill.uid == nid]
        if skills:
            return skills[0]
        return None

    def get_weapon(self) -> Optional[ItemObject]:
        return self.equipped_weapon

    def get_accessory(self) -> Optional[ItemObject]:
        return self.equipped_accessory

    def can_equip(self, item: ItemObject) -> bool:
        return item_system.equippable(self, item) and item_funcs.available(self, item)

    def autoequip(self):
        logging.debug("Autoequipping...")
        all_items = item_funcs.get_all_items(self)
        # Do an an initial check that the weapon is still good
        if self.equipped_weapon and not self.can_equip(self.equipped_weapon):
            self.unequip(self.equipped_weapon)
        if not self.equipped_weapon:
            for item in all_items:
                if not item_system.is_accessory(self, item):
                    if self.can_equip(item):
                        self.equip(item)
                        break
        if self.equipped_accessory and not self.can_equip(self.equipped_accessory):
            self.unequip(self.equipped_accessory)
        if not self.equipped_accessory:
            for item in all_items:
                if item_system.is_accessory(self, item):
                    if self.can_equip(item):
                        self.equip(item)
                        break
        # keep accessories sorted after items
        self.items = sorted(self.items, key=lambda item: item_system.is_accessory(self, item))

    def equip(self, item):
        if item_system.is_accessory(self, item) and item is self.equipped_accessory:
            return  # Don't need to do anything
        elif item is self.equipped_weapon:
            return  # Don't need to do anything
        logging.debug("Equipping %s" % item)
        if item_system.is_accessory(self, item):
            if self.equipped_accessory:
                self.unequip(self.equipped_accessory, item)
            self.equipped_accessory = item
        else:
            if self.equipped_weapon:
                self.unequip(self.equipped_weapon, item)
            self.equipped_weapon = item
        item_system.on_equip_item(self, item)
        skill_system.on_equip_item(self, item)

    def unequip(self, item, swap_to=None):
        if item is self.equipped_weapon or item is self.equipped_accessory:
            logging.debug("Unequipping %s" % item)
            if item_system.is_accessory(self, item):
                self.equipped_accessory = swap_to
            else:
                self.equipped_weapon = swap_to
            skill_system.on_unequip_item(self, item)
            item_system.on_unequip_item(self, item)

    def add_item(self, item):
        index = len(self.items)
        self.insert_item(index, item)

    def bring_to_top_item(self, item):
        if item_system.is_accessory(self, item):
            self.items.remove(item)
            self.items.insert(len(self.nonaccessories), item)
        else:
            self.items.remove(item)
            self.items.insert(0, item)

    def insert_item(self, index, item):
        logging.debug("Unit insert_item %s at %s" % (item, index))
        if item in self.items:
            self.items.remove(item)
            self.items.insert(index, item)
        else:
            self.items.insert(index, item)
            item.change_owner(self.nid)
            # Statuses here
            item_system.on_add_item(self, item)
            skill_system.on_add_item(self, item)

    def remove_item(self, item):
        # Remove item before we unequip, so that the autoequip does not
        # re-equip the item
        logging.debug("Unit remove_item %s" % item)
        self.items.remove(item)
        if self.equipped_weapon is item or self.equipped_accessory is item:
            self.unequip(item)
        if item.multi_item:
            if self.equipped_weapon in item_funcs.get_all_items_from_multi_item(self, item):
                self.unequip(self.equipped_weapon)
            elif self.equipped_accessory in item_funcs.get_all_items_from_multi_item(self, item):
                self.unequip(self.equipped_accessory)
        item.change_owner(None)
        # Status effects
        skill_system.on_remove_item(self, item)
        item_system.on_remove_item(self, item)

    def get_internal_level(self) -> int:
        klass = DB.classes.get(self.klass)
        if klass.tier == 0:
            return self.level - klass.max_level
        elif klass.tier == 1:
            return self.level
        else:
            running_total = self.level
            # Need do while
            counter = 5
            while counter > 0:
                counter -= 1  # Just to make sure no infinite loop
                promotes_from = klass.promotes_from
                if promotes_from:
                    klass = DB.classes.get(promotes_from)
                    running_total += klass.max_level
                else:
                    return running_total
                if klass.tier <= 0:
                    return running_total
            return running_total

    def wait(self, actively_chosen: bool = False):
        unit_funcs.wait(self, actively_chosen)

    @property
    def finished(self):
        return self._finished

    @property
    def has_attacked(self):
        return self._finished or self._has_attacked

    @property
    def has_traded(self):
        return self._finished or self._has_attacked or self._has_traded

    @property
    def has_moved(self):
        return self._finished or self._has_attacked or self._has_traded or self._has_moved

    @property
    def has_moved_any_distance(self):
        return self.position != self.previous_position

    @finished.setter
    def finished(self, val):
        self._finished = val

    @has_attacked.setter
    def has_attacked(self, val):
        self._has_attacked = val

    @has_traded.setter
    def has_traded(self, val):
        self._has_traded = val

    @has_moved.setter
    def has_moved(self, val):
        self._has_moved = val

    def get_action_state(self):
        return (self._finished, self._has_attacked, self._has_traded, self._has_moved,
                self.has_rescued, self.has_dropped, self.has_taken, self.has_given)

    def set_action_state(self, state):
        self._finished = state[0]
        self._has_attacked = state[1]
        self._has_traded = state[2]
        self._has_moved = state[3]

        self.has_rescued = state[4]
        self.has_dropped = state[5]
        self.has_taken = state[6]
        self.has_given = state[7]

    def reset(self):
        self._finished = False
        self._has_attacked = False
        self._has_traded = False
        self._has_moved = False

        self.has_rescued = False
        self.has_dropped = False
        self.has_taken = False
        self.has_given = False
        self.has_run_ai = False

        self.strike_partner = None

    def __repr__(self):
        return "Unit %s: %s" % (self.nid, self.position)

    def save(self):
        s_dict = {'nid': self.nid,
                  'prefab_nid': self.prefab_nid,
                  'position': self.position,
                  'team': self.team,
                  'party': self.party,
                  'klass': self.klass,
                  'variant': self.variant,
                  'faction': self.faction,
                  'level': self.level,
                  'exp': self.exp,
                  'generic': self.generic,
                  'persistent': self.persistent,
                  'ai': self.ai,
                  'roam_ai': self.roam_ai,
                  'ai_group': self.ai_group,
                  'items': [item.uid for item in self.items],
                  'name': self.name,
                  'desc': self.desc,
                  'tags': self._tags,
                  'stats': self.stats,
                  'growths': self.growths,
                  'growth_points': self.growth_points,
                  'stat_cap_modifiers': self.stat_cap_modifiers,
                  'starting_position': self.starting_position,
                  'wexp': self.wexp,
                  'portrait_nid': self.portrait_nid,
                  'affinity': self.affinity,
                  'skills': [(skill_info.skill_obj.uid, skill_info.source, skill_info.source_type) for skill_info in self._skills],
                  'notes': self.notes,
                  'current_hp': self.current_hp,
                  'current_mana': self.current_mana,
                  'current_fatigue': self.current_fatigue,
                  'traveler': self.traveler,
                  'current_guard_gauge': self.current_guard_gauge,
                  'built_guard': self.built_guard,
                  'dead': self.dead,
                  'action_state': self.get_action_state(),
                  '_fields': self._fields,
                  'equipped_weapon': self.equipped_weapon.uid if self.equipped_weapon else None,
                  'equipped_accessory': self.equipped_accessory.uid if self.equipped_accessory else None,
                  }
        return s_dict

    @classmethod
    def restore(cls, s_dict, game):
        self = cls(s_dict['nid'])
        self.prefab_nid = s_dict.get('prefab_nid', s_dict['nid'])
        if s_dict['position']:
            self.position = self.previous_position = tuple(s_dict['position'])
        else:
            self.position = self.previous_position = None
        self.team = s_dict['team']
        self.party = s_dict['party']
        self.klass = s_dict['klass']
        self.variant = s_dict['variant']
        self.level = s_dict['level']
        self.exp = s_dict['exp']
        self.generic = s_dict['generic']
        self.persistent = s_dict.get('persistent', not s_dict.get('generic'))

        self.ai = s_dict['ai']
        self.roam_ai = s_dict.get('roam_ai', None)
        self.ai_group = s_dict.get('ai_group', None)

        self.items = [game.get_item(item_uid) for item_uid in s_dict['items']]
        self.items = [i for i in self.items if i]

        self.faction = s_dict['faction']
        self.name = s_dict['name']
        self.desc = s_dict['desc']
        self._tags = s_dict['tags']
        self.stats = s_dict['stats']
        self.growths = s_dict['growths']
        self.growth_points = s_dict['growth_points']
        self.stat_cap_modifiers = s_dict.get('stat_cap_modifiers', {})
        self.wexp = s_dict['wexp']
        self.portrait_nid = s_dict['portrait_nid']
        self.affinity = s_dict.get('affinity', None)
        self.notes = s_dict.get('notes', [])
        if s_dict['starting_position']:
            self.starting_position = tuple(s_dict['starting_position'])
        else:
            self.starting_position = None
        self._fields = s_dict.get('_fields', {})

        self.equipped_weapon = None
        self.equipped_accessory = None

        self._skills = [UnitSkill(game.get_skill(skill_uid), source, source_type) for skill_uid, source, source_type in s_dict['skills']]
        self._skills = [s for s in self._skills if s.get()]

        self.current_hp = s_dict['current_hp']
        self.current_mana = s_dict['current_mana']
        self.current_fatigue = s_dict['current_fatigue']
        self._movement_left = equations.parser.movement(self)
        self.current_guard_gauge = s_dict.get('current_guard_gauge', 0)

        self.traveler = s_dict['traveler']
        self.strike_partner = None
        self.lead_unit = False
        self.built_guard = s_dict.get('built_guard', False)

        equipped_weapon_uid = s_dict.get('equipped_weapon')
        if equipped_weapon_uid is not None:
            self.equipped_weapon = game.get_item(equipped_weapon_uid)
        else:
            self.equipped_weapon = self.get_weapon()

        equipped_accessory_uid = s_dict.get('equipped_accessory')
        if equipped_accessory_uid is not None:
            self.equipped_accessory = game.get_item(equipped_accessory_uid)
        else:
            self.equipped_accessory = self.get_accessory()

        # -- Other properties
        self.dead = s_dict['dead']
        self.is_dying = False
        action_state = s_dict.get('action_state')
        if action_state:
            self.set_action_state(action_state)
        else:
            self.reset()
        self.has_run_ai = False

        self._sprite = None
        self._sound = None
        self._battle_anim = None

        self.current_move = None  # Holds the move action the unit last used
        # Maybe move to movement manager?

        for s in self._skills:
            skill_system.after_add_from_restore(self, s.get())
        self._visible_skills_cache.clear()

        return self

    def __hash__(self):
        return hash(self.nid)

    def __eq__(self, other: UnitObject) -> bool:
        return isinstance(other, UnitObject) and self.nid == other.nid