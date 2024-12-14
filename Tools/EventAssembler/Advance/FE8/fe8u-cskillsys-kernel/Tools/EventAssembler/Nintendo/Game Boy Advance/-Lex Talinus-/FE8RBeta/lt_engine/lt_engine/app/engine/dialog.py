from enum import Enum
import re
from typing import Callable, List

from app.constants import WINHEIGHT, WINWIDTH
from app.engine import config as cf
from app.events import event_portrait, screen_positions
from app.engine import engine, image_mods, text_funcs
from app.engine.base_surf import create_base_surf
from app.engine.fonts import FONT
from app.engine.game_state import game
from app.engine.graphics.ingame_ui.ui_funcs import calc_align
from app.engine.graphics.text.text_renderer import (MATCH_CAPTURE_TAG_RE,
                                                    fix_tags, render_text,
                                                    text_width)
from app.engine.sound import get_sound_thread
from app.engine.sprites import SPRITES
from app.events.speak_style import SpeakStyle
from app.utilities import utils
from app.utilities.enums import Alignments, HAlignment

import logging

from app.utilities.str_utils import SHIFT_NEWLINE

MATCH_DIALOG_COMMAND_RE = re.compile('(\{[^\{]*?\})')
def process_dialog_shorthand(text: str) -> str:
    if not text:
        return text
    # special char: this is a unicode single-line break.
    # basically equivalent to {br}
    # the first char shouldn't be one of these
    if text[0] == SHIFT_NEWLINE:
        text = text[1:]
    text = text.replace(SHIFT_NEWLINE, '{sub_break}')  # sub break to distinguish it
    text = text.replace('\n', '{br}')
    text = text.replace('|', '{w}{br}')
    text = text.replace('{semicolon}', ';')
    return text

class DialogState(Enum):
    PROCESS = 'process'  # Normal display of characters one at a time
    TRANSITION_IN = 'transition_in'  # Dialog is fading in
    PAUSE = 'pause'  # Pause processing for some amount of time
    PAUSE_BEFORE_WAIT = 'pause_before_wait'  # Pause processing before we wait for user input, so that user input cannot skip too quickly
    WAIT = 'wait'  # Wait for user input
    DONE = 'done'  # Dialog has nothing else to do and can be removed
    NEW_LINE = 'new_line'  # Pause while we move to a new line
    COMMAND_PAUSE = 'command_pause'  # Pause caused by a {p} command

