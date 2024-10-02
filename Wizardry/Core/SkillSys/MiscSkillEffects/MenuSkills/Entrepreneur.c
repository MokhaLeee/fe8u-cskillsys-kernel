#include "common-chax.h"
#include "kernel-lib.h"
#include "map-anims.h"
#include "skill-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"

#if defined(SID_Entrepreneur) && (COMMON_SKILL_VALID(SID_Entrepreneur))

u16 CONST_DATA gDefaultShopInventory[] = {
    ITEM_SWORD_IRON, ITEM_LANCE_IRON, ITEM_AXE_IRON, ITEM_BOW_IRON, ITEM_ANIMA_FIRE, ITEM_STAFF_HEAL, ITEM_NONE,
};

u8 Entrepreneur_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (UNIT_CLASS_ID(gActiveUnit) == CLASS_PHANTOM)
        return MENU_NOTSHOWN;

    if (SkillTester(gActiveUnit, SID_Entrepreneur))
        return MENU_ENABLED;

    for (int i = 0; i < ARRAY_COUNT_RANGE1x1; i++)
    {
        struct Unit * unit = GetUnitAtPosition(gActiveUnit->xPos + gVecs_1x1[i].x, gActiveUnit->yPos + gVecs_1x1[i].y);

        if (!UNIT_IS_VALID(unit) || !AreUnitsAllied(gActiveUnit->index, unit->index))
            continue;

        if (SkillTester(unit, SID_Entrepreneur))
            return MENU_ENABLED;
    }

    return MENU_NOTSHOWN;
}

u8 Entrepreneur_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_Entrepreneur_FRtext);
        return MENU_ACT_SND6B;
    }

    gActionData.unk08 = SID_Entrepreneur;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

static void callback_anim(ProcPtr proc)
{
    // Nothing happens here for now
}

static void callback_exec(ProcPtr proc)
{
    StartShopScreen(gActiveUnit, NULL, SHOP_TYPE_ARMORY, proc);
}

bool Action_Entrepreneur(ProcPtr parent)
{
    NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
    return true;
}
#endif