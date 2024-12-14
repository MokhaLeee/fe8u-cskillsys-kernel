import random
from functools import lru_cache

import app.utilities as utils
from app.constants import (TILEHEIGHT, TILEWIDTH, TILEX, TILEY, WINHEIGHT,
                           WINWIDTH)
from app.data.database.database import DB
from app.engine import (combat_calcs, engine, icons, item_system,
                        skill_system)
from app.engine.fonts import FONT
from app.engine.game_counters import ANIMATION_COUNTERS
from app.engine.game_state import game
from app.engine.sound import get_sound_thread
from app.engine.sprites import SPRITES


class HealthBar():
    time_for_change_min = 200
    speed = utils.frames2ms(1)   # 1 frame for each hp point

    def __init__(self, unit):
        self.unit = unit

        self.displayed_hp = self.unit.get_hp()
        self.old_hp = self.displayed_hp

        self.transition_flag = False
        self.time_for_change = self.time_for_change_min
        self.last_update = 0

    def set_hp(self, val):
        self.displayed_hp = val

    def update(self):
        # print(self.displayed_hp, self.unit.get_hp(), self.transition_flag)
        # Check to see if we should begin showing transition
        if self.displayed_hp != self.unit.get_hp() and not self.transition_flag:
            self.transition_flag = True
            self.time_for_change = max(self.time_for_change_min, abs(self.displayed_hp - self.unit.get_hp()) * self.speed)
            self.last_update = engine.get_time()

        # Check to see if we should update
        if self.transition_flag:
            time = (engine.get_time() - self.last_update) / self.time_for_change
            new_val = int(utils.lerp(self.old_hp, self.unit.get_hp(), time))
            self.set_hp(new_val)
            if time >= 1:
                self.set_hp(self.unit.get_hp())
                self.old_hp = self.displayed_hp
                self.transition_flag = False

