import math

from typing import Callable, List, Tuple, TYPE_CHECKING
from app.utilities.typing import NID

from app.constants import TILEWIDTH, TILEHEIGHT, WINWIDTH, WINHEIGHT
from app.data.resources.resources import RESOURCES
from app.data.database.database import DB

from app.utilities import utils
from app.engine import engine, image_mods, icons, unit_funcs, \
    action, banner, skill_system, exp_funcs
from app.engine.sprites import SPRITES
from app.engine.sound import get_sound_thread
from app.engine.fonts import FONT
from app.events import triggers
from app.engine.state import State
from app.engine.state_machine import SimpleStateMachine
from app.engine.animations import Animation
from app.engine.game_state import game
from app.engine.graphics.text.text_renderer import render_text
from app.utilities.enums import HAlignment

from app.engine.objects.unit import UnitObject
from app.engine.source_type import SourceType

class ExpState(State):
    name = 'exp'
    transparent = True
    state = None

    dark_fuzz_background = image_mods.make_translucent(SPRITES.get('bg_black'), 0.66)

    def start(self):
        if game.exp_instance:
            self.unit, self.exp_gain, self.combat_object, self.starting_state = \
                game.exp_instance.pop()
        else:
            game.state.back()
            return 'repeat'
        # Check if we need to use a custom method
        self.method = None
        if game.memory.get('exp_method'):
            self.method = game.memory['exp_method']
            game.memory['exp_method'] = None

        self.old_exp = self.unit.exp
        self.old_level = self.unit.level
        self.unit_klass = DB.classes.get(self.unit.klass)
        self.auto_promote = exp_funcs.has_autopromote(self.unit)

        # For mana
        self.old_mana = self.unit.get_mana()
        self.max_mana = self.unit.get_max_mana()
        self.mana_to_gain = 0
        if game.mana_instance:
            mana_instances_for_unit = [instance for instance in game.mana_instance if instance[0].nid == self.unit.nid]
            for instance in mana_instances_for_unit:
                self.mana_to_gain += instance[1]
                game.mana_instance.remove(instance)
            if self.mana_to_gain + self.old_mana > self.max_mana:
                self.mana_to_gain = self.max_mana - self.old_mana
        self.mana_bar = None

        self.state = SimpleStateMachine(self.starting_state)
        self.start_time = engine.get_time()

        self.exp_bar = None
        self.level_up_animation = None
        self.level_up_screen = None

        if not self.auto_promote:
            max_exp = 100 * (self.unit_klass.max_level - self.old_level) - self.old_exp
            self.exp_gain = min(self.exp_gain, max_exp)

        self.total_time_for_exp = max(1, utils.frames2ms(abs(self.exp_gain)))  # 1 frame per exp

        self.stat_changes = None
        self.new_wexp = None

        if exp_funcs.can_give_exp(self.unit, self.exp_gain) or self.mana_to_gain or \
                self.starting_state in ('promote', 'class_change', 'stat_booster'):
            pass
        else:
            # We're done here, since the unit is at max level and has no stats to gain, mana to gain, or exp to lose
            game.state.back()
            return 'repeat'

        # determine source for trigger later
        # purposefully explicit here
        self.source = 'exp_gain'
        if self.starting_state == 'stat_booster':
            self.source = 'stat_change'
        elif self.starting_state == 'class_change':
            self.source = 'class_change'
        elif self.starting_state == 'promote':
            self.source = 'promote'

        self.level_up_sound_played = False

    def begin(self):
        game.cursor.hide()

    def end(self):
        # Just in case, make sure this doesn't last forever
        get_sound_thread().stop_sfx('Experience Gain')

    def create_level_up_logo(self):
        if self.combat_object:
            anim = RESOURCES.animations.get('LevelUpBattle')
            if anim:
                anim = Animation(anim, (0, 6))
                anim.set_tint_after_delay(40)
                self.level_up_animation = anim
        else:
            if self.unit.position:
                x, y = self.unit.position
                left = (x - game.camera.get_x() - 2) * TILEWIDTH
                top = (y - game.camera.get_y() - 1) * TILEHEIGHT
                pos = (left, top)
            else:
                pos = (WINWIDTH//2, WINHEIGHT//2)
            anim = RESOURCES.animations.get('LevelUpMap')
            if anim:
                anim = Animation(anim, pos)
                self.level_up_animation = anim

    def update(self):
        current_time = engine.get_time()

        # Initiating State
        if self.state.get_state() == 'init':
            self.exp_bar = ExpBar(self.old_exp, center=not self.combat_object)
            self.start_time = current_time

            if self.mana_to_gain:
                self.mana_bar = ManaBar(self.old_mana, center=not self.combat_object)
                self.mana_bar.bar_max = self.unit.get_max_mana()

            self.state.change('exp_wait')

        # Wait before starting to increment exp
        elif self.state.get_state() == 'exp_wait':
            self.exp_bar.update(self.old_exp)
            if self.mana_bar:
                self.mana_bar.update(self.old_mana)
            if current_time - self.start_time > 466:
                self.state.change('exp0')
                self.start_time = current_time
                get_sound_thread().play_sfx('Experience Gain', True)

        # Increment exp until done or 100 exp is reached
        elif self.state.get_state() == 'exp0':
            progress = (current_time - self.start_time)/float(self.total_time_for_exp)
            progress = utils.clamp(progress, 0, 1)
            exp_set = self.old_exp + progress * self.exp_gain
            self.exp_bar.update(exp_set)
            exp_set = int(exp_set)

            if self.mana_bar:
                mana_set = self.old_mana + progress * self.mana_to_gain
                self.mana_bar.update(mana_set)
                mana_set = int(mana_set)

            if exp_set >= self.old_exp + self.exp_gain:
                get_sound_thread().stop_sfx('Experience Gain')

            if exp_set >= 100:
                max_level = self.unit_klass.max_level
                if self.unit.level >= max_level:  # Do I promote?
                    get_sound_thread().stop_sfx('Experience Gain')
                    if self.auto_promote:
                        self.exp_bar.update(100)
                        get_sound_thread().play_sfx('Level Up')
                    else:
                        self.exp_bar.update(99)
                    self.state.clear()
                    self.state.change('prepare_promote')
                    self.state.change('exp_leave')
                    self.exp_bar.fade_out()
                    self.start_time = current_time
                else:
                    self.state.change('exp100')

            if exp_set < 0 and self.exp_gain < 0:
                if self.unit.level > 1:
                    self.state.change('exp-100')
                else:
                    get_sound_thread().stop_sfx('Experience Gain')

            if current_time - self.start_time >= self.total_time_for_exp + 500:
                get_sound_thread().stop_sfx('Experience Gain')  # Just in case
                self.state.clear()
                self.state.change('exp_leave')
                self.exp_bar.fade_out()
                self.start_time = current_time

        elif self.state.get_state() == 'exp_leave':
            done = self.exp_bar.update()
            if done:
                action.do(action.GainExp(self.unit, self.exp_gain))
                if self.mana_to_gain:
                    action.do(action.ChangeMana(self.unit, self.mana_to_gain))
                action.do(action.UpdateRecords('exp_gain', (self.unit.nid, self.exp_gain, self.unit.klass)))
                self.state.back()
                self.start_time = current_time
                # If we're ready to leave
                if len(self.state.state) <= 0:
                    game.state.back()
                # Otherwise, we're probably about to level up!

        elif self.state.get_state() == 'exp100':
            progress = (current_time - self.start_time)/float(self.total_time_for_exp)
            exp_set = self.old_exp + (self.exp_gain * progress) - 100
            exp_set = min(self.old_exp + self.exp_gain - 100, exp_set)
            self.exp_bar.update(exp_set)
            exp_set = int(exp_set)

            if exp_set >= self.old_exp + self.exp_gain - 100:
                get_sound_thread().stop_sfx('Experience Gain')

            # Extra time to account for pause at end
            if current_time - self.start_time >= self.total_time_for_exp + 333:
                old_growth_points = self.unit.growth_points.copy()
                self.stat_changes = unit_funcs.get_next_level_up(self.unit, self.unit.get_internal_level(), self.method)
                action.do(action.GrowthPointChange(self.unit, old_growth_points, self.unit.growth_points))
                action.do(action.IncLevel(self.unit))
                action.do(action.ApplyStatChanges(self.unit, self.stat_changes, False))
                action.do(action.UpdateRecords('level_gain', (self.unit.nid, self.unit.level, self.unit.klass)))
                self.create_level_up_logo()
                self.state.clear()
                self.state.change('level_up')
                self.state.change('exp_leave')
                self.exp_bar.fade_out()
                self.start_time = current_time

        elif self.state.get_state() == 'exp-100':
            progress = (current_time - self.start_time)/float(self.total_time_for_exp)
            exp_set = 100 + self.old_exp + (self.exp_gain * progress)
            exp_set = max(100 + self.old_exp + self.exp_gain, exp_set)
            self.exp_bar.update(exp_set)
            exp_set = int(exp_set)

            if exp_set <= 100 + self.old_exp + self.exp_gain:
                get_sound_thread().stop_sfx('Experience Gain')

            # Extra time to account for pause at end
            if current_time - self.start_time >= self.total_time_for_exp + 333:
                old_growth_points = self.unit.growth_points.copy()
                stat_changes = unit_funcs.get_next_level_up(self.unit, self.unit.get_internal_level() - 1, self.method)
                self.stat_changes = {nid: -value for nid, value in stat_changes.items()}  # Make negative
                action.do(action.GrowthPointChange(self.unit, old_growth_points, self.unit.growth_points))
                action.do(action.SetLevel(self.unit, self.unit.level - 1))
                action.do(action.ApplyStatChanges(self.unit, self.stat_changes, False))
                self.create_level_up_logo()
                self.state.clear()
                self.state.change('level_up')
                self.state.change('exp_leave')
                self.exp_bar.fade_out()
                self.start_time = current_time

        elif self.state.get_state() == 'level_up':
            if not self.level_up_sound_played:
                get_sound_thread().play_sfx('Level Up')
                self.level_up_sound_played = True

            if self.level_up_animation.update():
                if self.combat_object:
                    self.combat_object.darken_ui()
                self.state.change('level_screen')
                self.start_time = current_time

        elif self.state.get_state() == 'level_screen':
            if not self.level_up_screen:
                self.level_up_screen = LevelUpScreen(
                    self, self.unit, self.stat_changes, self.old_level, self.unit.level)
            if self.level_up_screen.update(current_time):
                game.state.back()
                game.events.trigger(triggers.UnitLevelUp(self.unit, self.stat_changes, self.source))
                if self.combat_object:
                    self.combat_object.lighten_ui()

                # check for weapon experience gain
                if self.new_wexp:
                    for weapon_nid, value in self.new_wexp.items():
                        action.do(action.AddWexp(self.unit, weapon_nid, value))

                # check for skill gain unless the unit is using a booster to
                # get to this screen
                if self.starting_state != "stat_booster":
                    ExpState.give_new_personal_skills(self.unit)
                    if self.starting_state == 'class_change':
                        if DB.constants.value('learn_skills_on_reclass'):
                            ExpState.give_all_class_skills(self.unit)
                    elif self.starting_state == 'promote':
                        if DB.constants.value('learn_skills_on_promote'):
                            ExpState.give_all_class_skills(self.unit)
                    else:
                        ExpState.give_new_class_skills(self.unit)

        # Wait 100 ms before transferring to the promotion state
        elif self.state.get_state() == 'prepare_promote':
            self.exp_bar.update()
            if current_time - self.start_time > 100:
                class_options = self.unit_klass.turns_into
                if self.auto_promote:
                    self.exp_bar.update(0)
                    if len(class_options) > 1:
                        game.memory['current_unit'] = self.unit
                        game.state.change('promotion_choice')
                        game.state.change('transition_out')
                        # We are leaving
                        self.state.clear()
                        self.state.change('wait')
                        self.start_time = current_time
                    elif len(class_options) == 1:
                        game.memory['current_unit'] = self.unit
                        game.memory['next_class'] = class_options[0]
                        game.state.change('promotion')
                        game.state.change('transition_out')  # We are leaving
                        self.state.clear()
                        self.state.change('wait')
                        self.start_time = current_time
                    else:
                        action.do(action.SetExp(self.unit, 99))
                        game.state.back()
                else:
                    action.do(action.SetExp(self.unit, 99))
                    game.state.back()

        elif self.state.get_state() in ('promote', 'class_change'):
            if self.state.get_state() == 'promote':
                promote_action = action.Promote(self.unit, game.memory['next_class'])
            else:
                promote_action = action.ClassChange(self.unit, game.memory['next_class'])
            self.stat_changes, self.new_wexp = promote_action.get_data()
            action.do(promote_action)
            action.do(action.UpdateRecords('level_gain', (self.unit.nid, self.unit.level, self.unit.klass)))

            if self.combat_object:
                self.combat_object.darken_ui()

            self.state.clear()
            self.state.change('level_screen')
            self.start_time = current_time

        elif self.state.get_state() == 'stat_booster':
            self.stat_changes = game.memory['stat_changes']
            self.exp_gain = 0
            self.old_level = self.unit.level
            self.state.change('level_screen')
            self.start_time = current_time

        elif self.state.get_state() == 'wait':
            if current_time - self.start_time > 1000:  # Wait a while
                game.state.back()

    def draw(self, surf):
        if not self.state:
            return surf

        if self.state.get_state() in ('init', 'exp_wait', 'exp_leave', 'exp0', 'exp100', 'exp-100', 'prepare_promote'):
            if self.mana_bar:
                self.mana_bar.draw(surf)
            if self.exp_bar:
                self.exp_bar.draw(surf)

        elif self.state.get_state() == 'level_up':
            surf.blit(self.dark_fuzz_background, (0, 0))
            if self.level_up_animation:
                self.level_up_animation.update()
                self.level_up_animation.draw(surf)

        elif self.state.get_state() == 'level_screen':
            if self.level_up_screen:
                self.level_up_screen.draw(surf)

        return surf

    @staticmethod
    def _give_skills(unit: UnitObject, avail_skills: List[Tuple[int, NID]],
                     comparison_func: Callable[[UnitObject, int], bool], source_type):
        for level_needed, skill_nid in avail_skills:
            if comparison_func(unit, level_needed):
                if skill_nid == 'Feat':
                    game.memory['current_unit'] = unit
                    game.state.change('feat_choice')
                else:
                    if skill_nid not in [skill.nid for skill in unit.skills]:
                        if source_type == SourceType.KLASS:
                            act = action.AddSkill(unit, skill_nid, source=unit.klass, source_type=SourceType.KLASS)
                        else:
                            act = action.AddSkill(unit, skill_nid, source=unit.nid, source_type=SourceType.PERSONAL)
                        action.do(act)
                        if act.skill_obj and not skill_system.hidden(act.skill_obj, unit):
                            game.alerts.append(banner.GiveSkill(unit, act.skill_obj))
                            game.state.change('alert')

    @staticmethod
    def give_new_class_skills(unit: UnitObject):
        def compare(unit, level_needed):
            return unit.level == level_needed

        unit_klass = DB.classes.get(unit.klass)
        ExpState._give_skills(unit, unit_klass.learned_skills, compare, source_type=SourceType.KLASS)

    @staticmethod
    def give_new_personal_skills(unit: UnitObject):
        def compare(unit, level_needed):
            return unit.get_internal_level() == level_needed

        unit_prefab = DB.units.get(unit.nid)
        if not unit_prefab:
            return
        ExpState._give_skills(unit, unit_prefab.learned_skills, compare, source_type=SourceType.PERSONAL)

    @staticmethod
    def give_all_class_skills(unit: UnitObject):
        def compare(unit, level_needed):
            return unit.level >= level_needed

        unit_klass = DB.classes.get(unit.klass)
        ExpState._give_skills(unit, unit_klass.learned_skills, compare, source_type=SourceType.KLASS)

class LevelUpScreen():
    bg = SPRITES.get('level_screen')
    bg = bg.convert_alpha()
    width = bg.get_width()
    height = bg.get_height()

    spark_time = 350
    level_up_wait = 1366

    underline = SPRITES.get('stat_underline')

    def __init__(self, parent, unit, stat_changes, old_level, new_level):
        self.parent = parent
        self.unit = unit
        self.stat_list = [stat_changes.get(nid, 0) for nid in DB.stats.keys()]
        self.stat_list = self.stat_list[:8]  # Can only show first 8 stats on level up
        self.old_level = old_level
        self.new_level = new_level

        self.current_spark = -1

        self.unit_scroll_offset = 80
        self.screen_scroll_offset = self.width + 32
        self.underline_offset = 36

        self.animations = []
        self.arrow_animations = []
        self.simple_nums = []

        self.state = 'scroll_in'
        self.start_time = 0

    def make_spark(self, topleft):
        anim = RESOURCES.animations.get('StatUpSpark')
        if anim:
            return Animation(anim, topleft)
        return None

    def topleft(self):
        return (6 - self.screen_scroll_offset, WINHEIGHT - 8 - self.height)

    def get_position(self, i, absolute=False):
        tl_offset = (0, 0)
        if absolute:
            tl_offset = self.topleft()
        if i >= 4:
            position = (self.width//2 + 8 + tl_offset[0], (i - 4) * 16 + 35 + tl_offset[1])
        else:
            position = (10 + tl_offset[0], i * 16 + 35 + tl_offset[1])
        return position

    def inc_spark(self):
        self.current_spark += 1
        if self.current_spark >= len(self.stat_list):
            return True
        elif self.stat_list[self.current_spark] == 0:
            return self.inc_spark()
        return False

    def update(self, current_time):
        if self.state == 'init_pause':
            if current_time - self.start_time > 268:
                self.state = 'scroll_in'
                self.start_time = current_time

        elif self.state == 'scroll_in':
            self.unit_scroll_offset = max(0, self.unit_scroll_offset - 8)
            self.screen_scroll_offset = max(0, self.screen_scroll_offset - 16)
            if self.unit_scroll_offset == 0 and self.screen_scroll_offset == 0:
                self.state = 'init_wait'
                self.start_time = current_time

        elif self.state == 'init_wait':
            if current_time - self.start_time > 500:
                if self.old_level == self.new_level:  # No level up spark
                    self.state = 'get_next_spark'
                else:
                    self.state = 'first_spark'
                    topleft = (87, 27)
                    self.animations.append(self.make_spark(topleft))
                    get_sound_thread().play_sfx('Level_Up_Level')
                self.start_time = current_time

        elif self.state == 'scroll_out':
            self.unit_scroll_offset += 10
            self.screen_scroll_offset += 20
            if current_time - self.start_time > 500:
                return True  # Done

        elif self.state == 'first_spark':
            if current_time - self.start_time > self.spark_time:
                self.state = 'get_next_spark'
                self.start_time = current_time

        elif self.state == 'get_next_spark':
            done = self.inc_spark()
            if done:
                game.events.trigger(triggers.DuringUnitLevelUp(self.unit, self.parent.stat_changes, self.parent.source))
                self.state = 'level_up_wait'
                self.start_time = current_time
            else:
                pos = self.get_position(self.current_spark)
                # Animations
                # Number
                increase = self.stat_list[self.current_spark]
                # Arrow
                if increase > 0:
                    anim = RESOURCES.animations.get('LevelUpArrow')
                elif increase < 0:
                    anim = RESOURCES.animations.get('LevelDownArrow')
                if anim:
                    arrow_animation = Animation(anim, (pos[0] + 45, pos[1] - 11), hold=True)
                    self.arrow_animations.append(arrow_animation)

                offset_pos = self.get_position(self.current_spark, True)
                # Spark
                spark_pos = offset_pos[0] + 8, offset_pos[1] - 19
                spark_anim = self.make_spark(spark_pos)
                if spark_anim:
                    self.animations.append(spark_anim)

                if increase > 0:
                    anim = RESOURCES.animations.get('LevelUpNumber' + str(increase))
                elif increase < 0:
                    anim = RESOURCES.animations.get('LevelDownNumber' + str(-increase))
                if anim:
                    number_animation = Animation(anim, (offset_pos[0] + 37, offset_pos[1] + 4), delay=80, hold=True)
                    self.animations.append(number_animation)
                else:
                    if increase > 0:
                        self.simple_nums.append(('stat', 'white', '+' + str(increase), (offset_pos[0] + 57, offset_pos[1] - 2), current_time))
                    elif increase < 0:
                        self.simple_nums.append(('stat', 'purple', str(increase), (offset_pos[0] + 57, offset_pos[1] - 2), current_time))

                get_sound_thread().play_sfx('Stat Up')
                self.underline_offset = 36 # for underline growing
                self.state = 'spark_wait'
                self.start_time = current_time

        elif self.state == 'spark_wait':
            if current_time - self.start_time > self.spark_time:
                self.state = 'get_next_spark'

        elif self.state == 'level_up_wait':
            if current_time - self.start_time > self.level_up_wait:
                self.animations.clear()
                self.simple_nums.clear()
                self.state = 'scroll_out'
                self.start_time = current_time

    def draw(self, surf):
        sprite = self.bg.copy()
        # Changes through entire color wheel
        # from t = 0 to t = 1 in
        # 180 * 10 milliseconds
        t = math.sin(math.radians((engine.get_time()//10) % 180))
        new_color = image_mods.blend_colors((88, 16, -40), (-80, -32, 40), t)

        # Render top
        klass = DB.classes.get(self.unit.klass)
        if FONT['text'].width(klass.name) > 60:
            FONT['narrow'].blit(klass.name, sprite, (12, 3))
        else:
            FONT['text'].blit(klass.name, sprite, (12, 3))
        FONT['text-yellow'].blit('Lv', sprite, (self.width//2 + 12, 3))
        if self.state in ('scroll_in', 'init_wait'):
            level = str(self.old_level)
        else:
            level = str(self.new_level)
        width = FONT['text-blue'].width(level)
        FONT['text-blue'].blit(level, sprite, (self.width//2 + 50 - width, 3))

        # Render underlines
        new_underline_surf = image_mods.change_color(self.underline, new_color)
        for idx, num in enumerate(self.stat_list[:self.current_spark + 1]):
            if num != 0:  # Stat change
                if idx == self.current_spark:
                    rect = (self.underline_offset, 0,
                            new_underline_surf.get_width() - self.underline_offset, 3)
                    new_underline_surf = engine.subsurface(new_underline_surf, rect)
                    # Change underline offset
                    self.underline_offset = max(0, self.underline_offset - 6)
                    pos = self.get_position(idx)
                    pos = (pos[0] + self.underline_offset//2 + 1, pos[1] + 10)
                else:
                    pos = self.get_position(idx)
                    pos = (pos[0] + 4, pos[1] + 11)
                sprite.blit(new_underline_surf, pos)

        # Update and draw arrow animations
        self.arrow_animations = [a for a in self.arrow_animations if not a.update()]
        for animation in self.arrow_animations:
            animation.draw(sprite, blend=new_color)

        # Draw stats
        for idx, stat in enumerate(DB.stats.keys()):
            # Can only show first 8 stats on levelup
            if idx >= len(self.stat_list):
                continue
            pos = self.get_position(idx)
            name = DB.stats.get(stat).name
            render_text(sprite, ['text'], [name], ['yellow'], pos)
            text = self.unit.stats[stat] - (self.stat_list[idx] if self.current_spark < idx else 0)
            width = FONT['text-blue'].width(str(text))
            FONT['text-blue'].blit(str(text), sprite, (pos[0] + 40 - width, pos[1]))

        pos = (6 - self.screen_scroll_offset, WINHEIGHT - 8 - self.height)
        surf.blit(sprite, pos)

        # Blit unit's pic
        right = WINWIDTH - 4
        bottom = WINHEIGHT + self.unit_scroll_offset
        icons.draw_portrait(surf, self.unit, bottomright=(right, bottom))

        # Update and draw animations
        self.animations = [a for a in self.animations if not a.update()]
        # offset = game.camera.get_x() * TILEWIDTH, game.camera.get_y() * TILEHEIGHT
        for animation in self.animations:
            animation.draw(surf)

        for font, color, text, pos, time in self.simple_nums:
            if engine.get_time() - time > 80:
                render_text(surf, [font], [text], [color], pos, align=HAlignment.RIGHT)

        return surf

class ExpBar():
    background = engine.subsurface(SPRITES.get('expbar'), (0, 0, 136, 24))
    begin = engine.subsurface(SPRITES.get('expbar'), (0, 24, 3, 7))
    middle = engine.subsurface(SPRITES.get('expbar'), (3, 24, 1, 7))
    end = engine.subsurface(SPRITES.get('expbar'), (4, 24, 2, 7))
    width = 136
    height = 24
    bar_max = 100

    def __init__(self, old_exp, center=True):
        self.bg_surf = self.create_bg_surf()
        if center:
            self.pos = WINWIDTH//2 - self.width//2, WINHEIGHT//2 - self.height//2
        else:
            self.pos = WINWIDTH//2 - self.width//2, WINHEIGHT - self.height

        self.offset = self.height//2  # Start with fade in
        self.done = False

        self.num = old_exp

    def create_bg_surf(self):
        surf = engine.create_surface((self.width, self.height), transparent=True)
        surf.blit(self.background, (0, 0))
        surf.blit(self.begin, (7, 9))
        return surf

    def fade_out(self):
        self.done = True

    def update(self, exp: float = None):
        if self.done:
            self.offset += 1  # So fade in and out
            if self.offset >= self.height//2:
                return True
        elif self.offset > 0:
            self.offset -= 1

        if exp is not None:
            self.num = exp

    def draw(self, surf):
        new_surf = engine.copy_surface(self.bg_surf)

        # Blit correct number of sprites for middle of exp bar
        idx = int(100 * max(0, self.num) / self.bar_max)
        for x in range(idx):
            new_surf.blit(self.middle, (10 + x, 9))

        # Blit end of exp bar
        new_surf.blit(self.end, (10 + idx, 9))

        # Blit current amount of exp
        FONT['number_small3'].blit_right(str(int(self.num)), new_surf, (self.width - 4, 4))

        # Transition
        new_surf = engine.subsurface(new_surf, (0, self.offset, self.width, self.height - self.offset * 2))

        surf.blit(new_surf, (self.pos[0], self.pos[1] + self.offset))
        return surf

class ManaBar(ExpBar):
    background = engine.subsurface(SPRITES.get('manabar'), (0, 0, 136, 24))
    begin = engine.subsurface(SPRITES.get('manabar'), (0, 24, 3, 7))
    middle = engine.subsurface(SPRITES.get('manabar'), (3, 24, 1, 7))
    end = engine.subsurface(SPRITES.get('manabar'), (4, 24, 2, 7))

    def __init__(self, old_exp, center=True):
        super().__init__(old_exp, center)
        self.pos = self.pos[0], self.pos[1] - self.height
