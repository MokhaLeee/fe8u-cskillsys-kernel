from app.constants import TILEWIDTH, TILEHEIGHT
from app.data.database.database import DB
from app.events.regions import RegionType

from app.engine.sprites import SPRITES
from app.engine import engine, line_of_sight
from app.engine.game_state import game

import logging

class HighlightController():
    starting_cutoff = 7

    def __init__(self):
        self.images = {'spell': SPRITES.get('highlight_green'),
                       'attack': SPRITES.get('highlight_red'),
                       'splash': SPRITES.get('highlight_lightred'),
                       'possible_move': SPRITES.get('highlight_lightblue'),
                       'move': SPRITES.get('highlight_blue'),
                       'aura': SPRITES.get('highlight_lightpurple'),
                       'spell_splash': SPRITES.get('highlight_lightgreen')}

        self.highlights = {k: set() for k in self.images}
        self.transitions = {k: self.starting_cutoff for k in self.images}

        self.last_update = 0
        self.update_idx = 0

        self.current_hover = None

        self.formation_highlights = []
        self.escape_highlights = []

    def check_in_move(self, position):
        return position in self.highlights['move']

    def add_highlight(self, position, name, allow_overlap=False):
        if not allow_overlap:
            for k in self.images:
                self.highlights[k].discard(position)
        self.highlights[name].add(position)
        self.transitions[name] = self.starting_cutoff

    def add_highlights(self, positions: set, name: str, allow_overlap: bool = False):
        if not allow_overlap:
            for k in self.images:
                self.highlights[k] -= positions
        self.highlights[name] |= positions
        self.transitions[name] = self.starting_cutoff

    def remove_highlights(self, name=None):
        if name:
            self.highlights[name].clear()
            self.transitions[name] = self.starting_cutoff
        else:
            for k in self.images:
                self.highlights[k].clear()
                self.transitions[k] = self.starting_cutoff
        self.current_hover = None

    def remove_aura_highlights(self):
        self.highlights['aura'].clear()

    def handle_hover(self):
        hover_unit = game.cursor.get_hover()
        if self.current_hover and hover_unit != self.current_hover:
            self.remove_highlights()
        if hover_unit and hover_unit != self.current_hover:
            self.display_highlights(hover_unit, light=True)
            self.display_aura_highlights(hover_unit)
        self.current_hover = hover_unit

    def display_moves(self, valid_moves, light=False):
        name = 'possible_move' if light else 'move'
        self.add_highlights(valid_moves, name)

    def display_possible_attacks(self, valid_attacks, light=False):
        name = 'splash' if light else 'attack'
        self.add_highlights(valid_attacks, name)

    def display_possible_spell_attacks(self, valid_attacks, light=False):
        name = 'spell_splash' if light else 'spell'
        self.add_highlights(valid_attacks, name)

    def display_highlights(self, unit, light=False):
        valid_moves = game.path_system.get_valid_moves(unit)

        if DB.constants.value('zero_move') and unit.get_ai() and not game.ai_group_active(unit.ai_group):
            ai_prefab = DB.ai.get(unit.get_ai())
            guard = ai_prefab.guard_ai()
            if guard:
                valid_moves = {unit.position}

        valid_attacks = game.target_system.get_all_attackable_positions_spells(unit, valid_moves)
        self.display_possible_spell_attacks(valid_attacks, light=light)
        valid_attacks = game.target_system.get_all_attackable_positions_weapons(unit, valid_moves)
        self.display_possible_attacks(valid_attacks, light=light)
        self.display_moves(valid_moves, light=light)
        return valid_moves

    def display_aura_highlights(self, unit):
        for skill in unit.skills:
            if skill.aura and not skill.hide_aura:
                positions = game.board.get_aura_positions(skill.subskill)
                aura_range = skill.aura_range.value
                if DB.constants.value('aura_los'):
                    positions = line_of_sight.line_of_sight({unit.position}, positions, aura_range)
                self.add_highlights(set(positions), 'aura', allow_overlap=True)

    def show_formation(self, positions: list):
        self.formation_highlights += positions

    def hide_formation(self):
        self.formation_highlights.clear()

    def update(self):
        self.update_idx = (self.update_idx + 1) % 64

    def draw(self, surf, cull_rect):
        # Handle Formation Highlight
        formation_image = SPRITES.get('highlight_blue')
        rect = (self.update_idx//4 * TILEWIDTH, 0, TILEWIDTH, TILEHEIGHT)
        formation_image = engine.subsurface(formation_image, rect)
        for position in self.formation_highlights:
            surf.blit(formation_image, (position[0] * TILEWIDTH - cull_rect[0], position[1] * TILEHEIGHT - cull_rect[1]))

        # Handle escape Highlight
        escape_image = SPRITES.get('highlight_yellow')
        rect = (self.update_idx//4 * TILEWIDTH, 0, TILEWIDTH, TILEHEIGHT)
        escape_image = engine.subsurface(escape_image, rect)
        for region in game.level.regions:
            if (region.region_type == RegionType.EVENT and region.sub_nid in ('Escape', 'Arrive')):
                for position in region.get_all_positions():
                    surf.blit(escape_image, (position[0] * TILEWIDTH - cull_rect[0], position[1] * TILEHEIGHT - cull_rect[1]))

        # Regular highlights
        for name, highlight_set in self.highlights.items():
            if not highlight_set:
                continue
            self.transitions[name] = max(0, self.transitions[name] - 1)
            cut_off = self.transitions[name]
            rect = (self.update_idx//4 * TILEWIDTH + cut_off, cut_off, TILEWIDTH - cut_off, TILEHEIGHT - cut_off)
            image = engine.subsurface(self.images[name], rect)
            for position in highlight_set:
                surf.blit(image, (position[0] * TILEWIDTH - cull_rect[0], position[1] * TILEHEIGHT - cull_rect[1]))
        return surf
