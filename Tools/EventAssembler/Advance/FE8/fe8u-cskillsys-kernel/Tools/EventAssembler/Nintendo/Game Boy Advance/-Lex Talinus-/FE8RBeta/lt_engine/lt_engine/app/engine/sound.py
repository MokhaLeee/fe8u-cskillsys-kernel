from app.data.resources.sounds import SongPrefab
from app.utilities.data import HasNid
from app.utilities.typing import NID
from enum import Enum
from typing import Set, List
import pygame

from app.utilities import utils
from app.data.resources.resources import RESOURCES
from app.engine import engine

import logging

class SongObject(HasNid):
    def __init__(self, prefab: SongPrefab):
        self.nid = prefab.nid
        self.song = pygame.mixer.Sound(prefab.full_path)
        self.battle = pygame.mixer.Sound(prefab.battle_full_path) if prefab.battle_full_path else None
        self.intro = pygame.mixer.Sound(prefab.intro_full_path) if prefab.intro_full_path else None

        self.channel = None

class MusicDict(dict):
    def preload(self, nids):
        for nid in nids:
            self.get(nid)

    def get(self, val):
        if val not in self:
            logging.debug("Loading %s into MusicDict", val)
            prefab = RESOURCES.music.get(val)
            if prefab:
                try:
                    self[val] = SongObject(prefab)
                except pygame.error as e:
                    logging.warning(e)
                    return None
            else:
                return None
        return self[val]

    def clear(self, song_to_keep: NID = None):
        if not song_to_keep:
            super().clear()
        else:
            our_keys = list(self.keys())
            for key in our_keys:
                if key != song_to_keep:
                    del self[key]

class SoundDict(dict):
    def get(self, val):
        if val not in self:
            sfx = RESOURCES.sfx.get(val)
            if sfx:
                self[val] = pygame.mixer.Sound(sfx.full_path)
            else:
                return None
        return self[val]

DEFAULT_FADE_TIME_MS = 400

