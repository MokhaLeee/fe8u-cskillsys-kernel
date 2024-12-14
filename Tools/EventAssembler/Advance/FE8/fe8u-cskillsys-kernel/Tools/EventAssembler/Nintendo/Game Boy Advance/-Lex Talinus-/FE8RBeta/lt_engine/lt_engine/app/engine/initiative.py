from app.utilities import utils
from app.engine import equations
from app.engine.game_state import game

class InitiativeTracker():
    def __init__(self):
        self.unit_line = []
        self.initiative_line = []
        self.current_idx = -1
        self.draw_me = True

    def clear(self):
        self.unit_line.clear()
        self.initiative_line.clear()
        self.current_idx = -1

    def at_start(self):
        return self.current_idx == 0 or self.current_idx == -1

    def next(self):
        self.current_idx += 1
        if self.current_idx >= len(self.unit_line):
            self.current_idx = 0

    def back(self):
        self.current_idx -= 1
        if self.current_idx < 0:
            self.current_idx = len(self.unit_line) - 1

    def start(self, units):
        # Sort descending
        units = list(sorted(units, key=lambda unit: equations.parser.get_initiative(unit), reverse=True))
        self.unit_line = [unit.nid for unit in units]
        self.initiative_line = [equations.parser.get_initiative(unit) for unit in units]
        self.current_idx = -1

    def get_current_unit(self):
        return game.get_unit(self.unit_line[self.current_idx])

    def get_previous_unit(self):
        return game.get_unit(self.unit_line[(self.current_idx - 1) % len(self.unit_line)])

    def get_next_unit(self):
        return game.get_unit(self.unit_line[(self.current_idx + 1) % len(self.unit_line)])

    def get_initiative(self, unit):
        idx = self.get_index(unit)
        if idx is not None:
            return self.initiative_line[idx]

    def get_index(self, unit):
        if unit.nid in self.unit_line:
            return self.unit_line.index(unit.nid)
        return None

    def append_unit(self, unit, initiative=-1):
        self.unit_line.append(unit.nid)
        self.initiative_line.append(initiative)

    def pop_unit(self):
        self.unit_line.pop()
        self.initiative_line.pop()
        # If we popped the current unit, move back to beginning
        if self.current_idx == len(self.unit_line):
            self.current_idx = 0

    def insert_unit(self, unit):
        initiative = equations.parser.get_initiative(unit)
        self._insort(unit.nid, initiative)

    def remove_unit(self, unit):
        if unit.nid in self.unit_line:
            idx = self.unit_line.index(unit.nid)
            self.unit_line.pop(idx)
            self.initiative_line.pop(idx)
            if self.current_idx >= idx:
                self.current_idx -= 1

    def insert_at(self, unit, idx: int, initiative=None) -> int:
        """
        Don't use the initiative argument unless you know what you are doing
        """
        if idx == len(self.unit_line):
            self.append_unit(unit, initiative)
            return idx
        idx = utils.clamp(idx, 0, len(self.initiative_line) - 1)
        if initiative is not None:
            initiative_at = initiative
        else:
            initiative_at = self.initiative_line[idx]
        self.unit_line.insert(idx, unit.nid)
        self.initiative_line.insert(idx, initiative_at)
        if self.current_idx > idx:
            self.current_idx += 1
        return idx

    def _insort(self, unit_nid: str, initiative: int):
        lo, hi = 0, len(self.unit_line)
        while lo < hi:
            mid = (lo + hi) // 2
            if initiative > self.initiative_line[mid]:
                hi = mid
            else:
                lo = mid + 1
        self.unit_line.insert(lo, unit_nid)
        self.initiative_line.insert(lo, initiative)
        if self.current_idx > lo:
            self.current_idx += 1

    def toggle_draw(self):
        self.draw_me = not self.draw_me
