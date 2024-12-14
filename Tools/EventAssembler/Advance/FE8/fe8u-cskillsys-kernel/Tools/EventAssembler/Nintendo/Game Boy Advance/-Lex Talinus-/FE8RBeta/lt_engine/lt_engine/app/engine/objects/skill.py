from __future__ import annotations
from app.utilities.data import Data

from app.data.database.database import DB
import app.engine.skill_component_access as SCA

class SkillObject():
    next_uid = 100

    def __init__(self, nid, name, desc, icon_nid=None, icon_index=(0, 0), components=None):
        self.uid = SkillObject.next_uid
        SkillObject.next_uid += 1

        self.nid = nid
        self.name = name

        self.owner_nid = None
        self.desc = desc

        self.icon_nid = icon_nid
        self.icon_index = icon_index

        self.components = components or Data()
        for component_key, component_value in self.components.items():
            self.__dict__[component_key] = component_value
            # Assign parent to component
            component_value.skill = self

        self.data = {}
        self.initiator_nid = None
        # For subskill
        self.subskill = None
        self.subskill_uid = None
        self.parent_skill = None

    @classmethod
    def from_prefab(cls, prefab, component_data=None):
        # Components NEED To be copies! Since they store individualized information
        components = Data()
        component_data = component_data or []

        if component_data:
            for component_nid, component_value in component_data:
                new_component = SCA.restore_component((component_nid, component_value))
                components.append(new_component, overwrite=True)

        else:
            for component in prefab.components:
                new_component = SCA.restore_component((component.nid, component.value))
                components.append(new_component, overwrite=True)

        return cls(prefab.nid, prefab.name, prefab.desc, prefab.icon_nid, prefab.icon_index, components)

    # If the attribute is not found
    def __getattr__(self, attr):
        if attr.startswith('__') and attr.endswith('__'):
            return super().__getattr__(attr)
        return None

    def __str__(self):
        return "Skill: %s %s" % (self.nid, self.uid)

    def __repr__(self):
        return "Skill: %s %s" % (self.nid, self.uid)

    def __hash__(self):
        return hash(self.uid)

    def __eq__(self, other: SkillObject) -> bool:
        return isinstance(other, SkillObject) and self.uid == other.uid

    def save(self):
        serial_dict = {}
        serial_dict['uid'] = self.uid
        serial_dict['nid'] = self.nid
        serial_dict['owner_nid'] = self.owner_nid
        serial_dict['data'] = self.data
        serial_dict['initiator_nid'] = self.initiator_nid
        serial_dict['subskill'] = self.subskill_uid
        components = [(component.nid, component.value) for component in self.components]
        serial_dict['components'] = components
        return serial_dict

    @classmethod
    def restore(cls, dat):
        prefab = DB.skills.get(dat['nid'])
        if prefab:
            components = dat.get('components', [])
            self = cls.from_prefab(prefab, components)
        else:
            desc = 'This is a placeholder for %s generated when the database cannot locate a skill' % dat['nid']
            self = cls(dat['nid'], 'Placeholder', desc)
        self.uid = dat['uid']
        self.owner_nid = dat['owner_nid']
        self.data = dat['data']
        self.initiator_nid = dat.get('initiator_nid', None)
        self.subskill_uid = dat.get('subskill', None)
        return self