MAX_HP_PER_BAR = 40
class CombatHealthBar(HealthBar):
    colors = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1]
    speed = utils.frames2ms(2)
    time_for_change_min = 0

    def __init__(self, unit):
        super().__init__(unit)
        self.full_hp_blip = SPRITES.get('full_hp_blip')
        self.empty_hp_blip = SPRITES.get('empty_hp_blip')
        self.overflow_hp_blip = SPRITES.get('overflow_hp_blip')
        self.overflowpurple_hp_blip = SPRITES.get('overflowpurple_hp_blip')
        self.end_full_hp_blip = engine.subsurface(self.full_hp_blip, (0, 0, 1, self.full_hp_blip.get_height()))
        self.end_damaged_hp_blip = engine.subsurface(self.empty_hp_blip, (0, 0, 1, self.empty_hp_blip.get_height()))
        self.color_tick = 0
        self.heal_sound_update = 0

    def update(self, skip=False):
        if self.displayed_hp < self.unit.get_hp():
            self.speed = utils.frames2ms(4)  # Slower speed when increasing hp
        else:
            self.speed = utils.frames2ms(2)
        super().update()
        self.color_tick = int(engine.get_time() / 16.67) % len(self.colors)

    def set_hp(self, val):
        current_time = engine.get_time()
        if self.displayed_hp < self.unit.get_hp() and current_time - self.heal_sound_update > self.speed:
            self.heal_sound_update = current_time
            get_sound_thread().stop_sfx('HealBoop')
            get_sound_thread().play_sfx('HealBoop')
        super().set_hp(val)

    def big_number(self) -> bool:
        return self.displayed_hp != self.unit.get_hp()

    def done(self) -> bool:
        return self.displayed_hp == self.unit.get_hp()

    @lru_cache(1)
    def _create_hp_bar_surf(self, full_hp: int, actual_hp: int, overflow: int = 0) -> engine.Surface:
        """Creates a single hp bar row.
        """
        surf = engine.create_surface((full_hp * 2 + 1, self.full_hp_blip.get_height()), transparent=True)
        if overflow > 2:
            hp_blip = engine.subsurface(self.overflowpurple_hp_blip, (self.colors[self.color_tick] * 2, 0, 2, self.overflowpurple_hp_blip.get_height()))
            damage_blip = hp_blip
        elif overflow == 2:
            hp_blip = engine.subsurface(self.overflowpurple_hp_blip, (self.colors[self.color_tick] * 2, 0, 2, self.overflowpurple_hp_blip.get_height()))
            damage_blip = engine.subsurface(self.overflow_hp_blip, (self.colors[self.color_tick] * 2, 0, 2, self.overflow_hp_blip.get_height()))
        elif overflow == 1:
            hp_blip = engine.subsurface(self.overflow_hp_blip, (self.colors[self.color_tick] * 2, 0, 2, self.overflow_hp_blip.get_height()))
            damage_blip = engine.subsurface(self.full_hp_blip, (self.colors[self.color_tick] * 2, 0, 2, self.full_hp_blip.get_height()))
        else:
            hp_blip = engine.subsurface(self.full_hp_blip, (self.colors[self.color_tick] * 2, 0, 2, self.full_hp_blip.get_height()))
            damage_blip = self.empty_hp_blip
        for idx in range(actual_hp):
            surf.blit(hp_blip, (idx * 2, 0))
        for idx in range(full_hp - actual_hp):
            surf.blit(damage_blip, ((idx + actual_hp) * 2, 0))
        if full_hp == actual_hp:
            end_blip = engine.subsurface(hp_blip, (0, 0, 1, hp_blip.get_height()))
        else:
            end_blip = engine.subsurface(damage_blip, (0, 0, 1, damage_blip.get_height()))
        surf.blit(end_blip, (full_hp * 2, 0))
        return surf

    @lru_cache(1)
    def _create_double_hp_bar_surf(self, full_hp: int, actual_hp: int, overflow: int = 0) -> engine.Surface:
        """Creates two HP bar rows stacked on top of one another"""
        surf = engine.create_surface((MAX_HP_PER_BAR * 2 + 1, 2 * self.full_hp_blip.get_height()), True)
        full_hp = min(full_hp, 2 * MAX_HP_PER_BAR)

        bottom_full_hp = min(full_hp, MAX_HP_PER_BAR)
        bottom_actual_hp = min(actual_hp, MAX_HP_PER_BAR)
        bottom_bar = self._create_hp_bar_surf(bottom_full_hp, bottom_actual_hp, overflow)
        surf.blit(bottom_bar, (0, self.full_hp_blip.get_height()))

        top_full_hp = max(full_hp - MAX_HP_PER_BAR, 0)
        top_actual_hp = max(actual_hp - MAX_HP_PER_BAR, 0)
        top_bar = self._create_hp_bar_surf(top_full_hp, top_actual_hp, overflow)
        surf.blit(top_bar, (0, 0))
        return surf

    def draw(self, surf, left, top):
        font = FONT['number_small2']
        if self.big_number():
            font = FONT['number_big2']
        if self.displayed_hp <= 240:
            font.blit_right(str(self.displayed_hp), surf, (left, top - 4))
        else:
            font.blit_right('??', surf, (left, top - 4))
        max_hp = self.unit.get_max_hp()
        curr_hp = self.displayed_hp
        if max_hp <= MAX_HP_PER_BAR:
            hp_bar = self._create_hp_bar_surf(max_hp, curr_hp)
            surf.blit(hp_bar, (left + 5, top + 1))
        else:
            overflow_level = 0
            while curr_hp > 2 * MAX_HP_PER_BAR:
                overflow_level += 1
                curr_hp -= 2 * MAX_HP_PER_BAR
            double_hp_bars = self._create_double_hp_bar_surf(max_hp, curr_hp, overflow_level)
            surf.blit(double_hp_bars, (left + 5, top - 4))