class Dialog():
    solo_flag = False
    cursor = SPRITES.get('waiting_cursor')
    cursor_offset = [0]*20 + [1]*2 + [2]*8 + [1]*2
    transition_speed = utils.frames2ms(10)
    pause_before_wait_time = utils.frames2ms(9)
    pause_time = utils.frames2ms(3)
    attempt_split: bool = True # Whether we attempt to split big chunks across multiple lines

    def __init__(self, text, portrait=None, background=None, position=None, width=None,
                 speaker=None, style_nid=None, autosize=False, speed: float = 1.0, font_color='black',
                 font_type='convo', num_lines=2, draw_cursor=True, message_tail='message_bg_tail',
                 transparency=0.05, name_tag_bg='name_tag', flags=None):
        flags = flags or set()
        self.plain_text = text
        self.portrait = portrait
        self.speaker = speaker
        self.style_nid = style_nid
        self.font_type = font_type
        self.font_color = font_color or 'black'
        self.autosize = autosize
        self.speed = speed if speed is not None else 1.0
        self.starting_speed = self.speed
        self.num_lines = num_lines
        self.draw_cursor_flag = draw_cursor
        self.font = FONT[self.font_type]
        if '{sub_break}' in self.plain_text:
            self.attempt_split = False

        self._state = DialogState.TRANSITION_IN

        self.no_wait = False
        self.text_commands = self.format_text(text)
        self.text_lines = []

        # Size
        if width:
            self.width = width
            if self.width % 8:
                self.width += 8 - self.width%8
            self.text_width = max(8, self.width - 16)
            self.determine_height()
        elif self.portrait or self.autosize:
            self.determine_size()
        else:
            self.text_width, self.text_height = (WINWIDTH - 24, self.num_lines * 16)
            self.width, self.height = self.text_width + 16, self.text_height + 16

        # Position
        if position:
            if isinstance(position, Alignments):
                pos_x, pos_y = calc_align((self.width, self.height), position)
            else:
                pos_x = position[0]
                pos_y = position[1]
        elif self.portrait:
            desired_center = self.determine_desired_center(self.portrait)
            pos_x = utils.clamp(desired_center - self.width//2, 8, WINWIDTH - 8 - self.width)
            if pos_x % 8 != 0:
                pos_x += 4
            if pos_x == 0:
                pos_x = 4
            pos_y = WINHEIGHT - self.height - event_portrait.EventPortrait.main_portrait_coords[3] - 4
        else:
            pos_x = 4
            pos_y = WINHEIGHT - self.height - 4
        self.position = pos_x, pos_y

        self.background = None
        self.tail = None
        self.dialog_transparency = transparency

        if background and background not in ('None', 'clear'):
            self.background = self.make_background(background)
        else:
            self.background = engine.create_surface((self.width, self.height), True)
        if message_tail and message_tail != 'None':
            self.tail = SPRITES.get(message_tail)

        self.name_tag_surf = create_base_surf(64, 16, name_tag_bg)

        # For drawing
        self.cursor_offset_index = 0
        self.text_index = 0
        self.total_num_updates = 0
        self.y_offset = 0 # How much to move lines (for when a new line is spawned)

        self.should_move_mouth = 'no_talk' not in flags
        self.should_speak_sound = 'no_sound' not in flags

        # For state transitions
        self.transition_progress = 0
        self.last_update = engine.get_time()

        self.hold = 'hold' in flags

        # For sound
        self.last_sound_update = 0

        if 'no_popup' in flags:
            self.last_update = engine.get_time() - 10000

    @classmethod
    def from_style(cls, style: SpeakStyle, text, portrait=None, width=None):
        style_as_dict = style.as_dict()
        if width:
            style_as_dict['width'] = width
        self = cls(text, portrait=portrait, autosize=False, **style_as_dict)
        return self

    @property
    def state(self) -> DialogState:
        return self._state

    @state.setter
    def state(self, value: DialogState):
        self._state = value

    def format_text(self, text):
        text = process_dialog_shorthand(text)
        if text.endswith('{no_wait}'):
            text = text[:-len('{no_wait}')]
            self.no_wait = True
        elif not text.endswith('{w}'):
            text += '{w}'
        processed_text: List[str] = []
        # obligatory regex explanation: turns "A line.{w} With some <red>text</>."
        # into ["A line.", "{w}", " With some ", "<red>", "text", "</>", "."]
        # and then decomposes the non-command/tag elements into individual chars.
        text_split_by_commands: List[str] = re.split(MATCH_DIALOG_COMMAND_RE, text)
        text_split_by_commands_and_tags: List[str] = []
        for block in text_split_by_commands:
            text_split_by_commands_and_tags += re.split(MATCH_CAPTURE_TAG_RE, block)
        for block in text_split_by_commands_and_tags:
            if block.startswith('<') and block.endswith('>'): # tag (e.g. "<red>")
                processed_text.append(block)
            elif block.startswith('{') and block.endswith('}'): # command (e.g. "{br}")
                processed_text.append(block)
            else: # normal char str (e.g. "hello")
                processed_text += list(block)
        return processed_text

    def determine_desired_center(self, portrait):
        x = self.portrait.position[0] + self.portrait.get_width()//2
        return screen_positions.get_desired_center(x)

    def determine_width(self):
        width = 0
        current_line = ''
        preceded_by_wait: bool = False
        waiting_cursor = False
        for command in self.text_commands:
            if command in ('{br}', '{break}', '{clear}', '{sub_break}'):
                if not preceded_by_wait or not self.attempt_split:
                    # Force it to be only one line
                    split_lines = self.get_lines_from_block(current_line, 1)
                else:
                    split_lines = self.get_lines_from_block(current_line)
                width = max(width, text_funcs.get_max_width(self.font_type, split_lines))
                if len(split_lines) == 1:
                    waiting_cursor = True
                current_line = ''
                preceded_by_wait = False
            elif command in ('{w}', '{wait}'):
                preceded_by_wait = True
            elif command.startswith('{'):
                pass
            else:
                current_line += command
        if current_line:
            if self.attempt_split:
                split_lines = self.get_lines_from_block(current_line)
            else:
                split_lines = self.get_lines_from_block(current_line, 1)
            width = max(width, text_funcs.get_max_width(self.font_type, split_lines))
            if len(split_lines) == 1:
                waiting_cursor = True
        if waiting_cursor:
            if len(split_lines) == 1:
                width += 16
        return width

    def determine_height(self):
        self.text_height = self.font.height * self.num_lines
        self.text_height = max(self.text_height, 16)
        self.height = self.text_height + 16

    def determine_size(self):
        self.text_width = self.determine_width()
        self.text_width = utils.clamp(self.text_width, 48, WINWIDTH - 32)
        self.width = self.text_width + 24 - self.text_width%8
        if self.width <= WINWIDTH - 16:
            self.width += 8
        self.determine_height()

    def make_background(self, background):
        surf = create_base_surf(self.width, self.height, background)
        return surf

    def get_lines_from_block(self, block, force_lines=None):
        if force_lines:
            num_lines = force_lines
        else:
            num_lines = self.num_lines
            if len(block) <= 24:
                num_lines = 1
        lines = text_funcs.split(self.font_type, block, num_lines, WINWIDTH - 16)
        return lines

    def _next_line(self):
        # Don't do this for the first line
        if len(self.text_lines) > self.num_lines - 1:
            self.state = DialogState.NEW_LINE
            self.y_offset = 16
        else:
            self.state = DialogState.PROCESS
            if self.portrait:
                if self.should_move_mouth:
                    self.portrait.talk()
                else:
                    self.portrait.stop_talking()
        self.text_lines.append("")

    def _add_letter(self, letter):
        self.text_lines[-1] += letter

    def _next_char(self, sound=True):  # Add the next character to the text_lines list
        if self.text_index >= len(self.text_commands):
            self.pause_before_wait()
            return
        command = self.text_commands[self.text_index]
        if command in ('{br}', '{break}', '{sub_break}'):
            self._next_line()
        elif command == '{w}' or command == '{wait}':
            self.pause_before_wait()
        elif command == '{clear}':
            self.text_lines.clear()
            self._next_line()
        elif command == '{p}':
            self.command_pause()
        elif command == '{tgm}':
            self.should_move_mouth = not self.should_move_mouth
            if self.portrait:
                if self.should_move_mouth:
                    self.portrait.talk()
                else:
                    self.portrait.stop_talking()
        elif command == '{tgs}':
            self.should_speak_sound = not self.should_speak_sound
        elif command == '{max_speed}':
            self.speed = 0
        elif command == '{starting_speed}':
            self.speed = self.starting_speed
        elif command == ' ':  # Check to see if we should move to next line
            # Wow this is cursed
            # Basically, we need to make sure we are using
            # a fixed tags version of what the dialog would look like,
            # if the next word was added to the current line (self.text_lines[-1])
            next_word = self._get_next_word(self.text_index)
            test_lines = self.text_lines[:-1] + [self.text_lines[-1] + ' ' + next_word]
            test_line = fix_tags(test_lines)[-1]
            # Remove any commands from line
            test_line = re.sub(r'\{[^}]*\}', '', test_line)
            next_width = text_width(self.font_type, test_line)
            if next_width > self.text_width:
                self._next_line()
            else:
                self._add_letter(' ')
                if sound:
                    self.play_talk_boop()
        elif command in ('.', ',', ';', '!', '?'):
            self._add_letter(command)
            self.pause()
        else:
            self._add_letter(command)
            if sound:
                self.play_talk_boop()
        self.text_index += 1

    def _get_next_word(self, text_index):
        word = ''
        for letter in self.text_commands[self.text_index + 1:]:
            if letter == ' ':
                break
            elif len(letter) > 1:  # Command
                if letter.startswith('{'):
                    break
                elif letter.startswith('<'):
                    # continue
                    word += letter
            else:
                word += letter
        return word

    def is_complete(self) -> bool:
        """
        Should no longer be drawn
        """
        return self.state == DialogState.DONE and not self.hold

    def is_done(self) -> bool:
        """
        Can move onto processing other commands
        """
        return self.state == DialogState.DONE

    def is_done_or_wait(self) -> bool:
        return self.state in (DialogState.DONE, DialogState.WAIT)

    def is_paused(self) -> bool:
        """
        Waiting for the event to finish processing it's {p} command
        """
        return self.state == DialogState.COMMAND_PAUSE

    def pause(self):
        if self.portrait:
            self.portrait.stop_talking()
        self.state = DialogState.PAUSE
        self.last_update = engine.get_time()

    def pause_before_wait(self):
        if self.portrait:
            self.portrait.stop_talking()
        self.state = DialogState.PAUSE_BEFORE_WAIT
        self.last_update = engine.get_time()

    def command_pause(self):
        if self.portrait:
            self.portrait.stop_talking()
        self.state = DialogState.COMMAND_PAUSE

    def command_unpause(self):
        if self.state == DialogState.COMMAND_PAUSE:
            self.state = DialogState.PROCESS
            if self.portrait and self.should_move_mouth:
                self.portrait.talk()

    def start_processing(self):
        if self.state == DialogState.TRANSITION_IN:
            self.state = DialogState.PROCESS
            self._next_line()

    def hurry_up(self):
        if self.state == DialogState.PROCESS:
            while self.state == DialogState.PROCESS:
                self._next_char(sound=False)
                # Skip pauses because we want maximum velocity of speech
                if self.state == DialogState.PAUSE:
                    self.state = DialogState.PROCESS
        elif self.state == DialogState.WAIT:
            if self.text_index >= len(self.text_commands):
                self.state = DialogState.DONE
            else:
                self.state = DialogState.PROCESS
                if self.portrait:
                    if self.should_move_mouth:
                        self.portrait.talk()
                    else:
                        self.portrait.stop_talking()

    def play_talk_boop(self):
        if cf.SETTINGS['talk_boop'] and engine.get_true_time() - self.last_sound_update > 32 and self.should_speak_sound:
            self.last_sound_update = engine.get_true_time()
            get_sound_thread().play_sfx('Talk_Boop')

    def update(self):
        current_time = engine.get_time()

        if self.state == DialogState.TRANSITION_IN:
            perc = (current_time - self.last_update) / self.transition_speed
            self.transition_progress = utils.clamp(perc, 0, 1)
            if self.transition_progress == 1:
                self._next_line()

        elif self.state == DialogState.PROCESS:
            if (cf.SETTINGS['text_speed'] * self.speed) > 0:
                num_updates = engine.get_delta() / (float(cf.SETTINGS['text_speed']) * self.speed)
                self.total_num_updates += num_updates
                while self.total_num_updates >= 1 and self.state == DialogState.PROCESS:
                    self.total_num_updates -= 1
                    self._next_char(sound=self.total_num_updates < 2)
                    if self.state != DialogState.PROCESS:
                        self.total_num_updates = 0
            else:
                while self.state == DialogState.PROCESS and (cf.SETTINGS['text_speed'] * self.speed) == 0:
                    self._next_char(sound=False)
                    # Skip regular pauses because we want MAXIMUM VELOCITY of characters
                    if self.state == DialogState.PAUSE:
                        self.state = DialogState.PROCESS
                self.play_talk_boop()

        elif self.state == DialogState.PAUSE_BEFORE_WAIT:
            if current_time - self.last_update > self.pause_before_wait_time:
                if self.no_wait:
                    self.state = DialogState.DONE
                else:
                    self.state = DialogState.WAIT

        elif self.state == DialogState.PAUSE:  # Regular pause for periods
            if current_time - self.last_update > self.pause_time:
                self.state = DialogState.PROCESS
                if self.portrait:
                    if self.should_move_mouth:
                        self.portrait.talk()
                    else:
                        self.portrait.stop_talking()

        elif self.state == DialogState.NEW_LINE:
            # Update y_offset
            self.y_offset = max(0, self.y_offset - 2)
            if self.y_offset == 0:
                self.state = DialogState.PROCESS
                if self.portrait:
                    if self.should_move_mouth:
                        self.portrait.talk()
                    else:
                        self.portrait.stop_talking()

        self.cursor_offset_index = (self.cursor_offset_index + 1) % len(self.cursor_offset)
        return True

    def warp_speed(self):
        """
        # Process the whole dialog and ignore all non-done states
        # We just want to get to the end
        # Essentially a simplified version of update()
        """
        self.start_processing()
        while self.state != DialogState.DONE:
            if self.state == DialogState.PROCESS:
                while self.state == DialogState.PROCESS:
                    self._next_char(sound=False)
                    # Skip pauses because we want MAXIMUM VELOCITY of characters
                    if self.state in (DialogState.PAUSE, DialogState.COMMAND_PAUSE):
                        self.state = DialogState.PROCESS

            elif self.state == DialogState.PAUSE_BEFORE_WAIT:
                self.state = DialogState.WAIT

            elif self.state == DialogState.WAIT:
                if self.text_index >= len(self.text_commands):
                    self.state = DialogState.DONE
                else:
                    self.state = DialogState.PROCESS

            elif self.state in (DialogState.PAUSE, DialogState.COMMAND_PAUSE):
                self.state = DialogState.PROCESS

            elif self.state == DialogState.NEW_LINE:
                self.y_offset = 0
                self.state = DialogState.PROCESS
        return True

    def draw_text(self, surf):
        end_x_pos, end_y_pos = 0, 0
        text_surf = engine.create_surface((self.text_width, self.text_height), transparent=True)

        # Draw line that's disappearing
        processed_text_lines = fix_tags(self.text_lines)
        if self.y_offset and len(processed_text_lines) > self.num_lines:
            x_pos = 0
            y_pos = -16 + self.y_offset
            line = processed_text_lines[-self.num_lines - 1]
            width = text_width(self.font_type, line)
            render_text(text_surf, [self.font_type], [line], [self.font_color], (x_pos, y_pos))
            x_pos += width

        display_lines = processed_text_lines[-self.num_lines:]
        for idx, line in enumerate(display_lines):
            x_pos = 0
            y_pos = 16 * idx
            if len(processed_text_lines) > self.num_lines:
                y_set = y_pos + self.y_offset
            else:
                y_set = y_pos
            width = text_width(self.font_type, line)
            render_text(text_surf, [self.font_type], [line], [self.font_color], (x_pos, y_set))
            x_pos += width

            end_x_pos = self.position[0] + 8 + x_pos
            end_y_pos = self.position[1] + 8 + y_pos

        surf.blit(text_surf, (self.position[0] + 8, self.position[1] + 8))
        return end_x_pos, end_y_pos

    def draw_tail(self, surf, portrait: event_portrait.EventPortrait):
        portrait_x = portrait.position[0] + portrait.get_width()//2
        portrait_y = portrait.position[1] + portrait.get_height()//2
        mirror_x = portrait_x < WINWIDTH//2
        mirror_y = self.position[1] > portrait_y
        if mirror_x:
            tail_surf = engine.flip_horiz(self.tail)
        else:
            tail_surf = self.tail
        if mirror_y:
            tail_surf = engine.flip_vert(tail_surf)
            y_pos = self.position[1] - tail_surf.get_height() + 2
        else:
            y_pos = self.position[1] + self.background.get_height() - 2
        x_pos = portrait_x + 20 if mirror_x else portrait_x - 36
        # If we wouldn't actually be on the dialog box
        if x_pos > self.background.get_width() + self.position[0] - 24:
            x_pos = self.position[0] + self.background.get_width() - 24
        elif x_pos < self.position[0] + 8:
            x_pos = self.position[0] + 8

        tail_surf = image_mods.make_translucent(tail_surf, self.dialog_transparency)
        surf.blit(tail_surf, (x_pos, y_pos))

    def draw_nametag(self, surf, name):
        if self.position[1] < 10: # if it would get cut off
            y_pos = self.position[1] + self.height - 6
        else:
            y_pos = self.position[1] - 10
        x_pos = self.position[0] - 4
        if x_pos < 0:
            x_pos = self.position[0] + 16
        name_tag_surf = self.name_tag_surf.copy()
        self.font.blit_center(name, name_tag_surf, (name_tag_surf.get_width()//2, name_tag_surf.get_height()//2 - self.font.height//2), self.font_color)
        surf.blit(name_tag_surf, (x_pos, y_pos))
        return surf

    def draw(self, surf: engine.Surface) -> engine.Surface:
        if self.background:
            if self.state == DialogState.TRANSITION_IN:
                # bg = image_mods.resize(self.background, (1, .5 + self.transition_progress/2.))
                new_width = max(1, self.background.get_width() - 10 + int(10*self.transition_progress))
                new_height = max(1, self.background.get_height() - 10 + int(10*self.transition_progress))
                bg = engine.transform_scale(self.background, (new_width, new_height))
                bg = image_mods.make_translucent(bg, self.dialog_transparency + (0.75 - self.dialog_transparency) * (1 - self.transition_progress))
                surf.blit(bg, (self.position[0], self.position[1] + self.height - bg.get_height()))
            else:
                bg = image_mods.make_translucent(self.background, self.dialog_transparency)
                surf.blit(bg, self.position)

        if self.state != DialogState.TRANSITION_IN:
            # Draw message tail
            if self.portrait and self.tail:
                self.draw_tail(surf, self.portrait)
            # Draw nametag
            if not self.portrait and self.speaker and self.speaker != 'Narrator':
                self.draw_nametag(surf, self.speaker)
            # Draw text
            end_pos = self.draw_text(surf)

            if self.state == DialogState.WAIT and self.draw_cursor_flag:
                cursor_pos = 4 + end_pos[0], \
                    6 + end_pos[1] + self.cursor_offset[self.cursor_offset_index]
                surf.blit(self.cursor, cursor_pos)

        return surf

class DynamicDialogWrapper():
    def __init__(self, text_func: Callable[[], str], portrait=None, background=None, position=None, width=None,
                 speaker=None, style_nid=None, autosize=False, speed: float = 1.0, font_color='black',
                 font_type='convo', num_lines=2, draw_cursor=True, message_tail='message_bg_tail', transparency: float=0.05,
                 name_tag_bg='name_tag', flags=None) -> None:
        # eval trick
        self.resolve_text_func: Callable[[], str] = text_func
        self.resolved_text = process_dialog_shorthand(self.resolve_text_func()).replace('{w}', '')
        # dialog props
        self.portrait = portrait
        self.background = background
        self.position = position
        self.width = width
        self.speaker = speaker
        self.style_nid = style_nid
        self.autosize = autosize
        self.speed = speed
        self.font_color = font_color
        self.font_type = font_type
        self.num_lines = num_lines
        self.draw_cursor = draw_cursor
        self.message_tail = message_tail
        self.transparency = transparency
        self.name_tag_bg = name_tag_bg
        self.flags = flags

        self.dialog = Dialog(self.resolved_text, portrait, background, position, width, speaker, style_nid, autosize, speed, font_color,
                             font_type, num_lines, draw_cursor, message_tail, transparency, name_tag_bg, flags)

    def update(self):
        new_text = process_dialog_shorthand(self.resolve_text_func()).replace('{w}', '')
        if new_text != self.resolved_text:
            self.resolved_text = new_text
            self.dialog = Dialog(self.resolved_text, self.portrait, self.background,
                                 self.position, self.width, self.speaker, self.style_nid,
                                 self.autosize, self.speed, self.font_color, self.font_type,
                                 self.num_lines, self.draw_cursor, self.message_tail, self.transparency,
                                 self.name_tag_bg, self.flags)
            self.dialog.last_update = engine.get_time() - 10000
        return self.dialog.update()

    def draw(self, surf) -> engine.Surface:
        self.dialog.draw(surf)

class LocationCard():
    exist_time = 2000
    transition_speed = 166  # 10 frames

    def __init__(self, text, background='menu_bg_brown'):
        self.plain_text = text
        self.font = FONT['text']
        self.font_name = 'text'

        self.text_lines = self.format_text(text)
        self.determine_size()
        self.position = (10, 1)

        if background:
            self.background = self.make_background(background)
        else:
            self.background = engine.create_surface((self.width, self.height), transparent=True)

        # For transition
        self.transition = 'start'
        self.transition_progress = 0
        self.transition_update = engine.get_time()
        self.start_time = engine.get_time()

    def format_text(self, text):
        return [text]

    def determine_size(self):
        self.width = text_funcs.get_max_width(self.font_name, self.text_lines) + 16
        self.height = len(self.text_lines) * self.font.height + 8

    def make_background(self, background):
        surf = create_base_surf(self.width, self.height, background)
        return surf

    def update(self):
        current_time = engine.get_time()

        if self.transition:
            perc = (current_time - self.transition_update) / self.transition_speed
            self.transition_progress = utils.clamp(perc, 0, 1)
            if self.transition_progress == 1:
                if self.transition == 'end':
                    return False
                self.transition = False

        if not self.transition and current_time - self.start_time > self.exist_time:
            self.transition_update = current_time
            self.transition = 'end'
            self.transition_progress = 0

        return True

    def draw(self, surf):
        bg = self.background.copy()
        # Draw text
        for idx, line in enumerate(self.text_lines):
            self.font.blit_center(line, bg, (bg.get_width()//2, idx * self.font.height + 4))

        if self.transition == 'start':
            # when the location would enter, it's transparency changes from
            # 1.0 (100% transprenct) to .1 (Which is 90% opaque).
            transparency = 1.0 - (0.9 * self.transition_progress)
            bg = image_mods.make_translucent(bg, transparency)
        elif self.transition == 'end':
            # When the location card would leave, it's transparency changes
            # from .1 (90% opaque) to 1.0 (100% transparency)
            transparency = .1 + (self.transition_progress * 0.9)
            bg = image_mods.make_translucent(bg, transparency)
        else:
            bg = image_mods.make_translucent(bg, .1)
        surf.blit(bg, self.position)

        return surf

class Credits():
    speed = 0.02

    def __init__(self, title, text, wait_flag=False, center_flag=True):
        self.title = title
        self.text = text
        self.title_font = FONT['credit_title']
        self.title_font_name = 'credit_title'
        self.font = FONT['credit']
        self.font_name = 'credit'

        self.center_flag = center_flag
        self.wait_flag = wait_flag
        self.waiting = False

        self.make_surf()

        self.position = [0, WINHEIGHT]

        self.pause_update = engine.get_time()
        self.has_paused = False
        self.start_update = engine.get_time()

    def make_surf(self):
        index = 0
        self.parsed_text = []
        for line in self.text:
            x_bound = WINWIDTH - 12 if self.center_flag else WINWIDTH - 88
            lines = text_funcs.line_wrap(self.font_name, line, x_bound)
            for li in lines:
                if self.center_flag:
                    x_pos = WINWIDTH//2 - text_width(self.font_name, li)//2
                else:
                    x_pos = 88
                y_pos = self.font.height * index + self.title_font.height
                index += 1
                self.parsed_text.append((li, index, (x_pos, y_pos)))

        self.num_lines = index

        size = (WINWIDTH, self.title_font.height + self.font.height * self.num_lines)
        self.surf = engine.create_surface(size, transparent=True)

        title_pos_x = 32
        self.title_font.blit(self.title, self.surf, (title_pos_x, 0))

        for text, index, pos in self.parsed_text:
            self.font.blit(text, self.surf, pos)

    def wait_time(self) -> int:
        time = int((self.num_lines + 2) * self.font.height * 50)
        if self.wait_flag:
            time += int(self.pause_time() * 2.1)
        return time

    def pause_time(self) -> int:
        return int((self.num_lines + 1) * 1000)

    def update(self):
        current_time = engine.get_time()

        if not self.waiting or current_time - self.pause_update > self.pause_time():
            self.waiting = False
            ms_passed = current_time - self.start_update
            if self.has_paused:
                ms_passed -= self.pause_time()
            self.position[1] = WINHEIGHT - (ms_passed * self.speed)
            # Should we pause?
            if self.wait_flag and WINHEIGHT//2 - self.surf.get_height()//2 >= self.position[1]:
                self.waiting = True
                self.wait_flag = False
                self.pause_update = current_time
                self.has_paused = True
        return True

    def draw(self, surf):
        surf.blit(self.surf, self.position)
        return surf

class Ending():
    """
    Contains a dialog
    """
    solo_flag = True
    wait_time = 5000
    background = SPRITES.get('endings_display')

    def __init__(self, portrait, title, text, unit):
        self.portrait = portrait
        self.title = title
        self.plain_text = text
        self.speaker = None  # Unused attribute to match Dialog
        self.unit = unit
        self.font = FONT['text']
        self.font_name = 'text'

        self.build_dialog()

        self.make_background()
        self.x_position = WINWIDTH

        self.wait_update = 0

    def build_dialog(self):
        self.dialog = Dialog(self.plain_text, num_lines=6, draw_cursor=False)
        self.dialog.position = (8, 40)
        self.dialog.text_width = WINWIDTH - 32
        self.dialog.width = self.dialog.text_width + 16
        self.dialog.font = FONT['text']
        self.dialog.font_type = 'text'
        self.dialog.font_color = 'white'

    def make_background(self):
        size = WINWIDTH, WINHEIGHT
        self.bg = engine.create_surface(size, transparent=True)
        self.bg.blit(self.background, (0, 0))
        self.bg.blit(self.portrait, (136, 57))

        title_pos_x = 68 - text_width(self.font_name, self.title)//2
        self.font.blit(self.title, self.bg, (title_pos_x, 24))

        # Stats
        if self.unit:
            kills = game.records.get_kills(self.unit.nid)
            damage = game.records.get_damage(self.unit.nid)
            healing = game.records.get_heal(self.unit.nid)

            FONT['text-yellow'].blit(text_funcs.translate('K'), self.bg, (136, 8))
            FONT['text-yellow'].blit(text_funcs.translate('D'), self.bg, (168, 8))
            FONT['text-yellow'].blit(text_funcs.translate('H'), self.bg, (200, 8))
            FONT['text-blue'].blit(str(kills), self.bg, (144, 8))
            dam = str(damage)
            if damage >= 1000:
                dam = dam[:-3] + '.' + dam[-3] + 'k'
            heal = str(healing)
            if healing >= 1000:
                heal = heal[:-3] + '.' + heal[-3] + 'k'
            FONT['text-blue'].blit(dam, self.bg, (176, 8))
            FONT['text-blue'].blit(heal, self.bg, (208, 8))

        return self.bg

    def is_complete(self) -> bool:
        """
        Should stop being drawn
        """
        return False

    def is_done(self) -> bool:
        return self.dialog.is_done()

    def is_done_or_wait(self) -> bool:
        return self.dialog.is_done_or_wait()

    def is_paused(self) -> bool:
        return self.dialog.is_paused()

    def hurry_up(self):
        self.dialog.hurry_up()

    def update(self):
        current_time = engine.get_time()

        # Move in
        if self.x_position > 0:
            self.x_position -= 8
            self.x_position = max(0, self.x_position)
        else:
            self.dialog.update()

        # Only wait for so long
        if self.wait_update:
            if current_time - self.wait_update > self.wait_time:
                self.dialog.state = DialogState.DONE
        elif self.is_done_or_wait():
            self.wait_update = current_time

        return False

    def draw(self, surf):
        bg = self.bg.copy()
        self.dialog.draw(bg)
        surf.blit(bg, (self.x_position, 0))

class PairedEnding(Ending):
    """
    Contains a dialog
    """
    background = SPRITES.get('paired_endings_display')

    def __init__(self, left_portrait, right_portrait, left_title, right_title, text, left_unit, right_unit):
        self.left_portrait = left_portrait
        self.right_portrait = right_portrait
        self.left_title = left_title
        self.right_title = right_title
        self.plain_text = text
        self.speaker = None  # Unused attribute to match Dialog
        self.left_unit = left_unit  # Used in stats
        self.right_unit = right_unit
        self.font_name = 'text'

        self.build_dialog()

        self.make_background()
        self.x_position = WINWIDTH

        self.wait_update = 0

    def make_background(self):
        size = WINWIDTH, WINHEIGHT
        self.bg = engine.create_surface(size, transparent=True)
        self.bg.blit(self.background, (0, 0))
        self.bg.blit(self.left_portrait, (8, 49))
        self.bg.blit(self.right_portrait, (136, 49))

        render_text(self.bg, [self.font_name], [self.left_title], [None], (68, 24), align=HAlignment.CENTER)
        render_text(self.bg, [self.font_name], [self.right_title], [None], (WINWIDTH - 68, WINHEIGHT - 24), align=HAlignment.CENTER)

        # Stats
        if self.left_unit:
            kills = game.records.get_kills(self.left_unit.nid)
            damage = game.records.get_damage(self.left_unit.nid)
            healing = game.records.get_heal(self.left_unit.nid)

            FONT['text-yellow'].blit(text_funcs.translate('K'), self.bg, (136, 8))
            FONT['text-yellow'].blit(text_funcs.translate('D'), self.bg, (168, 8))
            FONT['text-yellow'].blit(text_funcs.translate('H'), self.bg, (200, 8))
            FONT['text-blue'].blit(str(kills), self.bg, (144, 8))
            dam = str(damage)
            if damage >= 1000:
                dam = dam[:-3] + '.' + dam[-3] + 'k'
            heal = str(healing)
            if healing >= 1000:
                heal = heal[:-3] + '.' + heal[-3] + 'k'
            FONT['text-blue'].blit(dam, self.bg, (176, 8))
            FONT['text-blue'].blit(heal, self.bg, (208, 8))

        if self.right_unit:
            kills = game.records.get_kills(self.right_unit.nid)
            damage = game.records.get_damage(self.right_unit.nid)
            healing = game.records.get_heal(self.right_unit.nid)

            FONT['text-yellow'].blit(text_funcs.translate('K'), self.bg, (8, WINHEIGHT - 23))
            FONT['text-yellow'].blit(text_funcs.translate('D'), self.bg, (40, WINHEIGHT - 23))
            FONT['text-yellow'].blit(text_funcs.translate('H'), self.bg, (72, WINHEIGHT - 23))
            FONT['text-blue'].blit(str(kills), self.bg, (16, WINHEIGHT - 23))
            dam = str(damage)
            if damage >= 1000:
                dam = dam[:-3] + '.' + dam[-3] + 'k'
            heal = str(healing)
            if healing >= 1000:
                heal = heal[:-3] + '.' + heal[-3] + 'k'
            FONT['text-blue'].blit(dam, self.bg, (48, WINHEIGHT - 23))
            FONT['text-blue'].blit(heal, self.bg, (80, WINHEIGHT - 23))

        return self.bg
