from __future__ import annotations
from app.data.database.units import UnitPrefab

from typing import TYPE_CHECKING

from app.data.database.klass import Klass
from app.engine.game_counters import ANIMATION_COUNTERS
from app.engine.objects.unit import UnitObject
from app.engine.unit_sprite import MapSprite
from app.sprites import SPRITES

if TYPE_CHECKING:
    from app.engine.engine import Surface
    from app.engine.objects.overworld import OverworldEntityObject, RoadObject

from typing import Tuple

from app.constants import TILEHEIGHT, TILEWIDTH
from app.data.database.database import DB
from app.data.database.overworld_node import OverworldNodePrefab
from app.engine.overworld.overworld_road_sprite_wrapper import OverworldRoadSpriteWrapper
from app.engine import engine, image_mods, skill_system
from app.engine.animations import MapAnimation
from app.engine.sound import get_sound_thread
from app.data.resources.map_icons import MapIcon
from app.data.resources.resources import RESOURCES
from app.utilities import utils
from app.utilities.typing import NID, Point

class FlagSprite():
    def __init__(self) -> None:
        self._sprite: Surface = SPRITES.get('objective_flag')

    @property
    def sprite(self):
        left = ANIMATION_COUNTERS.flag_counter.count * 24
        surf = engine.subsurface(self._sprite, (left, 0, 24, 12))
        return surf

