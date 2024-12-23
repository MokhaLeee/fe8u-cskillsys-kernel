#include "common-chax.h"
#include "LimitedShopStock.h"
#include "action-expa.h"

extern struct KeyStatusBuffer sKeyStatusBuffer;

void SU_SaveShopStock(void* target, int size) {
    WriteAndVerifySramFast(gCurrentShopStocks, target, size);
}

void SU_LoadShopStock(void* source, int size) {
    ReadSramFast(source, gCurrentShopStocks, size);
}

void InitShopStock() {
    int i = 0;
    ShopStockEntry* limitedItems = gShopStockTable[gPlaySt.chapterIndex];
    if (limitedItems) {
        while (limitedItems[i].item) {
            (*gCurrentShopStocks)[i] = limitedItems[i].stock;
            ++i;
        }
        (*gCurrentShopStocks)[i] = 0; //Termintator
    }
    else {
        memset(*gCurrentShopStocks, 0, 1); //Terminator only
    }
}

int GetItemStockEntryNumber(u16 item) {
    u8 itemID = GetItemIndex(item);
    ShopStockEntry* limitedItems = gShopStockTable[gPlaySt.chapterIndex];
    if (limitedItems) {
        for (int i = 0; limitedItems[i].item; ++i) {
            if (limitedItems[i].item == itemID) {
                return i;
            }
        }
    }
    return (-1); //Item is not a stock item
}

int GetItemStock(u16 item) {
    int stockItem = GetItemStockEntryNumber(item);
    if (stockItem != (-1)) {
        return (*gCurrentShopStocks)[stockItem];
    }
    return (-1); 
}

