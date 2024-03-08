#include "global.h"
#include "uimenu.h"
#include "bmmenu.h"
#include "bmunit.h"
#include "bmitem.h"
#include "icon.h"
#include "uiselecttarget.h"
#include "bmtarget.h"
#include "uiutils.h"
#include "bmidoten.h"
#include "menuitempanel.h"
#include "playerphase.h"
#include "bmmind.h"

#include "common-chax.h"
#include "combat-art.h"
#include "weapon-range.h"

STATIC_DECLAR const struct MenuItemDef gCombatArtItemMenuItems[];
STATIC_DECLAR u8 CombatArtItemSelMenuOnCancel(struct MenuProc * menu, struct MenuItemProc * menuItem);

const struct MenuDef gCombatArtItemMenuDef = {
    {1, 1, 0xe, 0},
    0,
    gCombatArtItemMenuItems,
    0, 0, 0,
    CombatArtItemSelMenuOnCancel,
    MenuAutoHelpBoxSelect,
    ConvoyMenu_HelpBox
};

STATIC_DECLAR u8 CombatArtItemSelMenuOnCancel(struct MenuProc * menu, struct MenuItemProc * menuItem)
{
    ResetCombatArtStatus();
    return ItemMenu_ButtonBPressed(menu, menuItem);
}

STATIC_DECLAR u8 CombatArtItemSelUsability(const struct MenuItemDef * def, int number);
STATIC_DECLAR u8 CombatArtItemSelEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
STATIC_DECLAR int CombatArtItemSelHover(struct MenuProc * menu, struct MenuItemProc * menuItem);
STATIC_DECLAR int CombatArtItemSelUnhover(struct MenuProc * menu, struct MenuItemProc * menuItem);

STATIC_DECLAR const struct MenuItemDef gCombatArtItemMenuItems[] = {
    {"", 0, 0, 0, 0x49, CombatArtItemSelUsability, UnknownMenu_Draw, CombatArtItemSelEffect, 0, CombatArtItemSelHover, CombatArtItemSelUnhover},
    {"", 0, 0, 0, 0x4A, CombatArtItemSelUsability, UnknownMenu_Draw, CombatArtItemSelEffect, 0, CombatArtItemSelHover, CombatArtItemSelUnhover},
    {"", 0, 0, 0, 0x4B, CombatArtItemSelUsability, UnknownMenu_Draw, CombatArtItemSelEffect, 0, CombatArtItemSelHover, CombatArtItemSelUnhover},
    {"", 0, 0, 0, 0x4C, CombatArtItemSelUsability, UnknownMenu_Draw, CombatArtItemSelEffect, 0, CombatArtItemSelHover, CombatArtItemSelUnhover},
    {"", 0, 0, 0, 0x4D, CombatArtItemSelUsability, UnknownMenu_Draw, CombatArtItemSelEffect, 0, CombatArtItemSelHover, CombatArtItemSelUnhover},
    {0}
};

STATIC_DECLAR u8 CombatArtItemSelUsability(const struct MenuItemDef * def, int number)
{
    int item = gActiveUnit->items[number];

    if (!(GetItemAttributes(item) & IA_WEAPON))
        return MENU_NOTSHOWN;

    if (!CanUnitUseWeapon(gActiveUnit, item))
        return MENU_NOTSHOWN;

    RegisterCombatArtStatus(gActiveUnit, GetBestRangeBonusCid(gActiveUnit, item));
    MakeTargetListForWeapon(gActiveUnit, item);

    if (GetSelectTargetCount() == 0)
        return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

STATIC_DECLAR u8 CombatArtItemSelEffect(struct MenuProc * menu, struct MenuItemProc * menuItem)
{
    EquipUnitItemSlot(gActiveUnit, menuItem->itemNumber);
    gActionData.itemSlotIndex = 0;

    ClearBg0Bg1();

    MakeTargetListForWeapon(gActiveUnit, gActiveUnit->items[0]);

    // NewTargetSelection(&gSelectInfoCombatArt);
    NewTargetSelectionRework(&gSelectInfoCombatArt);

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_ENDFACE;
}

STATIC_DECLAR int CombatArtItemSelHover(struct MenuProc * menu, struct MenuItemProc * menuItem)
{
    int reach;
    int item = gActiveUnit->items[menuItem->itemNumber];

    UpdateMenuItemPanel(menuItem->itemNumber);

    BmMapFill(gBmMapMovement, -1);
    BmMapFill(gBmMapRange, 0);

    RegisterCombatArtStatus(gActiveUnit, GetBestRangeBonusCid(gActiveUnit, item));
    reach = GetUnitWeaponReachBits(gActiveUnit, menuItem->itemNumber);
    GenerateUnitStandingReachRange(gActiveUnit, reach);

    DisplayMoveRangeGraphics(MOVLIMITV_RMAP_RED);
    return 0;
}

STATIC_DECLAR int CombatArtItemSelUnhover(struct MenuProc * menu, struct MenuItemProc * menuItem)
{
    if (!(menu->state & MENU_STATE_ENDING))
        HideMoveRangeGraphics();

    return 0;
}
