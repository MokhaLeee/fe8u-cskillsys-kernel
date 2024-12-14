from dataclasses import dataclass

from app.utilities.data import Data, Prefab
from app.utilities import str_utils


@dataclass
class PartyPrefab(Prefab):
    nid: str = None
    name: str = None
    leader: str = None

class PartyCatalog(Data[PartyPrefab]):
    datatype = PartyPrefab

    def create_new(self, db):
        nids = [d.nid for d in self]
        nid = name = str_utils.get_next_name("New Party", nids)
        new_party = PartyPrefab(nid, name, db.units[0].nid)
        self.append(new_party)
        return new_party
