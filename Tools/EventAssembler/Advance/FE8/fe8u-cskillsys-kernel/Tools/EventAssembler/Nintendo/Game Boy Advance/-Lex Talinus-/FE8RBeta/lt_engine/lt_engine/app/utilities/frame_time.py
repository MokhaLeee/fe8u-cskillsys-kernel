import time, functools
from app.engine import engine

def frame_time(func):
    func.current_time = 0
    func.total_time = 0

    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        a = time.perf_counter() * 1000
        output = func(*args, **kwargs)
        b = time.perf_counter() * 1000
        if engine.get_time() != func.current_time:
            func.current_time = engine.get_time()
            print("%s: %05.3f" % (func.__name__, func.total_time))
            func.total_time = 0
        func.total_time += b - a
        return output
    return wrapper
