#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void _SetPlusAndMinusStatDebuff(struct Unit * unit, struct Unit * unit_ally)
{
#if (defined(SID_Plus) && COMMON_SKILL_VALID(SID_Plus))
    #if (defined(SID_Minus) && COMMON_SKILL_VALID(SID_Minus))
        if (SkillTester(unit, SID_Plus))
            if (SkillTester(unit_ally, SID_Minus))
            {
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_POW);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_MAG);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_SKL);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_SPD);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_LCK);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_DEF);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_RES);
            }

        if (SkillTester(unit, SID_Minus))
            if (SkillTester(unit_ally, SID_Plus))
            {
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_POW);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_MAG);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_SKL);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_SPD);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_LCK);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_DEF);
            SetUnitStatDebuff(unit_ally, UNIT_STAT_BUFF_OATH_RES);
            }
    #endif
#endif
}

static void _ClearPlusAndMinusStatDebuff(struct Unit * unit)
{
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_POW);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_MAG);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_SKL);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_SPD);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_LCK);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_DEF);
    ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_RES);
}

bool PrePhsae_TickPlusAndMinusSkillStatus(ProcPtr proc)
{
    int i, j;
    bool ally_in_range = false;

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        struct Unit * unit = GetUnit(i);
        struct Unit * unit_ally;
        if (!UNIT_IS_VALID(unit))
            continue;

        for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++)
        {
            int _x = unit->xPos + gVecs_1x1[j].x;
            int _y = unit->yPos + gVecs_1x1[j].y;

            unit_ally = GetUnitAtPosition(_x, _y);
            if (!UNIT_IS_VALID(unit_ally))
                continue;

            if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            if (AreUnitsAllied(unit->index, unit_ally->index))
            {
                ally_in_range = true;
                break;
            }
        }

        if (!ally_in_range)
            _ClearPlusAndMinusStatDebuff(unit);
        else
            _SetPlusAndMinusStatDebuff(unit, unit_ally);
    }
    return false;
}
