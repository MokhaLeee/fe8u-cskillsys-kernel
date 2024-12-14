import logging
import math
import os
import collections
from datetime import datetime
import time
from app import lt_log

from app.constants import WINWIDTH, WINHEIGHT, VERSION, FPS
from app.engine import engine
from app.data.database.database import DB

import app.engine.config as cf

def start(title, from_editor=False):
    if from_editor:
        engine.constants['standalone'] = False
    engine.init()
    icon = engine.image_load('favicon.ico')
    engine.set_icon(icon)

    from app.engine import sprites
    sprites.load_images()

    from app.engine import fonts
    fonts.load_fonts()

    # Hack to get icon to show up in windows
    try:
        import ctypes
        myappid = u'rainlash.lextalionis.ltmaker.current' # arbitrary string
        ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)
    except:
        print("Maybe not Windows? (but that's OK)")

    engine.DISPLAYSURF = engine.build_display(engine.SCREENSIZE)
    engine.SCREENSIZE = (engine.DISPLAYSURF.get_width(), engine.DISPLAYSURF.get_height())
    engine.update_time()
    engine.set_title(title + ' - v' + VERSION)
    print("Version: %s" % VERSION)
    if DB.game_flags.has_fatal_errors and not from_editor:
        raise Exception("Fatal errors detected in game. If you are the developer, please validate and then save your game data before proceeding. Aborting launch.")

screenshot = False
def save_screenshot(raw_events: list, surf):
    global screenshot
    for e in raw_events:
        if e.type == engine.KEYDOWN and e.key == engine.key_map['`']:
            screenshot = True
            if not os.path.isdir('screenshots'):
                os.mkdir('screenshots')
        elif e.type == engine.KEYUP and e.key == engine.key_map['`']:
            screenshot = False
        elif e.type == engine.KEYDOWN and e.key == engine.key_map['f12']:
            if not os.path.isdir('screenshots'):
                os.mkdir('screenshots')
            current_time = str(datetime.now()).replace(' ', '_').replace(':', '.')
            engine.save_surface(surf, 'screenshots/LT_%s.png' % current_time)
    if screenshot:
        current_time = str(datetime.now()).replace(' ', '_').replace(':', '.')
        engine.save_surface(surf, 'screenshots/LT_%s.bmp' % current_time)

def draw_fps(surf, fps_records):
    from app.engine.fonts import FONT
    total_time = sum(fps_records)
    num_frames = len(fps_records)
    fps = int(num_frames / (total_time / 1000))
    max_frame = max(fps_records)
    min_fps = 1000 // max_frame

    FONT['small-white'].blit(str(fps), surf, (surf.get_width() - 20, 0))
    FONT['small-white'].blit(str(min_fps), surf, (surf.get_width() - 20, 12))

def draw_soft_reset(surf, remaining_time: int):
    from app.engine.fonts import FONT
    FONT['chapter-yellow'].blit(str(remaining_time), surf, (surf.get_width()//2 - 4, surf.get_height()//2 - 4))

def check_soft_reset(game, inp) -> bool:
    return game.state.current() != 'title_start' and \
        inp.is_pressed('SELECT') and inp.is_pressed('BACK') and \
        inp.is_pressed('START')

def run(game):
    from app.engine.sound import get_sound_thread
    from app.engine.game_counters import ANIMATION_COUNTERS
    from app.engine.input_manager import get_input_manager

    ANIMATION_COUNTERS.reset()

    get_sound_thread().reset()
    get_sound_thread().set_music_volume(cf.SETTINGS['music_volume'])
    get_sound_thread().set_sfx_volume(cf.SETTINGS['sound_volume'])

    surf = engine.create_surface((WINWIDTH, WINHEIGHT))
    # import time
    clock = engine.Clock()
    fps_records = collections.deque(maxlen=FPS)
    inp = get_input_manager()

    _error_mode = False
    _error_msg = ''
    _soft_reset_start_time: int = None  # UTC time.time()
    SOFT_RESET_TIME = 3  # seconds
    while True:
        # start = time.time_ns()
        engine.update_time()
        fps_records.append(engine.get_delta())
        # print(engine.get_delta())

        raw_events = engine.get_events()

        if raw_events == engine.QUIT:
            break

        event = inp.process_input(raw_events)

        # Handle soft reset
        if check_soft_reset(game, inp):
            # Set the start time if not already set
            if not _soft_reset_start_time:
                _soft_reset_start_time = time.time()
            if time.time() - SOFT_RESET_TIME >= _soft_reset_start_time:
                _soft_reset_start_time = None
                _error_mode = False
                game.state.change('title_start')
                game.state.update([], surf)
                continue
        else:
            _soft_reset_start_time = None

        # game loop. catch and log any errors in this loop.
        if _error_mode:
            surf = engine.write_system_msg(surf, _error_msg)
            if inp.is_pressed('SELECT') or inp.is_pressed('BACK'):
                log_file = lt_log.get_log_fname()
                if log_file:
                    os.startfile(log_file)
        else:
            try:
                surf, repeat = game.state.update(event, surf)
                while repeat:  # Let's the game traverse through state chains
                    # print("Repeating States:\t", game.state.state)
                    surf, repeat = game.state.update([], surf)
                # print("States:\t\t\t", game.state.state)

                if cf.SETTINGS['display_fps']:
                    draw_fps(surf, fps_records)
                if _soft_reset_start_time:
                    draw_soft_reset(surf, math.ceil(SOFT_RESET_TIME - (time.time() - _soft_reset_start_time)))
            except Exception as e:
                logging.exception("Game crashed with exception.")
                log_file_loc = lt_log.get_log_dir() or ''
                _error_msg = "Game crashed with exception:\n%s\nPlease press either the **SELECT** or **BACK** keys to open the log file. Please send the contents of the log file to the game developer to resolve this issue.\nLogs can be found in **%s**" % (str(e).strip(), str(log_file_loc))
                _error_mode = True
                # If we're in editor/debug mode, just throw the error normally
                if cf.SETTINGS['debug']:
                    raise e

        get_sound_thread().update(raw_events)

        engine.push_display(surf, engine.SCREENSIZE, engine.DISPLAYSURF)

        save_screenshot(raw_events, surf)

        engine.update_display()
        # end = time.time_ns()
        # milliseconds_elapsed = (end - start)/1e6
        # if milliseconds_elapsed > 10:
        #     print("Engine took too long: %f" % milliseconds_elapsed)

        game.playtime += clock.tick()

def run_in_isolation(obj):
    """
    Requires that the object has
    1) take_input function that takes in the event
    2) update function
    3) draw function that returns the surface to be drawn
    """
    from app.engine.sound import get_sound_thread
    from app.engine.input_manager import get_input_manager

    get_sound_thread().reset()
    get_sound_thread().set_music_volume(cf.SETTINGS['music_volume'])
    get_sound_thread().set_sfx_volume(cf.SETTINGS['sound_volume'])

    surf = engine.create_surface((WINWIDTH, WINHEIGHT))
    clock = engine.Clock()
    while True:
        engine.update_time()

        raw_events = engine.get_events()
        if raw_events == engine.QUIT:
            break
        event = get_input_manager().process_input(raw_events)

        obj.take_input(event)
        obj.update()
        surf = obj.draw(surf)

        get_sound_thread().update(raw_events)

        engine.push_display(surf, engine.SCREENSIZE, engine.DISPLAYSURF)
        save_screenshot(raw_events, surf)

        engine.update_display()
        clock.tick()

def run_combat(mock_combat):
    run_in_isolation(mock_combat)

def run_event(event):
    run_in_isolation(event)
