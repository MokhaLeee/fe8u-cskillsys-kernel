class McostGrid():
    default_value = 1

    def __init__(self):
        self.grid = []
        self.terrain_types = []
        self.unit_types = []

    @property
    def row_headers(self):
        return self.terrain_types

    @property
    def column_headers(self):
        return self.unit_types

    def set(self, coord, val):
        x, y = coord
        self.grid[y][x] = val

    def get(self, coord):
        x, y = coord
        return self.grid[y][x]

    def get_mcost(self, unit_type, terrain_type):
        cidx = self.unit_types.index(unit_type)
        ridx = self.terrain_types.index(terrain_type)
        return self.get((cidx, ridx))

    def width(self):
        return len(self.unit_types)

    def height(self):
        return len(self.terrain_types)

    def add_row(self, name):
        self.terrain_types.append(name)
        self.grid.append([self.default_value] * self.width())

    def add_column(self, name):
        self.unit_types.append(name)
        for row in self.grid:
            row.append(self.default_value)

    def insert_column(self, name, idx):
        self.unit_types.insert(idx, name)
        for row in self.grid:
            row.insert(idx, self.default_value)

    def insert_row(self, name, idx):
        self.terrain_types.insert(idx, name)
        self.grid.insert(idx, [self.default_value] * self.width())

    def delete_column(self, idx):
        self.unit_types.pop(idx)
        for row in self.grid:
            row.pop(idx)

    def delete_row(self, idx):
        self.terrain_types.pop(idx)
        self.grid.pop(idx)

    def get_row(self, idx):
        return self.grid[idx]

    def get_column(self, idx):
        return [row[idx] for row in self.grid]

    def set_row(self, idx, data):
        self.grid[idx] = data

    def set_column(self, idx, data):
        for row, val in enumerate(data):
            self.grid[row][idx] = val

    def get_terrain_types(self):
        return self.terrain_types

    def get_unit_types(self):
        return self.unit_types

    def save(self):
        return ([x[:] for x in self.grid], self.terrain_types[:], self.unit_types[:])

    def restore(self, data):
        self.grid = data[0]
        self.terrain_types = data[1]
        self.unit_types = data[2]
        return self