class MapHealthBar(HealthBar):
    time_for_change_min = 200
    speed = utils.frames2ms(1)
    health_outline = SPRITES.get('map_health_outline')
    health_bar = SPRITES.get('map_health_bar')

    def draw(self, surf, left, top):
        total = max(1, self.unit.get_max_hp())
        fraction_hp = utils.clamp(self.displayed_hp / total, 0, 1)
        index_pixel = int(12 * fraction_hp) + 1

        surf.blit(self.health_outline, (left, top + 13))
        if fraction_hp > 0:
            bar = engine.subsurface(self.health_bar, (0, 0, index_pixel, 1))
            surf.blit(bar, (left + 1, top + 14))

        return surf

class MapCombatHealthBar(HealthBar):
    display_numbers = True
    health_bar = SPRITES.get('health_bar')

    def draw(self, surf):
        total = max(1, self.unit.get_max_hp())
        fraction_hp = utils.clamp(self.displayed_hp / total, 0, 1)
        index_pixel = int(50 * fraction_hp)
        position = 25, 22
        surf.blit(engine.subsurface(self.health_bar, (0, 0, index_pixel, 2)), position)

        # Blit HP number
        if self.display_numbers:
            font = FONT['number_small2']
            if self.transition_flag:
                font = FONT['number_big2']
            s = str(self.displayed_hp)
            position = 22 - font.size(s)[0], 15
            font.blit(s, surf, position)

        return surf

