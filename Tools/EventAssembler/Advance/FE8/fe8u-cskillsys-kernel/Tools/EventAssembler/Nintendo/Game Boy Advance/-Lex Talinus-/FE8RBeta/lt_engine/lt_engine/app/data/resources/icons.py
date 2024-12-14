from typing import Dict, Tuple
from app.data.resources.base_catalog import ManifestCatalog

class IconSheet():
    def __init__(self, nid, full_path=None, subicon_dict = None):
        self.nid = nid
        self.full_path = full_path
        self.image = None
        self.pixmap = None
        self._subicon_dict: Dict[str, Tuple[int, int]] = subicon_dict or {}
        self._index_to_name: Dict[Tuple[int, int], str] = {tuple(index): name for name, index in self._subicon_dict.items()}

    def set_alias(self, name, index):
        self._subicon_dict[name] = index
        self._index_to_name[index] = name

    def get_index(self, name):
        return self._subicon_dict.get(name, None)

    def get_alias(self, index):
        return self._index_to_name.get(index, None) or "%s %d, %d" % (self.nid, index[0], index[1])

    def set_full_path(self, full_path):
        self.full_path = full_path

    def save(self):
        return {
            'nid': self.nid,
            'subicon_dict': self._subicon_dict
        }

    @classmethod
    def restore(cls, s_dict):
        if isinstance(s_dict, str): # old version where we only saved the nid
            self = cls(s_dict)
        else:
            self = cls(
                s_dict['nid'],
                subicon_dict=s_dict.get('subicon_dict')
            )
        return self

class Icon():
    def __init__(self, nid, icon_index, parent_nid):
        self.image = None
        self.pixmap = None

        self.nid = nid
        self.icon_index = icon_index
        self.parent_nid = parent_nid

class IconCatalog(ManifestCatalog[IconSheet]):
    manifest = 'icons.json'
    title = 'icons'
    filetype = '.png'
    datatype = IconSheet

class Icon16Catalog(IconCatalog):
    manifest = 'icons16.json'

class Icon32Catalog(IconCatalog):
    manifest = 'icons32.json'

class Icon80Catalog(IconCatalog):
    manifest = 'icons80.json'
