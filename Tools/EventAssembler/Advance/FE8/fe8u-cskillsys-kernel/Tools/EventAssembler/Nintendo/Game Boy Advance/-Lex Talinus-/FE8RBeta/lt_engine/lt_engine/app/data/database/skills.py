from dataclasses import field
from typing import Optional, Tuple

import app.engine.skill_component_access as SCA
from app.data.category import Categories, CategorizedCatalog
from app.data.database.components import Component
from app.utilities.data import Data, Prefab
from app.utilities.typing import NID
from app.utilities import str_utils


class SkillPrefab(Prefab):
    nid: str
    name: str
    desc: str
    components: Data[Component]
    icon_nid: Optional[NID]
    icon_index: Tuple[int, int]

    def __init__(self, nid, name, desc, icon_nid=None, icon_index=(0, 0), components=None):
        self.nid = nid
        self.name = name
        self.desc = desc

        self.icon_nid = icon_nid
        self.icon_index = icon_index

        self.components = components or Data()
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
        skill_components = Data()
        components = [SCA.restore_component(val) for val in dat['components']]
        components = [c for c in components if c]
        for component in components:
            skill_components.append(component)
        i = cls(dat['nid'], dat['name'], dat['desc'],
                dat['icon_nid'], dat['icon_index'],
                skill_components)
        return i

class SkillCatalog(CategorizedCatalog[SkillPrefab]):
    datatype = SkillPrefab

    def get_feats(self) -> list:
        feats = []
        for skill in self:
            for component in skill.components:
                if component.nid == 'feat':
                    feats.append(skill)
                    break
        return feats

    def create_new(self, db):
        nids = [d.nid for d in self]
        nid = name = str_utils.get_next_name("New Skill", nids)
        new_skill = SkillPrefab(nid, name, '')
        self.append(new_skill)
        return new_skill
