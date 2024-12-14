from __future__ import annotations
from typing import List

from app.engine.objects.overworld import OverworldEntityObject, OverworldNodeObject, RoadObject
from .overworld_manager import OverworldManager
from app.engine.overworld.overworld_movement_manager import \
    OverworldMovementManager
from app.utilities.typing import NID


class OverworldMove():
    """
    A user-issued move to a party on the overworld between two nodes.
    """
    def __init__(self, entity: OverworldEntityObject | NID, target_node: OverworldNodeObject | NID,
                 overworld: OverworldManager, event=False, follow=True, speed_adj=1,
                 remove_last=False, path=None, linger=250, after_move_callback=None, mute=False):
        if isinstance(entity, OverworldEntityObject):
            entity = entity.nid
        self.entity = overworld.entities[entity]
        self.speed_adj = speed_adj
        self.linger = linger

        # pathfinding
        if not path and self.entity.on_node:
            if isinstance(target_node, NID):
                target_node = overworld.nodes[target_node]
            old_node = overworld.nodes[self.entity.on_node]
            new_node = target_node

            roads: List[RoadObject] = overworld.shortest_path(old_node, new_node)
            if remove_last:
                roads.pop()
            self.path = [old_node.position]

            for road in roads:
                # fetch the actual coordinate paths from the roads and orient them properly
                tpath = road.prefab
                if tpath[0] == self.path[-1]:
                    self.path = self.path + tpath
                else:
                    self.path = self.path + tpath[::-1]
        elif not path and not self.entity.on_node: # path directly to node from where we're at
            self.path = [target_node.position]
            if self.entity.display_position != self.path[0]:
                self.path.insert(0, self.entity.display_position)
        else: # queue custom path
            self.path = path
            if self.entity.display_position != self.path[0]:
                self.path.insert(0, self.entity.display_position)
        # finally, since movement is queued LIFO, we reverse the path
        # so the first step is last in the list
        self.path.reverse()
        self.event = event
        self.follow = follow
        self.after_move_callback = after_move_callback
        self.mute = mute

    def queue(self, movement_manager: OverworldMovementManager):
        if self.path:
            movement_manager.begin_move(self.entity, self.path, self.event, self.follow, self.speed_adj, self.linger, self.after_move_callback, self.mute)