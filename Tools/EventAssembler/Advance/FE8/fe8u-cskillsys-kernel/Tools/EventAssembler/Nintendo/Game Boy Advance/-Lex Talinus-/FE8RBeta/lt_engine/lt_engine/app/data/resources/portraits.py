from app.data.resources.base_catalog import ManifestCatalog

class PortraitPrefab():
    def __init__(self, nid, full_path=None, pix=None):
        self.nid = nid
        self.full_path = full_path
        self.image = None
        self.pixmap = pix

        self.blinking_offset = [0, 0]
        self.smiling_offset = [0, 0]
        self.info_offset = 0

    def set_full_path(self, full_path):
        self.full_path = full_path

    def save(self):
        s_dict = {}
        s_dict['nid'] = self.nid
        s_dict['blinking_offset'] = self.blinking_offset
        s_dict['smiling_offset'] = self.smiling_offset
        s_dict['info_offset'] = self.info_offset
        return s_dict

    @classmethod
    def restore(cls, s_dict):
        self = cls(s_dict['nid'])
        self.blinking_offset = [int(_) for _ in s_dict['blinking_offset']]
        self.smiling_offset = [int(_) for _ in s_dict['smiling_offset']]
        self.info_offset = int(s_dict.get('info_offset', 0))
        return self

class PortraitCatalog(ManifestCatalog[PortraitPrefab]):
    manifest = 'portraits.json'
    title = 'portraits'
    datatype = PortraitPrefab
