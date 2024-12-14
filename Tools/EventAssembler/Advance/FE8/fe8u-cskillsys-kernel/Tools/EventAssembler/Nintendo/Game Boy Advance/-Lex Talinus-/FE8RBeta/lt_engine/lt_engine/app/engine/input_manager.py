from __future__ import annotations

from typing import Tuple

from app.constants import WINWIDTH, WINHEIGHT
from app.engine import engine
from app.engine import config as cf

class InputManager():
    def __init__(self):
        self.init_joystick()

        self.buttons = ('UP', 'DOWN', 'LEFT', 'RIGHT', 'SELECT', 'BACK', 'INFO', 'AUX', 'START')
        self.toggle_buttons = self.buttons[4:]  # These buttons cause state to change

        self.update_key_map()

        # Build and down button checker
        self.keys_pressed = {}
        self.joys_pressed = {}
        for button in self.buttons:
            self.keys_pressed[button] = False
            self.joys_pressed[button] = False

        self.update_joystick_control()

        self.key_up_events = []
        self.key_down_events = []
        self.change_keymap_mode = False
        self.current_mouse_position = None

        self.unavailable_button = None

    # The joystick needs be plugged in before this method is called
    def init_joystick(self):
        if engine.joystick_avail():
            joystick = engine.get_joystick()
            joystick.init()
            self.joystick = joystick
            self.joystick_name = joystick.get_name()
            print("Joystick Controller: %s" % self.joystick_name)
        else:
            self.joystick = None
            self.joystick_name = None

    def get_joystick_init(self) -> bool:
        return self.joystick and self.joystick.get_init()

    def set_change_keymap(self, val):
        self.change_keymap_mode = val

    def is_pressed(self, button):
        return self.keys_pressed[button] or self.joys_pressed[button]

    def just_pressed(self, button):
        return button in self.key_down_events
    
    def directional_input_pressed(self):
        directional_inputs = ('UP', 'DOWN', 'LEFT', 'RIGHT')
        return any([input_pressed for input_pressed in directional_inputs if self.is_pressed(input_pressed)])

    def screen_denominator(self) -> Tuple[float, float]:
        if cf.SETTINGS['fullscreen']:  # Fullscreen
            x, y = engine.get_screen_size()
            return (x / WINWIDTH, y / WINHEIGHT)
        else:
            return float(cf.SETTINGS['screen_size']), float(cf.SETTINGS['screen_size'])

    def get_mouse_position(self):
        if self.current_mouse_position:
            x, y = self.screen_denominator()
            return (self.current_mouse_position[0] // x,
                    self.current_mouse_position[1] // y)
        else:
            return None

    def get_real_mouse_position(self):
        """
        # Works whether or not mouse has been moved recently
        """
        if not cf.SETTINGS['mouse']:
            return None
        mouse_pos = engine.get_mouse_pos()
        x, y = self.screen_denominator()
        mouse_pos = (mouse_pos[0] // x,
                     mouse_pos[1] // y)
        if engine.get_mouse_focus():
            return mouse_pos
        else:  # Returns None if mouse is not in screen
            return None

    def update(self):
        self.update_key_map()
        self.update_joystick_control()

    def update_key_map(self):
        self.key_map = {}
        self.key_map['UP'] = cf.SETTINGS['key_UP']
        self.key_map['LEFT'] = cf.SETTINGS['key_LEFT']
        self.key_map['RIGHT'] = cf.SETTINGS['key_RIGHT']
        self.key_map['DOWN'] = cf.SETTINGS['key_DOWN']
        self.key_map['SELECT'] = cf.SETTINGS['key_SELECT']
        self.key_map['START'] = cf.SETTINGS['key_START']
        self.key_map['BACK'] = cf.SETTINGS['key_BACK']
        self.key_map['AUX'] = cf.SETTINGS['key_AUX']
        self.key_map['INFO'] = cf.SETTINGS['key_INFO']

        self.map_keys = {v: k for k, v in self.key_map.items()}

    def update_joystick_control(self):
        self.joystick_control = {}

        self.joystick_control['SELECT'] = [('is_button', 0)]  # A
        self.joystick_control['BACK'] = [('is_button', 1)]  # B
        self.joystick_control['START'] = [('is_button', 3), ('is_button', 6), ('is_button', 7)]  # Y, Select, Start
        self.joystick_control['INFO'] = [('is_button', 2), ('is_button', 5), ('is_axis', 2, -0.5, 4)] # X, RB, R2
        self.joystick_control['AUX'] = [('is_button', 4), ('is_axis', 2, 0.5, 5)] # LB, L2
        # hat
        self.joystick_control['LEFT'] = [('is_hat', 0, 'x', -1, 0), ('is_axis', 0, -0.5, 0)]
        self.joystick_control['RIGHT'] = [('is_hat', 0, 'x', 1, 1), ('is_axis', 0, 0.5, 1)]
        self.joystick_control['UP'] = [('is_hat', 0, 'y', 1, 2), ('is_axis', 1, -0.5, 2)]
        self.joystick_control['DOWN'] = [('is_hat', 0, 'y', -1, 3), ('is_axis', 1, 0.5, 3)]

        # handle buttons that need to know when they were last pressed
        self.button_state = {k: False for k in range(10)}
        self.hat_state = {k: False for k in range(4)}
        self.axis_state = {k: False for k in range(6)}

    def process_input(self, events):
        self.key_up_events.clear()
        self.key_down_events.clear()

        # Check keyboard
        for event in events:
            if event.type == engine.KEYUP or event.type == engine.KEYDOWN:
                button = self.map_keys.get(event.key)
                key_up = event.type == engine.KEYUP

                if button:
                    # Update keys pressed
                    self.keys_pressed[button] = not key_up
                    if key_up:
                        self.key_up_events.append(button)
                    else:
                        self.key_down_events.append(button)
                elif self.change_keymap_mode and event.type == engine.KEYDOWN:
                    self.unavailable_button = event.key
                    return 'NEW'

        # Check mouse
        if not self.change_keymap_mode and cf.SETTINGS['mouse']:
            self.current_mouse_position = None
            for event in events:
                if event.type == engine.MOUSEBUTTONDOWN:
                    lmb = event.button == 1
                    if lmb:
                        self.key_down_events.append('SELECT')
                    rmb = event.button == 3
                    if rmb:
                        self.key_down_events.append('BACK')
                    mmb = event.button == 2
                    if mmb:
                        self.key_down_events.append('INFO')
                    wheel_up = event.button == 4
                    wheel_down = event.button == 5
                    if wheel_up:
                        self.key_down_events.append('UP')
                    else:
                        self.key_up_events.append('UP')
                    if wheel_down:
                        self.key_down_events.append('DOWN')
                    else:
                        self.key_up_events.append('DOWN')
                    position = event.pos
                    self.current_mouse_position = position
                elif event.type == engine.MOUSEMOTION:
                    position = event.pos
                    self.current_mouse_position = position

        # Check game pad
        if not self.joystick:
            self.init_joystick()
        # Need to re-init joystick if we closed and then
        # re-opened the engine from the editor
        if self.joystick and not self.get_joystick_init():
            self.init_joystick()

        if self.joystick and not self.change_keymap_mode:
            self.handle_joystick()

        # Return the correct event for this frame
        # Gives priority to later inputs
        # Remove reversed to give priority to earlier inputs
        for button in reversed(self.key_down_events):
            if button in self.toggle_buttons:
                return button
        # If only arrow keys pressed, return last one pressed
        if self.key_down_events:
            return self.key_down_events[-1]

    def handle_joystick(self):
        def update_state(pushed, state, button_id, button):
            # If state change
            if pushed != state[button_id]:
                self.joys_pressed[button] = pushed
                state[button_id] = pushed
                if pushed:
                    self.key_down_events.append(button)
                else:
                    self.key_up_events.append(button)
                return True
            return False

        for button in self.buttons:
            controls = self.joystick_control.get(button)
            for control in reversed(controls):
                # If the button behaves like a normal button
                if control[0] == 'is_button' and self.joystick.get_numbuttons() > control[1]:
                    pushed = self.joystick.get_button(control[1])
                    update_state(pushed, self.button_state, control[1], button)

                # If the button is controlured to a hat direction
                elif control[0] == 'is_hat' and self.joystick.get_numhats() > control[1]:
                    status = self.joystick.get_hat(control[1])
                    if control[2] == 'x':  # Which axis
                        amount = status[0]
                    else:  # 'y'
                        amount = status[1]
                    pushed = amount == control[3]  # Binary amounts
                    update_state(pushed, self.hat_state, control[4], button)

                # If the button is controlured to a joystick
                elif control[0] == 'is_axis' and self.joystick.get_numaxes() > control[1]:
                    amount = self.joystick.get_axis(control[1])
                    if control[2] < 0:
                        pushed = amount < control[2]
                    elif control[2] > 0:
                        pushed = amount > control[2]
                    update_state(pushed, self.axis_state, control[3], button)

INPUT: InputManager = None
def get_input_manager() -> InputManager:
    global INPUT
    if not INPUT:
        INPUT = InputManager()
    return INPUT
