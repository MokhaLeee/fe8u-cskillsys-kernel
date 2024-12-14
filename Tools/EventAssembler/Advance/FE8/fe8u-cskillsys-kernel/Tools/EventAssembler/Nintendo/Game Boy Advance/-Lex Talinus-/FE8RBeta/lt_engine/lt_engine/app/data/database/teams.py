from dataclasses import dataclass

from typing import List, Optional, Set, Tuple

from app.utilities.data import Data, Prefab
from app.utilities.typing import NID
from app.utilities import str_utils


@dataclass
class Team(Prefab):
    nid: NID
    map_sprite_palette: NID = None  # Used for map sprites
    combat_variant_palette: str = None  # Used for battle animation
    combat_color: str = 'red'
    phase_change_sound_effect: Optional[NID] = None  # Defaults to "Next Turn" in engine

    @classmethod
    def default(cls):
        return cls('')

class TeamCatalog(Data[Team]):
    datatype = Team
    # Order determine phase order
    # These teams cannot be removed
    default_teams = [
        'player', 'enemy', 'enemy2', 'other',
    ]
    default_combat_palettes = [
        'GenericBlue', 'GenericRed', 'GenericPurple', 'GenericGreen',
    ]
    default_colors = [
        'blue', 'red', 'purple', 'green',
    ]

    def __init__(self):
        super().__init__()
        self.alliance_pairs: Set[Tuple[NID, NID]] = set()
        self.add_defaults()

    def add_defaults(self):
        if 'player' not in self.keys():
            self.alliance_pairs.add(('other', 'player'))
        for idx, nid in enumerate(self.default_teams):
            if nid not in self.keys():
                team = Team(
                    nid, 'map_sprite_%s' % self.default_colors[idx],
                    self.default_combat_palettes[idx], 
                    self.default_colors[idx])
                self.append(team)

    def save(self):
        vals: list = super().save()
        return (vals, list(self.alliance_pairs))
        
    def restore(self, vals):
        if not vals:
            vals = []
        if len(vals) == 2:
            vals, alliance_pairs = vals
        else:  # Deprecated support
            vals = vals
            alliance_pairs = [('other', 'player')]
        self = super().restore(vals)

        self.alliance_pairs = set()
        for a, b in alliance_pairs:
            self.alliance_pairs.add((a, b))

        self.add_defaults()
        return self

    # These functions handle alliances using the 
    # alliance_pairs data set
    def set_allies(self, main_team: NID, teams: List[NID]):
        for team_nid in self.keys():
            if main_team == team_nid:
                continue
            elif main_team < team_nid:
                pair = (main_team, team_nid)
            else:
                pair = (team_nid, main_team)
            if team_nid in teams:
                self.alliance_pairs.add(pair)
            else:
                self.alliance_pairs.discard(pair)

    def get_allies(self, team: NID) -> List[NID]:
        allies = [team]
        for pair in self.alliance_pairs:
            if team == pair[0]:
                allies.append(pair[1])
            elif team == pair[1]:
                allies.append(pair[0])
        return allies

    def is_allied(self, team1: NID, team2: NID) -> bool:
        if team1 == team2:
            return True
        pair1 = (team1, team2)
        pair2 = (team2, team2)
        if pair1 in self.alliance_pairs:
            return True
        elif pair2 in self.alliance_pairs:
            return True
        return False

    def player(self) -> Team:
        # player is a special team that is used often throughout the engine
        # because it is the user's controlled team
        return self.get('player')

    @property    
    def allies(self) -> List[NID]:
        return self.get_allies('player')

    @property    
    def enemies(self) -> List[NID]:
        return [team.nid for team in self if team.nid not in self.allies]

    def create_new(self, db):
        nids = [d.nid for d in self]
        nid = str_utils.get_next_name("New Team", nids)
        new_team = Team(nid)
        self.append(new_team)
        return new_team
