from __future__ import annotations
from app.utilities.typing import NID
from app.data.database.difficulty_modes import DifficultyModePrefab, GrowthOption, PermadeathOption
from app.utilities.data import Prefab

class DifficultyModeObject(Prefab):
    def __init__(self, nid: NID = None, permadeath: bool = None, growths: GrowthOption = None):
        self.nid: NID = nid
        self.permadeath: bool = permadeath
        self.growths: GrowthOption = growths
        self.enemy_autolevels: int = 0
        self.enemy_truelevels: int = 0
        self.boss_autolevels: int = 0
        self.boss_truelevels: int = 0

    def save(self):
        return {'nid': self.nid,
                'permadeath': self.permadeath,
                'growths': self.growths,
                'enemy_autolevels': self.enemy_autolevels,
                'enemy_truelevels': self.enemy_truelevels,
                'boss_autolevels': self.boss_autolevels,
                'boss_truelevels': self.boss_truelevels,
                }

    @classmethod
    def restore(cls, s_dict):
        difficulty_mode = cls(s_dict['nid'], s_dict['permadeath'], s_dict['growths'])
        difficulty_mode.enemy_autolevels = s_dict.get('enemy_autolevels', 0)
        difficulty_mode.enemy_truelevels = s_dict.get('enemy_truelevels', 0)
        difficulty_mode.boss_autolevels = s_dict.get('boss_autolevels', 0)
        difficulty_mode.boss_truelevels = s_dict.get('boss_truelevels', 0)
        return difficulty_mode

    @classmethod
    def from_prefab(cls, prefab: DifficultyModePrefab) -> DifficultyModeObject:
        difficulty_mode = cls()
        difficulty_mode.nid = prefab.nid
        difficulty_mode.permadeath = prefab.permadeath_choice == PermadeathOption.CLASSIC
        difficulty_mode.growths = prefab.growths_choice
        return difficulty_mode
