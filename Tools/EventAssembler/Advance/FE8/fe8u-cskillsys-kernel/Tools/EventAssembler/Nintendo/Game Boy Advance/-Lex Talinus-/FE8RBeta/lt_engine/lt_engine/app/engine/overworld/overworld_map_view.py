from app.engine.objects.overworld import OverworldNodeProperty
from app.engine.overworld.overworld_manager import OverworldManager
import logging

from app.constants import TILEHEIGHT, TILEWIDTH, WINHEIGHT, WINWIDTH
from app.engine import engine
from app.engine.graphics.ingame_ui.overworld_ui import OverworldTravelUI
from app.engine.overworld.overworld_cursor import OverworldCursor

class OverworldMapView():
    def __init__(self, overworld: OverworldManager = None, cursor: OverworldCursor = None):
        self.overworld = overworld
        self.cursor = cursor
        self.overworld_ui = OverworldTravelUI(overworld, cursor)

    def draw_nodes(self, surf, cull_rect):
        for node in self.overworld.revealed_nodes:
            if not node.sprite:
                return
            node.sprite.update()
            if node.prefab.pos:
                if OverworldNodeProperty.IS_NEXT_LEVEL in self.overworld.node_props(node.nid):
                    surf = node.sprite.draw(surf, cull_rect, True)
                else:
                    surf = node.sprite.draw(surf, cull_rect)

    def draw_roads(self, surf, full_size, cull_rect):
        for road in self.overworld.revealed_roads:
            road.sprite.update()
            surf = road.sprite.draw(surf, full_size, cull_rect)

    def draw_entities(self, surf, cull_rect):
        overworld_entities = self.overworld.entities.values()
        # get rid of the unplaced entities
        overworld_entities = [entity for entity in overworld_entities if entity.sprite and entity.display_position is not None]
        # Only draw units within 2 tiles of cull_rect
        culled_entities = \
            [entity for entity in overworld_entities if entity.sprite.draw_anyway() or
             (cull_rect[0] - TILEWIDTH*2 < entity.display_position[0] * TILEWIDTH < cull_rect[0] + cull_rect[2] + TILEWIDTH*2 and
              cull_rect[1] - TILEHEIGHT*2 < entity.display_position[1] * TILEHEIGHT < cull_rect[1] + cull_rect[3] + TILEHEIGHT*2)]
        for entity in culled_entities:
            # special behavior for the hovered entity
            if self.cursor:
                sel_entity = self.cursor.get_hover()[0]
                if sel_entity and entity.nid == sel_entity.nid:
                    entity.sprite.hovered = True
                else:
                    entity.sprite.hovered = False
            else:
                entity.sprite.hovered = False

            entity.sprite.update()
            entity.sound.update()
            if entity.display_position:
                surf = entity.sprite.draw(surf, cull_rect)

    def draw(self, camera_cull=None, subsurface_cull=None):
        if not self.overworld:
            logging.error('%s: %s : no overworld to draw?', type(self).__name__, 'draw()')
            return engine.create_surface((WINWIDTH, WINHEIGHT))
        self.overworld.tilemap.update()
        cull_rect = camera_cull
        full_size = self.overworld.tilemap.width * TILEWIDTH, self.overworld.tilemap.height * TILEHEIGHT

        # cut out our base
        map_image = self.overworld.tilemap.get_full_image(cull_rect)
        surf = engine.copy_surface(map_image)
        surf = surf.convert_alpha()

        # draw the map objects: roads, nodes, and entities
        self.draw_roads(surf, full_size, cull_rect)
        self.draw_nodes(surf, cull_rect)
        self.overworld.tilemap.animations = [anim for anim in self.overworld.tilemap.animations if not anim.update()]
        for anim in self.overworld.tilemap.animations:
            anim.draw(surf, offset=(-self.overworld.camera.get_x(), -self.overworld.camera.get_y()))

        self.draw_entities(surf, cull_rect)

        if self.cursor:
            surf = self.cursor.draw(surf, cull_rect)

        # weather overlay
        for weather in self.overworld.tilemap.weather:
            weather.update()
            weather.draw(surf, cull_rect[0], cull_rect[1])

        surf = self.overworld_ui.draw(surf)

        return surf
