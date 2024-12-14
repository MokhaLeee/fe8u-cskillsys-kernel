from typing import Dict
from app.data.resources.resources import RESOURCES
from app.engine import bmpfont, image_mods
from app.utilities.typing import NID

NORMAL_FONT_COLORS = ['white', 'blue', 'green', 'red', 'orange', 'grey', 'yellow', 'brown', 'purple']

# Load in default, uncolored fonts
FONT: Dict[NID, bmpfont.BmpFont] = {}
NORMAL_FONT_COLORS = set()
def load_fonts():
    global FONT
    global NORMAL_FONT_COLORS
    FONT.clear()
    for font in RESOURCES.fonts.values():
        bmp_font = bmpfont.BmpFont(font, font.image_path(), font.index_path())
        FONT[font.nid] = bmp_font
        base_surf = bmp_font.get_base_surf()
        for color_name, palette in font.palettes.items():
            if color_name == font.default_color:
                paletted_surf = base_surf
            else:
                palette_map = {orig: new for orig, new in zip(map(tuple, font.palettes[font.default_color]), map(tuple, palette))}
                paletted_surf = image_mods.color_convert_alpha(base_surf.copy(), palette_map)
            bmp_font.surfaces[color_name] = paletted_surf

            # for ease of access
            font_name_with_color = '%s-%s' % (font.nid, color_name)
            FONT[font_name_with_color] = bmpfont.BmpFont(font, font.image_path(), font.index_path())
            FONT[font_name_with_color].default_color = color_name
            FONT[font_name_with_color].surfaces[color_name] = paletted_surf
    NORMAL_FONT_COLORS = RESOURCES.fonts.get("text").palettes.keys()
