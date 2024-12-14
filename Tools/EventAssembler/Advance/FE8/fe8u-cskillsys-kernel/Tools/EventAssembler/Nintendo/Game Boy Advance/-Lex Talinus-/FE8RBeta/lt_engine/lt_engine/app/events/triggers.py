from __future__ import annotations
import dataclasses
from dataclasses import dataclass
from typing import TYPE_CHECKING, Any, ClassVar, Dict, List, Optional, Tuple

if TYPE_CHECKING:
    from app.engine.objects.item import ItemObject
    from app.engine.objects.unit import UnitObject
    from app.engine.objects.region import RegionObject
    from app.engine.combat.playback import PlaybackBrush

from app.utilities.typing import NID


@dataclass()
class EventTrigger():
    """
    The description of the trigger and where it happens. This is a template
    for the docstring.

        some_var_name: the description of the var.
        some_other_var_name: the description of the other var.
    """
    nid: ClassVar[NID]

    def to_args(self):
        return self.__dict__.copy()

@dataclass()
class GenericTrigger(EventTrigger):
    """A generic trigger containing common fields. Use to trigger
    anonymous events.
    """
    nid: ClassVar[NID] = None
    unit1: UnitObject = None
    unit2: UnitObject = None
    position: Tuple[int, int] = None
    local_args: Dict[str, Any] = None

    def to_args(self):
        self_dict = self.__dict__.copy()
        del self_dict['local_args']
        if self.local_args:
            final_args = self.local_args.copy()
            final_args.update(self_dict)
        else:
            final_args = self_dict
        return final_args

@dataclass(init=True)
class LevelStart(EventTrigger):
    """
    Occurs at the very beginning of a level. The chapter screen and title is usually
    displayed here, as well as introductory cinematics.
    """
    nid: ClassVar[NID] = 'level_start'

@dataclass(init=True)
class LevelEnd(EventTrigger):
    """
    This occurs once `win_game` is set in another event. This is called at the
    end of gameplay, and usually handles end cinematics before going to
    the save screen or overworld.
    """
    nid: ClassVar[NID] = 'level_end'

@dataclass(init=True)
class OverworldStart(EventTrigger):
    """
    Occurs upon entering the overworld.
    """
    nid: ClassVar[NID] = 'overworld_start'

@dataclass(init=True)
class LevelSelect(EventTrigger):
    """
    Occurs when an overworld entity is about to issue a move to the node
    containing the next level. Because of implementation detail, when
    this event occurs, it supersedes any queued moves. Therefore, the
    entity will _not move_ to the selected node. Any events that use
    this trigger should include a scripted move if movement is desired.
    """
    nid: ClassVar[NID] = 'level_select'

@dataclass(init=True)
class PhaseChange(EventTrigger):
    """
    Occurs whenever the phase changes. Check `team` to figure out whose phase it is now.
    """
    nid: ClassVar[NID] = 'phase_change'
    team: NID

@dataclass(init=True)
class TurnChange(EventTrigger):
    """
    Occurs immediately before turn changes to Player Phase. Useful for dialogue or reinforcements.
    """
    nid: ClassVar[NID] = 'turn_change'

@dataclass(init=True)
class EnemyTurnChange(EventTrigger):
    """
    Occurs immediately before turn changes to Enemy Phase.
    Useful for "same turn reinforcements" and other evil deeds.
    """
    nid: ClassVar[NID] = 'enemy_turn_change'

@dataclass(init=True)
class Enemy2TurnChange(EventTrigger):
    """
    Occurs immediately before turn changes to Second Enemy's Phase.
    """
    nid: ClassVar[NID] = 'enemy2_turn_change'

@dataclass(init=True)
class OtherTurnChange(EventTrigger):
    """
    Occurs immediately before turn changes to Other Phase.
    """
    nid: ClassVar[NID] = 'other_turn_change'

@dataclass(init=True)
class OnRegionInteract(EventTrigger):
    """
    Occurs when a unit interacts with an event region.
    All event region type events (like Shop, Armory, Visit, etc.)
    follow this trigger's format.

        unit1: the unit that is interacting.
        position: the position of the unit.
        region: the event region.
    """
    nid: ClassVar[NID] = 'on_region_interact'
    unit1: UnitObject
    position: Tuple[int, int]
    region: RegionObject

