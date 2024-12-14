import math
from app import counters

from app.constants import WINWIDTH, WINHEIGHT
from app.utilities import utils
from app.engine.sprites import SPRITES
from app.engine.fonts import FONT
from app.engine.input_manager import get_input_manager

from app.engine import engine, image_mods, icons
from app.engine.graphics.text.text_renderer import render_text, text_width

class DamageNumber():
    time_bounce = 400
    time_pause = 600
    time_total = 1200

    def __init__(self, num, idx, length, left, color):
        image = SPRITES.get('damage_numbers_' + color)
        if color.startswith('small'):
            self.small = True
        else:
            self.small = False

        self.num = num
        self.idx = idx
        self.length = length
        self.left = left
        self.true_image = engine.subsurface(image, (num*16, 0, 16, 16))
        self.image = None
        self.done = False
        self.start_time = engine.get_time()
        self.top_pos = 0
        self.state = -1
        if self.small:
            self.init_time = 50 * self.idx
        else:
            self.init_time = 50 * self.idx + 50

    def update(self):
        new_time = float(engine.get_time() - self.start_time)
        # Totally transparent start_up
        if self.state == -1:
            if new_time > self.init_time:
                self.state = 0
        # Initial bouncing and fading in
        if self.state == 0:
            state_time = new_time - self.init_time
            # Position
            self.top_pos = 10 * math.exp(-state_time/250) * math.sin(state_time/25)
            # Transparency
            new_transparency = (200 - state_time)/200.  # First 200 milliseconds of transparency
            self.image = image_mods.make_translucent(self.true_image, new_transparency)
            if state_time > self.time_bounce:
                self.state = 1
                self.top_pos = 0
        # Pause
        if self.state == 1:
            if new_time > self.init_time + self.time_bounce + self.time_pause:
                self.state = 2
        # Fade out and up
        if self.state == 2:
            state_time = new_time - self.init_time - self.time_bounce - self.time_pause
            # Position
            self.top_pos = state_time/10
            # Transparency
            new_transparency = state_time/150
            self.image = image_mods.make_translucent(self.true_image, new_transparency)
            if new_time > self.time_total:
                self.done = True

    def draw(self, surf, pos):
        if self.image:
            if self.small:
                true_pos = pos[0] - 4*self.length + 8*self.idx, pos[1] - self.top_pos
            else:
                true_pos = pos[0] - 7*self.length + 14*self.idx, pos[1] - self.top_pos
            surf.blit(self.image, true_pos)

