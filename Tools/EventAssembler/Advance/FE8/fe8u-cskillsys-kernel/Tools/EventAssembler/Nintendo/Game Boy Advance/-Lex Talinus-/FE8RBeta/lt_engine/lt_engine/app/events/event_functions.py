from __future__ import annotations

import ast
import random
from typing import TYPE_CHECKING, Any, Dict, List, Optional, Tuple

from app.constants import WINHEIGHT, WINWIDTH
from app.data.database.database import DB
from app.data.database.level_units import GenericUnit, UniqueUnit
from app.data.resources.resources import RESOURCES
from app.data.resources.sounds import SFXPrefab, SongPrefab
from app.engine import (action, background, banner, base_surf, dialog, engine,
                        icons, image_mods, item_funcs, item_system,
                        save, skill_system, unit_funcs)
from app.engine.achievements import ACHIEVEMENTS
from app.engine.animations import MapAnimation
from app.engine.combat import interaction
from app.engine.game_menus.menu_components.generic_menu.simple_menu_wrapper import \
    SimpleMenuUI
from app.engine.graphics.text.text_renderer import rendered_text_width, text_width
from app.engine.graphics.ui_framework.premade_animations.animation_templates import (
    fade_anim, translate_anim)
from app.engine.graphics.ui_framework.premade_components.plain_text_component import \
    PlainTextLine
from app.engine.graphics.ui_framework.ui_framework import UIComponent
from app.engine.graphics.ui_framework.ui_framework_animation import \
    InterpolationType
from app.engine import exp_funcs
from app.engine.objects.item import ItemObject
from app.engine.objects.region import RegionObject
from app.engine.objects.tilemap import TileMapObject
from app.engine.objects.unit import UnitObject
from app.engine.persistent_records import RECORDS
from app.engine.sound import SongObject, get_sound_thread
from app.events import event_commands, regions, triggers
from app.events.event_portrait import EventPortrait
from app.events.screen_positions import parse_screen_position
from app.events.speak_style import SpeakStyle
from app.sprites import SPRITES
from app.utilities import str_utils, utils
from app.utilities.enums import Alignments, HAlignment, Orientation, VAlignment
from app.utilities.type_checking import check_valid_type
from app.utilities.typing import NID, Point
from app.engine.source_type import SourceType

if TYPE_CHECKING:
    from app.events.event import Event

def comment(self: Event, flags=None):
    pass

def finish(self: Event, flags=None):
    self.end()

def wait(self: Event, time: int, flags=None):
    current_time = engine.get_time()
    self.wait_time = current_time + time
    self.state = 'waiting'

def end_skip(self: Event, flags=None):
    if not self.super_skip:
        self.do_skip = False

def music(self: Event, music: SongPrefab | SongObject | NID, fade_in=400, flags=None):
    music_nid = self._resolve_nid(music)
    if self.do_skip:
        fade_in = 0
    if music_nid == 'None':
        get_sound_thread().fade_to_pause(fade_out=fade_in)
    else:
        get_sound_thread().fade_in(music_nid, fade_in=fade_in)

def music_fade_back(self: Event, fade_out=400, flags=None):
    if self.do_skip:
        fade_out = 0
    get_sound_thread().fade_back(fade_out=fade_out)

def music_clear(self: Event, fade_out=0, flags=None):
    if self.do_skip:
        fade_out = 0
    if fade_out > 0:
        get_sound_thread().fade_clear(fade_out)
    else:
        get_sound_thread().clear()

def sound(self: Event, sound: SFXPrefab | NID, volume: float=1.0, flags=None):
    sound_nid = self._resolve_nid(sound)
    get_sound_thread().play_sfx(sound_nid, volume=volume)

def stop_sound(self: Event, sound: SFXPrefab | NID, flags=None):
    sound_nid = self._resolve_nid(sound)
    get_sound_thread().stop_sfx(sound_nid)

def change_music(self: Event, phase, music: SongPrefab | SongObject | NID, flags=None):
    music_nid = self._resolve_nid(music)
    if music_nid == 'None':
        action.do(action.ChangePhaseMusic(phase, None))
    else:
        action.do(action.ChangePhaseMusic(phase, music_nid))

def change_special_music(self: Event, special_music_type: str, music: SongPrefab | SongObject | NID, flags=None):
    music_nid = self._resolve_nid(music)
    if special_music_type == 'title_screen':
        # title screen must persist past the current game
        RECORDS.replace('_music_title_screen', music_nid)
    elif special_music_type == 'promotion':
        action.do(action.SetGameVar('_music_promotion', music_nid))
    elif special_music_type == 'class_change':
        action.do(action.SetGameVar('_music_class_change', music_nid))
    elif special_music_type == 'game_over':
        action.do(action.SetGameVar('_music_game_over', music_nid))

def add_portrait(self: Event, portrait, screen_position: Tuple | str, slide=None, expression_list: Optional[List[str]]=None, speed_mult: float=1.0, flags=None):
    flags = flags or set()

    portrait_prefab, name = self._get_portrait(portrait)
    # If already present, don't add
    if name in self.portraits and not self.portraits[name].remove:
        return False

    position, mirror = parse_screen_position(screen_position)

    priority = self.priority_counter
    if 'low_priority' in flags:
        priority -= 1000
    self.priority_counter += 1

    if 'mirror' in flags:
        mirror = not mirror

    transition = True
    if 'immediate' in flags or self.do_skip:
        transition = False
    speed_mult = 1 / max(speed_mult, 0.001)

    new_portrait = EventPortrait(portrait_prefab, position, priority, transition,
                                 slide, mirror, name, speed_mult=speed_mult)
    self.portraits[name] = new_portrait

    new_portrait.set_expression(expression_list or set())

    if 'immediate' in flags or 'no_block' in flags or self.do_skip:
        pass
    else:
        self.wait_time = engine.get_time() + new_portrait.transition_speed + 33  # 16 frames
        self.state = 'waiting'

    return True

def multi_add_portrait(self: Event, portrait1, screen_position1, portrait2, screen_position2,
                       portrait3=None, screen_position3=None, portrait4=None, screen_position4=None, flags=None):
    commands = []
    # Portrait1
    commands.append(event_commands.AddPortrait({'Portrait': portrait1, 'ScreenPosition': screen_position1}, {'no_block'}))
    # Portrait2
    flags = {'no_block'} if portrait3 else set()
    commands.append(event_commands.AddPortrait({'Portrait': portrait2, 'ScreenPosition': screen_position2}, flags))
    if portrait3:
        flags = {'no_block'} if portrait4 else set()
        commands.append(event_commands.AddPortrait({'Portrait': portrait3, 'ScreenPosition': screen_position3}, flags))
    if portrait4:
        commands.append(event_commands.AddPortrait({'Portrait': portrait4, 'ScreenPosition': screen_position4}, set()))
    self.command_queue += commands

def remove_portrait(self: Event, portrait, speed_mult: float = 1.0, slide=None, flags=None):
    flags = flags or set()

    _, name = self._get_portrait(portrait)
    if name not in self.portraits:
        return False

    speed_mult = 1 / max(speed_mult, 0.001)

    if 'immediate' in flags or self.do_skip:
        event_portrait = self.portraits.pop(name)
    else:
        event_portrait = self.portraits[name]
        event_portrait.end(speed_mult, slide)

    if 'immediate' in flags or 'no_block' in flags or self.do_skip:
        pass
    else:
        self.wait_time = engine.get_time() + event_portrait.transition_speed + 33
        self.state = 'waiting'

def multi_remove_portrait(self: Event, portrait1, portrait2, portrait3=None, portrait4=None, flags=None):
    commands = []
    commands.append(event_commands.RemovePortrait({'Portrait': portrait1}, {'no_block'}))
    flags = {'no_block'} if portrait3 else set()
    commands.append(event_commands.RemovePortrait({'Portrait': portrait2}, flags))
    if portrait3:
        flags = {'no_block'} if portrait4 else set()
        commands.append(event_commands.RemovePortrait({'Portrait': portrait3}, flags))
    if portrait4:
        commands.append(event_commands.RemovePortrait({'Portrait': portrait4}, set()))
    self.command_queue += commands

def move_portrait(self: Event, portrait, screen_position: Tuple, speed_mult: float=1.0, flags=None):
    flags = flags or set()

    _, name = self._get_portrait(portrait)
    event_portrait = self.portraits.get(name)
    if not event_portrait:
        return False

    position, _ = parse_screen_position(screen_position)

    if 'immediate' in flags or self.do_skip:
        event_portrait.quick_move(position)
    else:
        event_portrait.move(position, speed_mult)

    if 'immediate' in flags or 'no_block' in flags or self.do_skip:
        pass
    else:
        self.wait_time = engine.get_time() + event_portrait.travel_time + 66
        self.state = 'waiting'