class Channel():
    fade_in_time = DEFAULT_FADE_TIME_MS
    fade_out_time = DEFAULT_FADE_TIME_MS
    playing_states = ("playing", "crossfade_out", "fade_in", "crossfade_in")

    def __init__(self, name, nid, end_event):
        self.name = name
        self.nid: int = nid
        self._channel = pygame.mixer.Channel(nid)
        self.local_volume = 0
        self.crossfade_volume = 1
        self.global_volume = 0

        self.end_event = end_event
        self._channel.set_endevent(end_event)

        self.current_song = None
        self.played_intro = False
        self.num_plays = -1

        self.last_state = "stopped"  # stopped, paused, playing
        self.state = "stopped"  # stopped, paused, fade_out, fade_in, playing
        self.last_update = 0

        self.last_play = 0  # Keeps track of whether we've already called _play recently
        # Because if we don't, we'll keep thinking play means we've changed songs and keep doing it
        # again and again

    def update(self, event_list, current_time):
        if self.state == "stopped":
            pass
        if self.state in self.playing_states:
            for event in event_list:
                if event.type == self._channel.get_endevent():
                    if current_time - self.last_play > 32:
                        self._play()
        if self.state == "paused":
            pass
        if self.state in ("fade_out", "crossfade_out"):
            progress = utils.clamp((current_time - self.last_update) / self.fade_out_time, 0, 1)
            # logging.debug("Fade out progress of %s: %s", self.nid, progress)
            if self.state == 'fade_out':
                self.local_volume = 1 - progress
            elif self.state == 'crossfade_out':
                self.crossfade_volume = 1 - progress
            self.reset_volume()
            # logging.debug("Fade out volume of %s: %s", self.nid, self._channel.get_volume())
            if progress >= 1:
                if self.state == 'fade_out':
                    logging.debug('%s faded out from %s', self.nid, self.last_state)
                    if self.last_state == 'playing':
                        self.state = "paused"
                        self.last_state = "paused"
                        self._channel.pause()
                    else:
                        # Could also have been told to fade out without ever starting to play
                        # In which case we don't need to do anything
                        self.state = 'stopped'
                        # This state machine is an absolute mess that
                        # needs to be considerably refactored
                        # self.last_state = 'stopped'
                    return True
                elif self.state == 'crossfade_out':
                    self.state = "playing"
                    self.last_state = "playing"
        if self.state in ("fade_in", "crossfade_in"):
            progress = utils.clamp((current_time - self.last_update) / self.fade_in_time, 0, 1)
            # logging.debug("Fade in progress of %s: %s", self.nid, progress)
            if self.state == 'fade_in':
                self.local_volume = progress
            elif self.state == 'crossfade_in':
                self.crossfade_volume = progress
            self.reset_volume()
            # logging.debug("Fade in volume of %s: %s", self.nid, self._channel.get_volume())
            if progress >= 1:
                self.state = "playing"
                self.last_state = "playing"
                return True
        return False

    def _play(self):
        logging.debug('%s _Play: %s %s', self.nid, self.last_state, self.num_plays)
        self.last_play = engine.get_time()
        if self.num_plays == 0:
            self.last_state = "stopped"
            self.state = "stopped"
            return
        if self.num_plays > 0:
            self.num_plays -= 1

        if self.name == "battle":
            if self.current_song.battle:
                self._channel.play(self.current_song.battle, 0)
                self.reset_volume()
        else:
            if self.current_song.intro and not self.played_intro:
                # logging.debug("Playing Intro %s", self.current_song.intro)
                self._channel.play(self.current_song.intro, 0)
                self.played_intro = True
            else:
                # logging.debug("Playing %s", self.current_song.song)
                self._channel.play(self.current_song.song, 0)
            self.reset_volume()

    def set_current_song(self, song, num_plays=-1):
        self.current_song = song
        self.num_plays = num_plays
        self.played_intro = False

    def set_fade_in_time(self, fade_in):
        self.fade_in_time = max(fade_in, 1)

    def set_fade_out_time(self, fade_out):
        self.fade_out_time = max(fade_out, 1)

    def clear(self):
        logging.debug("%s Clear", self.nid)
        self._channel.stop()
        self.current_song = None
        self.num_plays = 0
        self.played_intro = False
        self.last_state = "stopped"
        self.state = "stopped"

    def fade_in(self):
        logging.debug("%s Fade In: %s", self.nid, self.last_state)
        if self.last_state == "paused":
            logging.debug("%s Unpause", self.nid)
            self._channel.unpause()
        elif self.last_state == "stopped":
            self._play()
        if not self.is_playing():  # Sometimes possible with weird timings
            self._play()
        self.last_state = "playing"
        self.state = "fade_in"
        self.last_update = engine.get_time()

    def fade_out(self):
        logging.debug("%s Fade Out: %s", self.nid, self.last_state)
        # Immediately finish crossfading
        if self.state == 'crossfade_out':
            self.crossfade_volume = 0
            self.reset_volume()
            self.last_state = "playing"
        elif self.state == 'crossfade_in':
            self.crossfade_volume = 1
            self.reset_volume()
            self.last_state = "playing"
        self.state = "fade_out"
        self.last_update = engine.get_time()

    def crossfade_in(self):
        self.last_state = "playing"
        self.state = "crossfade_in"
        self.last_update = engine.get_time()

    def crossfade_out(self):
        self.last_state = "playing"
        self.state = "crossfade_out"
        self.last_update = engine.get_time()

    def pause(self):
        logging.debug("%s Pause: %s", self.nid, self.last_state)
        self._channel.pause()
        self.last_state = "paused"
        self.state = "paused"

    def resume(self):
        logging.debug("%s Resume: %s", self.nid, self.last_state)
        self._channel.unpause()
        self.last_state = "playing"
        self.state = "playing"

    def stop(self):
        logging.debug("%s Stop: %s", self.nid, self.last_state)
        self._channel.stop()
        self.played_intro = False
        self.last_state = "stopped"
        self.state = "stopped"

    def is_playing(self) -> bool:
        return self._channel.get_busy()

    def set_volume(self, volume):
        self.global_volume = volume
        self.reset_volume()

    def reset_volume(self):
        volume = utils.clamp(self.crossfade_volume * self.local_volume * self.global_volume, 0, 1)
        self._channel.set_volume(volume)

