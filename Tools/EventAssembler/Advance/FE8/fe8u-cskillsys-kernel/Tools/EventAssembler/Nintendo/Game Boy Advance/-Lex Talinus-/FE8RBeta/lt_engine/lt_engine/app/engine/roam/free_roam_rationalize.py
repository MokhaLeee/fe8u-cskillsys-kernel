from app.engine.game_state import game
from app.engine.state import MapState
from app.engine.movement.rationalize_movement_component import RationalizeMovementComponent

class FreeRoamRationalizeState(MapState):
    name = 'free_roam_rationalize'

    def begin(self):
        self.movement_components = self._get_units_to_rationalize()
        for mc in self.movement_components:
            game.movement.add(mc)

    def _get_units_to_rationalize(self):
        movement_components = []
        for unit in game.get_all_units():
            if unit.sprite.get_roam_position():
                new_movement_component = RationalizeMovementComponent(unit)
                movement_components.append(new_movement_component)
        return movement_components

    def update(self):
        super().update()
        game.movement.update()
        if len(game.movement) <= 0:
            game.state.back()
            return 'repeat'
