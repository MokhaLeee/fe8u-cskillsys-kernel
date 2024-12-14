from typing import List

from app.constants import FRAMERATE
from app.utilities import str_utils
from app.data.resources.base_catalog import ManifestCatalog

class MapAnimation():
    def __init__(self, nid, full_path=None, pixmap=None):
        self.nid = nid
        self.full_path = full_path
        self.image = None
        self.pixmap = pixmap

        self.frame_x, self.frame_y = 1, 1
        self.num_frames = 1
        self.speed: int = 75
        self.frame_times: List[int] = [1]
        self.use_frame_time: bool = False

    def set_full_path(self, full_path):
        self.full_path = full_path

    def save(self):
        s_dict = {}
        s_dict['nid'] = self.nid
        s_dict['frame_x'] = self.frame_x
        s_dict['frame_y'] = self.frame_y
        s_dict['num_frames'] = self.num_frames
        s_dict['speed'] = self.speed
        s_dict['frame_times'] = ','.join([str(_) for _ in self.frame_times])
        s_dict['use_frame_time'] = self.use_frame_time
        return s_dict

    @classmethod
    def restore(cls, s_dict):
        self = cls(s_dict['nid'])
        self.frame_x = s_dict['frame_x']
        self.frame_y = s_dict['frame_y']
        self.num_frames = s_dict['num_frames']
        if s_dict.get('frame_times') is not None: # Current method
            self.speed = s_dict['speed']
            self.frame_times = [int(_) for _ in s_dict['frame_times'].split(',')]
            self.use_frame_time = s_dict['use_frame_time']
        else:  # Backwards compatibility method
            if str_utils.is_int(s_dict['speed']):
                self.speed = int(s_dict['speed'])
                self.frame_times = [int(s_dict['speed']) // FRAMERATE for _ in range(self.num_frames)]
                self.use_frame_time = False
            else:
                self.speed = 75
                self.frame_times = [int(_) for _ in s_dict['speed'].split(',')]
                self.use_frame_time = True
        return self

class MapAnimationCatalog(ManifestCatalog[MapAnimation]):
    manifest = 'animations.json'
    title = 'map_animations'
    datatype = MapAnimation
