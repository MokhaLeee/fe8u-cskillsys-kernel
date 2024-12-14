from __future__ import annotations

from typing import List, Tuple
from typing import TYPE_CHECKING

from app.data.database.database import DB
from app.engine import engine, action
from app.engine.game_state import game
from app.engine.graphics.dialog.narration_dialogue import NarrationDialogue
from app.engine.objects.overworld import OverworldNodeObject, OverworldEntityObject
from app.engine.overworld.overworld_actions import OverworldMove
from app.engine.overworld.overworld_movement_manager import \
    OverworldMovementManager
from app.utilities.typing import NID, Point

if TYPE_CHECKING:
    from app.events.event import Event

def overworld_cinematic(self: Event, overworld_nid=None, flags=None):
    if not overworld_nid:
        if DB.overworlds.values():
            overworld_nid = DB.overworlds.values()[0].nid
        else:
            self.logger.error('No overworlds in the DB - why are you calling the overworld command?')
            return
    # save level state
    self.prev_game_boundary = game.boundary
    self.prev_board = game.board

    # Remove all units from the map
    # But remember their original positions for later
    previous_unit_pos = {}
    for unit in self.game.units:
        if unit.position:
            previous_unit_pos[unit.nid] = unit.position
            act = action.LeaveMap(unit)
            act.execute()
    current_tilemap_nid = self.game.level.tilemap.nid
    self.game.level_vars['_prev_pos_%s' % current_tilemap_nid] = previous_unit_pos

    from app.engine.overworld.overworld_states import OverworldFreeState
    OverworldFreeState.set_up_overworld_game_state(overworld_nid)

def set_overworld_position(self: Event, overworld_entity: NID, overworld_location: NID | Tuple[float, float], flags=None):
    flags = flags or set()

    if isinstance(overworld_location, tuple): # setting to a direct coordinate
        display_pos = overworld_location
        overworld = game.overworld_controller
        entity = overworld.entities[overworld_entity]
        if entity is None:
            self.logger.error('{}: Party with NID {} not found.'.format('set_overworld_position', overworld_entity))
            return
        entity.display_position = display_pos
    else: # setting position to node
        overworld_node_nid: NID = overworld_location
        overworld = game.overworld_controller
        entity = overworld.entities[overworld_entity]
        node = overworld.nodes[overworld_node_nid]
        if entity is None or node is None:
            if entity is None:
                self.logger.error('{}: Party with NID {} not found.'.format('set_overworld_position', overworld_entity))
            if node is None:
                self.logger.error('{}: Node with NID {} not found.'.format('set_overworld_position', overworld_node_nid))
            return
        # both node and entity exist
        if overworld_node_nid not in overworld.revealed_node_nids:
            # node isn't enabled yet
            self.logger.error('{}: Node {} exists, but is not yet enabled in overworld {}.'.format('set_overworld_position', overworld_node_nid, overworld._overworld.nid))
            return
        else:
            # both exist and node is enabled
            # move the entity
            entity.on_node = node.nid
    if 'no_animate' not in flags:
        entity.sprite.set_transition('fade_in')

