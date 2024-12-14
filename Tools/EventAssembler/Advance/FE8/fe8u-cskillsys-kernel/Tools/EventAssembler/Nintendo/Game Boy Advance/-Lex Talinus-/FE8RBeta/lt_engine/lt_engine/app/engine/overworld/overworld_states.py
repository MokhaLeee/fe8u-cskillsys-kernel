from app.events import triggers
import logging

import app.engine.config as cf
from app.data.database.database import DB
from app.engine import engine, menus
from app.engine.fluid_scroll import FluidScroll
from app.engine.game_state import game
from app.engine.input_manager import get_input_manager
from app.engine.objects.overworld import (OverworldNodeObject,
                                          OverworldNodeProperty, OverworldEntityTypes)
from app.engine.overworld.overworld_actions import OverworldMove
from app.engine.overworld.overworld_manager import OverworldManager
from app.engine.overworld.overworld_map_view import OverworldMapView
from app.engine.overworld.overworld_movement_manager import \
    OverworldMovementManager
from app.engine.sound import get_sound_thread
from app.engine.state import MapState, State
from app.events import triggers
from app.utilities.typing import NID


class OverworldFreeState(MapState):
    """The main overworld state - sprite is on the map and you can navigate around.
    """
    name = 'overworld'

    @classmethod
    def set_up_overworld_game_state(cls, overworld_to_load: NID = None):
        game.generic()

        from .overworld_cursor import OverworldCursor
        from .overworld_map_view import OverworldMapView
        from .overworld_movement_manager import OverworldMovementManager

        if not overworld_to_load:
            if game.game_vars.get('_next_overworld_nid') is not None:
                overworld_to_load: NID = game.game_vars['_next_overworld_nid']
            else:
                # we really shouldn't be here
                game.state.back()
                return
        game.cursor = OverworldCursor(game.camera)
        game.overworld_controller = OverworldManager(game.overworld_registry[overworld_to_load], game.cursor, game.game_vars.get('_next_level_nid'), game.camera)
        game.movement = OverworldMovementManager(game.overworld_controller)
        game.map_view = OverworldMapView(game.overworld_controller, game.cursor)
        game.set_up_game_board(game.overworld_registry[overworld_to_load].tilemap)

        # assign the next level
        if game.overworld_controller.next_level:
            next_level_node = game.overworld_controller.node_by_level(game.overworld_controller.next_level)
            if next_level_node:
                game.overworld_controller.set_node_property(next_level_node.nid, OverworldNodeProperty.IS_NEXT_LEVEL, True)

    def start(self):
        OverworldFreeState.set_up_overworld_game_state()
        logging.info('Fade in Overworld Music')
        song = game.overworld_controller.music
        fade = game.game_vars.get('_phase_music_fade_ms', 400)
        if song:
            get_sound_thread().fade_in(song.nid, fade_in=fade, from_start=True)
        else:
            get_sound_thread().fade_to_pause(fade_out=fade)
        self.begin_time = engine.get_time()

        if not game.overworld_controller.selected_party_node():
            # if it's not, add it to the current level's node and forcibly enable it
            try:
                current_level = DB.levels[DB.levels.index(game.overworld_controller.next_level) - 1].nid
            except:
                current_level = DB.levels.values()[0].nid
            current_level_node = game.overworld_controller.node_by_level(current_level)
            if not current_level_node:
                current_level_node = list(game.overworld_controller.nodes.values())[0]
            if current_level_node not in game.overworld_controller.revealed_nodes:
                game.overworld_controller.enable_node(current_level_node)
            game.overworld_controller.move_party_to_node(game.overworld_controller.selected_entity.nid, current_level_node.nid)
            logging.warning('%s: no position detected. automatically placing party %s at node %s. Use the set_overworld_position command \
                                    in a previous event to circumvent this.',
                            'OverworldFreeState',
                            game.overworld_controller.selected_entity.nid,
                            current_level_node.name)

        game.cursor.set_pos(game.overworld_controller.selected_party_node().position)
        game.camera.force_center(*game.overworld_controller.selected_party_node().position)
        if game.overworld_controller.next_level:
            game.events.trigger(triggers.OverworldStart(), level_nid=game.overworld_controller.next_level)

    def begin(self):
        game.cursor.show()

    def take_input(self, event):
        game.cursor.set_speed_state(get_input_manager().is_pressed('BACK'))
        game.cursor.take_input()

        if event == 'BACK': # flick our cursor back to our party
            party_loc = game.overworld_controller.selected_party_node()
            if party_loc:
                game.cursor.set_pos(party_loc.position)

        elif event == 'SELECT': # this is where the fun begins
            cur_pos = game.cursor.position
            selected_node: OverworldNodeObject = game.overworld_controller.node_at(cur_pos)

            if selected_node: # if we clicked on a node
                entity = game.overworld_controller.entity_at(selected_node.position)
                if entity and entity.team == 'player' and entity.dtype == OverworldEntityTypes.PARTY: # there's a party underneath us, select it and launch the party menu
                    game.overworld_controller.select_entity(entity)
                    get_sound_thread().play_sfx('Select 5')
                    game.state.change('overworld_party_option_menu')
                    return
                else:   # we selected a node without a party
                    party_node = game.overworld_controller.selected_party_node()
                    if game.overworld_controller.any_path(party_node, selected_node):  # if there is a path from our party to this node
                        # if there is an event that will take place upon reaching this node, or this is the next level, stop short and trigger event start
                        if game.events.should_trigger(triggers.OnOverworldNodeSelect(game.overworld_controller.selected_entity, selected_node.nid),
                                                      level_nid=game.overworld_controller.next_level) or selected_node.prefab.level == game.overworld_controller.next_level:
                            movement = OverworldMove(game.overworld_controller.selected_entity, selected_node, game.overworld_controller, event=True, remove_last=True)
                            if selected_node.prefab.level == game.overworld_controller.next_level:
                                game.state.change('overworld_next_level')
                            else:
                                game.game_vars['_target_node_nid'] = selected_node.nid
                                game.state.change('overworld_on_node')
                        else:
                            movement = OverworldMove(game.overworld_controller.selected_entity, selected_node.nid, game.overworld_controller)
                        # queue camera movement to unit
                        game.camera.do_slow_pan(1000)
                        game.camera.set_center(party_node.position[0], party_node.position[1])
                        game.state.change('overworld_movement')
                        movement.queue(game.movement)

            else:   # clicked on empty space, trigger the general menu
                get_sound_thread().play_sfx('Select 5')
                game.state.change('overworld_game_option_menu')

    def update(self):
        super().update()

    def draw(self, surf):
        if isinstance(game.map_view, OverworldMapView):
            return super().draw(surf)
        else:
            return surf

