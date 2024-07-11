#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void _SetRouseStatDebuf(struct Unit * unit)
{
#if (defined(SID_RouseStrength) && COMMON_SKILL_VALID(SID_RouseStrength))
    if (SkillTester(unit, SID_RouseStrength))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ROUSE_POW);
#endif

#if (defined(SID_RouseMagic) && COMMON_SKILL_VALID(SID_RouseMagic))
    if (SkillTester(unit, SID_RouseMagic))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ROUSE_MAG);
#endif

#if (defined(SID_RouseSkill) && COMMON_SKILL_VALID(SID_RouseSkill))
    if (SkillTester(unit, SID_RouseSkill))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ROUSE_SKL);
#endif

#if (defined(SID_RouseSpeed) && COMMON_SKILL_VALID(SID_RouseSpeed))
    if (SkillTester(unit, SID_RouseSpeed))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ROUSE_SPD);
#endif

#if (defined(SID_RouseLuck) && COMMON_SKILL_VALID(SID_RouseLuck))
    if (SkillTester(unit, SID_RouseLuck))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ROUSE_LCK);
#endif

#if (defined(SID_RouseDefense) && COMMON_SKILL_VALID(SID_RouseDefense))
    if (SkillTester(unit, SID_RouseDefense))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ROUSE_DEF);
#endif

#if (defined(SID_RouseResistance) && COMMON_SKILL_VALID(SID_RouseResistance))
    if (SkillTester(unit, SID_RouseResistance))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ROUSE_RES);
#endif

#if (defined(SID_RouseMovement) && COMMON_SKILL_VALID(SID_RouseMovement))
    if (SkillTester(unit, SID_RouseMovement))
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ROUSE_MOV);
#endif
}

bool PrePhsae_TickRouseSkillStatus(ProcPtr proc)
{
    int i, j;

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        bool ally_in_range = false;

        struct Unit * unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit))
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

            if (AreUnitsAllied(unit->index, unit_ally->index))
            {
                ally_in_range = true;
                break;
            }
        }

        if (ally_in_range)
            _SetRouseStatDebuf(unit);
    }
    return false;
}
