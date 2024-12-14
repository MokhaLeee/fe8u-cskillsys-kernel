from app.engine.objects.skill import SkillObject
from typing import Union

# Tracking origin of skills for removal policies
from enum import Enum
        
class SourceType(Enum):
    AURA = ('aura', False, False)
    TERRAIN = ('terrain', False, False)
    ITEM = ('item', False, False)
    REGION = ('region', False, False)
    TRAVELER = ('traveler', False, False)
    KLASS = ('klass', False, True)
    PERSONAL = ('personal', False, True)
    FATIGUE = ('fatigue', False, True)
    GLOBAL = ('global', False, False)
    DEFAULT = ('default', True, True)

    def __init__(self, nid, displaceable, removable):
        self.nid = nid
        self.displaceable = displaceable
        self.removable = removable