class SkillIcon():
    def __init__(self, skill, right, small=False, center=False):
        # small doesn't seem to be ever used
        self.skill = skill
        self.right = right
        self.center = center
        self.small = small
        self.font = 'text'
        self.text = self.skill.name
        self.text_width = text_width(self.font, self.text)
        icon = icons.get_icon(self.skill)
        if icon:
            if self.small:
                self.surf = icon
            else:
                self.surf = engine.create_surface((self.text_width + 22, 32), transparent=True)
                if self.center:
                    render_text(self.surf, [self.font], [self.text], None, topleft=(10, 0))
                    self.surf.blit(icon, (self.text_width//2 + 4, 16))
                elif self.right:
                    render_text(self.surf, [self.font], [self.text], None, topleft=(20, 0))
                    self.surf.blit(icon, (0, 0))
                else:
                    render_text(self.surf, [self.font], [self.text], None, topleft=(0, 0))
                    self.surf.blit(icon, (self.text_width + 4, 0))
        else:
            self.surf = engine.create_surface((self.text_width + 22, 16), transparent=True)
            if self.center:
                render_text(self.surf, [self.font], [self.text], None, topleft=(10, 0))
            elif self.right:
                render_text(self.surf, [self.font], [self.text], None, topleft=(20, 0))
            else:
                render_text(self.surf, [self.font], [self.text], None, topleft=(0, 0))

        self.image = self.surf

        self.start_time = engine.get_time()
        self.done: bool = False
        self.state = 'in'

        self.fade_time = 300 if self.small else 400
        self.hold_time = 700 if self.small else 1100

        self.left_pos = 0

    def update(self):
        current_time = engine.get_time() - self.start_time

        if self.state == 'in':
            self.left_pos = 10 * math.exp(-current_time/250) * math.sin(current_time/25)
            transparency = 1 - utils.clamp(current_time/200, 0, 1)
            self.image = image_mods.make_translucent(self.surf, transparency)
            if current_time > self.fade_time:
                self.state = 'hold'
                self.left_pos = 0

        elif self.state == 'hold':
            self.image = self.surf
            if current_time > self.hold_time:
                self.state = 'out'

        elif self.state == 'out':
            state_time = current_time - self.hold_time
            transparency = utils.clamp(state_time/300, 0, 1)
            self.image = image_mods.make_translucent(self.surf, transparency)
            if state_time > self.fade_time:
                self.done = True

    def draw(self, surf, pos=None):
        if self.small and pos:
            pos = pos[0] + self.left_pos, pos[1]
            surf.blit(self.image, pos)
        elif pos:
            pos = pos[0] + self.left_pos - self.text_width//2 - 8, pos[1]
            surf.blit(self.image, pos)
        else:
            if self.center:
                x_pos = (WINWIDTH - 26 - self.text_width + self.left_pos)//2
            elif self.right:
                x_pos = WINWIDTH - 26 - self.text_width + self.left_pos
            else:
                x_pos = self.left_pos + 4
            surf.blit(self.image, (x_pos, 32))

class ScrollArrow():
    def __init__(self, direction, topleft, offset=0):
        self.images = {'up': SPRITES.get('scroll_arrows'),
                       'down': engine.flip_horiz(engine.flip_vert(SPRITES.get('scroll_arrows'))),
                       'left': SPRITES.get('page_arrows'),
                       'right': engine.flip_horiz(engine.flip_vert(SPRITES.get('page_arrows')))}

        self.x, self.y = topleft
        self.direction = direction
        self.arrow_counter = counters.arrow_counter(offset)
        self.offset = []

    def pulse(self):
        self.arrow_counter.pulse()
        self.offset = [4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1]

    def draw(self, surf):
        self.arrow_counter.update()
        if self.direction == 'up':
            pos = (self.x, self.y - (self.offset.pop(0) if self.offset else 0))
            surf.blit(engine.subsurface(self.images['up'], (0, self.arrow_counter.get() * 8, 14, 8)), pos)
        elif self.direction == 'down':
            pos = (self.x, self.y + (self.offset.pop(0) if self.offset else 0))
            surf.blit(engine.subsurface(self.images['down'], (0, self.arrow_counter.get() * 8, 14, 8)), pos)
        elif self.direction == 'left':
            pos = (self.x - (self.offset.pop(0) if self.offset else 0), self.y)
            surf.blit(engine.subsurface(self.images['left'], (self.arrow_counter.get() * 8, 0, 8, 14)), pos)
        elif self.direction == 'right':
            pos = (self.x + (self.offset.pop(0) if self.offset else 0), self.y)
            surf.blit(engine.subsurface(self.images['right'], (self.arrow_counter.get() * 8, 0, 8, 14)), pos)

class ScrollBar():
    def __init__(self):
        self.top = engine.subsurface(SPRITES.get('scroll_bar'), (0, 0, 7, 1))
        self.bottom = engine.subsurface(SPRITES.get('scroll_bar'), (0, 2, 7, 1))
        self.middle = engine.subsurface(SPRITES.get('scroll_bar'), (0, 1, 7, 1))
        self.fill = engine.subsurface(SPRITES.get('scroll_bar'), (0, 3, 7, 1))
        self.arrow_counter = counters.arrow_counter()

    def draw(self, surf, topright, scroll, limit, num_options, option_height=16):
        """
        surf -- Surface to draw the scroll bar on
        topright -- Topright coordinate to display scroll bar
        scroll -- How far down the menu has scrolled
        limit -- How many options can be displayed at once
        num_options -- Total number of options in menu
        option_height -- height of each option
        """
        self.arrow_counter.update()

        # Get properties
        x = topright[0] - 9
        y = topright[1] + 12
        height = limit * option_height - 20
        start_fraction = scroll / num_options
        end_fraction = min(1, (scroll + limit) / num_options)

        # Draw parts
        surf.blit(self.top, (x, y))
        surf.blit(self.bottom, (x, y + height + 2))
        for num in range(1, height + 2):
            surf.blit(self.middle, (x, y + num))

        # Draw bar
        start_pos = int(start_fraction * height)
        end_pos = int(end_fraction * height)
        for num in range(start_pos, end_pos + 1):
            surf.blit(self.fill, (x, y + num + 1))

        # Draw arrows
        if start_pos > 0:
            top_arrow = engine.subsurface(SPRITES.get('scroll_bar'), (8, 4 + self.arrow_counter.get() * 6, 8, 6))
            surf.blit(top_arrow, (x - 1, y - 7))
        if end_pos < height:
            bottom_arrow = engine.subsurface(SPRITES.get('scroll_bar'), (0, 4 + self.arrow_counter.get() * 6, 8, 6))
            surf.blit(bottom_arrow, (x - 1, y + height + 4))

class Logo():
    def __init__(self, texture, center, num_frames=8, speed=64):
        self.texture = texture
        self.center = center
        self.num_frames = num_frames
        self.speed = speed

        self.height = self.texture.get_height()//self.num_frames
        self.width = self.texture.get_width()

        self.counter = 0
        self.anim = [0, 0] + list(range(1, self.num_frames - 1)) + \
            [self.num_frames - 1, self.num_frames - 1] + list(reversed(range(1, self.num_frames - 1)))
        self.last_update = engine.get_time()
        self.transition_counter = 0

        self.image = self.get_image()
        self.draw_image = self.image

        self.state = "idle"

    def get_image(self):
        rect = (0, self.anim[self.counter] * self.height, self.width, self.height)
        return engine.subsurface(self.texture, rect)

    def update(self):
        current_time = engine.get_time()
        diff = current_time - self.last_update

        if diff > self.speed:
            self.counter += 1
            self.counter %= len(self.anim)
            self.image = self.get_image()
            self.last_update = current_time - (diff - self.speed)

        if self.state == 'idle':
            self.draw_image = self.image

        elif self.state == 'out':
            self.transition_counter -= 1
            self.draw_image = engine.subsurface(self.image, (0, self.height//2 - self.transition_counter, self.width, self.transition_counter * 2))

            if self.transition_counter <= 0:
                self.state = 'in'
                self.texture = self.next_texture
                self.height = self.texture.get_height()//self.num_frames
                self.width = self.texture.get_width()
                self.image = self.get_image()

        elif self.state == 'in':
            self.transition_counter += 1
            if self.transition_counter >= self.height//2:
                self.transition_counter = self.height//2
                self.state = 'idle'

            self.draw_image = engine.subsurface(self.image, (0, self.height//2 - self.transition_counter, self.width, self.transition_counter * 2))

    def draw(self, surf):
        engine.blit_center(surf, self.draw_image, self.center)

    def switch_image(self, new_image):
        self.next_texture = new_image
        self.transition_counter = self.height//2
        self.state = 'out'

class PopUpDisplay():
    def __init__(self, topright):
        self.topright = topright
        self.update_num = -200

    def start(self, text):
        self.update_num = 100
        if isinstance(text, int):
            money = text
            if money >= 0:
                font = FONT['text-green']
            else:
                font = FONT['text-red']
            my_str = str(money)
            if money >= 0:
                my_str = '+' + my_str
        else:
            font = FONT['text-blue']
            my_str = str(text)
        self.width = font.width(my_str) + 8
        self.surf = engine.create_surface((self.width, 16), transparent=True)
        font.blit(my_str, self.surf, (0, 0))

    def draw(self, surf):
        if self.update_num > -200:
            self.update_num -= 5
            # Fade in and move up
            if self.update_num > 0:
                my_surf = image_mods.make_translucent(self.surf, self.update_num/100.)
                surf.blit(my_surf, (self.topright[0] - self.width + 8, self.topright[1] + self.update_num//5))
            # Fade out
            else:
                if self.update_num < -100:
                    my_surf = image_mods.make_translucent(self.surf, (-self.update_num - 100)/100.)
                else:
                    my_surf = self.surf
                surf.blit(my_surf, (self.topright[0] - self.width + 8, self.topright[1]))

class MouseIndicator():
    def __init__(self) -> None:
        self.mouse_indicator_right = SPRITES.get('mouse_indicator_right')
        self.mouse_indicator_left = engine.flip_horiz(SPRITES.get('mouse_indicator_right'))
        self.mouse_indicator_top = SPRITES.get('mouse_indicator_top')
        self.mouse_indicator_bottom = engine.flip_vert(SPRITES.get('mouse_indicator_top'))

    def draw(self, surf):
        mouse_position = get_input_manager().get_real_mouse_position()
        if mouse_position:
            mouse_x, mouse_y = mouse_position
            if mouse_x <= 16:
                surf.blit(self.mouse_indicator_left, (0, 0), None, engine.BLEND_RGB_ADD)
            elif mouse_x >= WINWIDTH - 16:
                surf.blit(self.mouse_indicator_right, (WINWIDTH - self.mouse_indicator_right.get_width(), 0), None, engine.BLEND_RGB_ADD)
            elif mouse_y <= 16:
                surf.blit(self.mouse_indicator_top, (0, 0), None, engine.BLEND_RGB_ADD)
            elif mouse_y >= WINHEIGHT - 16:
                surf.blit(self.mouse_indicator_bottom, (0, WINHEIGHT - self.mouse_indicator_bottom.get_height()), None, engine.BLEND_RGB_ADD)
