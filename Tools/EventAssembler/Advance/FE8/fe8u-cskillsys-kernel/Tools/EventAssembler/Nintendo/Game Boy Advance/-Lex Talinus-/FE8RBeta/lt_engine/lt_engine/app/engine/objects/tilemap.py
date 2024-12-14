from app.utilities.typing import NID
from typing import List
from app.constants import TILEWIDTH, TILEHEIGHT, AUTOTILE_FRAMES, COLORKEY
from app.utilities.data import Data, Prefab

from app.data.resources.resources import RESOURCES

from app.engine import engine, image_mods, particles, animations

class LayerObject():
    transition_speed = 333

    def __init__(self, nid: str, foreground: bool, parent):
        self.nid: str = nid
        self.foreground: bool = foreground
        self.parent = parent
        self.visible = True
        self.terrain = {}
        self.image = None
        self.autotile_images = []
        self.pixel_bounds = None

        # For fade in
        self.state = None
        self.translucence = 1
        self.start_update = 0
        self.autotile_frame = 0

    def set_image(self, image):
        self.image = image

    def should_draw(self, cull_rect) -> bool:
        """
        Check to see if my bounds intersect with the
        culling rect
        """
        ans = self.pixel_bounds and cull_rect[0] < self.pixel_bounds[2] and \
            cull_rect[0] + cull_rect[2] > self.pixel_bounds[0] and \
            cull_rect[1] < self.pixel_bounds[3] and \
            cull_rect[1] + cull_rect[3] > self.pixel_bounds[1]
        return ans

    def get_image(self, cull_rect):
        # Cull to only the part I need
        im = engine.subsurface(self.image, cull_rect)
        if self.state in ('fade_in', 'fade_out'):
            im = im.convert_alpha()
            im = image_mods.make_translucent(im, self.translucence)
        return im

    def get_autotile_image(self, cull_rect):
        if not self.autotile_images:
            return None
        im = engine.subsurface(self.autotile_images[self.autotile_frame], cull_rect)
        if self.state in ('fade_in', 'fade_out'):
            im = im.convert_alpha()
            return image_mods.make_translucent(im, self.translucence)
        return im

    def quick_show(self):
        self.visible = True

    def quick_hide(self):
        self.visible = False

    def show(self):
        """
        Fades in the layer
        """
        if not self.visible:
            self.visible = True
            self.state = 'fade_in'
            self.translucence = 1
            self.start_update = engine.get_time()

    def hide(self):
        """
        Fades out the layer
        """
        if self.visible:
            self.visible = False
            self.state = 'fade_out'
            self.translucence = 0
            self.start_update = engine.get_time()

    def update(self) -> bool:
        current_time = engine.get_time()
        in_state = bool(self.state)

        if self.state == 'fade_in':
            self.translucence = 1 - (current_time - self.start_update)/self.transition_speed
            if self.translucence <= 0:
                self.state = None
        elif self.state == 'fade_out':
            self.translucence = (current_time - self.start_update)/self.transition_speed
            if self.translucence >= 1:
                self.state = None

        if self.autotile_images:
            autotile_wait = int(self.parent.autotile_fps * 16.66)
            frame = (current_time // autotile_wait) % len(self.autotile_images)
            if frame != self.autotile_frame:
                self.autotile_frame = frame
                in_state = True  # Requires update to image when autotiles turn over

        return in_state

    def save(self):
        s_dict = {}
        s_dict['nid'] = self.nid
        s_dict['visible'] = self.visible
        return s_dict

    # Restore not needed -- handled in TileMapObjects deserialize function

class TileMapObject(Prefab):
    def __init__(self):
        super().__init__()
        self.weather: List[particles.SimpleParticleSystem] = []
        self.animations: List[animations.MapAnimation] = []
        self.high_animations: List[animations.MapAnimation] = []
        self.width: int = 0
        self.height: int = 0
        self.nid: NID = None

    @classmethod
    def from_prefab(cls, prefab):
        self = cls()
        self.nid = prefab.nid
        self.width = prefab.width
        self.height = prefab.height
        self.autotile_fps = prefab.autotile_fps
        self.layers = Data()

        # Stitch together image layers
        for layer in prefab.layers:
            new_layer = LayerObject(layer.nid, layer.foreground, self)
            # Terrain
            for coord, terrain_nid in layer.terrain_grid.items():
                new_layer.terrain[coord] = terrain_nid

            # Image
            image = engine.create_surface((self.width * TILEWIDTH, self.height * TILEHEIGHT))
            engine.fill(image, COLORKEY)
            engine.set_colorkey(image, COLORKEY, rleaccel=True)
            # Autotile Images
            autotile_images = [engine.create_surface((self.width * TILEWIDTH, self.height * TILEHEIGHT)) for _ in range(AUTOTILE_FRAMES)]
            for im in autotile_images:
                engine.fill(im, COLORKEY)
                engine.set_colorkey(im, COLORKEY, rleaccel=True)

            # Build pixel bounds
            coords = layer.sprite_grid.keys()
            if coords:
                left_bound = min(coord[0] for coord in coords) * TILEWIDTH
                right_bound = (max(coord[0] for coord in coords) + 1) * TILEWIDTH
                top_bound = min(coord[1] for coord in coords) * TILEHEIGHT
                bottom_bound = (max(coord[1] for coord in coords) + 1) * TILEHEIGHT
                new_layer.pixel_bounds = [left_bound, top_bound, right_bound, bottom_bound]

            has_autotiles = False
            for coord, tile_sprite in layer.sprite_grid.items():
                tileset = RESOURCES.tilesets.get(tile_sprite.tileset_nid)
                if not tileset.image:
                    tileset.image = engine.image_load(tileset.full_path)
                if not tileset.autotile_image and tileset.autotile_full_path:
                    tileset.autotile_image = engine.image_load(tileset.autotile_full_path)
                pos = tile_sprite.tileset_position

                rect = (pos[0] * TILEWIDTH, pos[1] * TILEHEIGHT, TILEWIDTH, TILEHEIGHT)
                sub_image = engine.subsurface(tileset.image, rect)
                image.blit(sub_image, (coord[0] * TILEWIDTH, coord[1] * TILEHEIGHT))

                # Handle Autotiles
                if pos in tileset.autotiles and tileset.autotile_image:
                    has_autotiles = True
                    column = tileset.autotiles[pos]
                    for idx, im in enumerate(autotile_images):
                        rect = (column * TILEWIDTH, idx * TILEHEIGHT, TILEWIDTH, TILEHEIGHT)
                        sub_image = engine.subsurface(tileset.autotile_image, rect)
                        im.blit(sub_image, (coord[0] * TILEWIDTH, coord[1] * TILEHEIGHT))

            new_layer.image = image
            if has_autotiles:
                new_layer.autotile_images = autotile_images
            self.layers.append(new_layer)

        # Base layer should be visible, rest invisible
        for layer in self.layers:
            layer.visible = False
        self.layers.get('base').visible = True

        self.weather = []
        self.animations = []

        return self

    def check_bounds(self, pos):
        return 0 <= pos[0] < self.width and 0 <= pos[1] < self.height

    def on_border(self, pos):
        return pos[0] == 0 or pos[1] == 0 or pos[0] == self.width - 1 or pos[1] == self.height - 1

    def get_terrain(self, pos):
        for layer in reversed(self.layers):
            if layer.visible and pos in layer.terrain:
                return layer.terrain[pos]
        return '0'

    def get_layer(self, pos):
        for layer in reversed(self.layers):
            if layer.visible and pos in layer.terrain:
                return layer.nid
        return None

    def background_layers(self) -> List[LayerObject]:
        return [layer for layer in self.layers if not layer.foreground]

    def foreground_layers(self) -> List[LayerObject]:
        return [layer for layer in self.layers if layer.foreground]

    def get_full_image(self, cull_rect):
        image = engine.create_surface((cull_rect[2], cull_rect[3]))
        engine.fill(image, COLORKEY)
        engine.set_colorkey(image, COLORKEY)
        layers = self.background_layers()
        for layer in layers:
            if (layer.visible or layer.state == 'fade_out') and \
                    layer.should_draw(cull_rect):
                main_image = layer.get_image(cull_rect)
                image.blit(main_image, (0, 0))
                autotile_image = layer.get_autotile_image(cull_rect)
                if autotile_image:
                    image.blit(autotile_image, (0, 0))
        return image

    def get_foreground_image(self, cull_rect):
        image = engine.create_surface((cull_rect[2], cull_rect[3]), transparent=True)
        layers = self.foreground_layers()
        for layer in layers:
            if (layer.visible or layer.state == 'fade_out') and \
                    layer.should_draw(cull_rect):
                main_image = layer.get_image(cull_rect)
                image.blit(main_image, (0, 0))
                autotile_image = layer.get_autotile_image(cull_rect)
                if autotile_image:
                    image.blit(autotile_image, (0, 0))
        return image

    def save_screenshot(self):
        import os
        from datetime import datetime

        if not os.path.isdir('screenshots'):
            os.mkdir('screenshots')
        current_time = str(datetime.now()).replace(' ', '_').replace(':', '.')

        image = self.get_full_image((0, 0, self.width * TILEWIDTH, self.height * TILEHEIGHT))
        image.blit(self.get_foreground_image((0, 0, self.width * TILEWIDTH, self.height * TILEHEIGHT)))
        engine.save_surface(image, 'screenshots/LT_%s_tilemap.png' % current_time)

    def update(self):
        for layer in self.layers:
            in_state = layer.update()
            if in_state:
                self.reset()

    def reset(self):
        pass

    def save(self):
        s_dict = {}
        s_dict['nid'] = self.nid
        s_dict['layers'] = [layer.save() for layer in self.layers]
        s_dict['weather'] = [weather.save() for weather in self.weather]
        s_dict['animations'] = [anim.save() for anim in self.animations]
        return s_dict

    @classmethod
    def restore(cls, s_dict):
        prefab = RESOURCES.tilemaps.get(s_dict['nid'])
        self = cls.from_prefab(prefab)
        self.restore_layers(s_dict['layers'])
        weather = s_dict.get('weather', [])
        for save_w in weather:
            if isinstance(save_w, tuple):
                nid, pos = save_w
                self.weather.append(particles.create_system(nid, self.width, self.height, pos))
            else:
                self.weather.append(particles.create_system(save_w, self.width, self.height))
        # Handle tile animations
        anims = s_dict.get('animations', [])
        self.animations = []
        for anim in anims:
            new_anim = animations.MapAnimation(
                RESOURCES.animations.get(anim['nid']),
                anim['pos'],
                loop=anim['loop'],
                hold=anim['hold'],
                reverse=anim['reverse'],
                speed_adj=anim['speed_adj'],
                contingent=anim.get('contingent', False))
            # @todo(mag): remove this eventually, this is just a stopgap to prevent crashes on load
            try:
                new_anim.set_tint(engine.BlendMode(anim.get('tint', 0)))
            except:
                pass
            self.animations.append(new_anim)

        return self

    def restore_layers(self, layer_list):
        for layer_dict in layer_list:
            nid = layer_dict['nid']
            visible = layer_dict['visible']
            self.layers.get(nid).visible = visible
