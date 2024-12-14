from dataclasses import dataclass
from typing import ClassVar, Tuple

from app.engine.objects.unit import UnitObject
from app.engine.objects.item import ItemObject
from app.engine.objects.skill import SkillObject


@dataclass
class PlaybackBrush:
    """
    Generic playback brush class
    """
    nid: ClassVar[str] = 'playback_brush'


@dataclass
class AttackerPhase(PlaybackBrush):
    """
    Added to playback when the attacker phase would begin
    """
    nid: ClassVar[str] = 'attacker_phase'


@dataclass
class AttackerPartnerPhase(PlaybackBrush):
    """
    Added to playback when the attacker's partner's phase would begin (only with Pair Up enabled)
    """
    nid: ClassVar[str] = 'attacker_partner_phase'


@dataclass
class DefenderPhase(PlaybackBrush):
    """
    Added to playback when the main defender's phase would begin
    """
    nid: ClassVar[str] = 'defender_phase'


@dataclass
class DefenderPartnerPhase(PlaybackBrush):
    """
    Added to playback when the defender's partner's phase would begin (only with Pair Up enabled)
    """
    nid: ClassVar[str] = 'defender_partner_phase'


@dataclass
class AttackPreProc(PlaybackBrush):
    """
    Added to playback when an attacker's pre proc effect should happen (processed at beginning of combat)
    Used for combat arts, as an example
    Automatically adds Icon to screen when proccing
    In Animation Combat, will add effect with same name as skill.nid
    """
    nid: ClassVar[str] = 'attack_pre_proc'
    unit: UnitObject
    skill: SkillObject  # Skill that is proccing


@dataclass
class DefensePreProc(PlaybackBrush):
    """
    Added to playback when a defender's pre proc effect should happen (processed at beginning of combat)
    Automatically adds Icon to screen when proccing
    In Animation Combat, will add effect with same name as skill.nid
    """
    nid: ClassVar[str] = 'defense_pre_proc'
    unit: UnitObject
    skill: SkillObject  # Skill that is proccing


@dataclass
class AttackProc(PlaybackBrush):
    """
    Added to playback when an attacker's proc effect should happen (processed at beginning of phase)
    Automatically adds Icon to screen when proccing
    In Animation Combat, will add effect with same name as skill.nid
    """
    nid: ClassVar[str] = 'attack_proc'
    unit: UnitObject
    skill: SkillObject  # Skill that is proccing


@dataclass
class DefenseProc(PlaybackBrush):
    """
    Added to playback when a defender's proc effect should happen (processed at beginning of phase)
    Automatically adds Icon to screen when proccing
    In Animation Combat, will add effect with same name as skill.nid
    """
    nid: ClassVar[str] = 'defense_proc'
    unit: UnitObject
    skill: SkillObject  # Skill that is proccing


@dataclass
class DefenseHitProc(PlaybackBrush):
    """
    Added to playback when a defender's proc effect should happen (processed at beginning of phase)
    Automatically adds Icon to screen when the hit strikes
    In Animation Combat, will add effect with same name as skill.nid
    """
    nid: ClassVar[str] = 'defense_hit_proc'
    unit: UnitObject
    skill: SkillObject  # Skill that is proccing


@dataclass
class AttackHitProc(PlaybackBrush):
    """
    Added to playback when a defender's proc effect should happen (processed at beginning of phase)
    Automatically adds Icon to screen when the hit strikes
    In Animation Combat, will add effect with same name as skill.nid
    """
    nid: ClassVar[str] = 'attack_hit_proc'
    unit: UnitObject
    skill: SkillObject  # Skill that is proccing


@dataclass
class MarkHit(PlaybackBrush):
    """
    Added to playback when an attack successfully hits
    """
    nid: ClassVar[str] = 'mark_hit'
    # The actual attacker for this attack, which may not be the main attacker (e.g., pair up)
    attacker: UnitObject
    defender: UnitObject
    main_attacker: UnitObject
    # The actual item used for this attack, which may not be the main item used for this combat (e.g., multi-items)
    item: ItemObject
    # Whether the attack was guarded (only posible with pairup)
    guard_hit: bool = False


@dataclass
class MarkCrit(PlaybackBrush):
    """
    Added to playback when an attack successfully crits
    MarkHit is generally not also included
    """
    nid: ClassVar[str] = 'mark_crit'
    # The actual attacker for this attack, which may not be the main attacker (e.g., pair up)
    attacker: UnitObject
    defender: UnitObject
    main_attacker: UnitObject
    # The actual item used for this attack, which may not be the main item used for this combat (e.g., multi-items)
    item: ItemObject


