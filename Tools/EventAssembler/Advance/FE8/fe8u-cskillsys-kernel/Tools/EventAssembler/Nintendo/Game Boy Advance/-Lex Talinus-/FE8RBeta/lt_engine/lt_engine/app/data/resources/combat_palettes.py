import json
import os
from pathlib import Path
import re
import shutil
from typing import Dict, List
from app.utilities.data import Prefab
from app.data.resources.base_catalog import ManifestCatalog
from app.data.resources.default_palettes import default_palettes

from app.constants import COLORKEY
from app.utilities.data_order import parse_order_keys_file

class Palette(Prefab):
    def __init__(self, nid):
        self.nid = nid
        # Mapping of color indices to true colors
        # Color indices are generally (0, 1) -> (240, 160, 240), etc.
        self.colors = {(0, 0): COLORKEY}

    def is_similar(self, colors) -> bool:
        counter = 0
        my_colors = [color for coord, color in self.colors.items()]
        for color in colors:
            if color in my_colors:
                counter += 1
        # Similar if more than 75% of colors match
        return (counter / len(colors)) > .75

    def assign_colors(self, colors: list):
        self.colors = {
            (int(idx % 8), int(idx / 8)): color for idx, color in enumerate(colors)
        }

    def get_colors(self) -> list:
        """
        # Returns just the colors in the right order
        # not the coord
        """
        colors = list(sorted([(coord[::-1], color) for coord, color in self.colors.items()]))
        colors = [color for coord, color in colors]
        return colors

    def save(self):
        return (self.nid, list(self.colors.items()))

    @classmethod
    def restore(cls, s):
        self = cls(s[0])
        self.colors = {tuple(k): tuple(v) for k, v in s[1].copy()}
        return self

    @classmethod
    def from_list(cls, nid, colors):
        self = cls(nid)
        self.assign_colors(colors)
        return self

class PaletteCatalog(ManifestCatalog[Palette]):
    datatype = Palette
    manifest = 'palettes.json'
    title = 'palettes'
    multi_loc = 'palette_data'

    def save(self, loc):
        # No need to finagle with full paths
        # Because Palettes don't have any connection to any actual file.
        self.dump(loc)

    def load(self, loc):
        single_loc = os.path.join(loc, self.manifest)
        multi_loc = os.path.join(loc, self.multi_loc)
        if not os.path.exists(multi_loc):  # use the old method, single location in palettes.json
            if not os.path.exists(single_loc):
                return
            palette_dict = self.read_manifest(single_loc)
            for s_dict in palette_dict:
                new_palette = Palette.restore(s_dict)
                self.append(new_palette)
        else:   # new distributed saving
            data_fnames = os.listdir(multi_loc)
            save_data = []
            for fname in data_fnames:
                if not fname.endswith('.json'):
                    continue
                save_loc = os.path.join(multi_loc, fname)
                # logging.info("Deserializing %s from %s" % ('palette data', save_loc))
                with open(save_loc) as load_file:
                    for data in json.load(load_file):
                        save_data.append(data)
            if '.orderkeys' in data_fnames:  # using order key file
                ordering = parse_order_keys_file(Path(multi_loc, '.orderkeys'))
                save_data = sorted(save_data, key=lambda data: ordering.index(data[0]) if data[0] in ordering else 99999)
            else:
                save_data = sorted(save_data, key=lambda obj: obj[2])
            for s_dict in save_data:
                new_palette = Palette.restore(s_dict)
                self.append(new_palette)

        # Always load in the default map sprite palettes
        for palette_nid, colors in default_palettes.items():
            if palette_nid not in self.keys():
                new_palette = Palette.from_list(palette_nid, colors)
                self.append(new_palette)

    def dump(self, loc):
        saves = [datum.save() for datum in self]
        save_dir = os.path.join(loc, self.multi_loc)
        if os.path.exists(save_dir):
            shutil.rmtree(save_dir)
        os.mkdir(save_dir)
        orderkeys: List[str] = []
        for idx, save in enumerate(saves):
            # ordering
            save = list(save)  # by default a tuple
            nid = save[0]
            orderkeys.append(nid)
            nid = re.sub(r'[\\/*?:"<>|]', "", nid)
            nid = nid.replace(' ', '_')
            save_loc = os.path.join(save_dir, nid + '.json')
            with open(save_loc, 'w') as serialize_file:
                json.dump([save], serialize_file, indent=4)
        with open(os.path.join(save_dir, '.orderkeys'), 'w') as orderkey_file:
            json.dump(orderkeys, orderkey_file, indent=4)
