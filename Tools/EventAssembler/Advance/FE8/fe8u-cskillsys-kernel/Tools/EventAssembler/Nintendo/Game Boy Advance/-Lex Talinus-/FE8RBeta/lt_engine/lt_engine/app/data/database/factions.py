from dataclasses import dataclass

from app.utilities.data import Data, Prefab
from app.utilities import str_utils

@dataclass
class Faction(Prefab):
    nid: str = None
    name: str = None
    desc: str = ""

    icon_nid: str = None
    icon_index: tuple = (0, 0)

class FactionCatalog(Data[Faction]):
    datatype = Faction

    def create_new(self, db):
        nids = [d.nid for d in self]
        nid = name = str_utils.get_next_name("New Faction", nids)
        new_faction = Faction(nid, name)
        self.append(new_faction)
        return new_faction
