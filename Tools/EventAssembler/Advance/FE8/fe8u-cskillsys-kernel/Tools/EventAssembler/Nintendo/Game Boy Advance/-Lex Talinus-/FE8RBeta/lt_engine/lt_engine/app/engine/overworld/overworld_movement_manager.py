import logging
from typing import Dict, List, Optional, Tuple

import app.engine.config as cf
from app.engine import engine
from app.engine.objects.overworld import OverworldEntityObject
from .overworld_manager import OverworldManager
from app.engine.sound import get_sound_thread
from app.utilities import utils
from app.utilities.typing import NID, Point

# Overworld movement should be slower and more cinematic than level movement
OVERWORLD_MOVEMENT_SPEED_MULTIPLIER = 2

class OverworldMovementData():
    def __init__(self, path, event, follow, speed_adj=1, linger=250, callback=None, muted=False):
        self.path = path
        self.last_update = 0
        self.last_update_position = path[-1]
        self.event = event
        self.follow = follow
        self.speed_adj = speed_adj
        self.linger = linger
        self.callback = callback
        self.muted = muted

class OverworldMovementManager():
    def __init__(self, overworld: OverworldManager):
        self.overworld: OverworldManager = overworld
        self.moving_entities: Dict[NID, OverworldMovementData] = {}
        self.camera_follow: NID = None
        self.abort_movement: Dict[NID, bool] = {}

    def begin_move(self, entity: OverworldEntityObject, path: List[Point], event: bool=False, follow: bool=True, speed_adj: float = 1, linger = 250, after_move_callback=None, mute=False):
        logging.info("Overworld Entity %s begin move: %s", entity.nid, path)
        # set the entity's display position to begin with
        entity.display_position = path[-1]
        if not self.camera_follow and follow:
            self.camera_follow = entity.nid
        self.moving_entities[entity.nid] = OverworldMovementData(path, event, follow, speed_adj, linger, after_move_callback, muted=mute)

    def __len__(self):
        return len(self.moving_entities)

    def get_last_update(self, nid: NID) -> int:
        data: OverworldMovementData = self.moving_entities.get(nid)
        if data:
            return data.last_update
        else:
            return 0

    def get_next_position(self, nid: NID) -> Point:
        data = self.moving_entities.get(nid)
        if data and data.path:
            return data.path[-1]
        else:
            return None

    def get_movement_speed_multiplier(self, nid: NID) -> float:
        data = self.moving_entities.get(nid)
        if data and data.speed_adj and data.speed_adj != 1.0:
            # 120 is default movespeed
            return data.speed_adj * 120
        else:
            return cf.SETTINGS['unit_speed']

    def get_following_unit(self) -> Optional[NID]:
        """Gets the nid of the unit that the camera is meant to follow.

        Returns:
            Optional[NID]: the nid of the unit to set the camera for
        """
        return self.camera_follow

    def done_moving(self, entity_nid: NID, data: OverworldMovementData, entity: OverworldEntityObject, surprise=False):
        del self.moving_entities[entity_nid]
        if entity.sound:
            entity.sound.stop()
        entity.sprite.change_state('normal')
        if self.overworld.node_at(entity.display_position):
            entity.on_node = self.overworld.node_at(entity.display_position).nid
        else:
            entity.on_node = None
        if data.callback:
            data.callback()
        if self.camera_follow == entity_nid:
            self.camera_follow = None
        if surprise:
            get_sound_thread().play_sfx('Surprise')
            entity.sprite.change_state('normal')

    def finish_all_movement(self):
        for entity_nid, data in self.moving_entities.items():
            entity = self.overworld.entities[entity_nid]
            destination = data.path[0]
            if self.overworld.node_at(destination):
                entity.on_node = self.overworld.node_at(destination).nid
            else:
                entity.display_position = destination
            if entity.sound:
                entity.sound.stop()
            entity.sprite.change_state('normal')
        self.moving_entities = {}

    def interrupt_movement(self, nid: NID):
        """It is possible that we may want to cut specific entities' queued movements short.
        This function will mark a flag that will stop the specified entity at the next node.

        Args:
            nid (NID): NID of the entity.
        """
        self.abort_movement[nid] = True

    def _time_to_walk_segment(self, unit_speed: int, segment: Tuple[Point, Point]) -> int:
        """Given a line segment in the overworld, generates an appropriate time to traverse
        the segment based on the given unit speed

        Args:
            unit_speed (int): how long the unit takes to travel (millisecond quantity).
                The larger this quantity, the slower.
            segment (Tuple[Point, Point]): the line segment to travel.

        Returns:
            int: the number of milliseconds it takes to traverse.
        """
        dist = utils.magnitude(utils.tuple_sub(segment[1], segment[0]))
        return int(dist * unit_speed)

    def update(self):
        current_time = engine.get_time()
        for entity_nid in list(self.moving_entities.keys()):
            data = self.moving_entities[entity_nid]
            entity = self.overworld.entities[entity_nid]
            if not entity:
                logging.error("Could not find entity with nid %s", entity_nid)
                del self.moving_entities[entity_nid]
                continue
            if entity.sprite.state != 'moving':
                entity.sprite.change_state('moving')
                if not data.muted:
                    entity.sound.play()
            starting_position = data.last_update_position
            ending_position = self.get_next_position(entity_nid)
            segment_being_traversed = (starting_position, ending_position)

            # calculate the progress towards the next point
            if starting_position == ending_position:
                percentage_progress = 1.0
            else:
                progress = current_time - data.last_update

                # set the position of the unit based on progress
                unit_speed = OVERWORLD_MOVEMENT_SPEED_MULTIPLIER * self.get_movement_speed_multiplier(entity_nid)
                percentage_progress = utils.clamp(progress / self._time_to_walk_segment(unit_speed, segment_being_traversed), 0, 1)

            if percentage_progress <= 0: # brief pause, don't play sound
                entity.sound.stop()
            else:
                if not data.muted:
                    entity.sound.play()

            if percentage_progress < 1: # we're still in the middle of walking a segment
                # update its display position
                direction = utils.tuple_sub(ending_position, starting_position)
                entity.display_position = utils.tuple_add(utils.tmult(direction, percentage_progress), starting_position)
                # update its direction
                entity.sprite.update_sprite_direction(direction)
            else:
                # we've finished walking a segment of the path
                data.last_update = current_time + data.linger
                new_position = data.path.pop()
                data.last_update_position = new_position
                entity.display_position = new_position

                if entity_nid in self.abort_movement and self.abort_movement[entity_nid]:
                    # we're trying to interrupt our movement. try to stop as soon as we reach a node.
                    if not self.overworld.node_at(entity.display_position) == None:
                        self.abort_movement[entity_nid] = False
                        self.done_moving(entity_nid, data, entity)
                elif data.path: # we're still moving
                    pass        # keep up the good work
                else: # Path is empty, so we are done
                    self.done_moving(entity_nid, data, entity)
