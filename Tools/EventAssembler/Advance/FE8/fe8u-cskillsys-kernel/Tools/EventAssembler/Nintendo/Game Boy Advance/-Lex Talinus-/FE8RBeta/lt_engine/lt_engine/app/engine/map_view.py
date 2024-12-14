import math

from app.constants import TILEWIDTH, TILEHEIGHT, TILEX, TILEY, WINWIDTH, WINHEIGHT
from app.events.regions import RegionType

from app.engine import config as cf
from app.engine import engine
from app.engine.fonts import FONT
from app.engine.game_state import game

from app.utilities.utils import magnitude, tmult, tuple_add, tuple_sub

class MapView():
    def __init__(self):
        self._unit_surf = engine.create_surface((WINWIDTH, WINHEIGHT), transparent=True)
        self._line_surf = engine.copy_surface(self._unit_surf)
        self._line_surf.fill((0, 0, 0, 0))

    def save_screenshot(self):
        import os
        from datetime import datetime

        if not os.path.isdir('screenshots'):
            os.mkdir('screenshots')
        current_time = str(datetime.now()).replace(' ', '_').replace(':', '.')

        cull_rect = (0, 0, game.tilemap.width * TILEWIDTH, game.tilemap.height * TILEWIDTH)
        surf = engine.create_surface(cull_rect[2:])
        map_image = game.tilemap.get_full_image(cull_rect)
        surf.blit(map_image, (0, 0))
        surf = surf.convert_alpha()

        # Draw units
        draw_units = [unit for unit in game.units if unit.sprite.position]
        draw_units = sorted(draw_units, key=lambda unit: unit.sprite.position[1])
        for unit in draw_units:
            unit.sprite.draw(surf, (0, 0))

        if game.tilemap.foreground_layers():
            foreground_image = game.tilemap.get_foreground_image(cull_rect)
            surf.blit(foreground_image, (0, 0))

        engine.save_surface(surf, 'screenshots/LT_%s_map_view.png' % current_time)

    def draw_units(self, surf, cull_rect, subsurface_rect=None):
        unit_surf = engine.copy_surface(self._unit_surf)
        cull_rect_in_tiles = cull_rect[0] / TILEWIDTH, cull_rect[1] / TILEHEIGHT, cull_rect[2] / TILEWIDTH, cull_rect[3] / TILEHEIGHT
        cull_rect_center_in_tiles = tuple_add(cull_rect_in_tiles[:2], tmult(cull_rect_in_tiles[2:], 0.5))

        # Update all units
        update_units = [unit for unit in game.units if unit.sprite.position]
        for unit in update_units:
            if game.is_roam():
                if unit.position:
                    norm_dist_from_center = max(1.0 - magnitude(tuple_sub(unit.position, cull_rect_center_in_tiles)) / ((TILEX + TILEY) / 2), 0)
                else:
                    norm_dist_from_center = 0.0
            else:
                norm_dist_from_center = 1.0
            unit.sprite.update()
            unit.sound.update(volume=norm_dist_from_center)

        pos_units = [unit for unit in update_units if unit is not game.cursor.cur_unit and unit.sprite.position]
        # Only draw units within 2 tiles of cull_rect
        culled_units = [unit for unit in pos_units if unit.sprite.draw_anyway() or
                        (cull_rect[0] - TILEWIDTH*2 < unit.sprite.position[0] * TILEWIDTH < cull_rect[0] + cull_rect[2] + TILEWIDTH*2 and
                         cull_rect[1] - TILEHEIGHT*2 < unit.sprite.position[1] * TILEHEIGHT < cull_rect[1] + cull_rect[3] + TILEHEIGHT*2)]
        if game.level_vars.get('_fog_of_war') or game.board.fog_region_set:
            culled_units = [unit for unit in culled_units if game.board.in_vision(unit.sprite.get_round_fake_pos() or unit.position)]
        draw_units = sorted(culled_units, key=lambda unit: unit.sprite.position[1])

        topleft = cull_rect[0], cull_rect[1]

        event = 'event' in game.state.state_names()
        for unit in draw_units:
            unit.sprite.draw(unit_surf, topleft)
            unit.sprite.draw_hp(unit_surf, topleft, event)
        for unit in draw_units:
            unit.sprite.draw_markers(unit_surf, topleft)

        # Draw the movement arrows
        game.cursor.draw_arrows(unit_surf, topleft)

        # Draw the main unit
        cur_unit = game.cursor.cur_unit
        if cur_unit and cur_unit.sprite.position:
            cur_unit.sprite.draw(unit_surf, topleft)
            cur_unit.sprite.draw_hp(unit_surf, topleft, event)
            if not event:
                cur_unit.sprite.draw_markers(unit_surf, topleft)

        if subsurface_rect:
            left, top = (subsurface_rect[0] - cull_rect[0], subsurface_rect[1] - cull_rect[1])
            unit_surf = engine.subsurface(unit_surf, (left, top, subsurface_rect[2], subsurface_rect[3]))
            surf.blit(unit_surf, (left, top))
        else:
            surf.blit(unit_surf, (0, 0))

    def draw(self, camera_cull=None, subsurface_cull=None):
        game.tilemap.update()
        # Camera Cull
        cull_rect = camera_cull
        shake = game.camera.get_shake()
        cull_rect = (cull_rect[0] + shake[0], cull_rect[1] + shake[1], cull_rect[2], cull_rect[3])

        full_size = game.tilemap.width * TILEWIDTH, game.tilemap.height * TILEHEIGHT

        if game.bg_tilemap:
            # cull calculations
            bg_size = game.bg_tilemap.width * TILEWIDTH, game.bg_tilemap.height * TILEHEIGHT
            x, y = cull_rect[:2]
            if x:
                x_proportion = float(x) / (full_size[0] - WINWIDTH)
                bg_x = x_proportion * (bg_size[0] - WINWIDTH)
            else:
                bg_x = 0
            if y:
                y_proportion = float(y) / (full_size[1] - WINHEIGHT)
                bg_y = y_proportion * (bg_size[1] - WINHEIGHT)
            else:
                bg_y = 0

            parallax_cull = (bg_x, bg_y, cull_rect[2], cull_rect[3])
            base_image = game.bg_tilemap.get_full_image(parallax_cull)
            map_image = game.tilemap.get_full_image(cull_rect)
            surf = engine.copy_surface(base_image)
            surf = surf.convert_alpha()
            surf.blit(map_image, shake)
        else:
            surf = engine.create_surface(cull_rect[2:])
            map_image = game.tilemap.get_full_image(cull_rect)
            surf.blit(map_image, shake)
            surf = surf.convert_alpha()

        surf = game.boundary.draw_auras(surf, full_size, cull_rect)
        surf = game.boundary.draw(surf, full_size, cull_rect)
        surf = game.boundary.draw_fog_of_war(surf, full_size, cull_rect)
        surf = game.highlight.draw(surf, cull_rect)

        self.draw_grid(surf, cull_rect)

        game.tilemap.animations = [anim for anim in game.tilemap.animations if not anim.update()]
        for anim in game.tilemap.animations:
            anim.draw(surf, offset=(-game.camera.get_x(), -game.camera.get_y()))

        if subsurface_cull:  # Forced smaller cull rect from animation combat black background
            # Make sure it has a width
            # Make the cull rect even smaller
            if subsurface_cull[2] > 0:
                subsurface_rect = cull_rect[0] + subsurface_cull[0], cull_rect[1] + subsurface_cull[1], subsurface_cull[2], subsurface_cull[3]
                self.draw_units(surf, cull_rect, subsurface_rect)
            else:
                pass # Don't draw units
        else:
            self.draw_units(surf, cull_rect)

        game.tilemap.high_animations = [anim for anim in game.tilemap.high_animations if not anim.update()]
        for anim in game.tilemap.high_animations:
            anim.draw(surf, offset=(-game.camera.get_x(), -game.camera.get_y()))

        if game.tilemap.foreground_layers():
            foreground_image = game.tilemap.get_foreground_image(cull_rect)
            surf.blit(foreground_image, (0, 0))

        # Handle time region text
        self.time_region_text(surf, cull_rect)

        surf = game.cursor.draw(surf, cull_rect)

        for weather in game.tilemap.weather:
            weather.update()
            weather.draw(surf, cull_rect[0], cull_rect[1])

        surf = game.ui_view.draw(surf)
        return surf

    def time_region_text(self, surf, cull_rect):
        font = FONT['text-yellow']
        current_time = engine.get_time()
        for region in game.level.regions:
            if region.time_left is not None and region.position:
                text = str(region.time_left)
                w = font.width(text)
                pos = (region.center[0] * TILEWIDTH - cull_rect[0], region.center[1] * TILEHEIGHT - cull_rect[1])
                pos = (pos[0] + TILEWIDTH//2 - w//2, pos[1] - TILEHEIGHT//2 - 1 + 2 * math.sin(current_time//500))
                font.blit(text, surf, pos)

    def draw_grid(self, surf, cull_rect):
        # Draw board grid
        line_surf = engine.copy_surface(self._line_surf)

        bounds = game.board.bounds
        
        regular_bounds = \
            bounds[0] == 0 and \
            bounds[1] == 0 and \
            bounds[2] == game.tilemap.width - 1 and \
            bounds[3] == game.tilemap.height - 1

        left = bounds[0] * TILEWIDTH - cull_rect[0]
        right = (bounds[2] + 1) * TILEWIDTH - cull_rect[0]
        top = bounds[1] * TILEHEIGHT - cull_rect[1]
        bottom = (bounds[3] + 1) * TILEHEIGHT - cull_rect[1]

        opacity = cf.SETTINGS['grid_opacity']  # Higher numbers show more grid
        outside_opacity = min(255, opacity + 56)
        # Draw small lines (grid)
        if opacity > 30:
            # Draw vertical lines
            for x in range(left, right, TILEWIDTH):
                engine.draw_line(line_surf, (0, 0, 0, opacity), (x - 1, top), (x - 1, bottom))
            # Draw horizontal lines
            for y in range(top, bottom, TILEHEIGHT):
                engine.draw_line(line_surf, (0, 0, 0, opacity), (left, y), (right, y))

        # Draw big lines (bounds)
        # Don't bother showing bounds if they are just normal bounds
        if not regular_bounds and cf.SETTINGS['show_bounds']:
            engine.draw_line(line_surf, (0, 0, 0, outside_opacity), (left - 2, top - 1), (right + 1, top - 1), width=3)
            engine.draw_line(line_surf, (0, 0, 0, outside_opacity), (left - 1, top - 1), (left - 1, bottom), width=3)
            engine.draw_line(line_surf, (0, 0, 0, outside_opacity), (right, top - 1), (right, bottom), width=3)
            engine.draw_line(line_surf, (0, 0, 0, outside_opacity), (left - 2, bottom), (right + 1, bottom), width=3)
        # Draw to map
        surf.blit(line_surf, (0, 0))

        return surf