class OverworldMovementState(State):
    """State in which overworld movement is handled and executed.
    """
    name = 'overworld_movement'
    transparent = True
    MOVEMENT_DELAY = 250

    def __init__(self, name):
        super().__init__(name=name)
        self.delay = 0

    def begin(self):
        self.delay = self.MOVEMENT_DELAY
        game.cursor.hide()
        self.cancel_movement = False

    def take_input(self, event):
        if event == 'BACK':
            self.cancel_movement = True

    def update(self):
        # make sure the camera is centered first
        if not game.camera.at_rest():
            return
        # slight delay after camera centers
        # this just looks better
        if self.delay >= 0:
            self.delay -= engine.get_delta()
            return

        # move the game pieces around
        movement: OverworldMovementManager = game.movement
        movement.update()
        if self.cancel_movement == True:
            try:
                movement.interrupt_movement(game.overworld_controller.selected_entity.nid)
                self.cancel_movement = False
            except: # this might be scripted, no interruption
                logging.warning("Exception raised in OverworldMovement cancel_movement handling.")
        # update the camera onto our following unit
        focal_unit_nid: NID = movement.get_following_unit()
        if focal_unit_nid:
            focal_unit = game.overworld_controller.entities[focal_unit_nid]
            unit_position = focal_unit.display_position
            game.cursor.set_pos((round(unit_position[0]), round(unit_position[1])))
            game.camera.set_center(*unit_position)
        if len(game.movement) <= 0:
            game.state.back()
            return 'repeat'

class OverworldNodeTransition(State):
    """State for triggering node events
    """
    name = 'overworld_on_node'
    transparent = True

    def start(self):
        logging.debug("Trigger node arrival event")
        if not game.events.trigger(triggers.OnOverworldNodeSelect(game.overworld_controller.selected_entity.nid, game.game_vars['_target_node_nid']), level_nid=game.overworld_controller.next_level):
            # no events, then just queue the move
            movement = OverworldMove(game.overworld_controller.selected_entity.nid,
                                     game.game_vars['_target_node_nid'],
                                     game.overworld_controller)
            game.state.change('overworld_movement')
            movement.queue(game.movement)
        return 'repeat'

    def update(self):
        game.state.back()
        game.game_vars['_target_node_nid'] = None

class OverworldLevelTransition(State):
    """State handling transition events between overworld and
    new level. This state should only last for one or two updates, maximum.
    """
    name = 'overworld_next_level'
    transparent = True

    def start(self):
        logging.debug("Begin Overworld-Level Transition State")
        if not game.events.trigger(triggers.LevelSelect(), level_nid=game.overworld_controller.next_level):
            # no events, then just queue the move
            movement = OverworldMove(game.overworld_controller.selected_entity.nid,
                                     game.overworld_controller.node_by_level(game.overworld_controller.next_level).nid,
                                     game.overworld_controller)
            game.state.change('overworld_movement')
            movement.queue(game.movement)
        return 'repeat'

    def update(self):
        self.go_to_next_level(game.overworld_controller.next_level)
        return 'repeat'

    def go_to_next_level(self, nid=None):
        game.sweep()
        if not nid:
            next_level_nid = game.overworld_controller.next_level
        else:
            next_level_nid = nid

        game.start_level(next_level_nid)

        game.memory['next_state'] = 'start_level_asset_loading'
        game.state.change('transition_to')

    def end(self):
        return 'repeat'