@dataclass
class MarkMiss(PlaybackBrush):
    """
    Added to playback when an attack misses
    """
    nid: ClassVar[str] = 'mark_miss'
    # The actual attacker for this attack, which may not be the main attacker (e.g., pair up)
    attacker: UnitObject
    defender: UnitObject
    main_attacker: UnitObject
    # The actual item used for this attack, which may not be the main item used for this combat (e.g., multi-items)
    item: ItemObject


@dataclass
class MarkGlancingHit(PlaybackBrush):
    """
    Added to playback when an attack does a glancing hit
    MarkHit is always also included in playback
    """
    nid: ClassVar[str] = 'mark_glancing_hit'
    # The actual attacker for this attack, which may not be the main attacker (e.g., pair up)
    attacker: UnitObject
    defender: UnitObject
    main_attacker: UnitObject
    # The actual item used for this attack, which may not be the main item used for this combat (e.g., multi-items)
    item: ItemObject


@dataclass
class DamageHit(PlaybackBrush):
    """
    Added to playback when an attack does damage
    Also utilized to generate damage numbers
    Processed by other components to determine whether damage was actually dealt (e.g, Lifelink, LevelExp)
    """
    nid: ClassVar[str] = 'damage_hit'
    attacker: UnitObject
    # The actual item used for this attack, which may not be the main item used for this combat (e.g., multi-items)
    item: ItemObject
    defender: UnitObject
    damage: int  # attempted damage number
    # actual damage dealt (cannot be more than defender's current HP)
    true_damage: int


@dataclass
class DamageCrit(PlaybackBrush):
    """
    Added to playback when an attack does damage from a critical strike
    Also utilized to generate damage numbers
    Processed by other components to determine whether damage was actually dealt (e.g, Lifelink, LevelExp)
    """
    nid: ClassVar[str] = 'damage_crit'
    attacker: UnitObject
    # The actual item used for this attack, which may not be the main item used for this combat (e.g., multi-items)
    item: ItemObject
    defender: UnitObject
    damage: int  # attempted damage number
    # actual damage dealt (cannot be more than defender's current HP)
    true_damage: int


@dataclass
class HealHit(PlaybackBrush):
    """
    Added to playback when an attack heals
    Also utilized to generate damage numbers
    Processed by other components to determine whether damage was actually healed (e.g, LiveToServer, HealExp)
    """
    nid: ClassVar[str] = 'heal_hit'
    attacker: UnitObject
    # The actual item used for this attack, which may not be the main item used for this combat (e.g., multi-items)
    item: ItemObject
    defender: UnitObject
    damage: int  # attempted number of hp points healed
    # Actual number of hp points healed (cannot be more than defender's missing HP)
    true_damage: int


@dataclass
class RescueHit(PlaybackBrush):
    """
    Added to playback when an attack rescues/warps
    Doesn't seem to actually be used for anything
    """
    nid: ClassVar[str] = 'rescue_hit'
    attacker: UnitObject
    item: ItemObject
    defender: UnitObject


@dataclass
class StatHit(PlaybackBrush):
    """
    Added to playback when an attack modifies the stats of target
    Only used in PermanentStatChange ItemComponent
    """
    nid: ClassVar[str] = 'stat_hit'
    attacker: UnitObject
    item: ItemObject
    defender: UnitObject


@dataclass
class StatusHit(PlaybackBrush):
    """
    Added to playback when an attack addeds a status to the target
    """
    nid: ClassVar[str] = 'status_hit'
    attacker: UnitObject
    item: ItemObject
    defender: UnitObject
    status: str  # nid of status that is added


@dataclass
class ShoveHit(PlaybackBrush):
    """
    Added to playback when an attack shoves, pivots, or draws back
    Doesn't seem to actually be used for anything
    """
    nid: ClassVar[str] = 'shove_hit'
    attacker: UnitObject
    item: ItemObject
    defender: UnitObject


@dataclass
class SwapHit(PlaybackBrush):
    """
    Added to playback when an attack swaps
    Doesn't seem to actually be used for anything
    """
    nid: ClassVar[str] = 'swap_hit'
    attacker: UnitObject
    item: ItemObject
    defender: UnitObject


@dataclass
class RefreshHit(PlaybackBrush):
    """
    Added to playback when an attack refreshes/resets that target
    Doesn't seem to actually be used for anything
    """
    nid: ClassVar[str] = 'refresh_hit'
    attacker: UnitObject
    item: ItemObject
    defender: UnitObject


@dataclass
class RestoreHit(PlaybackBrush):
    """
    Added to playback when an attack removes a status from the target
    Doesn't seem to actually be used for anything
    """
    nid: ClassVar[str] = 'restore_hit'
    attacker: UnitObject
    item: ItemObject
    defender: UnitObject


