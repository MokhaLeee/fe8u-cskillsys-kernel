import random

from app.engine import engine
from app import counters
from app.utilities import utils
from app.constants import COLORKEY, PORTRAIT_WIDTH

class InfoMenuPortrait():
    main_portrait_coords = (0, 0, 96, 80)

    halfblink = (PORTRAIT_WIDTH - 32, 48, 32, 16)
    fullblink = (PORTRAIT_WIDTH - 32, 64, 32, 16)

    stat_screen_mouth_coords = (PORTRAIT_WIDTH - 32, 80, 32, 16)

    def __init__(self, portrait, should_blink: bool = False):
        self.portrait = portrait
        if not self.portrait.image:
            self.portrait.image = engine.image_load(self.portrait.full_path)
        self.portrait.image = self.portrait.image.convert()
        engine.set_colorkey(self.portrait.image, COLORKEY, rleaccel=True)
        self.main_portrait = engine.subsurface(self.portrait.image, self.main_portrait_coords)
        self.mouth_section = engine.subsurface(self.portrait.image, self.stat_screen_mouth_coords)

        self.should_blink = should_blink
        offset_blinking = range(-2000, 2000, 125)
        self.blink_counter = \
            counters.generic3counter(7000 + random.choice(offset_blinking), utils.frames2ms(3), utils.frames2ms(3))
        self.blink_counter.last_update = engine.get_time()

    def create_image(self):
        main_image = self.main_portrait.copy()

        if self.should_blink:
            if self.blink_counter.count == 0:
                blink_image = None
            elif self.blink_counter.count == 1:
                blink_image = engine.subsurface(self.portrait.image, self.halfblink)
            elif self.blink_counter.count == 2:
                blink_image = engine.subsurface(self.portrait.image, self.fullblink)
            if blink_image:
                main_image.blit(blink_image, self.portrait.blinking_offset)

        main_image.blit(self.mouth_section, self.portrait.smiling_offset)
        return main_image

    def update(self):
        self.blink_counter.update(engine.get_time())
