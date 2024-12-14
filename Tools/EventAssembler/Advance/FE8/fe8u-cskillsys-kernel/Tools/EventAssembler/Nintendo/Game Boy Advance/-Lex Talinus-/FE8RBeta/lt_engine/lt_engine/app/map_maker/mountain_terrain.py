import functools

try:
    import cPickle as pickle
except ImportError:
    import pickle

from PyQt5.QtCore import QThread
from PyQt5.QtGui import QPixmap

from app.map_maker.utilities import random_choice, flood_fill
from app.map_maker.terrain import Terrain
from app.map_maker.mountain_process import NaiveBacktrackingThread, AlgorithmXThread

class MountainTerrain(Terrain):
    terrain_like = ('Mountain',)
    organization = {}
    current_threads = []  # Keeps track of the currently running threads

    @property
    def check_flood_fill(self):
        return True

    def determine_sprite(self, tilemap, pos: tuple, autotile_num: int) -> QPixmap:
        coord = self.organization[pos]  # Mountain Group Coord
        if coord == (12, 16):
            coord = random_choice([(12, 16), (11, 16)], pos)
        pix = self.get_pixmap(coord, autotile_num)
        return pix

    def initial_process(self):
        self.quit_all_threads()
        data_loc = 'app/map_maker/mountain_data_generation/mountain_data.p'
        with open(data_loc, 'rb') as fp:
            self.mountain_data = pickle.load(fp)
        self.border_dict = {}  # Coord: Index (0-15)
        self.index_dict = {i: set() for i in range(16)}  # Index: Coord 
        self.noneless_rules = {}

        for coord, rules in self.mountain_data.items():
            north_edge = None in rules['up']
            south_edge = None in rules['down']
            east_edge = None in rules['right']
            west_edge = None in rules['left']
            index = 1 * north_edge + 2 * east_edge + 4 * south_edge + 8 * west_edge
            self.border_dict[coord] = index
            self.index_dict[index].add(coord)
            # Keep track of the rules when None is not present as well
            noneless_rules = {}
            noneless_rules['up'] = {k: v for k, v in rules['up'].items() if k is not None}
            noneless_rules['down'] = {k: v for k, v in rules['down'].items() if k is not None}
            noneless_rules['left'] = {k: v for k, v in rules['left'].items() if k is not None}
            noneless_rules['right'] = {k: v for k, v in rules['right'].items() if k is not None}
            self.noneless_rules[coord] = noneless_rules

        for index, coord in self.index_dict.items():
            print(index, sorted(coord))

    def single_process(self, tilemap):
        positions: set = tilemap.get_all_terrain(self.nid)
        self.organization.clear()
        groupings: list = [] # of sets
        counter: int = 0
        limit: int = int(1e6)
        while positions and counter < limit:
            pos = positions.pop()
            near_positions: set = flood_fill(tilemap, pos)
            groupings.append(near_positions)
            for near_pos in near_positions:
                positions.discard(near_pos)
            counter += 1
        if counter >= limit:
            raise RuntimeError("Unexpected infinite loop in mountain flood_fill")

        while groupings:
            group = groupings.pop()
            if not (group & tilemap.terrain_grid_to_update):
                # Don't need to bother updating this one if no intersections
                continue

            # Quit out of threads that are already operating on this group
            for thread in self.current_threads[:]:
                if thread.group & group:
                    self.quit_thread(thread)
            # Now we can process this group
            self.process_group(tilemap, group)

    def process_group(self, tilemap, group: set):
        # Just fill it up with generic pieces
        self.generic_fill(group)
        self.tilemap = tilemap

        # But then, start the thread
        if len(group) < 12:
            thread = NaiveBacktrackingThread(tilemap, self.mountain_data, self.noneless_rules, group)
        else:
            thread = AlgorithmXThread(tilemap, self.mountain_data, self.noneless_rules, group)
        thread.finished.connect(functools.partial(self.mountain_process_finished, thread))
        thread.waiting.connect(functools.partial(self.mountain_processing, thread))
        self.current_threads.append(thread)
        thread.start(QThread.LowPriority)

    def generic_fill(self, group):
        for pos in group:
            valid_coords = self.index_dict[15]
            valid_coord = random_choice(list(valid_coords), pos)
            self.organization[pos] = valid_coord

    def quit_thread(self, thread):
        thread.stop()
        thread.wait()
        if thread in self.current_threads:
            self.current_threads.remove(thread)

    def quit_all_threads(self):
        for thread in self.current_threads:
            thread.stop()
            thread.wait()
        self.current_threads.clear()

    def mountain_process_finished(self, thread):
        print("Finished", id(thread), thread.did_complete)
        if thread.did_complete:
            self.organization.update(thread.organization)
            # Update the image since the user may not have requested a change -- this does it manually
            for pos in thread.organization.keys():
                sprite = self.determine_sprite(self.tilemap, pos, 0)
                self.tilemap.tile_grid[pos] = sprite
        else:
            self.generic_fill(thread.group)
        if thread in self.current_threads:
            self.current_threads.remove(thread)

    def mountain_processing(self, thread):
        print("Processing... %s" % id(thread))
        for pos, coord in thread.organization.items():
            sprite = self.get_pixmap(coord, 0)
            self.tilemap.tile_grid[pos] = sprite
