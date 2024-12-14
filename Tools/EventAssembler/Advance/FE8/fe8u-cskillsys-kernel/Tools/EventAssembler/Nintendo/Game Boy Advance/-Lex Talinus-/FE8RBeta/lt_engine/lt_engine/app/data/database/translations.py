from dataclasses import dataclass

from app.utilities.data import Data, Prefab
from app.utilities import str_utils

@dataclass
class Translation(Prefab):
    nid: str = None
    text: str = ""

class TranslationCatalog(Data[Translation]):
    datatype = Translation

    def add_new_default(self, db):
        nid = str_utils.get_next_name("word", self.keys())
        new_translation = Translation(nid, "")
        self.append(new_translation)
        return new_translation