class ChannelPair():
    def __init__(self, nid):
        event = pygame.USEREVENT + nid//2  # 24, 25, 26, 27

        self.nid = nid
        self.channel = Channel("music", nid, event)
        self.battle = Channel("battle", nid + 1, event)

        self.battle_mode = False
        self.battle.crossfade_volume = 0
        self.battle.reset_volume()

        self.current_song = None

    def is_playing(self):
        return (self.channel.state in self.channel.playing_states) or \
            (self.battle.state in self.battle.playing_states)

    def is_fading_out(self):
        return self.channel.state == 'fade_out'

    def update(self, event_list, current_time):
        res1 = self.channel.update(event_list, current_time)
        res2 = self.battle.update(event_list, current_time)
        return res1 or res2

    def set_current_song(self, song, num_plays=-1):
        song.channel = self
        self.current_song = song
        self.channel.set_current_song(song, num_plays)
        self.battle.set_current_song(song, num_plays)

    def crossfade(self):
        logging.debug("%s Crossfade", self.nid)
        if self.battle_mode:
            self.battle_mode = False
            self.channel.crossfade_in()
            self.battle.crossfade_out()
        else:
            self.battle_mode = True
            self.channel.crossfade_out()
            self.battle.crossfade_in()

    def set_fade_in_time(self, fade_in):
        logging.debug("Fade in time set to %s", fade_in)
        self.channel.set_fade_in_time(fade_in)
        self.battle.set_fade_in_time(fade_in)

    def set_fade_out_time(self, fade_out):
        logging.debug("Fade out time set to %s", fade_out)
        self.channel.set_fade_out_time(fade_out)
        self.battle.set_fade_out_time(fade_out)

    def clear(self):
        if self.current_song:
            self.current_song.channel = None
        self.current_song = None
        self.channel.clear()
        self.battle.clear()

    def fade_in(self):
        self.channel.fade_in()
        self.battle.fade_in()

    def fade_out(self):
        self.channel.fade_out()
        self.battle.fade_out()

    def pause(self):
        self.channel.pause()
        self.battle.pause()

    def resume(self):
        self.channel.resume()
        self.battle.resume()

    def stop(self):
        self.channel.stop()
        self.battle.stop()

    def set_volume(self, volume):
        logging.debug("%s Set Volume: %s", self.nid, volume)
        self.channel.set_volume(volume)
        self.battle.set_volume(volume)

class GlobalMusicState(Enum):
    STOPPED = 'stopped'
    PLAYING = 'playing'
    FADE_IN = 'fade_in'
    FADE_OUT_TO_PAUSE = 'fade_out_to_pause'
    FADE_OUT_TO_STOP = 'fade_out_to_stop'
    FADE_OUT_TO_PLAY = 'fade_out_to_play'
    FADE_OUT_TO_FADE_IN = 'fade_out_to_fade_in'
    PAUSED = 'paused'

