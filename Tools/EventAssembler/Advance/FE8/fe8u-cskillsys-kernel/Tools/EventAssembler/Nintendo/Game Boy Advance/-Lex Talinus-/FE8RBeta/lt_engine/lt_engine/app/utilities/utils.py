from __future__ import annotations

import colorsys
import hashlib
import math
import sys
from collections import Counter
from operator import add, sub
from typing import Tuple


def frames_to_ms(num_frames: float) -> int:
    """at 60 fps, each frame would happen in 16.67 ms"""
    return int(16.67 * num_frames)
frames2ms = frames_to_ms  # Alternate name

class Multiset(Counter):
    def __contains__(self, item):
        return self[item] > 0

def clamp(i, bound_a, bound_b):
    max_ = max(bound_a, bound_b)
    min_ = min(bound_a, bound_b)
    return min(max_, max(min_, i))

def sign(n):
    if n > 0:
        return 1
    elif n == 0:
        return 0
    else:
        return -1

def distance(pos1, pos2) -> float:
    """
    Euclidean distance
    """
    return math.sqrt((pos1[0] - pos2[0])**2 + (pos1[1] - pos2[1])**2)

def model_wave(time, period, width) -> float:
    """
    Models a rise time of width/2 followed immediately by a fall time of width/2
    Each wave is separated by (period - width) milliseconds
    """
    cur_time = time % period
    half_width = width//2
    if cur_time < half_width:
        return float(cur_time) / half_width
    elif cur_time < width:
        return 1 - float(cur_time - half_width) / half_width
    else:
        return 0

def calculate_distance(pos1: tuple, pos2: tuple) -> int:
    """
    Taxicab/Manhattan distance
    """
    return abs(pos1[0] - pos2[0]) + abs(pos1[1] - pos2[1])

def process_terms(terms):
    """
    Processes weighted lists
    """
    weight_sum = sum(term[1] for term in terms)
    if weight_sum <= 0:
        return 0
    return sum(float(val * weight) for weight, val in terms) / weight_sum

"""Vector Tuple Math
"""
def dot_product(a: tuple, b: tuple) -> float:
    return sum(a[i] * b[i] for i in range(len(b)))

def tuple_sub(a: tuple, b: tuple) -> tuple:
    return tuple(map(sub, a, b))

def tuple_add(a: tuple, *b: tuple) -> tuple:
    accum = a
    for next_tup in b:
        accum = tuple(map(add, accum, next_tup))
    return accum

def magnitude(a: tuple) -> float:
    return math.sqrt(a[0] * a[0] + a[1] * a[1])

def normalize(a: tuple) -> tuple:
    mag = magnitude(a)
    if mag != 0:
        return (a[0] / mag, a[1] / mag)
    return a

def tmult(a: tuple, b: float) -> tuple:
    return tuple([a_i * b for a_i in a])

def tmax(a: tuple, b: tuple) -> tuple:
    return tuple(map(max, a, b))

def tclamp(a: tuple, lower: tuple, upper: tuple) -> tuple:
    return tuple(map(clamp, a, lower, upper))

def strhash(s: str) -> int:
    """
    Converts a string to a corresponding integer
    """
    h = hashlib.md5(s.encode('utf-8'))
    h = int(h.hexdigest(), base=16)
    return h

