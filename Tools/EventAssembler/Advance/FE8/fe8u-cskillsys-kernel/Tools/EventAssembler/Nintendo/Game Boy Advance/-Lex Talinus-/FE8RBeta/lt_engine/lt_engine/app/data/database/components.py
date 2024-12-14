from __future__ import annotations

import copy
from enum import Enum, IntEnum
from typing import Any, List, Optional, Tuple

from app.utilities import str_utils
from app.utilities.data import Data


class ComponentType(Enum):
    Bool = 0
    Int = 1
    Float = 2
    String = 3
    WeaponType = 4  # Stored as Nids
    WeaponRank = 5  # Stored as Nids
    Unit = 6  # Stored as Nids
    Class = 7  # Stored as Nids
    Tag = 8
    Color3 = 9
    Color4 = 10
    Item = 12  # Stored as Nids
    Skill = 13  # Stored as Nids
    Stat = 14  # Stored as Nids
    MapAnimation = 15  # Stored as Nids
    Equation = 16  # Stored as Nids
    MovementType = 17  # Stored as Nid
    Sound = 18  # Stored as Nid
    AI = 19  # Stored as Nid
    Music = 20  # Stored as Nid
    CombatAnimation = 21  # Stored as Nid
    EffectAnimation = 22  # Stored as Nid
    Affinity = 23  # Stored as Nid
    Terrain = 24 # stored as Nid
    Event = 80
    List = 100  # List containing Items
    Dict = 101  # List of Tuples of [Item, integer]
    FloatDict = 102  # List of Tuples of [Item, float]
    MultipleChoice = 103 # item is a string value from a number of choices
    MultipleOptions = 104 # Deprecated
    StringDict = 105  # List of Tuples of [Item, str]
    NewMultipleOptions = 106 # item is a dict of string options with types that can be individually configured


def convert_type_from_string(tstr: str, ttype: ComponentType):
    if ttype == ComponentType.Int:
        return int(tstr)
    if ttype == ComponentType.Float:
        return float(tstr)
    else:
        return tstr


def get_objs_using(objs: list, expose: ComponentType | Tuple[ComponentType, ComponentType],
                   value: Any) -> list:
    affected_items = []

    def _check(expose_type: Tuple[ComponentType, ComponentType], subvalue: List[Any]) -> bool:
        if expose_type[0] == ComponentType.List:
            for i, val in enumerate(subvalue):
                if val == value:
                    return True
        elif expose_type[0] in (ComponentType.Dict, ComponentType.FloatDict, ComponentType.StringDict):
            for i, val in enumerate(subvalue):
                if val[0] == value:
                    return True
        return False

    for obj in objs:
        for component in obj.components:
            if component.expose == expose and component.value == value:
                affected_items.append(obj)
            elif isinstance(component.expose, tuple) and component.expose[1] == expose and \
                    _check(component.expose, component.value):
                affected_items.append(obj)
            elif component.expose == ComponentType.NewMultipleOptions:
                for key, expose_type in component.options.items():
                    if expose_type == expose and component.value[key] == value:
                        affected_items.append(obj)
                        break
                    elif isinstance(expose_type, tuple) and expose_type[1] == expose and \
                            _check(expose_type, component.value[key]):
                        affected_items.append(obj)
                        break

    return affected_items


def swap_values(objs: list, expose: ComponentType | Tuple[ComponentType, ComponentType],
                old_value: Any, new_value: Any) -> None:
    def _swap(expose_type: Tuple[ComponentType, ComponentType], value: List[Any]) -> None:
        if expose_type[0] == ComponentType.List:
            for i, val in enumerate(value):
                if val == old_value:
                    value[i] = new_value
        elif expose_type[0] in (ComponentType.Dict, ComponentType.FloatDict, ComponentType.StringDict):
            for i, val in enumerate(value):
                if val[0] == old_value:
                    value[i][0] = new_value

    for obj in objs:
        for component in obj.components:
            if component.expose == expose and component.value == old_value:
                component.value = new_value
            elif isinstance(component.expose, tuple) and component.expose[1] == expose:
                _swap(component.expose, component.value)
            elif component.expose == ComponentType.NewMultipleOptions:
                for key, expose_type in component.options.items():
                    if expose_type == expose and component.value[key] == old_value:
                        component.value[key] = new_value
                    elif isinstance(expose_type, tuple) and expose_type[1] == expose:
                        _swap(expose_type, component.value[key])


class Component():
    nid: str
    desc: str
    author: str = 'rainlash'
    expose: Optional[ComponentType] = None  # Attribute
    paired_with: list = []
    tag: Enum
    value = None

    def __init__(self, value=None):
        if value is not None:
            self.value = value

    @property
    def name(self):
        name = self.__class__.__name__
        return str_utils.ignore_numbers(str_utils.camel_case(name))

    @classmethod
    def class_name(cls):
        name = cls.__name__
        return str_utils.ignore_numbers(str_utils.camel_case(name))

    def defines(self, function_name):
        return hasattr(self, function_name)

    @classmethod
    def copy(cls, other):
        return cls(other.value)

    def save(self):
        if isinstance(self.value, Data):
            return self.nid, self.value.save()
        elif isinstance(self.value, list):
            # Handle lists of lists with deepcopy
            return self.nid, copy.deepcopy(self.value)
        else:
            return self.nid, self.value
