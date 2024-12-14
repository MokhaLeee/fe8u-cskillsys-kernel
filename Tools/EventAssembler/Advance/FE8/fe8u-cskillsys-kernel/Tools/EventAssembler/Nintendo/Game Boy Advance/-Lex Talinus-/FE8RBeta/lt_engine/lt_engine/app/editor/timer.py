import time

from PyQt5.QtWidgets import QWidget
from PyQt5.QtCore import QTimer, pyqtSignal

from app import constants
from app import counters
from app.utilities.utils import frames2ms

from app.editor.settings import MainSettingsController

class Timer(QWidget):
    tick_elapsed = pyqtSignal()

    def __init__(self, fps=60):
        super().__init__()
        self.main_timer = QTimer()
        self.main_timer.timeout.connect(self.tick)
        timer_speed = int(1000/float(fps))
        self.main_timer.setInterval(timer_speed)
        self.main_timer.start()

        self.autosave_timer = QTimer()
        autosave_time = MainSettingsController().get_autosave_time(5)
        self.autosave_timer.setInterval(int(autosave_time * 60 * 1000))
        self.autosave_timer.start()

        self.passive_counter = counters.generic3counter(frames2ms(32), frames2ms(4))
        self.active_counter = counters.generic3counter(frames2ms(13), frames2ms(6))
        self.move_sprite_counter = counters.simplecounter((frames2ms(13), frames2ms(6), frames2ms(13), frames2ms(6)))

    def tick(self):
        current_time = int(round(time.time() * 1000))
        self.passive_counter.update(current_time)
        self.active_counter.update(current_time)
        self.move_sprite_counter.update(current_time)
        self.tick_elapsed.emit()

    def start(self):
        self.main_timer.start()
        self.autosave_timer.start()

    def start_for_editor(self):
        self.main_timer.start()

    def stop(self):
        self.main_timer.stop()
        self.autosave_timer.stop()

    def stop_for_editor(self):
        self.main_timer.stop()

TIMER = None
def get_timer():
    global TIMER
    if not TIMER:
        TIMER = Timer(constants.FPS//2)
    return TIMER
