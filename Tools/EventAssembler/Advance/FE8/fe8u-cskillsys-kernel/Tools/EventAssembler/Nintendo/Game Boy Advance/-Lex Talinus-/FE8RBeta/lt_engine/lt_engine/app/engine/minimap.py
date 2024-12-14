import logging

from app.constants import WINWIDTH, WINHEIGHT, TILEX, TILEY

from app.utilities import utils
from app.sprites import SPRITES
from app.engine.sound import get_sound_thread
from app.data.database.database import DB
from app.engine import engine, image_mods
from app.engine.state import MapState
from app.engine.game_state import game

from app.engine.input_manager import get_input_manager

class Link():
    def __init__(self, pos):
        self.position = pos
        self.adjacent_links = set()
        self.chain = None
        self.orientation = None

    def __repr__(self):
        return '%s %s' % (self.position, self.orientation)

class CliffManager():
    def __init__(self, cliff_positions, size):
        self.unexplored = set([Link(pos) for pos in cliff_positions])
        unexplored_length = len(self.unexplored)
        self.chains = []
        if self.unexplored:
            self.gen_chains()

        self.width, self.height = size
        self.orientation_grid = [0 for _ in range(self.width*self.height)]

        chain_length = sum(len(chain) for chain in self.chains)
        assert chain_length == unexplored_length, "%s, %s" % (chain_length, unexplored_length)

        for chain in self.chains:
            self.place_chain(chain)

    def gen_chains(self):
        current_chain = set()
        explored = []
        explored.append(self.unexplored.pop())
        while explored:
            current_link = explored.pop()
            current_chain.add(current_link)
            current_link.chain = current_chain
            adj = self.get_adjacent(current_link)
            if adj:
                for a in adj:
                    self.make_adjacent(current_link, a)
                    explored.append(a)
                self.unexplored -= adj
            elif explored:
                continue
            else:
                self.chains.append(current_chain)
                if self.unexplored:
                    current_chain = set()
                    current_link = self.unexplored.pop()
                    explored.append(current_link)

    def make_adjacent(self, a, b):
        a.adjacent_links.add(b)
        b.adjacent_links.add(a)

    def is_adjacent(self, pos1, pos2) -> bool:
        return pos1 in ((pos2[0], pos2[1] - 1), (pos2[0] - 1, pos2[1]), (pos2[0] + 1, pos2[1]), (pos2[0], pos2[1] + 1))

    def get_adjacent(self, current_link):
        adj = set()
        pos = current_link.position
        for link in self.unexplored:
            if self.is_adjacent(link.position, pos):
                adj.add(link)
        for link in self.unexplored:
            # If you are at a diagonal and you are not adjacent to anything I am already adjacent to
            if link.position in ((pos[0] - 1, pos[1] - 1), (pos[0] - 1, pos[1] + 1), (pos[0] + 1, pos[1] - 1), (pos[0] + 1, pos[1] + 1)) and \
                    not any(self.is_adjacent(a.position, link.position) for a in adj):
                adj.add(link)
        return adj

    def place_chain(self, chain):
        if len(chain) == 1:
            c_link = next(iter(chain))
            c_link.orientation = 1
            x, y = c_link.position
            self.orientation_grid[x + y*self.width] = 1
            return
        # Look for endings links (ie, have only one adjacency)
        ending_links = [link for link in chain if len(link.adjacent_links) == 1]
        if len(ending_links) == 0: # Cycle
            ending_link = next(iter(chain))
        # Initial set-up
        else:
            ending_link = ending_links[0]
        assert len(ending_link.adjacent_links) >= 1
        adj_link = next(iter(ending_link.adjacent_links))
        if len(chain) == 2: # Only if there are no middle links
            dx, dy = self.get_difference(ending_link, adj_link)
            if dx == 0:
                ending_link.orientation = 2
            elif dy == 0:
                ending_link.orientation = 1
            elif dx == dy:
                ending_link.orientation = 3
            else:
                ending_link.orientation = 4
        # Now iterate through
        explored = set()
        explored.add((ending_link, adj_link))
        while explored:
            prev, current = explored.pop()
            other_adjs = current.adjacent_links - set([prev])
            if other_adjs:
                for adj in other_adjs:
                    self.find_orientation(prev, current, adj)
                    explored.add((current, adj))
            else:
                self.find_ending_orientation(prev, current)
        # get starting point now -- only if there were middle links
        if len(chain) != 2:
            self.find_ending_orientation(adj_link, ending_link)

        # Lastly, commit it to the orientation grid
        for link in chain:
            x, y = link.position
            self.orientation_grid[x + y*self.width] = link.orientation

    def find_orientation(self, prev, current, nxt):
        pdx, pdy = self.get_difference(prev, current)
        ndx, ndy = self.get_difference(current, nxt)
        tdx, tdy = self.get_difference(prev, nxt)
        if tdx == 0:
            current.orientation = 2
            return
        if tdy == 0:
            current.orientation = 1
            return
        slope = tdy/float(tdx)
        if slope > 0:
            current.orientation = 3
        else:
            current.orientation = 4
        return

    def find_ending_orientation(self, prev, current):
        dx, dy = self.get_difference(prev, current)
        if dy == 0:
            if prev.orientation == 1:
                current.orientation = 1
            elif prev.orientation == 2:
                current.orientation = 2
            elif prev.orientation == 3:
                current.orientation = 4
            else:
                current.orientation = 3
        elif dx == 0:
            if prev.orientation == 1:
                current.orientation = 1
            elif prev.orientation == 2:
                current.orientation = 2
            elif prev.orientation == 3:
                current.orientation = 4
            else:
                current.orientation = 3
        elif dx == dy:
            current.orientation = 3
        else:
            current.orientation = 4

    def get_difference(self, a, b):
        dx = b.position[0] - a.position[0]
        dy = b.position[1] - a.position[1]
        return dx, dy

    def get_orientation(self, pos):
        x, y = pos
        orientation = self.orientation_grid[x + y*self.width]
        if orientation == 2:
            return (9, 6)
        elif orientation == 3:
            return (11, 6)
        elif orientation == 4:
            return (10, 6)
        else:
            return (8, 6)

