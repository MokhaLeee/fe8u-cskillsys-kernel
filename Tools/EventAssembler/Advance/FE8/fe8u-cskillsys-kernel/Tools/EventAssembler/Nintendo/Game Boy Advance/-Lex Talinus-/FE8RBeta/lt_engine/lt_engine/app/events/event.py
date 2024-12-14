from __future__ import annotations
from app.data.database.units import UnitPrefab
from app.data.resources.portraits import PortraitPrefab
from app.data.resources.resources import RESOURCES
from app.engine.objects.item import ItemObject
from app.engine.objects.skill import SkillObject
from app.engine.text_evaluator import TextEvaluator

import logging
from typing import Any, Callable, Dict, List, Tuple, Optional

import app.engine.config as cf
import app.engine.graphics.ui_framework as uif
from app.constants import WINHEIGHT, WINWIDTH
from app.data.database.database import DB
from app.engine import (action, background, dialog, engine, evaluate,
                        image_mods, item_funcs)
from app.engine.game_state import GameState
from app.engine.movement import movement_funcs
from app.engine.objects.overworld import OverworldNodeObject
from app.engine.objects.unit import UnitObject
from app.engine.sound import get_sound_thread
from app.events import event_commands, triggers
from app.events.event_processor import EventProcessor
from app.events.event_portrait import EventPortrait
from app.events.event_prefab import EventPrefab
from app.events.event_version import EventVersion
from app.events.python_eventing.errors import EventError
from app.events.python_eventing.python_event_processor import PythonEventProcessor
from app.events.python_eventing.utils import SAVE_COMMAND_NIDS
from app.events.speak_style import SpeakStyle
from app.utilities import str_utils, utils, static_random
from app.utilities.data import HasNid
from app.utilities.typing import NID, Color3, Point

class EvaluateException(EventError):
    what = "Could not evaluate expression."

