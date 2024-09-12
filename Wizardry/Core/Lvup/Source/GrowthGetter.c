#include "common-chax.h"
#include "lvup.h"
#include "skill-system.h"
#include "strmag.h"
#include "constants/skills.h"

STATIC_DECLAR int GetUnitCommonGrowthBonus(int status, struct Unit *unit)
{
    int new = status;

#if defined(SID_Blossom) && (COMMON_SKILL_VALID(SID_Blossom))
    if (SkillTester(unit, SID_Blossom))
        new = new + status * 2;
#endif

#if defined(SID_Aptitude) && (COMMON_SKILL_VALID(SID_Aptitude))
    if (SkillTester(unit, SID_Aptitude))
        new = new + Div(status * SKILL_EFF0(SID_Aptitude), 100);
#endif

    return new;
}

int GetUnitHpGrowth(struct Unit *unit)
{
    int status = unit->pCharacterData->growthHP;
    status = GetUnitCommonGrowthBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->maxHP / 2;
#endif

    return status;
}

int GetUnitPowGrowth(struct Unit *unit)
{
    int status = unit->pCharacterData->growthPow;
    status = GetUnitCommonGrowthBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->pow;
#endif

    return status;
}

int GetUnitMagGrowth(struct Unit *unit)
{
    int status = GetUnitBasicMagGrowth(unit);
    status = GetUnitCommonGrowthBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + GetUnitMagic(unit);
#endif

    return status;
}

int GetUnitSklGrowth(struct Unit *unit)
{
    int status = unit->pCharacterData->growthSkl;
    status = GetUnitCommonGrowthBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->skl;
#endif

    return status;
}

int GetUnitSpdGrowth(struct Unit *unit)
{
    int status = unit->pCharacterData->growthSpd;
    status = GetUnitCommonGrowthBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->spd;
#endif

    return status;
}

int GetUnitLckGrowth(struct Unit *unit)
{
    int status = unit->pCharacterData->growthLck;
    status = GetUnitCommonGrowthBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->lck;
#endif

    return status;
}

int GetUnitDefGrowth(struct Unit *unit)
{
    int status = unit->pCharacterData->growthDef;
    status = GetUnitCommonGrowthBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->def;
#endif

    return status;
}

int GetUnitResGrowth(struct Unit *unit)
{
    int status = unit->pCharacterData->growthRes;
    status = GetUnitCommonGrowthBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->def;
#endif

    return status;
}
