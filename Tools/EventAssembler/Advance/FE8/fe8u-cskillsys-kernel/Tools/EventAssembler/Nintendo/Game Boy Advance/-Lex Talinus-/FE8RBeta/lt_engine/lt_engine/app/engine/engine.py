import sys
from typing import Tuple
from enum import Enum

import pygame
import pygame.image
import pygame.time

from app.constants import WINWIDTH, WINHEIGHT, FPS
from app.engine import config as cf

import logging

constants = {'current_time': 0,
             'last_time': 0,
             'delta_t': 0,
             'standalone': True,
             'running': True}

fast_quit = False

# === engine functions ===
def init():
    pygame.mixer.pre_init(44100, -16, 2, 128 * 2**cf.SETTINGS['sound_buffer_size'])
    pygame.init()
    pygame.mixer.init()
    pygame.font.init()
    logging.info("Engine Init Completed")

def simple_init():
    pygame.init()

def set_icon(icon):
    pygame.display.set_icon(icon)

def set_title(text):
    pygame.display.set_caption(text)

def build_display(size):
    try:
        return pygame.display.set_mode(size, pygame.FULLSCREEN if cf.SETTINGS['fullscreen'] else 0)
    except pygame.error as e:
        logging.exception(e)
        logging.error("Your screen is probably too small to activate fullscreen with this resolution. Close the engine and editor. Go to your saves/config.ini file and change the screensize variable to 1. Then try again.")
        return pygame.display.set_mode(size)

def get_screen_size():
    return pygame.display.get_surface().get_size()

def push_display(surf, size, new_surf):
    pygame.transform.scale(surf, size, new_surf)

def update_display():
    pygame.display.update()

def remove_display():
    pygame.display.quit()

def terminate(crash=False):
    on_end(crash)
    if pygame.mixer.get_init():
        pygame.mixer.music.stop()
        pygame.mixer.quit()
    pygame.quit()
    if constants['standalone']:
        sys.exit()

def on_end(crash=False):
    cf.save_settings()

# === timing functions ===
def update_time():
    # All measured in milliseconds
    constants['last_time'] = constants['current_time']
    constants['current_time'] = pygame.time.get_ticks()
    constants['delta_t'] = constants['current_time'] - constants['last_time']

def get_time() -> int:
    return constants['current_time']

def get_last_time():
    return constants['last_time']

def get_true_time():
    return pygame.time.get_ticks()

def get_delta():
    return constants['delta_t']

# === drawing functions ===
BLEND_RGB_ADD = pygame.BLEND_RGB_ADD
BLEND_RGB_SUB = pygame.BLEND_RGB_SUB
BLEND_RGB_MULT = pygame.BLEND_RGB_MULT
BLEND_RGBA_ADD = pygame.BLEND_RGBA_ADD
BLEND_RGBA_SUB = pygame.BLEND_RGBA_SUB
BLEND_RGBA_MULT = pygame.BLEND_RGBA_MULT

class BlendMode(Enum):
    NONE = 0
    BLEND_RGB_ADD = 1
    BLEND_RGB_SUB = 2
    BLEND_RGB_MULT = 3

    @staticmethod
    def convert(blendmode):
        if blendmode == BlendMode.BLEND_RGB_ADD:
            return pygame.BLEND_RGB_ADD
        elif blendmode == BlendMode.BLEND_RGB_SUB:
            return pygame.BLEND_RGB_SUB
        elif blendmode == BlendMode.BLEND_RGB_MULT:
            return pygame.BLEND_RGB_MULT
        return 0

Surface = pygame.Surface

def blit(dest, source, pos=(0, 0), mask=None, blend=0):
    dest.blit(source, pos, mask, blend)