def overworld_move_unit(self: Event, overworld_entity: NID, overworld_location: Tuple[int, int] | NID = None,
                        speed: float = None, point_list: List[Point] = None, flags=None):
    flags = flags or set()
    # default values
    speed_adj = speed
    path = point_list
    entity_nid = overworld_entity
    target_location = overworld_location

    if speed_adj is None:
        speed_adj = 5

    # function
    entity = game.overworld_controller.entities.get(entity_nid, None)
    if not entity:
        self.logger.error("%s: Could not find an entity with nid %s", 'overworld_move_unit', entity_nid)
        return
    if not entity.display_position:
        self.logger.error("%s: Attempting to move entity %s with no position - is it on the overworld?", 'overworld_move_unit', entity_nid)
        return

    follow = 'no_follow' not in flags
    block = 'no_block' not in flags
    disable_after = 'disable_after' in flags
    mute = 'no_sound' in flags

    if disable_after:
        def disable_func():
            entity.sprite.set_transition('fade_out')
            entity.on_node = None
            entity.display_position = None
    else:
        def disable_func():
            pass

    if not entity:
        self.logger.error("%s: Couldn't find entity %s", "overworld_move_unit", entity_nid)
        return
    if not entity.display_position:
        self.logger.error("Unit not on map!")
        return
    if path:
        # i'm disabling linger for arbitrary paths as a default, because i think it makes more sense that way
        movement = OverworldMove(entity, None, game.overworld_controller, event=True, follow=follow, speed_adj=speed_adj, path=path, linger=0, after_move_callback=disable_func, mute=mute)
        if self.do_skip: # we are skipping; resolve this instantly
            entity.display_position = path[-1]
            return
    elif isinstance(target_location, NID):
        target_node: OverworldNodeObject = game.overworld_controller.nodes.get(target_location, None)
        if entity.on_node: # we're pathfinding to a node
            entity_node = game.overworld_controller.nodes[entity.on_node]
            if game.overworld_controller.any_path(entity_node, target_node): # if there is a path from the unit to this node
                if self.do_skip: # we are skipping; resolve this instantly
                    entity.on_node = target_node.nid
                    return
        movement = OverworldMove(entity, target_node, game.overworld_controller, event=True, follow=follow, speed_adj=speed_adj, after_move_callback=disable_func, mute=mute)
    elif isinstance(target_location, Tuple):
        path = [target_location]
        movement = OverworldMove(entity, None, game.overworld_controller, event=True, follow=follow, speed_adj=speed_adj, path=path, linger=0, after_move_callback=disable_func, mute=mute)
        if self.do_skip: # we are skipping; resolve this instantly
            entity.display_position = path[-1]
            return
    else:
        self.logger.error("%s: No valid path or target location specified: path: %s | target_location: %s", "overworld_move_unit", str(path), str(target_location))
        return

    if block:
        movement.queue(game.movement)
        self.state = 'paused'
        game.state.change('overworld_movement')
        return
    # else switch internal state
    if not hasattr(self, 'overworld_movement_manager'):
        self.overworld_movement_manager = OverworldMovementManager(game.overworld_controller)
    movement.queue(self.overworld_movement_manager)
    if follow:
        game.camera.do_slow_pan(1000)
        game.camera.set_center(entity.display_position[0], entity.display_position[1])
    if 'overworld_movement' not in self.should_update.keys(): # queue continuous update function if not exists
        def update_movement(should_skip: bool):
            if should_skip:
                self.overworld_movement_manager.finish_all_movement()
                return True
            # make sure the camera is centered first
            if not game.camera.at_rest():
                return False
            self.overworld_movement_manager.update()
            focal_unit_nid = self.overworld_movement_manager.get_following_unit()
            if focal_unit_nid:
                focal_unit = game.overworld_controller.entities[focal_unit_nid]
                unit_position = focal_unit.display_position
                game.cursor.set_pos((round(unit_position[0]), round(unit_position[1])))
                game.camera.set_center(*unit_position)
            if len(self.overworld_movement_manager) <= 0:
                return True
            return False
        self.should_update['overworld_movement'] = update_movement

def reveal_overworld_node(self: Event, overworld_node_nid, flags=None):
    flags = flags or set()

    overworld = game.overworld_controller
    node = overworld.nodes[overworld_node_nid]
    if not node:
        self.logger.error('{}: Node with NID {} not found'.format('reveal_overworld_node', overworld_node_nid))
        return
    overworld.enable_node(overworld_node_nid)
    if self.do_skip or 'immediate' in flags:
        pass
    else:
        node.sprite.set_transition('fade_in')
        self.wait_time = engine.get_time() + node.sprite.transition_time
        self.state = 'waiting'

def reveal_overworld_road(self: Event, node1, node2, flags=None):
    flags = flags or set()

    overworld = game.overworld_controller
    # both nodes should be enabled
    if node1 not in overworld.revealed_node_nids or node2 not in overworld.revealed_node_nids:
        return
    # both nodes should be neighbors
    if node2 not in overworld.connected_nodes(node1, force=True):
        return

    road = overworld.get_road(node1, node2, True)
    if not road:
        self.logger.error('{}: Road between nodes {} and {} not found'.format('reveal_overworld_road', node1, node2))
        return
    overworld.enable_road(road.nid)
    if self.do_skip or 'immediate' in flags:
        pass
    else:
        road.sprite.set_transition('fade_in')
        self.wait_time = engine.get_time() + road.sprite.transition_time
        self.state = 'waiting'

