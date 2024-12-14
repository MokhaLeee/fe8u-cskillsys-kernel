from __future__ import annotations
import logging
from typing import TYPE_CHECKING, Optional

import app.engine.item_component_access as ICA
from app.data.database.database import DB
from app.utilities.data import Data

if TYPE_CHECKING:
    from app.data.database.item_components import ItemComponent

class ItemObject():
    next_uid = 100

    def __init__(self, nid, name, desc, icon_nid=None, icon_index=(0, 0), components: Optional[Data[ItemComponent]]=None):
        self.uid = ItemObject.next_uid
        ItemObject.next_uid += 1

        self.nid = nid
        self.name = name

        self.owner_nid = None
        self.desc = desc

        self.icon_nid = icon_nid
        self.icon_index = icon_index

        self.droppable = False

        self.components: Data[ItemComponent] = components or Data()
        for component_key, component_value in self.components.items():
            self.__dict__[component_key] = component_value
            # Assign parent to component
            component_value.item = self

        self.data = {}

        # For subitems
        self.subitem_uids = []
        self.subitems = []
        self.parent_item = None

    @property
    def tags(self) -> set:
        all_tags = set()
        for component in self.components:
            if component.nid == 'item_tags':
                all_tags |= set(self.components.get('item_tags').value)
        return all_tags

    def change_owner(self, nid):
        self.owner_nid = nid
        for item in self.subitems:
            item.owner_nid = nid

    @classmethod
    def from_prefab(cls, prefab, component_data=None):
        # Components NEED To be copies! Since they store individualized information
        components = Data()
        component_data = component_data or []

        if component_data:
            # Regular components
            for component_nid, component_value in component_data:
                new_component = ICA.restore_component((component_nid, component_value))
                components.append(new_component, overwrite=True)

        else:
            # Check if there is a prefab
            for component in prefab.components:
                if component.nid == 'item_prefab':
                    item_prefab_nid = component.value
                    item_prefab = DB.items.get(item_prefab_nid)
                    if not item_prefab:
                        logging.error("Couldn't find %s for %s", item_prefab_nid, prefab.nid)
                        break
                    for component in item_prefab.components:
                        new_component = ICA.restore_component((component.nid, component.value))
                        components.append(new_component)
                    break

            # Regular components
            for component in prefab.components:
                new_component = ICA.restore_component((component.nid, component.value))
                components.append(new_component, overwrite=True)

        return cls(prefab.nid, prefab.name, prefab.desc, prefab.icon_nid, prefab.icon_index, components)

    # If the attribute is not found
    def __getattr__(self, attr):
        if attr.startswith('__') and attr.endswith('__'):
            return super().__getattr__(attr)
        return None

    def __str__(self):
        return "Item: %s %s" % (self.nid, self.uid)

    def __repr__(self):
        return "Item: %s %s" % (self.nid, self.uid)

    def __hash__(self):
        return hash(self.uid)

    def __eq__(self, other: ItemObject) -> bool:
        return isinstance(other, ItemObject) and self.uid == other.uid

    def save(self):
        serial_dict = {}
        serial_dict['uid'] = self.uid
        serial_dict['nid'] = self.nid
        serial_dict['name'] = self.name
        serial_dict['desc'] = self.desc
        serial_dict['owner_nid'] = self.owner_nid
        serial_dict['droppable'] = self.droppable
        serial_dict['data'] = self.data
        serial_dict['subitems'] = self.subitem_uids
        components = [(component.nid, component.value) for component in self.components]
        serial_dict['components'] = components
        return serial_dict

    @classmethod
    def restore(cls, dat):
        prefab = DB.items.get(dat['nid'])
        if prefab:
            components = dat.get('components', [])
            self = cls.from_prefab(prefab, components)
            self.name = dat.get('name', prefab.name)
            self.desc = dat.get('desc', prefab.desc)
        else:
            desc = 'This is a placeholder for %s generated when the database cannot locate an item' % dat['nid']
            self = cls(dat['nid'], 'Placeholder', desc)
        self.uid = dat['uid']
        self.owner_nid = dat['owner_nid']
        self.droppable = dat['droppable']
        self.data = dat['data']
        self.subitem_uids = dat.get('subitems', [])
        return self
