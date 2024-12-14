from __future__ import annotations

import logging
from typing import Dict, Generic, List, Optional, Tuple, Type, TypeVar

from app.utilities.typing import NID, Protocol


class HasNid(Protocol):
    nid: NID

T = TypeVar('T', bound=HasNid)

class Data(Generic[T]):
    """
    Only accepts data points that have nid attribute
    Generally behaves as a list first and a dictionary second
    """
    datatype: Type = T

    def __init__(self, vals: Optional[List[T]] = None):
        if vals:
            self._list: List[T] = vals
            self._dict: Dict[NID, T] = {val.nid: val for val in vals}
        else:
            self._list: List[T] = []
            self._dict: Dict[NID, T] = {}

    def values(self) -> List[T]:
        return self._list

    def keys(self) -> List[NID]:
        return [val.nid for val in self._list]

    def items(self) -> List[Tuple[NID, T]]:
        return [(val.nid, val) for val in self._list]

    def get(self, key: NID, fallback: Optional[T] = None) -> Optional[T]:
        return self._dict.get(key, fallback)

    def sort(self, sort_func=None):
        self._list = sorted(self._list, key=sort_func)
        self._dict.clear()
        for val in self._list:
            self._dict[val.nid] = val

    def update_nid(self, val: T, nid: NID, set_nid=True):
        for k, v in self._dict.items():
            if v == val:
                del self._dict[k]
                if set_nid:
                    val.nid = nid
                self._dict[nid] = val
                break

    def find_key(self, val: T) -> Optional[NID]:
        for k, v in self._dict.items():
            if v == val:
                return k

    def change_key(self, old_key: NID, new_key: NID):
        if old_key in self._dict:
            old_value = self._dict[old_key]
            del self._dict[old_key]
            old_value.nid = new_key
            self._dict[new_key] = old_value
        else:
            logging.error('%s not found in self._dict' % old_key)

    def append(self, val: T, overwrite: bool = False):
        if val.nid not in self._dict:
            self._list.append(val)
            self._dict[val.nid] = val
        elif overwrite:
            self.remove_key(val.nid)
            self._list.append(val)
            self._dict[val.nid] = val
        else:
            logging.warning("%s already present in data" % val.nid)

    def delete(self, val: T):
        # Fails silently
        if val.nid in self._dict:
            self._list.remove(val)
            del self._dict[val.nid]

    def remove_key(self, key: NID):
        val = self._dict[key]
        self._list.remove(val)
        del self._dict[key]

    def pop(self, idx: Optional[int] = None):
        if idx is None:
            idx = len(self._list) - 1
        r = self._list[idx]
        if r.nid in self._dict:
            r = self._list.pop(idx)
            del self._dict[r.nid]
        else:
            logging.error("Tried to delete %s which wasn't present in data" % r.nid)

    def insert(self, idx: int, val: T):
        self._list.insert(idx, val)
        self._dict[val.nid] = val

    def clear(self):
        self._list = []
        self._dict = {}

    def index(self, nid: NID) -> int:
        for idx, val in enumerate(self._list):
            if val.nid == nid:
                return idx
        raise ValueError

    def move_index(self, old_index: int, new_index: int):
        if old_index == new_index:
            return
        obj = self._list.pop(old_index)
        self._list.insert(new_index, obj)

    # def begin_insert_row(self, index):
    #     self.drop_to = index

    # Saving functions
    def save(self):
        if self.datatype and issubclass(self.datatype, Prefab):
            return [elem.save() for elem in self._list]
        else:
            return self._list[:]

    def restore(self, vals: T):
        self.clear()
        if self.datatype and issubclass(self.datatype, Prefab):
            for s_dict in vals:
                new_val = self.datatype.restore(s_dict)
                self.append(new_val)
        else:
            for val in vals:
                self.append(val)
        return self

    # Magic Methods
    def __repr__(self):
        return repr(self._list)

    def __len__(self):
        return len(self._list)

    def __getitem__(self, idx):
        return self._list[idx]

    def __iter__(self):
        return iter(self._list)

class Prefab():
    def save(self):
        s_dict = {}
        for attr in self.__dict__.items():
            name, value = attr
            value = self.save_attr(name, value)
            s_dict[name] = value
        return s_dict

    def save_attr(self, name, value):
        if isinstance(value, Data):
            value = value.save()
        else:  # int, str, float, list, dict
            value = value
        return value

    @classmethod
    def restore(cls, s_dict):
        self = cls.default()
        for attr_name, attr_value in self.__dict__.items():
            value = self.restore_attr(attr_name, s_dict.get(attr_name))
            if value is not None:
                setattr(self, attr_name, value)
        return self

    def restore_attr(self, name, value):
        if isinstance(value, Data):
            value = value.restore()
        else:
            value = value
        return value

    @classmethod
    def default(cls):
        return cls()
