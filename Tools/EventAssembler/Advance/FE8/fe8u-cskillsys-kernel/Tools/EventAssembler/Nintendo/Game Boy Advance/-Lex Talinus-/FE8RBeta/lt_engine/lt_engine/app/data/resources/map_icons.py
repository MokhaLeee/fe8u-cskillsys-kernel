from app.data.resources.base_catalog import ManifestCatalog

class MapIcon():
    def __init__(self, nid, full_path=None):
        self.nid = nid
        self.full_path = full_path
        self.image = None
        self.pixmap = None

    def set_full_path(self, full_path):
        self.full_path = full_path

    def get_pixmap(self):
        from PyQt5.QtGui import QPixmap
        if not self.pixmap:
            self.pixmap = QPixmap(self.full_path)
        return self.pixmap

    def save(self):
        return self.nid

    @classmethod
    def restore(cls, nid):
        self = cls(nid)
        return self

class MapIconCatalog(ManifestCatalog[MapIcon]):
    manifest = 'map_icons.json'
    title = 'map icons'
    datatype = MapIcon

    @classmethod
    def DEFAULT(self):
        return 'map_node'
