from __future__ import annotations
from enum import Enum
from typing import Any, List
from app.engine.exp_calculator import ExpCalcType
from app.data.database.difficulty_modes import GrowthOption
from app.utilities.data import Data
from app.utilities.typing import NID

class ConstantType(Enum):
    BOOL = 1
    INT = 2
    FLOAT = 3
    STR = 4
    MUSIC = 5
    POSITIVE_INT = 6

class ConstantTag(str, Enum):
    MAJOR_FEATURES = 'major_features'
    PAIR_UP = 'pair_up'
    INVENTORY = 'inventory'
    LINE_OF_SIGHT = 'line_of_sight'
    LEVELING = 'leveling'
    AI = 'ai'
    WEXP = 'wexp'
    MUSIC = 'music'
    TITLE = 'title'
    AESTHETIC = 'aesthetic'
    EXP = 'exp'
    OTHER = 'other'
    HIDDEN = 'hidden'

class Constant(object):
    def __init__(self, nid: NID, name: str = '', attr: ConstantType | List[str] = ConstantType.BOOL, 
                 default_value: Any = False, tag: ConstantTag = ConstantTag.OTHER):
        self.nid: NID = nid
        self.name: str = name
        self.attr: ConstantType | List[str] = attr
        self.value: Any = default_value
        self.tag: ConstantTag = tag

    def set_value(self, val):
        self.value = val

    def save(self):
        return (self.nid, self.value)

class ConstantCatalog(Data[Constant]):
    def save(self):
        return [elem.save() for elem in self._list]

    def restore(self, ser_data):
        # Assign each constant with the correct value
        for nid, value in ser_data:
            constant = self.get(nid)
            if constant:
                constant.value = value

    def value(self, const_nid):
        constant = self.get(const_nid)
        if constant:
            return constant.value
        raise ValueError("No such Constant %s" % const_nid)

    def total_items(self):
        return self.value('num_items') + self.value('num_accessories')

