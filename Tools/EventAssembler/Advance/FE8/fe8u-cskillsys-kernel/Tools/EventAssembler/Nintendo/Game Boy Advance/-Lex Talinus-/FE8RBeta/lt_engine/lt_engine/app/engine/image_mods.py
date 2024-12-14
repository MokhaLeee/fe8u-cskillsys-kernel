from app.constants import COLORKEY
from app.utilities import utils
from app.engine import engine

def color_convert(image, conversion_dict):
    image = image.convert()
    px_array = engine.make_pixel_array(image)
    for old_color, new_color in conversion_dict.items():
        px_array.replace(old_color, new_color)
    px_array.close()
    return image

def color_convert_alpha(image, conversion_dict):
    px_array = engine.make_pixel_array(image)
    for old_color, new_color in conversion_dict.items():
        px_array.replace(old_color, new_color)
    px_array.close()
    return image

def invert_surface(image):
    # Using px_array is about 2x as fast as native
    # Using Cython invert is about >200x faster than native
    px_array = engine.make_pixel_array(image)
    colorkey = image.map_rgb(COLORKEY)
    for x in range(image.get_width()):
        for y in range(image.get_height()):
            if px_array[x, y] != colorkey:
                color = image.unmap_rgb(px_array[x, y])
                px_array[x, y] = (255 - color[0], 255 - color[1], 255 - color[2])
    px_array.close()

def make_gray(image):
    for row in range(image.get_width()):
        for col in range(image.get_height()):
            color = image.get_at((row, col))
            if color[3] != 0:
                avg = int(color[0] * 0.298 + color[1] * 0.587 + color[2] * 0.114)
                image.set_at((row, col), (avg, avg, avg, color[3]))
    return image


def make_gray_colorkey(image):
    for row in range(image.get_width()):
        for col in range(image.get_height()):
            color = image.get_at((row, col))
            if color[3] != 0 and color[:3] != COLORKEY:
                avg = int(color[0] * 0.298 + color[1] * 0.587 + color[2] * 0.114)
                image.set_at((row, col), (avg, avg, avg, color[3]))
    return image

def make_anim_gray(image):
    # Different because animations have a small box of green around them
    for row in range(image.get_width()):
        for col in range(image.get_height()):
            color = image.get_at((row, col))
            if color[3] != 0 and (color[0], color[1], color[2]) != (128, 160, 128):
                avg = int(color[0] * 0.298 + color[1] * 0.587 + color[2] * 0.114)
                image.set_at((row, col), (avg, avg, avg, color[3]))
    return image

def make_translucent(image, t):
    """
    transparency measured from 0.0 to 1.0, where 0.0 is fully opaque
    and 1.0 is fully transparent
    """

    alpha = 255 - int(255 * t)
    alpha = utils.clamp(alpha, 0, 255)
    image = engine.copy_surface(image)
    engine.fill(image, (255, 255, 255, alpha), None, engine.BLEND_RGBA_MULT)

    return image

def make_translucent_blend(image, alpha):
    """
    alpha measured from 0 to 255, where 0 is fully opaque
    and 255 is fully transparent
    """
    alpha = 255 - utils.clamp(alpha, 0, 255)
    image = engine.copy_surface(image)
    engine.fill(image, (alpha, alpha, alpha), None, engine.BLEND_RGB_MULT)

    return image

# Making a blended color translucent is the same as making it black
def make_black_colorkey(image, black):
    """
    blackness measured from 0.0 to 1.0, where 0.0 is no change to color
    """
    black = 1 - black
    black = int(255 * black)
    black = utils.clamp(black, 0, 255)
    temp = engine.create_surface((image.get_width(), image.get_height()), transparent=True)
    temp.blit(image, (0, 0))
    engine.fill(temp, (black, black, black), None, engine.BLEND_RGB_MULT)

    return temp

def make_white(image, white):
    """
    whiteness measured from 0.0 to 1.0, where 0.0 is no change to color
    """
    white = int(255 * white)
    white = utils.clamp(white, 0, 255)
    image = engine.copy_surface(image)
    engine.fill(image, (white, white, white), None, engine.BLEND_RGB_ADD)

    return image

def change_color(image, color: tuple):
    """
    Additively blends a color with the image
    """
    image = engine.copy_surface(image)
    for idx, band in enumerate(color):
        blend_mode = engine.BLEND_RGB_ADD
        if band < 0:
            blend_mode = engine.BLEND_RGB_SUB
            band = -band
        if idx == 0:
            new_color = (band, 0, 0)
        elif idx == 1:
            new_color = (0, band, 0)
        else:
            new_color = (0, 0, band)
        engine.fill(image, new_color, None, blend_mode)
    return image

def change_color_alpha(image, color):
    new_image = change_color(image, color)
    engine.fill(new_image, (255, 255, 255, color[3]), None, engine.BLEND_RGBA_MULT)
    image.blit(new_image, (0, 0))

    return image

def true_tint(image, color):
    """
    Assumes image has per-pixel alpha and that color is len == 4
    If color[3] == 255, actually converts image to have the exact color
    specified, no lighter or darker
    """
    if len(color) == 3:
        color = (*color, 255)
    new_image = image.copy()
    engine.fill(new_image, (0, 0, 0, 255), None, engine.BLEND_RGBA_MULT)
    engine.fill(new_image, (color[0], color[1], color[2], 0), None, engine.BLEND_RGBA_ADD)
    transparency = 1 - utils.clamp(color[3] / 255., 0, 1)
    new_image = make_translucent(new_image, transparency)
    image.blit(new_image, (0, 0))
    return image

def add_tint(image, color):
    """
    Assumes image has per-pixel alpha and that color is len == 3
    """
    image = image.copy()
    engine.fill(image, (color[0], color[1], color[2], 0), None, engine.BLEND_RGBA_ADD)
    return image

def sub_tint(image, color):
    """
    Assumes image has per-pixel alpha and that color is len == 3
    """
    image = image.copy()
    engine.fill(image, (color[0], color[1], color[2], 0), None, engine.BLEND_RGBA_SUB)
    return image

def blend_colors(color1, color2, t):
    """
    gets a color between color1 and color2 depending on t
    t is between 0 and 1
    """
    t = utils.clamp(t, 0, 1)
    color_diff = (color2[0] - color1[0], color2[1] - color1[1], color2[2] - color1[2])
    new_color = []
    for idx, chroma in enumerate(color_diff):
        new_color.append(int(chroma * t) + color1[idx])

    return new_color

def screen_dodge(image, color):
    image = engine.copy_surface(image)
    # Invert image
    inv = engine.copy_surface(image)
    engine.fill(inv, (255, 255, 255))
    inv.blit(image, (0, 0), None, engine.BLEND_RGBA_SUB)
    # Multiply with new color
    inv_color = tuple([255 - c for c in color])
    engine.fill(inv, inv_color, None, engine.BLEND_RGBA_MULT)
    # Invert image again
    new_inv = engine.copy_surface(inv)
    engine.fill(new_inv, (255, 255, 255))
    new_inv.blit(inv, (0, 0), None, engine.BLEND_RGBA_SUB)
    return new_inv

def resize(image, scale):
    x_scale, y_scale = scale
    new_scale = int(image.get_width() * x_scale), int(image.get_height() * y_scale)
    return engine.transform_scale(image, new_scale)
