from app.constants import WINWIDTH, WINHEIGHT

from app.engine.sprites import SPRITES
from app.engine.sound import get_sound_thread
from app.engine.fonts import FONT
from app.engine.state import State
from app.engine import background, image_mods, engine

from app.engine.game_state import game

class ChapterTitleState(State):
    name = 'chapter_title'

    sigil1 = SPRITES.get('chapter_title_sigil').convert_alpha()
    sigil2 = SPRITES.get('chapter_title_sigil2').convert_alpha()

    def start(self):
        """
        Displays the chapter title screen
        """
        self.state = 'begin'
        self.wait_time = engine.get_time()

        self.bg = background.TransitionBackground(SPRITES.get('chapter_title_bg'), fade=False)
        self.ribbon = SPRITES.get('chapter_title_ribbon')

        self.sigil_fade = 0
        self.banner_grow_y = 6
        self.banner_grow_x = 0
        self.banner_fade = 0

        # Fade in music
        self.music_flag = False
        if game.memory.get('chapter_title_music'):
            song = get_sound_thread().fade_in(game.memory.get('chapter_title_music'))
            if song:
                self.music_flag = True
        if not self.music_flag:
            song = get_sound_thread().fade_in('Chapter Sound')
            if song:
                self.music_flag = True

        self.title = game.memory.get('chapter_title_title')
        if not self.title:
            self.title = game.level.name

        game.state.change('transition_in')
        return 'repeat'

    def take_input(self, event):
        if event in ('START', 'SELECT', 'BACK'):
            if self.music_flag:
                get_sound_thread().stop()
            game.state.change('transition_pop')
            self.state = 'end'
            return 'repeat'

    def update(self):
        current_time = engine.get_time()

        if self.state == 'begin':
            if current_time - self.wait_time > 50 * 16:
                self.state = 'sigil'
                self.wait_time = current_time

        elif self.state == 'sigil':
            self.sigil_fade = (current_time - self.wait_time) / 1100
            if self.sigil_fade >= 1:
                self.sigil_fade = 1
                self.state = 'banner_grow_x'

        elif self.state == 'banner_grow_x':
            self.banner_grow_x += 10
            if self.banner_grow_x >= WINWIDTH:
                self.banner_grow_x = WINWIDTH
                self.state = 'banner_grow_y'

        elif self.state == 'banner_grow_y':
            self.banner_grow_y += 2
            if self.banner_grow_y >= self.ribbon.get_height():
                self.banner_grow_y = self.ribbon.get_height()
                self.state = 'ribbon_fade_in'

        elif self.state == 'ribbon_fade_in':
            self.banner_fade += 0.02
            if self.banner_fade >= 1:
                self.banner_fade = 1
                self.wait_time = current_time
                self.state = 'wait'

        elif self.state == 'wait':
            if current_time - self.wait_time > 5000:
                self.state = 'sigil_out'

        elif self.state == 'sigil_out':
            self.sigil_fade -= .02
            if self.sigil_fade <= 0:
                self.sigil_fade = 0
                self.state = 'ribbon_close'
                if self.music_flag:
                    get_sound_thread().fade_to_stop()

        elif self.state == 'ribbon_close':
            self.banner_grow_y -= 2
            if self.banner_grow_y <= 0:
                self.banner_grow_y = 0
                self.state = 'end'
                game.state.change('transition_pop')

        elif self.state == 'end':
            pass

    def draw(self, surf):
        engine.fill(surf, (0, 0, 0))
        self.bg.update()
        self.bg.draw(surf)

        # # Draw sigil
        sigil_outline = image_mods.make_translucent(self.sigil1, 1 - self.sigil_fade)
        sigil_middle = image_mods.make_translucent(self.sigil2, 1 - self.sigil_fade)
        center_x = WINWIDTH//2 - sigil_outline.get_width()//2
        center_y = WINHEIGHT//2 - sigil_outline.get_height()//2
        surf.blit(sigil_outline, (center_x + 1, center_y + 1))
        surf.blit(sigil_middle, (center_x, center_y))

        # # Draw ribbon
        if self.state in ('ribbon_fade_in', 'wait', 'ribbon_close', 'sigil_out', 'end'):
            new_ribbon = engine.copy_surface(self.ribbon)
            name_width = FONT['chapter-yellow'].width(self.title)
            position = (new_ribbon.get_width()//2 - name_width//2, self.ribbon.get_height()//2 - 6)
            FONT['chapter-yellow'].blit(self.title, new_ribbon, position)
            rect = (0, (self.ribbon.get_height() - self.banner_grow_y)//2, self.ribbon.get_width(), self.banner_grow_y)
            new_ribbon = engine.subsurface(new_ribbon, rect)
            engine.blit_center(surf, new_ribbon)

        # # Draw banner
        banner = image_mods.make_translucent(SPRITES.get('chapter_title_banner'), self.banner_fade)
        banner = engine.subsurface(banner, (0, 0, self.banner_grow_x, self.banner_grow_y))
        engine.blit_center(surf, banner)

        return surf
