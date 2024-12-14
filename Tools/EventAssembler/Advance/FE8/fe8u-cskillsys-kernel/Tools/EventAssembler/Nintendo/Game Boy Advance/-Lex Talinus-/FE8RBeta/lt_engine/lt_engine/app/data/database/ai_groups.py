from dataclasses import dataclass

from app.utilities.data import Prefab

@dataclass
class AIGroup(Prefab):
    nid: str = None
    trigger_threshold: int = None