def hash_to_color(h: int) -> tuple:
    hue = h % 359
    saturation_array = lightness_array = [0.35, 0.5, 0.65]
    saturation = saturation_array[h // 360 % len(saturation_array)]
    idx = int(h * saturation // 360 % len(lightness_array))
    lightness = lightness_array[idx]
    color = colorsys.hls_to_rgb(hue / 360., lightness, saturation)
    return tuple([int(_ * 255) for _ in color])

def color_to_hex(c: tuple) -> str:
    return '#%02x%02x%02x' % (c[0], c[1], c[2])

def hex_to_color(s: str) -> tuple:
    s = s.lstrip('#').lstrip('0x')
    assert(len(s) == 6)
    r = int(s[:2], 16)
    g = int(s[2:4], 16)
    b = int(s[4:], 16)
    return (r, g, b)

def hsv2rgb(h: float, s: float, v: float) -> tuple:
    return tuple(round(i * 255) for i in colorsys.hsv_to_rgb(h, s, v))

def rgb2hsv(r: int, g: int, b: int) -> tuple:
    return tuple(colorsys.rgb_to_hsv(r, g, b))

def round_pos(pos: Tuple[float, float]) -> Tuple[int, int]:
    """
    # Convert position to integer form
    """
    return (int(round(pos[0])), int(round(pos[1])))

def diff_to_floor(d: float):
    return d - math.floor(d)

def diff_to_ceil(d: float):
    return abs(d - math.ceil(d))

def average_pos(pos_list: list, as_int=False) -> tuple:
    avg_x, avg_y = 0, 0
    for x, y in pos_list:
        avg_x += x
        avg_y += y
    avg_x /= len(pos_list)
    avg_y /= len(pos_list)
    if as_int:
        return (int(math.round(avg_x)), int(math.round(avg_y)))
    else:
        return (avg_x, avg_y)

def farthest_away_pos(pos: tuple, valid_moves: set, enemy_pos: set):
    if valid_moves and enemy_pos:
        avg_pos = average_pos(enemy_pos)
        # First furthest away point from avg_pos
        # Then closest point to current position
        return sorted(valid_moves, key=lambda move: (calculate_distance(avg_pos, move), -calculate_distance(pos, move)))[-1]
    else:
        return None

def smart_farthest_away_pos(position, valid_moves: set, enemy_pos: set):
    # Figure out avg position of enemies
    if valid_moves and enemy_pos:
        avg_x, avg_y = 0, 0
        for pos, mag in enemy_pos:
            diff_x = position[0] - pos[0]
            diff_y = position[1] - pos[1]
            diff_x /= mag
            diff_y /= mag
            avg_x += diff_x
            avg_y += diff_y
        avg_x /= len(enemy_pos)
        avg_y /= len(enemy_pos)
        # Now have vector pointing away from average enemy position
        # I want the dot product between that vector and the vector of each possible move
        # The highest dot product is the best
        return sorted(valid_moves, key=lambda move: dot_product((move[0] - position[0], move[1] - position[1]), (avg_x, avg_y)))[-1]
    else:
        return None

def raytrace(pos1: tuple, pos2: tuple) -> list:
    """
    Draws line between pos1 and pos2 for a taxicab grid
    """
    x0, y0 = pos1
    x1, y1 = pos2
    tiles = []
    dx = abs(x1 - x0)
    dy = abs(y1 - y0)
    x, y = x0, y0
    n = 1 + dx + dy
    x_inc = 1 if x1 > x0 else -1
    y_inc = 1 if y1 > y0 else -1
    error = dx - dy
    dx *= 2
    dy *= 2

    while n > 0:
        tiles.append((x, y))
        if error > 0:
            x += x_inc
            error -= dy
        else:
            y += y_inc
            error += dx
        n -= 1
    return tiles

def flatten_list(initial_list) -> list:
    final_list = []
    for item in initial_list:
        if isinstance(item, list):
            final_list += flatten_list(item)
        else:
            final_list.append(item)
    return final_list

def is_windows() -> bool:
    return sys.platform.startswith('win')

# Testing
if __name__ == '__main__':
    c = Counter()
    # import time
    # orig_time = time.time_ns()
    with open('../../../english_corpus.txt') as corpus:
        for line in corpus:
            i = strhash(line)
            b = i % 359
            c[b] += 1
    for n in range(1000000):
        i = strhash(str(n))
        b = i % 359
        c[b] += 1
    # print((time.time_ns() - orig_time)/1e6)
    print(sorted(c.items()))
