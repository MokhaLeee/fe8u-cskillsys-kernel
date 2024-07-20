#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"

/* LynJump */
u8 SupplyUsability(const struct MenuItemDef * def, int number)
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
