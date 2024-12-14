from datetime import datetime
import sys, os
import pathlib
import logging
import threading
import time
from typing import Optional
import app.utilities.platformdirs as appdirs

from app.constants import APP_AUTHOR, APP_NAME, VERSION

# Taken from: https://stackoverflow.com/a/61043789
# Maxxim's answer
class LogFormatter(logging.Formatter):
    COLOR_CODES = {
        logging.CRITICAL: "\033[1;35m", # bright/bold magenta
        logging.ERROR: "\033[1;31m", # bright/bold red
        logging.WARNING: "\033[1;33m", # bright/bold yellow
        logging.INFO: "\033[0;37m", # white / light gray
        logging.DEBUG: "\033[1;30m", # bright/bold black / dark gray
    }

    RESET_CODE = "\033[0m"

    def __init__(self, color: bool, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.color: bool = color

    def format(self, record, *args, **kwargs):
        if self.color and record.levelno in self.COLOR_CODES:
            record.color_on = self.COLOR_CODES[record.levelno]
            record.color_off = self.RESET_CODE
        else:
            record.color_on = ""
            record.color_off = ""
        return super().format(record, *args, **kwargs)

# Setup logging
def setup_logging(console_log_output, console_log_level, console_log_color, logfile_file, logfile_log_level, logfile_log_color, log_line_template):
    logger = logging.getLogger()

    logger.setLevel(logging.DEBUG)

    console_log_output = console_log_output.lower()
    if console_log_output == "stdout":
        console_log_output = sys.stdout
    elif console_log_output == "stderr":
        console_log_output = sys.stderr
    else:
        print("Failed to set console output: invalid output: '%s'" % console_log_output)
        return False
    console_handler = logging.StreamHandler(console_log_output)

    # Set console log level
    try:
        console_handler.setLevel(console_log_level.upper())
    except Exception as exception:
        print("Failed to set console log level: invalid level: '%s'" % console_log_level)
        print(exception)
        return False

    # Create and set formatter, add console handler to logger
    console_formatter = LogFormatter(fmt=log_line_template, color=console_log_color)
    console_handler.setFormatter(console_formatter)
    logger.addHandler(console_handler)

    # Create log file handler
    try:
        logfile_handler = logging.FileHandler(logfile_file, 'w', 'utf-8')
    except Exception as exception:
        print("Failed to set up log file: %s" % str(exception))
        return False

    # Set log file log level
    try:
        logfile_handler.setLevel(logfile_log_level.upper())
    except:
        print("Failed to set log file log level: invalid level: '%s'" % logfile_log_level)
        return False

    # Create and set formatter, add log file handler to logger
    logfile_formatter = LogFormatter(fmt=log_line_template, color=logfile_log_color)
    logfile_handler.setFormatter(logfile_formatter)
    logger.addHandler(logfile_handler)

    return True

def get_log_fname() -> Optional[str]:
    log_files = [i.baseFilename for i in logging.getLogger().handlers if hasattr(i, 'baseFilename')]
    return log_files[0] if log_files else None

def create_debug_log(log_dir: os.PathLike) -> os.PathLike:
    """
    Destroys logs older than 24 hours
    Then initializes a current log
    """
    for root, _, files in os.walk(log_dir):
        for name in files:
            fn = os.path.join(root, name)
            if fn.endswith('.log') and os.path.exists(fn):
                last_modified_hours_ago = (time.time() - os.path.getmtime(fn)) / 3600
                if last_modified_hours_ago > 24:
                    os.remove(fn)
    current_time = str(datetime.now()).replace(' ', '_').replace(':', '.')
    nfn = log_dir / pathlib.Path('debug.' + str(current_time) + '.log')
    # creates the file
    with open(nfn, 'w'):
        pass
    return nfn

def get_log_dir() -> os.PathLike:
    appdata_dir = pathlib.Path(appdirs.user_log_dir(APP_NAME, APP_AUTHOR))
    if not os.path.isdir(appdata_dir):
        os.makedirs(appdata_dir)
    return appdata_dir

def touch_log():
    current_time = time.time()
    log = get_log_fname()
    if log:
        # touch once per hour
        os.utime(log, (current_time, current_time))
        t = threading.Timer(3600.0, touch_log)
        t.daemon = True
        t.start()

def create_logger() -> bool:
    try:
        debug_fn = create_debug_log(get_log_dir())
    except PermissionError:
        print("No permission to write to AppData.")
        return False
    success = setup_logging(console_log_output="stdout", console_log_level="warning", console_log_color=False,
                            logfile_file=debug_fn, logfile_log_level="debug", logfile_log_color=False,
                            log_line_template="%(color_on)s%(relativeCreated)d %(levelname)7s:%(module)16s: %(message)s")
    if not success:
        print("Failed to setup logging")
        return False
    logging.info('*** Lex Talionis Engine Version %s ***' % VERSION)
    touch_log()
    return True
