from dataclasses import asdict, dataclass, field
import os
from pathlib import Path
from typing import Dict, List, Optional
from app.data.database.game_flags import dataclass_from_dict

from app.data.resources.base_catalog import ManifestCatalog
from app.utilities.typing import Color4

@dataclass
class Font():
    nid: str                                                                             #: NID of the font.
    file_name: str = None                                                                #: Root path without file ending, e.g. `project.ltproj/resources/convo`.
    fallback_ttf: str = None                                                             #: name of the .ttf file (in this directory) to be used as a fallback if the main font cannot render anything
    fallback_size: int = 16                                                              #: what size the fallback font should be rendered at. This may well differ depending on the font, especially at these lower resolutions!
    default_color: Optional[str] = None                                                  #: The key of the default color in the palette, if any.
    outline_font: bool = False                                                           #: whether this font has an outline
    palettes: Dict[str, List[Color4]] = field(default_factory=dict)                      #: A dictionary of color names to the palette of the font color.

    def image_path(self):
        return self.file_name + '.png'

    def index_path(self):
        return self.file_name + '.idx'

    def ttf_path(self):
        if self.fallback_ttf:
            return str(Path(self.file_name).parent / self.fallback_ttf)

    def primary_color(self, color):
        palette = self.palettes.get(color)
        if palette:
            return palette[0]
        return None

    def secondary_color(self, color):
        palette = self.palettes.get(color)
        if palette:
            if len(palette) > 1:
                return palette[1]
            return palette[0]
        return None

    def save(self):
        s_dict = asdict(self)
        del s_dict["file_name"]
        return s_dict

    @classmethod
    def restore(cls, s_dict):
        return dataclass_from_dict(cls, s_dict)

class FontCatalog(ManifestCatalog[Font]):
    datatype = Font
    manifest = 'fonts.json'
    title = 'fonts'
    filetype = ''

    def load(self, loc):
        resource_dict = self.read_manifest(os.path.join(loc, self.manifest))
        if len(resource_dict) == 0:
            self.load('default.ltproj/resources/fonts')
            return
        for s_dict in resource_dict:
            new_resource = self.datatype.restore(s_dict)
            new_resource.file_name = os.path.join(loc, new_resource.nid)
            self.append(new_resource)

    def valid_files(self) -> set:
        files = set()
        for datum in self:
            files.add(datum.nid + '.idx')
            files.add(datum.nid + '.png')
            files.add(datum.fallback_ttf)
        if None in files:
            files.remove(None)
        return files

    def save(self, loc):
        for datum in self:
            new_idx_path = os.path.join(loc, datum.nid + '.idx')
            if os.path.abspath(datum.index_path()) != os.path.abspath(new_idx_path):
                self.make_copy(datum.index_path(), new_idx_path)
            new_image_path = os.path.join(loc, datum.nid + '.png')
            if os.path.abspath(datum.image_path()) != os.path.abspath(new_image_path):
                self.make_copy(datum.image_path(), new_image_path)
            if datum.ttf_path():
                new_ttf_path = os.path.join(loc, datum.fallback_ttf)
                if os.path.abspath(datum.ttf_path()) != os.path.abspath(new_ttf_path):
                    self.make_copy(datum.ttf_path(), new_ttf_path)
            datum.file_name = os.path.join(loc, datum.nid)
        self.dump(loc)
