from app.engine.sprites import SPRITES
from app.engine import engine

import logging

# For bg surfs that don't have `_bg` in their name
HARDCODED_BG_SURFS = [
    'name_tag'
]

def create_base_surf(width, height, base='menu_bg_base') -> engine.Surface:
    sprite = SPRITES.get(base, 'menu_bg_base')
    if base and '_bg' not in base and base not in HARDCODED_BG_SURFS:
        new_base_surf = engine.create_surface((width, height), transparent=True)
        new_base_surf.blit(sprite, (0, 0))
        return new_base_surf

    base_width = sprite.get_width()
    base_height = sprite.get_height()

    full_width = width - width%(base_width//3)
    full_height = height - height%(base_height//3)
    width = base_width//3
    height = base_height//3

    assert full_width%width == 0, "The dimensions of the menu are wrong (width: %d %d)" % (full_width, width)
    assert full_height%height == 0, "The dimensions of the menu are wrong (height: %d %d)" % (full_height, height)

    surf = engine.create_surface((full_width, full_height), transparent=True)

    topleft = engine.subsurface(sprite, (0, 0, width, height))
    top = engine.subsurface(sprite, (width, 0, width, height))
    topright = engine.subsurface(sprite, (2 * width, 0, width, height))
    midleft = engine.subsurface(sprite, (0, height, width, height))
    mid = engine.subsurface(sprite, (width, height, width, height))
    midright = engine.subsurface(sprite, (2 * width, height, width, height))
    botleft = engine.subsurface(sprite, (0, 2 * height, width, height))
    bot = engine.subsurface(sprite, (width, 2 * height, width, height))
    botright = engine.subsurface(sprite, (2 * width, 2 * height, width, height))

    # center sprite
    for x in range(full_width//width - 2):
        for y in range(full_height//height - 2):
            surf.blit(mid, ((x + 1) * width, (y + 1) * height))

    # edges
    for x in range(full_width//width - 2):
        surf.blit(top, ((x + 1) * width, 0))
        surf.blit(bot, ((x + 1) * width, full_height - height))

    for y in range(full_height//height - 2):
        surf.blit(midleft, (0, (y + 1) * height))
        surf.blit(midright, (full_width - width, (y + 1) * height))

    # corners
    surf.blit(topleft, (0, 0))
    surf.blit(topright, (full_width - width, 0))
    surf.blit(botleft, (0, full_height - height))
    surf.blit(botright, (full_width - width, full_height - height))

    return surf

def create_highlight_surf(width) -> engine.Surface:
    if width < 5:
        raise ValueError("Highlight surf too short - why are you even calling this?")
    sprite: engine.Surface = SPRITES.get('equipment_highlight')

    base_width = sprite.get_width()
    base_height = sprite.get_height()

    left_endcap = engine.subsurface(sprite, (0, 0, 2, base_height))
    middle_segment = engine.subsurface(sprite, (3, 0, 1, base_height))
    right_endcap = engine.subsurface(sprite, (base_width - 2, 0, 2, base_height))

    surf = engine.create_surface((width, base_height), transparent=True)
    surf.blit(left_endcap, (0, 0))
    for middle_x in range(2, width - 2):
        surf.blit(middle_segment, (middle_x, 0))
    surf.blit(right_endcap, (width - 2, 0))

    return surf