@dataclass(init=True)
class UnitDeath(EventTrigger):
    """
    Occurs when any unit dies, including generics.

        unit1: the unit that died.
        unit2: the unit that killed them (can be None).
        position: the position they died at.
    """
    nid: ClassVar[NID] = 'unit_death'
    unit1: UnitObject
    unit2: UnitObject
    position: Tuple[int, int]

@dataclass(init=True)
class UnitWait(EventTrigger):
    """
    Occurs when any unit waits.

        unit1: the unit that waited.
        position: the position they waited at.
        region: region under the unit (can be None)
        actively_chosen: boolean for whether the player actively selected Wait
    """
    nid: ClassVar[NID] = 'unit_wait'
    unit1: UnitObject
    position: Tuple[int, int]
    region: Optional[RegionObject]
    actively_chosen: bool

@dataclass(init=True)
class UnitSelect(EventTrigger):
    """
    Occurs when a unit is selected by the cursor.

        unit1: the unit that was selected.
        position: the position they were selected at.
    """
    nid: ClassVar[NID] = 'unit_select'
    unit1: UnitObject
    position: Tuple[int, int]

@dataclass(init=True)
class UnitLevelUp(EventTrigger):
    """
    Occurs whenever a unit levels up.

        unit1: the unit that changed their level.
        stat_changes: a dict containing their stat changes.
        source: One of ('exp_gain', 'stat_change', 'class_change', 'promote', 'event') describing how the unit got to this point.
    """
    nid: ClassVar[NID] = 'unit_level_up'
    unit1: UnitObject
    stat_changes: Dict[NID, int]
    source: str

@dataclass(init=True)
class DuringUnitLevelUp(EventTrigger):
    """
    Occurs during a unit's level-up screen, immediately after stat changes are granted. This event is useful for implementing level-up quotes.

        unit1: the unit that gained/lost stats.
        stat_changes: a dict containing their stat changes.
        source: One of ('exp_gain', 'stat_change', 'class_change', 'promote') describing how the unit got to this screen.
    """
    nid: ClassVar[NID] = 'during_unit_level_up'
    unit1: UnitObject
    stat_changes: Dict[NID, int]
    source: str

@dataclass(init=True)
class CombatStart(EventTrigger):
    """
    Occurs when non-scripted combat is begun between any two units. Useful for boss quotes.

        unit1: the unit who initiated combat.
        unit2: the target of the combat (can be None).
        item: the item/ability used by unit1.
        position: the position of the unit1.
        is_animation_combat: a boolean denoting whether or not we are in an actual animation or merely a map animation.
    """
    nid: ClassVar[NID] = 'combat_start'
    unit1: UnitObject
    unit2: UnitObject
    position: Tuple[int, int]
    item: ItemObject
    is_animation_combat: bool

@dataclass(init=True)
class CombatEnd(EventTrigger):
    """
    This trigger fires at the end of combat. Useful for checking win or loss conditions.

        unit1: the unit who initiated combat.
        unit2: the target of the combat (can be None).
        item: the item/ability used by unit1.
        position: contains the position of unit1.
        playback: a list of the playback brushes from the combat.
    """
    nid: ClassVar[NID] = 'combat_end'
    unit1: UnitObject
    unit2: UnitObject
    position: Tuple[int, int]
    item: ItemObject
    playback: List[PlaybackBrush]

@dataclass(init=True)
class OnTalk(EventTrigger):
    """
    This trigger fires when two units "Talk" to one another.

        unit1: the unit who is the talk initiator.
        unit2: the unit who is the talk receiver.
        position: the position of unit1 (is None if triggered during free roam)
    """
    nid: ClassVar[NID] = 'on_talk'
    unit1: UnitObject
    unit2: UnitObject
    position: Tuple[int, int]

@dataclass(init=True)
class OnSupport(EventTrigger):
    """
    This trigger fires when two units "Support" to one another.

        unit1: the unit who is the support initiator.
        unit2: the unit who is the support receiver.
        position: the position of unit1 (could be None, for instance during Base).
        support_rank_nid: contains the nid of the support rank (e.g. `A`, `B`, `C`, or `S`)
        is_replay: whether or not this is just a replay of the support convo from the base menu.
    """
    nid: ClassVar[NID] = 'on_support'
    unit1: UnitObject
    unit2: UnitObject
    position: Tuple[int, int]
    support_rank_nid: NID
    is_replay: bool

