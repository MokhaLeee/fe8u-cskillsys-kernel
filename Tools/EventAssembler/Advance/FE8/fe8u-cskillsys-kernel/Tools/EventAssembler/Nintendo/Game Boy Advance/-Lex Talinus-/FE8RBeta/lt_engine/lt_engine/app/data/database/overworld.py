from __future__ import annotations

from ast import literal_eval as make_tuple
from typing import TYPE_CHECKING, Dict, List, Optional

from app.utilities.typing import Point

from app.data.database.overworld_node import OverworldNodeCatalog, OverworldNodePrefab
from app.utilities.data import Data, Prefab

class OverworldPrefab(Prefab):
    def __init__(self, nid: str, name: str):
        self.nid: str = nid
        self.name: str = name
        self.tilemap: Optional[str] = None               # Tilemap Nid - background
        self.music: Optional[str] = None                 # Music Nid
        self.overworld_nodes: OverworldNodeCatalog = OverworldNodeCatalog()
        self.map_paths: Dict[str, List[Point]] = {}   # dict that maps string points_to_key(start_point, end_point) to a
                                                      # list of coords that define the road between those two nodes
                                                      # (See points_to_key function below)
        self.border_tile_width: int = 0        # how many tiles thick the border should be around the overworld map.
                                               # this is to support maps with fancy borders that shouldn't be navigable or selectable,
                                               # or just to lock off the sides of the map (Sacred Stones does this)

    def save_attr(self, name, value):
        if name == 'overworld_nodes':
            value = [node.save() for node in value]
        else:
            value = super().save_attr(name, value)
        return value

    def restore_attr(self, name, value):
        if name == 'overworld_nodes':
            value = Data([OverworldNodePrefab.restore(map_node) for map_node in value])
        else:
            value = super().restore_attr(name, value)
        return value

    @classmethod
    def default(cls):
        return cls('0', 'Magvel')

    @classmethod
    def points_to_key(cls, p1: Point, p2: Point) -> str:
        """Given two points, turns them into a string key. This function is order-agnostic,
        that is, two points in either order will map to the same key.

        Args:
            p1 Tuple(int, int): point 1 (in this context, usually starting point of a road)
            p2 Tuple(int, int): point 2 (usually end point)

        Return:
            A string key corresponding to these points
        """
        p1 = (int(p1[0]), int(p1[1]))
        p2 = (int(p2[0]), int(p2[1]))
        if p1[0] < p2[0]:
            return str(p1) + '-' + str(p2)
        elif p1[0] == p2[0] and p1[1] < p2[1]:
            return str(p1) + '-' + str(p2)
        else:
            return str(p2) + '-' + str(p1)

    @classmethod
    def string_to_tuples(cls, tstring: str) -> List[Point]:
        """Given a string of format '(a, b)-(c, d)', converts them into two points:
        the counterpoint of the function above.
        Args:
            tstring (str): A string in the format '(a, b)-(c, d)'
        Return:
            A list of two points [(a,b), (c,d)]
        """
        spl = tstring.split('-')
        return [make_tuple(spl[0]), make_tuple(spl[1])]

class OverworldCatalog(Data[OverworldPrefab]):
    datatype = OverworldPrefab
