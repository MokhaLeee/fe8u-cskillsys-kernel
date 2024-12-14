from dataclasses import dataclass
from app.utilities.typing import NID

@dataclass
class RoamInfo:
    roam: bool = False
    roam_unit_nid: NID = None

    def clear(self):
        self.roam = False
        self.roam_unit = None