class SoundController():
    fade_out_states = (
        GlobalMusicState.FADE_OUT_TO_PLAY,
        GlobalMusicState.FADE_OUT_TO_STOP,
        GlobalMusicState.FADE_OUT_TO_PAUSE,
        GlobalMusicState.FADE_OUT_TO_FADE_IN,
    )

    def __init__(self):
        pygame.mixer.set_num_channels(16)
        pygame.mixer.set_reserved(8)  # Reserve the first 8 channels for music
        self.global_music_volume = 1.0
        self.global_sfx_volume = 1.0

        self.channel1 = ChannelPair(0)
        self.channel2 = ChannelPair(2)  # Skip each time because battle channel
        self.channel3 = ChannelPair(4)
        self.channel4 = ChannelPair(6)

        self.channel_stack = [self.channel1, self.channel2, self.channel3, self.channel4]
        self.song_stack: List[SongObject] = []

        self._state = GlobalMusicState.STOPPED

        self.PRELOADTHREAD = None

    @property
    def state(self) -> GlobalMusicState:
        return self._state

    @state.setter
    def state(self, value: GlobalMusicState):
        logging.info("Changing State to %s" % value)
        self._state = value

    # === Volume ===
    def mute(self):
        self.current_channel.set_volume(0)

    def lower(self):
        for channel in self.channel_stack:
            channel.set_volume(0.25 * self.global_music_volume)

    def unmute(self):
        for channel in self.channel_stack:
            channel.set_volume(self.global_music_volume)

    def get_music_volume(self):
        return self.global_music_volume

    def set_music_volume(self, volume):
        self.global_music_volume = volume
        for channel in self.channel_stack:
            channel.set_volume(self.global_music_volume)

    def get_sfx_volume(self):
        return self.global_sfx_volume

    def set_sfx_volume(self, volume):
        self.global_sfx_volume = volume

    # === Music state ===
    @property
    def current_channel(self):
        return self.channel_stack[-1]

    def clear(self):
        logging.debug("Clear")
        self.stop()
        for channel in self.channel_stack:
            channel.clear()
        self.song_stack.clear()

    def fade_clear(self, fade_out=DEFAULT_FADE_TIME_MS):
        logging.debug('Fade to Clear')
        self.current_channel.set_fade_out_time(fade_out)
        self.current_channel.fade_out()
        self.song_stack.clear()
        self.state = GlobalMusicState.FADE_OUT_TO_STOP

    def fade_to_stop(self, fade_out=DEFAULT_FADE_TIME_MS):
        logging.debug('Fade to Stop')
        self.current_channel.set_fade_out_time(fade_out)
        self.current_channel.fade_out()
        self.state = GlobalMusicState.FADE_OUT_TO_STOP

    def fade_to_pause(self, fade_out=DEFAULT_FADE_TIME_MS):
        logging.debug('Fade to Pause')
        self.current_channel.set_fade_out_time(fade_out)
        self.current_channel.fade_out()
        self.state = GlobalMusicState.FADE_OUT_TO_PAUSE

    def pause(self):
        logging.debug('Pause')
        self.current_channel.pause()
        self.state = GlobalMusicState.PAUSED

    def resume(self):
        self.current_channel.resume()
        self.state = GlobalMusicState.PLAYING

    def is_playing(self) -> bool:
        return self.current_channel.is_playing()

    def _set_next_song(self, song, num_plays, fade_in=DEFAULT_FADE_TIME_MS):
        # Clear the oldest channel and use it
        # to play the next song
        logging.info("Set Next Song: %s" % song)
        oldest_channel = self.channel_stack[0]
        oldest_channel.clear()
        self.channel_stack.remove(oldest_channel)
        self.channel_stack.append(oldest_channel)
        oldest_channel.set_fade_in_time(fade_in)
        oldest_channel.set_current_song(song, num_plays)

    def battle_fade_in(self, next_song, fade=DEFAULT_FADE_TIME_MS, from_start=True):
        song = MUSIC.get(next_song)
        if not song:
            logging.warning("Song does not exist")
            return None
        if song.battle:
            self.crossfade(fade)
            return song
        else:
            return self.fade_in(next_song, fade_in=fade, from_start=from_start)

    def battle_fade_back(self, song, from_start=True):
        if song.battle:
            self.crossfade()
        elif from_start:
            self.fade_back()

    def crossfade(self, fade=DEFAULT_FADE_TIME_MS):
        self.current_channel.set_fade_in_time(fade)
        self.current_channel.set_fade_out_time(fade)
        self.current_channel.crossfade()
        return True

    def get_current_song(self):
        is_playing = self.is_playing()
        if is_playing and self.song_stack:
            return self.song_stack[-1]
        return None

    def fade_in(self, next_song: NID, num_plays=-1, fade_in=DEFAULT_FADE_TIME_MS, from_start=False):
        logging.info("Fade in %s" % next_song)
        next_song = MUSIC.get(next_song)
        if not next_song:
            logging.warning("Song does not exist")
            return None

        any_music_is_playing = self.is_playing()
        current_song = self.get_current_song()

        # Confirm that we're not just replacing the same song
        if current_song is next_song:
            logging.info("Song already present")
            return None

        # Determine what state we should be going to next
        logging.debug("Currently playing on channel %s? %s", self.current_channel.nid, any_music_is_playing)
        if any_music_is_playing:
            self.current_channel.set_fade_out_time(fade_in)
            self.current_channel.fade_out()
            self.state = GlobalMusicState.FADE_OUT_TO_FADE_IN
        elif self.state in self.fade_out_states:
            any_music_is_playing = True  # So we don't fade in immediately
            self.state = GlobalMusicState.FADE_OUT_TO_FADE_IN
        else:
            self.state = GlobalMusicState.FADE_IN

        # Determine if song is already in stack
        for song in self.song_stack:
            # If so, move to top of stack
            if song is next_song:
                logging.info("Pull up %s" % next_song)
                self.song_stack.remove(song)
                self.song_stack.append(song)
                # If we can use our old channel
                if song.channel and song.channel.current_song == song:
                    # Move to top
                    logging.info("Using original channel")
                    if from_start:
                        logging.info("Rewinding song to beginning")
                        song.channel.stop()  # Stop it now, so when it fades in, it will start from beginning
                    self.channel_stack.remove(song.channel)
                    self.channel_stack.append(song.channel)
                    song.channel.num_plays = num_plays
                    song.channel.set_fade_in_time(fade_in)
                    logging.debug("Any Music is Playing? %s", any_music_is_playing)
                    if any_music_is_playing:
                        pass
                    else:
                        song.channel.fade_in()
                else:  # New channel and start song over
                    self._set_next_song(song, num_plays, fade_in)
                break
        else: # Song is not in stack
            logging.info("New song %s" % next_song)
            self.song_stack.append(next_song)
            logging.debug("Any music is playing? %s", any_music_is_playing)
            # Clear the oldest channel and use it
            # If the oldest channel is the one that is originally fading out
            # set any_music_is_playing to false
            oldest_channel = self.channel_stack[0]
            if oldest_channel.is_fading_out():
                logging.debug("Oldest Channel %s is fading out" % oldest_channel.nid)
                any_music_is_playing = False
            self._set_next_song(next_song, num_plays, fade_in)
            if any_music_is_playing:
                pass
            else:
                next_song.channel.fade_in()

        return self.song_stack[-1]

    def fade_back(self, fade_out=DEFAULT_FADE_TIME_MS):
        logging.info("Fade back")

        if not self.song_stack:
            return
        current_channel = self.current_channel
        current_channel.set_fade_out_time(fade_out)
        current_channel.fade_out()
        last_song = self.song_stack.pop()

        # Where do we go next
        next_song = self.song_stack[-1] if self.song_stack else None
        if next_song:
            logging.info("Fade out to Fade in")
            self.state = GlobalMusicState.FADE_OUT_TO_FADE_IN
        else:
            logging.info("Fade out to Stop")
            self.state = GlobalMusicState.FADE_OUT_TO_STOP

        # Move current channel down to bottom of world
        self.channel_stack.remove(current_channel)
        self.channel_stack.insert(0, current_channel)

    def stop(self):
        self.current_channel.stop()
        self.state = GlobalMusicState.STOPPED

    def update(self, event_list):
        current_time = engine.get_time()

        any_changes = False
        for channel in self.channel_stack:
            if channel.update(event_list, current_time):
                any_changes = True

        if any_changes:
            logging.debug("Channel changed its state")
            if self.state == GlobalMusicState.FADE_OUT_TO_FADE_IN:
                logging.debug('Update Fade In')
                self.current_channel.set_volume(self.global_music_volume)
                self.current_channel.fade_in()
                self.state = GlobalMusicState.FADE_IN
            elif self.state == GlobalMusicState.FADE_OUT_TO_STOP:
                logging.debug('Update Fade to Stop')
                self.stop()
            elif self.state == GlobalMusicState.FADE_OUT_TO_PAUSE:
                logging.debug('Update Fade to Pause')
                self.pause()
            elif self.state == GlobalMusicState.FADE_IN:
                self.state = GlobalMusicState.PLAYING

        if self.state == GlobalMusicState.PLAYING and not self.is_playing():
            logging.warning("In PLAYING state but not playing music")
            self.current_channel.set_fade_in_time(1)
            self.current_channel.fade_in()

    # === Other Miscellaneous Funcs ===
    def play_sfx(self, sound, loop=False, volume=1):
        sfx = SFX.get(sound)
        if sfx:
            vol = utils.clamp(self.global_sfx_volume * volume, 0, 1)
            sfx.set_volume(vol)
            if loop:
                sfx.play(-1)
            else:
                sfx.play()
            return sfx
        return None

    def stop_sfx(self, sound):
        sfx = SFX.get(sound)
        if sfx:
            sfx.stop()
            return sfx
        return None

    def load_songs(self, nids: Set[NID]):
        MUSIC.preload(nids)

    def flush(self, should_interrupt_current_song=True):
        """Simply flushes the song cache from memory - this prevents memory bloat.

        Args:
            should_interrupt_current_song (bool, optional): Whether or not to keep the current song playing while flushing all others.
                                                            Defaults to True.
        """
        current_song_nid = None
        if not should_interrupt_current_song:
            current_song = self.get_current_song()
            if current_song:
                print(current_song.nid)
                current_song_nid = current_song.nid
        MUSIC.clear(current_song_nid)
        SFX.clear()

    def reset(self):
        """
        Needs to reset the sounds that are stored in memory
        so if the main editor runs the engine again
        we can reload everything like new
        """
        MUSIC.clear()
        SFX.clear()
        self.__init__()

MUSIC = MusicDict()
SFX = SoundDict()

_soundthread: SoundController = None

def get_sound_thread():
    global _soundthread
    if not _soundthread:
        _soundthread = SoundController()
    return _soundthread
