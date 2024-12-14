from app.utilities.data import Data
from app.utilities import str_utils

import logging

class CombatAnimationCommand():
    def __init__(self, nid=None, name='', attr=bool, value=(True,), tag=None, desc=''):
        self.nid: str = nid
        self.name: str = name
        self.attr: tuple = attr  # Can have multiple attributes
        self.value: tuple = value
        self.tag: str = tag
        self.desc: str = desc

    def __repr__(self):
        return "%s: %s" % (self.nid, self.value)

    @classmethod
    def copy(cls, other):
        return cls(other.nid, other.name, other.attr, other.value, other.tag, other.desc)

    def save(self):
        return self.nid, self.value

    def has_frames(self) -> bool:
        return self.attr and 'frame' in self.attr

    def has_effect(self) -> bool:
        return self.attr and 'effect' in self.attr

    def get_frames(self) -> list:
        return [self.value[idx] for idx in range(len(self.attr)) if self.attr[idx] == 'frame']

    def increment_frame_count(self, inc=1):
        """
        Change the number of frames a frame should be displayed for
        """
        if self.tag == 'frame':
            self.value = (self.value[0] + inc, *self.value[1:])

    def set_frame_count(self, val=1):
        """
        Set the number of frames a frame should be displayed
        """
        if self.tag == 'frame':
            self.value = (val, *self.value[1:])

def parse_attr(attr, text: str):
    if attr is None:
        return None
    elif attr is bool:
        return True
    elif attr is int:
        return int(text)
    elif attr == 'color':
        return tuple(int(_) for _ in text.split(','))
    elif attr == 'frame':
        return text
    elif attr == 'sound':
        return text
    elif attr == 'effect':
        return text

def generate_text(command: CombatAnimationCommand) -> str:
    s = [command.nid]
    if isinstance(command.attrs, tuple):
        for idx, attr in command.attrs:
            if command.value[idx] is not None:
                s.append(str(command.value[idx]))
    return ';'.join(s)

def parse_text(text: str) -> CombatAnimationCommand:
    split_text = text.split(';')
    logging.info("Import %s", split_text)
    command_nid = split_text[0]
    if command_nid == 'f':
        if len(split_text) == 3: 
            command_nid = 'frame'
        elif len(split_text) == 4:
            command_nid = 'dual_frame'
        elif len(split_text) == 5:
            command_nid = 'frame_with_offset'
            vals = str_utils.intify(split_text[4])
            split_text = [split_text[0], split_text[1], split_text[2], vals[0], vals[1]]
    elif command_nid == 'of':
        command_nid = 'over_frame'
    elif command_nid == 'uf':
        command_nid = 'under_frame'
    elif command_nid == 'self_flash_white':
        command_nid = 'self_tint'
        split_text.append('255,255,255')
    elif command_nid == 'enemy_flash_white':
        command_nid = 'enemy_tint'
        split_text.append('255,255,255')
    elif command_nid == 'screen_flash_white':
        split_text.append('255,255,255')
        if len(split_text) > 3:
            command_nid = 'screen_blend_with_fade_out'
        else:
            command_nid = 'screen_blend'
    elif command_nid == 'set_parent_opacity':
        command_nid = 'parent_opacity'
    elif command_nid == 'parent_tint_loop':
        num_frames = split_text[1]
        command_nid = 'parent_tint_blend'
        split_text.clear()
        split_text.append('parent_tint_blend')
        split_text.append(num_frames)
        split_text.append('0,0,0')
        split_text.append('248,248,248')
    elif command_nid == 'effect':
        if len(split_text) > 2:
            command_nid = 'effect_with_offset'
            vals = str_utils.intify(split_text[2])
            split_text.pop()
            split_text.append(vals[0])
            split_text.append(vals[1])
    elif command_nid == 'under_effect':
        if len(split_text) > 2:
            command_nid = 'under_effect_with_offset'
            vals = str_utils.intify(split_text[2])
            split_text.pop()
            split_text.append(vals[0])
            split_text.append(vals[1])
    elif command_nid == 'enemy_effect':
        if len(split_text) > 2:
            command_nid = 'enemy_effect_with_offset'
            vals = str_utils.intify(split_text[2])
            split_text.pop()
            split_text.append(vals[0])
            split_text.append(vals[1])
    command = get_command(command_nid)
    if not command:
        return None
    values = []
    if command.attr:
        for idx, attr in enumerate(command.attr):
            if len(split_text) > idx + 1:
                value = parse_attr(attr, split_text[idx + 1])
            elif attr is bool:
                value = True
            else:
                value = None
            values.append(value)
    if len(values) == 0:
        pass
    else:
        command.value = tuple(values)
    return command