def create_overworld_entity(self: Event, nid, unit=None, team=None, flags=None):
    flags = flags or set()

    if 'delete' in flags:
        game.overworld_controller.delete_entity(nid)
        return
    if nid in game.overworld_controller.entities.keys():
        self.logger.error('%s: Entity with nid %s already exists', 'create_overworld_entity', nid)
        return
    if unit:
        if unit not in DB.units.keys():
            self.logger.error('%s: No such unit with nid %s', 'create_overworld_entity', unit)
            return
        if team not in DB.teams.keys():
            team = 'player'
        new_entity = OverworldEntityObject.from_unit_prefab(nid, None, unit, team)
        game.overworld_controller.add_entity(new_entity)

def disable_overworld_entity(self: Event, nid, flags=None):
    flags = flags or set()

    entity = game.overworld_controller.entities.get(nid, None)
    if not entity:
        self.logger.error("%s: No such entity exists with nid %s", "disable_overworld_entity", nid)
        return
    if 'no_animate' not in flags:
        entity.sprite.set_transition('fade_out')
    entity.on_node = None
    entity.display_position = None

def toggle_narration_mode(self: Event, direction, speed=None, flags=None):
    toggle_which = direction.lower()
    if speed:
        anim_duration = int(speed)
    else:
        anim_duration = 1000
    # initialize if the component doesn't exist
    if not self.overlay_ui.has_child('event_narration'):
        narration_component = NarrationDialogue('event_narration', self.overlay_ui, anim_duration)
        self.overlay_ui.add_child(narration_component)
        narration_component.disable()
    else:
        narration_component: NarrationDialogue = self.overlay_ui.get_child('event_narration')

    # animate in or out
    if not self.do_skip:
        if toggle_which == 'open':

            narration_component.enter()
            self.wait_time = engine.get_time() + anim_duration
            self.state = 'waiting'
        else:
            narration_component: NarrationDialogue = self.overlay_ui.get_child('event_narration')
            narration_component.exit()
            self.wait_time = engine.get_time() + anim_duration
            self.state = 'waiting'


def narrate(self: Event, speaker, string, flags=None):
    if not self.overlay_ui.has_child('event_narration'):
        self.logger.error("%s: Not currently in overworld narration mode", "overworld_speak")
        return

    narration_component: NarrationDialogue = self.overlay_ui.get_child('event_narration')
    narrator = speaker
    dialogue = string

    # add the hurry up handler if it doesn't already exist
    if 'event_narration_listener' not in self.functions_listening_for_input:
        def handle_input(event):
            narration_component.hurry_up(event)
        self.functions_listening_for_input['event_narration_listener'] = handle_input

    narration_component.push_text(narrator, dialogue)
    if 'no_block' in flags or self.do_skip:
        pass
    else:
        # block state execution while dialogue resolves
        def should_block():
            return not narration_component.finished()
        self.should_remain_blocked.append(should_block)
        self.state = 'blocked'

def set_overworld_menu_option_enabled(self: Event, overworld_node_nid: NID, overworld_node_menu_option: NID, setting: bool, flags=None):
    overworld = self.game.overworld_controller
    overworld.toggle_menu_option_enabled(overworld_node_nid, overworld_node_menu_option, setting)

def set_overworld_menu_option_visible(self: Event, overworld_node_nid: NID, overworld_node_menu_option: NID, setting: bool, flags=None):
    overworld = self.game.overworld_controller
    overworld.toggle_menu_option_visible(overworld_node_nid, overworld_node_menu_option, setting)

def enter_level_from_overworld(self: Event, level_nid: str, flags=None):
    game.overworld_controller.next_level = level_nid
    self.game.level_vars['_enter_level'] = True