def blit_center(dest, source, pos=(WINWIDTH//2, WINHEIGHT//2), mask=None, blend=0):
    x = pos[0] - source.get_width()//2
    y = pos[1] - source.get_height()//2
    dest.blit(source, (x, y), mask, blend)

def create_surface(size, transparent=False) -> pygame.Surface:
    if transparent:
        surf = pygame.Surface(size, pygame.SRCALPHA, 32)
        surf = surf.convert_alpha()
    else:
        surf = pygame.Surface(size)
        surf = surf.convert()
    return surf

def create_simple_surface(size) -> pygame.Surface:
    return pygame.Surface(size)

def copy_surface(surf):
    return surf.copy()

def save_surface(surf, fn):
    pygame.image.save(surf, fn)

def subsurface(surf, rect) -> pygame.Surface:
    x, y, width, height = rect
    twidth = max(0, min(surf.get_width() - x, width))
    theight = max(0, min(surf.get_height() - y, height))
    tx = max(0, x)
    ty = max(0, y)
    return surf.subsurface(tx, ty, twidth, theight)

def image_load(fn, convert=False, convert_alpha=False):
    image = pygame.image.load(fn)
    if convert:
        image = image.convert()
    elif convert_alpha:
        image = image.convert_alpha()
    return image

def surf_to_raw(surf: pygame.Surface, format: str) -> str:
    """Converts a given surface into a raw byte representation.

    Args:
        surf (pygame.Surface): surf to convert
        format (str): any valid format string (https://www.pygame.org/docs/ref/image.html#pygame.image.tostring)

    Returns:
        str: raw byte buffer version of surf
    """
    return pygame.image.tostring(surf, format)

def raw_to_surf(raw_bytes: str, size: Tuple[int, int], format: str) -> pygame.Surface:
    """converts a raw image string into a surface.

    Args:
        raw_bytes (str): raw data
        size (Tuple[int, int]): size of surface to be created
        format (str): the format string that was used to encode the raw data
            (https://www.pygame.org/docs/ref/image.html#pygame.image.tostring)

    Returns:
        pygame.Surface: the surface consisting of the raw image
    """
    return pygame.image.fromstring(raw_bytes, size, format)

def fill(surf, color, mask=None, blend=0):
    surf.fill(color, mask, blend)

def set_alpha(surf, alpha, rleaccel=False):
    if rleaccel:
        surf.set_alpha(alpha, pygame.RLEACCEL)
    else:
        surf.set_alpha(alpha)

def set_colorkey(surf, color, rleaccel=True):
    if rleaccel:
        surf.set_colorkey(color, pygame.RLEACCEL)
    else:
        surf.set_colorkey(color)

def make_pixel_array(surf):
    return pygame.PixelArray(surf)

def draw_line(surf, color, start, end, width=1):
    return pygame.draw.line(surf, color, start, end, width)

# === transform functions ===
def flip_horiz(surf):
    return pygame.transform.flip(surf, 1, 0)

def flip_vert(surf):
    return pygame.transform.flip(surf, 0, 1)

def transform_scale(surf, scale):
    return pygame.transform.scale(surf, scale)

def transform_rotate(surf, degrees):
    return pygame.transform.rotate(surf, degrees)

# === event functions ===
def get_key_name(key_code):
    return pygame.key.name(key_code)

key_map = {"enter": pygame.K_RETURN,
           "tab": pygame.K_TAB,
           "backspace": pygame.K_BACKSPACE,
           "pageup": pygame.K_PAGEUP,
           "f12": pygame.K_F12,
           "`": pygame.K_BACKQUOTE,
           "1": pygame.K_1,
           "2": pygame.K_2,
           "3": pygame.K_3,
           "4": pygame.K_4,
           "5": pygame.K_5,
           }

events = []
def get_events():
    global events
    events.clear()
    if fast_quit:
        terminate()
        return pygame.QUIT
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            terminate()
            return pygame.QUIT
        if event.type == pygame.KEYDOWN and cf.SETTINGS['debug']:
            if event.key == pygame.K_ESCAPE:
                terminate()
                return pygame.QUIT
        events.append(event)
    return events

# === controls functions ===
QUIT = pygame.QUIT
KEYUP = pygame.KEYUP
KEYDOWN = pygame.KEYDOWN
MOUSEBUTTONDOWN = pygame.MOUSEBUTTONDOWN
MOUSEBUTTONUP = pygame.MOUSEBUTTONUP
MOUSEMOTION = pygame.MOUSEMOTION

def get_pressed():
    return pygame.key.get_pressed()

def joystick_avail() -> bool:
    return pygame.joystick.get_init() and pygame.joystick.get_count()

def get_joystick():
    return pygame.joystick.Joystick(0)

def get_mouse_pos():
    return pygame.mouse.get_pos()

def get_mouse_focus():
    return pygame.mouse.get_focused()

# === loop functions ===
DISPLAYSURF = None
SCREENSIZE = (WINWIDTH * min(cf.SETTINGS['screen_size'], 5), WINHEIGHT * min(cf.SETTINGS['screen_size'], 5))

class Clock():
    def __init__(self) -> None:
        self.clock = pygame.time.Clock()

    def tick(self) -> int:
        return self.clock.tick(FPS)

# === System Messages

SYSTEM_FONT_SIZE = 14
def get_system_font():
    return pygame.font.Font(None, SYSTEM_FONT_SIZE)

def write_system_msg(surf, msg: str):
    """Writes a message to the screen.
    Use asterisks like **this** in order
    to emphasize a word.
    """
    fill(surf, (0, 0, 0))
    width = surf.get_width()
    font = get_system_font()
    x = 0
    y = 0
    lines = msg.splitlines()
    emphasis = False
    for line in lines:
        for word in line.split(' '):
            tmp = word
            if word.startswith('**'):
                emphasis = True
            tmp = word.strip('*')
            tmp += ' '
            wwidth = font.size(tmp)[0]
            if x + wwidth > width:
                x = 0
                y += SYSTEM_FONT_SIZE
            color = (0, 255, 0) if emphasis else (255, 255, 255) # green for emphasis
            wsurf = font.render(tmp, True, color)
            surf.blit(wsurf, (x, y))
            x += wwidth
            if word.endswith('**'):
                emphasis = False
        y += SYSTEM_FONT_SIZE
        x = 0
    return surf