anim_commands = Data([
    CombatAnimationCommand('frame', 'Display Frame', (int, 'frame'), (0, None), 'frame', 'Displays the animation image for # of frames'),
    CombatAnimationCommand('wait', 'Wait', (int,), (0,), 'frame', "Don't display any image for # of frames"),
    CombatAnimationCommand('over_frame', 'Display Over Frame', (int, 'frame'), (0, None), 'frame', 'Displays the animation image above all other images for # of frames'),
    CombatAnimationCommand('under_frame', 'Display Under Frame', (int, 'frame'), (0, None), 'frame', 'Displays the animation image below all other images for # of frames'),
    CombatAnimationCommand('dual_frame', 'Display Dual Frame', (int, 'frame', 'frame'), (0, None, None), 'frame', 'Display two animation images at the same time for # of frames. The second is always the bottommost image drawn'),
    CombatAnimationCommand('frame_with_offset', 'Display Frame With Offset', (int, 'frame', int, int), (0, None, 0, 0), 'frame', 'Displays the animation image with a custom (x, y) offset for # of frames'),
    
    CombatAnimationCommand('sound', 'Play Sound', ('sound',), (None,), 'sound'),
    CombatAnimationCommand('stop_sound', 'Stop Sound', ('sound',), (None,), 'sound'),

    CombatAnimationCommand('start_hit', 'Start Normal Hit Routine', None, None, 'process'),
    CombatAnimationCommand('wait_for_hit', 'Wait for End of Normal Hit Routine', ('frame', 'frame'), (None, None), 'process'),
    CombatAnimationCommand('miss', 'Miss', None, None, 'process'),
    CombatAnimationCommand('spell', 'Cast Spell', ('effect',), (None,), 'process'),
    CombatAnimationCommand('spell_hit', 'Spell Hit', None, None, 'process'),

    CombatAnimationCommand('self_tint', 'Tint Self', (int, 'color'), (0, (248, 248, 248)), 'aesthetic1'),
    CombatAnimationCommand('parent_tint', 'Tint Parent', (int, 'color'), (0, (248, 248, 248)), 'aesthetic1'),
    CombatAnimationCommand('enemy_tint', 'Tint Enemy', (int, 'color'), (0, (248, 248, 248)), 'aesthetic1'),
    CombatAnimationCommand('parent_tint_blend', 'Tint Parent Between 2 Colors', (int, 'color', 'color'), (0, (0, 0, 0), (248, 248, 248)), 'aesthetic1'),
    CombatAnimationCommand('self_screen_dodge', 'Screen Dodge Self', (int, 'color'), (0, (248, 248, 248)), 'aesthetic1'),
    CombatAnimationCommand('enemy_screen_dodge', 'Screen Dodge Enemy', (int, 'color'), (0, (248, 248, 248)), 'aesthetic1'),
    CombatAnimationCommand('background_blend', 'Tint Background', (int, 'color'), (0, (248, 248, 248)), 'aesthetic1'),
    CombatAnimationCommand('foreground_blend', 'Tint Foreground', (int, 'color'), (0, (248, 248, 248)), 'aesthetic1'),
    CombatAnimationCommand('screen_blend', 'Tint Entire Screen', (int, 'color'), (0, (248, 248, 248)), 'aesthetic1'),
    CombatAnimationCommand('screen_blend_with_fade_out', 'Tint Entire Screen (Fade Out Tint)', (int, int, 'color'), (0, 0, (248, 248, 248)), 'aesthetic1'),
    CombatAnimationCommand('opacity', 'Set Opacity', (int,), (0,), 'aesthetic1'),
    CombatAnimationCommand('parent_opacity', 'Set Parent Opacity', (int,), (0,), 'aesthetic1'),

    CombatAnimationCommand('platform_shake', 'Shake Platform', None, None, 'aesthetic2'),
    CombatAnimationCommand('screen_shake', 'Shake Screen', None, None, 'aesthetic2'),
    CombatAnimationCommand('hit_spark', 'Show Hit Spark', None, None, 'aesthetic2'),
    CombatAnimationCommand('crit_spark', 'Show Crit Spark', None, None, 'aesthetic2'),
    CombatAnimationCommand('darken', 'Darken Background', None, None, 'aesthetic2'),
    CombatAnimationCommand('lighten', 'Lighten Background', None, None, 'aesthetic2'),

    CombatAnimationCommand('effect', 'Show Effect On Self', ('effect',), (None,), 'effect'),
    CombatAnimationCommand('under_effect', 'Show Effect Under Self', ('effect',), (None,), 'effect'),
    CombatAnimationCommand('enemy_effect', 'Show Effect On Enemy', ('effect',), (None,), 'effect'),
    CombatAnimationCommand('enemy_under_effect', 'Show Effect Under Enemy', ('effect',), (None,), 'effect'),
    CombatAnimationCommand('effect_with_offset', 'Show Effect On Self With Offset', ('effect', int, int), (None, 0, 0), 'effect'),
    CombatAnimationCommand('under_effect_with_offset', 'Show Effect Under Self With Offset', ('effect', int, int), (None, 0, 0), 'effect'),
    CombatAnimationCommand('enemy_effect_with_offset', 'Show Effect On Enemy With Offset', ('effect', int, int), (None, 0, 0), 'effect'),
    CombatAnimationCommand('clear_all_effects', 'Clear All Effects', None, None, 'effect'),

    CombatAnimationCommand('pan', 'Pan Screen', None, None, 'aesthetic3'),
    CombatAnimationCommand('blend', 'Set Frame Blending', (bool,), (True,), 'aesthetic3'),
    CombatAnimationCommand('static', 'Set Has Static Position', (bool,), (True,), 'aesthetic3'),
    CombatAnimationCommand('ignore_pan', 'Set Ignore Pan', (bool,), (True,), 'aesthetic3'),
    
    CombatAnimationCommand('start_loop', 'Start Loop', None, None, 'loop'),
    CombatAnimationCommand('end_loop', 'End Loop', None, None, 'loop'),
    CombatAnimationCommand('end_parent_loop', 'Break Parent Loop', None, None, 'loop'),
    CombatAnimationCommand('end_child_loop', 'Break All Effect Loops', None, None, 'loop')
])
    

def get_command(nid) -> CombatAnimationCommand:
    base = anim_commands.get(nid)
    if base:
        return CombatAnimationCommand.copy(base)
    return None
