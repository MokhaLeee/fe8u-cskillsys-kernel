from app.map_maker.utilities import random_choice

class EmptyMatrixError(Exception):
    pass

class Cell:
    __slots__ = ['up', 'down', 'left', 'right', 'header', 'indexes']

    def __init__(self):
        self.up = self.down = self.left = self.right = self
        self.header = None
        self.indexes = None

    def __str__(self):
        return f"Node: {self.indexes}"

    def __repr__(self):
        return f"Cell[{self.indexes}]"

class HeaderCell(Cell):
    __slots__ = ['name', 'size', 'primary', 'is_first']

    def __init__(self, name, primary=True):
        super().__init__()
        self.name = name
        self.size: int = 0
        self.primary: bool = primary
        self.is_first: bool = False

class RainAlgorithmX:
    def __init__(self, column_headers: list, row_headers: list, rows: list, 
                 pos: tuple = (0, 0), seed: int = 0):
        # Only used for random choices
        self.pos = pos
        self.seed = seed

        self.solution_rows = [] # List of row cell
        self.bad_solutions = {} # Key: Depth int, Value set of row cells

        self.header = HeaderCell("<H>")
        self.header.is_first = True
        self.num_rows = self.num_columns = 0
        self.column_list = []

        # Add columns
        previous = self.header
        # Link every column together
        for name, primary in column_headers:
            cell = HeaderCell(name, primary)
            cell.indexes = (-1, self.num_columns)
            self.column_list.append(cell)
            if primary:
                previous.right = cell
                cell.left = previous
                previous = cell
            self.num_columns += 1

        previous.right = self.header
        self.header.left = previous

        # Add rows
        self.row_headers = row_headers
        for row in rows:
            previous = None
            start = None
            cell = None
            for idx in row:
                cell = Cell()
                cell.indexes = (self.num_rows, idx)

                if previous:
                    previous.right = cell
                    cell.left = previous
                else:
                    start = cell

                column_header_cell = self.column_list[idx]
                # Link the cell with the previous one and with the correct column cell
                last = column_header_cell.up
                last.down = cell
                cell.up = last
                column_header_cell.up = cell
                cell.down = column_header_cell
                cell.header = column_header_cell
                column_header_cell.size += 1
                previous = cell

            start.left = cell
            cell.right = start
            self.num_rows += 1

    def get_solution(self):
        return sorted([self.row_headers[row_cell.indexes[0]] for row_cell in self.solution_rows])

    def choose_column(self) -> HeaderCell:
        """
        Finds the column with the minimum size.
        Returns it's index
        """
        if self.header.right.is_first:
            raise EmptyMatrixError()
        col_min = self.header.right
        for col in iterate_cell(self.header, 'right'):
            if not col.is_first and col.size < col_min.size:
                col_min = col
        return col_min

    def solve(self) -> bool:
        counter = 0
        limit = int(1e6)
        while counter < limit:
            counter += 1
            if self.header.right == self.header:
                # Solution is found
                return True
            output = self.process()
            if not output:
                # No valid solutions at all
                return False
        if counter >= limit:
            print("Infinite Loop detected!")
        return True

    def subsolve(self):
        if self.header.right == self.header:
            return True
        output = self.process()
        if not output:
            return False
        return None

    def revert(self):
        # No solutions at this level
        depth = len(self.solution_rows)
        if depth in self.bad_solutions:
            self.bad_solutions[depth].clear()
        if self.solution_rows:
            bad_row = self.solution_rows.pop()
            for j_cell in iterate_cell(bad_row, 'left'):
                self.uncover(j_cell.header)
            self.uncover(bad_row.header)
            if depth - 1 not in self.bad_solutions:
                self.bad_solutions[depth - 1] = set()
            self.bad_solutions[depth - 1].add(bad_row)
            return True
        else:
            return False  # No valid solutions at all

    def process(self):
        # print("Process")
        # Choose a column
        column_header_cell = self.choose_column()
        # print("Column Index:", column_header_cell.indexes[1])

        if column_header_cell.size == 0:
            return self.revert()

        # Choose a row that has a 1 in the column
        bad_rows = self.bad_solutions.get(len(self.solution_rows), set())
        possible_rows = [row for row in iterate_cell(column_header_cell, 'down') if row not in bad_rows]
        # print("Possible Row Idxs", [r.indexes[0] for r in possible_rows])
        if not possible_rows:
            return self.revert()
        row = random_choice(possible_rows, self.pos, self.seed)
        # print("Row choice", row)

        # Include row in solution
        self.solution_rows.append(row)

        # Cover
        self.cover(column_header_cell)
        for j_cell in iterate_cell(row, 'right'):
            self.cover(j_cell.header)

        return True

    def cover(self, column_header_cell):
        """
        # Covers the column of the column_header_cell and all rows connected to that column
        """
        column_header_cell.right.left = column_header_cell.left
        column_header_cell.left.right = column_header_cell.right

        for i_cell in iterate_cell(column_header_cell, 'down'):
            for j_cell in iterate_cell(i_cell, 'right'):
                j_cell.down.up = j_cell.up
                j_cell.up.down = j_cell.down
                j_cell.header.size -= 1

    def uncover(self, column_header_cell):
        """
        # Uncovers the column of the column_header_cell and also all the row connected to that column
        """
        for i_cell in iterate_cell(column_header_cell, 'up'):
            for j_cell in iterate_cell(i_cell, 'left'):
                j_cell.header.size += 1
                j_cell.down.up = j_cell.up.down = j_cell

        column_header_cell.right.left = column_header_cell.left.right = column_header_cell

def iterate_cell(cell, direction):
    cur = getattr(cell, direction)
    while cur is not cell:
        yield cur
        cur = getattr(cur, direction)

# Testing code.
if __name__ == '__main__':
    import cProfile
    pr = cProfile.Profile()

    def test1():
        columns = [('a', True), ('b', True), ('c', True), ('d', False), ('e', True)]
        rows = [[1, 2, 4],
                [0, 1, 3],
                [0],
                [0, 1, 2, 3, 4]]
        row_names = ['row%i' % i for i in range(len(rows))]
        return columns, row_names, rows

    def test2():
        columns = [('a', True), ('b', True), ('c', True), ('d', True), ('e', True), ('f', True), ('g', True)]
        rows = [[2, 4, 5],
                [0],
                [1, 2, 5],
                [0, 3],
                [1, 6],
                [3, 4, 6]]
        row_names = ['row%i' % i for i in range(len(rows))]
        return columns, row_names, rows

    def test3():
        columns = [('a', True), ('b', True), ('c', True), ('d', True), ('e', True), ('f', True), ('g', True)]
        rows = [[0, 3, 6],
                [0, 3],
                [3, 4, 6],
                [2, 4, 5],
                [1, 2, 5, 6],
                [1, 6],
                [0, 3, 4]]
        row_names = ['row%i' % i for i in range(len(rows))]
        return columns, row_names, rows

    columns, row_names, rows = test3()
    pr.enable()
    for seed in range(10):
        print("Seed: %d" % seed)
        d = RainAlgorithmX(columns, row_names, rows, seed=seed)
        output = d.solve()
        if output:
            print(d.get_solution())
        else:
            print("No valid solution")
    pr.disable()
    pr.print_stats(sort='time')
