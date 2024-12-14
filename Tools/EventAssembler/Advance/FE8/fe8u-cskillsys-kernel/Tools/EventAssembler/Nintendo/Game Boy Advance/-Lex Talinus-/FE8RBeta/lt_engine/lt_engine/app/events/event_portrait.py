import math
import random
from typing import Optional

from app.utilities.typing import Point
from app.data.resources.portraits import PortraitPrefab

from app import counters
from app.utilities import utils
from app.constants import PORTRAIT_WIDTH, PORTRAIT_HEIGHT, COLORKEY

from app.engine import engine, image_mods

class EventPortrait():
    width, height = PORTRAIT_WIDTH, PORTRAIT_HEIGHT
    main_portrait_coords = (0, 0, 96, 80)
    chibi_coords = (width - 32, 16, 32, 32)

    halfblink = (width - 32, 48, 32, 16)
    fullblink = (width - 32, 64, 32, 16)

    openmouth = (0, height - 16, 32, 16)
    halfmouth = (32, height - 16, 32, 16)
    closemouth = (64, height - 16, 32, 16)

    opensmile = (0, height - 32, 32, 16)
    halfsmile = (32, height - 32, 32, 16)
    closesmile = (64, height - 32, 32, 16)

    base_transition_speed = utils.frames2ms(14)
    travel_time = utils.frames2ms(15)
    bop_time = utils.frames2ms(8)
    saturation_time = utils.frames2ms(10)
    travel_speed_mult = 1

    def __init__(self, portrait: PortraitPrefab, position: Point, priority,
                 transition=False, slide=None, mirror=False, name='', expressions=None,
                 speed_mult=1):
        self.portrait = portrait
        if not self.portrait.image:
            self.portrait.image = engine.image_load(self.portrait.full_path)
        self.width = self.portrait.image.get_width()
        self.height = self.portrait.image.get_height()
        self.portrait.image = self.portrait.image.convert()
        engine.set_colorkey(self.portrait.image, COLORKEY, rleaccel=True)
        self.position = position
        self.priority = priority
        self.transition = transition
        self.transition_speed = self.base_transition_speed * max(speed_mult, 0.001)
        self.transition_update = engine.get_time()
        self.slide = slide
        self.mirror = mirror
        self.name = name
        self.expressions = expressions or set()

        self.transition_progress = 0
        self.main_portrait = engine.subsurface(self.portrait.image, self.main_portrait_coords)
        self.chibi = engine.subsurface(self.portrait.image, self.chibi_coords)

        self.talk_on = False
        self.remove = False

        # For moving
        self.moving = False
        self.orig_position = None
        self.next_position = None

        # For talking
        self.talk_state = 0
        self.last_talk_update = 0
        self.next_talk_update = 0

        # For blinking
        # Blinking set up
        self.offset_blinking = [x for x in range(-2000, 2000, 125)]
        # 3 frames for each
        self.blink_counter = counters.generic3counter(7000 + random.choice(self.offset_blinking), utils.frames2ms(3), utils.frames2ms(3))

        # For bop
        self.bops_remaining = 0
        self.bop_state = False
        self.bop_height = 2
        self.last_bop = None

        # For saturation
        self.saturation = 1.
        self.saturation_direction = 0

    def get_width(self):
        return 96

    def get_height(self):
        return 80

    def set_expression(self, expression_list):
        self.expressions = expression_list

    def saturate(self):
        self.saturation_direction = 1

    def desaturate(self):
        self.saturation_direction = -1

    def bop(self, num=2, height=2):
        self.bops_remaining = num
        self.bop_state = False
        self.bop_height = height
        self.last_bop = engine.get_time()

    def move(self, position, speed_mult=1):
        self.orig_position = self.position
        self.next_position = position
        self.moving = True
        self.travel_speed_mult = max(0.001, speed_mult)

        self.travel_time = self.determine_travel_time(utils.distance(self.next_position, self.orig_position))
        self.travel_time = int(self.travel_time / speed_mult)

    def quick_move(self, position):
        self.position = position

    def determine_travel_time(self, distance):
        counter = 0
        while distance > 0:
            counter += 1
            change = int(round(distance / 8))
            change = utils.clamp(change, 1, 8)
            distance -= change
        return utils.frames2ms(counter)

    def talk(self):
        self.talk_on = True

    def stop_talking(self):
        self.talk_on = False

    def update_talk(self, current_time):
        # update mouth
        if self.talk_on and current_time - self.last_talk_update > self.next_talk_update:
            self.last_talk_update = current_time
            chance = random.randint(1, 10)
            if self.talk_state == 0:
                # 10% chance to skip to state 2
                if chance == 1:
                    self.talk_state = 2
                    self.next_talk_update = random.randint(70, 160)
                else:
                    self.talk_state = 1
                    self.next_talk_update = random.randint(30, 50)
            elif self.talk_state == 1:
                # 10% chance to go back to state 0
                if chance == 1:
                    self.talk_state = 0
                    self.next_talk_update = random.randint(50, 100)
                else:
                    self.talk_state = 2
                    self.next_talk_update = random.randint(70, 160)
            elif self.talk_state == 2:
                # 10% chance to skip back to state 0
                # 10% chance to go back to state 1
                chance = random.randint(1, 10)
                if chance == 1:
                    self.talk_state = 0
                    self.next_talk_update = random.randint(50, 100)
                elif chance == 2:
                    self.talk_state = 1
                    self.next_talk_update = random.randint(30, 50)
                else:
                    self.talk_state = 3
                    self.next_talk_update = random.randint(30, 50)
            elif self.talk_state == 3:
                self.talk_state = 0
                self.next_talk_update = random.randint(50, 100)
        if not self.talk_on:
            self.talk_state = 0

    def create_image(self):
        main_image = self.main_portrait.copy()
        # For smile image
        if "OpenMouth" in self.expressions:
            if "Smile" in self.expressions:
                mouth_image = engine.subsurface(self.portrait.image, self.opensmile)
            else:
                mouth_image = engine.subsurface(self.portrait.image, self.openmouth)
        elif "Smile" in self.expressions:
            if self.talk_state == 0:
                mouth_image = engine.subsurface(self.portrait.image, self.closesmile)
            elif self.talk_state == 1 or self.talk_state == 3:
                mouth_image = engine.subsurface(self.portrait.image, self.halfsmile)
            elif self.talk_state == 2:
                mouth_image = engine.subsurface(self.portrait.image, self.opensmile)
        else:
            if self.talk_state == 0:
                mouth_image = engine.subsurface(self.portrait.image, self.closemouth)
            elif self.talk_state == 1 or self.talk_state == 3:
                mouth_image = engine.subsurface(self.portrait.image, self.halfmouth)
            elif self.talk_state == 2:
                mouth_image = engine.subsurface(self.portrait.image, self.openmouth)

        # For blink image
        if "CloseEyes" in self.expressions:
            blink_image = engine.subsurface(self.portrait.image, self.fullblink)
        elif "HalfCloseEyes" in self.expressions:
            blink_image = engine.subsurface(self.portrait.image, self.halfblink)
        elif "OpenEyes" in self.expressions:
            blink_image = None
        else:
            if self.blink_counter.count == 0:
                blink_image = None
            elif self.blink_counter.count == 1:
                blink_image = engine.subsurface(self.portrait.image, self.halfblink)
            elif self.blink_counter.count == 2:
                blink_image = engine.subsurface(self.portrait.image, self.fullblink)

        # Piece together image
        if blink_image:
            main_image.blit(blink_image, self.portrait.blinking_offset)
        main_image.blit(mouth_image, self.portrait.smiling_offset)
        return main_image

    def update(self) -> bool:
        current_time = engine.get_time()
        delta_time = engine.get_delta()
        self.update_talk(current_time)
        self.blink_counter.update(current_time)

        if self.saturation_direction != 0:
            self.saturation += self.saturation_direction * delta_time / self.saturation_time
            self.saturation = utils.clamp(self.saturation, 0, 1)
            # If reached one of the two extremes
            if self.saturation == 0 or self.saturation == 1:
                self.saturation_direction = 0

        if self.transition:
            # 14 frames for unit face to appear
            perc = (current_time - self.transition_update) / self.transition_speed
            if self.remove:
                perc = 1 - perc
            self.transition_progress = perc
            if perc > 1 or perc < 0:
                self.transition = False
                self.transition_progress = utils.clamp(perc, 0, 1)
                if self.remove:
                    return True

        if self.moving:
            distance = utils.distance(self.next_position, self.position)
            if distance == 0:
                self.position = self.next_position
                self.moving = False
                self.bop_state = False
                # self.bop(num=1, height=1)
            else:
                # The below does not actually contain the CORRECT true-to-GBA algorithm
                # Just a close simple approximation, because I could not determine the GBA algorithm perfectly
                # 15 frames (250 ms) to lerp 24 pixels
                # 30 frames (500 ms) to lerp 120 pixels
                # 45 frames? (750 ms) to lerp 264 pixels
                travel_mag = int(round(distance / 8))
                travel_mag = utils.clamp(travel_mag, 1, 8)
                if travel_mag in (1, 4, 5, 6, 7):
                    self.bop_state = True
                    self.bop_height = 1
                # Multiply by travel speed
                travel_mag = min(self.travel_speed_mult * travel_mag, distance)
                diff_x = self.next_position[0] - self.position[0]
                diff_y = self.next_position[1] - self.position[1]
                angle = math.atan2(diff_y, diff_x)
                updated_position = (self.position[0] + (travel_mag * math.cos(angle)),
                                    self.position[1] + (travel_mag * math.sin(angle)))
                # updated_position = (self.position[0] + (travel_mag * direction), self.position[1])
                self.position = updated_position

        if self.bops_remaining:
            if current_time - self.last_bop > self.bop_time:
                self.last_bop += self.bop_time
                if self.bop_state:
                    self.bops_remaining -= 1
                self.bop_state = not self.bop_state

        return False

    def draw(self, surf):
        image = self.create_image()
        if self.mirror:
            image = engine.flip_horiz(image)

        if self.saturation < 1:
            blackness = 0.5 * (1 - self.saturation)
            image = image_mods.make_black_colorkey(image, blackness)

        if self.transition:
            if self.slide:
                image = image_mods.make_translucent(image.convert_alpha(), 1 - self.transition_progress)
            else:
                image = image_mods.make_black_colorkey(image, 1 - self.transition_progress)

        position = self.position

        slide_length = 24
        if self.slide == 'right':
            position = position[0] + slide_length - int(slide_length * self.transition_progress), self.position[1]
        elif self.slide == 'left':
            position = position[0] - slide_length + int(slide_length * self.transition_progress), self.position[1]

        if self.bop_state:
            position = position[0], position[1] + self.bop_height

        surf.blit(image, position)

    def end(self, speed_mult=1, slide: Optional[str] = None):
        self.transition = True
        self.remove = True
        if slide:  # Use the existing slide if not specified
            self.slide = slide
        self.transition_speed = self.base_transition_speed * max(speed_mult, 0.001)
        self.transition_update = engine.get_time()
