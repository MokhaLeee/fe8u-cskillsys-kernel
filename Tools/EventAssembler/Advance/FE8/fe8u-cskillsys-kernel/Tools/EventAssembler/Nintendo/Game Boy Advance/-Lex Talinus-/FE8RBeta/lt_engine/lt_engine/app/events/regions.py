from typing import Optional

from enum import Enum
from app.utilities.data import Prefab

class RegionType(str, Enum):
    NORMAL = 'normal'
    STATUS = 'status'
    EVENT = 'event'
    FORMATION = 'formation'
    FOG = 'fog'
    VISION = 'vision'
    TERRAIN = 'terrain'

class Region(Prefab):
    def __init__(self, nid):
        self.nid = nid
        self.region_type = RegionType.NORMAL
        self.position = None
        self.size = [1, 1]

        self.sub_nid = None
        self.condition: str = 'True'
        self.time_left: Optional[int] = None
        self.only_once: bool = False
        self.interrupt_move: bool = False

    @classmethod
    def restore(cls, s_dict):
        self = super(Region, cls).restore(s_dict)
        # Move time left from sub_nid
        # to time left parameter
        if s_dict['region_type'] == 'time':
            self.time_left = self.sub_nid
            self.sub_nid = None
        return self

    def restore_attr(self, name, value):
        if name == 'region_type':
            # Replace deprecated time region with normal region
            if value == 'time':
                value = 'normal'
            value = RegionType(value)
        else:
            value = super().save_attr(name, value)
        return value

    @property
    def area(self):
        return self.size[0] * self.size[1]

    @property
    def center(self) -> tuple:
        if self.position:
            x = int(self.position[0] + self.size[0] // 2)
            y = int(self.position[1] + self.size[1] // 2)
            return x, y
        else:
            return None

    def contains(self, pos: tuple) -> bool:
        x, y = pos
        if self.position:
            return self.position[0] <= x < self.position[0] + self.size[0] and \
                self.position[1] <= y < self.position[1] + self.size[1]
        else:
            return False

    def get_all_positions(self):
        if self.position:
            positions = []
            for i in range(self.position[0], self.position[0] + self.size[0]):
                for j in range(self.position[1], self.position[1] + self.size[1]):
                    positions.append((i, j))
            return positions
        else:
            return []

    @classmethod
    def default(cls):
        return cls('None')