class OverworldGameOptionMenuState(State):
    """When you click on an empty space, the Unit-Status-Guide-Options-Save menu
    that appears; this is that state.
    """
    name = 'overworld_game_option_menu'
    transparent = True

    def __init__(self, name=None):
        super().__init__(name=name)
        self.fluid = FluidScroll()

    def start(self):
        game.cursor.hide()
        options = ['Unit', 'Status', 'Options', 'Save']
        info_desc = ['Unit_desc', 'Status_desc', 'Options_desc', 'Save_desc']
        ignore = [True, False, False, False]

        unlocked_lore = [lore for lore in DB.lore if lore.nid in game.unlocked_lore and lore.category == 'Guide']
        if unlocked_lore:
            options.insert(2, 'Guide')
            info_desc.insert(2, 'Guide_desc')
            ignore.insert(2, False)
        if cf.SETTINGS['debug']:
            options.insert(0, 'Debug')
            info_desc.insert(0, 'Debug_desc')
            ignore.insert(0, False)
        self.menu = menus.Choice(None, options, info=info_desc)
        self.menu.set_ignore(ignore)

    def make_save(self):
        logging.info('%s: Creating Overworld Save...', 'make_save')
        game.memory['next_state'] = 'title_save'
        game.memory['save_kind'] = 'overworld'
        game.state.change('transition_to')

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.menu.handle_mouse()
        if 'DOWN' in directions:
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_down(first_push)
        elif 'UP' in directions:
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_up(first_push)

        if event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            game.state.back()

        elif event == 'SELECT':
            get_sound_thread().play_sfx('Select 1')
            selection = self.menu.get_current()
            if selection == 'Save':
                self.make_save()
            elif selection == 'Guide':
                game.memory['next_state'] = 'base_guide'
                game.state.change('transition_to')
            elif selection == 'Options':
                game.memory['next_state'] = 'settings_menu'
                game.state.change('transition_to')
            # @TODO Implement these
            # elif selection == 'Status':
            #     pass
            #     # game.memory['next_state'] = 'status_screen'
            #     # game.state.change('transition_to')
            # elif selection == 'Unit':
            #     pass
            #     # game.state.change('unit_menu')
            #     # game.state.change('transition_out')

        elif event == 'INFO':
            self.menu.toggle_info()

    def update(self):
        self.menu.update()

    def draw(self, surf):
        self.menu.draw(surf)
        return surf

class OverworldPartyOptionMenu(State):
    """The party management menu that opens up when clicking on the selected party
    """
    name = 'overworld_party_option_menu'
    transparent = True

    def __init__(self, name=None):
        super().__init__(name=name)
        self.fluid = FluidScroll()

    def start(self):
        game.cursor.hide()
        options = ['Base Camp']
        info_desc = ['Convoy_desc']
        ignore = [False]

        self.events = [None for option in options]
        current_node = game.overworld_controller.selected_party_node()
        all_options = current_node.menu_options

        additional_option_names = [option.option_name for option in all_options if game.overworld_controller.menu_option_visible(current_node.nid, option.nid)]
        additional_ignore = [not game.overworld_controller.menu_option_enabled(current_node.nid, option.nid) for option in all_options if game.overworld_controller.menu_option_visible(current_node.nid, option.nid)]
        additional_events = [option.event for option in all_options if game.overworld_controller.menu_option_visible(current_node.nid, option.nid)]
        additional_info = [None for option in all_options if game.overworld_controller.menu_option_visible(current_node.nid, option.nid)]

        options += additional_option_names
        ignore += additional_ignore
        self.events += additional_events
        #Options seem to need info_descs, no idea what to do with those. They're just None for now to prevent crashes
        info_desc += additional_info

        self.menu = menus.Choice(None, options, info=info_desc)
        self.menu.set_ignore(ignore)

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.menu.handle_mouse()
        if 'DOWN' in directions:
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_down(first_push)
        elif 'UP' in directions:
            get_sound_thread().play_sfx('Select 6')
            self.menu.move_up(first_push)

        if event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            game.state.back()

        elif event == 'SELECT':
            get_sound_thread().play_sfx('Select 1')
            selection = self.menu.get_current()
            if selection == 'Base Camp':
                game.memory['next_state'] = 'base_main'
                game.state.change('transition_to')
            else:
                game.state.back()
                selected_index = self.menu.get_current_index()
                event_to_trigger = self.events[selected_index]
                valid_events = DB.events.get_by_nid_or_name(event_to_trigger, None)
                for event_prefab in valid_events:
                    game.events.trigger_specific_event(event_prefab.nid)

        elif event == 'INFO':
            self.menu.toggle_info()

    def update(self):
        self.menu.update()

    def draw(self, surf):
        self.menu.draw(surf)
        return surf