bool IsItemInStock(u16 item) {
    int stockItem = GetItemStockEntryNumber(item);
    if (stockItem != (-1)) {
        if ((*gCurrentShopStocks)[stockItem]) {
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
    return TRUE;
}

void ReduceItemStock(u16 item) {
    int stockItem = GetItemStockEntryNumber(item);
    if (stockItem != (-1) && (*gCurrentShopStocks)[stockItem] > 0) {
        (*gCurrentShopStocks)[stockItem]--;
    }
}

LYN_REPLACE_CHECK(DrawShopItemLine);
void DrawShopItemLine(struct Text* th, int item, struct Unit* unit, u16* dst) {
    DrawItemMenuLine(th, item, IsItemDisplayUsable(unit, item), dst);

    if (IsItemSellable(item) != 0) {
        PutNumber(dst + 0x11, 2, GetItemSellPrice(item));
    } else {
        Text_InsertDrawString(th, 0x5C, 2, GetStringFromIndex(0x537));
    }

    return;
}

void DrawStockedItemLine(struct Text* text, int item, s8 isUsable, u16* mapOut) {
    // Determine the text color based on item usability and stock
    u8 textColor;
    if (GetItemStock(item) != -1 && isUsable) {
        textColor = TEXT_COLOR_SYSTEM_GOLD;
    } else {
        textColor = isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY;
    }

    // Set the determined color
    Text_SetParams(text, 0, textColor);
    Text_DrawStringASCII(text, GetItemName(item));

    PutText(text, mapOut + 2);

#ifndef CONFIG_INFINITE_DURABILITY
    PutNumberOrBlank(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
    PutNumberOrBlank(mapOut + 20, TEXT_COLOR_SYSTEM_BLUE, GetItemStock(item));
#endif

    DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}


LYN_REPLACE_CHECK(DrawShopItemPriceLine);
void DrawShopItemPriceLine(struct Text* th, int item, struct Unit* unit, u16* dst) {
    u8 visible;

    int price = GetItemPurchasePrice(unit, item);

    if (unit == 0) {
        visible = 1;
    } else {
        visible = IsItemDisplayUsable(unit, item);
    }

    DrawStockedItemLine(th, item, visible, dst);

    PutNumber(
        dst + 0x11,
        (int)GetPartyGoldAmount() >= price
            ? 2
            : 1,
        price
    );

    return;
}

LYN_REPLACE_CHECK(Shop_Loop_BuyKeyHandler);
void Shop_Loop_BuyKeyHandler(struct ProcShop* proc) {
    u8 unkA;
    u32 unkC;
    int price;
    int a;
    int b;

    Shop_TryMoveHandPage();

    BG_SetPosition(2, 0, ShopSt_GetBg2Offset());

    unkA = proc->curIndex;
    unkC = ShopSt_GetHeadLoc() != unkA;

    proc->curIndex = ShopSt_GetHeadLoc();
    proc->unk_5d = ShopSt_GetHandLoc();

    proc->unk_5e = proc->curIndex;
    proc->unk_5f = proc->unk_5d;

    a = proc->curIndex;
    a *= 16;

    b = ((proc->unk_5d * 16)) - 72;

    DisplayUiHand(40, a - b);

    if ((proc->helpTextActive != 0) && (unkC != 0)) {
        a = (proc->curIndex * 16);
        b = ((proc->unk_5d * 16) - 72);
        StartItemHelpBox(56, a - b, proc->shopItems[proc->curIndex]);
    }

    DisplayShopUiArrows();

    if (IsShopPageScrolling() != 0) {
        return;
    }

    if (proc->helpTextActive != 0) {
        if (sKeyStatusBuffer.heldKeys & (B_BUTTON | R_BUTTON)) {
            proc->helpTextActive = 0;
            CloseHelpBox();
        }

        return;
    }

    if (sKeyStatusBuffer.heldKeys & R_BUTTON) {
        proc->helpTextActive = 1;
        a = (proc->curIndex * 16);
        b = ((proc->unk_5d * 16) - 72);
        StartItemHelpBox(56, a - b, proc->shopItems[proc->curIndex]);

        return;
    }

    price = GetItemPurchasePrice(proc->unit, proc->shopItems[proc->curIndex]);

    if (sKeyStatusBuffer.heldKeys & A_BUTTON) {
        if (!IsItemInStock(proc->shopItems[proc->curIndex])) {
            StartShopDialogue(OutOfStockTextBase, proc);

            Proc_Goto(proc, 1);
        }
        else if (price > (int)GetPartyGoldAmount()) {
            StartShopDialogue(0x8B2, proc);
            // SHOP_TYPE_ARMORY: "You don't have the money![.][A]"
            // SHOP_TYPE_VENDOR: "You're short of funds.[A]"
            // SHOP_TYPE_SECRET_SHOP: "Heh! Not enough money![A]"

            Proc_Goto(proc, 1);
        } else {
            SetTalkNumber(price);
            StartShopDialogue(0x8B5, proc);
            // SHOP_TYPE_ARMORY: "How does [.][G] gold[.][NL]sound to you?[.][Yes]"
            // SHOP_TYPE_VENDOR: "That's worth [.][G] gold.[NL]Is that all right?[Yes]"
            // SHOP_TYPE_SECRET_SHOP: "That is worth [G] gold.[NL]Is that acceptable?[.][Yes]"

            Proc_Break(proc);
        }

        return;
    }

    if (sKeyStatusBuffer.heldKeys & B_BUTTON) {
        PlaySFX(0x6B, 0x100, 0, 1);
        Proc_Goto(proc, 7);

        return;
    }

    return;
}

LYN_REPLACE_CHECK(HandleShopBuyAction);
void HandleShopBuyAction(struct ProcShop* proc) {
    PlaySeDelayed(0xB9, 8);

    gActionData.unitActionType = UNIT_ACTION_SHOPPED;

    SetPartyGoldAmount(GetPartyGoldAmount() - GetItemPurchasePrice(proc->unit, proc->shopItems[proc->curIndex]));
    ReduceItemStock(proc->shopItems[proc->curIndex]);

    UpdateShopItemCounts(proc);
    DrawShopSoldItems(proc);

    DisplayGoldBoxText(gBG0TilemapBuffer + 0xDB);

    return;
}
