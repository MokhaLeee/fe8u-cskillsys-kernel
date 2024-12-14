from __future__ import annotations

from typing import Optional, Tuple

import app.engine.item_component_access as ICA
from app.data.category import CategorizedCatalog
from app.data.database.item_components import ItemComponent
from app.utilities import str_utils
from app.utilities.data import Data, Prefab
from app.utilities.typing import NID


class ItemPrefab(Prefab):
    nid: NID
    name: str
    desc: str
    components: Data[ItemComponent]
    icon_nid: Optional[NID]
    icon_index: Tuple[int, int]

    def __init__(self, nid, name, desc, icon_nid=None, icon_index=(0, 0), components: Optional[Data[ItemComponent]]=None):
        self.nid = nid
        self.name = name
        self.desc = desc

        self.icon_nid = icon_nid
        self.icon_index = icon_index

        self.components = components or Data[ItemComponent]()
        for component_key, component_value in self.components.items():
            self.__dict__[component_key] = component_value

    # If the attribute is not found
    def __getattr__(self, attr):
        if attr.startswith('__') and attr.endswith('__'):
            return super().__getattr__(attr)
        return None

    def __getitem__(self, key):
        return self.__dict__.get(key)

    def __setitem__(self, key, item):
        self.__dict__[key] = item

    def save(self):
        serial_dict = {'nid': self.nid,
                       'name': self.name,
                       'desc': self.desc,
                       'icon_nid': self.icon_nid,
                       'icon_index': self.icon_index,
                       'components': [c.save() for c in self.components]
                       }
        return serial_dict

    @classmethod
    def restore(cls, dat):
        components = [ICA.restore_component(val) for val in dat['components']]
        components = [c for c in components if c]

        # NOTE: Backwards Compatability. Guarantees that additional paired components
        # added since last version are default initialized.
        all_components = Data()
        for component in components:
            all_components.append(component)
            if component.paired_with:
                for paired_nid in component.paired_with:
                    if not any([paired_nid == component.nid for component in components]):
                        component_class = ICA.get_component(paired_nid)
                        new_paired = ICA.restore_component((paired_nid, component_class.value))
                        all_components.append(new_paired)

        i = cls(dat['nid'], dat['name'], dat['desc'],
                dat['icon_nid'], dat['icon_index'],
                all_components)
        return i

class ItemCatalog(CategorizedCatalog[ItemPrefab]):
    datatype = ItemPrefab

    def create_new(self, db):
        nids = [d.nid for d in self]
        nid = name = str_utils.get_next_name("New Item", nids)
        new_item = ItemPrefab(nid, name, '')
        self.append(new_item)
        return new_item
