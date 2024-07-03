#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void _SetPlusAndMinusStatDebuff(struct Unit * unit)
{
    SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_POW);
    SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_MAG);
    SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_SKL);
    SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_SPD);
    SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_LCK);
    SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_DEF);
    SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATH_RES);
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

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        struct Unit * unit = GetUnit(i);

        bool exec_buff = false;

        FORCE_DECLARE bool act_plus  = false;
        FORCE_DECLARE bool act_minus = false;

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

#if (defined(SID_Plus) && (COMMON_SKILL_VALID(SID_Plus)))
        if (!SkillTester(unit, SID_Plus))
            act_plus = true;
#endif

#if (defined(SID_Minus) && (COMMON_SKILL_VALID(SID_Minus)))
        if (!SkillTester(unit, SID_Minus))
            act_minus = true;
#endif

        if (!act_minus && !act_plus)
            continue;

        for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++)
        {
            int _x = unit->xPos + gVecs_1x1[j].x;
            int _y = unit->yPos + gVecs_1x1[j].y;

            struct Unit * unit_ally = GetUnitAtPosition(_x, _y);
            if (!UNIT_IS_VALID(unit_ally))
                continue;

            if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            if (!AreUnitsAllied(unit->index, unit_ally->index))
                continue;

            if
            (
#if (defined(SID_Plus) && (COMMON_SKILL_VALID(SID_Plus)))
                (SkillTester(unit_ally, SID_Plus) && act_plus)
#else
                false
#endif
                ||
#if (defined(SID_Minus) && (COMMON_SKILL_VALID(SID_Minus)))
                (SkillTester(unit_ally, SID_Minus) && act_minus)
#else
                false
#endif
            )
            {
                exec_buff = true;
                _SetPlusAndMinusStatDebuff(unit_ally);
            }
        }

        if (!exec_buff)
            _ClearPlusAndMinusStatDebuff(unit);
        else
            _SetPlusAndMinusStatDebuff(unit);
    }
    return false;
}
