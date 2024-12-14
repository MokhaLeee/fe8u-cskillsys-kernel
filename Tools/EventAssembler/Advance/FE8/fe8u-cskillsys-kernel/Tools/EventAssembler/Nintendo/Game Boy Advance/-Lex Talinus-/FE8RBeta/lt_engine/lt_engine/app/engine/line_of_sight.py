from app.utilities import utils
from enum import IntEnum

from app.engine.game_state import game
from app.engine import skill_system
from app.engine.bresenham_line_algorithm import get_line

class Visibility(IntEnum):
    Unknown = 0
    Dark = 1
    Lit = 2

def line_of_sight(source_pos: list, dest_pos: list, max_range: int) -> list:
    all_tiles = {}
    for pos in dest_pos:
        if pos in source_pos:
            all_tiles[pos] = Visibility.Lit
        else:
            all_tiles[pos] = Visibility.Unknown

    # Iterate over remaining tiles
    for pos, vis in all_tiles.items():
        if vis == Visibility.Unknown:
            for s_pos in source_pos:
                if utils.calculate_distance(pos, s_pos) <= max_range and get_line(s_pos, pos, game.board.get_opacity):
                    all_tiles[pos] = Visibility.Lit
                    break
            else:
                all_tiles[pos] = Visibility.Dark

    lit_tiles = [pos for pos in dest_pos if all_tiles[pos] != Visibility.Dark]
    return lit_tiles

def simple_check(dest_pos: tuple, team: str, default_range: int) -> bool:
    """
    Returns true if can see position with line of sight
    """
    info = [(unit.position, skill_system.sight_range(unit)) for unit in game.units if unit.position and unit.team == team]
    for s_pos, extra_range in info:
        if s_pos == dest_pos:
            return True
        elif utils.calculate_distance(dest_pos, s_pos) <= default_range + extra_range and get_line(s_pos, dest_pos, game.board.get_opacity):
            return True
    return False

if __name__ == '__main__':
    import random, time
    num_trials = 100000  # 400 +/- 30 ms
    random_nums = [random.randint(0, 9) for i in range(num_trials * 4)]
    start = time.time_ns() / 1e6
    for x in range(num_trials):
        out = bool(get_line(
            (random_nums[x * 4], random_nums[x * 4 + 1]), 
            (random_nums[x * 4 + 2], random_nums[x * 4 + 3]),
            lambda x: False))
    end = time.time_ns() / 1e6
    print(end - start)

    print(out)
