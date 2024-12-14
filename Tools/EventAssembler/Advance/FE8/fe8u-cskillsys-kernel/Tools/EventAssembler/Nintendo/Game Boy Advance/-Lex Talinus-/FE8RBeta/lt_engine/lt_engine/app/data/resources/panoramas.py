import os
import shutil

from app.data.resources.base_catalog import ManifestCatalog

class Panorama():
    """
    A collection of background images
    """
    def __init__(self, nid, full_path=None, num_frames=0):
        self.nid = nid
        self.full_path = full_path  # Ignores numbers at the end
        self.num_frames = num_frames
        self.images = []
        self.pixmaps = []

        # self.idx = 0

    def set_full_path(self, full_path):
        self.full_path = full_path

    def get_all_paths(self):
        paths = []
        if self.num_frames == 1:
            paths.append(self.full_path)
        else:
            for idx in range(self.num_frames):
                path = self.full_path[:-4] + str(idx) + '.png'
                paths.append(path)
        return paths

    def save(self):
        return (self.nid, self.num_frames)

    @classmethod
    def restore(cls, s_tuple):
        self = cls(s_tuple[0], num_frames=s_tuple[1])
        return self

class PanoramaCatalog(ManifestCatalog[Panorama]):
    manifest = 'panoramas.json'
    title = 'panoramas'
    datatype = Panorama

    def save(self, loc):
        for panorama in self:
            new_full_path = os.path.join(loc, panorama.nid + '.png')
            if os.path.abspath(panorama.full_path) != os.path.abspath(new_full_path):
                if panorama.num_frames > 1:
                    paths = panorama.get_all_paths()
                    for idx, path in enumerate(paths):
                        shutil.copy(path, new_full_path[:-4] + str(idx) + '.png')
                else:
                    shutil.copy(panorama.full_path, new_full_path)
                panorama.set_full_path(new_full_path)
        self.dump(loc)

    def valid_files(self) -> set:
        valid_filenames = set()
        for panorama in self:
            if panorama.num_frames > 1:
                for idx in range(panorama.num_frames):
                    valid_filenames.add(panorama.nid + str(idx) + '.png')
            else:
                valid_filenames.add(panorama.nid + '.png')
        return valid_filenames
