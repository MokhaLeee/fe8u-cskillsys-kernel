from dataclasses import dataclass

from app.utilities.data import Data, Prefab
from app.utilities import str_utils

@dataclass
class Lore(Prefab):
    nid: str = None
    name: str = None
    title: str = None
    category: str = "Character"
    text: str = ""

class LoreCatalog(Data[Lore]):
    datatype = Lore

    def create_new(self, db):
        nids = [d.nid for d in self]
        nid = name = str_utils.get_next_name("New Lore", nids)
        new_lore = Lore(nid, name, name)
        self.append(new_lore)
        return new_lore
