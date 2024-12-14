from __future__ import annotations

from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from app.engine.state import State

import logging


class SimpleStateMachine():
    def __init__(self, starting_state):
        self.state = []
        self.state.append(starting_state)

    def change(self, new_state):
        self.state.append(new_state)

    def back(self):
        self.state.pop()

    def get_state(self):
        if self.state:
            return self.state[-1]
        return None

    def clear(self):
        self.state.clear()

class StateMachine():
    def __init__(self):
        self.state = []
        self.temp_state = []
        self.prev_state = None

    def load_states(self, starting_states=None, temp_state=None):
        from app.engine import (base, chapter_title, debug_mode, dialog_log,
                                feat_choice, game_over, general_states,
                                level_up, minimap, objective_menu,
                                player_choice, prep, promotion,
                                settings, status_upkeep, text_entry,
                                title_screen, trade, transitions, turnwheel,
                                victory_screen)
        from app.engine.game_menus.menu_states import unit_menu_state
        from app.engine.info_menu import info_menu_state
        from app.engine.overworld import overworld_states
        from app.engine.roam import free_roam_state, free_roam_rationalize
        from app.events import event_state
        self.all_states = \
            {'title_start': title_screen.TitleStartState,
             'title_main': title_screen.TitleMainState,
             'title_load': title_screen.TitleLoadState,
             'title_restart': title_screen.TitleRestartState,
             'title_mode': title_screen.TitleModeState,
             'title_new': title_screen.TitleNewState,
             'title_new_child': title_screen.TitleNewChildState,
             'title_extras': title_screen.TitleExtrasState,
             'title_all_saves': title_screen.TitleAllSavesState,
             'title_wait': title_screen.TitleWaitState,
             'title_save': title_screen.TitleSaveState,
             'in_chapter_save': title_screen.TitleSaveState,
             'transition_in': transitions.TransitionInState,
             'transition_out': transitions.TransitionOutState,
             'transition_pop': transitions.TransitionPopState,
             'transition_double_pop': transitions.TransitionDoublePopState,
             'transition_to': transitions.TransitionToState,
             'transition_to_with_pop': transitions.TransitionToWithPopState,
             'start_level_asset_loading': general_states.LoadingState,
             'turn_change': general_states.TurnChangeState,
             'initiative_upkeep': general_states.InitiativeUpkeep,
             'free': general_states.FreeState,
             'option_menu': general_states.OptionMenuState,
             'option_child': general_states.OptionChildState,
             'settings_menu': settings.SettingsMenuState,
             'objective_menu': objective_menu.ObjectiveMenuState,
             'unit_menu': unit_menu_state.UnitMenuState,
             'info_menu': info_menu_state.InfoMenuState,
             'phase_change': general_states.PhaseChangeState,
             'move': general_states.MoveState,
             'movement': general_states.MovementState,
             'wait': general_states.WaitState,
             'canto_wait': general_states.CantoWaitState,
             'move_camera': general_states.MoveCameraState,
             'dying': general_states.DyingState,
             'menu': general_states.MenuState,
             'item': general_states.ItemState,
             'subitem_child': general_states.SubItemChildState,
             'item_child': general_states.ItemChildState,
             'item_discard': general_states.ItemDiscardState,
             'targeting': general_states.TargetingState,
             'trade': trade.TradeState,
             'combat_trade': trade.CombatTradeState,
             'weapon_choice': general_states.WeaponChoiceState,
             'spell_choice': general_states.SpellChoiceState,
             'combat_art_choice': general_states.CombatArtChoiceState,
             'combat_targeting': general_states.CombatTargetingState,
             'item_targeting': general_states.ItemTargetingState,
             'combat': general_states.CombatState,
             'alert': general_states.AlertState,
             'ai': general_states.AIState,
             'shop': general_states.ShopState,
             'repair_shop': general_states.RepairShopState,
             'unlock_select': general_states.UnlockSelectState,
             'exp': level_up.ExpState,
             'promotion_choice': promotion.PromotionChoiceState,
             'class_change_choice': promotion.ClassChangeChoiceState,
             'promotion': promotion.PromotionState,
             'class_change': promotion.ClassChangeState,
             'feat_choice': feat_choice.FeatChoiceState,
             'turnwheel': turnwheel.TurnwheelState,
             'game_over': game_over.GameOverState,
             'chapter_title': chapter_title.ChapterTitleState,
             'event': event_state.EventState,
             'player_choice': player_choice.PlayerChoiceState,
             'text_entry': text_entry.TextEntryState,
             'text_confirm': text_entry.TextConfirmState,
             'victory': victory_screen.VictoryState,
             'minimap': minimap.MinimapState,
             'status_upkeep': status_upkeep.StatusUpkeepState,
             'status_endstep': status_upkeep.StatusUpkeepState,
             'prep_main': prep.PrepMainState,
             'prep_pick_units': prep.PrepPickUnitsState,
             'prep_formation': prep.PrepFormationState,
             'prep_formation_select': prep.PrepFormationSelectState,
             'prep_manage': prep.PrepManageState,
             'prep_manage_select': prep.PrepManageSelectState,
             'optimize_all_choice': prep.OptimizeAllChoiceState,
             'base_manage': prep.PrepManageState,
             'base_manage_select': prep.PrepManageSelectState,
             'prep_trade_select': prep.PrepTradeSelectState,
             'prep_trade': trade.PrepTradeState,
             'prep_items': prep.PrepItemsState,
             'base_items': prep.PrepItemsState,
             'supply_items': prep.PrepItemsState,
             'prep_restock': prep.PrepRestockState,
             'prep_use': prep.PrepUseState,
             'prep_market': prep.PrepMarketState,
             'base_main': base.BaseMainState,
             'base_market_select': base.BaseMarketSelectState,
             'base_bexp_select': base.BaseBEXPSelectState,
             'base_bexp_allocate': base.BaseBEXPAllocateState,
             'base_convos_child': base.BaseConvosChildState,
             'base_supports': base.BaseSupportsState,
             'base_codex_child': base.BaseCodexChildState,
             'base_library': base.BaseLibraryState,
             'base_guide': base.BaseGuideState,
             'base_records': base.BaseRecordsState,
             'base_sound_room': base.BaseSoundRoomState,
             'base_achievement': base.BaseAchievementState,
             'extras_sound_room': base.BaseSoundRoomState,
             'free_roam': free_roam_state.FreeRoamState,
             'free_roam_rationalize': free_roam_rationalize.FreeRoamRationalizeState,
             'debug': debug_mode.DebugState,
             'overworld': overworld_states.OverworldFreeState,
             'overworld_movement': overworld_states.OverworldMovementState,
             'overworld_game_option_menu': overworld_states.OverworldGameOptionMenuState,
             'overworld_party_option_menu': overworld_states.OverworldPartyOptionMenu,
             'overworld_on_node': overworld_states.OverworldNodeTransition,
             'overworld_next_level': overworld_states.OverworldLevelTransition,
             'dialog_log': dialog_log.DialogLogState
             }

        if starting_states:
            for state_name in starting_states:
                self.state.append(self.all_states[state_name](state_name))
        if temp_state:
            self.temp_state = temp_state

    def state_names(self):
        return [s.name for s in self.state]

    def change(self, new_state):
        self.temp_state.append(new_state)

    def back(self):
        self.temp_state.append('pop')

    def clear(self):
        self.temp_state.append('clear')

    def refresh(self):
        # Clears all states except the top one
        self.state = self.state[-1:]

    def current(self):
        if self.state:
            return self.state[-1].name

    def current_state(self) -> State:
        if self.state:
            return self.state[-1]

    def get_prev_state(self) -> State:
        if self.state and len(self.state) > 1:
            return self.state[-2]

    def exit_state(self, state):
        if state.processed:
            state.processed = False
            state.end()
        state.finish()

    def from_transition(self):
        return self.prev_state in ('transition_out', 'transition_to', 'transition_pop', 'transition_double_pop')

    def process_temp_state(self):
        if self.temp_state:
            logging.debug("Temp State: %s", self.temp_state)
        for transition in self.temp_state:
            if transition == 'pop':
                if self.state:
                    state = self.state[-1]
                    self.exit_state(state)
                    self.state.pop()
            elif transition == 'clear':
                for state in reversed(self.state):
                    self.exit_state(state)
                self.state.clear()
            else:
                new_state = self.all_states[transition](transition)
                self.state.append(new_state)
        if self.temp_state:
            logging.debug("State: %s", self.state_names())
        self.temp_state.clear()

    def update(self, event, surf):
        if not self.state:
            return None, False
        state = self.state[-1]
        repeat_flag = False  # Whether we run the state machine again in the same frame
        # Start
        if not state.started:
            state.started = True
            start_output = state.start()
            if start_output == 'repeat':
                repeat_flag = True
            self.prev_state = state.name
        # Begin
        if not repeat_flag and not state.processed:
            state.processed = True
            begin_output = state.begin()
            if begin_output == 'repeat':
                repeat_flag = True
        # Take Input
        if not repeat_flag:
            input_output = state.take_input(event)
            if input_output == 'repeat':
                repeat_flag = True
        # Update
        if not repeat_flag:
            update_output = state.update()
            if update_output == 'repeat':
                repeat_flag = True
        # Draw
        if not repeat_flag:
            # Handles transparency of states
            idx = -1
            while True:
                if self.state[idx].transparent and len(self.state) >= (abs(idx) + 1):
                    idx -= 1
                else:
                    break
            while idx <= -1:
                surf = self.state[idx].draw(surf)
                idx += 1
        # End
        if self.temp_state and state.processed:
            state.processed = False
            state.end()
        # Finish
        self.process_temp_state()  # This is where FINISH is taken care of
        return surf, repeat_flag

    def save(self):
        return [state.name for state in self.state], self.temp_state[:]  # Needs to be a copy!!!
