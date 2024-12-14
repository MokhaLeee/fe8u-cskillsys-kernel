import sys

from app.data.database.database import DB
from app.engine.game_state import game

class Record():
    def __init__(self):
        self.turn = game.turncount
        self.level_nid = game.level.nid if game.level else None

    def save(self):
        ser_dict = {}
        for attr in self.__dict__.items():
            name, value = attr
            ser_dict[name] = value
        return (self.__class__.__name__, ser_dict)

    @classmethod
    def restore(cls, ser_dict):
        self = cls.__new__(cls)
        for name, value in ser_dict.items():
            setattr(self, name, value)
        return self

class KillRecord(Record):
    def __init__(self, killer: str, killee: str):
        super().__init__()
        self.killer = killer
        self.killee = killee

class DamageRecord(Record):
    def __init__(self, dealer: str, receiver: str, item_nid: str, over_damage: int, damage: int, kind: str):
        super().__init__()
        self.dealer = dealer
        self.receiver = receiver
        self.item_nid = item_nid
        self.over_damage = over_damage
        self.damage = damage
        self.kind = kind  # hit, crit, miss

class ItemRecord(Record):
    def __init__(self, user: str, item_nid: str):
        super().__init__()
        self.user = user
        self.item_nid = item_nid

class StealRecord(Record):
    def __init__(self, stealer: str, stealee: str, item_nid: str):
        super().__init__()
        self.stealer = stealer
        self.stealee = stealee
        self.item_nid = item_nid

class CombatRecord(Record):
    def __init__(self, attacker: str, defender: str, result: str):
        super().__init__()
        self.attacker = attacker
        self.defender = defender
        self.result = result

class LevelRecord(Record):
    def __init__(self, unit_nid: str, num: int, klass: str):
        super().__init__()
        self.unit_nid = unit_nid
        self.num = num
        self.klass = klass

class MoneyRecord(Record):
    def __init__(self, party_nid: str, num: int):
        super().__init__()
        self.party_nid = party_nid
        self.num = num

