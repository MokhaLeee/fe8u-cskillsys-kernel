from dataclasses import dataclass
from typing import Tuple, ClassVar
from enum import Enum, auto
from app.engine.objects.unit import UnitObject
from app.engine.objects.region import RegionObject

class RoamAIAction(Enum):
    NONE = auto()
    MOVE = auto()
    WAIT = auto()
    INTERACT = auto()

@dataclass
class AIStruct():
    unit: UnitObject
    action_type: ClassVar[RoamAIAction] = RoamAIAction.NONE

@dataclass
class Wait(AIStruct):
    unit: UnitObject
    time: int
    action_type: ClassVar[RoamAIAction] = RoamAIAction.WAIT

@dataclass
class MoveTo(AIStruct):
    unit: UnitObject
    target: Tuple[int, int]
    desired_proximity: float
    action_type: ClassVar[RoamAIAction] = RoamAIAction.MOVE

@dataclass
class Interact(AIStruct):
    unit: UnitObject
    region: RegionObject
    desired_proximity: float
    action_type: ClassVar[RoamAIAction] = RoamAIAction.INTERACT