class OverworldNodeSprite():
    def __init__(self, node: OverworldNodePrefab):
        self.node: OverworldNodePrefab = node

        self.transition_state = 'normal' # for fading

        self.transition_counter = 0
        self.transition_time = 1000

        self.position: Point = node.pos
        self.offset: Tuple[int, int] = (0, 0)
        self.map_icon: MapIcon = RESOURCES.map_icons.get(self.node.icon)
        self.flag_sprite: FlagSprite = FlagSprite()
        self.load_sprites()

    def load_sprites(self):
        if self.map_icon and not self.map_icon.image:
            self.map_icon.image = engine.image_load(self.map_icon.full_path)

    def set_transition(self, new_state):
        self.transition_state = new_state
        self.transition_counter = self.transition_time

    def update_transition(self):
        self.transition_counter -= engine.get_delta()
        if self.transition_counter < 0:
            self.transition_counter = 0
            if self.transition_state in ('fade_out', 'fade_in'):
                self.set_transition('normal')

    # Normally drawing units is culled to those on the screen
    # Unit sprites matching this will be drawn anyway
    def draw_anyway(self):
        return self.transition_state != 'normal'

    def update(self):
        self.update_transition()

    def create_image(self) -> Surface:
        if not self.map_icon:  # This shouldn't happen, but if it does...
            self.map_icon = RESOURCES.map_icons.DEFAULT()
            self.load_sprites()
        return self.map_icon.image

    def get_topleft(self, cull_rect: Tuple[int, int, int, int]) -> Point:
        return utils.tuple_sub(utils.tuple_add((self.position[0] * TILEWIDTH, self.position[1] * TILEHEIGHT),
                                                self.offset),
                               (cull_rect[0], cull_rect[1]))

    def draw(self, surf: Surface, cull_rect: Tuple[int, int, int, int], has_flag=False):
        image = self.create_image()
        flag = engine.copy_surface(self.flag_sprite.sprite)
        topleft = self.get_topleft(cull_rect)

        # Handle transitions
        progress = utils.clamp((self.transition_time - self.transition_counter) / self.transition_time, 0, 1)
        if self.transition_state in ('fade_out'):
            image = image_mods.make_translucent(image.convert_alpha(), progress)
            flag = image_mods.make_translucent(flag.convert_alpha(), progress)
        elif self.transition_state in ('fade_in'):
            progress = 1 - progress
            image = image_mods.make_translucent(image.convert_alpha(), progress)
            flag = image_mods.make_translucent(flag.convert_alpha(), progress)

        topleft = utils.tuple_sub(topleft, ((image.get_width() - 16)//2, image.get_height() - 16))
        surf.blit(image, topleft)
        if has_flag:
            surf.blit(flag, (topleft[0] + 6, topleft[1] - flag.get_height() + 2))
        return surf

class OverworldRoadSprite():
    """Contains logic for drawing roads between nodes.
    """
    # @TODO make this configurable
    ROAD_COLOR = (248, 248, 200, 250)           # GOLDEN YELLOW
    ROAD_UNDERLAY_COLOR = (232, 216, 136, 160)  # DARKER YELLOW

    def __init__(self, road: RoadObject):
        self.road = road
        self.road_sprite = OverworldRoadSpriteWrapper()

        self.transition_state = 'normal' # for fading

        self.transition_counter = 0
        self.transition_time = 1000

        self.cached_surf: Surface = None
        self.redraw = True

    def set_transition(self, new_state):
        self.transition_state = new_state
        self.transition_counter = self.transition_time

    def update_transition(self):
        self.transition_counter -= engine.get_delta()
        if self.transition_counter < 0:
            self.transition_counter = 0
            if self.transition_state in ('fade_out', 'fade_in'):
                self.set_transition('normal')

    # Normally drawing units is culled to those on the screen
    # Unit sprites matching this will be drawn anyway
    def draw_anyway(self):
        return self.transition_state != 'normal'

    def update(self):
        self.update_transition()

    def draw(self, surf: Surface, full_size: Tuple[int, int], cull_rect: Tuple[int, int, int, int]):
        if self.redraw:
            # this redraws the entire road system instead of just on a cull; this is important for caching
            road_surf: Surface = engine.create_surface(full_size, True)
            unpacked_road = OverworldRoadSpriteWrapper.road_to_full_points_list(self.road.prefab)
            for i in range(len(unpacked_road)):
                neighbors = []
                if i != 0:
                    neighbors.append(unpacked_road[i-1])
                if i < len(unpacked_road) - 1:
                    neighbors.append(unpacked_road[i + 1])
                self.road_sprite.draw_tile(road_surf, unpacked_road[i], neighbors)
            self.cached_surf = road_surf
            self.redraw = False
        else:
            road_surf = self.cached_surf

        if self.transition_state in ('fade_out'):
            progress = utils.clamp((self.transition_time - self.transition_counter) / self.transition_time, 0, 1)
            road_surf = image_mods.make_translucent(road_surf.convert_alpha(), progress)
        elif self.transition_state in ('fade_in'):
            progress = utils.clamp((self.transition_time - self.transition_counter) / self.transition_time, 0, 1)
            progress = 1 - progress
            road_surf = image_mods.make_translucent(road_surf.convert_alpha(), progress)

        # because the road drawing uses caching, it's better to cull manually here
        culled_road_surf = engine.subsurface(road_surf, cull_rect)
        surf.blit(culled_road_surf, (0, 0))
        return surf

class OverworldUnitSprite():
    def __init__(self, unit_object: UnitObject | UnitPrefab, parent: OverworldEntityObject, team: NID):
        self.unit: UnitObject | UnitPrefab = unit_object
        self.team = team
        self.state = 'normal'                       # What state the image sprite is in
        self.image_state = 'passive'                # What the image looks like
        self.transition_state = 'normal'
        self.parent = parent

        self.hovered: bool = False

        self.map_sprite: MapSprite = None

        self.transition_counter = 0
        self.transition_time = 450

        self.fake_position: Tuple[int, int] = None  # Maybe this will have a use
        self.net_position = None
        self.offset = [0, 0]

        self.flicker = []
        self.vibrate = []
        self.vibrate_counter = 0
        self.animations = {}

        self.load_sprites()

    def load_sprites(self):
        klass: Klass = DB.classes.get(self.unit.klass)
        nid = klass.map_sprite_nid
        variant = skill_system.change_variant(self.unit) if isinstance(self.unit, UnitObject) else None
        if variant:
            nid += variant
        res = RESOURCES.map_sprites.get(nid)
        if not res:  # Try without unit variant
            res = RESOURCES.map_sprites.get(klass.map_sprite_nid)
        if not res:
            self.map_sprite = None
            return

        map_sprite = MapSprite(res, self.team)
        self.map_sprite = map_sprite

    # Normally drawing units is culled to those on the screen
    # Unit sprites matching this will be drawn anyway
    def draw_anyway(self):
        return self.transition_state != 'normal'

    def add_animation(self, animation_nid):
        anim = RESOURCES.animations.get(animation_nid)
        if anim:
            anim = MapAnimation(anim, (0, 0), loop=True)
            self.animations[animation_nid] = anim

    def remove_animation(self, animation_nid):
        if animation_nid in self.animations:
            del self.animations[animation_nid]

    def begin_flicker(self, total_time, color, direction='add'):
        self.flicker.append((engine.get_time(), total_time, color, direction, False))

    def start_flicker(self, start_time, total_time, color, direction='add', fade_out=False):
        self.flicker.append((engine.get_time() + start_time, total_time, color, direction, fade_out))

    def start_vibrate(self, start_time, total_time):
        self.vibrate.append((engine.get_time() + start_time, total_time))

    def set_transition(self, new_state):
        self.transition_state = new_state
        self.transition_counter = self.transition_time  # 450

        if self.transition_state == 'normal':
            self.offset = [0, 0]
            self.fake_position = None
        elif self.transition_state == 'fake_in':
            self.fake_position = None
            self.change_state('fake_transition_in')
        elif self.transition_state in ('fake_out', 'rescue'):
            self.fake_position = self.parent.display_position
            self.change_state('fake_transition_out')
        elif self.transition_state == 'fade_in':
            self.fake_position = None
        elif self.transition_state == 'fade_out':
            self.fake_position = self.parent.display_position
        elif self.transition_state == 'fade_move':
            self.fake_position = self.parent.display_position
        elif self.transition_state == 'warp_in':
            get_sound_thread().play_sfx('WarpEnd')
        elif self.transition_state == 'warp_out':
            get_sound_thread().play_sfx('Warp')
            self.fake_position = self.parent.display_position
        elif self.transition_state == 'warp_move':
            get_sound_thread().play_sfx('Warp')
            self.fake_position = self.parent.display_position

    def change_state(self, new_state):
        self.state = new_state
        if self.state == 'fake_transition_in':
            pos = (self.parent.display_position[0] + utils.clamp(self.offset[0], -1, 1),
                   self.parent.display_position[1] + utils.clamp(self.offset[1], -1, 1))
            pos = (pos[0] - self.parent.display_position[0], pos[1] - self.parent.display_position[1])
            self.net_position = (-pos[0], -pos[1])
            self.update_sprite_direction(self.net_position)
        elif self.state == 'fake_transition_out':
            pos = (utils.clamp(self.offset[0], -1, 1),
                   utils.clamp(self.offset[1], -1, 1))
            self.net_position = pos
            self.update_sprite_direction(self.net_position)

    def update_sprite_direction(self, direction_vector: Tuple[int, int]):
        if abs(direction_vector[0]) >= abs(direction_vector[1]):
            if direction_vector[0] > 0:
                self.image_state = 'right'
            elif direction_vector[0] < 0:
                self.image_state = 'left'
            else:
                self.image_state = 'down'  # default
        else:
            if direction_vector[1] < 0:
                self.image_state = 'up'
            else:
                self.image_state = 'down'

    def update(self):
        self.update_state()
        self.update_transition()

    def update_state(self):
        if self.state == 'normal':
            self.offset = [0, 0]
            if self.hovered:
                self.image_state = 'active'
            else:
                self.image_state = 'passive'
        elif self.state == 'moving':
            return
        elif self.state == 'fake_transition_in':
            if self.offset[0] > 0:
                self.offset[0] -= 2
            elif self.offset[0] < 0:
                self.offset[0] += 2
            if self.offset[1] > 0:
                self.offset[1] -= 2
            elif self.offset[1] < 0:
                self.offset[1] += 2
            if self.offset[0] == 0 and self.offset[1] == 0:
                self.set_transition('normal')
                self.change_state('normal')
        elif self.state == 'fake_transition_out':
            if self.offset[0] > 0:
                self.offset[0] += 2
            elif self.offset[0] < 0:
                self.offset[0] -= 2
            if self.offset[1] > 0:
                self.offset[1] += 2
            elif self.offset[1] < 0:
                self.offset[1] -= 2

            if abs(self.offset[0]) > TILEWIDTH or abs(self.offset[1]) > TILEHEIGHT:
                self.set_transition('normal')
                self.change_state('normal')

    def update_transition(self):
        self.transition_counter -= engine.get_delta()
        if self.transition_counter < 0:
            self.transition_counter = 0
            self.fake_position = None
            if self.transition_state in ('fade_out', 'warp_out', 'fade_in', 'warp_in'):
                self.set_transition('normal')
            elif self.transition_state == 'fade_move':
                self.set_transition('fade_in')
            elif self.transition_state == 'warp_move':
                self.set_transition('warp_in')

    def select_frame(self, image, state):
        if state == 'passive' or state == 'gray':
            return image[ANIMATION_COUNTERS.passive_sprite_counter.count].copy()
        elif state == 'active':
            return image[ANIMATION_COUNTERS.active_sprite_counter.count].copy()
        else:
            return image[ANIMATION_COUNTERS.move_sprite_counter.count].copy()

    def create_image(self, state):
        if not self.map_sprite:  # This shouldn't happen, but if it does...
            res = RESOURCES.map_sprites[0]
            self.map_sprite = MapSprite(res, self.unit.team)
        image = getattr(self.map_sprite, state)
        image = self.select_frame(image, state)
        return image

    def get_topleft(self, cull_rect: Tuple[int, int, int, int]) -> Point:
        x, y = self.parent.display_position
        left = x * TILEWIDTH + self.offset[0] - cull_rect[0]
        top = y * TILEHEIGHT + self.offset[1] - cull_rect[1]
        return (left, top)

    def draw(self, surf: Surface, cull_rect: Tuple[int, int, int, int]):
        if not self.parent.display_position:
            return surf
        current_time = engine.get_time()
        image = self.create_image(self.image_state)
        left, top = self.get_topleft(cull_rect)

        self.vibrate_counter += 1
        for vibrate in self.vibrate[:]:
            starting_time, total_time = vibrate
            if engine.get_time() >= starting_time:
                if engine.get_time() > starting_time + total_time:
                    self.vibrate.remove(vibrate)
                    continue
                left += (1 if self.vibrate_counter % 2 else -1)

        # Handle transitions
        if self.transition_state in ('fade_out', 'warp_out', 'fade_move', 'warp_move'):
            progress = utils.clamp((self.transition_time - self.transition_counter) / self.transition_time, 0, 1)
            image = image_mods.make_translucent(image.convert_alpha(), progress)
        elif self.transition_state in ('fade_in', 'warp_in'):
            progress = utils.clamp((self.transition_time - self.transition_counter) / self.transition_time, 0, 1)
            progress = 1 - progress
            image = image_mods.make_translucent(image.convert_alpha(), progress)

        for flicker in self.flicker[:]:
            starting_time, total_time, color, direction, fade_out = flicker
            if engine.get_time() >= starting_time:
                if engine.get_time() > starting_time + total_time:
                    self.flicker.remove(flicker)
                    continue
                if fade_out:
                    time_passed = engine.get_time() - starting_time
                    color = tuple((total_time - time_passed) * float(c) // total_time for c in color)
                if direction == 'add':
                    image = image_mods.add_tint(image.convert_alpha(), color)
                elif direction == 'sub':
                    image = image_mods.sub_tint(image.convert_alpha(), color)

        topleft = left - max(0, (image.get_width() - 16)//2), top - 24
        surf.blit(image, topleft)

        # Draw animations
        for animation in self.animations.values():
            animation.draw(surf, (left, top))

        return surf
