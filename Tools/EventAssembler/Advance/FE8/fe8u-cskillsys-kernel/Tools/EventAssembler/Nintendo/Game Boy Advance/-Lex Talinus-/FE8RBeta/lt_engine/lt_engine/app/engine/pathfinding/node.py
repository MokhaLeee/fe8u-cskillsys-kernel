class Node():
    __slots__ = ['reachable', 'cost', 'x', 'y', 'parent', 'g', 'h', 'f', 'true_f']

    def __init__(self, x: int, y: int, reachable: bool, cost: float):
        """
        Initialize new cell
        reachable - is cell reachable? is not a wall?
        cost - how many movement points to reach
        """
        self.reachable: bool = reachable
        self.cost: float = cost
        self.x: int = x
        self.y: int = y
        self.reset()

    def reset(self):
        self.parent: Node = None
        self.g: float = 0  # True distance to starting position (takes into account movement costs)
        self.h: float = 0  # Approximate distance to goal position (only Euclidean/Manhattan)
        self.f: float = 0  # Final heuristic (g + h)
        self.true_f: float = 0  # f but does not include minor adjustment for preferring a straight line path

    def __gt__(self, n) -> bool:
        return self.cost > n

    def __lt__(self, n) -> bool:
        return self.cost < n

    def __repr__(self):
        return "Node(%d, %d): cost=%d, g=%d, h=%d, f=%f, %s" % (self.x, self.y, self.cost, self.g, self.h, self.f, self.reachable)