@dataclass(init=True)
class OnBaseConvo(EventTrigger):
    """
    This trigger fires when the player selects a base conversation to view.

        base_convo: contains the name of the base conversation.
        unit: DEPRECATED, contains the name of the base conversation.
    """
    nid: ClassVar[NID] = 'on_base_convo'
    base_convo: NID
    unit: NID  # DEPRECATED - Just a copy of the base_convo

@dataclass(init=True)
class OnPrepStart(EventTrigger):
    """
    Occurs each time the player enters preps.
    """
    nid: ClassVar[NID] = 'on_prep_start'

@dataclass(init=True)
class OnBaseStart(EventTrigger):
    """
    Occurs each time the player enters base.
    """
    nid: ClassVar[NID] = 'on_base_start'

@dataclass(init=True)
class OnTurnwheel(EventTrigger):
    """
    Occurs after the turnwheel is used. Events that happen within are
    not recorded within the turnwheel and therefore will not be reversed
    upon turnwheel activation.
    """
    nid: ClassVar[NID] = 'on_turnwheel'

@dataclass(init=True)
class OnTitleScreen(EventTrigger):
    """
    Occurs before the title screen is shown.
    """
    nid: ClassVar[NID] = 'on_title_screen'

@dataclass(init=True)
class OnStartup(EventTrigger):
    """
    Occurs whenever the engine starts.
    """
    nid: ClassVar[NID] = 'on_startup'

@dataclass(init=True)
class TimeRegionComplete(EventTrigger):
    """
    Occurs when a time region runs out of time and would be removed.
        position: the position of the region that has run out of time.
        region: the region that has run out of time.
    """
    nid: ClassVar[NID] = 'time_region_complete'
    position: Tuple[int, int]
    region: RegionObject

@dataclass(init=True)
class OnOverworldNodeSelect(EventTrigger):
    """
    Occurs when an entity is about to issue a move to a node
    (which may or may not contain the next level, or
    any level at all). Because of implementation detail,
    when this event occurs, it supersedes any queued moves.
    Therefore, the entity will _not move_ to the selected node.
    Any events that use this trigger should include a scripted move
    if movement is desired.

        entity_nid: Contains the id of entity that will issue a move.
        node_nid: Contains the id of the node.
    """
    nid: ClassVar[NID] = 'on_overworld_node_select'
    entity_nid: NID
    node_nid: NID

@dataclass(init=True)
class RoamPressStart(EventTrigger):
    """
    Occurs when the `start` key is pressed in Free Roam.

        unit1: The current roam unit.
        unit2: the closest nearby other unit.
    """
    nid: ClassVar[NID] = 'roam_press_start'
    unit1: UnitObject
    unit2: UnitObject

@dataclass(init=True)
class RoamPressInfo(EventTrigger):
    """
    Occurs when the `info` key is pressed in Free Roam.

        unit1: The current roam unit.
        unit2: the closest nearby other unit.
    """
    nid: ClassVar[NID] = 'roam_press_info'
    unit1: UnitObject
    unit2: UnitObject

@dataclass(init=True)
class RoamPressAux(EventTrigger):
    """
    Occurs when the `aux` key is pressed in Free Roam.

        unit1: The current roam unit.
        unit2: the closest nearby other unit.
    """
    nid: ClassVar[NID] = 'roam_press_aux'
    unit1: UnitObject
    unit2: UnitObject

@dataclass(init=True)
class RoamingInterrupt(EventTrigger):
    """
    Occurs when the player enters an `interrupt` region on the map.

        unit1: The current roam unit.
        position: The position of the current roam unit
        region: The region that was triggered.
    """
    nid: ClassVar[NID] = 'roaming_interrupt'
    unit1: UnitObject
    position: Tuple[int, int]
    region: RegionObject

@dataclass(init=True)
class RegionTrigger(EventTrigger):
    """
    Special trigger. This trigger has a custom nid, and will be created whenever you make an interactable
    event region.

        nid: the nid of the region
        unit1: The unit triggering the region
        position: The position of the unit triggering the region
        region: the name of the region that was triggered
        item: the item used to trigger this region (used with unlock staves and keys)
    """
    nid: NID
    unit1: UnitObject
    position: Tuple[int, int]
    region: RegionObject
    item: ItemObject = None

ALL_TRIGGERS = [tclass for tclass in EventTrigger.__subclasses__() if hasattr(tclass, 'nid') and tclass is not GenericTrigger]