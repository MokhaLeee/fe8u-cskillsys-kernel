from app.engine.sound import get_sound_thread
from app.engine import action, engine
from app.engine.game_state import game

class DeathManager():
    def __init__(self):
        self.dying_units = {}

    def should_die(self, unit):
        unit.is_dying = True
        self.dying_units[unit.nid] = 0

    def miracle(self, unit):
        unit.is_dying = False
        if unit.nid in self.dying_units:
            del self.dying_units[unit.nid]
        unit.sprite.flicker.clear()
        unit.sprite.change_state('normal')
        unit.sprite.set_transition('normal')

    def force_death(self, unit):
        unit.is_dying = False
        action.do(action.Die(unit))
        if unit.nid in self.dying_units:
            del self.dying_units[unit.nid]

    def update(self) -> bool:
        current_time = engine.get_time()
        for unit_nid in list(self.dying_units.keys()):
            death_counter = self.dying_units[unit_nid]
            unit = game.get_unit(unit_nid)
            if death_counter == 0:
                get_sound_thread().play_sfx('Death')
                unit.sprite.start_flicker(0, unit.sprite.default_transition_time, (255, 255, 255), fade_out=False)
                unit.sprite.set_transition('fade_out')
                self.dying_units[unit_nid] = engine.get_time()

            elif current_time - death_counter >= unit.sprite.default_transition_time - 50:
                self.force_death(unit)

        return not self.dying_units  # Done when no dying units left

    def is_dying(self, unit):
        return unit.nid in self.dying_units
