from app.utilities import str_utils
from dataclasses import dataclass, field
from typing import List

from app.utilities.data import Data, Prefab
from app.utilities.typing import NID

valid_types = ["str", "list", "kv"]

class RawListDataObjectBase():
    nid: str = None

@dataclass
class RawDataPrefab(Prefab):
    nid: str = None
    dtype: str = "str"
    svalue: str = field(default_factory=str)                    # arbitrary string data
    kvvalue: list = field(default_factory=list)                 # arbitrary key-value pair
    lovalue: list = field(default_factory=list)                 # list of arbitrary objects
    oattrs: List[str] = field(default_factory=lambda:['nid'])             # official object attributes for objs in lovalue; all objects should have nid (see RawListDataObjectBase)

    def __repr__(self):
        return str(vars(self))

    def __str__(self) -> str:
        return str(vars(self))

    def save_attr(self, name, value):
        if name == 'lovalue':
            return [vars(obj) for obj in self.lovalue]
        else:
            return super().save_attr(name, value)

    @property
    def value(self):
        if self.dtype == "str":
            return self.svalue
        elif self.dtype == "kv":
            return dict(self.kvvalue)
        elif self.dtype == "list":
            return Data(self.lovalue)
        return ""

    def restore_attr(self, name, value):
        if name == 'lovalue':
            if value:
                lovalue = []
                for obj_dict in value:
                    obj = RawListDataObjectBase()
                    for k, v in obj_dict.items():
                        setattr(obj, k, v)
                    lovalue.append(obj)
                    # for backwards compatability
                    if not getattr(obj, 'nid'):
                        nids = [o.nid for o in lovalue]
                        nid = str_utils.get_next_generic_nid("key", nids)
                        setattr(obj, 'nid', nid)
                return lovalue
        if name == 'oattrs':
            if 'nid' not in value:
                value = ['nid'] + value
            return value
        else:
            return super().restore_attr(name, value)

class RawDataCatalog(Data[RawDataPrefab]):
    datatype = RawDataPrefab

    def get(self, key: NID, fallback=None):
        if key in self.keys():
            return super().get(key).value
        return super().get(key, fallback)


    def get_prefab(self, key: NID, fallback=None):
        return super().get(key, fallback)
