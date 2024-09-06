#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"
#include "constants/texts.h"

extern u16 gUnknown_085A0D4C[];

enum
{
    LINES_MAX = 5
};

struct UnitInfoWindowProc
{
    /* 00 */ PROC_HEADER;

    /* 2C */ struct Unit *unit;

    /* 30 */ struct Text name;
    /* 38 */ struct Text lines[LINES_MAX];

    /* 60 */ u8 x;
    /* 61 */ u8 y;
    /* 62 */ u8 xUnitSprite;
    /* 63 */ u8 xNameText;
};

int GetUnitInfoWindowX(struct Unit *unit, int width);
struct UnitInfoWindowProc *UnitInfoWindow_DrawBase(struct UnitInfoWindowProc *proc, struct Unit *unit, int x, int y, int width, int lines);

u8 StealPlus_Usability(const struct MenuItemDef *def, int number)
{
#if !CHAX
    if (!(UNIT_CATTRIBUTES(gActiveUnit) & CA_STEAL))
        return MENU_NOTSHOWN;
#endif

    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    MakeTargetListForSteal(gActiveUnit);
    if (GetSelectTargetCount() == 0)
        return MENU_NOTSHOWN;

    if (GetUnitItemCount(gActiveUnit) == UNIT_ITEM_COUNT)
        return MENU_DISABLED;

    return MENU_ENABLED;
}

LYN_REPLACE_CHECK(RefreshUnitStealInventoryInfoWindow);
void RefreshUnitStealInventoryInfoWindow(struct Unit *unit)
{
    int i;
    int itemCount;
    int xPos;
    struct UnitInfoWindowProc *proc;

    itemCount = GetUnitItemCount(unit);

    xPos = GetUnitInfoWindowX(unit, 0xd);

    proc = UnitInfoWindow_DrawBase(0, unit, xPos, 0, 0xd, itemCount);

    bool stealPlus = false;

#if defined(SID_StealPlus) && (COMMON_SKILL_VALID(SID_StealPlus))
    if (SkillTester(gActiveUnit, SID_StealPlus))
        stealPlus = true;
#endif

    for (i = 0; i < itemCount; i++)
    {
        int yPos = 0 + i * 2 + 3;

        int item = unit->items[i];
        s8 stealable = IsItemStealable(item);

        // Unit's CON must exceed the weight of the weapon and it can't be the first weapon in their inventory
        if (_GetUnitCon(gActiveUnit) > GetItemWeight(item) && stealPlus && i != 0)
            stealable = 1;

        ClearText(proc->lines + i);

        Text_SetColor(proc->lines + i, stealable ? 0 : 1);
        Text_DrawString(proc->lines + i, GetItemName(item));

        PutText(proc->lines + i, gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 3, yPos));

        PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 11, yPos), stealable ? 2 : 1, GetItemUses(item));
        DrawIcon(gBG0TilemapBuffer + TILEMAP_INDEX(xPos + 1, yPos), GetItemIconId(item), 0x4000);
    }

    return;
}

u8 StealPlus_Effect(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
    if (menuItem->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, 0x862); // TODO msgid "Your inventory is full.[.]"
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForSteal(gActiveUnit);

    NewTargetSelection(&gSelectInfo_Steal);

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

LYN_REPLACE_CHECK(StealMapSelect_Init);
void StealMapSelect_Init(ProcPtr menu)
{
    StartUnitInventoryInfoWindow(menu);
    StartSubtitleHelp(menu, GetStringFromIndex(0x86D)); // TODO: msgid "Select which unit to steal from."

    return;
}

LYN_REPLACE_CHECK(StealMapSelect_SwitchIn);
u8 StealMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget *target)
{
    ChangeActiveUnitFacing(target->x, target->y);

    RefreshUnitStealInventoryInfoWindow(GetUnit(target->uid));

    return 0; // BUG?
}

LYN_REPLACE_CHECK(StealMapSelect_Select);
u8 StealMapSelect_Select(ProcPtr proc, struct SelectTarget *target)
{
    int pos;

    gActionData.targetIndex = target->uid;

    ResetIconGraphics();
    LoadIconPalettes(4);

    StartOrphanMenu(&gStealItemMenuDef);

    EndTargetSelection(proc);

    CallARM_FillTileRect(gBG1TilemapBuffer + 0x42, gUnknown_085A0D4C, 0x1000);

    pos = (56 - GetStringTextLen(GetStringFromIndex(GetUnit(gActionData.targetIndex)->pCharacterData->nameTextId))) / 2;

    PutDrawText(0, gBG0TilemapBuffer + 0x63, 0, pos, 7, GetStringFromIndex(GetUnit(gActionData.targetIndex)->pCharacterData->nameTextId));

    PutFace80x72_Core(gBG0TilemapBuffer + 0x63 + 0x40, GetUnitPortraitId(GetUnit(gActionData.targetIndex)), 0x200, 5);

    return 0;
}

LYN_REPLACE_CHECK(StealItemMenuCommand_Usability);
u8 StealItemMenuCommand_Usability(const struct MenuItemDef *def, int number)
{
    if (GetUnit(gActionData.targetIndex)->items[number] == 0)
    {
        return MENU_NOTSHOWN;
    }

#if defined(SID_StealPlus) && (COMMON_SKILL_VALID(SID_StealPlus))
    if (SkillTester(gActiveUnit, SID_StealPlus))
    {
        if (_GetUnitCon(gActiveUnit) > GetItemWeight(GetUnit(gActionData.targetIndex)->items[number]) && number != 0)
            return MENU_ENABLED;
    }
#endif

    if (!IsItemStealable(GetUnit(gActionData.targetIndex)->items[number]))
    {
        return MENU_DISABLED;
    }

    return MENU_ENABLED;
}

LYN_REPLACE_CHECK(StealItemMenuCommand_Draw);
int StealItemMenuCommand_Draw(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
    int item = GetUnit(gActionData.targetIndex)->items[menuItem->itemNumber];
    s8 isStealable = IsItemStealable(item);

#if defined(SID_StealPlus) && (COMMON_SKILL_VALID(SID_StealPlus))
    if (SkillTester(gActiveUnit, SID_StealPlus))
        if (_GetUnitCon(gActiveUnit) > GetItemWeight(item) && menuItem->itemNumber != 0)
            isStealable = 1;
#endif

    DrawItemMenuLine(&menuItem->text, item, isStealable, gBG0TilemapBuffer + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile));

    return 0; // vanilla returns without a type, likely due to older compilers defaulting to 0
}

LYN_REPLACE_CHECK(StealItemMenuCommand_Effect);
u8 StealItemMenuCommand_Effect(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
    if (menuItem->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, 0x855); // TODO: msgid "Weapons, magic, and[.][NL]staves can't be stolen.[.]"
        return MENU_ACT_SND6B;
    }

    gActionData.itemSlotIndex = menuItem->itemNumber;
    gActionData.unitActionType = UNIT_ACTION_STEAL;

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}