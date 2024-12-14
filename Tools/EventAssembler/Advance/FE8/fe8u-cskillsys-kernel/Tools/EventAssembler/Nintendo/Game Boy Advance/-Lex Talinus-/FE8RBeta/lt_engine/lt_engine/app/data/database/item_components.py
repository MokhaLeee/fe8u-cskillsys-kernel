from __future__ import annotations
from enum import Enum
from typing import TYPE_CHECKING, Optional

from app.data.database.components import Component, ComponentType, get_objs_using

if TYPE_CHECKING:
    from app.engine.objects.item import ItemObject


class ItemTags(Enum):
    BASE = 'base'
    TARGET = 'target'
    WEAPON = 'weapon'
    USES = 'uses'
    EXP = 'exp'
    CLASS_CHANGE = 'class_change'
    EXTRA = 'extra'
    UTILITY = 'utility'
    SPECIAL = 'special'
    FORMULA = 'formula'
    AOE = 'aoe'
    AESTHETIC = 'aesthetic'
    ADVANCED = 'advanced'

    CUSTOM = 'custom'
    HIDDEN = 'hidden'
    DEPRECATED = 'deprecated'

class ItemComponent(Component):
    item: Optional[ItemObject] = None

def get_items_using(expose: ComponentType, value, db) -> list:
    return get_objs_using(db.items.values(), expose, value)
