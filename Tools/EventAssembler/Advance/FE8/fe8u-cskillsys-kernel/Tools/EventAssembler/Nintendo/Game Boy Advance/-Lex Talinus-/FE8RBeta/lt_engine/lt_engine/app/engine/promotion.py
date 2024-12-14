from app.constants import WINWIDTH, WINHEIGHT

from app.data.database.database import DB
from app.data.resources.resources import RESOURCES
from app.utilities import utils

from app.engine.fonts import FONT
from app.engine.sprites import SPRITES
from app.engine.sound import get_sound_thread
from app.engine.state import State
from app.engine import background, menus, engine, dialog, text_funcs, icons, \
    action, item_system, battle_animation
from app.engine.combat.mock_combat import MockCombat
from app.engine.game_state import game
from app.engine.fluid_scroll import FluidScroll

class PromotionChoiceState(State):
    name = 'promotion_choice'
    bg = None

    def __init__(self, name=None):
        self.name = name
        self.bg = background.create_background('settings_background')

    def _get_choices(self):
        self.class_options = game.memory.get('promo_options', None) or self.unit_klass.turns_into
        game.memory['promo_options'] = None
        return [DB.classes.get(c).name for c in self.class_options]

    def _proceed(self, next_class):
        game.memory['current_unit'] = self.unit
        game.memory['next_class'] = next_class
        game.memory['next_state'] = 'promotion'
        game.state.change('transition_to_with_pop')

    def start(self):
        self.fluid = FluidScroll()

        self.can_go_back = game.memory.get('can_go_back', False)
        game.memory['can_go_back'] = None
        self.combat_item = game.memory.get('combat_item')
        game.memory['combat_item'] = None
        self.unit = game.memory['current_unit']
        self.unit_klass = DB.classes.get(self.unit.klass)
        display_options = self._get_choices()

        self.menu = menus.Choice(self.unit, display_options, (14, 13))
        self.menu.set_limit(5)
        self.child_menu = None

        self.animations = []
        self.weapon_icons = []
        for option in self.class_options:
            anim = battle_animation.get_battle_anim(self.unit, None, klass=option)
            if anim:
                anim.pair(self, None, True, 0)
            self.animations.append(anim)
            weapons = []
            klass = DB.classes.get(option)
            for weapon_nid, weapon in klass.wexp_gain.items():
                if weapon.usable:
                    weapons.append(weapon_nid)
            self.weapon_icons.append(weapons)

        # Platforms
        if game.tilemap and self.unit.position:
            terrain = game.tilemap.get_terrain(self.unit.position)
            platform_type = DB.terrain.get(terrain).platform
        else:
            platform_type = 'Floor'
        platform = RESOURCES.platforms[platform_type + '-Melee']
        self.left_platform = engine.image_load(platform)
        self.right_platform = engine.flip_horiz(self.left_platform.copy())

        # For anim swoop in
        self.anim_offset = 120
        self.target_anim_offset = False

        self.current_desc = self._get_desc()

        game.state.change('transition_in')
        return 'repeat'

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        current_idx = self.menu.get_current_index()
        self.menu.handle_mouse()
        new_idx = self.menu.get_current_index()
        if current_idx != new_idx:  # Mouse moved
            self.target_anim_offset = True
            self.current_desc = self._get_desc()

        if 'DOWN' in directions:
            get_sound_thread().play_sfx('Select 6')
            if self.child_menu:
                self.child_menu.move_down(first_push)
            else:
                self.menu.move_down(first_push)
                self.target_anim_offset = True
                self.current_desc = self._get_desc()
        elif 'UP' in directions:
            get_sound_thread().play_sfx('Select 6')
            if self.child_menu:
                self.child_menu.move_up(first_push)
            else:
                self.menu.move_up(first_push)
                self.target_anim_offset = True
                self.current_desc = self._get_desc()

        elif event == 'BACK':
            if self.child_menu:
                get_sound_thread().play_sfx('Select 4')
                self.child_menu = None
            elif self.can_go_back:
                get_sound_thread().play_sfx('Select 4')
                game.state.back()
                if self.combat_item:
                    action.do(action.HasNotAttacked(self.unit))
                    item_system.reverse_use(self.unit, self.combat_item)
            else:
                # Can't go back...
                get_sound_thread().play_sfx('Error')

        elif event == 'SELECT':
            if self.child_menu:
                selection = self.child_menu.get_current()
                if selection == 'Change':
                    get_sound_thread().play_sfx('Select 1')
                    self._proceed(self.class_options[self.menu.get_current_index()])
                else:
                    get_sound_thread().play_sfx('Select 4')
                    self.child_menu = None
            else:
                get_sound_thread().play_sfx('Select 1')
                selection = self.menu.get_current()
                options = ['Change', 'Cancel']
                self.child_menu = menus.Choice(selection, options, self.menu)

    def _get_desc(self):
        current_klass = self.class_options[self.menu.get_current_index()]
        desc = DB.classes.get(current_klass).desc
        d = dialog.Dialog(desc.replace('\n', '{br}'))
        d.position = (6, 112)
        d.text_width = WINWIDTH - 28
        d.width = d.text_width + 16
        d.font_type = 'convo'
        d.font_color = 'white'
        d.font = FONT['convo-white']
        d.draw_cursor_flag = False
        return d

    def update(self):
        self.menu.update()
        if self.child_menu:
            self.child_menu.update()
        if self.current_desc:
            self.current_desc.update()

        if self.target_anim_offset:
            self.anim_offset += 8
            if self.anim_offset >= 120:
                self.target_anim_offset = False
                self.anim_offset = 120
        elif self.anim_offset > 0:
            self.anim_offset -= 8
            if self.anim_offset < 0:
                self.anim_offset = 0

        anim = self.animations[self.menu.get_current_index()]
        if anim:
            anim.update()

    def draw(self, surf):
        if not self.started:
            return surf

        self.bg.draw(surf)

        top = WINHEIGHT - 72
        surf.blit(self.left_platform, (WINWIDTH//2 - self.left_platform.get_width() + self.anim_offset + 52, top))
        surf.blit(self.right_platform, (WINWIDTH//2 + self.anim_offset + 52, top))
        anim = self.animations[self.menu.get_current_index()]
        if anim:
            anim.draw(surf, (self.anim_offset + 12, 0))

        # Class Reel
        FONT['class'].blit(self.menu.get_current(), surf, (114, 5))

        # Weapon Icons
        for idx, weapon in enumerate(self.weapon_icons[self.menu.get_current_index()]):
            icons.draw_weapon(surf, weapon, (130 + 16 * idx, 32))

        if self.menu:
            self.menu.draw(surf)
        if self.child_menu:
            self.child_menu.draw(surf)

        surf.blit(SPRITES.get('promotion_description'), (6, 112))
        if self.current_desc:
            self.current_desc.draw(surf)

        return surf

class ClassChangeChoiceState(PromotionChoiceState):
    name = 'class_change_choice'

    def _get_choices(self):
        if game.memory.get('promo_options', None):
            self.class_options = game.memory['promo_options']
            game.memory['promo_options'] = None
        elif not self.unit.generic:
            unit_prefab = DB.units.get(self.unit.nid)
            self.class_options = [c for c in unit_prefab.alternate_classes if c != self.unit.klass]
        else:  # Just every class, lol?
            self.class_options = [c.nid for c in DB.classes.values() if c.nid != self.unit.klass]
        if DB.constants.value('class_change_same_tier'):
            self.class_options = [c for c in self.class_options if DB.classes.get(c).tier == DB.classes.get(self.unit.klass).tier]
        return [DB.classes.get(c).name for c in self.class_options]

    def _proceed(self, next_class):
        game.memory['current_unit'] = self.unit
        game.memory['next_class'] = next_class
        game.memory['next_state'] = 'class_change'
        game.state.change('transition_to_with_pop')

class PromotionState(State, MockCombat):
    name = 'promotion'
    bg = None

    def _finalize(self, current_time):
        self.state = 'level_up'
        self.last_update = current_time
        game.exp_instance.append((self.unit, 0, self, 'promote'))
        game.state.change('exp')

    def start(self):
        self.create_background()

        music = 'music_%s' % self.name
        self.promotion_song = None
        if game.game_vars.get('_' + music):
            self.promotion_song = \
                get_sound_thread().fade_in(game.game_vars.get('_' + music), fade_in=50)
        elif DB.constants.value(music):
            self.promotion_song = \
                get_sound_thread().fade_in(DB.constants.value(music), fade_in=50)

        self.unit = game.memory['current_unit']
        color = DB.teams.get(self.unit.team).combat_color

        # Old Right Animation
        self.right_battle_anim = battle_animation.get_battle_anim(self.unit, None)
        # New Left Animation
        next_class = game.memory['next_class']
        self.left_battle_anim = battle_animation.get_battle_anim(self.unit, None, klass=next_class)
        self.current_battle_anim = self.right_battle_anim

        platform_type = 'Floor'
        platform = RESOURCES.platforms[platform_type + '-Melee']
        self.left_platform = engine.image_load(platform)
        self.right_platform = engine.flip_horiz(self.left_platform.copy())

        # Name tag
        self.name_tag = SPRITES.get('combat_name_right_' + color).copy()
        width = FONT['text-brown'].width(self.unit.name)
        FONT['text-brown'].blit(self.unit.name, self.name_tag, (36 - width // 2, 8))

        # For darken backgrounds and drawing
        self.setup_dark()
        self.last_update = engine.get_time()
        self.state = 'init'

        if not self.right_battle_anim or not self.left_battle_anim:
            self._finalize(engine.get_time())
        else:
            game.state.change('transition_in')
            self.left_battle_anim.pair(self, self.right_battle_anim, False, 0)
            self.right_battle_anim.pair(self, self.left_battle_anim, True, 0)

        return 'repeat'

    def begin(self):
        self.last_update = engine.get_time()

    def start_anim(self, effect_nid):
        anim = self.current_battle_anim
        effect = anim.get_effect(effect_nid, pose='Attack')
        if effect:
            anim.add_effect(effect)

    def update(self):
        current_time = engine.get_time() - self.last_update
        self.current_state = self.state

        if self.state == 'init':
            if current_time > utils.frames2ms(25):
                self.state = 'right'
                self.start_anim('Promotion1')

        elif self.state == 'right':
            if not self.current_battle_anim.child_effects:
                self.current_battle_anim = self.left_battle_anim
                self.state = 'left'
                self.start_anim('Promotion2')

        elif self.state == 'left':
            if not self.current_battle_anim.child_effects:
                self.state = 'wait'

        elif self.state == 'wait':
            if current_time > utils.frames2ms(100):
                self._finalize(engine.get_time())

        elif self.state == 'level_up':
            self.state = 'leave'

        elif self.state == 'leave':
            if current_time > utils.frames2ms(10):
                game.state.change('transition_pop')
                self.state = 'done'
                if self.promotion_song:
                    get_sound_thread().fade_back()
                return 'repeat'

        if self.state != self.current_state:
            self.last_update = engine.get_time()

        # Update anims
        if self.current_battle_anim:
            self.current_battle_anim.update()

        if self.state == 'done':
            return 'repeat'

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        else:
            return surf

        combat_surf = engine.copy_surface(self.combat_surf)

        # Platforms
        top = WINHEIGHT - 72
        combat_surf.blit(self.left_platform, (WINWIDTH//2 - self.left_platform.get_width(), top))
        combat_surf.blit(self.right_platform, (WINWIDTH//2, top))

        # Name Tag
        combat_surf.blit(self.name_tag, (WINWIDTH + 3 - self.name_tag.get_width(), 0))

        self.color_ui(combat_surf)

        surf.blit(combat_surf, (0, 0))

        if self.current_battle_anim:
            self.current_battle_anim.draw(surf)

        self.foreground.draw(surf)

        return surf

class ClassChangeState(PromotionState):
    name = 'class_change'

    def _finalize(self, current_time):
        self.state = 'level_up'
        self.last_update = current_time
        game.exp_instance.append((self.unit, 0, self, 'class_change'))
        game.state.change('exp')