# === mostly aesthetic brushes below ===

# Most of these will be added to playback during a combat
# but only some will actually be processed by the combat object
# depending on whether it's a Map Combat or an Animation Combat

@dataclass
class HitSound(PlaybackBrush):
    """
    Aesthetic brush for playing a specific sound on a hit
    For instance, No Damage sound when the attack does no damage
    Also processed by StatusUpkeep
    set map_only to true if it should only be played in a map combat
    """
    nid: ClassVar[str] = 'hit_sound'
    sound: str
    map_only: bool = False


@dataclass
class CastSound(PlaybackBrush):
    """
    Aesthetic brush for playing a specific sound
    Processed by a Map Combat only (Combat animations do this in Animation Combat)
    """
    nid: ClassVar[str] = 'cast_sound'
    sound: str


@dataclass
class HitAnim(PlaybackBrush):
    """
    Aesthetic brush for playing a specific animation on a hit
    For instance, MapNoDamage when the attack does no damage
    Processed by a Map Combat only (Combat animations do this in Animation Combat)
    """
    nid: ClassVar[str] = 'hit_anim'
    anim: str
    defender: UnitObject


@dataclass
class CastAnim(PlaybackBrush):
    """
    Aesthetic brush for playing a specific animation
    Processed by a Map Combat only (Combat animations do this in Animation Combat)
    Also processed by StatusUpkeep
    """
    nid: ClassVar[str] = 'cast_anim'
    anim: str


@dataclass
class TargetCastAnim(PlaybackBrush):
    """
    Aesthetic brush for playing a specific animation on all targets in AoE
    Processed by a Map Combat only (Combat animations do this in Animation Combat)
    """
    nid: ClassVar[str] = 'target_cast_anim'
    anim: str
    pos: Tuple[int, int]


@dataclass
class UnitTintAdd(PlaybackBrush):
    """
    Aesthetic brush for adding a brightening tint to unit's sprite
    Processed by a Map Combat only (Combat animations do this in Animation Combat)
    Also processed by StatusUpkeep
    """
    nid: ClassVar[str] = 'unit_tint_add'
    unit: UnitObject
    color: Tuple[int, int, int]


@dataclass
class UnitTintSub(PlaybackBrush):
    """
    Aesthetic brush for adding a darkening tint to unit's sprite
    Processed by a Map Combat only (Combat animations do this in Animation Combat)
    Also processed by StatusUpkeep
    """
    nid: ClassVar[str] = 'unit_tint_sub'
    unit: UnitObject
    color: Tuple[int, int, int]


@dataclass
class CritTint(PlaybackBrush):
    """
    Aesthetic brush for adding a brightening tint to unit's sprite that mimics the GBA critical effect
    Generally only used when critting
    Processed by a Map Combat only (Combat animations do this in Animation Combat)
    """
    nid: ClassVar[str] = 'crit_tint'
    unit: UnitObject
    color: Tuple[int, int, int]


@dataclass
class CritVibrate(PlaybackBrush):
    """
    Aesthetic brush for vibrating the unit's map sprite for a critical hit
    Generally only used when critting
    Processed by a Map Combat only (Combat animations do this in Animation Combat)
    """
    nid: ClassVar[str] = 'crit_vibrate'
    unit: UnitObject


@dataclass
class Shake(PlaybackBrush):
    """
    Aesthetic brush for shaking the health bars
    Shake is an integer 1-4 which determines the intensity
    Processed by a Map Combat only (damage_hit and damage_crit brushes do the shaking in Animation Combat)
    """
    nid: ClassVar[str] = 'shake'
    shake: int


@dataclass
class CombatEffect(PlaybackBrush):
    """
    Added to playback to mark that a unit's beginning of phase combat effects have occured (to prevent reoccurrence each phase)
    Ex: Legendary weapon effect, which only occurs once even if you double later on
    Only generated and used by Animation Combat
    """
    nid: ClassVar[str] = 'combat_effect'
    attacker_nid: str


@dataclass
class AlternateBattlePose(PlaybackBrush):
    """
    Aesthetic brush for using a different pose for combat during an animation combat
    Can be used if you have custom Luna attack animation or similar
    Processed by an Animation Combat only, and only if you would hit or crit
    """
    nid: ClassVar[str] = 'alternate_battle_pose'
    alternate_pose: str  # Pose to be used instead of the basic "Hit", "Crit", or "Miss"


@dataclass
class DamageNumbers(PlaybackBrush):
    """
    Aesthetic only damage numbers
    Only processed by StatusUpkeep
    """
    nid: ClassVar[str] = 'damage_numbers'
    unit: UnitObject
    damage: int