constants = ConstantCatalog([
    Constant('num_items', "Max number of Items in inventory", ConstantType.INT, 5, ConstantTag.INVENTORY),
    Constant('num_accessories', "Max number of Accessories in inventory", ConstantType.INT, 0, ConstantTag.INVENTORY),
    Constant('turnwheel', "Turnwheel", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('initiative', "Per Unit Initiative Order", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('fatigue', "Fatigue", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('reset_fatigue', "Automatically reset fatigue to 0 for benched units", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('minimap', "Enable Minimap", ConstantType.BOOL, True, ConstantTag.MAJOR_FEATURES),
    Constant('pairup', "Pair Up", ConstantType.BOOL, False, ConstantTag.PAIR_UP),
    Constant('limit_attack_stance', "Limit Attack Stance to first attack only", ConstantType.BOOL, False, ConstantTag.PAIR_UP),
    Constant('attack_stance_only', "Only attack stance allowed", ConstantType.BOOL, False, ConstantTag.PAIR_UP),
    Constant('player_pairup_only', "Only player units can pairup", ConstantType.BOOL, False, ConstantTag.PAIR_UP),
    Constant('lead', "Global Leadership Stars", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('bexp', "Bonus Experience", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('rd_bexp_lvl', "Always gain 3 stat-ups when using Bonus Exp.", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('support', "Supports", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('overworld', "Overworld", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('overworld_start', "Start in Overworld", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('unit_notes', "Unit Notes", ConstantType.BOOL, False, ConstantTag.MAJOR_FEATURES),
    Constant('crit', "Allow Criticals", ConstantType.BOOL, True, ConstantTag.MAJOR_FEATURES),
    Constant('reset_uses', "Reset uses on droppable items when acquired", ConstantType.BOOL, True, ConstantTag.OTHER),
    Constant('allow_negative_as', "Allow Attack Speed to be negative", ConstantType.BOOL, False, ConstantTag.OTHER),
    Constant('trade', "Can trade items on map", ConstantType.BOOL, True, ConstantTag.OTHER),
    Constant('growth_info', "Can view unit growths in Info Menu", ConstantType.BOOL, True, ConstantTag.OTHER),
    Constant('glancing_hit', "Chance (%) to score a glancing hit", ConstantType.INT, 0, ConstantTag.MAJOR_FEATURES),
    Constant('backpropagate_difficulty_growths', "Apply difficulty bonus growths to past levels", ConstantType.BOOL, True, ConstantTag.OTHER),
    Constant('traveler_time_decrement', "Timed skills applied to traveler units will decrease.", ConstantType.BOOL, False, ConstantTag.OTHER),
    Constant('line_of_sight', "Force items and abilities to obey line of sight rules", ConstantType.BOOL, False, ConstantTag.LINE_OF_SIGHT),
    Constant('aura_los', "Force auras to obey line of sight rules", ConstantType.BOOL, False, ConstantTag.LINE_OF_SIGHT),
    Constant('fog_los', "Fog of War will also be affected by line of sight", ConstantType.BOOL, False, ConstantTag.LINE_OF_SIGHT),
    Constant('ai_fog_of_war', "AI will also be affected by Fog of War", ConstantType.BOOL, False, ConstantTag.AI),
    Constant('def_double', "Defender can double counterattack", ConstantType.BOOL, True, ConstantTag.OTHER),
    Constant('unit_stats_as_bonus', "Add class stats to non-generic base stats", ConstantType.BOOL, False, ConstantTag.LEVELING),
    Constant('enemy_leveling', "Method for autoleveling generic units", [growth.value for growth in GrowthOption if growth != GrowthOption.PLAYER_CHOICE] + ["Match"], "Match", ConstantTag.LEVELING),
    Constant('auto_promote', "Units will promote automatically upon reaching max level", ConstantType.BOOL, False, ConstantTag.LEVELING),
    Constant('promote_skill_inheritance', "Promoted units will have the skills of their previous classes", ConstantType.BOOL, True, ConstantTag.LEVELING),
    Constant('promote_level_reset', "Promotion resets level back to 1", ConstantType.BOOL, True, ConstantTag.LEVELING),
    Constant('class_change_level_reset', "Class Change resets level back to 1", ConstantType.BOOL, False, ConstantTag.LEVELING),
    Constant('learn_skills_on_reclass', "Learn the skills of your new class when you reclass", ConstantType.BOOL, True, ConstantTag.LEVELING),
    Constant('learn_skills_on_promote', "Learn the skills of your new class when you promote", ConstantType.BOOL, True, ConstantTag.LEVELING),
    Constant('negative_growths', "Negative growth rates will reduce stats", ConstantType.BOOL, True, ConstantTag.LEVELING),
    Constant('class_change_same_tier', "Class Change only between classes of the same tier", ConstantType.BOOL, False, ConstantTag.LEVELING),
    Constant('generic_feats', "Generic units will be given random feats when appropriate", ConstantType.BOOL, False, ConstantTag.LEVELING),
    Constant('min_damage', "Min damage dealt by an attack", ConstantType.INT, 0),
    Constant('boss_crit', "Final blow on boss will use critical animation", ConstantType.BOOL, False, ConstantTag.AESTHETIC),
    Constant('battle_platforms', "Use battle platforms when battle backgrounds are on", ConstantType.BOOL, True, ConstantTag.AESTHETIC),
    Constant('roam_hide_hp', "Hide hp bars during free roam", ConstantType.BOOL, False, ConstantTag.AESTHETIC),
    Constant('autogenerate_grey_map_sprites', 'Automatically generate grey "wait" map sprites', ConstantType.BOOL, True, ConstantTag.AESTHETIC),
    Constant('translucent_unit_sprite', "A phantom of current unit will appear at cursor's position", ConstantType.BOOL, False, ConstantTag.AESTHETIC),
    Constant('talk_display', "If enough room, display who a unit can talk to in info menu", ConstantType.BOOL, False, ConstantTag.AESTHETIC),
    Constant('show_abilities', "Display range of extra abilities", ConstantType.BOOL, False, ConstantTag.AESTHETIC),
    Constant('info_menu_blink', "Portraits will blink in info menu", ConstantType.BOOL, False, ConstantTag.AESTHETIC),
    Constant('convoy_on_death', "Items held by dead player units are sent to convoy", ConstantType.BOOL, ConstantTag.OTHER),
    Constant('repair_shop', "Access the Repair Shop in prep and base", ConstantType.BOOL, False, ConstantTag.OTHER),
    Constant('sound_room_in_codex', "Can access sound room from Codex menu in base", ConstantType.BOOL, True, ConstantTag.OTHER),
    Constant('give_and_take', "Units can give a unit after taking a unit", ConstantType.BOOL, ConstantTag.OTHER),
    Constant('combat_art_category', "Combat Arts get put in their own category in the menu", ConstantType.BOOL, False, ConstantTag.OTHER),
    Constant('reset_mana', "Mana resets to full for units upon completion of the chapter", ConstantType.BOOL, ConstantTag.OTHER),
    Constant('double_splash', "When doubling, splash/aoe applied on second strike", ConstantType.BOOL, ConstantTag.OTHER),
    Constant('num_save_slots', "Number of save slots", ConstantType.POSITIVE_INT, 3, ConstantTag.TITLE),
    Constant('sell_modifier', "Value multiplier when selling an item", ConstantType.FLOAT, 0.5, ConstantTag.INVENTORY),
    Constant('attack_zero_hit', "Enemy AI attacks even if Hit is 0", ConstantType.BOOL, True, ConstantTag.AI),
    Constant('attack_zero_dam', "Enemy AI attacks even if Damage is 0", ConstantType.BOOL, True, ConstantTag.AI),
    Constant('zero_move', "Show Movement as 0 if AI does not move", ConstantType.BOOL, False, ConstantTag.AI),
    Constant('game_nid', "Game Unique Identifier", ConstantType.STR, "LT", ConstantTag.TITLE),
    Constant('title', "Game Title", ConstantType.STR, "Lex Talionis Game", ConstantTag.TITLE),
    Constant('title_particles', "Display particle effect on title screen", ConstantType.BOOL, True, ConstantTag.TITLE),
    Constant('title_sound', "Access sound room in Extras on title screen", ConstantType.BOOL, True, ConstantTag.TITLE),
    Constant('music_main', "Music to play on title screen", ConstantType.MUSIC, None, ConstantTag.MUSIC),
    Constant('music_promotion', "Music to play on promotion", ConstantType.MUSIC, None, ConstantTag.MUSIC),
    Constant('music_class_change', "Music to play on class change", ConstantType.MUSIC, None, ConstantTag.MUSIC),
    Constant('music_game_over', "Music to play on game over", ConstantType.MUSIC, 'Game Over', ConstantTag.MUSIC),
    Constant('restart_phase_music', "Restart phase music at beginning of new phase", ConstantType.BOOL, True, ConstantTag.MUSIC),
    Constant('restart_battle_music', "Restart battle music at beginning of each combat", ConstantType.BOOL, True, ConstantTag.MUSIC),
    Constant('kill_wexp', "Kills give double weapon exp", ConstantType.BOOL, True, ConstantTag.WEXP),
    Constant('double_wexp', "Each hit when doubling grants weapon exp", ConstantType.BOOL, True, ConstantTag.WEXP),
    Constant('miss_wexp', "Gain weapon exp even on miss", ConstantType.BOOL, True, ConstantTag.WEXP),
    # Experience constants below
    Constant('exp_curve', "How linear the exp curve is; Higher = less linear", ConstantType.FLOAT, 0.035, ConstantTag.EXP),
    Constant('exp_magnitude', "How much base exp is received for each interaction", ConstantType.FLOAT, 10, ConstantTag.EXP),
    Constant('exp_offset', "Tries to keep player character this many levels above enemies", ConstantType.INT, 0, ConstantTag.EXP),
    Constant('gexp_max', "Maximum exp that can be earned from a hit", ConstantType.FLOAT, 30, ConstantTag.EXP),
    Constant('gexp_min', "Minimum exp that can be earned from a hit", ConstantType.FLOAT, 1, ConstantTag.EXP),
    Constant('gexp_slope', "How sharply exp drops off", ConstantType.FLOAT, 0.25, ConstantTag.EXP),
    Constant('gexp_intercept', "Exp earned by two equal-level units fighting", ConstantType.FLOAT, 10, ConstantTag.EXP),
    Constant('exp_formula', "Which exp formula to use", [calcType.value for calcType in ExpCalcType], ExpCalcType.STANDARD.value, ConstantTag.EXP),
    Constant('kill_multiplier', "Exp multiplier on kill", ConstantType.FLOAT, 3, ConstantTag.EXP),
    Constant('boss_bonus', "Extra exp for killing a boss", ConstantType.INT, 40, ConstantTag.EXP),
    Constant('min_exp', "Min exp gained in combat", ConstantType.INT, 1, ConstantTag.EXP),
    Constant('default_exp', "Default exp gain", ConstantType.INT, 11, ConstantTag.EXP),
    Constant('heal_curve', "How much to multiply the amount healed by to determine experience gain", ConstantType.FLOAT, 0, ConstantTag.EXP),
    Constant('heal_magnitude', "Added to total exp for healing", ConstantType.INT, 0, ConstantTag.EXP),
    Constant('heal_offset', "Modifies expected healing", ConstantType.INT, 11, ConstantTag.EXP),
    Constant('heal_min', "Min exp gained for healing", ConstantType.INT, 11, ConstantTag.EXP),
])
