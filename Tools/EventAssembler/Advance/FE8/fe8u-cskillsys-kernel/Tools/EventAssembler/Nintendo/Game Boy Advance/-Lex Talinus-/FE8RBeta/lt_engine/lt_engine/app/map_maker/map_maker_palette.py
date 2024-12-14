import glob, os
import json
from dataclasses import dataclass

from PyQt5.QtCore import Qt, QAbstractListModel

from app.utilities.data import Data

@dataclass
class MapMakerPalette:
    nid: str
    name: str
    author: str
    palette_path: str

class MapMakerPaletteCatalog(Data[MapMakerPalette]):
    datatype = MapMakerPalette

class PaletteModel(QAbstractListModel):
    def __init__(self, data, window):
        super().__init__(window)
        self._data = data
        self.window = window

    def rowCount(self, parent=None):
        return len(self._data)

    def data(self, index, role):
        if not index.isValid():
            return None
        if role == Qt.DisplayRole:
            palette = self._data[index.row()]
            text = "%s (%s)" % (palette.name, palette.author)
            return text
        return None

PALETTES = MapMakerPaletteCatalog()

def load_palettes():
    valid_palette_folders = glob.glob('app/map_maker/palettes/*')
    for fn in valid_palette_folders:
        metadata_path = os.path.join(fn, 'metadata.json')
        if os.path.isdir(fn) and os.path.exists(metadata_path):
            # Read metadata
            with open(metadata_path) as fp:
                metadata = json.load(fp)
                new_palette = MapMakerPalette(metadata['name'], metadata['name'], metadata['author'], fn)
                PALETTES.append(new_palette)

load_palettes()
