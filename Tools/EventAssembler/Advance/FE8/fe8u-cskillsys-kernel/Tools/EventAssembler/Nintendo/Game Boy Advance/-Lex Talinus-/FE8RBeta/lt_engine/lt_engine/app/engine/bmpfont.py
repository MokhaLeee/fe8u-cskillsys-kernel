from dataclasses import dataclass
from typing import Dict, Optional, Tuple
from functools import lru_cache
from app.data.resources.fonts import Font

from app.engine import engine
import pygame

from app.utilities.typing import Color4

OUTLINE_WIDTH = 1

@lru_cache()
def default_render_with_outline(font: pygame.font.Font, char: str, height: int, internal_color: Color4, outline_color: Color4) -> Tuple[engine.Surface, int]:
    text_width, _ = font.size(char)
    surf = engine.create_surface((text_width + 2 * OUTLINE_WIDTH, height), True)
    internal = font.render(char, False, internal_color)
    outline = font.render(char, False, outline_color)
    for i in -OUTLINE_WIDTH, OUTLINE_WIDTH:
        surf.blit(outline, (i + OUTLINE_WIDTH, OUTLINE_WIDTH))
        surf.blit(outline, (OUTLINE_WIDTH, i + OUTLINE_WIDTH))
    surf.blit(internal, (OUTLINE_WIDTH, OUTLINE_WIDTH))
    return surf, text_width

@lru_cache()
def default_render(font: pygame.font.Font, char: str, height: int, color: Color4) -> Tuple[engine.Surface, int]:
    text_width, _ = font.size(char)
    surf = engine.create_surface((text_width, height), True)
    black = font.render(char, False, color)
    surf.blit(black, (0, 0))
    return surf, text_width

@dataclass
class CharGlyph():
    """Class representing a char position and dimension on the sheet"""
    x: int
    y: int
    char_width: int

class BmpFont():
    def __init__(self, font_info: Font, png_path: str, idx_path: str):
        self.all_uppercase = False
        self.all_lowercase = False
        self.stacked = False
        self.chartable: Dict[str, CharGlyph] = {}
        self.idx_path = idx_path
        self.png_path = png_path
        self.space_offset = 0
        self._width = 8
        self.height = 16
        self.memory: Dict[str, Dict[str, Tuple[engine.Surface, int]]] = {}

        self.font_info = font_info
        self.fallback_font = None
        if self.font_info.ttf_path():
            self.fallback_font = pygame.font.Font(self.font_info.ttf_path(), font_info.fallback_size)

        with open(self.idx_path, 'r', encoding='utf-8') as fp:
            for x in fp.readlines():
                words = x.strip().split()
                if words[0] == 'alluppercase':
                    self.all_uppercase = True
                elif words[0] == 'alllowercase':
                    self.all_lowercase = True
                elif words[0] == 'stacked':
                    self.stacked = True
                elif words[0] == 'space_offset':
                    self.space_offset = int(words[1])
                elif words[0] == "width":
                    self._width = int(words[1])
                elif words[0] == "height":
                    self.height = int(words[1])
                else:  # Default to index entry.
                    if words[0] == "space":
                        words[0] = ' '
                    if self.all_uppercase:
                        words[0] = words[0].upper()
                    if self.all_lowercase:
                        words[0] = words[0].lower()
                    self.chartable[words[0]] = CharGlyph(int(words[1]) * self._width,
                                                         int(words[2]) * self.height,
                                                         int(words[3]))

        self.default_color = font_info.default_color or 'default'
        self.surfaces: Dict[str, engine.Surface] = {}
        self.surfaces[font_info.default_color or 'default'] = engine.image_load(self.png_path)
        # engine.set_colorkey(self.surface, (0, 0, 0), rleaccel=True)

    def get_base_surf(self) -> engine.Surface:
        return self.surfaces[self.default_color]

    def modify_string(self, string: str) -> str:
        if self.all_uppercase:
            string = string.upper()
        if self.all_lowercase:
            string = string.lower()
        # string = string.replace('_', ' ')
        return string

    @lru_cache()
    def _get_char_width(self, c: str) -> int:
        if c in self.chartable:
            return self.chartable[c].char_width
        if self.fallback_font:
            if self.font_info.outline_font:
                return default_render_with_outline(self.fallback_font, c, self.height, (0, 0, 0), (0, 0, 0))[1]
            else:
                return default_render(self.fallback_font, c, self.height, (0, 0, 0))[1]
        return 8

    @lru_cache()
    def _get_char_from_surf(self, c: str, color: str = None) -> Tuple[engine.Surface, int]:
        if not color:
            color = self.default_color
        if c not in self.chartable:
            if self.fallback_font:
                if self.font_info.outline_font:
                    return default_render_with_outline(self.fallback_font, c, self.height, tuple(self.font_info.primary_color(color)), tuple(self.font_info.secondary_color(color)))
                else:
                    return default_render(self.fallback_font, c, self.height, tuple(self.font_info.primary_color(color)))
            else:
                cx, cy, cwidth = 0, 0, 8
                print("unknown char: %s" % c)
        else:
            c_info = self.chartable[c]
            cx, cy, cwidth = c_info.x, c_info.y, c_info.char_width
        base_surf = self.surfaces.get(color, self.surfaces[self.default_color])
        char_surf = engine.subsurface(base_surf, (cx, cy, self._width, self.height))
        return (char_surf, cwidth)

    @lru_cache()
    def _get_stacked_char_from_surf(self, c: str, color: str = None) -> Tuple[engine.Surface, engine.Surface, int]:
        if not color:
            color = self.default_color
        if c not in self.chartable:
            cx, cy, cwidth = 0, 0, 8
            print("unknown char: %s" % c)
        else:
            c_info = self.chartable[c]
            cx, cy, cwidth = c_info.x, c_info.y, c_info.char_width
        base_surf = self.surfaces.get(color)
        high_surf = engine.subsurface(base_surf, (cx, cy, self._width, self.height))
        lowsurf = engine.subsurface(base_surf, (cx, cy + self.height, self._width, self.height))
        return (high_surf, lowsurf, cwidth)

    def blit(self, string, surf, pos=(0, 0), color: Optional[str] = None, no_process=False):
        if not color:
            color = self.default_color
        def normal_render(left, top, string: str, bcolor):
            for c in string:
                c_surf, char_width = self._get_char_from_surf(c, bcolor)
                engine.blit(surf, c_surf, (left, top))
                left += char_width + self.space_offset

        def stacked_render(left, top, string: str, bcolor):
            for c in string:
                highsurf, lowsurf, char_width = self._get_stacked_char_from_surf(c, bcolor)
                engine.blit(surf, lowsurf, (left, top))
                engine.blit(surf, highsurf, (left, top))
                left += char_width + self.space_offset

        x, y = pos

        string = self.modify_string(string)

        if self.stacked:
            stacked_render(x, y, string, color)
        else:
            normal_render(x, y, string, color)

    def blit_right(self, string, surf, pos, color=None):
        width = self.width(string)
        self.blit(string, surf, (pos[0] - width, pos[1]), color)

    def blit_center(self, string, surf, pos, color=None):
        width = self.width(string)
        self.blit(string, surf, (pos[0] - width//2, pos[1]), color)

    def size(self, string):
        """
        Returns the length and width of a bitmapped string
        """
        return (self.width(string), self.height)

    def width(self, string):
        """
        Returns the width of a bitmapped string
        """
        length = 0
        string = self.modify_string(string)
        for c in string:
            length += self._get_char_width(c)
        return length
