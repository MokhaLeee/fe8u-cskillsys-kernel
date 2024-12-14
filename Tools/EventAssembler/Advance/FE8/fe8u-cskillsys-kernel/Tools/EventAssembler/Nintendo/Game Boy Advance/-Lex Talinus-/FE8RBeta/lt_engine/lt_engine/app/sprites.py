from app.constants import WINHEIGHT, WINWIDTH
import os
from dataclasses import dataclass
from app.engine import engine

@dataclass
class BasicSprite(object):
    full_path: str = None
    pixmap: str = None
    image: engine.Surface = None

@dataclass
class SpecialSprite(object):
    # pregenerated sprites, e.g. black screen/white screen
    full_path: str = None
    pixmap: str = None
    _image: engine.Surface = None

    # because these are pregenerated, they have to be converted only when used, with pygame initialized
    converted: bool = False

    @property
    def image(self):
        if not self.converted:
            self._image = self._image.convert_alpha()
            self.converted = True
        return self._image

class SpriteDict(dict):
    def get(self, val, fallback='bg_black_tile'):
        '''Retrieves an image in the form of a pygame surface.
        Fallback provides a backup image if the specified one
        cannot be found'''
        if val in self:
            return self[val].image
        # Defaults to this
        return self[fallback].image

def load_sprites(root):
    for root, dirs, files in os.walk(root):
        for name in files:
            if name.endswith('.png'):
                full_name = os.path.join(root, name)
                SPRITES[name[:-4]] = BasicSprite(full_name)

def load_special_sprites():
    black_bg = engine.create_simple_surface((WINWIDTH, WINHEIGHT))
    black_bg_sprite = SpecialSprite(_image=black_bg)
    SPRITES['bg_black'] = black_bg_sprite

SPRITES = SpriteDict()
load_sprites('sprites/')
load_special_sprites()

def reset():
    SPRITES.clear()
    load_sprites('sprites/')
    load_special_sprites()
