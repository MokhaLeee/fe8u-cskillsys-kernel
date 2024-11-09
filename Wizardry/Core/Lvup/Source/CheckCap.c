#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "kernel-lib.h"

STATIC_DECLAR void CheckBattleUnitStatCapsVanilla(struct Unit *unit, struct BattleUnit *bu)
{
    int limitBreaker = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        limitBreaker = SKILL_EFF0(SID_LimitBreakerPlus);
#endif

#if defined(SID_OgreBody) && (COMMON_SKILL_VALID(SID_OgreBody))
    if (SkillTester(unit, SID_OgreBody))
    {
        if (unit->maxHP > SKILL_EFF0(SID_OgreBody))
            unit->maxHP = SKILL_EFF0(SID_OgreBody);
    }
    else
    {
        if (unit->maxHP > UNIT_MHP_MAX(unit) + limitBreaker)
            unit->maxHP = UNIT_MHP_MAX(unit) + limitBreaker;
    }
#else
    #ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
        if (unit->maxHP > KUNIT_MHP_MAX(unit) + limitBreaker)
            unit->maxHP = KUNIT_MHP_MAX(unit) + limitBreaker;
    #else
        if (unit->maxHP > UNIT_MHP_MAX(unit) + limitBreaker)
            unit->maxHP = UNIT_MHP_MAX(unit) + limitBreaker;
    #endif
#endif

    if ((unit->pow + bu->changePow) > UNIT_POW_MAX(unit) + limitBreaker)
        bu->changePow = (UNIT_POW_MAX(unit)  + limitBreaker) - unit->pow;

    if ((unit->skl + bu->changeSkl) > UNIT_SKL_MAX(unit) + limitBreaker)
        bu->changeSkl = (UNIT_SKL_MAX(unit) + limitBreaker) - unit->skl;

    if ((unit->spd + bu->changeSpd) > UNIT_SPD_MAX(unit) + limitBreaker)
        bu->changeSpd = (UNIT_SPD_MAX(unit) + limitBreaker) - unit->spd;

    if ((unit->def + bu->changeDef) > UNIT_DEF_MAX(unit) + limitBreaker)
        bu->changeDef = (UNIT_DEF_MAX(unit) + limitBreaker) - unit->def;

    if ((unit->res + bu->changeRes) > UNIT_RES_MAX(unit) + limitBreaker)
        bu->changeRes = (UNIT_RES_MAX(unit) + limitBreaker) - unit->res;

    if ((unit->lck + bu->changeLck) > UNIT_LCK_MAX(unit) + limitBreaker)
        bu->changeLck = (UNIT_LCK_MAX(unit) + limitBreaker) - unit->lck;
}

STATIC_DECLAR void UnitCheckStatCapsVanilla(struct Unit *unit)
{
    int limitBreaker = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        limitBreaker = SKILL_EFF0(SID_LimitBreakerPlus);
#endif

#if defined(SID_OgreBody) && (COMMON_SKILL_VALID(SID_OgreBody))
    if (SkillTester(unit, SID_OgreBody))
    {
        if (unit->maxHP > SKILL_EFF0(SID_OgreBody))
            unit->maxHP = SKILL_EFF0(SID_OgreBody);
    }
    else
    {
        if (unit->maxHP > UNIT_MHP_MAX(unit) + limitBreaker)
            unit->maxHP = UNIT_MHP_MAX(unit) + limitBreaker;
    }
#else
    #ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
        if (unit->maxHP > KUNIT_MHP_MAX(unit) + limitBreaker)
            unit->maxHP = KUNIT_MHP_MAX(unit) + limitBreaker;
    #else
        if (unit->maxHP > UNIT_MHP_MAX(unit) + limitBreaker)
            unit->maxHP = UNIT_MHP_MAX(unit) + limitBreaker;
    #endif
#endif

    if (unit->pow > UNIT_POW_MAX(unit) + limitBreaker)
        unit->pow = UNIT_POW_MAX(unit) + limitBreaker;

    if (unit->skl > UNIT_SKL_MAX(unit) + limitBreaker)
        unit->skl = UNIT_SKL_MAX(unit) + limitBreaker;

    if (unit->spd > UNIT_SPD_MAX(unit) + limitBreaker)
        unit->spd = UNIT_SPD_MAX(unit) + limitBreaker;

    if (unit->def > UNIT_DEF_MAX(unit) + limitBreaker)
        unit->def = UNIT_DEF_MAX(unit) + limitBreaker;

    if (unit->res > UNIT_RES_MAX(unit) + limitBreaker)
        unit->res = UNIT_RES_MAX(unit) + limitBreaker;

    if (unit->lck > UNIT_LCK_MAX(unit) + limitBreaker)
        unit->lck = UNIT_LCK_MAX(unit) + limitBreaker;

    if (unit->conBonus > (UNIT_CON_MAX(unit) - UNIT_CON_BASE(unit)))
        unit->conBonus = (UNIT_CON_MAX(unit) - UNIT_CON_BASE(unit));

    if (unit->movBonus > (UNIT_MOV_MAX(unit) - UNIT_MOV_BASE(unit)))
        unit->movBonus = (UNIT_MOV_MAX(unit) - UNIT_MOV_BASE(unit));
}

LYN_REPLACE_CHECK(CheckBattleUnitStatCaps);
void CheckBattleUnitStatCaps(struct Unit *unit, struct BattleUnit *bu)
{
    CheckBattleUnitStatCapsVanilla(unit, bu);

    int limitBreaker = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        limitBreaker = SKILL_EFF0(SID_LimitBreakerPlus);
#endif

    /* Hooks */
    if ((UNIT_MAG(unit) + BU_CHG_MAG(bu)) > GetUnitMaxMagic(unit) + limitBreaker)
        BU_CHG_MAG(bu) = (GetUnitMaxMagic(unit) + limitBreaker) - UNIT_MAG(unit);
}

#if 0
LYN_UNUSED_REPLACE_CHECK(UnitCheckStatCaps);
void UnitCheckStatCaps(struct Unit * unit)
#else
/* External hook to save spaces */
void _UnitCheckStatCaps(struct Unit * unit)
#endif
{
    UnitCheckStatCapsVanilla(unit);

    int limitBreaker = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

#if defined(SID_LimitBreakerPlus) && (COMMON_SKILL_VALID(SID_LimitBreakerPlus))
    if (SkillTester(unit, SID_LimitBreakerPlus))
        limitBreaker = SKILL_EFF0(SID_LimitBreakerPlus);
#endif

    /* Hooks */
    if (UNIT_MAG(unit) > GetUnitMaxMagic(unit) + limitBreaker)
        UNIT_MAG(unit) = GetUnitMaxMagic(unit) + limitBreaker;
}
