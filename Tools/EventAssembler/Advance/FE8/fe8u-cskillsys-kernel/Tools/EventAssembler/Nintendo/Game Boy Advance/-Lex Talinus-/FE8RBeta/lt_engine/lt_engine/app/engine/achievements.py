import logging
from app.utilities.data import Data, Prefab

from app.engine import persistent_data

from app.data.database.database import DB

class Achievement(Prefab):
    def __init__(self, nid: str = '', name: str = '', desc: str = '', complete: bool = False, hidden=False):
        self.nid = nid
        self.name = name
        self.desc = desc
        self.complete = bool(complete)
        self.hidden = bool(hidden)

    def set_complete(self, complete: bool):
        self.complete = bool(complete)

    def get_complete(self) -> bool:
        return self.complete

    def get_hidden(self) -> bool:
        return self.hidden and not self.get_complete()

class AchievementManager(Data):
    datatype = Achievement

    def __init__(self, location):
        super().__init__()
        self.location = location

    def add_achievement(self, nid: str, name: str, desc: str, complete=False, hidden=False):
        if nid not in self.keys():
            new_achievement = Achievement(nid, name, desc, complete, hidden)
            self.append(new_achievement)
        else:
            logging.info("Attempted to define already existing achievement with nid %s", nid)
        persistent_data.serialize(self.location, self.save())

    def update_achievement(self, nid, name, desc, hidden):
        if nid in self.keys():
            a = self.get(nid)
            a.name = name
            a.desc = desc
            a.hidden = hidden
        else:
            logging.info("Attempted to update non-existant achievement with nid %s", nid)
        persistent_data.serialize(self.location, self.save())

    def remove_achievement(self, nid: str):
        if nid in self.keys():
            self.remove_key(nid)
        else:
            logging.info("Attempted to remove non-existant achievement with nid %s", nid)
        persistent_data.serialize(self.location, self.save())

    def check_achievement(self, nid: str) -> bool:
        return nid in self.keys() and self.get(nid).get_complete()

    def complete_achievement(self, nid: str, complete: bool, banner: bool) -> bool:
        if nid in self.keys():
            a = self.get(nid)
            a.set_complete(complete)
        else:
            logging.info("Attempted to complete non-existant achievement with nid %s", nid)
            return False
        persistent_data.serialize(self.location, self.save())
        return complete

    def clear_achievements(self):
        self.clear()
        persistent_data.serialize(self.location, self.save())

def reset():
    game_id = str(DB.constants.value('game_nid'))
    location = 'saves/' + game_id + '-achievements.p'
    ACHIEVEMENTS.location = location
    data = persistent_data.deserialize(location)
    if data:
        ACHIEVEMENTS.restore(data)
    else:
        ACHIEVEMENTS.clear()

# Make sure to reload all achievements whenever we start the engine
game_id = str(DB.constants.value('game_nid'))
location = 'saves/' + game_id + '-achievements.p'
achievement_data = persistent_data.deserialize(location)
ACHIEVEMENTS = AchievementManager(location)
if achievement_data:
    ACHIEVEMENTS.restore(achievement_data)
