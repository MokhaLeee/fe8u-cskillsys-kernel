#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"

u8 DanceCommandUsabilityRework(const struct MenuItemDef * def, int number)
{
#if defined(SID_Dance) && (COMMON_SKILL_VALID(SID_Dance))
    if (!SkillTester(gActiveUnit, SID_Dance))
#else
    if (1)
#endif
        return MENU_NOTSHOWN;

    gBmSt.um_tmp_item = ITEM_DANCE;
    return sub_80230F0(def);
}

u8 StealCommandUsabilityRework(const struct MenuItemDef * def, int number)
{
#if defined(SID_Steal) && (COMMON_SKILL_VALID(SID_Steal))
    if (!SkillTester(gActiveUnit, SID_Steal))
#else
    if (1)
#endif
        return MENU_NOTSHOWN;

    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    MakeTargetListForSteal(gActiveUnit);
    if (GetSelectTargetCount() == 0)
        return MENU_NOTSHOWN;

    if (GetUnitItemCount(gActiveUnit) == UNIT_ITEM_COUNT)
        return MENU_DISABLED;

    return MENU_ENABLED;
}

u8 SummonCommandUsabilityRework(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

#if defined(SID_Summon) && (COMMON_SKILL_VALID(SID_Summon))
    if (!SkillTester(gActiveUnit, SID_Summon))
#else
    if (1)
#endif
        return MENU_NOTSHOWN;

    MakeTargetListForSummon(gActiveUnit);
    if (GetSelectTargetCount() == 0)
        return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

u8 PickCommandUsabilityRework(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

#if defined(SID_LockTouch) && (COMMON_SKILL_VALID(SID_LockTouch))
    if (!SkillTester(gActiveUnit, SID_LockTouch))
#else
    if (1)
#endif
        return MENU_NOTSHOWN;

    MakeTargetListForPick(gActiveUnit);
    if (GetSelectTargetCount() == 0)
        return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

u8 SupplyUsabilityRework(const struct MenuItemDef * def, int number)
{
    const struct Vec2 vec_rang1[4] = {
                  { 0, -1},
        {-1,  0},           { 1,  0},
                  { 0,  1}
    };

    int i;

#if 0
    if (!HasConvoyAccess())
        return MENU_NOTSHOWN;
#endif

    if (UNIT_CLASS_ID(gActiveUnit) == CLASS_PHANTOM)
        return MENU_NOTSHOWN;

#if defined(SID_Supply) && (COMMON_SKILL_VALID(SID_Supply))
    if (SkillTester(gActiveUnit, SID_Supply))
#else
    if (0)
#endif
        return MENU_ENABLED;

    for (i = 0; i < 4; i++)
    {
        struct Unit * unit = GetUnitAtPosition(
            gActiveUnit->xPos + vec_rang1[i].x,
            gActiveUnit->yPos + vec_rang1[i].y
        );

        if (!UNIT_IS_VALID(unit) || !AreUnitsAllied(gActiveUnit->index, unit->index))
            continue;

#if defined(SID_Supply) && (COMMON_SKILL_VALID(SID_Supply))
        if (SkillTester(unit, SID_Supply))
#else
        if (0)
#endif
            return MENU_ENABLED;
    }
    return MENU_NOTSHOWN;
}
