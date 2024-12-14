from __future__ import annotations
from enum import Enum
from typing import Tuple, Callable

class Direction(str, Enum):
    CENTER = 'center'
    NORTH = 'north'
    EAST = 'east'
    SOUTH = 'south'
    WEST = 'west'
    NORTHEAST = 'northeast'
    SOUTHEAST = 'southeast'
    SOUTHWEST = 'southwest'
    NORTHWEST = 'northwest'

    LEFT = 'west'
    UP = 'north'
    RIGHT = 'east'
    DOWN = 'south'

    UP_LEFT = 'northwest'
    UP_RIGHT = 'northeast'
    DOWN_RIGHT = 'southeast'
    DOWN_LEFT = 'southwest'

    @staticmethod
    def parse_map_direction(x: int, y: int) -> Direction:
        if x > 0:
            if y > 0:
                return Direction.DOWN_RIGHT
            elif y == 0:
                return Direction.RIGHT
            elif y < 0:
                return Direction.UP_RIGHT
        elif x == 0:
            if y > 0:
                return Direction.DOWN
            elif y == 0:
                return Direction.NONE
            elif y < 0:
                return Direction.UP
        elif x < 0:
            if y > 0:
                return Direction.DOWN_LEFT
            elif y == 0:
                return Direction.LEFT
            elif y < 0:
                return Direction.UP_LEFT

    @staticmethod
    def which_vertical_dir(direction: Direction) -> int:
        if (direction == Direction.UP or
            direction == Direction.UP_LEFT or
                direction == Direction.UP_RIGHT):
            return 1
        elif (direction == Direction.DOWN or
              direction == Direction.DOWN_LEFT or
                direction == Direction.DOWN_RIGHT):
            return -1
        else:
            return 0

    @staticmethod
    def which_horizontal_dir(direction: Direction) -> int:
        if (direction == Direction.UP_LEFT or
            direction == Direction.DOWN_LEFT or
                direction == Direction.LEFT):
            return -1
        elif (direction == Direction.RIGHT or
              direction == Direction.UP_RIGHT or
                direction == Direction.DOWN_RIGHT):
            return 1
        else:
            return 0

    @staticmethod
    def next(direction, pos: Tuple[int, int]) -> Tuple[int, int]:
        if direction == Direction.NORTH:
            pos = pos[0], pos[1] - 1
        elif direction == Direction.SOUTH:
            pos = pos[0], pos[1] + 1
        elif direction == Direction.EAST:
            pos = pos[0] + 1, pos[1]
        elif direction == Direction.WEST:
            pos = pos[0] - 1, pos[1]
        return pos

    @staticmethod
    def opposite(direction: Direction) -> Direction:
        if direction == Direction.NORTH:
            return Direction.SOUTH
        elif direction == Direction.SOUTH:
            return Direction.NORTH
        elif direction == Direction.EAST:
            return Direction.WEST
        elif direction == Direction.WEST:
            return Direction.EAST
        elif direction == Direction.NORTHEAST:
            return Direction.SOUTHWEST
        elif direction == Direction.SOUTHEAST:
            return Direction.NORTHWEST
        elif direction == Direction.SOUTHWEST:
            return Direction.NORTHEAST
        elif direction == Direction.NORTHWEST:
            return Direction.SOUTHEAST
        return None

    @staticmethod
    def determine(old: Tuple[int, int], new: Tuple[int, int]) -> Direction:
        oldx, oldy = old
        newx, newy = new
        if newx > oldx:
            if newy > oldy:
                return Direction.SOUTHEAST
            elif newy < oldy:
                return Direction.NORTHEAST
            else:
                return Direction.EAST
        elif newx < oldx:
            if newy > oldy:
                return Direction.SOUTHWEST
            elif newy < oldy:
                return Direction.NORTHWEST
            else:
                return Direction.WEST
        else:
            if newy > oldy:
                return Direction.SOUTH
            elif newy < oldy:
                return Direction.NORTH
            else:
                return Direction.CENTER

    @staticmethod
    def sorting_key(direction, size: Tuple[int, int]) -> Callable[Tuple[int, int]]:
        width, height = size
        if direction == Direction.CENTER:
            return lambda x: abs(x[0] - width/2.) + abs(x[1] - height/2.)
        elif direction == Direction.NORTH:
            return lambda x: abs(x[0] - width/2.) + x[1]
        elif direction == Direction.SOUTH:
            return lambda x: abs(x[0] - width/2.) + -x[1]
        elif direction == Direction.EAST:
            return lambda x: -x[0] + abs(x[1] - height/2.)
        elif direction == Direction.WEST:
            return lambda x: x[0] + abs(x[1] - height/2.)
        elif direction == Direction.SOUTHWEST:
            return lambda x: x[0] + -x[1]
        elif direction == Direction.NORTHEAST:
            return lambda x: -x[0] + x[1]
        elif direction == Direction.NORTHWEST:
            return lambda x: x[0] + x[1]
        elif direction == Direction.SOUTHEAST:
            return lambda x: -x[0] + -x[1]
        else:
            raise ValueError("%s is not a valid Direction" % direction)

def get_cardinal_positions(pos: tuple) -> tuple:
    return (
        (pos[0], pos[1] - 1), # North
        (pos[0] + 1, pos[1]), # East
        (pos[0], pos[1] + 1), # South
        (pos[0] - 1, pos[1]), # West
    )

def get_diagonal_positions(pos: tuple) -> tuple:
    return (
        (pos[0] + 1, pos[1] - 1),  # Northeast
        (pos[0] + 1, pos[1] + 1),  # Southeast
        (pos[0] - 1, pos[1] + 1),  # Southwest
        (pos[0] - 1, pos[1] - 1),  # Northwest
    )
