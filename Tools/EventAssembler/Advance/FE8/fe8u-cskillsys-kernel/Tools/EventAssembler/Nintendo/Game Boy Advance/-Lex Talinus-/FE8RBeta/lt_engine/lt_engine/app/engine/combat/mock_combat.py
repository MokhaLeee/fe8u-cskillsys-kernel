import random

from app.constants import WINWIDTH, WINHEIGHT
from app.data.resources.resources import RESOURCES

from app.engine.sprites import SPRITES

from app.utilities import utils
from app.utilities.typing import NID

from app.engine import engine, gui, image_mods, background
from app.engine.animations import Animation


class MockCombat():
    def __init__(self, left_anim, right_anim, at_range=0, pose='Attack', lp_anim=None, rp_anim=None):
        self.left_battle_anim = left_anim
        self.lp_battle_anim = lp_anim
        self.right_battle_anim = right_anim
        self.rp_battle_anim = rp_anim
        self.current_battle_anim = self.right_battle_anim

        self.at_range = at_range
        self.pose = pose

        self.last_update = engine.get_time()
        self.state = 'init'

        # Build platforms
        platform_type = 'Floor'
        if self.at_range:
            suffix = '-Ranged'
        else:
            suffix = '-Melee'
        platform_full_loc = RESOURCES.platforms.get(platform_type + suffix)
        self.left_platform = engine.image_load(platform_full_loc)
        self.right_platform = engine.flip_horiz(self.left_platform.copy())
        self.battle_background = None

        self.setup_dark()
        self.setup_ui()
        self.bar_offset = 1
        self.name_offset = 1
        self.platform_offset = 1

        # For pan
        self.focus_right: bool = True
        self.setup_pan()

        # For shake
        self.setup_shake()

        self.create_background()

        self.battle_music = None
        self.damage = 0

    def create_background(self):
        res = RESOURCES.panoramas.get('promotion_background')
        img = None
        if res:
            img = engine.image_load(res.full_image)
        if img:
            self.bg = background.SpriteBackground(img, fade=False)
        else:
            self.bg = background.create_background('default_background')

    def setup_dark(self):
        self.darken_background = 0
        self.target_dark = 0
        self.darken_ui_background = 0
        self.foreground = background.Foreground()
        self.combat_surf = engine.create_surface((WINWIDTH, WINHEIGHT), transparent=True)

    def setup_ui(self):
        self.bar_offset = 0
        self.name_offset = 0
        self.platform_offset = 0
        self.damage_numbers = []
        self.proc_icons = []
        self.animations = []

        self.left_stats = None
        self.right_stats = None

    def setup_pan(self):
        self.pan_dir = 0
        if self.at_range == 1:
            self.pan_max = 16
            self.pan_move = 4
        elif self.at_range == 2:
            self.pan_max = 32
            self.pan_move = 8
        elif self.at_range >= 3:
            self.pan_max = 120
            self.pan_move = 25
        else:
            self.pan_max = 0
            self.pan_move = 0

        if self.focus_right:
            self.pan_offset = -self.pan_max
        else:
            self.pan_offset = self.pan_max

    def setup_shake(self):
        self.shake_set = [(0, 0)]
        self.shake_offset = (0, 0)
        self.current_shake = 0
        self.platform_shake_set = [(0, 0)]
        self.platform_shake_offset = (0, 0)
        self.platform_current_shake = 0

    def update_anims(self):
        # Update battle anims
        self.left_battle_anim.update()
        self.right_battle_anim.update()
        if self.lp_battle_anim:
            self.lp_battle_anim.update()
        if self.rp_battle_anim:
            self.rp_battle_anim.update()

        # Update shake
        if self.current_shake:
            self.shake_offset = self.shake_set[self.current_shake - 1]
            self.current_shake += 1
            if self.current_shake > len(self.shake_set):
                self.current_shake = 0
        if self.platform_current_shake:
            self.platform_shake_offset = self.platform_shake_set[self.platform_current_shake - 1]
            self.platform_current_shake += 1
            if self.platform_current_shake > len(self.platform_shake_set):
                self.platform_current_shake = 0

    def take_input(self, event):
        pass

    def update(self):
        current_time = engine.get_time() - self.last_update
        self.current_state = self.state

        if self.state == 'init':
            if current_time > 400:
                self.state = 'begin_phase'

        if self.state == 'begin_phase':
            self.damage = random.randint(4, 25)
            self.set_up_combat_animation()

        elif self.state == 'anim':
            if self.left_battle_anim.done() and self.right_battle_anim.done():
                self.state = 'end_phase'

        elif self.state == 'hp_change':
            proceed = self.current_battle_anim.can_proceed()
            if proceed and current_time > utils.frames2ms(27):
                self.current_battle_anim.resume()
                self.state = 'anim'

        elif self.state == 'end_phase':
            self.state = 'begin_phase'

        elif self.state == 'end_combat':
            if self.left_battle_anim.done() and self.right_battle_anim.done():
                self.state = 'done'
                self.focus_exp()
                self.move_camera()

        elif self.state == 'done':
            pass # done

        if self.state != self.current_state:
            self.last_update = engine.get_time()

        self.update_anims()

        return self.state == 'done'

    def set_up_combat_animation(self):
        self.state = 'anim'
        self.current_battle_anim = self.right_battle_anim
        self.current_battle_anim.start_anim(self.pose)
        self.focus_right = True
        self.move_camera()

    def start_hit(self, sound=True, miss=False):
        self.last_update = engine.get_time()
        if not miss:
            self.state = 'hp_change'
            self.generate_damage_numbers()

        if miss:
            self.damage = 0
            self.miss_anim()

    def spell_hit(self):
        self.last_update = engine.get_time()
        self.state = 'hp_change'
        self.generate_damage_numbers()

    def generate_damage_numbers(self):
        str_damage = str(self.damage)
        for idx, num in enumerate(str_damage):
            d = gui.DamageNumber(int(num), idx, len(str_damage), True, 'red')
            self.damage_numbers.append(d)

    def get_damage(self):
        return self.damage

    def get_from_playback(self, s: str):
        return []

    def get_glancing_hit(self):
        glancing_hit_marks = self.get_from_playback('mark_glancing_hit')
        return glancing_hit_marks

    def left_team(self):
        return 'enemy'

    def right_team(self):
        return 'player'

    def get_color(self, team: NID) -> str:
        if team == 'player':
            return 'blue'
        else:
            return 'red'

    def hit_modifiers(self):
        if self.get_damage() > 0 and self.get_glancing_hit():
            self.glancing_hit()

    def hit_spark(self):
        if self.get_damage() > 0:
            if self.get_glancing_hit():
                self.glancing_hit()
            else:
                if self.current_battle_anim is self.right_battle_anim:
                    position = (-110 + WINWIDTH//2 - 120, -30)
                else:
                    position = (-40 + WINWIDTH//2 - 120, -30)
                anim_nid = 'HitSpark'
                animation = RESOURCES.animations.get(anim_nid)
                if animation:
                    anim = Animation(animation, position)
                    anim.set_tint(engine.BlendMode.BLEND_RGB_ADD)
                    self.animations.append(anim)
        else:
            self.no_damage()

    def crit_spark(self):
        if self.get_damage() > 0:
            anim_nid = 'CritSpark'
            animation = RESOURCES.animations.get(anim_nid)
            if animation:
                position = (-40 + WINWIDTH//2 - 120, -30)
                anim = Animation(animation, position)
                if self.current_battle_anim is self.right_battle_anim:
                    pass
                else:
                    anim.sprite = engine.flip_horiz(anim.sprite)
                anim.set_tint(engine.BlendMode.BLEND_RGB_ADD)
                self.animations.append(anim)
        else:
            self.no_damage()

    def no_damage(self):
        if self.current_battle_anim is self.right_battle_anim or self.current_battle_anim is self.rp_battle_anim:
            position = (52, 21)
            team = self.right_team()
        else:
            position = (WINWIDTH//2, 21)
            team = self.left_team()
        color = self.get_color(team)
        anim_nid = 'NoDamage%s' % color.capitalize()
        animation = RESOURCES.animations.get(anim_nid)
        if animation:
            anim = Animation(animation, position)
            self.animations.append(anim)
        # Also offset battle animation by lr_offset
        self.current_battle_anim.lr_offset = [-1, -2, -3, -2, -1]

    def glancing_hit(self):
        if self.current_battle_anim is self.right_battle_anim or self.current_battle_anim is self.rp_battle_anim:
            position = (64, 21)
            team = self.right_team()
        else:
            position = (WINWIDTH//2, 21)
            team = self.left_team()
        color = self.get_color(team)
        anim_nid = 'GlancingHit%s' % color.capitalize()
        animation = RESOURCES.animations.get(anim_nid)
        if animation:
            anim = Animation(animation, position)
            self.animations.append(anim)
        # Also offset battle animation by lr_offset
        self.current_battle_anim.lr_offset = [-1, -3, -5, -3, -1]

    def miss_anim(self):
        if self.current_battle_anim is self.right_battle_anim:
            position = (72, 21)
            team = self.right_team()
        else:
            position = (128, 21)  # Enemy's position
            team = self.left_team()
        color = self.get_color(team)
        anim_nid = 'Miss%s' % color.capitalize()
        animation = RESOURCES.animations.get(anim_nid)
        if animation:
            anim = Animation(animation, position)
            self.animations.append(anim)

    def shake(self):
        if self.get_damage() > 0:
            self._shake(1)
        else:
            self._shake(2)

    def _shake(self, num):
        self.current_shake = 1
        if num == 1: # Normal Hit
            self.shake_set = [(3, 3), (0, 0), (0, 0), (-3, -3), (0, 0), (0, 0), (3, 3), (0, 0), (-3, -3), (0, 0),
                              (3, 3), (0, 0), (-3, -3), (3, 3), (0, 0)]
        elif num == 2: # No Damage
            self.shake_set = [(1, 1), (1, 1), (1, 1), (-1, -1), (-1, -1), (-1, -1), (0, 0)]
        elif num == 3: # Spell Hit
            self.shake_set = [(0, 0), (-3, -3), (0, 0), (0, 0), (0, 0), (3, 3), (0, 0), (0, 0), (-3, -3), (0, 0),
                              (0, 0), (3, 3), (0, 0), (-3, -3), (0, 0), (3, 3), (0, 0), (-3, -3), (3, 3), (3, 3),
                              (0, 0)]
        elif num == 4: # Critical Hit
            self.shake_set = [(-6, -6), (0, 0), (0, 0), (0, 0), (6, 6), (0, 0), (0, 0), (-6, -6), (0, 0), (0, 0),
                              (6, 6), (0, 0), (-6, -6), (0, 0), (6, 6), (0, 0), (4, 4), (0, 0), (-4, -4), (0, 0),
                              (4, 4), (0, 0), (-4, -4), (0, 0), (4, 4), (0, 0), (-2, -2), (0, 0), (2, 2), (0, 0),
                              (-2, -2), (0, 0), (2, 2), (0, 0), (-1, -1), (0, 0), (1, 1), (0, 0)]

    def platform_shake(self):
        self.platform_current_shake = 1
        self.platform_shake_set = [(0, 1), (0, 0), (0, -1), (0, 0), (0, 1), (0, 0), (-1, -1), (0, 1), (0, 0)]

    def screen_flash(self, num_frames, color, fade_out=0):
        self.foreground.flash(num_frames, fade_out, color)

    def darken(self):
        self.target_dark += 0.5

    def lighten(self):
        self.target_dark -= 0.5

    def darken_ui(self):
        self.darken_ui_background = 1

    def lighten_ui(self):
        self.darken_ui_background = -3

    def pan_away(self):
        self.focus_right = not self.focus_right
        self.move_camera()

    def pan_back(self):
        self.focus_right = True
        self.move_camera()

    def focus_exp(self):
        self.focus_right = True

    def move_camera(self):
        if self.focus_right and self.pan_offset != -self.pan_max:
            self.pan_dir = -self.pan_move
        elif not self.focus_right and self.pan_offset != self.pan_max:
            self.pan_dir = self.pan_move

    def color_ui(self, combat_surf):
        if self.darken_ui_background:
            self.darken_ui_background = min(self.darken_ui_background, 4)
            color = 255 - abs(self.darken_ui_background * 24)
            engine.fill(combat_surf, (color, color, color), None, engine.BLEND_RGB_MULT)
            self.darken_ui_background += 1

    def draw_ui(self, surf) -> tuple:
        platform_trans = WINHEIGHT - 72
        platform_top = WINHEIGHT - 72
        if self.darken_background or self.target_dark:
            bg = image_mods.make_translucent(SPRITES.get('bg_black'), 1 - self.darken_background)
            surf.blit(bg, (0, 0))
            if self.target_dark > self.darken_background:
                self.darken_background += 0.125
            elif self.target_dark < self.darken_background:
                self.darken_background -= 0.125
        # Pan
        if self.pan_dir != 0:
            self.pan_offset += self.pan_dir
            if self.pan_offset > self.pan_max:
                self.pan_offset = self.pan_max
                self.pan_dir = 0
            elif self.pan_offset < -self.pan_max:
                self.pan_offset = -self.pan_max
                self.pan_dir = 0

        total_shake_x = self.shake_offset[0] + self.platform_shake_offset[0]
        total_shake_y = self.shake_offset[1] + self.platform_shake_offset[1]
        # Platform
        from app.data.database.database import DB
        if not self.battle_background or DB.constants.value('battle_platforms'):
            top = platform_top + (platform_trans - self.platform_offset * platform_trans) + total_shake_y
            if self.at_range:
                surf.blit(self.left_platform, (WINWIDTH // 2 - self.left_platform.get_width() - 11 - self.pan_max + total_shake_x + self.pan_offset, top))
                surf.blit(self.right_platform, (WINWIDTH // 2 + 11 + self.pan_max + total_shake_x + self.pan_offset, top))
            else:
                surf.blit(self.left_platform, (WINWIDTH // 2 - self.left_platform.get_width() + total_shake_x, top))
                surf.blit(self.right_platform, (WINWIDTH // 2 + total_shake_x, top))
        # Animation
        if self.at_range:
            right_range_offset = 24 + self.pan_max
            left_range_offset = -24 - self.pan_max
        else:
            right_range_offset, left_range_offset = 0, 0
        return left_range_offset, right_range_offset, total_shake_x, total_shake_y

    def draw_anims(self, surf):
        self.animations = [anim for anim in self.animations if not anim.update()]
        for anim in self.animations:
            anim.draw(surf)

    def draw_damage_numbers(self, surf, offsets):
        left_range_offset, right_range_offset, total_shake_x, total_shake_y = offsets
        for damage_num in self.damage_numbers:
            damage_num.update()
            if damage_num.left:
                x_pos = WINWIDTH//2 - 26 + left_range_offset - total_shake_x + self.pan_offset
            else:
                x_pos = WINWIDTH//2 + 26 + right_range_offset - total_shake_x + self.pan_offset
            damage_num.draw(surf, (x_pos, WINHEIGHT - 120))
        self.damage_numbers = [d for d in self.damage_numbers if not d.done]

    def draw(self, surf):
        self.bg.draw(surf)
        left_range_offset, right_range_offset, total_shake_x, total_shake_y = \
            self.draw_ui(surf)
        shake = (-total_shake_x, total_shake_y)

        lp_range_offset = left_range_offset - 20
        rp_range_offset = right_range_offset + 20
        self.left_battle_anim.draw_under(surf, shake, left_range_offset, self.pan_offset)
        self.right_battle_anim.draw_under(surf, shake, right_range_offset, self.pan_offset)
        if self.lp_battle_anim:
            self.lp_battle_anim.draw(surf, shake, lp_range_offset, self.pan_offset)
        self.left_battle_anim.draw(surf, shake, left_range_offset, self.pan_offset)
        if self.rp_battle_anim:
            self.rp_battle_anim.draw(surf, shake, rp_range_offset, self.pan_offset)
        self.right_battle_anim.draw(surf, shake, right_range_offset, self.pan_offset)
        self.right_battle_anim.draw_over(surf, shake, right_range_offset, self.pan_offset)
        self.left_battle_anim.draw_over(surf, shake, left_range_offset, self.pan_offset)

        self.draw_anims(surf)
        self.draw_damage_numbers(surf, (left_range_offset, right_range_offset, total_shake_x, total_shake_y))

        self.foreground.draw(surf)
        return surf
