from app.engine.sprites import SPRITES

from app.engine import engine, image_mods
from app.engine.state import State
from app.engine.game_state import game

transition_speed = 1

class TransitionInState(State):
    name = 'transition_in'
    transparent = True

    start_time = 0
    wait_time = 133
    bg = None

    def start(self):
        self.bg = SPRITES.get('bg_black').convert_alpha()
        self.start_time = engine.get_time()
        if game.memory.get('transition_speed'):
            self.transition_speed = game.memory['transition_speed']
        else:
            self.transition_speed = transition_speed
        self.wait_time = self.transition_speed * 133

    def update(self):
        if engine.get_time() >= self.start_time + self.wait_time:
            game.state.back()
            return 'repeat'

    def draw(self, surf):
        if self.bg:
            proc = (engine.get_time() - self.start_time) / self.wait_time
            bg = image_mods.make_translucent(self.bg, proc)
            engine.blit_center(surf, bg)
        return surf

    def finish(self):
        game.memory['transition_speed'] = None

class TransitionOutState(TransitionInState):
    name = 'transition_out'
    transparent = True

    def draw(self, surf):
        if self.bg:
            proc = (engine.get_time() - self.start_time) / self.wait_time
            bg = image_mods.make_translucent(self.bg, 1 - proc)
            engine.blit_center(surf, bg)
        return surf

class TransitionPopState(TransitionOutState):
    name = 'transition_pop'

    def update(self):
        if engine.get_time() >= self.start_time + self.wait_time:
            game.state.back()
            game.state.back()
            return 'repeat'

class TransitionDoublePopState(TransitionPopState):
    name = 'transition_double_pop'

    def update(self):
        if engine.get_time() >= self.start_time + self.wait_time:
            game.state.back()
            game.state.back()
            game.state.back()
            return 'repeat'

class TransitionToState(TransitionOutState):
    name = 'transition_to'
    transparent = True

    def update(self):
        if engine.get_time() >= self.start_time + self.wait_time:
            game.state.back()
            game.state.change(game.memory['next_state'])
            return 'repeat'

class TransitionToWithPopState(TransitionOutState):
    name = 'transition_to_with_pop'
    transparent = True

    def update(self):
        if engine.get_time() >= self.start_time + self.wait_time:
            game.state.back()
            game.state.back()
            game.state.change(game.memory['next_state'])
            return 'repeat'
