import math

from PyQt5.QtGui import QPixmap, QPainter, qRgb

from app.constants import TILEWIDTH, TILEHEIGHT
from app.map_maker.utilities import random_choice, edge_random, flood_fill
from app.utilities import utils
from app.map_maker.wang_terrain import WangCorner2Terrain

class CliffTerrain(WangCorner2Terrain):
    terrain_like = ('Cliff')
    second_start_px = 96
    organization = {}

    @property
    def check_flood_fill(self):
        return 'diagonal'

    def _find_longest_path(self, group: set) -> list:
        # https://stackoverflow.com/questions/21880419/how-to-find-the-longest-simple-path-in-a-graph
        path = []
        best_path = []
        used = set()

        def find_path(pos):
            nonlocal path
            nonlocal best_path
            used.add(pos)
            adj = {(pos[0] - 1, pos[1] - 1), (pos[0], pos[1] - 1), (pos[0] + 1, pos[1] - 1),
                   (pos[0] - 1, pos[1]), (pos[0] + 1, pos[1]),
                   (pos[0] - 1, pos[1] + 1), (pos[0], pos[1] + 1), (pos[0] + 1, pos[1] + 1)}
            for v in adj & group:
                if v not in used:
                    path.append(v)
                    if len(path) > len(best_path):
                        best_path = path[:]
                    find_path(v)
                    path.pop()
            used.discard(pos)

        for pos in group:
            path.append(pos)
            if len(path) > len(best_path):
                best_path = path[:]
            find_path(pos)
            path.pop()

        return best_path

    def _calc_corners(self, tilemap, pos: tuple, partners: list) -> tuple:
        corner_topleft = False
        corner_bottomleft = False
        corner_topright = False
        corner_bottomright = False
        for other_pos in partners:
            # Topleft
            if other_pos[0] == pos[0] - 1 and other_pos[1] == pos[1] - 1:
                corner_topleft = True
            # Topright
            elif other_pos[0] == pos[0] + 1 and other_pos[1] == pos[1] - 1:
                corner_topright = True
            # Bottomleft
            elif other_pos[0] == pos[0] - 1 and other_pos[1] == pos[1] + 1:
                corner_bottomleft = True
            # Bottomright
            elif other_pos[0] == pos[0] + 1 and other_pos[1] == pos[1] + 1:
                corner_bottomright = True
            # Top
            elif other_pos[0] == pos[0] and other_pos[1] == pos[1] - 1:
                # if edge_random(other_pos, pos) < 0.5:
                if edge_random(other_pos, pos) < 0.5:
                    corner_topright = True
                else:
                    corner_topleft = True
            # Bottom
            elif other_pos[0] == pos[0] and other_pos[1] == pos[1] + 1:
                if edge_random(pos, other_pos) < 0.5:
                    corner_bottomright = True
                else:
                    corner_bottomleft = True
            # Left
            elif other_pos[0] == pos[0] - 1 and other_pos[1] == pos[1]:
                if edge_random(other_pos, pos) < 0.5:
                    corner_topleft = True
                else:
                    corner_bottomleft = True
            # Right
            elif other_pos[0] == pos[0] + 1 and other_pos[1] == pos[1]:
                if edge_random(pos, other_pos) < 0.5:
                    corner_topright = True
                else:
                    corner_bottomright = True
        return corner_topright, corner_bottomright, corner_bottomleft, corner_topleft

    def _chain_end_process(self, tilemap, pos: tuple, other_pos: tuple) -> tuple:
        topright, bottomright, bottomleft, topleft = \
            self._calc_corners(tilemap, pos, [other_pos])
        if topright:
            bottomleft = True
        elif bottomright:
            topleft = True
        elif topleft:
            bottomright = True
        elif bottomleft:
            topright = True

        return topright, bottomright, bottomleft, topleft

    def single_process(self, tilemap):
        positions: set = tilemap.get_all_terrain(self.nid)
        self.organization.clear()
        groupings: list = [] # of sets
        counter: int = 0
        while positions and counter < 99999:
            pos = positions.pop()
            near_positions: set = flood_fill(tilemap, pos, diagonal=True)
            groupings.append(near_positions)
            positions -= near_positions
            counter += 1
        if counter >= 99999:
            raise RuntimeError("Unexpected infinite loop in cliff flood_fill")

        while groupings:
            group = groupings.pop()
            if not (group & tilemap.terrain_grid_to_update):
                # Don't need to bother updating this one if no intersections
                continue
            longest_path: list = self._find_longest_path(group)

            # Handle the case where the longest path does not include some members of the group
            present = set(longest_path)
            new_group = group - present  # The leftovers become a new group
            if new_group:
                groupings.append(new_group)

            # now that we have longest path, we can fill in according to rules
            for idx, pos in list(enumerate(longest_path))[1:-1]:  # Skip first
                prev_pos = longest_path[idx - 1]              
                next_pos = longest_path[idx + 1]
                topright, bottomright, bottomleft, topleft = \
                    self._calc_corners(tilemap, pos, [prev_pos, next_pos])
                
                self.organization[pos] = (topright, bottomright, bottomleft, topleft)
            # For first and last path
            if len(longest_path) > 1:
                self.organization[longest_path[0]] = self._chain_end_process(tilemap, longest_path[0], longest_path[1])
                self.organization[longest_path[-1]] = self._chain_end_process(tilemap, longest_path[-1], longest_path[-2])
            else:
                self.organization[longest_path[0]] = (True, False, False, True)  # Facing down

    def _determine_index(self, tilemap, pos: tuple) -> tuple:
        corner_topright, corner_bottomright, corner_bottomleft, corner_topleft = self.organization[pos]
        index = 1 * corner_topright + \
            2 * corner_bottomright + \
            4 * corner_bottomleft + \
            8 * corner_topleft
        return index

    def set_tileset(self):
        super().set_tileset()
        self.limits = {k: self._find_limit(k) for k in range(16)}
        self.second_limits = {k: self._find_limit(k, self.second_start_px) for k in range(16)}
        print(self.second_limits)

    def _find_limit(self, idx: int, offset: int = 0) -> int:
        bg_color = qRgb(0, 0, 0)
        img = self.tileset_pixmap.toImage()
        x = idx * TILEWIDTH
        for y in range(offset, img.height(), TILEHEIGHT):
            current_color = img.pixel(x, y)
            if current_color == bg_color:
                return (y - offset) // TILEHEIGHT
        return (img.height() // TILEHEIGHT)

    def get_display_pixmap(self):
        if not self.display_pixmap:
            main_pix = QPixmap(16, 16)
            painter = QPainter()
            painter.begin(main_pix)
            painter.drawPixmap(0, 0, self.tileset_pixmap.copy(TILEWIDTH * 15, 0, TILEWIDTH, TILEHEIGHT))
            painter.end()
            self.display_pixmap = main_pix
        return self.display_pixmap

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        index = self._determine_index(tilemap, pos)
        right, bottom, x_stronger = self._determine_cliff_vector(tilemap, pos)
        use_bottomright = True
        if index in (3, 12):
            use_bottomright = right
        elif index in (5, 7, 13):
            if x_stronger:
                use_bottomright = right
            else:
                use_bottomright = bottom
        elif index in (6, 9):
            use_bottomright = bottom
        elif index in (10, 11, 14):
            if x_stronger:
                use_bottomright = not right
            else:
                use_bottomright = bottom
        if use_bottomright:
            new_coords = [(index, k) for k in range(self.limits[index])]
        else:
            new_coords = [(index, k + self.second_start_px//TILEHEIGHT) for k in range(self.second_limits[index])]

        coord = random_choice(new_coords, pos)
        pix = self.get_pixmap(coord, autotile_num)
        return pix

    def _determine_cliff_vector(self, tilemap, pos: tuple) -> tuple:
        """
        Determines chirality of the cliff position
        """
        closest_cliff_marker = list(sorted(tilemap.cliff_markers, key=lambda x: utils.distance(pos, x)))[0]
        x_diff = closest_cliff_marker[0] - (pos[0] + .5)
        y_diff = closest_cliff_marker[1] - (pos[1] + .5)
        angle = math.atan2(y_diff, x_diff)
        if angle < 0:
            angle += 2 * math.pi
        bottom = angle >= math.pi  # vector points up
        right = 0.5*math.pi <= angle <= 1.5*math.pi  # vector points left
        # Tells you whether the angle is more horizontal or vertical
        x_stronger = angle < 0.25*math.pi or angle > 1.75*math.pi or 0.75*math.pi < angle < 1.25*math.pi
        # print("vert", pos, angle, right, bottom, x_stronger)
        return right, bottom, x_stronger
