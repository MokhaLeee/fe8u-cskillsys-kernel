from typing import Callable, Dict
import inspect

from app.events import event_functions, overworld_event_functions
from app.utilities.typing import NID

FUNCTION_CATALOG: Dict[NID, Callable] = {}

# Get all event command implementations
def get_catalog():
    global FUNCTION_CATALOG
    if not FUNCTION_CATALOG:
        # general_funcs = {func: getattr(event_functions, func) for func in dir(event_functions) if callable(getattr(event_functions, func)) and not func.startswith("__")}
        # overworld_funcs = {func: getattr(overworld_event_functions, func) for func in dir(overworld_event_functions) if callable(getattr(overworld_event_functions, func)) and not func.startswith("__")}
        # print([(k, v.__module__) for (k, v) in inspect.getmembers(event_functions, inspect.isfunction)])
        general_funcs = {k: v for (k, v) in inspect.getmembers(event_functions, inspect.isfunction) if not k.startswith("__") and v.__module__ == 'app.events.event_functions'}
        overworld_funcs = {k: v for (k, v) in inspect.getmembers(overworld_event_functions, inspect.isfunction) if not k.startswith("__") and v.__module__ == 'app.events.overworld_event_functions'}
        general_funcs.update(overworld_funcs)
        FUNCTION_CATALOG = general_funcs
    return FUNCTION_CATALOG

def reset_catalog():
    global FUNCTION_CATALOG
    FUNCTION_CATALOG = {}
