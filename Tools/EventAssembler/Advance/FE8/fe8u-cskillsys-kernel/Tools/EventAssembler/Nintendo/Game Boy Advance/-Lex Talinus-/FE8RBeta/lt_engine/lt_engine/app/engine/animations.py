from app.constants import TILEHEIGHT, TILEWIDTH
from app.engine import engine, image_mods
from app.utilities import utils

# Generic Animation Object
# Used, for instance, for miss and no damage animations

class Animation():
    def __init__(self, anim, position, delay=0, loop=False, hold=False, reverse=False, speed_adj: float = 1, contingent=False):
        self.nid = anim.nid
        if not anim.image:
            anim.image = engine.image_load(anim.full_path)
            anim.image = anim.image.convert_alpha()
        self.sprite = anim.image
        self.xy_pos = position
        self.position = position
        self.frame_x, self.frame_y = anim.frame_x, anim.frame_y
        self.num_frames = anim.num_frames
        self.anim_speed = anim.speed
        self.anim_frame_times = anim.frame_times
        self.use_frame_time = anim.use_frame_time
        self.speed_adj = speed_adj
        self.delay = delay
        self.loop = loop
        self.hold = hold
        self.reverse = reverse
        self.enabled = True
        self.tint: engine.BlendMode = engine.BlendMode.NONE
        self.tint_after_delay = None
        self.contingent = contingent

        self.width = self.sprite.get_width() // self.frame_x
        self.height = self.sprite.get_height() // self.frame_y

        self.image = engine.subsurface(self.sprite, (0, 0, self.width, self.height))

        self.counter = 0
        self.frames_held = 0
        self.first_update = engine.get_time()

    def save(self) -> tuple:
        return {'nid': self.nid,
                'pos': self.xy_pos,
                'loop': self.loop,
                'hold': self.hold,
                'reverse': self.reverse,
                'speed_adj': self.speed_adj,
                'tint': self.tint.value,
                'contingent': self.contingent}

    @property
    def speed(self):
        return int(self.anim_speed) * self.speed_adj

    @property
    def frame_times(self):
        return [int(frames * self.speed_adj) for frames in self.anim_frame_times]

    def use_center(self):
        self.position = self.position[0] - self.width//2, self.position[1] - self.height//2

    def is_ready(self, current_time):
        return self.enabled and (current_time - self.first_update >= self.delay)

    def get_position(self, offset):
        if offset:
            return self.position[0] + offset[0], self.position[1] + offset[1]
        else:
            return self.position

    def set_tint(self, val: engine.BlendMode):
        self.tint = val

    def set_tint_after_delay(self, i):
        self.tint_after_delay = i

    def get_wait(self) -> int:
        # Returns number of milliseconds
        if self.use_frame_time:
            return utils.frames2ms(sum(self.frame_times))
        else:
            return self.num_frames * self.speed

    def update(self):
        current_time = engine.get_time()
        if not self.is_ready(current_time):
            return

        done = False
        if self.use_frame_time:
            # Frame by frame timing
            num_frames = self.frame_times[self.counter]
            self.frames_held += 1
            if self.frames_held >= num_frames:
                self.frames_held = 0
                self.counter += 1
            if self.counter >= min(len(self.frame_times), self.num_frames):
                if self.loop:
                    self.counter = 0
                    self.frames_held = 0
                    self.delay = 0
                elif self.hold:
                    self.counter = self.num_frames - 1
                else:
                    self.counter = self.num_frames - 1
                    done = True
        else:  
            # Constant ms timing
            self.counter = int(current_time - self.first_update) // self.speed
            if self.counter >= self.num_frames:
                if self.loop:
                    self.counter = 0
                    self.first_update = current_time
                    self.delay = 0
                elif self.hold:
                    self.counter = self.num_frames - 1
                else:
                    self.counter = self.num_frames - 1
                    done = True

        if self.tint_after_delay == self.counter:
            self.tint = engine.BlendMode.BLEND_RGB_ADD

        # Now actually create image
        if self.reverse:
            frame_counter = self.num_frames - 1 - self.counter
        else:
            frame_counter = self.counter
        left = (frame_counter % self.frame_x) * self.width
        top = (frame_counter // self.frame_x) * self.height
        self.image = engine.subsurface(self.sprite, (left, top, self.width, self.height))

        return done

    def draw(self, surf, offset=None, blend=None):
        current_time = engine.get_time()
        if not self.is_ready(current_time):
            return surf
        x, y = self.get_position(offset)
        if blend:
            image = image_mods.change_color(self.image, blend)
        else:
            image = self.image
        if self.tint:
            engine.blit(surf, image, (x, y), None, engine.BlendMode.convert(self.tint))
        else:
            surf.blit(image, (x, y))
        return surf

class MapAnimation(Animation):
    def __init__(self, anim, position, delay=0, loop=False, hold=False, reverse=False, speed_adj: float = 1, contingent=False):
        super().__init__(anim, position, delay, loop, hold, reverse, speed_adj=speed_adj, contingent=contingent)
        self.position = self.position[0] * TILEWIDTH, self.position[1] * TILEHEIGHT
        self.use_center()

    def use_center(self):
        self.position = self.position[0] + TILEWIDTH//2 - self.width//2, self.position[1] + TILEHEIGHT//2 - self.height//2

    def get_position(self, offset):
        if offset:
            return self.position[0] + offset[0] * TILEWIDTH, self.position[1] + offset[1] * TILEHEIGHT
        else:
            return self.position
