#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"
#include "rn.h"
#include "bwl.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

STATIC_DECLAR int GetStatIncreaseRandC(int growth)
{
    int result = 0;

    while (growth > 100)
    {
        result++;
        growth -= 100;
    }

    if (Roll1RandC(growth))
        result++;

    return result;
}

static int GetStatIncreaseFixed(int growth, int ref)
{
    return simple_div(growth + simple_mod(growth * ref, 100), 100);
}

static void UnitLvup_Vanilla(struct BattleUnit * bu, int bonus)
{
    struct Unit * unit = GetUnit(bu->unit.index);
    int statCounter = 0;
    int limitBreaker = 0;
    FORCE_DECLARE bool tripleUpExecuted = false;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        limitBreaker = SKILL_EFF0(SID_LimitBreakerPlus);
#endif

    // Create an array of stat pointers
    s8 * statChanges[] = { &bu->changeHP,  &bu->changePow, &bu->changeSkl, &bu->changeSpd,
                           &bu->changeLck, &bu->changeDef, &bu->changeRes, &BU_CHG_MAG(bu) };

    /*
    ** Right now, if a unit has over a 100% growth and gets more than one
    ** stat from it when they're only 1 point away from their cap, the
    ** level up screen will still show the bonus stats.
    ** I could fix it, but it'd just make this function even chunkier.
    ** If there's a smarter way to write all this, please do.
    */
    if (unit->maxHP < unit->pClassData->maxHP + limitBreaker)
        *statChanges[0] = GetStatIncrease(GetUnitHpGrowth(unit) + bonus);
    if (unit->pow < unit->pClassData->maxPow + limitBreaker)
        *statChanges[1] = GetStatIncrease(GetUnitPowGrowth(unit) + bonus);
    if (unit->skl < unit->pClassData->maxSkl + limitBreaker)
        *statChanges[2] = GetStatIncrease(GetUnitSklGrowth(unit) + bonus);
    if (unit->spd < unit->pClassData->maxSpd + limitBreaker)
        *statChanges[3] = GetStatIncrease(GetUnitSpdGrowth(unit) + bonus);
    if (unit->lck < 30 + limitBreaker) // subject to change
        *statChanges[4] = GetStatIncrease(GetUnitLckGrowth(unit) + bonus);
    if (unit->def < unit->pClassData->maxDef + limitBreaker)
        *statChanges[5] = GetStatIncrease(GetUnitDefGrowth(unit) + bonus);
    if (unit->res < unit->pClassData->maxRes + limitBreaker)
        *statChanges[6] = GetStatIncrease(GetUnitResGrowth(unit) + bonus);
    if (GetUnitMagic(unit) < GetUnitMaxMagic(unit) + limitBreaker)
        *statChanges[7] = GetStatIncrease(GetUnitMagGrowth(unit) + bonus);

    // For each increased stat, increment statCounter
    statCounter += *statChanges[0] > 0 ? 1 : 0;
    statCounter += *statChanges[1] > 0 ? 1 : 0;
    statCounter += *statChanges[2] > 0 ? 1 : 0;
    statCounter += *statChanges[3] > 0 ? 1 : 0;
    statCounter += *statChanges[4] > 0 ? 1 : 0;
    statCounter += *statChanges[5] > 0 ? 1 : 0;
    statCounter += *statChanges[6] > 0 ? 1 : 0;
    statCounter += *statChanges[7] > 0 ? 1 : 0;

#if (defined(SID_TripleUp) && (COMMON_SKILL_VALID(SID_TripleUp)))
    if (BattleSkillTester(bu, SID_TripleUp))
    {
        // Check if any values are greater than 0
        int anyStatIncrease = 0; // Flag to track if there's any stats to increase
        for (u8 i = 0; i < ARRAY_COUNT(statChanges); i++)
        {
            if (*statChanges[i] == 0)
            {
                anyStatIncrease = 1; // Set the flag if any stat is greater than 0
                break;               // No need to continue checking once we find a positive value
            }
        }

        if (anyStatIncrease)
        {
            // Set the upper limit of stats to increase (accounting for previous increases before this skill)
            while (statCounter < 3)
            {
                // Get a random index using NextRN_N and ARRAY_COUNT
                int randIndex = NextRN_N(ARRAY_COUNT(statChanges));

                // If the randomly chosen stat hasn't increased, force it to increase
                if (*statChanges[randIndex] == 0)
                {
                    *statChanges[randIndex] = 1;
                    statCounter++;
                }
            }
            tripleUpExecuted = true;
        }
    }
#endif

#if (defined(SID_DoubleUp) && (COMMON_SKILL_VALID(SID_DoubleUp)))
    if (BattleSkillTester(bu, SID_DoubleUp) && !tripleUpExecuted)
    {
        // Check if any values are greater than 0
        int anyStatIncrease = 0; // Flag to track if there's any stat greater than 0
        for (u8 i = 0; i < ARRAY_COUNT(statChanges); i++)
        {
            if (*statChanges[i] > 0)
            {
                anyStatIncrease = 1; // Set the flag if any stat is greater than 0
                break;               // No need to continue checking once we find a positive value
            }
        }

        if (anyStatIncrease)
        {
            // Set the upper limit of stats to increase (accounting for previous increases before this skill)
            while (statCounter < 2)
            {
                // Get a random index using NextRN_N and ARRAY_COUNT
                int randIndex = NextRN_N(ARRAY_COUNT(statChanges));

                // If the randomly chosen stat hasn't increased, force it to increase
                if (*statChanges[randIndex] == 0)
                {
                    *statChanges[randIndex] = 1;
                    statCounter++;
                }
            }
        }
    }
#endif

#if (defined(SID_Mercurious) && (COMMON_SKILL_VALID(SID_Mercurious)))
    if (BattleSkillTester(bu, SID_Mercurious) && Roll1RN(SKILL_EFF0(SID_Mercurious)))
        for (u8 i = 0; i < ARRAY_COUNT(statChanges); i++)
            *statChanges[i] *= SKILL_EFF1(SID_Mercurious);
#endif

#if (defined(SID_Velocity) && (COMMON_SKILL_VALID(SID_Velocity)))
    if (BattleSkillTester(bu, SID_Velocity) && Roll1RN(SKILL_EFF0(SID_Velocity)))
        GetUnit(bu->unit.index)->movBonus += 1;
#endif
}