class Event():
    skippable = {"wait", "bop_portrait", "sound",
                 "location_card", "credits", "ending"}

    def __init__(self, event_prefab: EventPrefab, trigger: triggers.EventTrigger, game: GameState = None):
        self._transition_speed: int = 250
        self._transition_color: Color3 = (0, 0, 0)

        self.nid = event_prefab.nid
        self.command_queue: List[event_commands.EventCommand] = []

        self.background = None

        self.trigger = trigger
        event_args = trigger.to_args()
        # alias
        if 'unit1' in event_args:
            event_args['unit'] = event_args['unit1']
        self.unit = event_args.get('unit1', None)
        self.unit2 = event_args.get('unit2', None)
        self.created_unit = None
        self.position = event_args.get('position', None)
        self.local_args = event_args or {}
        if game:
            self.game = game
        else:
            from app.engine.game_state import game
            self.game = game

        self._generic_setup()

        self.text_evaluator = TextEvaluator(self.logger, self.game, self.unit, self.unit2, self.position, self.local_args)
        if event_prefab.version() != EventVersion.EVENT:
            self.processor = PythonEventProcessor(self.nid, event_prefab.source, self.game)
        else:
            self.processor = EventProcessor(self.nid, event_prefab.source, self.text_evaluator)

    def _generic_setup(self):
        self.portraits: Dict[str, EventPortrait] = {}
        self.text_boxes: List[dialog.Dialog] = []
        self.other_boxes: List[Tuple[NID, Any]] = []
        self.overlay_ui = uif.UIComponent.create_base_component()
        self.overlay_ui.name = self.nid
        self.foreground_overlay_ui = uif.UIComponent.create_base_component()
        self.foreground_overlay_ui.name = self.nid

        self.prev_state = None
        self.state = 'processing'

        self.turnwheel_flag = 0  # Whether to enter the turnwheel state after this event is finished
        self.battle_save_flag = 0  # Whether to enter the battle save state after this event is finished

        self.wait_time: int = 0

        # Handles keeping the order that unit sprites should be drawn
        self.priority_counter = 1
        self.do_skip = False
        self.super_skip = False

        # For transition
        self.transition_state = None
        self.transition_progress = 0
        self.transition_update = 0
        self.transition_speed: int = self._transition_speed
        self.transition_color: Color3 = self._transition_color
        self.transition_background: Optional[background.PanoramaBackground] = None

        # For map animations
        self.animations = []

        # a way of passing key input events down to individual events
        # map between name of listener, and listener function
        self.functions_listening_for_input: Dict[str, Callable[[str]]] = {}

        # a way for any arbitrary event to block state processing until an arbitrary condition is fulfilled
        self.should_remain_blocked: List[Callable[[], bool]] = []

        # a method of queueing unblocked actions that require updating (e.g. movement)
        # update functions should return False once they are finished (so they can be removed from the queue)
        # they should receive an argument that represents whether or not we're in skip mode to facilitate skipping
        # and avoid infinite loops.
        # maps name to function
        self.should_update: Dict[str, Callable[[bool], bool]] = {}

        self.logger = logging.getLogger()

    @property
    def unit1(self):
        return self.unit

    def save(self):
        ser_dict = {}
        ser_dict['nid'] = self.nid
        ser_dict['unit1'] = self.unit.nid if self.unit else None
        ser_dict['unit2'] = self.unit2.nid if self.unit2 else None
        ser_dict['position'] = self.position
        ser_dict['local_args'] = {k: action.Action.save_obj(v) for k, v in self.local_args.items()}
        ser_dict['processor_state'] = self.processor.save()
        return ser_dict

    @classmethod
    def restore(cls, ser_dict, game: GameState):
        unit = game.get_unit(ser_dict['unit1'])
        unit2 = game.get_unit(ser_dict['unit2'])
        position = ser_dict['position']
        local_args = ser_dict.get('local_args', {})
        local_args = {k: action.Action.restore_obj(v) for k, v in local_args.items()}
        nid = ser_dict['nid']
        prefab = DB.events.get_by_nid_or_name(nid)[0]
        self = cls(prefab, triggers.GenericTrigger(unit, unit2, position, local_args), game)
        self.processor = EventProcessor.restore(ser_dict['processor_state'], self.text_evaluator)
        return self

    def finished(self):
        return self.processor.finished() and not self.command_queue

    def update(self):
        # update all internal updates, remove the ones that are finished
        self.should_update = {name: to_update for name, to_update in self.should_update.items() if not to_update(self.do_skip)}

        # Update movement so that no_block works correctly
        if self.game.movement:
            self.game.movement.update()

        self._update_state()
        self._update_transition()

    def _update_state(self, dialog_log=True):
        current_time = engine.get_time()
        # Can move through its own internal state up to 5 times in a frame
        counter = 0
        while counter < 5:
            counter += 1
            if self.state != self.prev_state:
                self.prev_state = self.state
                self.logger.debug("Event State: %s", self.state)

            if self.state == 'waiting':
                if current_time > self.wait_time:
                    self.state = 'processing'
                else:
                    break

            elif self.state == 'processing':
                if self.finished():
                    self.end()
                else:
                    self.process()
                if self.state == 'paused':
                    break  # Necessary so we don't go right back to processing

            elif self.state == 'dialog':
                if self.text_boxes:
                    if self.text_boxes[-1].is_done():
                        if dialog_log:
                            speaker = self.text_boxes[-1].speaker
                            plain_text = self.text_boxes[-1].plain_text
                            action.do(action.LogDialog(speaker, plain_text))
                        self.state = 'processing'
                        if self.text_boxes[-1].is_complete():
                            self.text_boxes.pop()
                    elif self.text_boxes[-1].is_paused():
                        self.state = 'processing'
                else:
                    self.state = 'processing'

            elif self.state == 'paused':
                self.state = 'processing'

            elif self.state == 'almost_complete':
                if not self.game.movement or not any([c.grid_move for c in self.game.movement.moving_entities]):
                    self.state = 'complete'

            elif self.state == 'complete':
                break

            elif self.state == 'blocked':
                # state is blocked; try to unblock
                should_still_be_blocked = False
                for check_still_blocked in self.should_remain_blocked:
                    if check_still_blocked(): # we haven't fulfilled unblock conditions
                        should_still_be_blocked = True
                        break
                if should_still_be_blocked:
                    break
                else:
                    # resume execution and clear blockers
                    self.should_remain_blocked.clear()
                    self.state = 'processing'

    def _update_transition(self):
        current_time = engine.get_time()
        # Handle transition
        if self.transition_state:
            perc = (current_time - self.transition_update) / self.transition_speed
            if self.transition_state == 'open':
                perc = 1 - perc
            self.transition_progress = utils.clamp(perc, 0, 1)
            if perc < 0:
                self.transition_state = None

    def take_input(self, event):
        if event == 'START' or event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            self.skip(event == 'START')

        elif event == 'SELECT' or event == 'RIGHT' or event == 'DOWN':
            if self.state == 'dialog':
                if not cf.SETTINGS['talk_boop']:
                    get_sound_thread().play_sfx('Select 1')
                self.hurry_up()

        for listener in self.functions_listening_for_input.values():
            listener(event)

    def draw(self, surf):
        self.animations = [anim for anim in self.animations if not anim.update()]
        for anim in self.animations:
            anim.draw(surf, offset=(-self.game.camera.get_x(), -self.game.camera.get_y()))

        if self.background:
            self.background.draw(surf)

        delete = [key for key, portrait in self.portraits.items() if portrait.update()]
        for key in delete:
            del self.portraits[key]

        # draw all uiframework elements
        ui_surf = self.overlay_ui.to_surf()
        surf.blit(ui_surf, (0, 0))

        sorted_portraits = sorted(self.portraits.values(), key=lambda x: x.priority)
        for portrait in sorted_portraits:
            portrait.draw(surf)

        # Draw other boxes
        self.other_boxes = [(nid, box) for (nid, box) in self.other_boxes if box.update()]
        for _, box in self.other_boxes:
            box.draw(surf)

        # Draw text/dialog boxes
        # if self.state == 'dialog':
        if not self.do_skip:
            to_draw = []
            for dialog_box in reversed(self.text_boxes):
                if not dialog_box.is_complete():
                    to_draw.insert(0, dialog_box)
                if dialog_box.solo_flag:
                    break
            for dialog_box in to_draw:
                dialog_box.update()
                dialog_box.draw(surf)

        # Fade to black
        if self.transition_state:
            s = engine.create_surface((WINWIDTH, WINHEIGHT), transparent=True)
            if self.transition_background:
                self.transition_background.draw(s)
                s = image_mods.make_translucent(s, 1 - self.transition_progress)
            else:
                s.fill((*self.transition_color, int(255 * self.transition_progress)))
            surf.blit(s, (0, 0))

        # draw all achievements
        ui_surf = self.foreground_overlay_ui.to_surf()
        surf.blit(ui_surf, (0, 0))

        return surf

    def end(self):
        self.state = 'almost_complete'

    def process(self):
        while self.state == 'processing':
            if not self.command_queue:
                next_command = self.processor.fetch_next_command()
                if not next_command:
                    break
                self.command_queue.append(next_command)
            command = self.command_queue.pop(0)

            # This shunts all SAVE COMMANDS to the back of the command queue, so that we never save the game while there are still commands on the queue.
            # This is unlikely to happen unless you used a macro to put multiple commands on the queue at once (and one of them is a save).
            while command.nid in SAVE_COMMAND_NIDS and self.command_queue: # if we have a save command but there are still more commands in the queue. We cannot save while there are commands on the queue. This should never happen
                if all([c.nid in SAVE_COMMAND_NIDS for c in self.command_queue]): # avoid infinite loop. This should NEVER happen (will happen if multiple save commands were queued).
                    raise Exception("Queued multiple save commands in event %s, line %d" % (self.nid, self.processor.get_current_line()))
                self.command_queue.append(command)  # Move the save command to the back of the queue
                command = self.command_queue.pop(0)

            self.logger.debug("Run Event Command: %s", command)
            try:
                if self.do_skip and command.nid in self.skippable:
                    pass
                else:
                    self.run_command(command)
            except EventError as e:
                raise e
            except Exception as e:
                raise Exception("Event execution failed with error in command %s" % self.processor.get_source_line(self.processor.get_current_line())) from e

    def skip(self, super_skip: bool = False):
        self.do_skip = True
        self.super_skip = super_skip
        if self.state != 'paused':
            self.state = 'processing'
        self.transition_state = None
        self.hurry_up()
        self.text_boxes.clear()
        self.other_boxes.clear()
        self.should_remain_blocked.clear()
        while self.should_update:
            self.should_update = {name: to_update for name, to_update in self.should_update.items() if not to_update(self.do_skip)}

    def hurry_up(self):
        if self.text_boxes:
            self.text_boxes[-1].hurry_up()

    def run_command(self, command: event_commands.EventCommand):
        from app.events.function_catalog import get_catalog
        self.logger.info('%s: %s, %s', command.nid, command.parameters, command.chosen_flags)
        parameters, flags = command.parameters, command.chosen_flags
        parameters = {str_utils.camel_to_snake(k): v for k, v in parameters.items()}
        self.logger.debug("%s, %s", parameters, flags)
        get_catalog()[command.nid](self, **parameters, flags=flags)

    def _object_to_str(self, obj) -> str:
        if hasattr(obj, 'uid'):
            return str(obj.uid)
        elif hasattr(obj, 'nid'):
            return str(obj.nid)
        else:
            return str(obj)

    def _eval_expr(self, expr: str, from_python: bool) -> Any:
        if from_python:
            return expr
        try:
            return self.text_evaluator.direct_eval(expr)
        except Exception as e:
            line = self.processor.get_current_line()
            exc = EvaluateException(self.nid, line + 1, self.processor.get_source_line(line))
            self.logger.error("'%s' Line %d: Could not evaluate %s (%s)" % (self.nid, line + 1, expr, e))
            exc.what = str(e)
            raise exc

    def _queue_command(self, event_command_str: str):
        try:
            command, _ = event_commands.parse_text_to_command(event_command_str, strict=True)
            if not command:
                raise SyntaxError("Unable to parse command", ("event.py", 0, 0, event_command_str))
            # We need to run convert_parse on these commands also!
            parameters, flags = event_commands.convert_parse(command, None)
            processed_command = command.__class__(parameters, flags, command.display_values)
            self.command_queue.append(processed_command)
        except Exception as e:
            logging.error('_queue_command: Unable to parse command "%s". %s', event_command_str, e)

    def _place_unit(self, unit, position, entry_type, entry_direc=None):
        position = tuple(position)
        if self.do_skip:
            action.do(action.ArriveOnMap(unit, position))
        elif entry_type == 'warp':
            action.do(action.WarpIn(unit, position))
        elif entry_type == 'swoosh':
            action.do(action.SwooshIn(unit, position))
        elif entry_type == 'fade':
            action.do(action.FadeIn(unit, position, entry_direc))
        else:  # immediate
            action.do(action.ArriveOnMap(unit, position))

    def _move_unit(self, movement_type, placement, follow, unit, position):
        position = tuple(position)
        position = self._check_placement(unit, position, placement)
        if not position:
            self.logger.warning("Couldn't determine valid position for %s?", unit.nid)
            return
        if movement_type == 'immediate' or self.do_skip:
            action.do(action.Teleport(unit, position))
        elif movement_type == 'warp':
            action.do(action.Warp(unit, position))
        elif movement_type == 'fade':
            action.do(action.FadeMove(unit, position))
        elif movement_type == 'normal':
            if unit.position == position:
                # Don't bother if identical
                return
            path = self.game.path_system.get_path(unit, position)
            action.do(action.Move(unit, position, path, event=True, follow=follow))
        return position

    def _add_unit_from_direction(self, unit, position, direction, placement) -> bool:
        offsets = [-1, 1, -2, 2, -3, 3, -4, 4, -5, 5, -6, 6, -7, 7]
        final_pos = None

        if direction == 'west':
            test_pos = (0, position[1])
            for x in offsets:
                if movement_funcs.check_traversable(unit, test_pos):
                    final_pos = test_pos
                    break
                else:
                    test_pos = (0, position[1] + x)
        elif direction == 'east':
            test_pos = (self.game.tilemap.width - 1, position[1])
            for x in offsets:
                if movement_funcs.check_traversable(unit, test_pos):
                    final_pos = test_pos
                    break
                else:
                    test_pos = (self.game.tilemap.width - 1, position[1] + x)
        elif direction == 'north':
            test_pos = (position[0], 0)
            for x in offsets:
                if movement_funcs.check_traversable(unit, test_pos):
                    final_pos = test_pos
                    break
                else:
                    test_pos = (position[0] + x, 0)
        elif direction == 'south':
            test_pos = (position[0], self.game.tilemap.height - 1)
            for x in offsets:
                if movement_funcs.check_traversable(unit, test_pos):
                    final_pos = test_pos
                    break
                else:
                    test_pos = (position[1] + x, self.game.tilemap.height - 1)
        if final_pos:
            final_pos = self._check_placement(unit, final_pos, placement)
        if final_pos:
            action.do(action.ArriveOnMap(unit, final_pos))
            return True
        return False

    def _get_position(self, next_pos, unit, group, unit_nid=None):
        if unit_nid is None:
            unit_nid = unit.nid
        if not next_pos:
            position = group.positions.get(unit_nid)
        elif next_pos.lower() == 'starting':
            position = unit.starting_position
        elif ',' in next_pos:
            position = self._parse_pos(next_pos)
        else:
            other_group = self.game.level.unit_groups.get(next_pos)
            position = other_group.positions.get(unit_nid)
        return position

    def _check_placement(self, unit, position, placement):
        if not self.game.tilemap.check_bounds(position):
            self.logger.error("%s: position out of bounds %s", 'check_placement', position)
            return None
        current_occupant = self.game.board.get_unit(position)
        if not current_occupant:
            current_occupant = self.game.movement.check_if_occupied_in_future(position)
        if current_occupant:
            if placement == 'giveup':
                self.logger.warning("Check placement (giveup): Unit already present on tile %s", position)
                return None
            elif placement == 'stack':
                return position
            elif placement == 'closest':
                position = self.game.target_system.get_nearest_open_tile(unit, position)
                if not position:
                    self.logger.warning("Somehow wasn't able to find a nearby open tile")
                    return None
                return position
            elif placement == 'push':
                new_pos = self.game.target_system.get_nearest_open_tile(current_occupant, position)
                if new_pos:
                    action.do(action.ForcedMovement(current_occupant, new_pos))
                    return position
                else:
                    self.logger.error("%s: No open tile found nearby for %s", 'check_placement', position)
                    return None
        else:
            return position

    def _copy_unit(self, unit_nid):
        level_prefab = DB.levels.get(self.game.level.nid)
        level_unit_prefab = level_prefab.units.get(unit_nid)
        if not level_unit_prefab:
            self.logger.warning("Could not find level unit prefab for unit with nid: %s", unit_nid)
            return None
        new_nid = str_utils.get_next_int(level_unit_prefab.nid, self.game.unit_registry.keys())
        new_unit = UnitObject.from_prefab(level_unit_prefab, self.game.current_mode, new_nid)
        new_unit.position = None
        new_unit.dead = False
        new_unit.party = self.game.current_party
        self.game.full_register(new_unit)
        return new_unit

    def _get_item_in_inventory(self, unit_nid: str, item: str, recursive=False) -> tuple[UnitObject, ItemObject]:
        if unit_nid.lower() == 'convoy':
            unit = self.game.get_party()
        else:
            unit = self._get_unit(unit_nid)
            if not unit:
                self.logger.error("Couldn't find unit with nid %s" % unit_nid)
                return None, None
        item_id = item
        if recursive:
            item_list = item_funcs.get_all_items_with_multiitems(unit.items)
        else:
            item_list = unit.items
        inids = [item.nid for item in item_list]
        iuids = [item.uid for item in item_list]
        if (item_id not in inids) and (not str_utils.is_int(item_id) or not int(item_id) in iuids):
            self.logger.error("Couldn't find item with id %s" % item)
            return None, None
        item = [item for item in item_list if (item.nid == item_id or (str_utils.is_int(item_id) and item.uid == int(item_id)))][0]
        return unit, item

    def _get_skill(self, unit_nid: str, skill: str, all_stacks: bool = False) -> tuple[UnitObject, SkillObject | List[SkillObject]]:
        unit = self._get_unit(unit_nid)
        if not unit:
            self.logger.error("Couldn't find unit with nid %s" % unit_nid)
            return None, None
        skill_id = skill
        skill_list = unit.skills
        snids = [skill.nid for skill in skill_list]
        suids = [skill.uid for skill in skill_list]
        if (skill_id not in snids) and (not str_utils.is_int(skill_id) or not int(skill_id) in suids):
            self.logger.error("Couldn't find skill with id %s" % skill)
            return None, None
        skill = [skill for skill in skill_list if (skill.nid == skill_id or (str_utils.is_int(skill_id) and skill.uid == int(skill_id)))]
        if not all_stacks:
            skill = skill[0]
        return unit, skill

    def _resolve_speak_style(self, speaker_or_style, *styles) -> SpeakStyle:
        curr_style = self.game.speak_styles['__default'].copy()
        styles = list(styles)
        if isinstance(speaker_or_style, SpeakStyle):
            o_style = speaker_or_style
        else:
            if not isinstance(speaker_or_style, str):
                speaker_or_style = self._resolve_nid(speaker_or_style)
            o_style = self.game.speak_styles.get(speaker_or_style)
        if o_style:
            styles.append(o_style)
        else:
            curr_style.speaker = speaker_or_style
        for style in styles:
            if isinstance(style, str):
                style = self.game.speak_styles.get(style)
            if style:
                curr_style = curr_style.update(style)
        return curr_style

    def _apply_stat_changes(self, unit, stat_changes, flags):
        # clamp stat changes
        stat_changes = {k: utils.clamp(v, -unit.stats[k], unit.get_stat_cap(k) - unit.stats[k]) for k, v in stat_changes.items()}

        immediate = 'immediate' in flags

        action.do(action.ApplyStatChanges(unit, stat_changes))
        if not immediate:
            self.game.memory['stat_changes'] = stat_changes
            self.game.exp_instance.append((unit, 0, None, 'stat_booster'))
            self.game.state.change('exp')
            self.state = 'paused'

    def _apply_growth_changes(self, unit, growth_changes):
        action.do(action.ApplyGrowthChanges(unit, growth_changes))

    def _parse_pos(self, pos: str | Point, is_float=False):
        if isinstance(pos, tuple):
            return pos
        position = None
        if ',' in pos:
            pos = pos.replace(')', '').replace('(', '').replace('[', '').replace(']', '')
            if is_float:
                position = tuple(float(_) for _ in pos.split(','))
            else:
                position = tuple(int(_) for _ in pos.split(','))
        elif not self.game.is_displaying_overworld() and self._get_unit(pos):
            unit = self._get_unit(pos)
            if unit.position:
                position = unit.position
            else:
                position = self.game.get_rescuers_position(unit)
        elif self.game.is_displaying_overworld() and self._get_overworld_location_of_object(pos):
            position = self._get_overworld_location_of_object(pos).position
        elif pos in self.game.level.regions.keys():
            return self.game.level.regions.get(pos).position
        else:
            valid_regions = \
                [tuple(region.position) for region in self.game.level.regions
                 if pos == region.sub_nid and region.position and
                 not self.game.board.get_unit(region.position)]
            if valid_regions:
                position = static_random.shuffle(valid_regions)[0]
        return position

    def _get_unit(self, unit: UnitPrefab | UnitObject | NID) -> UnitObject:
        return self.game.get_unit(self._resolve_nid(unit))

    def _get_overworld_location_of_object(self, text, entity_only=False, node_only=False) -> OverworldNodeObject:
        if self.game.overworld_controller:
            if not node_only and text in self.game.overworld_controller.entities:
                entity_at_nid = self.game.overworld_controller.entities[text]
                if entity_at_nid.on_node:
                    entity_node = self.game.overworld_controller.nodes[entity_at_nid.on_node]
                    return entity_node

            if not entity_only and text in self.game.overworld_controller.nodes:
                node_at_nid = self.game.overworld_controller.nodes[text]
                if node_at_nid:
                    return node_at_nid
        return None

    def _saturate_portrait(self, portrait):
        for port in self.portraits.values():
            port.desaturate()
        portrait.saturate()

    def _get_portrait(self, obj: UnitObject | UnitPrefab | PortraitPrefab | NID | SpeakStyle) -> Tuple[Optional[PortraitPrefab], str]:
        if isinstance(obj, SpeakStyle):
            nid = obj.speaker
        else:
            nid = self._resolve_nid(obj)
        unit = self._get_unit(nid)
        if unit:
            name = unit.nid
        else:
            name = nid
        if unit and unit.portrait_nid:
            portrait = RESOURCES.portraits.get(unit.portrait_nid)
        elif name in DB.units.keys():
            portrait = RESOURCES.portraits.get(DB.units.get(name).portrait_nid)
        else:
            portrait = RESOURCES.portraits.get(name)
        if not portrait:
            self.logger.error("add_portrait: Couldn't find portrait %s" % name)
            return None, nid
        return portrait, nid

    def _resolve_nid(self, obj: HasNid):
        try:
            return obj.nid
        except:
            return obj