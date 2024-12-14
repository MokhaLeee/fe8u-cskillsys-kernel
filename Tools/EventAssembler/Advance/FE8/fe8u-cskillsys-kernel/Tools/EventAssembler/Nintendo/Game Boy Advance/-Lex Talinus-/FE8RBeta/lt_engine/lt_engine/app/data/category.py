from __future__ import annotations

from dataclasses import dataclass, field
from typing import Dict, Generic, List, TypeVar
from app.utilities.data import Data, HasNid

from app.utilities.typing import NID

T = TypeVar('T', bound=HasNid)
@dataclass
class Categories(Dict[NID, List[str]]):
    @classmethod
    def load(cls, s_dict: Dict[NID, str]) -> Categories:
        categories = cls()
        categories.clear()
        for nid, cat_str in s_dict.items():
            categories[nid] = cat_str.split('/')
        return categories

    def save(self) -> Dict[NID, str]:
        s_dict = {}
        categories = sorted(list(self.items()))
        for nid, cats in categories:
            s_dict[nid] = '/'.join(cats)
        return s_dict

    def __str__(self) -> str:
        return super().__str__()

    def __repr__(self) -> str:
        return super().__repr__()

class CategorizedCatalog(Data[T], Generic[T]):
    categories: Categories = field(default_factory=Categories)