static void UnitLvup_RandC(struct BattleUnit * bu, int bonus)
{
    struct Unit * unit = GetUnit(bu->unit.index);

    bu->changeHP = GetStatIncreaseRandC(GetUnitHpGrowth(unit) + bonus);
    bu->changePow = GetStatIncreaseRandC(GetUnitPowGrowth(unit) + bonus);
    bu->changeSkl = GetStatIncreaseRandC(GetUnitSklGrowth(unit) + bonus);
    bu->changeSpd = GetStatIncreaseRandC(GetUnitSpdGrowth(unit) + bonus);
    bu->changeLck = GetStatIncreaseRandC(GetUnitLckGrowth(unit) + bonus);
    bu->changeDef = GetStatIncreaseRandC(GetUnitDefGrowth(unit) + bonus);
    bu->changeRes = GetStatIncreaseRandC(GetUnitResGrowth(unit) + bonus);

    BU_CHG_MAG(bu) = GetStatIncreaseRandC(GetUnitMagGrowth(unit) + bonus);
}

static void UnitLvup_Fixed(struct BattleUnit * bu, int bonus)
{
    struct Unit * unit = GetUnit(bu->unit.index);

    int ref = unit->level - 1;
    if (CA_PROMOTED & UNIT_CATTRIBUTES(unit))
        ref = ref + 19;

    bu->changeHP = GetStatIncreaseFixed(GetUnitHpGrowth(unit) + bonus, ref += 5);
    bu->changePow = GetStatIncreaseFixed(GetUnitPowGrowth(unit) + bonus, ref += 5);
    bu->changeSkl = GetStatIncreaseFixed(GetUnitSklGrowth(unit) + bonus, ref += 5);
    bu->changeSpd = GetStatIncreaseFixed(GetUnitSpdGrowth(unit) + bonus, ref += 5);
    bu->changeLck = GetStatIncreaseFixed(GetUnitLckGrowth(unit) + bonus, ref += 5);
    bu->changeDef = GetStatIncreaseFixed(GetUnitDefGrowth(unit) + bonus, ref += 5);
    bu->changeRes = GetStatIncreaseFixed(GetUnitResGrowth(unit) + bonus, ref += 5);

    BU_CHG_MAG(bu) = GetStatIncreaseFixed(GetUnitMagGrowth(unit) + bonus, ref += 5);
}

static void UnitLvup_100(struct BattleUnit * bu, int bonus)
{
    bu->changeHP = 1;
    bu->changePow = 1;
    bu->changeSkl = 1;
    bu->changeSpd = 1;
    bu->changeLck = 1;
    bu->changeDef = 1;
    bu->changeRes = 1;
    BU_CHG_MAG(bu) = 1;
}

static void UnitLvup_0(struct BattleUnit * bu, int bonus)
{
    return;
}

STATIC_DECLAR void UnitLvupCore(struct BattleUnit * bu, int bonus)
{
    static void (*const funcs[])(struct BattleUnit * bu, int bonus) = {
        [0] = UnitLvup_Vanilla, [1] = UnitLvup_RandC, [2] = UnitLvup_Fixed, [3] = UnitLvup_100, [4] = UnitLvup_0
    };

    int mode;

    if (TUTORIAL_MODE())
        mode = gpKernelDesigerConfig->lvup_mode_tutorial;
    if (gPlaySt.config.controller || (gPlaySt.chapterStateBits & PLAY_FLAG_HARD))
        mode = gpKernelDesigerConfig->lvup_mode_hard;
    else
        mode = gpKernelDesigerConfig->lvup_mode_normal;

    if (mode > 4)
        mode = 0;

    funcs[mode](bu, bonus);

    if (gpKernelDesigerConfig->guaranteed_lvup)
    {
        int total = bu->changeHP + bu->changePow + bu->changeSkl + bu->changeSpd + bu->changeLck + bu->changeDef +
            bu->changeRes + BU_CHG_MAG(bu);

        if (total == 0)
            UnitLvupCore(bu, bonus + 5);
    }
}

static inline int get_metis_tome_growth_bonus(void)
{
    extern const u8 MetisTomeGrowthBonus;
    return MetisTomeGrowthBonus;
}

LYN_REPLACE_CHECK(CheckBattleUnitLevelUp);
void CheckBattleUnitLevelUp(struct BattleUnit * bu)
{
    if (CanBattleUnitGainLevels(bu) && bu->unit.exp >= 100)
    {
        int bonus = (bu->unit.state & US_GROWTH_BOOST) ? get_metis_tome_growth_bonus() : 0;

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
        else if (
            UNIT_LEVEL_MAX_RE == bu->unit.level ||
            UNIT_RECORDED_LEVEL_MAX == (bu->unit.level + GetUnitHiddenLevel(&bu->unit)))
        {
            bu->expGain -= bu->unit.exp;
            bu->unit.exp = UNIT_EXP_DISABLED;
        }

        TryAddSkillLvup(GetUnitFromCharIdAndFaction(UNIT_CHAR_ID(&bu->unit), FACTION_BLUE), bu->unit.level);
        UnitLvupCore(bu, bonus);
    }
}