#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void _SetMoodyStatDebuff(struct Unit *unit)
{
    int buff = NextRN_N(8);
    int debuff;

    do
    {
        debuff = NextRN_N(8);
    } while (debuff == buff);

    switch (buff)
    {
    case 0:
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_MOODY_POW);
        break;
    case 1:
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_MOODY_MAG);
        break;
    case 2:
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_MOODY_SKL);
        break;
    case 3:
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_MOODY_SPD);
        break;
    case 4:
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_MOODY_LCK);
        break;
    case 5:
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_MOODY_DEF);
        break;
    case 6:
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_MOODY_RES);
        break;
    case 7:
        SetUnitStatDebuff(unit, UNIT_STAT_BUFF_MOODY_MOV);
        break;
    }

    switch (debuff)
    {
    case 0:
        SetUnitStatDebuff(unit, UNIT_STAT_DEBUFF_POW);
        break;
    case 1:
        SetUnitStatDebuff(unit, UNIT_STAT_DEBUFF_MAG);
        break;
    case 2:
        SetUnitStatDebuff(unit, UNIT_STAT_DEBUFF_SKL);
        break;
    case 3:
        SetUnitStatDebuff(unit, UNIT_STAT_DEBUFF_SPD);
        break;
    case 4:
        SetUnitStatDebuff(unit, UNIT_STAT_DEBUFF_LCK);
        break;
    case 5:
        SetUnitStatDebuff(unit, UNIT_STAT_DEBUFF_DEF);
        break;
    case 6:
        SetUnitStatDebuff(unit, UNIT_STAT_DEBUFF_RES);
        break;
    case 7:
        SetUnitStatDebuff(unit, UNIT_STAT_DEBUFF_MOV);
        break;
    }
}

bool PrePhase_TickMoodySkillStatus(ProcPtr proc)
{
    int i;

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        struct Unit *unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

#if (defined(SID_Moody) && COMMON_SKILL_VALID(SID_Moody))
        if (SkillTester(unit, SID_Moody))
            _SetMoodyStatDebuff(unit);
#endif
    }
    return false;
}
