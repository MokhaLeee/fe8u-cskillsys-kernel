import os
import shutil

from app.data.resources.base_catalog import ManifestCatalog
from app.utilities.data import HasNid

class SongPrefab(HasNid):
    def __init__(self, nid, full_path=None):
        self.nid = nid
        self.full_path = full_path
        # self.length = None

        # Mutually exclusive. Can't have both start and battle versions
        self.intro_full_path = None
        self.battle_full_path = None

    def set_full_path(self, full_path):
        self.full_path = full_path

    def set_intro_full_path(self, full_path):
        self.intro_full_path = full_path

    def set_battle_full_path(self, full_path):
        self.battle_full_path = full_path

    def full_save(self):
        return (self.nid, self.intro_full_path, self.battle_full_path)

    def save(self):
        return (self.nid, True if self.intro_full_path else False, True if self.battle_full_path else False)

    @classmethod
    def restore(cls, s_tuple):
        self = cls(s_tuple[0])
        self.intro_full_path = s_tuple[1]
        self.battle_full_path = s_tuple[2]
        # if len(s_tuple) > 3:
        #     self.length = s_tuple[3]
        return self

class MusicCatalog(ManifestCatalog[SongPrefab]):
    filetype = '.ogg'
    manifest = 'music.json'
    title = 'music'
    datatype = SongPrefab

    def load(self, loc):
        music_dict = self.read_manifest(os.path.join(loc, self.manifest))
        for s_dict in music_dict:
            new_song = SongPrefab.restore(s_dict)
            new_song.set_full_path(os.path.join(loc, new_song.nid + '.ogg'))
            if new_song.battle_full_path:
                new_song.set_battle_full_path(os.path.join(loc, new_song.nid + '-battle.ogg'))
            if new_song.intro_full_path:
                new_song.set_intro_full_path(os.path.join(loc, new_song.nid + '-intro.ogg'))
            self.append(new_song)

    def save(self, loc):
        for song in self:
            # Full Path
            new_full_path = os.path.join(loc, song.nid + '.ogg')
            if os.path.abspath(song.full_path) != os.path.abspath(new_full_path):
                shutil.copy(song.full_path, new_full_path)
                song.set_full_path(new_full_path)
            # Battle Full Path
            new_full_path = os.path.join(loc, song.nid + '-battle.ogg')
            if song.battle_full_path and os.path.abspath(song.battle_full_path) != os.path.abspath(new_full_path):
                shutil.copy(song.battle_full_path, new_full_path)
                song.set_battle_full_path(new_full_path)
            # Intro Full Path
            new_full_path = os.path.join(loc, song.nid + '-intro.ogg')
            if song.intro_full_path and os.path.abspath(song.intro_full_path) != os.path.abspath(new_full_path):
                shutil.copy(song.intro_full_path, new_full_path)
                song.set_intro_full_path(new_full_path)
        self.dump(loc)

    def valid_files(self) -> set:
        valid_filenames = {song.nid + '.ogg' for song in self}
        valid_filenames |= {song.nid + '-battle.ogg' for song in self}
        valid_filenames |= {song.nid + '-intro.ogg' for song in self}
        return valid_filenames

class SFXPrefab(HasNid):
    def __init__(self, nid, full_path=None, tag=None):
        self.nid = nid
        self.tag = tag
        # self.length = None
        self.full_path = full_path

    def set_full_path(self, full_path):
        self.full_path = full_path

    def save(self):
        return (self.nid, self.tag)

    @classmethod
    def restore(cls, s_tuple):
        self = cls(s_tuple[0], tag=s_tuple[1])
        # if len(s_tuple) > 2:
        #     self.length = s_tuple[2]
        return self

class SFXCatalog(ManifestCatalog[SFXPrefab]):
    manifest = 'sfx.json'
    title = 'sfx'
    filetype = '.ogg'
    datatype = SFXPrefab
