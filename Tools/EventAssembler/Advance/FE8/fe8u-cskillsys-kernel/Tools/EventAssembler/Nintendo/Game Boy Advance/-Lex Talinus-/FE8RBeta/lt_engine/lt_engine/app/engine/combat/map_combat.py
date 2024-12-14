from app.data.resources.resources import RESOURCES

from app.engine.combat.solver import CombatPhaseSolver

from app.engine.sound import get_sound_thread
from app.engine import engine, combat_calcs, gui, action, item_system, skill_system
from app.engine.health_bar import MapCombatInfo
from app.engine.animations import MapAnimation
from app.engine.game_state import game

from app.engine.combat.simple_combat import SimpleCombat


class MapCombat(SimpleCombat):
    alerts: bool = True

    def __init__(self, attacker, main_item, items, positions, main_target_positions, splash_positions, script, total_rounds=1):
        self._full_setup(attacker, main_item, items, positions,
                         main_target_positions, splash_positions)
        self.state_machine = CombatPhaseSolver(
            attacker, self.main_item, self.items,
            self.defenders, self.splashes, self.target_positions,
            self.defender, self.def_item, script, total_rounds)

        self.last_update = engine.get_time()
        self.state = 'init'
        self.hp_bar_time = 400

        self._skip = False
        self.full_playback = []
        self.playback = []
        self.actions = []

        self.animations = []
        self.health_bars = {}

        self.first_phase = True

    def skip(self):
        self._skip = True
        self.attacker.sprite.reset()
        if self.attacker.strike_partner:
            self.attacker.strike_partner.sprite.reset()
        if self.defender:
            self.defender.sprite.reset()
            if self.defender.strike_partner:
                self.defender.strike_partner.sprite.reset()

    def update(self) -> bool:
        current_time = engine.get_time() - self.last_update
        current_state = self.state

        # Only for the very first phase
        if self.state == 'init':
            self.start_combat()
            self.start_event()
            self.state = 'init_pause'

        elif self.state == 'init_pause':
            if self._skip or current_time > 200:
                self.state = 'begin_phase'

        # print("Map Combat %s" % self.state)
        elif self.state == 'begin_phase':
            # Get playback
            if not self.state_machine.get_state():
                self.clean_up()
                return True
            self.actions, self.playback = self.state_machine.do()
            self.full_playback += self.playback
            if not self.actions and not self.playback:
                self.state_machine.setup_next_state()
                return False
            if not item_system.no_map_hp_display(self.attacker, self.main_item):
                self._build_health_bars()
            if self.first_phase:
                self.set_up_pre_proc_animation('attack_pre_proc')
                self.set_up_pre_proc_animation('defense_pre_proc')
                self.set_up_other_proc_icons()
                self.add_proc_icon.memory.clear()
                self.first_phase = False

            # Camera
            if self.get_from_playback('defender_phase') or self.get_from_playback('defender_partner_phase'):
                game.cursor.set_pos(self.attacker.position)
            else:
                if self.defender:
                    game.cursor.set_pos(self.defender.position)
                elif self.target_positions[0]:
                    game.cursor.set_pos(self.target_positions[0])
            if not self._skip:
                game.state.change('move_camera')

            # Sprites
            if self.get_from_playback('defender_phase') or self.get_from_playback('defender_partner_phase'):
                if self.defender:
                    if self.get_from_playback('defender_phase'):
                        self.defender.sprite.change_state('combat_attacker')
                    elif self.get_from_playback('defender_partner_phase'):
                        self.defender.strike_partner.sprite.change_state(
                            'combat_attacker')
                self.attacker.sprite.change_state('combat_counter')
                if self.attacker.strike_partner:
                    self.attacker.strike_partner.sprite.change_state(
                        'combat_counter')
            else:
                if self.get_from_playback('defender_partner_phase'):
                    self.attacker.strike_partner.sprite.change_state(
                        'combat_attacker')
                else:
                    self.attacker.sprite.change_state('combat_attacker')
                    if self.attacker.strike_partner:
                        self.attacker.strike_partner.sprite.change_state(
                            'combat_counter')
                if self.defender:
                    self.defender.sprite.change_state('combat_defender')
                    if self.defender.strike_partner:
                        self.defender.strike_partner.sprite.change_state(
                            'combat_defender')
            self.state = 'red_cursor'

        elif self.state == 'red_cursor':
            if self.defender:
                game.cursor.combat_show()
            else:
                game.cursor.hide()
            # Handle proc effects
            self.set_up_proc_animation('attack_proc')
            self.set_up_proc_animation('attack_hit_proc')
            self.set_up_proc_animation('defense_proc')
            self.set_up_proc_animation('defense_hit_proc')

            self.state = 'start_anim'

        elif self.state == 'start_anim':
            if self._skip or current_time > 400:
                game.cursor.hide()
                game.highlight.remove_highlights()
                animation_brushes = self.get_from_playback('cast_anim')
                for brush in animation_brushes:
                    anim = RESOURCES.animations.get(brush.anim)
                    pos = game.cursor.position
                    if anim:
                        anim = MapAnimation(anim, pos)
                        self.animations.append(anim)

                animation_brushes = self.get_from_playback('target_cast_anim')
                for brush in animation_brushes:
                    anim = RESOURCES.animations.get(brush.anim)
                    if anim:
                        anim = MapAnimation(anim, brush.pos)
                        self.animations.append(anim)

                self.state = 'sound'

        elif self.state == 'sound':
            if self._skip or current_time > 250:
                if self.defender and self.defender.sprite.state == 'combat_attacker' and \
                        self.get_from_playback('defender_phase'):
                    self.defender.sprite.change_state('combat_anim')
                elif self.defender and self.defender.strike_partner and \
                        self.defender.strike_partner.sprite.state == 'combat_attacker' and \
                        self.get_from_playback('defender_partner_phase'):
                    self.defender.strike_partner.sprite.change_state(
                        'combat_anim')
                elif self.get_from_playback('attacker_partner_phase'):
                    self.attacker.strike_partner.sprite.change_state(
                        'combat_anim')
                else:
                    self.attacker.sprite.change_state('combat_anim')
                sound_brushes = self.get_from_playback('cast_sound')
                for brush in sound_brushes:
                    get_sound_thread().play_sfx(brush.sound)

                self.state = 'anim'

        elif self.state == 'anim':
            if self._skip or current_time > 83:
                self._handle_playback()
                self._apply_actions()

                # Force update hp bars so we can get timing info
                for hp_bar in self.health_bars.values():
                    hp_bar.update()
                if self.health_bars:
                    self.hp_bar_time = max(hp_bar.get_time_for_change()
                                           for hp_bar in self.health_bars.values())
                else:
                    self.hp_bar_time = 0
                self.state = 'hp_bar_wait'

        elif self.state == 'hp_bar_wait':
            if self._skip or current_time > self.hp_bar_time:
                self.state = 'end_phase'

        elif self.state == 'end_phase':
            if self._skip or current_time > 550:
                if self.defender and self.defender.sprite.state == 'combat_anim':
                    self.defender.sprite.change_state('combat_attacker')
                elif self.defender and self.defender.strike_partner and \
                        self.defender.strike_partner.sprite.state == 'combat_anim':
                    self.defender.strike_partner.sprite.change_state(
                        'combat_attacker')
                elif self.attacker.strike_partner and \
                        self.attacker.strike_partner.sprite.state == 'combat_anim':
                    self.attacker.strike_partner.sprite.change_state(
                        'combat_attacker')
                else:
                    self.attacker.sprite.change_state('combat_attacker')
                self._end_phase()
                self.state_machine.setup_next_state()
                self.state = 'begin_phase'

        if self.state != current_state:
            self.last_update = engine.get_time()

        if self.state not in ('begin_phase', 'red_cursor'):
            for hp_bar in self.health_bars.values():
                hp_bar.update()

        return False

    def _build_health_bars(self):
        if (self.defender and self.all_splash) or len(self.all_splash) > 1:
            # Many splash attacks
            # No health bars!!
            self.health_bars.clear()

        else:
            # P1 on P1
            if self.defender and self.attacker is self.defender:
                hit = combat_calcs.compute_hit(
                    self.attacker, self.defender, self.main_item, self.def_item, 'attack', self.state_machine.get_attack_info())
                mt = combat_calcs.compute_damage(
                    self.attacker, self.defender, self.main_item, self.def_item, 'attack', self.state_machine.get_attack_info())
                if self.attacker not in self.health_bars:
                    attacker_health = MapCombatInfo(
                        'p1', self.attacker, self.main_item, self.defender, (hit, mt))
                    self.health_bars[self.attacker] = attacker_health
                else:
                    self.health_bars[self.attacker].update_stats((hit, mt))

            # P1 on P2
            elif self.defender:
                hit = combat_calcs.compute_hit(
                    self.attacker, self.defender, self.main_item, self.def_item, 'attack', self.state_machine.get_attack_info())
                mt = combat_calcs.compute_damage(
                    self.attacker, self.defender, self.main_item, self.def_item, 'attack', self.state_machine.get_attack_info())
                if self.attacker not in self.health_bars:
                    attacker_health = MapCombatInfo(
                        'p1', self.attacker, self.main_item, self.defender, (hit, mt))
                    self.health_bars[self.attacker] = attacker_health
                else:
                    self.health_bars[self.attacker].update_stats((hit, mt))

                if combat_calcs.can_counterattack(self.attacker, self.main_item, self.defender, self.def_item):
                    hit = combat_calcs.compute_hit(
                        self.defender, self.attacker, self.def_item, self.main_item, 'defense', self.state_machine.get_defense_info())
                    mt = combat_calcs.compute_damage(
                        self.defender, self.attacker, self.def_item, self.main_item, 'defense', self.state_machine.get_defense_info())
                else:
                    hit, mt = None, None
                if self.defender not in self.health_bars:
                    defender_health = MapCombatInfo(
                        'p2', self.defender, self.def_item, self.attacker, (hit, mt))
                    self.health_bars[self.defender] = defender_health
                else:
                    self.health_bars[self.defender].update_stats((hit, mt))

            # P1 on single splash
            elif len(self.all_splash) == 1:
                defender = self.all_splash[0]
                hit = combat_calcs.compute_hit(
                    self.attacker, defender, self.main_item, None, 'attack', self.state_machine.get_attack_info())
                mt = combat_calcs.compute_damage(
                    self.attacker, defender, self.main_item, None, 'attack', self.state_machine.get_attack_info())
                if self.attacker not in self.health_bars:
                    attacker_health = MapCombatInfo(
                        'p1', self.attacker, self.main_item, defender, (hit, mt))
                    self.health_bars[self.attacker] = attacker_health
                else:
                    self.health_bars[self.attacker].update_stats((hit, mt))

                if defender not in self.health_bars:
                    splash_health = MapCombatInfo(
                        'splash', defender, None, self.attacker, (None, None))
                    self.health_bars[defender] = splash_health

    def _handle_playback(self):
        for brush in self.playback:
            if brush.nid == 'unit_tint_add':
                brush.unit.sprite.begin_flicker(333, brush.color, 'add')
            elif brush.nid == 'unit_tint_sub':
                brush.unit.sprite.begin_flicker(333, brush.color, 'sub')
            elif brush.nid == 'crit_tint':
                brush.unit.sprite.begin_flicker(33, brush.color, 'add')
                # Delay five frames
                brush.unit.sprite.start_flicker(83, 33, brush.color, 'add')
                # Delay five more frames
                brush.unit.sprite.start_flicker(
                    166, 333, brush.color, 'add', fade_out=True)
            elif brush.nid == 'crit_vibrate':
                # In 10 frames, start vibrating for 12 frames
                brush.unit.sprite.start_vibrate(166, 200)
            elif brush.nid == 'hit_sound':
                get_sound_thread().play_sfx(brush.sound)
            elif brush.nid == 'shake':
                shake = brush.shake
                if self.get_from_playback('attack_proc'):
                    shake = 3  # Force critical type shake
                for health_bar in self.health_bars.values():
                    health_bar.shake(shake)
            elif brush.nid == 'hit_anim':
                anim = RESOURCES.animations.get(brush.anim)
                pos = brush.defender.position
                if anim and pos:
                    anim = MapAnimation(anim, pos)
                    self.animations.append(anim)
            elif brush.nid == 'damage_hit':
                damage = brush.damage
                if damage <= 0:
                    continue
                str_damage = str(damage)
                target = brush.defender
                for idx, num in enumerate(str_damage):
                    d = gui.DamageNumber(int(num), idx, len(
                        str_damage), target.position, 'small_red')
                    target.sprite.damage_numbers.append(d)
            elif brush.nid == 'damage_crit':
                damage = brush.damage
                if damage <= 0:
                    continue
                str_damage = str(damage)
                target = brush.defender
                for idx, num in enumerate(str_damage):
                    d = gui.DamageNumber(int(num), idx, len(
                        str_damage), target.position, 'small_yellow')
                    target.sprite.damage_numbers.append(d)
            elif brush.nid == 'heal_hit':
                damage = brush.damage
                if damage <= 0:
                    continue
                str_damage = str(damage)
                target = brush.defender
                for idx, num in enumerate(str_damage):
                    d = gui.DamageNumber(int(num), idx, len(
                        str_damage), target.position, 'small_cyan')
                    target.sprite.damage_numbers.append(d)

    def _apply_actions(self):
        """
        Actually commit the actions that we had stored!
        """
        for act in self.actions:
            action.do(act)

    def set_up_proc_animation(self, mark_type):
        marks = self.get_from_playback(mark_type)
        for mark in marks:
            self.add_proc_icon(mark.unit, mark.skill)

    def set_up_pre_proc_animation(self, mark_type):
        marks = self.get_from_full_playback(mark_type)
        for mark in marks:
            self.add_proc_icon(mark.unit, mark.skill)

    def set_up_other_proc_icons(self):
        for skill in self.attacker.skills:
            if skill_system.get_show_skill_icon(self.attacker, skill):
                self.add_proc_icon(self.attacker, skill)
        if self.defender:
            for skill in self.defender.skills:
                if skill_system.get_show_skill_icon(self.defender, skill):
                    self.add_proc_icon(self.defender, skill)

    def add_proc_icon(self, unit, skill):
        if unit in self.health_bars:
            # Check if we should be hiding this skill
            if skill_system.get_hide_skill_icon(unit, skill):
                return
            if skill.nid in self.add_proc_icon.memory.get(unit.nid, []):
                return

            health_bar = self.health_bars[unit]
            right = health_bar.ordering == 'right' or health_bar.ordering == 'middle'
            skill_icon = gui.SkillIcon(skill, right)
            health_bar.add_skill_icon(skill_icon)

            # Make sure the same proc icon never shows up twice in the same phase
            if unit.nid not in self.add_proc_icon.memory:
                self.add_proc_icon.memory[unit.nid] = []
            self.add_proc_icon.memory[unit.nid].append(skill.nid)
    add_proc_icon.memory = {}

    def _end_phase(self):
        pass

    def handle_support_pairs(self, pairs):
        for pair in pairs:
            unit1, unit2 = pair
            if not unit1.position or not unit2.position:
                continue
            unit1.sprite.add_animation('support_up', loop=False)
            unit2.sprite.add_animation('support_up', loop=False)

    def draw(self, surf):
        # Animations
        self.animations = [
            anim for anim in self.animations if not anim.update()]
        for anim in self.animations:
            anim.draw(surf, offset=(-game.camera.get_x(), -game.camera.get_y()))

        for hp_bar in self.health_bars.values():
            hp_bar.draw(surf)

        return surf