def mirror_portrait(self: Event, portrait, speed_mult: float = 1.0, flags=None):
    flags = flags or set()

    _, name = self._get_portrait(portrait)
    event_portrait = self.portraits.get(name)
    if not event_portrait:
        return False

    speed_mult_calc = 1 / max(speed_mult, 0.001)

    flipped_portrait = \
        EventPortrait(
            event_portrait.portrait,
            event_portrait.position,
            event_portrait.priority,
            False, None, not event_portrait.mirror,
            name, speed_mult=speed_mult_calc)
    if self.text_boxes and self.text_boxes[-1].portrait == event_portrait:
        self.text_boxes[-1].portrait = flipped_portrait

    if self.do_skip:
        self.portraits[name] = flipped_portrait
    else:
        if 'fade' in flags:
            # Removal of portrait also happens
            commands = []
            commands.append(event_commands.RemovePortrait({'Portrait': name, 'SpeedMult': speed_mult}, {'no_block'}))

            command_flags = set()
            if (not event_portrait.mirror) ^ (event_portrait.position[0] < WINWIDTH // 2):
                command_flags.add("mirror")
            if 'no_block' in flags:
                command_flags.add("no_block")
            commands.append(event_commands.AddPortrait({'Portrait': name, 'ScreenPosition': event_portrait.position, 'SpeedMult': speed_mult}, command_flags))
            self.command_queue += commands
        else:
            # Immediate removal followed by a transition in
            self.portraits[name] = flipped_portrait
            if 'no_block' in flags:
                pass
            else:
                self.wait_time = engine.get_time() + event_portrait.transition_speed + 33
                self.state = 'waiting'

def bop_portrait(self: Event, portrait, flags=None):
    flags = flags or set()

    _, name = self._get_portrait(portrait)
    event_portrait = self.portraits.get(name)
    if not event_portrait:
        return False
    event_portrait.bop()
    if 'no_block' in flags:
        pass
    else:
        self.wait_time = engine.get_time() + 666
        self.state = 'waiting'

def expression(self: Event, portrait, expression_list: List[str], flags=None):
    _, name = self._get_portrait(portrait)
    event_portrait = self.portraits.get(name)
    if not event_portrait:
        return False
    event_portrait.set_expression(expression_list)

def speak_style(self: Event, style: NID, speaker: NID=None, position: Alignments | Point=None,
                width: int=None, speed: float=None, font_color: NID=None, font_type: NID=None,
                background: NID=None, num_lines: int=None, draw_cursor: bool=None,
                message_tail: NID=None, transparency: float=None, name_tag_bg: NID=None, flags=None):
    flags = flags or set()
    style_obj = SpeakStyle(style, speaker, position, width, speed, font_color, font_type, background,
                           num_lines, draw_cursor, message_tail, transparency, name_tag_bg, flags)
    if style in self.game.speak_styles:
        style_obj = self.game.speak_styles[style].update(style_obj)
    self.game.speak_styles[style] = style_obj

def say(self: Event, speaker_or_style: str, text: List[str], text_position: Point | Alignments=None, width=None, style_nid=None, text_speed=None,
          font_color=None, font_type=None, dialog_box=None, num_lines=None, draw_cursor=None,
          message_tail=None, transparency=None, name_tag_bg=None, flags=None):
    joined_text = '{sub_break}'.join(text)
    speak(self, speaker_or_style, joined_text, text_position, width, style_nid, text_speed, font_color, font_type, dialog_box, num_lines, draw_cursor,
          message_tail, transparency, name_tag_bg, flags)

def speak(self: Event, speaker_or_style: str, text, text_position: Point | Alignments=None, width=None, style_nid=None, text_speed=None,
          font_color=None, font_type=None, dialog_box=None, num_lines=None, draw_cursor=None,
          message_tail=None, transparency=None, name_tag_bg=None, flags=None):
    flags = flags or set()
    text = dialog.process_dialog_shorthand(text)

    if 'no_block' in flags:
        text += '{no_wait}'

    if draw_cursor:
        cursor = draw_cursor.lower() in self.true_vals
    else:
        cursor = None

    manual_style = SpeakStyle(None, None, text_position, width, text_speed, font_color,
                              font_type, dialog_box, num_lines, cursor, message_tail, transparency, name_tag_bg, flags)

    style = self._resolve_speak_style(speaker_or_style, style_nid, manual_style)
    speaker = style.speaker or ''
    if speaker == 'None':
        speaker = ''
    if speaker.startswith('"') and speaker.endswith('"'):
        speaker = speaker[1:-1]
    unit = self._get_unit(speaker)
    if unit:
        speaker = unit.nid
    portrait = self.portraits.get(speaker)

    if portrait and style.num_lines == 0: # should auto choose 1 or 2 lines
        style.num_lines = 2
        if '{br}' not in text and '{clear}' not in text and text_width(style.font_type, text) < WINWIDTH // 2:
            style.num_lines = 1

    # Process text for commands
    blocks = str_utils.matched_block_expr(text, '{', '}')
    for block in blocks:
        if block.startswith('{command:') and block.endswith('}'):
            event_command_str = block[len('{command:'):-1]
        elif block.startswith('{c:') and block.endswith('}'):
            event_command_str = block[len('{c:'):-1]
        else:
            continue
        text = text.replace(block, '{p}', 1)  # Replace first instance
        self._queue_command(event_command_str)
        if speaker:
            self._queue_command('unpause;%s' % speaker)
        else:
            self._queue_command('unpause')

    flags = style.flags

    # Determine whether this should be skipped
    # Hold speaks are not skipped
    if self.do_skip and 'hold' not in flags:
        pass  # Skip me!
    else:
        autosize = 'fit' in flags
        new_dialog = \
            dialog.Dialog(text, portrait, style.background, style.position, style.width, speaker=speaker,
                          style_nid=style_nid, autosize=autosize, speed=style.speed,
                          font_color=style.font_color, font_type=style.font_type, num_lines=style.num_lines,
                          draw_cursor=style.draw_cursor, message_tail=style.message_tail, transparency=style.transparency,
                          name_tag_bg=style.name_tag_bg, flags=flags)
        self.text_boxes.append(new_dialog)

        if self.do_skip:
            # Which means we must have held, so just process the whole dialog immediately
            new_dialog.warp_speed()
            if self.game.action_log:
                action.do(action.LogDialog(new_dialog.speaker, new_dialog.plain_text))
        elif 'no_block' in flags:
            pass
        else:  # Usually we go to a dialog state
            self.state = 'dialog'
    # End else

    # Bring portrait to forefront
    if portrait and 'low_priority' not in flags:
        portrait.priority = self.priority_counter
        self.priority_counter += 1

def unhold(self: Event, nid, flags=None):
    for box in self.text_boxes:
        if box.style_nid == nid:
            box.hold = False

def unpause(self: Event, nid=None, flags=None):
    if nid is None and self.text_boxes:
        # Just unpause the most recent text box
        box = self.text_boxes[-1]
        box.command_unpause()

    else:
        for box in reversed(self.text_boxes):
            if box.speaker == nid:
                box.command_unpause()
                break
        else:
            if not self.do_skip:  # Don't bother warning if we are skipping
                self.logger.warning("Did not find any text box with speaker: %s", nid)
    if self.do_skip:
        pass
    else:
        self.state = 'dialog'

def transition(self: Event, direction=None, speed=None, color3=None, panorama=None, flags=None):
    flags = flags or set()
    current_time = engine.get_time()
    if direction:
        self.transition_state = direction.lower()
    elif self.transition_state == 'close':
        self.transition_state = 'open'
    else:
        self.transition_state = 'close'
    self.transition_speed = max(1, speed) if speed else self._transition_speed
    self.transition_color = color3 or self._transition_color
    self.transition_background = None
    if panorama:
        panorama = RESOURCES.panoramas.get(panorama)
        if panorama:
            self.transition_background = background.PanoramaBackground(panorama)

    if not self.do_skip:
        self.transition_update = current_time
        if 'no_block' in flags:
            pass
        else:
            self.wait_time = current_time + int(self.transition_speed * 1.33)
            self.state = 'waiting'

def change_background(self: Event, panorama=None, flags=None):
    flags = flags or set()
    if panorama:
        panorama = RESOURCES.panoramas.get(panorama)
        if not panorama:
            return
        self.background = background.PanoramaBackground(panorama)
    else:
        self.background = None
    if 'keep_portraits' in flags:
        pass
    else:
        self.portraits.clear()

def pause_background(self: Event, pause_at=None, flags=None):
    if not self.background:
        self.logger.warning("No current background to pause!")
        return
    self.background.pause(pause_at)

def unpause_background(self: Event, flags=None):
    if not self.background:
        self.logger.warning("No current background to unpause")
        return
    self.background.unpause()

def disp_cursor(self: Event, show_cursor: bool, flags=None):
    if show_cursor:
        self.game.cursor.show()
    else:
        self.game.cursor.hide()

def move_cursor(self: Event, position, speed=None, flags=None):
    flags = flags or set()

    _position = self._parse_pos(position)
    if not _position:
        self.logger.error("move_cursor: Could not determine position from %s" % position)
        return
    position = _position

    self.game.cursor.set_pos(position)
    if 'immediate' in flags or self.do_skip:
        self.game.camera.force_xy(*position)
    else:
        if speed:
            # we are using a custom camera speed
            duration = speed
            self.game.camera.do_slow_pan(duration)
        self.game.camera.set_xy(*position)
        if 'no_block' in flags:
            pass
        else:
            self.game.state.change('move_camera')
            self.state = 'paused'  # So that the message will leave the update loop

def center_cursor(self: Event, position, speed=None, flags=None):
    flags = flags or set()

    _position = self._parse_pos(position)
    if not _position:
        self.logger.error("center_cursor: Could not determine position from %s" % position)
        return
    position = _position

    self.game.cursor.set_pos(position)
    if 'immediate' in flags or self.do_skip:
        self.game.camera.force_center(*position)
    else:
        if speed:
            # we are using a custom camera speed
            duration = speed
            self.game.camera.do_slow_pan(duration)
        self.game.camera.set_center(*position)
        if 'no_block' in flags:
            pass
        else:
            self.game.state.change('move_camera')
            self.state = 'paused'  # So that the message will leave the update loop

def flicker_cursor(self: Event, position, flags=None):
    # This is a macro that just adds new commands to command list
    move_cursor_command = event_commands.MoveCursor({'Position': position}, flags)
    disp_cursor_command1 = event_commands.DispCursor({'ShowCursor': True})
    wait_command = event_commands.Wait({'Time': 1000})
    disp_cursor_command2 = event_commands.DispCursor({'ShowCursor': False})
    self.command_queue += [
        move_cursor_command,
        disp_cursor_command1,
        wait_command,
        disp_cursor_command2
    ]

def screen_shake(self: Event, duration: int, shake_type=None, flags=None):
    flags = flags or set()
    shake_type = shake_type or 'default'

    shake_offset = None
    if shake_type == 'default':
        shake_offset = [(0, -2), (0, -2), (0, 0), (0, 0)]
    elif shake_type == 'combat':
        shake_offset = [(-3, -3), (0, 0), (3, 3), (0, 0)]
    elif shake_type == 'kill':
        shake_offset = [(3, 3), (0, 0), (0, 0), (3, 3), (-3, -3), (3, 3), (-3, -3), (0, 0)]
    elif shake_type == 'random':
        shake_offset = [(random.randint(-4, 4), random.randint(-4, 4)) for _ in range(16)]
    elif shake_type == 'celeste':
        shake_offset = [(random.choice([-1, 1]), random.choice([-1, 1])) for _ in range(16)]

    if not shake_offset:
        self.logger.error("shake mode %s not recognized by screen shake command.", shake_type)
        return

    if self.game.camera:
        self.game.camera.set_shake(shake_offset, duration)
    if self.background:
        self.background.set_shake(shake_offset, duration)
    if 'no_block' in flags:
        pass
    else:
        self.wait_time = engine.get_time() + duration
        self.state = 'waiting'

def screen_shake_end(self: Event, flags=None):
    if self.game.camera:
        self.game.camera.reset_shake()
    if self.background:
        self.background.reset_shake()

def game_var(self: Event, nid, expression, flags=None):
    val = self._eval_expr(expression, 'from_python' in flags)
    if check_valid_type(val):
        action.do(action.SetGameVar(nid, val))
    else:
        self.logger.error("game_var: %s is not a valid variable", val)

def inc_game_var(self: Event, nid, expression=None, flags=None):
    if expression:
        val = self._eval_expr(expression, 'from_python' in flags)
        if check_valid_type(val):
            action.do(action.SetGameVar(nid, self.game.game_vars.get(nid, 0) + val))
        else:
            self.logger.error("inc_game_var: %s is not a valid variable", val)
    else:
        action.do(action.SetGameVar(nid, self.game.game_vars.get(nid, 0) + 1))

def level_var(self: Event, nid, expression, flags=None):
    val = self._eval_expr(expression, 'from_python' in flags)
    if check_valid_type(val):
        action.do(action.SetLevelVar(nid, val))
    else:
        self.logger.error("level_var: %s is not a valid variable", val)

def inc_level_var(self: Event, nid, expression=None, flags=None):
    if expression:
        val = self._eval_expr(expression, 'from_python' in flags)
        if check_valid_type(val):
            action.do(action.SetLevelVar(nid, self.game.level_vars.get(nid, 0) + val))
        else:
            self.logger.error("inc_level_var: %s is not a valid variable", val)
    else:
        action.do(action.SetLevelVar(nid, self.game.level_vars.get(nid, 0) + 1))

def set_next_chapter(self: Event, chapter, flags=None):
    if chapter not in DB.levels.keys():
        self.logger.error("set_next_chapter: %s is not a valid chapter nid" % chapter)
        return
    action.do(action.SetGameVar("_goto_level", chapter))

def enable_convoy(self: Event, activated: bool, flags=None):
    action.do(action.SetGameVar("_convoy", activated))

def enable_supports(self: Event, activated: bool, flags=None):
    action.do(action.SetGameVar("_supports", activated))

def enable_turnwheel(self: Event, activated: bool, flags=None):
    action.do(action.SetGameVar("_turnwheel", activated))

def enable_fog_of_war(self: Event, activated: bool, flags=None):
    action.do(action.SetLevelVar("_fog_of_war", activated))

def set_fog_of_war(self: Event, fog_of_war_type, radius: int, ai_radius: Optional[int]=None, other_radius: Optional[int]=None, flags=None):
    fowt = fog_of_war_type.lower()
    if fowt == 'gba':
        fowt = 1
    elif fowt == 'thracia':
        fowt = 2
    else:
        fowt = 0
    action.do(action.SetLevelVar('_fog_of_war_type', fowt))
    action.do(action.SetLevelVar('_fog_of_war_radius', radius))
    if ai_radius is not None:
        action.do(action.SetLevelVar('_ai_fog_of_war_radius', ai_radius))
    if other_radius is not None:
        action.do(action.SetLevelVar('_ai_fog_of_war_radius', other_radius))

def end_turn(self: Event, team: NID = None, flags=None):
    self.logger.info('Force end of turn.')
    if team is not None:
        if team not in DB.teams.keys():
            self.logger.error("end_turn: %s is not a valid team team nid" % team)
            return
        if not any(unit.team == team for unit in self.game.units if unit.position and 'Tile' not in unit.tags):
            self.logger.error("end_turn: %s has no units on the team" % team)
            return
        # Skip turns until the next team is the one we want
        while self.game.phase.get_next() != team:
            self.game.phase.next()

    if self.game.phase.get_next() == 'player':
        self.game.state.change('turn_change')
        self.game.state.change('status_endstep')
    else:
        self.game.state.change('turn_change')
        self.game.state.change('status_endstep')
        self.game.state.change('ai')
        self.game.ui_view.remove_unit_display()

def win_game(self: Event, flags=None):
    self.game.level_vars['_win_game'] = True

def lose_game(self: Event, flags=None):
    self.game.level_vars['_lose_game'] = True

def main_menu(self: Event, flags=None):
    self.game.level_vars['_main_menu'] = True

def force_chapter_clean_up(self: Event, flags=None):
    self.game.clean_up(full=False)

def skip_save(self: Event, true_or_false: bool, flags=None):
    action.do(action.SetLevelVar('_skip_save', true_or_false))

def activate_turnwheel(self: Event, force: bool=True, flags=None):
    self.turnwheel_flag = 2 if force else 1

def battle_save(self: Event, flags=None):
    flags = flags or set()
    if 'immediately' in flags:
        self.state = 'paused'
        self.game.memory['save_kind'] = 'battle'
        self.game.memory['next_state'] = 'in_chapter_save'
        self.game.state.change('transition_to')
    else:  # Wait until after this event to make the save
        self.battle_save_flag = True

def delete_save(self: Event, save_slot=None, flags=None):
    if save_slot.lower() == 'suspend':
        save.delete_suspend()
    else:
        save.delete_save(self.game, save_slot)

def clear_turnwheel(self: Event, flags=None):
    self.game.action_log.set_first_free_action()

def stop_turnwheel_recording(self: Event, flags=None):
    self.game.action_log.stop_recording()

def start_turnwheel_recording(self: Event, flags=None):
    self.game.action_log.start_recording()

def change_tilemap(self: Event, tilemap, position_offset=None, load_tilemap=None, flags=None):
    """
    Cannot be turnwheeled
    """
    flags = flags or set()

    tilemap_nid = tilemap
    tilemap_prefab = RESOURCES.tilemaps.get(tilemap_nid)
    if not tilemap_prefab:
        self.logger.error("change_tilemap: Couldn't find tilemap %s" % tilemap_nid)
        return

    if position_offset:
        position_offset = tuple(str_utils.intify(position_offset))
    else:
        position_offset = (0, 0)
    if load_tilemap:
        reload_map_nid = load_tilemap
    else:
        reload_map_nid = tilemap_nid

    reload_map = 'reload' in flags
    # For Overworld
    if reload_map and self.game.is_displaying_overworld(): # just go back to the level
        from app.engine import level_cursor, map_view
        from app.engine.movement import movement_system
        self.game.cursor = level_cursor.LevelCursor(self.game)
        self.game.movement = movement_system.MovementSystem(self.game.cursor, self.game.camera)
        self.game.map_view = map_view.MapView()
        self.game.boundary = self.prev_game_boundary
        self.game.board = self.prev_board
        if reload_map and self.game.level_vars.get('_prev_pos_%s' % reload_map_nid):
            for unit_nid, pos in self.game.level_vars['_prev_pos_%s' % reload_map_nid].items():
                # Reload unit's position with position offset
                final_pos = pos[0] + position_offset[0], pos[1] + position_offset[1]
                if self.game.tilemap.check_bounds(final_pos):
                    unit = self.game.get_unit(unit_nid)
                    act = action.ArriveOnMap(unit, final_pos)
                    act.execute()
        return

    # Reset cursor position
    self.game.cursor.set_pos((0, 0))

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

    # Remove all regions from the map
    # But remember their original positions for later
    previous_region_pos = {}
    for region in list(self.game.level.regions):
        if region.position:
            previous_region_pos[region.nid] = region.position
            act = action.RemoveRegion(region)
            act.execute()
    self.game.level_vars['_prev_region_%s' % current_tilemap_nid] = previous_region_pos

    tilemap = TileMapObject.from_prefab(tilemap_prefab)
    self.game.level.tilemap = tilemap
    if self.game.is_displaying_overworld():
        # we were in the overworld before this, so we should probably reset cursor and such
        from app.engine import level_cursor, map_view
        from app.engine.movement import movement_system
        self.game.cursor = level_cursor.LevelCursor(self.game)
        self.game.movement = movement_system.MovementSystem(self.game.cursor, self.game.camera)
        self.game.map_view = map_view.MapView()
    self.game.set_up_game_board(self.game.level.tilemap)

    # If we're reloading the map
    if reload_map and self.game.level_vars.get('_prev_pos_%s' % reload_map_nid):
        for unit_nid, pos in self.game.level_vars['_prev_pos_%s' % reload_map_nid].items():
            # Reload unit's position with position offset
            final_pos = pos[0] + position_offset[0], pos[1] + position_offset[1]
            if self.game.tilemap.check_bounds(final_pos):
                unit = self.game.get_unit(unit_nid)
                act = action.ArriveOnMap(unit, final_pos)
                act.execute()

    if reload_map and self.game.level_vars.get('_prev_region_%s' % reload_map_nid):
        for region_nid, pos in self.game.level_vars['_prev_region_%s' % reload_map_nid].items():
            region = self.game.get_region(region_nid)
            if region:
                region.position = pos[0] + position_offset[0], pos[1] + position_offset[1]
                act = action.AddRegion(region)
                act.execute()

    # Can't use turnwheel to go any further back
    self.game.action_log.set_first_free_action()

def change_bg_tilemap(self: Event, tilemap=None, flags=None):
    flags = flags or set()

    tilemap_nid = tilemap
    tilemap_prefab = RESOURCES.tilemaps.get(tilemap_nid)
    if not tilemap_prefab:
        self.game.level.bg_tilemap = None
        return

    tilemap = TileMapObject.from_prefab(tilemap_prefab)
    action.do(action.ChangeBGTileMap(tilemap))

def set_game_board_bounds(self: Event, min_x: int, min_y: int, max_x: int, max_y: int, flags=None):
    if not self.game.board:
        self.logger.warning("set_game_board_bounds: No game board available")
    elif max_x <= min_x:
        self.logger.warning("set_game_board_bounds: MaxX must be strictly greater than MinX, (MinX: %d, MaxX: %d)", min_x, max_x)
    elif max_y <= min_y:
        self.logger.warning("set_game_board_bounds: MaxY must be strictly greater than MinY, (MinY: %d, MaxY: %d)", min_y, max_y)
    else:
        bounds = (min_x, min_y, max_x, max_y)
        action.do(action.SetGameBoardBounds(bounds))

def remove_game_board_bounds(self: Event, flags=None):
    if self.game.board:
        bounds = (0, 0, self.game.tilemap.width - 1, self.game.tilemap.height - 1)
        action.do(action.SetGameBoardBounds(bounds))
    else:
        self.logger.warning("remove_game_board_bounds: No game board available")

def load_unit(self: Event, unique_unit, team=None, ai=None, flags=None):
    unit_nid = unique_unit
    if self.game.get_unit(unit_nid):
        self.logger.error("load_unit: Unit with NID %s already exists!" % unit_nid)
        return
    unit_prefab = DB.units.get(unit_nid)
    if not unit_prefab:
        self.logger.error("load_unit: Could not find unit %s in database" % unit_nid)
        return
    if not team:
        team = 'player'
    if not ai:
        ai = 'None'
    level_unit_prefab = UniqueUnit(unit_nid, team, ai, None)
    new_unit = UnitObject.from_prefab(level_unit_prefab, self.game.current_mode)
    new_unit.party = self.game.current_party
    self.game.full_register(new_unit)

def make_generic(self: Event, nid, klass, level: int, team, ai=None, faction=None, animation_variant=None, item_list: Optional[List[str]]=None, flags=None):
    assign_unit = False
    # Get input
    unit_nid = nid
    if not unit_nid:
        unit_nid = str_utils.get_next_int('201', [unit.nid for unit in self.game.units])
        assign_unit = True
    elif self.game.get_unit(unit_nid):
        self.logger.error("make_generic: Unit with NID %s already exists!" % unit_nid)
        return

    if klass not in DB.classes.keys():
        self.logger.error("make_generic: Class %s doesn't exist in database " % klass)
        return
    if not ai:
        ai = 'None'
    if not faction:
        faction = DB.factions[0].nid
    starting_items = item_list or []
    level_unit_prefab = GenericUnit(unit_nid, animation_variant, level, klass, faction, starting_items, [], team, ai)
    new_unit = UnitObject.from_prefab(level_unit_prefab, self.game.current_mode)
    new_unit.party = self.game.current_party
    #self.game.full_register(new_unit)
    action.do(action.RegisterUnit(new_unit))

    if assign_unit:
        self.created_unit = new_unit
        self.text_evaluator.local_args['created_unit'] = new_unit

def create_unit(self: Event, unit, nid=None, level: Optional[int]=None, position=None, entry_type=None, placement=None, flags=None):
    flags = flags or set()

    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("create_unit: Couldn't find unit %s" % unit)
        return
    unit = new_unit
    # Get input
    assign_unit = False
    unit_nid = nid
    if not unit_nid:
        unit_nid = str_utils.get_next_int('201', [unit.nid for unit in self.game.units])
        assign_unit = True
    elif self.game.get_unit(unit_nid):
        self.logger.error("create_unit: Unit with NID %s already exists!" % unit_nid)
        return

    level = level or unit.level
    if position:
        position = self._parse_pos(position)
    if not entry_type:
        entry_type = 'fade'
    if not placement:
        placement = 'giveup'

    faction = unit.faction
    if not faction:
        faction = DB.factions[0].nid
    level_unit_prefab = GenericUnit(
        unit_nid, unit.variant, level, unit.klass, faction, [item.nid for item in unit.items], [], unit.team, unit.ai)
    new_unit = UnitObject.from_prefab(level_unit_prefab, self.game.current_mode)

    if 'copy_stats' in flags:
        new_unit.stats = unit.stats.copy()

    new_unit.party = self.game.current_party
    # self.game.full_register(new_unit)
    action.do(action.RegisterUnit(new_unit))
    if assign_unit:
        self.created_unit = new_unit
        self.text_evaluator.created_unit = new_unit
    if DB.constants.value('initiative'):
        action.do(action.InsertInitiative(new_unit))

    if position:
        position = self._check_placement(new_unit, position, placement)
    if position:
        self._place_unit(new_unit, position, entry_type)

def add_unit(self: Event, unit, position=None, entry_type=None, placement=None, animation_type=None, flags=None):
    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("add_unit: Couldn't find unit %s" % unit)
        return
    unit = new_unit
    if unit.position:
        self.logger.error("add_unit: Unit already on map!")
        return
    if unit.dead:
        self.logger.error("add_unit: Unit is dead!")
        return
    # If the unit is already on the map as a traveler
    for u in self.game.get_all_units():
        if u.traveler == unit.nid:
            self.logger.error("add_unit: Unit is already traveling with %s", u.nid)
            return

    position = self._parse_pos(position) if position else unit.starting_position
    if not position:
        self.logger.error("add_unit: No position found!")
        return
    if not entry_type:
        entry_type = 'fade'
    if not placement:
        placement = 'giveup'

    if not animation_type or animation_type == 'fade':
        fade_direction = None
    else:
        fade_direction = animation_type

    position = self._check_placement(unit, position, placement)
    if not position:
        self.logger.error("add_unit: Couldn't get a good position %s %s %s" % (position, entry_type, placement))
        return None
    if DB.constants.value('initiative'):
        action.do(action.InsertInitiative(unit))
    self._place_unit(unit, position, entry_type, fade_direction)

def move_unit(self: Event, unit, position=None, movement_type=None, placement=None, speed=None, flags=None):
    flags = flags or set()

    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("move_unit: Couldn't find unit %s" % unit)
        return
    unit = new_unit
    if not unit.position:
        self.logger.error("move_unit: Unit not on map!")
        return

    position = self._parse_pos(position) if position else unit.starting_position
    if not position:
        self.logger.error("move_unit: No position found!")
        return

    if not movement_type:
        movement_type = 'normal'
    if not placement:
        placement = 'giveup'
    follow = 'no_follow' not in flags

    position = self._check_placement(unit, position, placement)
    if not position:
        self.logger.error("move_unit: Couldn't get a good position %s %s %s" % (position, movement_type, placement))
        return None

    # In case the unit is currently still moving
    if self.game.movement.is_moving(unit):
        self.game.movement.stop(unit)
        unit.sprite.reset()

    if movement_type == 'immediate':
        action.do(action.Teleport(unit, position))
    elif movement_type == 'warp':
        action.do(action.Warp(unit, position))
    elif movement_type == 'swoosh':
        action.do(action.Swoosh(unit, position))
    elif movement_type == 'fade':
        action.do(action.FadeMove(unit, position))
    elif movement_type == 'normal':
        path = self.game.path_system.get_path(unit, position)
        if path:
            if self.do_skip:
                action.do(action.Teleport(unit, position))
            elif speed:
                action.do(action.Move(unit, position, path, event=True, follow=follow, speed=speed))
            else:
                action.do(action.Move(unit, position, path, event=True, follow=follow))
        else:
            self.logger.error("move_unit: no valid path for %s from %s to %s" % (unit, unit.position, position))
            return None

    if 'no_block' in flags or self.do_skip:
        pass
    else:
        self.state = 'paused'
        self.game.state.change('movement')

def remove_unit(self: Event, unit, remove_type=None, animation_type=None, flags=None):
    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("remove_unit: Couldn't find unit %s" % unit)
        return
    unit = new_unit
    if not unit.position:
        self.logger.error("remove_unit: Unit not on map!")
        return
    if not remove_type:
        remove_type = 'fade'
    if not animation_type or animation_type == 'fade':
        fade_direction = None
    else:
        fade_direction = animation_type

    if DB.constants.value('initiative'):
        action.do(action.RemoveInitiative(unit))
    if self.do_skip:
        action.do(action.LeaveMap(unit))
    elif remove_type == 'warp':
        action.do(action.WarpOut(unit))
    elif remove_type == 'swoosh':
        action.do(action.SwooshOut(unit))
    elif remove_type == 'fade':
        action.do(action.FadeOut(unit, fade_direction))
    else:  # immediate
        action.do(action.LeaveMap(unit))

def kill_unit(self: Event, unit, flags=None):
    flags = flags or set()

    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("kill_unit: Couldn't find unit %s" % unit)
        return
    unit = new_unit

    if DB.constants.value('initiative'):
        action.do(action.RemoveInitiative(unit))

    # Separate from their rescuer if currently rescued
    if self.game.get_rescuer(unit):
        rescuer = self.game.get_rescuer(unit)
        action.execute(action.Separate(rescuer, unit, rescuer.position))

    if not unit.position:
        unit.dead = True
    elif 'immediate' in flags:
        unit.dead = True
        action.do(action.Die(unit))
    else:
        self.game.death.should_die(unit)
        self.game.state.change('dying')
    self.game.events.trigger(triggers.UnitDeath(unit, None, unit.position))
    skill_system.on_death(unit)
    self.state = 'paused'

def remove_all_units(self: Event, flags=None):
    for unit in self.game.units:
        if unit.position:
            action.do(action.LeaveMap(unit))

def remove_all_enemies(self: Event, flags=None):
    for unit in self.game.units:
        if unit.position and unit.team in DB.teams.enemies:
            action.do(action.FadeOut(unit))

def interact_unit(self: Event, unit, position, combat_script: Optional[List[str]]=None, ability=None, rounds=1, flags=None):
    flags = flags or set()

    actor = self._get_unit(unit)
    if not actor or not actor.position:
        self.logger.error("interact_unit: Couldn't find %s" % unit)
        return
    target = self._parse_pos(position)
    if not target:
        self.logger.error("interact_unit: Couldn't find target %s" % position)
        return

    script = combat_script

    total_rounds = utils.clamp(rounds, 1, 99)

    items = item_funcs.get_all_items(actor)
    item = None
    # Get item
    if ability:
        item_nid = ability
        for i in items:
            if item_nid == i.nid:
                item = i
                break
        else:  # Create item on the fly
            item_prefab = DB.items.get(ability)
            if not item_prefab:
                self.logger.error("interact_unit: Couldn't find item with nid %s" % ability)
                return
            # Create item
            item = ItemObject.from_prefab(item_prefab)
            item_system.init(item)
            self.game.register_item(item)
    else:
        if actor.get_weapon():
            item = actor.get_weapon()
        elif items:
            item = items[0]
        else:
            self.logger.error("interact_unit: Unit does not have item!")
            return

    interaction.start_combat(
        actor, target, item, event_combat=True, script=script, total_rounds=total_rounds,
        arena='arena' in flags, force_animation='force_animation' in flags, force_no_animation='force_no_animation' in flags)
    self.state = "paused"

def recruit_generic(self: Event, unit, nid, name, flags=None):
    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("recruit_generic: Couldn't find unit %s" % unit)
        return
    unit = new_unit
    action.do(action.SetPersistent(unit))
    action.do(action.SetNid(unit, nid))
    action.do(action.SetName(unit, name))
    for item in unit.items:
        action.do(action.SetItemOwner(item, nid))
    for skill in unit.all_skills:
        action.do(action.SetSkillOwner(skill, nid))

def set_name(self: Event, unit, string, flags=None):
    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("set_name: Couldn't find unit %s" % unit)
        return
    action.do(action.SetName(actor, string))

def set_current_hp(self: Event, unit, hp: int, flags=None):
    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("set_current_hp: Couldn't find unit %s" % unit)
        return
    action.do(action.SetHP(actor, hp))

def set_current_mana(self: Event, unit, mana: int, flags=None):
    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("set_current_mana: Couldn't find unit %s" % unit)
        return
    action.do(action.SetMana(actor, mana))

def add_fatigue(self: Event, unit, fatigue: int, flags=None):
    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("add_fatigue: Couldn't find unit %s" % unit)
        return
    action.do(action.ChangeFatigue(actor, fatigue))

def set_unit_field(self: Event, unit, key, value, flags=None):
    flags = flags or set()

    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("set_unit_field: Couldn't find unit %s" % unit)
        return
    try:
        value = self._eval_expr(value, 'from_python' in flags)
    except:
        self.logger.error("set_unit_field: Could not evaluate {%s}" % value)
        return
    should_increment = False
    if 'increment_mode' in flags:
        should_increment = True
    action.do(action.ChangeField(actor, key, value, should_increment))

def set_unit_note(self: Event, unit, key: str, value: str, flags=None):
    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("set_unit_note: Couldn't find unit %s" % unit)
        return

    action.do(action.SetUnitNote(actor, key, value))

def remove_unit_note(self: Event, unit, key: str, flags=None):
    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("remove_unit_note: Couldn't find unit %s" % unit)
        return

    if key not in (cat for cat, note in actor.notes):
        self.logger.warning("remove_unit_note: Couldn't find note with category of %s" % key)
        return

    action.do(action.RemoveUnitNote(actor, key))

def resurrect(self: Event, global_unit, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("resurrect: Couldn't find unit %s" % global_unit)
        return
    if unit.is_dying:
        self.game.death.miracle(unit)
    elif unit.dead:
        action.do(action.Resurrect(unit))
    action.do(action.Reset(unit))
    action.do(action.SetHP(unit, 1000))

def reset(self: Event, unit, flags=None):
    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("reset: Couldn't find unit %s" % unit)
        return
    action.do(action.Reset(actor))

def has_attacked(self: Event, unit, flags=None):
    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("has_attacked: Couldn't find unit %s" % unit)
        return
    action.do(action.HasAttacked(actor))

def has_traded(self: Event, unit, flags=None):
    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("has_traded: Couldn't find unit %s" % unit)
        return
    action.do(action.HasTraded(actor))

def has_finished(self: Event, unit, flags=None):
    actor = self._get_unit(unit)
    if not actor:
        self.logger.error("has_finished: Couldn't find unit %s" % unit)
        return
    action.do(action.Wait(actor))

def add_group(self: Event, group, starting_group=None, entry_type=None, placement=None, flags=None):
    flags = flags or set()

    new_group = self.game.level.unit_groups.get(group)
    if not new_group:
        self.logger.error("add_group: Couldn't find group %s" % group)
        return
    group = new_group
    next_pos = starting_group
    if not entry_type:
        entry_type = 'fade'
    if not placement:
        placement = 'giveup'
    create = 'create' in flags
    for unit_nid in group.units:
        unit = self.game.get_unit(unit_nid)
        if create:
            unit = self._copy_unit(unit_nid)
            if not unit:
                continue
        elif unit.position or unit.dead:
            continue
        position = self._get_position(next_pos, unit, group, unit_nid)
        if not position:
            continue
        position = tuple(position)
        position = self._check_placement(unit, position, placement)
        if not position:
            self.logger.warning("add_group: Couldn't determine valid position for %s?", unit.nid)
            continue
        if DB.constants.value('initiative'):
            action.do(action.InsertInitiative(unit))
        self._place_unit(unit, position, entry_type)

def spawn_group(self: Event, group, cardinal_direction, starting_group, movement_type=None, placement=None, flags=None):
    flags = flags or set()

    new_group = self.game.level.unit_groups.get(group)
    if not new_group:
        self.logger.error("spawn_group: Couldn't find group %s", group)
        return
    group = new_group
    cardinal_direction = cardinal_direction.lower()
    if cardinal_direction not in ('east', 'west', 'north', 'south'):
        self.logger.error("spawn_group: %s not a legal cardinal direction", cardinal_direction)
        return
    next_pos = starting_group
    if not movement_type:
        movement_type = 'normal'
    if not placement:
        placement = 'giveup'
    create = 'create' in flags
    follow = 'no_follow' not in flags

    for unit_nid in group.units:
        unit = self.game.get_unit(unit_nid)
        if create:
            unit = self._copy_unit(unit_nid)
            if not unit:
                continue
        elif unit.position or unit.dead:
            self.logger.warning("spawn_group: Unit %s in group %s already on map or dead", unit.nid, group.nid)
            continue
        position = self._get_position(next_pos, unit, group, unit_nid)
        if not position:
            continue
        if self._add_unit_from_direction(unit, position, cardinal_direction, placement):
            if DB.constants.value('initiative'):
                action.do(action.InsertInitiative(unit))
            if unit.position != tuple(position):  # Only move unit if we aren't already there
                self._move_unit(movement_type, placement, follow, unit, position)
        else:
            self.logger.error("spawn_group: Couldn't add unit %s to position %s" % (unit.nid, position))

    if 'no_block' in flags or self.do_skip:
        pass
    else:
        self.state = 'paused'
        self.game.state.change('movement')

def move_group(self: Event, group, starting_group, movement_type=None, placement=None, flags=None):
    flags = flags or set()

    new_group = self.game.level.unit_groups.get(group)
    if not new_group:
        self.logger.error("move_group: Couldn't find group %s" % group)
        return
    group = new_group
    next_pos = starting_group
    if not movement_type:
        movement_type = 'normal'
    if not placement:
        placement = 'giveup'
    follow = 'no_follow' not in flags

    for unit_nid in group.units:
        unit = self.game.get_unit(unit_nid)
        if not unit.position:
            continue
        position = self._get_position(next_pos, unit, group)
        if not position:
            continue
        self._move_unit(movement_type, placement, follow, unit, position)

    if 'no_block' in flags or self.do_skip:
        pass
    else:
        self.state = 'paused'
        self.game.state.change('movement')

def remove_group(self: Event, group, remove_type=None, flags=None):
    new_group = self.game.level.unit_groups.get(group)
    if not new_group:
        self.logger.error("remove_group: Couldn't find group %s" % group)
        return
    group = new_group
    if not remove_type:
        remove_type = 'fade'
    for unit_nid in group.units:
        unit = self.game.get_unit(unit_nid)
        if DB.constants.value('initiative'):
            action.do(action.RemoveInitiative(unit))
        if unit.position:
            if self.do_skip:
                action.do(action.LeaveMap(unit))
            elif remove_type == 'warp':
                action.do(action.WarpOut(unit))
            elif remove_type == 'fade':
                action.do(action.FadeOut(unit))
            else:  # immediate
                action.do(action.LeaveMap(unit))

def give_item(self: Event, global_unit_or_convoy, item, party=None, flags=None):
    flags = flags or set()
    global_unit = global_unit_or_convoy

    if global_unit.lower() == 'convoy':
        unit = None
    else:
        unit = self._get_unit(global_unit)
        if not unit:
            self.logger.error("give_item: Couldn't find unit with nid %s" % global_unit)
            return
    item_id = item
    if item_id in DB.items.keys():
        item = item_funcs.create_item(None, item_id)
        self.game.register_item(item)
    elif str_utils.is_int(item_id) and int(item_id) in self.game.item_registry:
        item = self.game.item_registry[int(item_id)]
    else:
        self.logger.error("give_item: Couldn't find item with nid %s" % item_id)
        return
    banner_flag = 'no_banner' not in flags
    action.do(action.SetDroppable(item, 'droppable' in flags))

    if unit:
        if item_funcs.inventory_full(unit, item):
            if 'no_choice' in flags:
                action.do(action.PutItemInConvoy(item))
                if banner_flag:
                    self.game.alerts.append(banner.SentToConvoy(item))
                    self.game.state.change('alert')
                    self.state = 'paused'
            else:
                action.do(action.GiveItem(unit, item))
                self.game.memory['item_discard_current_unit'] = unit
                self.game.state.change('item_discard')
                self.state = 'paused'
                if banner_flag:
                    self.game.alerts.append(banner.AcquiredItem(unit, item))
                    self.game.state.change('alert')
        else:
            action.do(action.GiveItem(unit, item))
            if banner_flag:
                self.game.alerts.append(banner.AcquiredItem(unit, item))
                self.game.state.change('alert')
                self.state = 'paused'
    else:
        action.do(action.PutItemInConvoy(item, party))
        if banner_flag:
            self.game.alerts.append(banner.SentToConvoy(item))
            self.game.state.change('alert')
            self.state = 'paused'

def equip_item(self: Event, global_unit, item, flags=None):
    flags = flags or set()
    recursive_flag = 'recursive' in flags
    item_input = item
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("equip_item: Couldn't find unit with nid %s" % global_unit)
        return
    unit, item = self._get_item_in_inventory(global_unit, item, recursive=recursive_flag)
    if not unit or not item:
        self.logger.error("equip_item: Item %s was invalid, see above" % item_input)
        return
    if item.multi_item:
        for subitem in item.subitems:
            if unit.can_equip(subitem):
                equip_action = action.EquipItem(unit, subitem)
                action.do(equip_action)
                return
        self.logger.error("equip_item: No valid subitem to equip in %s" % item.nid)
    elif unit.can_equip(item):
        equip_action = action.EquipItem(unit, item)
        action.do(equip_action)
    else:
        self.logger.error("equip_item: %s is not an item that can be equipped" % item.nid)

def remove_item(self: Event, global_unit_or_convoy, item, party=None, flags=None):
    flags = flags or set()
    global_unit = global_unit_or_convoy

    unit, item = self._get_item_in_inventory(global_unit, item)
    if not unit or not item:
        self.logger.error("remove_item: Either unit or item was invalid, see above")
        return
    banner_flag = 'no_banner' not in flags

    if global_unit.lower() == 'convoy':
        action.do(action.RemoveItemFromConvoy(item, party))
    else:
        action.do(action.RemoveItem(unit, item))
        if banner_flag:
            item = DB.items.get(item.nid)
            b = banner.TakeItem(unit, item)
            self.game.alerts.append(b)
            self.game.state.change('alert')
            self.state = 'paused'

def move_item(self: Event, giver, receiver, item=None, flags=None):
    flags = flags or set()

    if item:
        unit, item = self._get_item_in_inventory(giver, item)
        if not unit or not item:
            self.logger.error("move_item: Either unit or item was invalid, see above")
            return
    else:
        item = None
        if giver.lower() == 'convoy':
            unit = self.game.get_party()
        else:
            unit = self.game.get_unit(giver)
            if not unit:
                self.logger.error("Could not find unit %s", giver)
                return

        if unit.items:
            item = unit.items[-1]
        else:
            self.logger.warning("Unit %s has no items", giver)
            return

    if giver.lower() == 'convoy':
        if receiver.lower() == 'convoy':
            self.logger.warning("No change, since moving from current convoy to current convoy")
        else:
            other_unit = self.game.get_unit(receiver)
            if not other_unit:
                self.logger.error("Could not find unit %s", receiver)
                return
            if not item_funcs.inventory_full(other_unit, item):
                action.do(action.TakeItemFromConvoy(other_unit, item))
            else:
                self.logger.warning("No space in unit %s's inventory", receiver)
                return
    else:
        if receiver.lower() == 'convoy':
            action.do(action.RemoveItem(unit, item))
            action.do(action.PutItemInConvoy(item))
        else:
            other_unit = self.game.get_unit(receiver)
            if not other_unit:
                self.logger.error("Could not find unit %s", receiver)
                return
            if not item_funcs.inventory_full(other_unit, item):
                action.do(action.MoveItem(unit, other_unit, item))
            else:
                self.logger.warning("No space in unit %s's inventory", receiver)
                return

def move_item_between_convoys(self: Event, item, party1, party2, flags=None):
    giver = self.game.get_party(party1)
    if not giver:
        self.logger.error("Could not find party with nid %s", party1)
        return
    receiver = self.game.get_party(party2)
    if not receiver:
        self.logger.error("Could not find party with nid %s", party2)
        return

    item_id = item
    item_list = giver.items
    item_list = [item_nid.strip() for item_nid in item_list]
    inids = [item.nid for item in item_list]
    iuids = [item.uid for item in item_list]
    if (item_id not in inids) and (not str_utils.is_int(item_id) or not int(item_id) in iuids):
        self.logger.error("Couldn't find item with id %s" % item)
        return
    item = [item for item in item_list if (item.nid == item_id or (str_utils.is_int(item_id) and item.uid == int(item_id)))][0]

    action.do(action.RemoveItemFromConvoy(item, party1))
    action.do(action.PutItemInConvoy(item, party2))

def open_convoy(self: Event, global_unit, flags=None):
    flags = flags or set()
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("open_convoy: Couldn't find unit with nid %s" % global_unit)
        return

    self.state = "paused"
    self.game.memory['current_unit'] = unit
    self.game.memory['next_state'] = 'supply_items'
    self.game.memory['include_other_units'] = 'include_other_units' in flags
    self.game.state.change('transition_to')

def set_item_uses(self: Event, global_unit_or_convoy, item, uses: int, flags=None):
    flags = flags or set()
    global_unit = global_unit_or_convoy
    recursive_flag = 'recursive' in flags
    unit, item = self._get_item_in_inventory(global_unit, item, recursive=recursive_flag)
    if not unit or not item:
        self.logger.error("set_item_uses: Either unit or item was invalid, see above")
        return
    if 'additive' in flags:
        if 'starting_uses' in item.data:
            uses = item.data['uses'] + uses
        elif 'starting_c_uses' in item.data:
            uses = item.data['c_uses'] + uses

    if 'starting_uses' in item.data:
        action.do(action.SetObjData(item, 'uses', utils.clamp(uses, 0, item.data['starting_uses'])))
    elif 'starting_c_uses' in item.data:
        action.do(action.SetObjData(item, 'c_uses', utils.clamp(uses, 0, item.data['starting_c_uses'])))
    else:
        self.logger.error("set_item_uses: Item %s does not have uses!" % item.nid)

def set_item_data(self: Event, global_unit_or_convoy, item, nid, expression, flags=None):
    flags = flags or set()
    global_unit = global_unit_or_convoy

    unit, item = self._get_item_in_inventory(global_unit, item)
    if not unit or not item:
        self.logger.error("set_item_data: Either unit or item was invalid, see above")
        return

    data_value = self._eval_expr(expression, 'from_python' in flags)
    action.do(action.SetObjData(item, nid, data_value))

def set_item_droppable(self: Event, global_unit, item, droppable, flags=None):
    flags = flags or set()

    unit, item = self._get_item_in_inventory(global_unit, item)
    if not unit or not item:
        self.logger.error("set_item_droppable: Either unit or item was invalid, see above")
        return

    action.do(action.SetDroppable(item, droppable))

def break_item(self: Event, global_unit_or_convoy, item, flags=None):
    flags = flags or set()
    global_unit = global_unit_or_convoy

    banner_flag = 'no_banner' not in flags

    unit, item = self._get_item_in_inventory(global_unit, item)
    if not unit or not item:
        self.logger.error("break_item: Either unit or item was invalid, see above")
        return

    if 'starting_uses' in item.data:
        action.do(action.SetObjData(item, 'uses', 0))
    elif 'starting_c_uses' in item.data:
        action.do(action.SetObjData(item, 'c_uses', 0))
    else:
        self.logger.error("break_item: Item %s does not have uses!" % item.nid)
        return

    item_system.on_broken(unit, item)
    should_alert = item_system.alerts_when_broken(unit, item)
    if should_alert and unit.team == 'player' and banner_flag:
        self.game.alerts.append(banner.BrokenItem(unit, item))
        self.game.state.change('alert')
        self.state = 'paused'

def change_item_name(self: Event, global_unit_or_convoy, item, string, flags=None):
    unit, item = self._get_item_in_inventory(global_unit_or_convoy, item)
    if not unit or not item:
        self.logger.error("change_item_name: Either unit or item was invalid, see above")
        return
    action.do(action.ChangeItemName(item, string))

def change_item_desc(self: Event, global_unit_or_convoy, item, string, flags=None):
    unit, item = self._get_item_in_inventory(global_unit_or_convoy, item)
    if not unit or not item:
        self.logger.error("change_item_desc: Either unit or item was invalid, see above")
        return
    action.do(action.ChangeItemDesc(item, string))

def add_item_to_multiitem(self: Event, global_unit_or_convoy, multi_item, child_item, flags=None):
    unit, item = self._get_item_in_inventory(global_unit_or_convoy, multi_item)
    if not unit or not item:
        self.logger.error("add_item_to_multiitem: Either unit or item was invalid, see above")
        return
    if not item.multi_item:
        self.logger.error("add_item_to_muliitem: Item %s is not a multi-item!" % item.nid)
        return
    subitem_prefab = DB.items.get(child_item)
    if not subitem_prefab:
        self.logger.error("add_item_to_multiitem: Couldn't find item with nid %s" % child_item)
        return
    if 'no_duplicate' in flags:
        children = {item.nid for item in item.subitems}
        if child_item in children:
            self.logger.info("add_item_to_multiitem: Item %s already exists on multi-item %s on unit %s" % (child_item, item.nid, unit.nid))
            return
    # Create subitem
    owner_nid = None
    if unit:
        owner_nid = unit.nid
    subitem = item_funcs.create_item(unit, subitem_prefab.nid, assign_ownership=False)
    self.game.register_item(subitem)
    action.do(action.AddItemToMultiItem(owner_nid, item, subitem))
    if 'equip' in flags and owner_nid:
        if unit.can_equip(subitem):
            action.do(action.EquipItem(unit, subitem))
        else:
            self.logger.error("add_item_to_multiitem: Subitem %s could not be equipped" % subitem)

def remove_item_from_multiitem(self: Event, global_unit_or_convoy, multi_item, child_item=None, flags=None):
    unit, item = self._get_item_in_inventory(global_unit_or_convoy, multi_item)
    if not unit or not item:
        self.logger.error("remove_item_from_multiitem: Either unit or item was invalid, see above")
        return
    if not item.multi_item:
        self.logger.error("remove_item_from_multiitem: Item %s is not a multi-item!" % item.nid)
        return
    if global_unit_or_convoy.lower() == 'convoy':
        owner_nid = None
    else:
        owner_nid = unit.nid
    if not child_item:
        # remove all subitems
        subitems = [subitem for subitem in item.subitems]
        for subitem in subitems:
            if owner_nid:
                action.do(action.UnequipItem(unit, subitem))
            action.do(action.RemoveItemFromMultiItem(owner_nid, item, subitem))
    else:
        # Check if item in multiitem
        subitem = None
        subitem_uids = [str(subitem.uid) for subitem in item.subitems]
        if str(child_item) in subitem_uids:
            subitem = [subitem for subitem in item.subitems if str(subitem.uid) == str(child_item)][0]
        else:
            subitem_nids = [subitem.nid for subitem in item.subitems]
            if child_item in subitem_nids:
                subitem = [subitem for subitem in item.subitems if subitem.nid == child_item][0]
        if not subitem:
            self.logger.error("remove_item_from_multiitem: Couldn't find subitem with nid/uid %s" % child_item)
            return
        # Unequip subitem if necessary
        if owner_nid:
            action.do(action.UnequipItem(unit, subitem))
        action.do(action.RemoveItemFromMultiItem(owner_nid, item, subitem))

def add_item_component(self: Event, global_unit_or_convoy, item, item_component, expression=None, flags=None):
    flags = flags or set()
    global_unit = global_unit_or_convoy
    component_nid = item_component
    recursive_flag = 'recursive' in flags
    unit, item = self._get_item_in_inventory(global_unit, item, recursive=recursive_flag)
    if not unit or not item:
        self.logger.error("add_item_component: Either unit or item was invalid, see above")
        return

    if expression is not None:
        component_value = self._eval_expr(expression, 'from_python' in flags)
    else:
        component_value = None

    action.do(action.AddItemComponent(item, component_nid, component_value))

def modify_item_component(self: Event, global_unit_or_convoy, item, item_component, expression, component_property=None, flags=None):
    flags = flags or set()
    global_unit = global_unit_or_convoy
    component_nid = item_component
    is_additive = 'additive' in flags
    recursive_flag = 'recursive' in flags
    unit, item = self._get_item_in_inventory(global_unit, item, recursive=recursive_flag)
    if not unit or not item:
        self.logger.error("modify_item_component: Either unit or item was invalid, see above")
        return

    component_value = self._eval_expr(expression, 'from_python' in flags)
    action.do(action.ModifyItemComponent(item, component_nid, component_value, component_property, is_additive))

def remove_item_component(self: Event, global_unit_or_convoy, item, item_component, flags=None):
    flags = flags or set()
    global_unit = global_unit_or_convoy
    component_nid = item_component
    recursive_flag = 'recursive' in flags
    unit, item = self._get_item_in_inventory(global_unit, item, recursive=recursive_flag)
    if not unit or not item:
        self.logger.error("remove_item_component: Either unit or item was invalid, see above")
        return

    action.do(action.RemoveItemComponent(item, component_nid))

def add_skill_component(self: Event, global_unit, skill, skill_component, expression=None, flags=None):
    flags = flags or set()
    component_nid = skill_component

    if expression is not None:
        component_value = self._eval_expr(expression, 'from_python' in flags)
    else:
        component_value = None

    unit, skill = self._get_skill(global_unit, skill, 'stack' in flags)
    if not unit or not skill:
        self.logger.error("add_skill_component: Either unit or skill was invalid, see above")
        return

    if 'stack' in flags:
        # skill is a List of Skills
        for sk in skill:
            action.do(action.AddSkillComponent(sk, component_nid, component_value))
    else:
        action.do(action.AddSkillComponent(skill, component_nid, component_value))

def modify_skill_component(self: Event, global_unit, skill, skill_component, expression, component_property=None, flags=None):
    flags = flags or set()
    component_nid = skill_component
    is_additive = 'additive' in flags

    unit, skill = self._get_skill(global_unit, skill, 'stack' in flags)
    if not unit or not skill:
        self.logger.error("modify_skill_component: Either unit or skill was invalid, see above")
        return

    component_value = self._eval_expr(expression, 'from_python' in flags)

    if 'stack' in flags:
        # skill is a List of Skills
        for sk in skill:
            action.do(action.ModifySkillComponent(sk, component_nid, component_value, component_property, is_additive))
    else:
        action.do(action.ModifySkillComponent(skill, component_nid, component_value, component_property, is_additive))

def remove_skill_component(self: Event, global_unit, skill, skill_component, flags=None):
    flags = flags or set()
    component_nid = skill_component

    unit, skill = self._get_skill(global_unit, skill, 'stack' in flags)
    if not unit or not skill:
        self.logger.error("remove_skill_component: Either unit or item was invalid, see above")
        return

    if 'stack' in flags:
        for sk in skill:
            action.do(action.RemoveSkillComponent(sk, component_nid))
    else:
        action.do(action.RemoveSkillComponent(skill, component_nid))

def give_money(self: Event, money: int, party=None, flags=None):
    flags = flags or set()

    if party:
        party_nid = party
    else:
        party_nid = self.game.current_party
    banner_flag = 'no_banner' not in flags

    action.do(action.GainMoney(party_nid, money))
    action.do(action.UpdateRecords('money', (party_nid, money)))
    if banner_flag:
        if money >= 0:
            b = banner.Advanced('Got <blue>{money}</> gold.'.format(money = str(money)), 'Item')
        else:
            b = banner.Advanced('Lost <blue>{money}</> gold.'.format(money = str(money * -1)), 'ItemBreak')
        self.game.alerts.append(b)
        self.game.state.change('alert')
        self.state = 'paused'

def give_bexp(self: Event, bexp: int, party=None, string=None, flags=None):
    flags = flags or set()

    if party:
        party_nid = party
    else:
        party_nid = self.game.current_party
    banner_flag = 'no_banner' not in flags

    action.do(action.GiveBexp(party_nid, bexp))

    if banner_flag:
        if string:
            b = banner.Advanced('<blue>{val2}</>: <blue>{val}</> BEXP.'.format(val2=string, val=bexp), 'Item')
        else:
            b = banner.Advanced('Got <blue>{val}</> BEXP.'.format(val=str(bexp)), 'Item')
        self.game.alerts.append(b)
        self.game.state.change('alert')
        self.state = 'paused'

def give_exp(self: Event, global_unit, experience: int, flags=None):
    flags = flags or set()

    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("give_exp: Couldn't find unit with nid %s" % global_unit)
        return
    exp = utils.clamp(experience, -100, 100)
    klass = DB.classes.get(unit.klass)
    max_exp = 100 * (klass.max_level - unit.level) - unit.exp
    exp = min(exp, max_exp)
    if 'silent' in flags:
        old_exp = unit.exp
        if old_exp + exp >= 100:
            if exp_funcs.can_give_exp(unit, exp):
                action.do(action.GainExp(unit, exp))
                # Since autolevel also fills current hp and current mana to max
                # We keep track of HP to reset back
                old_hp, old_mana = unit.get_hp(), unit.get_mana()
                autolevel_to(self, global_unit, unit.level + 1)
                action.do(action.SetHP(unit, old_hp))
                action.do(action.SetMana(unit, old_mana))
            else:
                self.logger.info("give_exp: Cannot raise exp of unit %s" % unit.nid)
        elif old_exp + exp < 0:
            if unit.level > 1:
                action.do(action.SetExp(unit, 100 + old_exp + exp))
                autolevel_to(self, global_unit, unit.level - 1)
            else:
                action.do(action.SetExp(unit, 0))
        elif exp > 0:
            if exp_funcs.can_give_exp(unit, exp):
                action.do(action.SetExp(unit, old_exp + exp))
            else:
                self.logger.info("give_exp: Cannot raise exp of unit %s" % unit.nid)
        else:  # Negative
            action.do(action.SetExp(unit, old_exp + exp))
    else:
        self.game.exp_instance.append((unit, exp, None, 'init'))
        self.game.state.change('exp')
        self.state = 'paused'

def set_exp(self: Event, global_unit, experience: int, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("set_exp: Couldn't find unit with nid %s" % global_unit)
        return
    exp = utils.clamp(experience, 0, 100)
    action.do(action.SetExp(unit, exp))

def give_wexp(self: Event, global_unit, weapon_type, integer: int, flags=None):
    flags = flags or set()

    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("give_wexp: Couldn't find unit with nid %s" % global_unit)
        return
    wexp = integer
    if 'no_banner' in flags:
        action.execute(action.AddWexp(unit, weapon_type, wexp))
    else:
        action.do(action.AddWexp(unit, weapon_type, wexp))
        self.state = 'paused'

def set_wexp(self: Event, global_unit, weapon_type, whole_number: int, flags=None):
    flags = flags or set()

    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("set_wexp: Couldn't find unit with nid %s" % global_unit)
        return
    wexp = whole_number
    if 'no_banner' in flags:
        action.execute(action.SetWexp(unit, weapon_type, wexp))
    else:
        action.do(action.SetWexp(unit, weapon_type, wexp))
        self.state = 'paused'

def give_skill(self: Event, global_unit, skill, initiator=None, flags=None):
    flags = flags or set()

    is_persistent = 'persistent' in flags
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("give_skill: Couldn't find unit with nid %s" % global_unit)
        return
    skill_nid = skill
    if skill_nid not in DB.skills.keys():
        self.logger.error("give_skill: Couldn't find skill with nid %s" % skill)
        return
    if initiator is not None:
        initiator = self._get_unit(initiator)
        if not initiator:
            self.logger.error("Couldn't find unit with nid %s" % initiator)
            return
    banner_flag = 'no_banner' not in flags
    if is_persistent:
        action.do(action.AddSkill(unit, skill_nid, initiator, source=unit.nid, source_type=SourceType.PERSONAL))
    else:
        action.do(action.AddSkill(unit, skill_nid, initiator))
    if banner_flag:
        skill = DB.skills.get(skill_nid)
        b = banner.GiveSkill(unit, skill)
        self.game.alerts.append(b)
        self.game.state.change('alert')
        self.state = 'paused'

def remove_skill(self: Event, global_unit, skill, count=-1, flags=None):
    flags = flags or set()

    unit = self._get_unit(global_unit)
    unit, found_skill = self._get_skill(global_unit, skill)
    if not unit or not found_skill:
        self.logger.error("add_skill_component: Either unit or skill was invalid, see above")
        return
    banner_flag = 'no_banner' not in flags

    if count == 1 or str_utils.is_int(skill):
        # use the object if we specifically requested it as a uid OR we only remove 1
        action.do(action.RemoveSkill(unit, found_skill, count))
    else:
        action.do(action.RemoveSkill(unit, skill, count))
    if banner_flag:
        b = banner.TakeSkill(unit, found_skill)
        self.game.alerts.append(b)
        self.game.state.change('alert')
        self.state = 'paused'

def set_skill_data(self: Event, global_unit, skill, nid, expression, flags=None):
    flags = flags or set()

    unit, found_skill = self._get_skill(global_unit, skill)
    if not unit or not found_skill:
        self.logger.error("add_skill_component: Either unit or skill was invalid, see above")
        return
    data_value = self._eval_expr(expression, 'from_python' in flags)

    action.do(action.SetObjData(found_skill, nid, data_value))

def change_ai(self: Event, global_unit, ai, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_ai: Couldn't find unit %s" % global_unit)
        return
    if ai in DB.ai.keys():
        action.do(action.ChangeAI(unit, ai))
    else:
        self.logger.error("change_ai: Couldn't find AI %s" % ai)
        return

def change_roam_ai(self: Event, global_unit, ai, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_roam_ai: Couldn't find unit %s" % global_unit)
        return
    if ai in DB.ai.keys():
        action.do(action.ChangeRoamAI(unit, ai))
    else:
        self.logger.error("change_roam_ai: Couldn't find AI %s" % ai)
        return

def change_ai_group(self: Event, global_unit, ai_group, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_ai_group: Couldn't find unit %s" % global_unit)
        return
    action.do(action.ChangeAIGroup(unit, ai_group))

def change_party(self: Event, global_unit, party, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_party: Couldn't find unit %s" % global_unit)
        return
    if party in DB.parties.keys():
        action.do(action.ChangeParty(unit, party))
    else:
        self.logger.error("change_party: Couldn't find Party %s" % party)
        return

def change_faction(self: Event, global_unit, faction, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_faction: Couldn't find unit %s" % global_unit)
        return
    if faction in DB.factions.keys():
        action.do(action.ChangeFaction(unit, faction))
    else:
        self.logger.error("change_party: Couldn't find Faction %s" % faction)
        return

def change_team(self: Event, global_unit, team, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_team: Couldn't find unit %s" % global_unit)
        return
    if team in DB.teams.keys():
        action.do(action.ChangeTeam(unit, team))
    else:
        self.logger.error("change_team: Not a valid team: %s" % team)
        return

def change_portrait(self: Event, global_unit, portrait_nid, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_portrait: Couldn't find unit %s" % global_unit)
        return
    portrait = RESOURCES.portraits.get(portrait_nid)
    if not portrait:
        self.logger.error("change_portrait: Couldn't find portrait %s" % portrait_nid)
        return
    action.do(action.ChangePortrait(unit, portrait_nid))

def change_unit_desc(self: Event, global_unit, string, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_unit_desc: Couldn't find unit %s" % global_unit)
        return
    action.do(action.ChangeUnitDesc(unit, string))

def change_affinity(self: Event, global_unit, affinity, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_affinity: Couldn't find unit %s" % global_unit)
        return
    action.do(action.ChangeAffinity(unit, affinity))

def change_stats(self: Event, global_unit, stat_list: Dict[NID, int], flags=None):
    flags = flags or set()

    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_stats: Couldn't find unit %s" % global_unit)
        return

    self._apply_stat_changes(unit, stat_list, flags)

def set_stats(self: Event, global_unit, stat_list: Dict[NID, int], flags=None):
    flags = flags or set()

    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("set_stats: Couldn't find unit %s" % global_unit)
        return

    stat_changes = {}
    for stat, stat_val in stat_list.items():
        current = unit.stats.get(stat, 0)
        stat_changes[stat] = stat_val - current

    self._apply_stat_changes(unit, stat_changes, flags)

def change_growths(self: Event, global_unit, stat_list: Dict[NID, int], flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_growths: Couldn't find unit %s" % global_unit)
        return

    self._apply_growth_changes(unit, stat_list)

def set_growths(self: Event, global_unit, stat_list: Dict[NID, int], flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("set_growths: Couldn't find unit %s" % global_unit)
        return

    growth_changes = {}
    for stat, stat_val in stat_list.items():
        current = unit.growths.get(stat, 0)
        growth_changes[stat] = stat_val - current

    self._apply_growth_changes(unit, growth_changes)

def change_stat_cap_modifiers(self: Event, global_unit, stat_list: Dict[NID, int], flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_stat_cap_modifiers: Couldn't find unit %s" % global_unit)
        return

    action.do(action.ChangeStatCapModifiers(unit, stat_list))

def set_stat_cap_modifiers(self: Event, global_unit, stat_list: Dict[NID, int], flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("set_stat_cap_modifiers: Couldn't find unit %s" % global_unit)
        return

    cap_changes = {}
    for stat, stat_val in stat_list.items():
        current = unit.stat_cap_modifiers.get(stat, 0)
        cap_changes[stat] = stat_val - current

    action.do(action.ChangeStatCapModifiers(unit, cap_changes))

def set_unit_level(self: Event, global_unit, level: int, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("set_unit_level: Couldn't find unit %s" % global_unit)
        return
    if level < 1:
        self.logger.error("Can't set level to less than 1")
        return
    action.do(action.SetLevel(unit, max(1, level)))

def autolevel_to(self: Event, global_unit, level: int, growth_method=None, flags=None):
    flags = flags or set()

    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("autolevel_to: Couldn't find unit %s" % global_unit)
        return
    final_level = level
    current_level = unit.level
    diff = final_level - current_level
    if diff == 0:
        self.logger.warning("autolevel_to: Unit %s is already that level!" % global_unit)
        return

    autolevel_act = action.AutoLevel(unit, diff, growth_method)
    action.do(autolevel_act)
    stat_changes = autolevel_act.stat_changes
    if 'hidden' in flags:
        pass
    else:
        action.do(action.SetLevel(unit, max(1, final_level)))
        event_trigger = triggers.UnitLevelUp(unit, stat_changes, 'event')
        did_trigger = self.game.events.trigger(event_trigger)
        if did_trigger:
            self.state = 'paused'
    if not unit.generic and DB.units.get(unit.nid):
        unit_prefab = DB.units.get(unit.nid)
        personal_skills = unit_funcs.get_personal_skills(unit, unit_prefab, current_level)
        for personal_skill in personal_skills:
            action.do(action.AddSkill(unit, personal_skill))
    class_skills = unit_funcs.get_starting_skills(unit, current_level)
    for class_skill in class_skills:
        action.do(action.AddSkill(unit, class_skill))

def set_mode_autolevels(self: Event, level: int, flags=None):
    flags = flags or set()

    autolevel = level
    if 'hidden' in flags:
        if 'boss' in flags:
            self.game.current_mode.boss_autolevels = autolevel
        else:
            self.game.current_mode.enemy_autolevels = autolevel
    else:
        if 'boss' in flags:
            self.game.current_mode.boss_truelevels = autolevel
        else:
            self.game.current_mode.enemy_truelevels = autolevel

def promote(self: Event, global_unit, klass_list: Optional[List[NID]]=None, flags=None):
    flags = flags or set()
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("promote: Couldn't find unit %s" % global_unit)
        return
    if klass_list:
        if len(klass_list) == 1:
            new_klass = klass_list[0]
        else:
            self.game.memory['promo_options'] = klass_list
            new_klass = None
    else:
        klass = DB.classes.get(unit.klass)
        if len(klass.turns_into) == 0:
            self.logger.error("promote: No available promotions for %s" % klass)
            return
        elif len(klass.turns_into) == 1:
            new_klass = klass.turns_into[0]
        else:
            new_klass = None

    self.game.memory['current_unit'] = unit
    silent = 'silent' in flags
    if self.game.memory.get('promo_options', False):
        if silent:
            self.logger.warning("promote: silent flag set with multiple klass options. Silent will be ignored.")
        self.game.state.change('promotion_choice')
        self.game.state.change('transition_out')
        self.state = 'paused'
    elif silent and new_klass:
        swap_class = action.Promote(unit, new_klass)
        action.do(swap_class)
        #check for new class skill
        unit_klass = DB.classes.get(unit.klass)
        for level_needed, class_skill_nid in unit_klass.learned_skills:
            if unit.level == level_needed:
                if class_skill_nid == 'Feat':
                    self.game.memory['current_unit'] = unit
                    self.game.state.change('feat_choice')
                    self.state = 'paused'
                else:
                    if class_skill_nid not in [skill.nid for skill in unit.skills]:
                        act = action.AddSkill(unit, class_skill_nid)
                        action.do(act)
        _, new_wexp = swap_class.get_data()
        # check for weapon experience gain
        if new_wexp:
            for weapon_nid, value in new_wexp.items():
                # Execute for silent mode
                action.execute(action.AddWexp(unit, weapon_nid, value))
        action.do(action.UpdateRecords('level_gain', (unit.nid, unit.level, unit.klass)))
    elif new_klass:
        self.game.memory['next_class'] = new_klass
        self.game.state.change('promotion')
        self.game.state.change('transition_out')
        self.state = 'paused'
    else:
        self.game.state.change('promotion_choice')
        self.game.state.change('transition_out')
        self.state = 'paused'

def change_class(self: Event, global_unit, klass_list: Optional[List[NID]]=None, flags=None):
    flags = flags or set()

    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("change_class: Couldn't find unit %s" % global_unit)
        return
    if klass_list:
        if len(klass_list) == 1:
            new_klass = klass_list[0]
        else:
            self.game.memory['promo_options'] = klass_list
            new_klass = None
    elif not unit.generic:
        unit_prefab = DB.units.get(unit.nid)
        if not unit_prefab.alternate_classes:
            self.logger.error("change_class: No available alternate classes for %s" % unit)
            return
        elif len(unit_prefab.alternate_classes) == 1:
            new_klass = unit_prefab.alternate_classes[0]
        else:  # Has many alternate classes
            new_klass = None
    else:
        self.logger.error("change_class: No available alternate classes for %s" % unit)
        return

    if new_klass == unit.klass:
        self.logger.error("change_class: No need to change classes")
        return

    self.game.memory['current_unit'] = unit
    silent = 'silent' in flags
    if silent and new_klass:
        swap_class = action.ClassChange(unit, new_klass)
        action.do(swap_class)
        #check for new class skill
        unit_klass = DB.classes.get(unit.klass)
        for level_needed, class_skill_nid in unit_klass.learned_skills:
            if unit.level == level_needed:
                if class_skill_nid == 'Feat':
                    self.game.memory['current_unit'] = unit
                    self.game.state.change('feat_choice')
                    self.state = 'paused'
                else:
                    if class_skill_nid not in [skill.nid for skill in unit.skills]:
                        act = action.AddSkill(unit, class_skill_nid)
                        action.do(act)
        _, new_wexp = swap_class.get_data()
        # check for weapon experience gain
        if new_wexp:
            for weapon_nid, value in new_wexp.items():
                # Execute for silent mode
                action.execute(action.AddWexp(unit, weapon_nid, value))
        action.do(action.UpdateRecords('level_gain', (unit.nid, unit.level, unit.klass)))
    elif new_klass:
        self.game.memory['next_class'] = new_klass
        self.game.state.change('class_change')
        self.game.state.change('transition_out')
        self.state = 'paused'
    else:
        self.game.state.change('class_change_choice')
        self.game.state.change('transition_out')
        self.state = 'paused'

def add_tag(self: Event, global_unit, tag, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("add_tag: Couldn't find unit %s" % global_unit)
        return
    if tag in DB.tags.keys():
        action.do(action.AddTag(unit, tag))

def remove_tag(self: Event, global_unit, tag, flags=None):
    unit = self._get_unit(global_unit)
    if not unit:
        self.logger.error("add_tag: Couldn't find unit %s" % global_unit)
        return
    if tag in DB.tags.keys():
        action.do(action.RemoveTag(unit, tag))

def add_talk(self: Event, unit1, unit2, flags=None):
    u1 = self._get_unit(unit1)
    if not u1:
        self.logger.error("add_talk: Couldn't find unit %s" % unit1)
        return
    u2 = self._get_unit(unit2)
    if not u2:
        self.logger.error("add_talk: Couldn't find unit %s" % unit2)
        return
    action.do(action.AddTalk(u1.nid, u2.nid))

def remove_talk(self: Event, unit1, unit2, flags=None):
    u1 = self._get_unit(unit1)
    if not u1:
        self.logger.error("remove_talk: Couldn't find unit %s" % unit1)
        return
    u2 = self._get_unit(unit2)
    if not u2:
        self.logger.error("remove_talk: Couldn't find unit %s" % unit2)
        return
    action.do(action.RemoveTalk(u1.nid, u2.nid))

def add_lore(self: Event, lore, flags=None):
    action.do(action.AddLore(lore))

def remove_lore(self: Event, lore, flags=None):
    action.do(action.RemoveLore(lore))

def add_base_convo(self: Event, nid, flags=None):
    self.game.base_convos[nid] = False

def ignore_base_convo(self: Event, nid, ignore: bool=True, flags=None):
    if nid in self.game.base_convos:
        self.game.base_convos[nid] = ignore

def remove_base_convo(self: Event, nid, flags=None):
    if nid in self.game.base_convos:
        del self.game.base_convos[nid]

def increment_support_points(self: Event, unit1, unit2, support_points: int, flags=None):
    _unit1 = self._get_unit(unit1)
    if not _unit1:
        _unit1 = DB.units.get(unit1)
    if not _unit1:
        self.logger.error("increment_support_points: Couldn't find unit %s" % unit1)
        return
    unit1 = _unit1
    _unit2 = self._get_unit(unit2)
    if not _unit2:
        _unit2 = DB.units.get(unit2)
    if not _unit2:
        self.logger.error("increment_support_points: Couldn't find unit %s" % unit2)
        return
    unit2 = _unit2
    inc = support_points
    prefabs = DB.support_pairs.get_pairs(unit1.nid, unit2.nid)
    if prefabs:
        prefab = prefabs[0]
        action.do(action.IncrementSupportPoints(prefab.nid, inc))
    else:
        self.logger.error("increment_support_points: Couldn't find prefab for units %s and %s" % (unit1.nid, unit2.nid))
        return

def unlock_support_rank(self: Event, unit1, unit2, support_rank, flags=None):
    _unit1 = self._get_unit(unit1)
    if not _unit1:
        _unit1 = DB.units.get(unit1)
    if not _unit1:
        self.logger.error("unlock_support_rank: Couldn't find unit %s" % unit1)
        return
    _unit2 = self._get_unit(unit2)
    if not _unit2:
        _unit2 = DB.units.get(unit2)
    if not _unit2:
        self.logger.error("unlock_support_rank: Couldn't find unit %s" % unit2)
        return
    rank = support_rank
    if rank not in DB.support_ranks.keys():
        self.logger.error("unlock_support_rank: Support rank %s not a valid rank!" % rank)
        return
    prefabs = DB.support_pairs.get_pairs(_unit1.nid, _unit2.nid)
    if prefabs:
        prefab = prefabs[0]
        action.do(action.UnlockSupportRank(prefab.nid, rank))
    else:
        self.logger.error("unlock_support_rank: Couldn't find prefab for units %s and %s" % (_unit1.nid, _unit2.nid))
        return

def disable_support_rank(self: Event, unit1, unit2, support_rank, flags=None):
    _unit1 = self._get_unit(unit1)
    if not _unit1:
        _unit1 = DB.units.get(unit1)
    if not _unit1:
        self.logger.error("disable_support_rank: Couldn't find unit %s" % unit1)
        return
    _unit2 = self._get_unit(unit2)
    if not _unit2:
        _unit2 = DB.units.get(unit2)
    if not _unit2:
        self.logger.error("disable_support_rank: Couldn't find unit %s" % unit2)
        return
    rank = support_rank
    if rank not in DB.support_ranks.keys():
        self.logger.error("disable_support_rank: Support rank %s not a valid rank!" % rank)
        return
    prefabs = DB.support_pairs.get_pairs(_unit1.nid, _unit2.nid)
    if prefabs:
        prefab = prefabs[0]
        action.do(action.DisableSupportRank(prefab.nid, rank))
    else:
        self.logger.error("disable_support_rank: Couldn't find prefab for units %s and %s" % (_unit1.nid, _unit2.nid))
        return

def add_market_item(self: Event, item, stock=-1, flags=None):
    if item not in DB.items.keys():
        self.logger.warning("add_market_item: %s is not a legal item nid", item)
        return
    if stock > -1:
        if item in self.game.market_items:
            self.game.market_items[item] += stock
        else:
            self.game.market_items[item] = stock
    else:
        self.game.market_items[item] = -1  # Any negative number means infinite

def remove_market_item(self: Event, item, stock: int=0, flags=None):
    if item not in DB.items.keys():
        self.logger.warning("remove_market_item: %s is not a legal item nid", item)
        return
    if stock and item in self.game.market_items:
        self.game.market_items[item] -= stock
        if self.game.market_items[item] <= 0:
            self.game.market_items.pop(item, None)
    else:
        self.game.market_items.pop(item, None)

def clear_market_items(self: Event, flags=None):
    self.game.market_items.clear()

def add_region(self: Event, region, position, size: Tuple, region_type, string=None, time_left=None, flags=None):
    flags = flags or set()

    if region in self.game.level.regions.keys():
        self.logger.error("add_region: RegionObject nid %s already present!" % region)
        return
    position = self._parse_pos(position)
    if not size:
        size = (1, 1)
    region_type = region_type.lower()
    sub_region_type = string

    new_region = RegionObject(region, regions.RegionType(region_type))
    new_region.position = position
    new_region.size = size
    new_region.sub_nid = sub_region_type
    new_region.time_left = time_left

    if 'only_once' in flags:
        new_region.only_once = True
    if 'interrupt_move' in flags:
        new_region.interrupt_move = True

    self.game.register_region(new_region)
    action.do(action.AddRegion(new_region))

def region_condition(self: Event, region, expression, flags=None):
    if region in self.game.level.regions.keys():
        region = self.game.level.regions.get(region)
        action.do(action.ChangeRegionCondition(region, expression))
    else:
        self.logger.error("region_condition: Couldn't find RegionObject %s" % region)

def remove_region(self: Event, region, flags=None):
    if region in self.game.level.regions.keys():
        region = self.game.level.regions.get(region)
        action.do(action.RemoveRegion(region))
    else:
        self.logger.error("remove_region: Couldn't find RegionObject %s" % region)

def remove_generics_from_region(self: Event, nid, flags=None):
    if nid in self.game.level.regions.keys():
        region = self.game.level.regions.get(nid)
        for position in region.get_all_positions():
            unit = self.game.get_unit(position)
            if unit and unit.generic:
                action.execute(action.LeaveMap(unit))
    else:
        self.logger.error("remove_generics_from_region: Couldn't find RegionObject %s" % nid)

def show_layer(self: Event, layer, layer_transition=None, flags=None):
    if layer not in self.game.level.tilemap.layers.keys():
        self.logger.error("show_layer: Could not find layer %s in tilemap" % layer)
        return
    if not layer_transition:
        layer_transition = 'fade'

    if self.game.level.tilemap.layers.get(layer).visible:
        self.logger.warning("show_layer: Layer %s is already visible!" % layer)
        return
    action.do(action.ShowLayer(layer, layer_transition))

def hide_layer(self: Event, layer, layer_transition=None, flags=None):
    if layer not in self.game.level.tilemap.layers.keys():
        self.logger.error("hide_layer: Could not find layer %s in tilemap" % layer)
        return
    if not layer_transition:
        layer_transition = 'fade'

    if not self.game.level.tilemap.layers.get(layer).visible:
        self.logger.warning("hide_layer: Layer %s is already hidden!" % layer)
        return
    action.do(action.HideLayer(layer, layer_transition))

def add_weather(self: Event, weather, position=None, flags=None):
    nid = weather.lower()
    pos = self._parse_pos(position) if position else None
    action.do(action.AddWeather(nid, pos))

def remove_weather(self: Event, weather, position=None, flags=None):
    nid = weather.lower()
    pos = self._parse_pos(position) if position else None
    action.do(action.RemoveWeather(nid, pos))

def change_objective_simple(self: Event, evaluable_string, flags=None):
    action.do(action.ChangeObjective('simple', evaluable_string))

def change_objective_win(self: Event, evaluable_string, flags=None):
    action.do(action.ChangeObjective('win', evaluable_string))

def change_objective_loss(self: Event, evaluable_string, flags=None):
    action.do(action.ChangeObjective('loss', evaluable_string))

def set_position(self: Event, position, flags=None):
    pos = self._parse_pos(position)
    self.position = pos
    self.text_evaluator.position = pos

def map_anim(self: Event, map_anim, float_position: Tuple[float, float] | NID, speed: float=1.0, flags=None):
    flags = flags or set()
    float_position = self._parse_pos(float_position, True)
    if map_anim not in RESOURCES.animations.keys():
        self.logger.error("map_anim: Could not find map animation %s" % map_anim)
        return
    mode = engine.BlendMode.NONE
    if 'blend' in flags:
        mode = engine.BlendMode.BLEND_RGB_ADD
    elif 'multiply' in flags:
        mode = engine.BlendMode.BLEND_RGB_MULT
    if 'permanent' in flags:
        action.do(action.AddMapAnim(map_anim, float_position, speed, mode, 'overlay' in flags))
    else:
        anim = RESOURCES.animations.get(map_anim)
        anim = MapAnimation(anim, float_position, speed_adj=speed)
        anim.set_tint(mode)
        self.animations.append(anim)

    if 'no_block' in flags or self.do_skip or 'permanent' in flags:
        pass
    else:
        self.wait_time = engine.get_time() + anim.get_wait()
        self.state = 'waiting'

def remove_map_anim(self: Event, map_anim, position, flags=None):
    flags = flags or set()
    pos = self._parse_pos(position, True)
    action.do(action.RemoveMapAnim(map_anim, pos, 'overlay' in flags))

def add_unit_map_anim(self: Event, map_anim: NID, unit: NID, speed: float=1.0, flags=None):
    flags = flags or set()

    if map_anim not in RESOURCES.animations.keys():
        self.logger.error("add_unit_map_anim: Could not find map animation %s" % map_anim)
        return
    unit_nid = unit
    unit = self._get_unit(unit_nid)
    if not unit:
        self.logger.error("add_unit_map_anim: Could not find unit %s" % unit_nid)
        return
    if 'permanent' in flags:
        action.do(action.AddAnimToUnit(map_anim, unit, speed, 'blend' in flags))
    else:
        anim = RESOURCES.animations.get(map_anim)
        pos = unit.position
        if pos:
            anim = MapAnimation(anim, pos, speed_adj=speed)
            anim.set_tint('blend' in flags)
            self.animations.append(anim)

    if 'no_block' in flags or self.do_skip or 'permanent' in flags:
        pass
    else:
        self.wait_time = engine.get_time() + anim.get_wait()
        self.state = 'waiting'

def remove_unit_map_anim(self: Event, map_anim, unit, flags=None):
    unit = self._get_unit(unit)
    if not unit:
        self.logger.error("remove_unit_map_anim: Could not find unit %s" % unit)
        return
    action.do(action.RemoveAnimFromUnit(map_anim, unit))

def merge_parties(self: Event, party1, party2, flags=None):
    host, guest = party1, party2
    if host not in DB.parties.keys():
        self.logger.error("merge_parties: Could not locate party %s" % host)
        return
    if guest not in DB.parties.keys():
        self.logger.error("merge_parties: Could not locate party %s" % guest)
        return
    guest_party = self.game.get_party(guest)
    # Merge units
    for unit in self.game.units:
        if unit.party == guest:
            action.do(action.ChangeParty(unit, host))
    # Merge items
    for item in guest_party.convoy:
        action.do(action.RemoveItemFromConvoy(item, guest))
        action.do(action.PutItemInConvoy(item, host))
    # Merge money
    action.do(action.GainMoney(host, guest_party.money))
    action.do(action.GainMoney(guest, -guest_party.money))
    # Merge bexp
    action.do(action.GiveBexp(host, guest_party.bexp))
    action.do(action.GiveBexp(guest, -guest_party.bexp))

def arrange_formation(self: Event, flags=None):
    player_units = self.game.get_units_in_party()
    stuck_units = [unit for unit in player_units if unit.position and not self.game.check_for_region(unit.position, 'formation')]
    unstuck_units = [unit for unit in player_units if unit not in stuck_units and not self.game.check_for_region(unit.position, 'formation')]
    unstuck_units = [unit for unit in unstuck_units if 'Blacklist' not in unit.tags]
    if DB.constants.value('fatigue') and self.game.game_vars.get('_fatigue') == 1:
        unstuck_units = [unit for unit in unstuck_units if unit.get_fatigue() < unit.get_max_fatigue()]
    # Place required units first
    unstuck_units = list(sorted(unstuck_units, key=lambda u: 'Required' in u.tags, reverse=True))
    num_slots = self.game.level_vars.get('_prep_slots')
    all_formation_spots = self.game.get_open_formation_spots()
    if num_slots is None:
        num_slots = len(all_formation_spots)
    assign_these = unstuck_units[:num_slots]
    for idx, unit in enumerate(assign_these):
        if len(all_formation_spots) > idx:
            position = all_formation_spots[idx]
            action.execute(action.ArriveOnMap(unit, position))
            action.execute(action.Reset(unit))

def prep(self: Event, pick_units_enabled: bool = False, music: SongPrefab | SongObject | NID = None, other_options: List[str] = None,
         other_options_enabled: List[Optional[bool]] = None, other_options_on_select: List[Optional[bool]] = None, flags=None):
    action.do(action.SetLevelVar('_prep_pick', pick_units_enabled))
    music_nid = self._resolve_nid(music)
    if music_nid:
        action.do(action.SetGameVar('_prep_music', music_nid))

    if other_options:
        options_list = other_options or []
        options_enabled = other_options_enabled or []
        options_events = other_options_on_select or []

        if len(options_enabled) <= len(options_list):
            options_enabled += [False] * (len(options_list) - len(options_events))
            action.do(action.SetGameVar('_prep_options_enabled', options_enabled))
        else:
            self.logger.error("prep: too many bools in option enabled list: ", other_options_enabled)
            return

        if len(options_events) <= len(options_list):
            options_events += [None] * (len(options_list) - len(options_events))
            action.do(action.SetGameVar('_prep_options_events', options_events))
        else:
            self.logger.error("prep: too many events in option event list: ", other_options_on_select)
            return
        action.do(action.SetGameVar('_prep_additional_options', options_list))
    else:
        action.do(action.SetGameVar('_prep_options_enabled', []))
        action.do(action.SetGameVar('_prep_options_events', []))
        action.do(action.SetGameVar('_prep_additional_options', []))

    self.game.state.change('prep_main')
    self.state = 'paused'  # So that the message will leave the update loop

def base(self: Event, background: str, music: SongPrefab | SongObject | NID = None, other_options: List[str] = None,
         other_options_enabled: List[bool] = None, other_options_on_select: List[bool] = None, flags=None):
    flags = flags or set()

    # set panorama
    action.do(action.SetGameVar('_base_bg_name', background))
    # set music
    music_nid = self._resolve_nid(music)
    if music_nid:
        action.do(action.SetGameVar('_base_music', music_nid))

    if other_options:
        options_list = other_options or []
        options_enabled = other_options_enabled or []
        options_events = other_options_on_select or []

        if len(options_enabled) <= len(options_list):
            options_enabled += [True] * (len(options_list) - len(options_events))
            action.do(action.SetGameVar('_base_options_disabled', [not b for b in options_enabled]))
        else:
            self.logger.error("base: too many bools in option enabled list: ", other_options_enabled)
            return

        if len(options_events) <= len(options_list):
            options_events += [None] * (len(options_list) - len(options_events))
            action.do(action.SetGameVar('_base_options_events', options_events))
        else:
            self.logger.error("base: too many events in option event list: ", other_options_on_select)
            return
        action.do(action.SetGameVar('_base_additional_options', options_list))
    else:
        action.do(action.SetGameVar('_base_options_disabled', []))
        action.do(action.SetGameVar('_base_options_events', []))
        action.do(action.SetGameVar('_base_additional_options', []))

    if 'show_map' in flags:
        action.do(action.SetGameVar('_base_transparent', True))
    else:
        action.do(action.SetGameVar('_base_transparent', False))

    self.game.state.change('base_main')
    self.state = 'paused'

def set_custom_options(self: Event, custom_options: List[str], custom_options_enabled: List[bool] = None,
                       custom_options_desc: List[str] = None, custom_options_on_select: List[str] = None, flags=None):
    flags = flags or set()

    options_list = custom_options or []
    options_enabled = custom_options_enabled or []
    options_desc = [option + '_desc' for option in options_list]
    options_events = custom_options_on_select or []

    if len(options_enabled) <= len(options_list):
        options_enabled += [True] * (len(options_list) - len(options_events))
        action.do(action.SetGameVar('_custom_options_disabled', [not b for b in options_enabled]))
    else:
        self.logger.error("set_custom_options: too many bools in option enabled list: ", custom_options_enabled)
        return

    if len(custom_options_desc) <= len(options_events):
        for idx, desc in enumerate(custom_options_desc):
            options_desc[idx] = desc
        action.do(action.SetGameVar('_custom_info_desc', options_desc))
    else:
        self.logger.error("set_custom_options: too many descriptions in option description list: ", custom_options_desc)
        return

    if len(options_events) <= len(options_list):
        options_events += [None] * (len(options_list) - len(options_events))
        action.do(action.SetGameVar('_custom_options_events', options_events))
    else:
        self.logger.error("set_custom_options: too many events in option event list: ", custom_options_on_select)
        return

    action.do(action.SetGameVar('_custom_additional_options', options_list))

def shop(self: Event, unit, item_list: List[str], shop_flavor=None, stock_list: List[int]=None, shop_id=None, flags=None):
    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("shop: Must have a unit visit the shop!")
        return
    unit = new_unit
    if shop_id is None:
        shop_id = self.nid
    self.game.memory['shop_id'] = shop_id
    self.game.memory['current_unit'] = unit
    shop_items = item_funcs.create_items(unit, item_list)
    self.game.memory['shop_items'] = shop_items

    if shop_flavor:
        self.game.memory['shop_flavor'] = shop_flavor.lower()
    else:
        self.game.memory['shop_flavor'] = 'armory'

    if stock_list:
        # Remember which items have already been bought for this shop...
        for idx, item in enumerate(item_list):
            item_history = '__shop_%s_%s' % (shop_id, item)
            if item_history in self.game.game_vars:
                stock_list[idx] -= self.game.game_vars[item_history]
        self.game.memory['shop_stock'] = stock_list
    else:
        self.game.memory['shop_stock'] = None

    self.game.state.change('shop')
    self.state = 'paused'

def choice(self: Event, nid: NID, title: str, choices: str, row_width: int = 0, orientation: Orientation = Orientation.VERTICAL,
           alignment: Alignments = Alignments.CENTER, bg: str = 'menu_bg_base', event_nid: str = None, entry_type: str = 'str',
           dimensions: Optional[Tuple[str, str]] = None, text_align: HAlignment = HAlignment.LEFT, flags=None):
    flags = flags or set()

    nid = nid or ""
    header = title

    if 'no_bg' in flags:
        bg = None

    # figure out function or list of NIDs
    data = []
    if 'expression' in flags:
        try:
            ast.parse(choices)
            def tryexcept(callback_expr):
                try:
                    val = self._eval_expr(self.text_evaluator._evaluate_all(callback_expr), 'from_python' in flags)
                    if isinstance(val, list):
                        return val or ['']
                    else:
                        return [self._object_to_str(val)]
                except Exception as e:
                    self.logger.error("choice: Choice %s failed to evaluate expression %s with error %s", nid, callback_expr, str(e))
                    return [""]
            data = lambda: tryexcept(choices)
        except:
            self.logger.error('choice: %s is not a valid python expression' % choices)
    else: # list of NIDs
        choices = self.text_evaluator._evaluate_all(choices)
        data = choices.split(',')
        data = [s.strip().replace('{comma}', ',') for s in data]
    data = data or ['']

    size = None
    if dimensions:
        rows, columns = dimensions
        size = (columns, rows)

    should_persist = 'persist' in flags
    no_cursor = 'no_cursor' in flags
    arrows = None
    if 'arrows' in flags:
        arrows = True
    elif 'no_arrows' in flags:
        arrows = False
    scroll_bar = None
    if 'scroll_bar' in flags:
        scroll_bar = True
    elif 'no_scroll_bar' in flags:
        scroll_bar = False
    backable = 'backable' in flags

    event_context = {
        'unit': self.unit,
        'unit2': self.unit2,
        'position': self.position,
        'local_args': self.local_args
    }
    self.game.memory['player_choice'] = (nid, header, data, row_width,
                                         orientation, entry_type, should_persist,
                                         alignment, bg, event_nid, size, no_cursor,
                                         arrows, scroll_bar, text_align, backable, event_context)
    self.game.state.change('player_choice')
    self.state = 'paused'

def unchoice(self: Event, flags=None):
    try:
        prev_state = self.game.state.get_prev_state()
        if prev_state.name == 'player_choice':
            prev_state_nid = prev_state.nid
            unchoose_prev_state = self.game.memory[prev_state_nid + '_unchoice']
            if unchoose_prev_state:
                unchoose_prev_state()
    except Exception as e:
        self.logger.error("unchoice: Unchoice failed: " + str(e))

def textbox(self: Event, nid: str, text: str, box_position: Point | Alignments=None,
            width=None, num_lines=None, style_nid=None, text_speed=None,
            font_color=None, font_type=None, bg=None, flags=None):
    flags = flags or set()

    textbox_style = None
    if style_nid and style_nid in self.game.speak_styles:
        textbox_style = self.game.speak_styles[style_nid]
    default_textbox_style = self.game.speak_styles['__default_text']

    if box_position:
        position = box_position
    elif textbox_style and textbox_style.position:
        position = textbox_style.position
    else:
        position = default_textbox_style.position

    if width:
        box_width = width
    elif textbox_style and textbox_style.width:
        box_width = textbox_style.width
    else:
        box_width = default_textbox_style.width

    if text_speed:
        speed = text_speed
    elif textbox_style and textbox_style.speed:
        speed = textbox_style.speed
    else:
        speed = default_textbox_style.speed

    if font_color:
        fcolor = font_color
    elif textbox_style and textbox_style.font_color:
        fcolor = textbox_style.font_color
    else:
        fcolor = default_textbox_style.font_color

    if font_type:
        ftype = font_type
    elif textbox_style and textbox_style.font_type:
        ftype = textbox_style.font_type
    else:
        ftype = default_textbox_style.font_type

    if bg:
        box_bg = bg
    elif textbox_style and textbox_style.background:
        box_bg = textbox_style.background
    else:
        box_bg = default_textbox_style.background

    if textbox_style and textbox_style.transparency is not None:
        transparency = textbox_style.transparency
    else:
        transparency = 0.05

    if num_lines:
        lines = num_lines
    elif textbox_style and textbox_style.num_lines:
        lines = textbox_style.num_lines
    else:
        # let's not default to 0 please
        lines = default_textbox_style.num_lines or 1

    if textbox_style and textbox_style.flags:
        flags = textbox_style.flags.union(flags)
    if 'expression' in flags:
        expr = lambda: ""
        try:
            # eval once to make sure it's eval-able
            ast.parse(text)
            def tryexcept(callback_expr) -> str:
                try:
                    val = self._eval_expr(self.text_evaluator._evaluate_all(callback_expr), 'from_python' in flags)
                    return str(val)
                except:
                    self.logger.error("textbox: failed to eval %s", callback_expr)
                    return ""
            expr = lambda: tryexcept(text)
        except:
            self.logger.error('textbox: %s is not a valid python expression' % text)
        textbox = dialog.DynamicDialogWrapper(
            expr, background=box_bg, position=position, width=box_width,
            style_nid=style_nid, speed=speed,
            font_color=fcolor, font_type=ftype, num_lines=lines,
            draw_cursor=False, transparency=transparency)
    else:
        text = self.text_evaluator._evaluate_all(text)
        text = dialog.process_dialog_shorthand(text)
        # textboxes shouldn't use {w} or |
        text = text.replace('{w}', '').replace('|', '{br}')
        textbox = \
            dialog.Dialog(text, background=box_bg, position=position, width=box_width,
                          style_nid=style_nid, speed=speed,
                          font_color=fcolor, font_type=ftype, num_lines=lines,
                          draw_cursor=False, transparency=transparency)
    self.other_boxes.append((nid, textbox))

def table(self: Event, nid: NID, table_data: str, title: str = None,
          dimensions: Tuple[int, int] = (0, 1), row_width: int = -1, alignment: Alignments = Alignments.TOP_LEFT,
          bg: str = 'menu_bg_base', entry_type: str = 'str', text_align: HAlignment = HAlignment.LEFT, flags=None):
    flags = flags or set()
    if 'no_bg' in flags:
        bg = None

    box_nids = [nid for nid, _ in self.other_boxes]
    if nid in box_nids:
        self.logger.error("table: UI element with nid %s already exists" % nid)
        return

    rows, cols = dimensions

    # figure out function or list of NIDs
    data = []
    if 'expression' in flags:
        try:
            # eval once to make sure it's eval-able
            ast.parse(table_data)
            def tryexcept(callback_expr):
                try:
                    val = self._eval_expr(self.text_evaluator._evaluate_all(callback_expr), 'from_python' in flags)
                    if isinstance(val, list):
                        return val or ['']
                    else:
                        return [self._object_to_str(val)]
                except:
                    self.logger.error("table: failed to eval %s", callback_expr)
                    return [""]
            data = lambda: tryexcept(table_data)
        except:
            self.logger.error('table: %s is not a valid python expression' % table_data)
    else: # list of NIDs
        table_data = self.text_evaluator._evaluate_all(table_data)
        data = table_data.split(',')
        data = [s.strip().replace('{comma}', ',') for s in data]
    data = data or [""]
    table_ui = SimpleMenuUI(
        data, entry_type, title=title, rows=rows, cols=cols,
        row_width=row_width, alignment=alignment, bg=bg,
        text_align=text_align)
    self.other_boxes.append((nid, table_ui))

def remove_table(self: Event, nid, flags=None):
    self.other_boxes = [(bnid, box) for (bnid, box) in self.other_boxes if bnid != nid]

def text_entry(self: Event, nid, string, positive_integer: int=16, illegal_character_list: Optional[List[str]]=None, flags=None):
    flags = flags or set()

    header = string
    limit = positive_integer
    force_entry = 'force_entry' in flags

    self.game.memory['text_entry'] = (nid, header, limit, illegal_character_list or [], force_entry)
    self.game.state.change('text_entry')
    self.state = 'paused'

def chapter_title(self: Event, music: SongPrefab | SongObject | NID=None, string=None, flags=None):
    custom_string = string
    self.game.memory['chapter_title_music'] = self._resolve_nid(music)
    self.game.memory['chapter_title_title'] = custom_string
    # End the skip here
    self.do_skip = False
    self.super_skip = False
    self.game.state.change('chapter_title')
    self.state = 'paused'

def draw_overlay_sprite(self: Event, nid, sprite_id, position=None, z_level=0, animation=None, speed=1000, flags=None):
    flags = flags or set()

    name = nid
    sprite_nid = sprite_id
    pos = (0, 0)
    if position:
        pos = tuple(position)
    anim_dir = animation

    anim_dur = speed

    sprite = SPRITES.get(sprite_nid)
    component = UIComponent.from_existing_surf(sprite)
    component.name = name
    component.disable()
    x, y = pos
    if anim_dir:
        if anim_dir == 'fade':
            enter_anim = fade_anim(0, 1, anim_dur)
            component.margin = (x, 0, y, 0)
        else:
            start_x, start_y = 0, 0
            if anim_dir == 'west':
                start_x = -component.width
                start_y = y
            elif anim_dir == 'east':
                start_x = WINWIDTH
                start_y = y
            elif anim_dir == 'north':
                start_x = x
                start_y = -component.height
            elif anim_dir == 'south':
                start_x = x
                start_y = WINHEIGHT
            enter_anim = translate_anim((start_x, start_y), (x, y), anim_dur, interp_mode=InterpolationType.CUBIC)
        component.save_animation(enter_anim, '!enter')
    else:
        component.margin = (x, 0, y, 0)
    if 'foreground' in flags:
        self.foreground_overlay_ui.add_child(component)
    else:
        self.overlay_ui.add_child(component)
    if self.do_skip:
        component.enable()
        return
    else:
        component.enter()

    if anim_dir and 'no_block' not in flags:
        self.wait_time = engine.get_time() + anim_dur
        self.state = 'waiting'

def remove_overlay_sprite(self: Event, nid, animation=None, speed=1000, flags=None):
    flags = flags or set()
    if 'foreground' in flags:
        component = self.foreground_overlay_ui.get_child(nid)
    else:
        component = self.overlay_ui.get_child(nid)
    if not component:
        return

    anim_dur = speed

    if animation:
        if animation == 'fade':
            exit_anim = fade_anim(1, 0, anim_dur)
        else:
            curr_x, curr_y = component.offset
            end_x, end_y = 0, 0
            if animation == 'west':
                end_x = -component.width
                end_y = curr_y
            elif animation == 'east':
                end_x = WINWIDTH
                end_y = curr_y
            elif animation == 'north':
                end_x = curr_x
                end_y = -component.height
            elif animation == 'south':
                end_x = curr_x
                end_y = WINHEIGHT
            exit_anim = translate_anim((curr_x, curr_y), (end_x, end_y), anim_dur, disable_after=True, interp_mode=InterpolationType.CUBIC)
        component.save_animation(exit_anim, '!exit')

    if 'foreground' in flags:
        overlay_ui = self.foreground_overlay_ui
    else:
        overlay_ui = self.overlay_ui

    if self.do_skip:
        overlay_ui._should_redraw = True
        component.disable()
    else:
        overlay_ui._should_redraw = True
        component.exit()
        if component.is_animating() and 'no_block' not in flags:
            self.wait_time = engine.get_time() + anim_dur
            self.state = 'waiting'

def alert(self: Event, string, item=None, skill=None, icon=None, flags=None):
    if item and item in DB.items.keys():
        custom_item = DB.items.get(item)
        self.game.alerts.append(banner.CustomIcon(string, custom_item))
    elif skill and skill in DB.skills.keys():
        custom_skill = DB.skills.get(skill)
        self.game.alerts.append(banner.CustomIcon(string, custom_skill))
    elif icon and any([sheet.get_index(icon) for sheet in RESOURCES.icons16]):
        self.game.alerts.append(banner.CustomIcon(string, icon))
    else:
        self.game.alerts.append(banner.Custom(string))
    self.game.state.change('alert')
    self.state = 'paused'

def victory_screen(self: Event, sound=None, flags=None):
    self.game.memory['victory_screen_sound'] = sound
    self.game.state.change('victory')
    self.state = 'paused'

def records_screen(self: Event, flags=None):
    self.game.state.change('base_records')
    self.state = 'paused'

def open_library(self: Event, flags=None):
    flags = flags or set()
    unlocked_lore = [lore for lore in DB.lore if lore.nid in self.game.unlocked_lore and lore.category != 'Guide']
    if unlocked_lore:
        self.state = "paused"
        if 'immediate' in flags:
            self.game.state.change('base_library')
        else:
            self.game.memory['next_state'] = 'base_library'
            self.game.state.change('transition_to')
    else:
        self.logger.warning("open_library: Skipping opening library because there is no unlocked lore")

def open_guide(self: Event, flags=None):
    flags = flags or set()
    unlocked_lore = [lore for lore in DB.lore if lore.nid in self.game.unlocked_lore and lore.category == 'Guide']
    if unlocked_lore:
        self.state = "paused"
        if 'immediate' in flags:
            self.game.state.change('base_guide')
        else:
            self.game.memory['next_state'] = 'base_guide'
            self.game.state.change('transition_to')
    else:
        self.logger.warning("open_guide: Skipping opening guide because there is no unlocked lore in the guide category")

def open_unit_management(self: Event, panorama=None, flags=None):
    flags = flags or set()
    if 'scroll' in flags:
        bg = background.create_background(panorama, True)
    else:
        bg = background.create_background(panorama, False)
    self.game.memory['base_bg'] = bg

    self.state = "paused"
    if 'immediate' in flags:
        self.game.state.change('base_manage')
    else:
        self.game.memory['next_state'] = 'base_manage'
        self.game.state.change('transition_to')

def open_trade(self: Event, unit1, unit2, flags=None):
    flags = flags or set()

    unit1_obj = self._get_unit(unit1)
    if not unit1_obj:
        self.logger.error("open_trade: Could not find unit %s" % unit1)
        return

    unit2_obj = self._get_unit(unit2)
    if not unit2_obj:
        self.logger.error("open_trade: Could not find unit %s" % unit2)
        return

    # Make sure the trade state knows who's trading
    self.game.cursor.cur_unit = unit1_obj
    self.game.memory['trade_partner'] = unit2_obj

    self.state = "paused"
    self.game.memory['next_state'] = 'trade'
    self.game.state.change('transition_to')

def show_minimap(self: Event, flags=None):
    cursor_was_hidden = False
    if self.game.cursor.is_hidden():
        cursor_was_hidden = True
        self.game.cursor.show()

    self.state = "paused"
    self.game.state.change('minimap')

    if cursor_was_hidden:
        hide_cursor_command = event_commands.DispCursor({'ShowCursor': False})
        self.command_queue.append(hide_cursor_command)

def open_achievements(self: Event, background: str, flags=None):
    # Set up the base background
    self.game.memory['base_bg'] = None
    action.do(action.SetGameVar('_base_bg_name', background))

    self.state = "paused"
    self.game.memory['next_state'] = 'base_achievement'
    self.game.state.change('transition_to')

def location_card(self: Event, string, flags=None):
    new_location_card = dialog.LocationCard(string)
    self.other_boxes.append((None, new_location_card))

    self.wait_time = engine.get_time() + new_location_card.exist_time
    self.state = 'waiting'

def credits(self: Event, role, credits, flags=None):
    flags = flags or set()

    title = role or ''
    credits = credits.split(',') if 'no_split' not in flags else [credits]
    wait = 'wait' in flags
    center = 'center' in flags

    new_credits = dialog.Credits(title, credits, wait, center)
    self.other_boxes.append((None, new_credits))

    self.wait_time = engine.get_time() + new_credits.wait_time()
    self.state = 'waiting'

def ending(self: Event, portrait, title, text, flags=None):
    unit = self._get_unit(portrait)
    if unit and unit.portrait_nid:
        portrait, _ = icons.get_portrait(unit)
        portrait = portrait.convert_alpha()
        portrait = image_mods.make_translucent(portrait, 0.2)
    else:
        self.logger.error("ending: Couldn't find unit or portrait %s" % portrait)
        return False

    new_ending = dialog.Ending(portrait, title, text, unit)
    self.text_boxes.append(new_ending)
    self.state = 'dialog'

def paired_ending(self: Event, left_portrait, right_portrait, left_title, right_title, text, flags=None):
    left_unit = self._get_unit(left_portrait)
    if left_unit and left_unit.portrait_nid:
        left_portrait, _ = icons.get_portrait(left_unit)
        left_portrait = engine.flip_horiz(left_portrait)
        left_portrait = left_portrait.convert_alpha()
        left_portrait = image_mods.make_translucent(left_portrait, 0.5)
    else:
        self.logger.error("ending: Couldn't find unit or portrait %s" % left_portrait)
        return False

    right_unit = self._get_unit(right_portrait)
    if right_unit and right_unit.portrait_nid:
        right_portrait, _ = icons.get_portrait(right_unit)
        right_portrait = right_portrait.convert_alpha()
        right_portrait = image_mods.make_translucent(right_portrait, 0.5)
    else:
        self.logger.error("ending: Couldn't find unit or portrait %s" % right_portrait)
        return False

    new_ending = dialog.PairedEnding(left_portrait, right_portrait, left_title, right_title, text, left_unit, right_unit)
    self.text_boxes.append(new_ending)
    self.state = 'dialog'

def pop_dialog(self: Event, flags=None):
    if self.text_boxes:
        self.text_boxes.pop()

def unlock(self: Event, unit, flags=None):
    # This is a macro that just adds new commands to command list
    find_unlock_command = event_commands.FindUnlock({'Unit': unit})
    spend_unlock_command = event_commands.SpendUnlock({'Unit': unit})
    self.command_queue.append(find_unlock_command)
    self.command_queue.append(spend_unlock_command)

def find_unlock(self: Event, unit, flags=None):
    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("find_unlock: Couldn't find unit with nid %s" % unit)
        return
    unit = new_unit
    region = self.local_args.get('region')
    if not region:
        self.logger.error("find_unlock: Can only find_unlock within a region's event script")
        return
    if skill_system.can_unlock(unit, region):
        self.game.memory['unlock_item'] = None
        return  # We're done here

    all_items = []
    for item in item_funcs.get_all_items(unit):
        if item_funcs.available(unit, item) and \
                item_system.can_unlock(unit, item, region):
            all_items.append(item)

    if len(all_items) > 1:
        self.game.memory['current_unit'] = unit
        self.game.memory['all_unlock_items'] = all_items
        self.game.state.change('unlock_select')
        self.state = 'paused'
    elif len(all_items) == 1:
        self.game.memory['unlock_item'] = all_items[0]
    else:
        self.logger.debug("find_unlock: Somehow unlocked event without being able to")
        self.game.memory['unlock_item'] = None

def spend_unlock(self: Event, unit, flags=None):
    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("spend_unlock: Couldn't find unit with nid %s" % unit)
        return
    unit = new_unit

    chosen_item = self.game.memory.get('unlock_item')
    self.game.memory['unlock_item'] = None
    if not chosen_item:
        return

    actions, playback = [], []
    # In order to proc uses, c_uses etc.
    item_system.start_combat(playback, unit, chosen_item, unit, chosen_item, None)
    item_system.on_hit(actions, playback, unit, chosen_item, unit, chosen_item, self.position, None, (0, 0), True)
    for act in actions:
        action.do(act)
    item_system.end_combat(playback, unit, chosen_item, unit, chosen_item, None)

    if unit.get_hp() <= 0:
        # Force can't die unlocking stuff, because I don't want to deal with that nonsense
        action.do(action.SetHP(unit, 1))

    # Check to see if we broke the item we were using
    if item_system.is_broken(unit, chosen_item):
        should_alert = item_system.alerts_when_broken(unit, chosen_item)
        item_system.on_broken(unit, chosen_item)
        if should_alert and unit.team == 'player':
            self.game.alerts.append(banner.BrokenItem(unit, chosen_item))
            self.game.state.change('alert')
            self.state = 'paused'

def trigger_script(self: Event, event, unit1=None, unit2=None, flags=None):
    if unit1:
        unit = self._get_unit(unit1)
    else:
        unit = self.unit
    if unit2:
        unit2 = self._get_unit(unit2)
    else:
        unit2 = self.unit2

    level_nid = self.game.level.nid if self.game.level else None
    valid_events = DB.events.get_by_nid_or_name(event, level_nid)
    for event_prefab in valid_events:
        self.game.events.trigger_specific_event(event_prefab.nid, unit, unit2, self.position, self.local_args)
        self.state = 'paused'

    if not valid_events:
        self.logger.error("trigger_script: Couldn't find any valid events matching name %s" % event)

def trigger_script_with_args(self: Event, event: str, arg_list: Dict[str, str] = None, flags=None):
    trigger_script = event
    valid_events = DB.events.get_by_nid_or_name(trigger_script, self.game.level.nid)

    # Process Arg List into local args directory
    local_args = arg_list

    for event_prefab in valid_events:
        self.game.events.trigger_specific_event(event_prefab.nid, local_args=local_args)
        self.state = 'paused'
    if not valid_events:
        self.logger.error("trigger_script_with_args: Couldn't find any valid events matching name %s" % trigger_script)
        return

def loop_units(self: Event, expression, event, flags=None):
    unit_list_str = expression
    try:
        unit_list = self._eval_expr(unit_list_str, 'from_python' in flags)
    except Exception as e:
        self.logger.error("loop_units: %s: Could not evalute {%s}" % (e, unit_list_str))
        return
    if not unit_list:
        self.logger.warning("loop_units: No units returned for list: %s" % (unit_list_str))
        return
    if not all((isinstance(unit_nid, str) or isinstance(unit_nid, UnitObject)) for unit_nid in unit_list):
        self.logger.error("loop_units: %s: could not evaluate to NID list {%s}" % ('loop_units', unit_list_str))
        return
    for unit_nid in unit_list:
        if not isinstance(unit_nid, str):
            unit_nid = unit_nid.nid  # Try this!
        macro_command = event_commands.TriggerScript({'Event': event, 'Unit1': unit_nid})
        self.command_queue.append(macro_command)

def change_roaming(self: Event, free_roam_enabled: bool, flags=None):
    self.game.action_log.set_first_free_action()
    self.game.set_roam(free_roam_enabled)

def change_roaming_unit(self: Event, unit, flags=None):
    unit = self._get_unit(unit)
    if unit:
        self.game.set_roam_unit(unit)
    else:
        self.game.clear_roam_unit()

def clean_up_roaming(self: Event, flags=None):
    # Not turnwheel compatible
    for unit in self.game.units:
        if unit.position and unit != self.game.get_roam_unit():
            action.do(action.FadeOut(unit))
    if DB.constants.value('initiative'):
        self.game.initiative.clear()
        if self.game.get_roam_unit():
            self.game.initiative.insert_unit(self.game.get_roam_unit().nid)

def add_to_initiative(self: Event, unit, integer: int, flags=None):
    # NOT CURRENTLY TURNWHEEL COMPATIBLE
    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("add_to_initiative: Couldn't find unit with nid %s" % unit)
        return
    unit = new_unit
    pos = integer
    if DB.constants.value('initiative'):
        self.game.initiative.remove_unit(unit)
        self.game.initiative.insert_at(unit, self.game.initiative.current_idx + pos)

def move_in_initiative(self: Event, unit, integer: int, flags=None):
    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("move_in_initiative: Couldn't find unit with nid %s" % unit)
        return
    unit = new_unit
    offset = integer
    action.do(action.MoveInInitiative(unit, offset))

def pair_up(self: Event, unit1, unit2, flags=None):
    new_unit1 = self._get_unit(unit1)
    if not new_unit1:
        self.logger.error("pair_up: Couldn't find unit with nid %s" % unit1)
        return
    follower = new_unit1
    if self.game.get_rescuers_position(follower) or follower.traveler:
        self.logger.error("pair_up: Rescuee is already traveling with somebody else")
        return
    new_unit2 = self._get_unit(unit2)
    if not new_unit2:
        self.logger.error("pair_up: Couldn't find unit with nid %s" % unit2)
        return
    leader = new_unit2
    if self.game.get_rescuers_position(leader) or leader.traveler:
        self.logger.error("pair_up: Rescuer is already traveling with somebody else")
        return
    if unit_funcs.can_pairup(leader, follower):
        action.do(action.PairUp(follower, leader))
    else:
        action.do(action.Rescue(leader, follower))

def separate(self: Event, unit, flags=None):
    new_unit = self._get_unit(unit)
    if not new_unit:
        self.logger.error("separate: Couldn't find unit with nid %s" % unit)
        return
    unit = new_unit
    if not unit.traveler:
        self.logger.error("separate: Unit is not traveling with anybody")
        return
    if DB.constants.value('pairup'):
        action.do(action.Separate(unit, unit.traveler, None))
    else:
        action.do(action.RemovePartner(unit))

def create_achievement(self: Event, nid: str, name: str, description: str, flags=None):
    flags = flags or set()

    completed = 'completed' in flags
    hidden = 'hidden' in flags
    ACHIEVEMENTS.add_achievement(nid, name, description, completed, hidden)

def update_achievement(self: Event, achievement: str, name: str, description:str, flags=None):
    flags = flags or set()

    hidden = 'hidden' in flags
    ACHIEVEMENTS.update_achievement(achievement, name, description, hidden)

def complete_achievement(self: Event, achievement: str, completed: bool, flags=None):
    flags = flags or set()
    nid = achievement

    banner = 'banner' in flags

    complete = ACHIEVEMENTS.complete_achievement(nid, completed, banner)

    def draw_achievement(self: Event, achievement):
        # Nested for unit test/event command reasons
        name = 'achievement_notification_' + achievement.nid
        font = "text"
        width = rendered_text_width([font], [achievement.name])
        sprite = base_surf.create_base_surf(width + 16, 24, 'menu_bg_achievement_dark')
        position = (WINWIDTH - width - 20, WINHEIGHT - 28)
        x, y = position

        component = UIComponent.from_existing_surf(sprite)
        component.name = name
        component.disable()

        start_x = x
        start_y = WINHEIGHT
        enter_anim = translate_anim((start_x, start_y), (x, y), 750, interp_mode=InterpolationType.CUBIC)
        exit_anim = translate_anim((x, y), (start_x, start_y), 750, disable_after=True, interp_mode=InterpolationType.CUBIC)
        component.save_animation(enter_anim, '!enter')
        component.save_animation(exit_anim, '!exit')

        achievement_name = PlainTextLine("name", component, '<white>' + achievement.name + '</white>', font)
        achievement_name.props.h_alignment = HAlignment.CENTER
        achievement_name.props.v_alignment = VAlignment.CENTER
        component.add_child(achievement_name)

        get_sound_thread().play_sfx("Item", volume=0.5)

        self.foreground_overlay_ui.add_child(component)
        if self.do_skip:
            component.enable()
            return
        else:
            component.enter()
    # End Func

    if banner and complete:
        draw_achievement(self, ACHIEVEMENTS.get(nid))

        # Remember to remove the command eventually
        anim_nid = 'achievement_notification_' + nid
        if self.do_skip:
            remove_overlay_sprite(self, anim_nid, flags={'foreground'})
        else:
            self.wait_time = engine.get_time() + 2000
            self.state = 'waiting'
            remove_overlay_sprite_command = event_commands.RemoveOverlaySprite({'Nid': anim_nid}, flags={'foreground'})
            self.command_queue.append(remove_overlay_sprite_command)

def clear_achievements(self: Event, flags=None):
    ACHIEVEMENTS.clear_achievements()

def create_record(self: Event, nid: str, expression: str | Any, flags=None):
    try:
        val = self._eval_expr(expression, 'from_python' in flags)
        RECORDS.create(nid, val)
    except Exception as e:
        self.logger.error("create_record: Could not evaluate %s (%s)" % (expression, e))

def update_record(self: Event, nid: str, expression: str | Any, flags=None):
    try:
        val = self._eval_expr(expression, 'from_python' in flags)
        RECORDS.update(nid, val)
    except Exception as e:
        self.logger.error("update_record: Could not evaluate %s (%s)" % (expression, e))

def replace_record(self: Event, nid: str, expression: str | Any, flags=None):
    try:
        val = self._eval_expr(expression, 'from_python' in flags)
        RECORDS.replace(nid, val)
    except Exception as e:
        self.logger.error("replace_record: Could not evaluate %s (%s)" % (expression, e))

def delete_record(self: Event, nid: str, flags=None):
    RECORDS.delete(nid)

def unlock_difficulty(self: Event, difficulty_mode: str, flags=None):
    RECORDS.unlock_difficulty(difficulty_mode)

def hide_combat_ui(self: Event, flags=None):
    self.game.game_vars["_hide_ui"] = True

def show_combat_ui(self: Event, flags=None):
    self.game.game_vars["_hide_ui"] = False
