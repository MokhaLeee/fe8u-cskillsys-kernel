#include "global.h"
#include "uimenu.h"
#include "bmmenu.h"
#include "bmunit.h"
#include "bmtarget.h"
#include "uiselecttarget.h"
#include "bmcontainer.h"
#include "bmphase.h"
#include "constants/items.h"
#include "constants/classes.h"

#include "common-chax.h"
#include "skill-system.h"
#include "chax-glb.h"
#include "constants/skills.h"

u8 DanceCommandUsabilityRework(const struct MenuItemDef * def, int number)
{
    if (!SkillTester(gActiveUnit, SID_Dance))
        return MENU_NOTSHOWN;

    gBmSt.um_tmp_item = ITEM_DANCE;
    return sub_80230F0(def);
}

u8 StealCommandUsabilityRework(const struct MenuItemDef * def, int number)
{
    if (!SkillTester(gActiveUnit, SID_Steal))
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

    if (!SkillTester(gActiveUnit, SID_Summon))
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

    if (!SkillTester(gActiveUnit, SID_LockTouch))
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

    if (SkillTester(gActiveUnit, SID_Supply))
        return MENU_ENABLED;

    for (i = 0; i < 4; i++)
    {
        struct Unit * unit = GetUnitAtPosition(
            gActiveUnit->xPos + vec_rang1[i].x,
            gActiveUnit->yPos + vec_rang1[i].y
        );

        if (!UNIT_IS_VALID(unit) || !AreUnitsAllied(gActiveUnit->index, unit->index))
            continue;

        if (SkillTester(unit, SID_Supply))
            return MENU_ENABLED;
    }
    return MENU_NOTSHOWN;
}
