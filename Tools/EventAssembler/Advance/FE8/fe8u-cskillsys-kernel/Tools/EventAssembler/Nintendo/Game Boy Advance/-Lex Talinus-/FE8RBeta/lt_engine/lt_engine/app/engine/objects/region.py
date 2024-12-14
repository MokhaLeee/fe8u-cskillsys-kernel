from typing import Optional, Tuple
from app.utilities.typing import NID

from app.events.regions import RegionType, Region

class RegionObject(Region):
    """
    Inherits from Region in order to access Region's helper funcs,
    like area and center
    """

    def __init__(self, nid: NID, region_type: RegionType, 
                 position: Tuple[int, int] = None, size: Tuple[int, int] = [1, 1], 
                 sub_nid: str = None, condition: str = 'True', time_left: Optional[int] = None,
                 only_once: bool = False, interrupt_move: bool = False):
        self.nid = nid
        self.region_type = region_type
        self.position = tuple(position) if position else None
        self.size = size

        self.sub_nid = sub_nid
        self.condition: str = condition
        self.time_left: Optional[int] = time_left
        self.only_once: bool = only_once
        self.interrupt_move: bool = interrupt_move

        self.data = {}

    @classmethod
    def from_prefab(cls, prefab):
        return cls(prefab.nid, prefab.region_type, prefab.position, prefab.size,
                   prefab.sub_nid, prefab.condition, prefab.time_left,
                   prefab.only_once, prefab.interrupt_move)

    def save(self) -> dict:
        serial_dict = {}
        serial_dict['nid'] = self.nid
        serial_dict['region_type'] = self.region_type
        serial_dict['position'] = self.position
        serial_dict['size'] = self.size
        serial_dict['sub_nid'] = self.sub_nid
        serial_dict['condition'] = self.condition
        serial_dict['time_left'] = self.time_left
        serial_dict['only_once'] = self.only_once
        serial_dict['interrupt_move'] = self.interrupt_move

        serial_dict['data'] = self.data
        return serial_dict

    @classmethod
    def restore(cls, dat: dict):
        self = cls(dat['nid'], dat['region_type'], dat['position'], dat['size'],
                   dat['sub_nid'], dat['condition'], dat['time_left'],
                   dat['only_once'], dat['interrupt_move'])
        self.data = dat['data']
        return self
