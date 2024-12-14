from __future__ import annotations
from app.engine.camera import Camera

from typing import TYPE_CHECKING, Tuple

from app.engine.game_counters import ANIMATION_COUNTERS
from app.engine.overworld.overworld_manager import OverworldManager

if TYPE_CHECKING:
    from app.engine.objects.overworld import OverworldNodeObject, OverworldEntityObject

from app.constants import TILEWIDTH
from app.engine import engine
from app.engine.cursor import BaseCursor
from app.engine.sprites import SPRITES

class OverworldCursor(BaseCursor):
    """Contains nearly identical functionality to the cursor, but
    overrides the sprite and certain overworld-specific functionality.
    """
    def __init__(self, camera: Camera):
        super().__init__(camera=camera)
        self.overworld: OverworldManager = None
        self._sprite = SPRITES.get('overworld_cursor')

    def set_overworld_manager(self, manager: OverworldManager):
        """Must be called in order to use the cursor.

        Args:
            manager (OverworldManager): Manager
        """
        self.overworld = manager
        self._bounds = self.overworld.map_bounds()
        self.position = (self.get_bounds()[0], self.get_bounds()[1])

    def set_speed_state(self, val: bool):
        if val:
            self._transition_speed = 2
        else:
            self._transition_speed = 1

    def autocursor(self, immediate=False):
        party_node = self.overworld.selected_party_node()
        if immediate:
            self.set_pos(party_node.position)
            if immediate:
                self.camera.force_center(*self.position)
            else:
                self.camera.set_center(*self.position)

    def get_hover(self) -> Tuple[OverworldEntityObject, OverworldNodeObject]:
        """Returns a tuple containing, in index 0, any entity that the cursor
        is hovering over and, in index 1, the node the cursor is hovering over.


        Returns:
            Tuple[OverworldEntityObject, OverworldNodeObject]: (Entity under cursor, Node under cursor)
        """
        node = self.overworld.node_at(self.position)
        entity = self.overworld.entity_at(self.position)
        return (entity, node)

    def get_image(self):
        left = (ANIMATION_COUNTERS.fps6_360counter.count % 10) * TILEWIDTH * 2
        return engine.subsurface(self._sprite, (left, 0, 32, 32))
