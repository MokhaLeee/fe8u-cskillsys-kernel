from app.engine.sound import get_sound_thread
from app.engine.state import MapState
from app.engine.game_state import game
from app.engine import action, menus, item_system, item_funcs
from app.engine.objects.item import ItemObject

def check_trade(item1: ItemObject, item1_owner, item2: ItemObject, item2_owner) -> bool:
    # Can't trade the same item to itself
    if item1 is item2:
        return False
    # Can always trade within the same menu
    if item1_owner is item2_owner:
        return True
    # Can't trade locked items
    if isinstance(item1, ItemObject) and item_system.locked(item1_owner, item1):
        return False
    if isinstance(item2, ItemObject) and item_system.locked(item2_owner, item2):
        return False
    # If items are the same type, we are good
    if isinstance(item1, ItemObject) and isinstance(item2, ItemObject) and \
            item_system.is_accessory(item1_owner, item1) == item_system.is_accessory(item2_owner, item2):
        return True

    # Now check if the trade is bad
    if isinstance(item1, ItemObject):
        if item_system.is_accessory(item1_owner, item1):
            if item2_owner and len(item2_owner.accessories) >= item_funcs.get_num_accessories(item2_owner):
                return False
        else:
            if item2_owner and len(item2_owner.nonaccessories) >= item_funcs.get_num_items(item2_owner):
                return False
    if isinstance(item2, ItemObject):
        if item_system.is_accessory(item2_owner, item2):
            if item1_owner and len(item1_owner.accessories) >= item_funcs.get_num_accessories(item1_owner):
                return False
        else:
            if item1_owner and len(item1_owner.nonaccessories) >= item_funcs.get_num_items(item1_owner):
                return False

    return True

class TradeState(MapState):
    name = 'trade'

    def has_traded(self):
        action.do(action.HasTraded(self.initiator))

    def start(self):
        game.cursor.hide()
        self.initiator = game.cursor.cur_unit
        self.initiator.sprite.change_state('chosen')
        self.partner = game.memory['trade_partner']

        self.menu = menus.Trade(self.initiator, self.partner)

    def do_trade(self) -> bool:
        item1 = self.menu.selected_option().get()
        item2 = self.menu.get_current_option().get()

        if self.menu.other_hand[0] == 0:
            self.item1_owner = self.initiator
        else:
            self.item1_owner = self.partner
        if self.menu.selecting_hand[0] == 0:
            self.item2_owner = self.initiator
        else:
            self.item2_owner = self.partner

        if not check_trade(item1, self.item1_owner, item2, self.item2_owner):
            self.menu.unset_selected_option()
            get_sound_thread().play_sfx('Error')
            return False

        if self.menu.other_hand[0] == 0:
            if self.menu.selecting_hand[0] == 0:
                action.do(action.TradeItem(self.initiator, self.initiator, item1, item2))
            else:
                action.do(action.TradeItem(self.initiator, self.partner, item1, item2))
        else:
            if self.menu.selecting_hand[0] == 0:
                action.do(action.TradeItem(self.partner, self.initiator, item1, item2))
            else:
                action.do(action.TradeItem(self.partner, self.partner, item1, item2))
        self.has_traded()

        self.menu.unset_selected_option()
        self.menu.update_options()
        return True

    def back(self):
        game.state.back()
        game.state.back()
        self.initiator.sprite.change_state('normal')

    def take_input(self, event):
        first_push = self.fluid.update()
        directions = self.fluid.get_directions()

        self.menu.handle_mouse()
        if 'DOWN' in directions:
            if self.menu.move_down(first_push):
                get_sound_thread().play_sfx('Select 6')
        elif 'UP' in directions:
            if self.menu.move_up(first_push):
                get_sound_thread().play_sfx('Select 6')

        if event == 'RIGHT':
            if self.menu.move_right():
                get_sound_thread().play_sfx('TradeRight')
        elif event == 'LEFT':
            if self.menu.move_left():
                get_sound_thread().play_sfx('TradeRight')

        elif event == 'BACK':
            get_sound_thread().play_sfx('Select 4')
            if self.menu.selected_option():
                self.menu.unset_selected_option()
            else:
                self.back()

        elif event == 'SELECT':
            if self.menu.selected_option():
                if self.do_trade():
                    get_sound_thread().play_sfx('Select 1')
                else:
                    get_sound_thread().play_sfx('Error')
            else:
                get_sound_thread().play_sfx('Select 1')
                self.menu.set_selected_option()

        elif event == 'INFO':
            self.menu.toggle_info()

    def update(self):
        super().update()
        self.menu.update()

    def draw(self, surf):
        surf = super().draw(surf)
        self.menu.draw(surf)
        return surf

class CombatTradeState(TradeState):
    name = 'combat_trade'

    def back(self):
        game.state.back()

class PrepTradeState(TradeState):
    name = 'prep_trade'

    def has_traded(self):
        # Prep Trade doesn't use up your turn
        pass

    def start(self):
        self.bg = game.memory['prep_bg']
        self.initiator = game.memory['unit1']
        self.partner = game.memory['unit2']

        self.menu = menus.Trade(self.initiator, self.partner)

        game.state.change('transition_in')
        return 'repeat'

    def back(self):
        game.state.back()

    def update(self):
        self.menu.update()

    def draw(self, surf):
        if self.bg:
            self.bg.draw(surf)
        self.menu.draw(surf)
        return surf
