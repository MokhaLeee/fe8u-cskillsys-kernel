#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "rng.h"

#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"
#include "rn.h"
#include "bwl.h"
#include "skill-system.h"

STATIC_DECLAR int GetStatIncreaseRandC(int growth, int amount)
{
    int result = 0;

    while (growth > 100)
    {
        result++;
        growth -= 100;
    }

    if (Roll1RandC(growth, amount * 4))
        result++;

    return result;
}

/* LynJump */
int GetStatIncrease(int growth)
{
    int result = 0;

    while (growth > 100)
    {
        result++;
        growth -= 100;
    }

    if (Roll1RN(growth))
        result++;

    return result;
}

STATIC_DECLAR void UnitLvupCore(struct BattleUnit * bu, int bonus)
{
    struct Unit * unit = GetUnit(bu->unit.index);
    int total;

    if (gPlaySt.config.controller || (gPlaySt.chapterStateBits & PLAY_FLAG_HARD))
    {
        /* Use switch style lvup on default */
        int randc_ref = 10 + 5 * UNIT_CHAR_ID(unit) + 4 * UNIT_CLASS_ID(unit) + 6 * unit->level;

        bu->changeHP  = GetStatIncreaseRandC(GetUnitHpGrowth(unit)  + bonus, randc_ref++);
        bu->changePow = GetStatIncreaseRandC(GetUnitPowGrowth(unit) + bonus, randc_ref++);
        bu->changeSkl = GetStatIncreaseRandC(GetUnitSklGrowth(unit) + bonus, randc_ref++);
        bu->changeSpd = GetStatIncreaseRandC(GetUnitSpdGrowth(unit) + bonus, randc_ref++);
        bu->changeLck = GetStatIncreaseRandC(GetUnitLckGrowth(unit) + bonus, randc_ref++);
        bu->changeDef = GetStatIncreaseRandC(GetUnitDefGrowth(unit) + bonus, randc_ref++);
        bu->changeRes = GetStatIncreaseRandC(GetUnitResGrowth(unit) + bonus, randc_ref++);

        BU_CHG_MAG(bu) = GetStatIncreaseRandC(GetUnitMagGrowth(unit) + bonus, randc_ref++);
    }
    else
    {
        /* for tutorial mode, use vanilla */
        bu->changeHP  = GetStatIncrease(GetUnitHpGrowth(unit)  + bonus);
        bu->changePow = GetStatIncrease(GetUnitPowGrowth(unit) + bonus);
        bu->changeSkl = GetStatIncrease(GetUnitSklGrowth(unit) + bonus);
        bu->changeSpd = GetStatIncrease(GetUnitSpdGrowth(unit) + bonus);
        bu->changeLck = GetStatIncrease(GetUnitLckGrowth(unit) + bonus);
        bu->changeDef = GetStatIncrease(GetUnitDefGrowth(unit) + bonus);
        bu->changeRes = GetStatIncrease(GetUnitResGrowth(unit) + bonus);

        BU_CHG_MAG(bu) = GetStatIncrease(GetUnitMagGrowth(unit) + bonus);
    }

    total = bu->changeHP +
            bu->changePow +
            bu->changeSkl +
            bu->changeSpd +
            bu->changeLck +
            bu->changeDef +
            bu->changeRes +
            BU_CHG_MAG(bu);

    if (total == 0)
        UnitLvupCore(bu, bonus + 5);
}

/* LynJump */
void CheckBattleUnitLevelUp(struct BattleUnit * bu)
{
    if (CanBattleUnitGainLevels(bu) && bu->unit.exp >= 100)
    {
        int bonus = (bu->unit.state & US_GROWTH_BOOST) ? 5: 0;

        bu->unit.exp -= 100;
        bu->unit.level++;

        if (UNIT_CATTRIBUTES(&bu->unit) & CA_MAXLEVEL10)
        {
            if (bu->unit.level == 10)
            {
                bu->expGain -= bu->unit.exp;
                bu->unit.exp = UNIT_EXP_DISABLED;
            }
        }
        else if (UNIT_LEVEL_MAX_RE == bu->unit.level ||
                 UNIT_RECORDED_LEVEL_MAX == (bu->unit.level + GetUnitHiddenLevel(&bu->unit)))
        {
            bu->expGain -= bu->unit.exp;
            bu->unit.exp = UNIT_EXP_DISABLED;
        }

        TryAddSkillLvup(GetUnitFromCharIdAndFaction(UNIT_CHAR_ID(&bu->unit), FACTION_BLUE), bu->unit.level);
        UnitLvupCore(bu, bonus);
    }
}