# Minimap
class MiniMap(object):
    # Constants
    minimap_tiles = SPRITES.get('Minimap_Tiles')
    minimap_units = SPRITES.get('Minimap_Sprites')
    minimap_cursor = SPRITES.get('Minimap_Cursor')
    cliffs = ('Cliff', 'Desert_Cliff', 'Snow_Cliff')
    complex_map = ('Wall', 'River', 'Sand', 'Sea', 'Lava')
    scale_factor = 4

    def __init__(self, tilemap, units):
        self.tilemap = tilemap
        self.width = self.tilemap.width
        self.height = self.tilemap.height
        self.colorkey = (0, 0, 0)
        self.surf = engine.create_surface((self.width*self.scale_factor, self.height*self.scale_factor))
        engine.set_colorkey(self.surf, self.colorkey, rleaccel=False)  # black is transparent
        self.pin_surf = engine.create_surface((self.width*self.scale_factor, self.height*self.scale_factor), transparent=True)

        # All the rest of this is used for occlusion generation
        self.bg = engine.copy_surface(self.surf)
        self.starting_scale = 0.25
        new_width = int(self.height*self.scale_factor*self.starting_scale)
        new_height = int(self.width*self.scale_factor*self.starting_scale)
        self.base_mask = engine.create_surface((new_width, new_height))
        engine.set_colorkey(self.base_mask, self.colorkey, rleaccel=False)
        engine.fill(self.base_mask, (255, 255, 255), None)

        # Handle cliffs
        cliff_positions = set()
        for x in range(self.width):
            for y in range(self.height):
                minimap_nid = self.get_minimap_key((x, y))
                if minimap_nid in self.cliffs:
                    cliff_positions.add((x, y))
        self.cliff_manager = CliffManager(cliff_positions, (self.width, self.height))

        # Build Terrain
        for x in range(self.width):
            for y in range(self.height):
                minimap_nid = self.get_minimap_key((x, y))
                sprite = self.handle_key(minimap_nid, (x, y))
                self.surf.blit(sprite, (x*self.scale_factor, y*self.scale_factor))

        # Fog of War
        if game.level_vars.get('_fog_of_war', False) or game.board.fog_region_set:
            for x in range(self.width):
                for y in range(self.height):
                    if not game.board.in_vision((x, y)):
                        mask = (x * self.scale_factor, y * self.scale_factor, self.scale_factor, self.scale_factor)
                        if game.level_vars.get('_fog_of_war_type', 0) == 2:
                            engine.fill(self.surf, (12, 12, 12), mask)
                        else:
                            engine.fill(self.surf, (128, 128, 128), mask, engine.BLEND_RGB_MULT)

            self.surf = self.surf.convert()

        # Build units
        self.build_units(units)

    def get_minimap_key(self, pos):
        terrain_nid = self.tilemap.get_terrain(pos)
        terrain = DB.terrain.get(terrain_nid)
        if terrain:
            minimap_nid = terrain.minimap
        else:
            minimap_nid = DB.minimap.single_map['Grass']
        return minimap_nid

    def handle_key(self, key, position):
        # print(key)
        # Normal keys
        if key in DB.minimap.single_map:
            return self.get_sprite(DB.minimap.single_map[key])
        # Bridge
        elif key == 'Bridge':
            if self.bridge_left_right(position):
                return self.get_sprite((0, 1))
            else:
                return self.get_sprite((8, 1))
        # Door
        elif key == 'Door':
            return self.door_type(position)
        # Wall, River, Desert, Sea, Lava
        elif key in DB.minimap.complex_map:
            return self.complex_shape(key, position)
        # Coast
        elif key == 'Coast':
            return self.coast(position)
        # Lava Coast
        elif key == 'Lava_Coast':
            return self.coast(position, sea_keys=('Lava',), coast=key, offset=(0, 2))
        # Cliff
        elif key in self.cliffs:
            pos = self.cliff_manager.get_orientation(position)
            if key == 'Desert_Cliff':
                pos = (pos[0] + 4, pos[1])
            elif key == 'Snow_Cliff':
                pos = (pos[0] - 4, pos[1] + 1)
            return self.get_sprite(pos)
        # Error!
        else:
            print("Error! Unrecognized Minimap Key %s" % key)

    def build_units(self, units):
        for unit in units:
            if unit.position and 'Tile' not in unit.tags and game.board.in_vision(unit.position):
                pos = unit.position[0] * self.scale_factor, unit.position[1] * self.scale_factor
                idx: int = DB.teams.index(unit.team)
                if idx * self.scale_factor < self.minimap_units.get_width():
                    self.pin_surf.blit(engine.subsurface(self.minimap_units, (self.scale_factor * idx, 0, self.scale_factor, self.scale_factor)), pos)
                else:
                    logging.error("No unit sprite for team %s at idx %s in minimap sprite: %s" % (unit.team, idx, 'Minimap_Sprites.png'))
                    self.pin_surf.blit(engine.subsurface(self.minimap_units, (0, 0, self.scale_factor, self.scale_factor)), pos)

    def coast(self, position, allow_recurse=True, coast_key='Coast', sea_keys=('Sea', 'Pier', 'River', 'Bridge'), offset=(0, 0)):
        # A is up, B is left, C is right, D is down
        # This code determines which minimap tiles fit assuming you only knew one side of the tile, and then intersects to find the best
        a, b, c, d, e, f, g, h = None, None, None, None, None, None, None, None
        up_pos = position[0], position[1] - 1
        if not self.tilemap.check_bounds(up_pos):
            a = {2, 3, 4, 8, 11, 12, 13}
        elif self.get_minimap_key(up_pos) in sea_keys:
            a = {0, 1, 5, 6, 7, 9, 10}
        elif self.get_minimap_key(up_pos) == coast_key:
            a = {2, 3, 5, 6, 7, 9, 10, 11, 12, 13}
        else:
            a = {2, 3, 4, 8, 11, 12, 13}
        left_pos = position[0] - 1, position[1]
        if not self.tilemap.check_bounds(left_pos):
            b = {1, 3, 4, 7, 10, 12, 13}
        elif self.get_minimap_key(left_pos) in sea_keys:
            b = {0, 2, 5, 6, 8, 9, 11}
        elif self.get_minimap_key(left_pos) == coast_key:
            b = {1, 4, 5, 6, 8, 9, 10, 11, 12, 13}
        else:
            b = {1, 3, 4, 7, 10, 12, 13}
        right_pos = position[0] + 1, position[1]
        if not self.tilemap.check_bounds(right_pos):
            c = {1, 2, 4, 6, 9, 11, 13}
        elif self.get_minimap_key(right_pos) in sea_keys:
            c = {0, 3, 5, 7, 8, 10, 12}
        elif self.get_minimap_key(right_pos) == coast_key:
            c = {1, 4, 5, 7, 8, 9, 10, 11, 12, 13}
        else:
            c = {1, 2, 4, 6, 9, 11, 13}
        down_pos = position[0], position[1] + 1
        if not self.tilemap.check_bounds(down_pos):
            d = {1, 2, 3, 5, 9, 10, 13}
        elif self.get_minimap_key(down_pos) in sea_keys:
            d = {0, 4, 6, 7, 8, 11, 12}
        elif self.get_minimap_key(down_pos) == coast_key:
            d = {2, 3, 6, 7, 8, 9, 10, 11, 12, 13}
        else:
            d = {1, 2, 3, 5, 9, 10, 13}
        topleft_pos = position[0] - 1, position[1] - 1
        if not self.tilemap.check_bounds(topleft_pos):
            e = {0, 1, 2, 3, 4, 7, 8, 10, 11, 12}
        elif self.get_minimap_key(topleft_pos) in sea_keys:
            e = {0, 1, 2, 5, 6, 7, 8, 9, 10, 11}
        elif self.get_minimap_key(topleft_pos) == coast_key:
            e = {0, 1, 2, 5, 6, 7, 8, 9, 10, 11, 12}
        else:
            e = {0, 1, 2, 3, 4, 7, 8, 9, 10, 11, 12}
        topright_pos = position[0] + 1, position[1] - 1
        if not self.tilemap.check_bounds(topright_pos):
            f = {0, 1, 2, 3, 4, 6, 8, 9, 11, 12}
        elif self.get_minimap_key(topright_pos) in sea_keys:
            f = {0, 1, 3, 5, 6, 7, 8, 9, 10, 12}
        elif self.get_minimap_key(topright_pos) == coast_key:
            f = {0, 1, 3, 5, 6, 7, 8, 9, 10, 11, 12}
        else:
            f = {0, 1, 2, 3, 4, 6, 8, 9, 10, 11, 12}
        bottomleft_pos = position[0] - 1, position[1] + 1
        if not self.tilemap.check_bounds(bottomleft_pos):
            g = {0, 1, 2, 3, 4, 5, 7, 9, 10, 12}
        elif self.get_minimap_key(bottomleft_pos) in sea_keys:
            g = {0, 2, 4, 5, 6, 7, 8, 9, 11, 12}
        elif self.get_minimap_key(bottomleft_pos) == coast_key:
            g = {0, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        else:
            g = {0, 1, 2, 3, 4, 5, 7, 9, 10, 11, 12}
        bottomright_pos = position[0] + 1, position[1] + 1
        if not self.tilemap.check_bounds(bottomright_pos):
            h = {0, 1, 2, 3, 4, 5, 6, 9, 10, 11}
        elif self.get_minimap_key(bottomright_pos) in sea_keys:
            h = {0, 3, 4, 5, 6, 7, 8, 10, 11, 12}
        elif self.get_minimap_key(bottomright_pos) == coast_key:
            h = {0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}
        else:
            h = {0, 1, 2, 3, 4, 5, 6, 9, 10, 11, 12}

        intersection = list(a & b & c & d & e & f & g & h)
        if len(intersection) == 0:
            value = 14
        elif len(intersection) == 1:
            value = intersection[0]
        elif len(intersection) >= 2:
            value = sorted(intersection)[0]
            # If we are a diagonal, we need to figure out what the left and right diagonals look like
            # if they are also diagonals ... then we can match better
            if allow_recurse and value in (9, 10, 11, 12):
                if self.tilemap.check_bounds(left_pos) and self.get_minimap_key(left_pos) == coast_key:
                    left = self.coast(left_pos, False)
                    if left == 9:
                        value = 10
                    elif left == 11:
                        value = 12
                if self.tilemap.check_bounds(right_pos) and self.get_minimap_key(right_pos) == coast_key:
                    right = self.coast(right_pos, False)
                    if right == 10:
                        value = 9
                    elif right == 12:
                        value = 11
        if not allow_recurse:
            return value
        if value == 0:
            row, column = 0, 7
        elif value == 1:
            row, column = 1, 7
        elif value == 2:
            row, column = 2, 7
        elif value == 3:
            row, column = 3, 7
        elif value == 4:
            row, column = 4, 7
        elif value == 5:
            row, column = 4, 6
        elif value == 6:
            row, column = 6, 6
        elif value == 7:
            row, column = 2, 6
        elif value == 8:
            row, column = 0, 6
        elif value == 9:
            row, column = 5, 6
        elif value == 10:
            row, column = 3, 6
        elif value == 11:
            row, column = 7, 6
        elif value == 12:
            row, column = 1, 6
        elif value == 13:
            row, column = 0, 5
        elif value == 14:
            row, column = 0, 0

        return self.get_sprite((row + offset[0], column + offset[1]))

    def bridge_left_right(self, position):
        # Keep running left along bridge until we leave bridge
        pos = position
        while self.tilemap.check_bounds(pos) and self.get_minimap_key(pos) == 'Bridge':
            pos = (pos[0] - 1, pos[1]) # Go left
        # If we hit sea or the end of the map, not left/right bridge
        if not self.tilemap.check_bounds(pos) or self.get_minimap_key(pos) in ('Sea', 'River', 'Wall', 'Lava'):
            return False
        else:
            return True

    def door_type(self, position):
        left_pos = position[0] - 1, position[1]
        right_pos = position[0] + 1, position[1]
        if self.tilemap.check_bounds(left_pos) and self.get_minimap_key(left_pos) == 'Door':
            return self.get_sprite((7, 1))
        elif self.tilemap.check_bounds(right_pos) and self.get_minimap_key(right_pos) == 'Door':
            return self.get_sprite((6, 1))
        else:
            return self.get_sprite((7, 0))

    def complex_shape(self, key, position):
        column = self.complex_map.index(key) + 2

        if key == 'Sand':
            keys = ('Sand', 'Desert', 'Desert_Cliff', 'Wall')
        elif key in ('Sea', 'River'):
            keys = ('Sea', 'Coast', 'River', 'Wall', 'Pier', 'Bridge')
        else:
            keys = (key, )

        row = 0

        left_pos = position[0] - 1, position[1]
        right_pos = position[0] + 1, position[1]
        top_pos = position[0], position[1] - 1
        bottom_pos = position[0], position[1] + 1

        if not self.tilemap.check_bounds(left_pos) or self.get_minimap_key(left_pos) in keys:
            row += 1
        if not self.tilemap.check_bounds(right_pos) or self.get_minimap_key(right_pos) in keys:
            row += 2
        if not self.tilemap.check_bounds(top_pos) or self.get_minimap_key(top_pos) in keys:
            row += 4
        if not self.tilemap.check_bounds(bottom_pos) or self.get_minimap_key(bottom_pos) in keys:
            row += 8

        return self.get_sprite((row, column))

    def get_sprite(self, pos):
        return engine.subsurface(self.minimap_tiles, (pos[0]*self.scale_factor, pos[1]*self.scale_factor, self.scale_factor, self.scale_factor))

    def draw(self, surf, camera_offset, progress=1):
        current_time = engine.get_time()%2000

        progress = utils.clamp(progress, 0, 1)
        image = engine.copy_surface(self.surf)
        units = self.pin_surf
        # Flicker pin surf
        if current_time > 1600:
            whiteness = 0.01 * (100 - abs(current_time - 1800)//2)
            units = image_mods.make_white(units, whiteness)
        image.blit(units, (0, 0))

        if progress != 1:
            image = self.occlude(engine.copy_surface(image), progress)

        # Minimap is now scrollable!
        x = camera_offset.current_x
        y = camera_offset.current_y
        viewport_width = WINWIDTH//self.scale_factor - 2  # In tiles should be 58
        viewport_height = WINHEIGHT//self.scale_factor - 2  # In tiles should be 38
        xperc = x / (self.width - TILEX) if self.width > TILEX else 0
        yperc = y / (self.height - TILEY) if self.height > TILEY else 0
        xdiff = max(self.width - viewport_width, 0)
        ydiff = max(self.height - viewport_height, 0)
        xprogress = int(xdiff * xperc * self.scale_factor)
        yprogress = int(ydiff * yperc * self.scale_factor)
        rect = (xprogress, yprogress, min(image.get_width(), WINWIDTH - 2 * self.scale_factor), min(image.get_height(), WINHEIGHT - 2 * self.scale_factor))
        image = engine.subsurface(image, rect)

        image = image_mods.make_translucent(image.convert_alpha(), .1)
        topleftpos = (max(4, WINWIDTH//2 - image.get_width()//2), max(4, WINHEIGHT//2 - image.get_height()//2))
        surf.blit(image, topleftpos)

        cursor_pos = topleftpos[0] + x*self.scale_factor - 1 - xprogress, topleftpos[1] + y*self.scale_factor - 1 - yprogress
        if progress == 1:
            minimap_cursor = self.minimap_cursor
            if current_time > 1600 or (current_time > 600 and current_time < 1000):
                if current_time > 1600:
                    whiteness = 0.01 * (100 - abs(current_time - 1800)//2)
                else:
                    whiteness = 0.01 * (100 - abs(current_time - 800)//2)
                minimap_cursor = image_mods.make_white(minimap_cursor, whiteness)
            surf.blit(minimap_cursor, cursor_pos)

    def occlude(self, surf, progress):
        # Generate Mask
        bg = engine.copy_surface(self.bg) # Copy background area for mask
        # Scale mask to correct size
        # Width is original width of starting mask (uses height because of 90 deg rot)
        width = int(self.height*self.scale_factor*self.starting_scale)
        # W_Add is how much more width should be added to get current_width
        w_add = int(self.height*self.scale_factor*(1-self.starting_scale)*progress)
        # Same goes for height
        height = int(self.width*self.scale_factor*self.starting_scale)
        h_add = int(self.width*self.scale_factor*(1-self.starting_scale)*progress)
        # Actually scale mask
        mask = engine.transform_scale(self.base_mask, (width + w_add, height + h_add))
        # Rotate mask by -90 degrees at max
        mask = engine.transform_rotate(mask, progress*-90)
        # Place mask within center of minimap
        bg.blit(mask, (bg.get_width()//2 - mask.get_width()//2, bg.get_height()//2 - mask.get_height()//2))

        # Apply mask to surf
        engine.blit(surf, bg, bg.get_rect().topleft, bg.get_rect(), engine.BLEND_RGB_MULT)
        return surf

class MinimapState(MapState):
    name = 'minimap'
    transparent = True

    def start(self):
        get_sound_thread().play_sfx('Map In')
        self.minimap = MiniMap(game.tilemap, game.units)
        self.arrive_flag = True
        self.arrive_time = engine.get_time()
        self.exit_flag = False
        self.exit_time = 0
        self.transition_time = 200
        self.progress = 200

    def take_input(self, event):
        game.cursor.set_speed_state(get_input_manager().is_pressed('BACK'))
        game.cursor.take_input()

        if event in ('BACK', 'SELECT', 'START') and not self.arrive_flag and not self.exit_flag:
            get_sound_thread().play_sfx('Map Out')
            self.exit_flag = True
            self.exit_time = engine.get_time()

    def update(self):
        current_time = engine.get_time()
        if self.arrive_flag:
            if current_time - self.transition_time > self.arrive_time:
                self.arrive_flag = False
        elif self.exit_flag:
            if current_time - self.transition_time > self.exit_time:
                game.state.back()

        self.progress = self.transition_time
        if self.arrive_flag:
            self.progress = min(self.transition_time, current_time - self.arrive_time)
        elif self.exit_flag:
            self.progress = max(0, self.transition_time - (current_time - self.exit_time))

    def draw(self, surf):
        perc = self.progress/float(self.transition_time)
        self.minimap.draw(surf, game.camera, perc)
        return surf
