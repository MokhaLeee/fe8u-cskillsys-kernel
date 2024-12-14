from app.sprites import SPRITES

from app.engine import engine

def load_images():
    for sprite in SPRITES.values():
        if sprite.full_path:
            sprite.image = engine.image_load(sprite.full_path)

load_images()