class MapCombatInfo():
    blind_speed = 1/8.  # 8 frames to fade in

    def __init__(self, draw_method, unit, item, target, stats):
        self.skill_icons = []
        self.ordering = None

        self.reset()
        self.change_unit(unit, item, target, stats, draw_method)
        # self.fade_in()

    def change_unit(self, unit, item, target=None, stats=None, draw_method=None):
        if draw_method:
            self.draw_method = draw_method
            self.true_position = None

        if unit != self.unit or target != self.target:
            self.fade_in()
        else:
            self.blinds = 1

        self.hp_bar = MapCombatHealthBar(unit)
        self.unit = unit
        self.item = item
        if target:
            self.target = target
        if stats:
            self.hit = stats[0]
            self.mt = stats[1]

        self.skill_icons.clear()

        # Handle surfaces
        team = DB.teams.get(unit.team)

        self.stats_surf = None
        self.bg_surf = SPRITES.get('health_' + team.combat_color).convert_alpha()
        self.c_surf = SPRITES.get('combat_stats_' + team.combat_color).convert_alpha()
        self.gem = SPRITES.get('combat_gem_' + team.combat_color).convert_alpha()

    def reset(self):
        self.draw_method = None
        self.true_position = None

        self.hp_bar = None
        self.unit = None
        self.item = None
        self.target = None
        self.hit = None
        self.mt = None

        self.blinds = 1
        self.reset_shake()

        self.stats_surf = None
        self.bg_surf = None
        self.c_surf = None
        self.gem = None

        self.skill_icons.clear()

    def fade_in(self):
        self.blinds = 0

    def fade_out(self):
        pass

    def shake(self, num):
        self.current_shake_idx = 1
        if num == 1:  # Normal hit
            self.shake_set = [(-3, -3), (0, 0), (3, 3), (0, 0)]
        elif num == 2:  # Kill
            self.shake_set = [(3, 3), (0, 0), (0, 0), (3, 3), (-3, -3), (3, 3), (-3, -3), (0, 0)]
        elif num == 3:  # Crit
            self.shake_set = [(random.randint(-4, 4), random.randint(-4, 4)) for _ in range(16)] + [(0, 0)]
        elif num == 4:  # Glancing hit
            self.shake_set = [(-1, -1), (0, 0), (1, 1), (0, 0)]

    def reset_shake(self):
        self.shake_set = [(0, 0)]  # How the hp bar will shake
        self.shake_offset = (0, 0)  # How it is currently shaking
        self.current_shake_idx = 0

    def handle_shake(self):
        if self.current_shake_idx:
            self.shake_offset = self.shake_set[self.current_shake_idx - 1]
            self.current_shake_idx += 1
            if self.current_shake_idx > len(self.shake_set):
                self.current_shake_idx = 0

    def add_skill_icon(self, skill_icon):
        self.skill_icons.append(skill_icon)

    def build_stat_surf(self):
        stat_surf = self.c_surf.copy()
        # Blit hit
        if self.hit is not None:
            hit = str(utils.clamp(self.hit, 0, 100))
        else:
            hit = '--'
        position = stat_surf.get_width() // 2 - FONT['number_small2'].size(hit)[0] - 1, -2
        FONT['number_small2'].blit(hit, stat_surf, position)
        # Blit damage
        if self.mt is not None:
            damage = str(max(0, self.mt))
        else:
            damage = '--'
        position = stat_surf.get_width() - FONT['number_small2'].size(damage)[0] - 2, -2
        FONT['number_small2'].blit(damage, stat_surf, position)
        return stat_surf

    def get_time_for_change(self):
        return self.hp_bar.time_for_change

    def force_position_update(self):
        if self.unit:
            width, height = self.bg_surf.get_width(), self.bg_surf.get_height()
            self.determine_position(width, height)

    def determine_position(self, width, height):
        self.true_position = self.draw_method
        if self.draw_method in ('p1', 'p2'):
            pos1 = self.unit.position
            pos2 = self.target.position
            camera_pos = game.camera.get_xy()
            if self.draw_method == 'p1':
                left = True if pos1[0] <= pos2[0] else False
            else:
                left = True if pos1[0] < pos2[0] else False
            self.ordering = 'left' if left else 'right'

            x_pos = WINWIDTH//2 - width if left else WINWIDTH//2
            rel_1 = pos1[1] - camera_pos[1]
            rel_2 = pos2[1] - camera_pos[1]

            # If both are on top of screen
            if rel_1 < TILEY//2 and rel_2 < TILEY//2:
                rel = max(rel_1, rel_2)
                y_pos = (rel + 1) * TILEHEIGHT + 12
            # If both are on bottom of screen
            elif rel_1 >= TILEY//2 and rel_2 >= TILEY//2:
                rel = min(rel_1, rel_2)
                y_pos = rel * TILEHEIGHT - 12 - height - 13  # Stat surf
            # Find largest gap and place it in the middle
            else:
                top_gap = min(rel_1, rel_2)
                bottom_gap = TILEY - 1 - max(rel_1, rel_2)
                middle_gap = abs(rel_1 - rel_2)
                if top_gap > bottom_gap and top_gap > middle_gap:
                    y_pos = top_gap * TILEHEIGHT - 12 - height - 13  # Stat surf
                elif bottom_gap > top_gap and bottom_gap > middle_gap:
                    y_pos = (TILEY - 1 - bottom_gap//2) * TILEHEIGHT - 12
                else:
                    y_pos = WINHEIGHT//4 - height//2 - 13//2 if rel_1 < TILEY//2 else 3*WINHEIGHT//4 - height//2 - 13//2
                    x_pos = WINWIDTH//4 - width//2 if pos1[0] - camera_pos[0] > TILEX//2 else 3*WINWIDTH//4 - width//2
                    self.ordering = 'middle'
            self.true_position = (x_pos, y_pos)

        elif self.draw_method == 'splash':
            pos = self.unit.sprite.position
            if pos:
                x_pos = pos[0] - game.camera.get_x()
                x_pos = utils.clamp(x_pos, 3, TILEX - 2)
                if pos[1] - game.camera.get_y() < TILEY//2:
                    y_pos = pos[1] - game.camera.get_y() + 2
                else:
                    y_pos = pos[1] - game.camera.get_y() - 3
                self.true_position = (x_pos * TILEWIDTH - width//2, y_pos * TILEHEIGHT - 8)
            else:
                x_pos = WINWIDTH//2 - width//2
                y_pos = WINHEIGHT//2 - height//2
                self.true_position = (x_pos, y_pos)
            self.ordering = 'middle'

    def update_stats(self, stats):
        self.hit, self.mt = stats
        self.stats_surf = None

    def update(self):
        # Make blinds wider
        self.blinds = utils.clamp(self.blinds, self.blinds + self.blind_speed, 1)

        if self.unit and self.blinds >= 1:
            self.handle_shake()
            self.hp_bar.update()

    def draw(self, surf):
        # Create background surface
        width, height = self.bg_surf.get_width(), self.bg_surf.get_height()
        true_height = height + self.c_surf.get_height()
        if self.hit or self.mt:
            bg_surf = engine.create_surface((width, true_height))
        else:
            bg_surf = engine.create_surface((width, height))
        bg_surf.blit(self.bg_surf, (0, 0))

        # Name
        name_width = FONT['text_numbers'].size(self.unit.name)[0]
        position = width - name_width - 4, 3
        FONT['text_numbers'].blit(self.unit.name, bg_surf, position)

        # Item
        if self.item:
            # Determine effectiveness
            icon = icons.get_icon(self.item)
            if icon:
                icon = item_system.item_icon_mod(self.unit, self.item, self.target, icon)
                bg_surf.blit(icon, (2, 3))

            # Blit advantage
            if skill_system.check_enemy(self.unit, self.target):
                adv = combat_calcs.compute_advantage(self.unit, self.target, self.item, self.target.get_weapon())
                disadv = combat_calcs.compute_advantage(self.unit, self.target, self.item, self.target.get_weapon(), False)

                up_arrow = engine.subsurface(SPRITES.get('arrow_advantage'), (ANIMATION_COUNTERS.arrow_counter.count * 7, 0, 7, 10))
                down_arrow = engine.subsurface(SPRITES.get('arrow_advantage'), (ANIMATION_COUNTERS.arrow_counter.count * 7, 10, 7, 10))

                if adv and adv.modification > 0:
                    bg_surf.blit(up_arrow, (11, 7))
                elif adv and adv.modification < 0:
                    bg_surf.blit(down_arrow, (11, 7))
                elif disadv and disadv.modification > 0:
                    bg_surf.blit(down_arrow, (11, 7))
                elif disadv and disadv.modification < 0:
                    bg_surf.blit(up_arrow, (11, 7))
        # End item

        bg_surf = self.hp_bar.draw(bg_surf)

        # Blit stat surf
        if self.hit is not None or self.mt is not None:
            if not self.stats_surf:
                self.stats_surf = self.build_stat_surf()
            bg_surf.blit(self.stats_surf, (0, height))

        if not self.true_position or self.draw_method == 'splash':
            self.determine_position(width, height)

        if self.hit is not None or self.mt is not None:
            blit_surf = engine.subsurface(bg_surf, (0, true_height//2 - int(true_height * self.blinds // 2), width, int(true_height * self.blinds)))
            y_pos = self.true_position[1] + true_height//2 - int(true_height * self.blinds // 2)
        else:
            blit_surf = engine.subsurface(bg_surf, (0, height//2 - int(height * self.blinds // 2), width, int(height * self.blinds)))
            y_pos = self.true_position[1] + height//2 - int(height * self.blinds // 2)
        x, y = (self.true_position[0] + self.shake_offset[0], y_pos + self.shake_offset[1])
        surf.blit(blit_surf, (x, y))

        # Gem
        if self.blinds >= 1 and self.gem and self.ordering:
            if self.ordering == 'left':
                position = (x + 2, y - 3)
            elif self.ordering == 'right':
                position = (x + 56, y - 3)
            elif self.ordering == 'middle':
                position = (x + 27, y - 3)
            surf.blit(self.gem, position)

        # Draw skill icons
        for idx, skill_icon in enumerate(self.skill_icons):
            skill_icon.update()
            x, y = self.true_position[0] + width // 2, self.true_position[1] - 16 + idx * 16
            skill_icon.draw(surf, (x, y))
        self.skill_icons = [s for s in self.skill_icons if not s.done]

        return surf