class Recordkeeper():
    """
    Needs to keep track of:
    Kills
    Damage Dealt
    Overkill Damage
    Damage Received
    Damage Prevented/Blocked
    Healing (Self and Other)
    Hits/Crits/Misses
    Levels Gained/Exp Gained
    Turnwheel Uses
    Deaths
    Using an Item
    Stealing an Item
    Recruiting a Unit
    Turns Taken
    Money Gained/Lost

    And for all these, needs to know what Chapter and Turn
    """

    def __init__(self):
        self.kills = []
        self.damage = []
        self.healing = []
        self.death = []
        self.item_use = []
        self.steal = []
        self.combat_results = []
        self.turns_taken = []
        self.levels = []
        self.exp = []
        self.money = []

    def save(self):
        ser_dict = {}
        ser_dict['kills'] = [record.save() for record in self.kills]
        ser_dict['damage'] = [record.save() for record in self.damage]
        ser_dict['healing'] = [record.save() for record in self.healing]
        ser_dict['death'] = [record.save() for record in self.death]
        ser_dict['item_use'] = [record.save() for record in self.item_use]
        ser_dict['steal'] = [record.save() for record in self.steal]
        ser_dict['combat_results'] = [record.save() for record in self.combat_results]
        ser_dict['turns_taken'] = [record.save() for record in self.turns_taken]
        ser_dict['levels'] = [record.save() for record in self.levels]
        ser_dict['exp'] = [record.save() for record in self.exp]
        ser_dict['money'] = [record.save() for record in self.money]
        return ser_dict

    @classmethod
    def restore(cls, ser_dict):
        self = cls()
        for name, data in ser_dict.items():
            cur_list = getattr(self, name)
            for obj in data:
                obj_name, value = obj
                record_type = getattr(sys.modules[__name__], obj_name)
                record = record_type.restore(value)
                cur_list.append(record)
        return self

    def append(self, record_type: str, data: tuple):
        if record_type == 'kill':
            self.kills.append(KillRecord(*data))
        elif record_type == 'damage':
            self.damage.append(DamageRecord(*data))
        elif record_type == 'heal':
            self.healing.append(DamageRecord(*data))
        elif record_type == 'death':
            self.death.append(KillRecord(*data))
        elif record_type == 'item_use':
            self.item_use.append(ItemRecord(*data))
        elif record_type == 'steal':
            self.steal.append(StealRecord(*data))
        elif record_type == 'hit':
            self.combat_results.append(CombatRecord(*data, 'hit'))
        elif record_type == 'miss':
            self.combat_results.append(CombatRecord(*data, 'miss'))
        elif record_type == 'crit':
            self.combat_results.append(CombatRecord(*data, 'crit'))
        elif record_type == 'turn':
            self.turns_taken.append(Record())
        elif record_type == 'level_gain':
            self.levels.append(LevelRecord(*data))
        elif record_type == 'exp_gain':
            self.exp.append(LevelRecord(*data))
        elif record_type == 'money':
            self.money.append(MoneyRecord(*data))

    def pop(self, record_type: str) -> Record:
        if record_type == 'kill':
            return self.kills.pop()
        elif record_type == 'damage':
            return self.damage.pop()
        elif record_type == 'heal':
            return self.healing.pop()
        elif record_type == 'death':
            return self.player_death.pop()  # Death records aren't overwritten by turnwheel
        elif record_type == 'item_use':
            return self.item_use.pop()
        elif record_type == 'steal':
            return self.steal.pop()
        elif record_type in ('hit', 'miss', 'crit'):
            return self.combat_results.pop()
        elif record_type == 'turn':
            return self.turns_taken.pop()
        elif record_type == 'level_gain':
            return self.levels.pop()
        elif record_type == 'exp_gain':
            return self.exp.pop()
        elif record_type == 'money_gain':
            return self.money.pop()

    # Interogation functions
    def get_levels(self) -> list:
        """
        Returns list of chapters played in order
        """
        levels = []
        for record in self.turns_taken:
            if record.level_nid not in levels and \
                    DB.levels.get(record.level_nid).should_record:
                levels.append(record.level_nid)
        return levels

    def get_turncounts(self, level_list: list) -> list:
        """
        For each level in list, return the number of turns spent in level/chapter
        """
        turncounts = []
        for level in level_list:
            max_turncount = 0
            for record in self.turns_taken:
                if record.level_nid == level:
                    max_turncount = max(record.turn, max_turncount)
            turncounts.append(max_turncount)
        return turncounts

    def get_kills(self, unit_nid: str, level_nid: str = None) -> int:
        """
        Returns number of kills by unit in chapter
        If level_nid is None, for all chapters
        """
        if level_nid is not None:
            return len([record for record in self.kills if record.killer == unit_nid and record.level_nid == level_nid])
        else:
            return len([record for record in self.kills if record.killer == unit_nid])

    def get_damage(self, unit_nid: str, level_nid: str = None) -> int:
        """
        Returns total damage dealt by unit in chapter
        If level_nid is None, for all chapters
        """
        if level_nid is not None:
            return sum([record.damage for record in self.damage if record.dealer == unit_nid and record.level_nid == level_nid])
        else:
            return sum([record.damage for record in self.damage if record.dealer == unit_nid])

    def get_heal(self, unit_nid: str, level_nid: str = None) -> int:
        """
        Returns total healing done by unit in chapter
        If level_nid is None, for all chapters
        """
        if level_nid is not None:
            return sum([record.damage for record in self.healing if record.dealer == unit_nid and record.receiver != unit_nid and record.level_nid == level_nid])
        else:
            return sum([record.damage for record in self.healing if record.dealer == unit_nid and record.receiver != unit_nid])

    def determine_score(self, unit_nid: str, level_nid: str = None) -> int:
        """
        Returns score for unit in chapter
        If level_nid is None, instead for all chapters
        """
        kill_score = self.get_kills(unit_nid, level_nid)
        damage_score = self.get_damage(unit_nid, level_nid)
        heal_score = self.get_heal(unit_nid, level_nid)

        return kill_score * 20 + damage_score + heal_score

    def get_mvp(self, level_nid: str = None) -> str:
        """
        Returns which unit has the highest score
        of units that participated in the level
        If level_nid is None, instead for all levels
        """
        best_score = -1
        mvp = None
        player_units = [unit for unit in game.units if unit.team == 'player' and unit.persistent]
        for unit in player_units:
            score = self.determine_score(unit.nid, level_nid)
            if score > best_score:
                best_score = score
                mvp = unit.nid
        if mvp:
            return mvp
        else:
            return game.get_all_units_in_party()[0].nid

    def get_killer(self, unit_nid: str, level_nid: str = None):
        """
        Returns the most recent killer of this unit in this level
        """
        for record in reversed(self.kills):
            if record.killee == unit_nid:
                if not level_nid or record.level_nid == level_nid:
                    return record.killer
        return None
