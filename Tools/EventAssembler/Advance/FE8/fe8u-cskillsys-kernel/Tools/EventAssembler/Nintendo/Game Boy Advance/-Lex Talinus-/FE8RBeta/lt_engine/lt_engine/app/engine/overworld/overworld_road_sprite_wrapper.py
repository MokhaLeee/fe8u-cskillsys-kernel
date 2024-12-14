from __future__ import annotations

import logging
from typing import List
from app.constants import TILEHEIGHT, TILEWIDTH
from app.engine import engine
from app.sprites import SPRITES
from app.utilities.direction import Direction
from app.utilities.typing import Point
from app.utilities.utils import dot_product, tmult, tuple_add, tuple_sub
from app.engine.engine import Surface

class OverworldRoadSpriteWrapper():
    """Not a real class, just factoring out some sprite-specific logic.
    If you don't have `overworld_routes.png` open as you're reading this code,
    it will make 0 sense.
    """
    SPRITE_DIMENSIONS = (8, 8)
    # the real spritesheet (see overworld_routes.png) includes sea routes
    # for a total of 120x8 pixels, but i'm not doing the dotted-line sea routes
    # since those seem like a pain, so I'm leaving them as a future todo
    SPRITESHEET_DIMENSIONS = (64, 8)

    def __init__(self):
        road_sprite = SPRITES['overworld_routes']
        self.sprite_dict = {}
        if road_sprite:
            self.get_image(road_sprite)
        self.road_sprite = road_sprite
        if self.road_sprite:
            self.subsprites: List[Surface] = []
            for x in range(0, self.SPRITESHEET_DIMENSIONS[0], self.SPRITE_DIMENSIONS[0]):
                self.subsprites.append(self.get_subimage(road_sprite, x))
            self.htop = self.subsprites[1]
            self.hbot = self.subsprites[2]
            self.vleft = self.subsprites[3]
            self.vright = self.subsprites[4]
            self.right_angle = self.subsprites[5]
            self.diag_main = self.subsprites[6]
            self.diag_corner = self.subsprites[7]

    def get_image(self, road_sprite):
        if not road_sprite.image:
            road_sprite.image = engine.image_load(road_sprite.full_path)

    def get_subimage(self, road_sprite, x):
        return engine.subsurface(road_sprite.image, (x, 0, 8, 8))

    def has_image(self) -> bool:
        return self.road_sprite is not None

    def rotate(self, sprite: Surface, angle: float) -> Surface:
        return engine.transform_rotate(sprite, angle)

    def draw(self, pos: Point, sprite: Surface, draw_engine: Surface):
        surf: Surface = draw_engine
        sprite: Surface = sprite
        surf.blit(sprite, pos)

    @classmethod
    def road_to_full_points_list(cls, road: List[Point]) -> List[Point]:
        """'unpacks' a road into constituent points (e.g. [(0, 0), (3, 3)] => [(0, 0), (1, 1), (2, 2), (3, 3)]).
        This requires that the road is a "locked" road, i.e. it only contains 45 and 90 degree angles.

        Args:
            road (List[Point]): initial road

        Returns:
            List[Point]: unpacked points
        """
        unpacked: List[Point] = []
        prev_point = road[0]
        for point in road:
            if point == prev_point:
                continue
            diff = tuple_sub(point, prev_point)
            length = max(abs(diff[0]), abs(diff[1]))
            inc = tmult(diff, 1 / length)
            inc = (int(inc[0]), int(inc[1]))
            start = prev_point
            while start != tuple(point):
                unpacked.append(start)
                start = tuple_add(start, inc)
            prev_point = point
        unpacked.append(road[-1])
        return unpacked

    def _draw_straight(self, draw_engine, tile_pos: Point, direction: Direction):
        left, top = tile_pos[0] * TILEWIDTH, tile_pos[1] * TILEHEIGHT
        off_x, off_y = TILEWIDTH // 2, TILEHEIGHT // 2
        # quadrants oriented like cartesian plane
        q1 = (left + off_x, top)
        q2 = (left, top)
        q3 = (left, top + off_y)
        q4 = (left + off_x, top + off_y)

        if direction == Direction.UP:
            self.draw(q2, self.vleft, draw_engine)
            self.draw(q1, self.vright, draw_engine)
        elif direction == Direction.DOWN:
            self.draw(q3, self.vleft, draw_engine)
            self.draw(q4, self.vright, draw_engine)
        elif direction == Direction.LEFT:
            self.draw(q2, self.htop, draw_engine)
            self.draw(q3, self.hbot, draw_engine)
        elif direction == Direction.RIGHT:
            self.draw(q1, self.htop, draw_engine)
            self.draw(q4, self.hbot, draw_engine)

    def _draw_diagonal(self, draw_engine, tile_pos: Point, direction: Direction, is_vertical_right_angle: bool = False):
        # shorthand
        left, top = tile_pos[0] * TILEWIDTH, tile_pos[1] * TILEHEIGHT
        off_x, off_y = TILEWIDTH // 2, TILEHEIGHT // 2
        # quadrants oriented like cartesian plane
        q1 = (left + off_x, top)
        q2 = (left, top)
        q3 = (left, top + off_y)
        q4 = (left + off_x, top + off_y)

        quadrant = q1
        if direction == Direction.UP_LEFT:
            quadrant = q2
            sprite = self.diag_main
        elif direction == Direction.UP_RIGHT:
            quadrant = q1
            sprite = self.rotate(self.diag_main, 90)
        elif direction == Direction.DOWN_LEFT:
            quadrant = q3
            sprite = self.rotate(self.diag_main, 90)
        elif direction == Direction.DOWN_RIGHT:
            quadrant = q4
            sprite = self.diag_main

        self.draw(quadrant, sprite, draw_engine)
        x, y = quadrant
        if is_vertical_right_angle:
            if quadrant == q1:
                self.draw((x, y - off_y), self.rotate(self.diag_corner, 270), draw_engine)
                self.draw((x - off_x, y), self.rotate(self.diag_corner, 270), draw_engine)
            elif quadrant == q2:
                self.draw((x, y - off_y), self.rotate(self.diag_corner, 0), draw_engine)
                self.draw((x + off_x, y), self.rotate(self.diag_corner, 0), draw_engine)
            elif quadrant == q3:
                self.draw((x, y + off_y), self.rotate(self.diag_corner, 90), draw_engine)
                self.draw((x + off_x, y), self.rotate(self.diag_corner, 90), draw_engine)
            elif quadrant == q4:
                self.draw((x, y + off_y), self.rotate(self.diag_corner, 180), draw_engine)
                self.draw((x - off_x, y), self.rotate(self.diag_corner, 180), draw_engine)
        else:
            if quadrant in [q1, q3]:
                self.draw((x, y - off_y), self.rotate(self.diag_corner, 90), draw_engine)
                self.draw((x, y + off_y), self.rotate(self.diag_corner, 270), draw_engine)
            else:
                self.draw((x, y - off_y), self.rotate(self.diag_corner, 0), draw_engine)
                self.draw((x, y + off_y), self.rotate(self.diag_corner, 180), draw_engine)

    def _draw_turn(self, draw_engine: Surface, tile_pos: Point, directions: List[Direction]):
        # shorthand
        left, top = tile_pos[0] * TILEWIDTH, tile_pos[1] * TILEHEIGHT
        off_x, off_y = TILEWIDTH // 2, TILEHEIGHT // 2
        # quadrants oriented like cartesian plane
        q1 = (left + off_x, top)
        q2 = (left, top)
        q3 = (left, top + off_y)
        q4 = (left + off_x, top + off_y)

        if Direction.UP in directions:
            if Direction.LEFT in directions: # up left
                self.draw(q2, self.rotate(self.right_angle, 270), draw_engine)
                self.draw(q3, self.hbot, draw_engine)
                self.draw(q1, self.vright, draw_engine)
            else: # up right
                self.draw(q1, self.rotate(self.right_angle, 180), draw_engine)
                self.draw(q2, self.vleft, draw_engine)
                self.draw(q4, self.hbot, draw_engine)
        else:
            if Direction.LEFT in directions: # down left
                self.draw(q3, self.right_angle, draw_engine)
                self.draw(q2, self.htop, draw_engine)
                self.draw(q4, self.vright, draw_engine)
            else: # down right
                self.draw(q4, self.rotate(self.right_angle, 270), draw_engine)
                self.draw(q1, self.htop, draw_engine)
                self.draw(q3, self.vleft, draw_engine)

    def draw_tile(self, draw_engine: Surface, tile_pos: Point, neighbor_points: List[Point]):
        """I apologize to anyone who has to read this code."""
        """Note: `neighbor_points` can be very far away, they are just used to establish angle"""
        if not self.has_image():
            logging.error("Road Sprite not found!")
            return
        # quadrants oriented like cartesian plane
        is_diagonal_vertical_right_angle = False

        # first, if we're at right angles, use special logic
        if len(neighbor_points) == 2:
            vec_a = tuple_sub(neighbor_points[0], tile_pos)
            vec_b = tuple_sub(neighbor_points[1], tile_pos)
            is_perpendicular = dot_product(vec_a, vec_b) == 0
            if is_perpendicular and (0 in vec_a or 0 in vec_b): # right angle, draw corner
                direcs = [Direction.parse_map_direction(v[0], v[1]) for v in [vec_a, vec_b]]
                self._draw_turn(draw_engine, tile_pos, direcs)
                return
            elif is_perpendicular and not (0 in vec_a or 0 in vec_b): # right angle but diagonal
                if Direction.which_vertical_dir(Direction.parse_map_direction(*vec_a)) != Direction.which_vertical_dir(Direction.parse_map_direction(*vec_b)):
                    is_diagonal_vertical_right_angle = True

        for point in neighbor_points:
            direc = Direction.parse_map_direction(*tuple_sub(point, tile_pos))
            if direc in [Direction.UP, Direction.DOWN, Direction.RIGHT, Direction.LEFT]:
                self._draw_straight(draw_engine, tile_pos, direc)
            else: # diagonal
                self._draw_diagonal(draw_engine, tile_pos, direc, is_diagonal_vertical_right_angle)
