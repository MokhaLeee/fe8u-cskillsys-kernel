from app.constants import WINWIDTH, WINHEIGHT
from app.data.database.database import DB

from app.engine.sound import get_sound_thread
from app.engine.sprites import SPRITES
from app.engine.fonts import FONT
from app.engine.state import MapState

from app.engine import base_surf, image_mods, text_funcs, engine
from app.engine.background import SpriteBackground
from app.engine.game_state import game

class VictoryState(MapState):
    name = 'victory'
    victory_surf = SPRITES.get('victory')
    vic_width, vic_height = victory_surf.get_size()
    num_transition_frames = 20

    def start(self):
        game.cursor.hide()
        img = SPRITES.get('focus_fade').convert_alpha()
        self.bg = SpriteBackground(img)

        self.state = 'init'
        self.stat_surf = self.create_stat_surf()

        self.stat_surf_target = self.stat_surf.get_height() + 4
        self.num_frame = 0

        get_sound_thread().lower()
        # Check for an assigned sound
        if game.memory.get('victory_screen_sound'):
            get_sound_thread().play_sfx(game.memory.get('victory_screen_sound'))
            game.memory['victory_screen_sound'] = None  # Reset for later
        else:
            get_sound_thread().play_sfx('StageClear')

    def create_stat_surf(self):
        turns = str(game.turncount)
        mvp = game.records.get_mvp(game.level.nid)
        unit = DB.units.get(mvp)
        if unit:
            mvp = unit.name
        else:
            mvp = '--'

        menu_width = 96
        bg = base_surf.create_base_surf(menu_width, 40, 'menu_bg_base')
        shimmer = SPRITES.get('menu_shimmer2')
        if shimmer:
            bg.blit(shimmer, (menu_width - 1 - shimmer.get_width(), 40 - 5 - shimmer.get_height()))
        bg = image_mods.make_translucent(bg, .1)

        turns_str = text_funcs.translate('Turns')
        mvp_str = text_funcs.translate('MVP')
        FONT['text-yellow'].blit(turns_str, bg, (4, 4))
        FONT['text-yellow'].blit(mvp_str, bg, (4, 20))
        FONT['text-blue'].blit_center(turns, bg, (menu_width - 40, 4))
        FONT['text-blue'].blit_center(mvp, bg, (menu_width - 40, 20))

        return bg

    def take_input(self, event):
        if self.state == 'allow_input' and event in ('SELECT', 'START', 'BACK'):
            self.state = 'leave'
            get_sound_thread().unmute()
            self.bg.fade_out()
            game.state.back()
            # game.state.change('transition_pop')
            return 'repeat'

    def update(self):
        super().update()
        if self.state == 'init':
            self.num_frame += 1
            if self.num_frame >= self.num_transition_frames:
                self.num_frame = self.num_transition_frames
                self.state = 'allow_input'

    def draw(self, surf):
        surf = super().draw(surf)
        offset = self.num_frame / self.num_transition_frames

        if self.bg:
            self.bg.draw(surf)

        pos = WINWIDTH//2 - self.stat_surf.get_width()//2, WINHEIGHT - (offset * self.stat_surf.get_height())
        surf.blit(self.stat_surf, pos)

        # Victory draw
        vic_surf = image_mods.make_translucent(self.victory_surf, 1 - offset)
        height = int(self.vic_height * offset)
        vic_surf = engine.transform_scale(vic_surf, (WINWIDTH, height))
        surf.blit(vic_surf, (0, self.vic_height//2 - height//2))

        return surf
