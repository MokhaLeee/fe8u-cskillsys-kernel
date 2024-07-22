#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "constants/texts.h"
#include "constants/skills.h"

STATIC_DECLAR bool AreAnyEnemyExists(void)
{
    int i;
    for (i = FACTION_RED + 1; i < GetFactionUnitAmount(FACTION_RED); i++)
    {
        struct Unit * unit = GetUnit(i);
        if (UNIT_IS_VALID(unit) && !(unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16)))
            return true;
    }
    return false;
}

u8 Teleportation_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING || CheckBitUES(gActiveUnit, UES_BIT_TSZUKU_SKILL_USED))
        return MENU_NOTSHOWN;

#if defined(SID_Teleportation) && (COMMON_SKILL_VALID(SID_Teleportation))
    if ((GetUnitCurrentHp(gActiveUnit) * 100) < (GetUnitMaxHp(gActiveUnit) * SKILL_EFF0(SID_Teleportation)))
        return MENU_DISABLED;
#endif

    if (!AreAnyEnemyExists())
        return MENU_DISABLED;

    return MENU_ENABLED;
}

u8 Teleportation_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    struct Vec2 vec;

    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_CommonFail);
        return MENU_ACT_SND6B;
    }

    if (GetTeleportationRandomPosition(gActiveUnit, &vec) == true)
    {
        gActionData.xMove = vec.x;
        gActionData.yMove = vec.y;
    }
    else
    {
        /* Yeah, Who said you can't teleport without movement LOL */
        gActionData.xMove = gActiveUnit->xPos;
        gActionData.yMove = gActiveUnit->yPos;
    }

    gActionData.subjectIndex = gActiveUnit->index;
    gActionData.unk08 = SID_Teleportation;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
