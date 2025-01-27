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
        new = new + SKILL_EFF0(SID_Aptitude);
#endif

/* This must come last */
#if defined(SID_NecroCopy) && (COMMON_SKILL_VALID(SID_NecroCopy))
    if (SkillTester(unit, SID_NecroCopy))
        new = 0;
#endif

    return new;
}

int GetUnitHpGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitHpGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->maxHP / 2;
#endif

    return status;
}

int GetUnitPowGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitPowGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->pow;
#endif

    return status;
}

int GetUnitMagGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitMagGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + GetUnitMagic(unit);
#endif

    return status;
}

int GetUnitSklGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitSklGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->skl;
#endif

    return status;
}

int GetUnitSpdGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitSpdGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->spd;
#endif

    return status;
}

int GetUnitLckGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitLckGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->lck;
#endif

    return status;
}

int GetUnitDefGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitDefGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->def;
#endif

    return status;
}

int GetUnitResGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitResGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->def;
#endif

    return status;
}

/* Person based growth */
int GetUnitHpGrowth(struct Unit *unit)  { return GetUnitHpGrowthBonus(unit->pCharacterData->growthHP, unit); }
int GetUnitPowGrowth(struct Unit *unit) { return GetUnitPowGrowthBonus(unit->pCharacterData->growthPow, unit); }
int GetUnitMagGrowth(struct Unit *unit) { return GetUnitMagGrowthBonus(GetUnitBasicMagGrowth(unit), unit); }
int GetUnitSklGrowth(struct Unit *unit) { return GetUnitSklGrowthBonus(unit->pCharacterData->growthSkl, unit); }
int GetUnitSpdGrowth(struct Unit *unit) { return GetUnitSpdGrowthBonus(unit->pCharacterData->growthSpd, unit); }
int GetUnitLckGrowth(struct Unit *unit) { return GetUnitLckGrowthBonus(unit->pCharacterData->growthLck, unit); }
int GetUnitDefGrowth(struct Unit *unit) { return GetUnitDefGrowthBonus(unit->pCharacterData->growthDef, unit); }
int GetUnitResGrowth(struct Unit *unit) { return GetUnitResGrowthBonus(unit->pCharacterData->growthRes, unit); }

/* Job based growth */
int GetUnitJobBasedHpGrowth(struct Unit *unit)  { return GetUnitHpGrowthBonus(unit->pClassData->growthHP, unit); }
int GetUnitJobBasedPowGrowth(struct Unit *unit) { return GetUnitPowGrowthBonus(unit->pClassData->growthPow, unit); }
int GetUnitJobBasedMagGrowth(struct Unit *unit) { return GetUnitMagGrowthBonus(GetUnitJobBasedBasicMagGrowth(unit), unit); }
int GetUnitJobBasedSklGrowth(struct Unit *unit) { return GetUnitSklGrowthBonus(unit->pClassData->growthSkl, unit); }
int GetUnitJobBasedSpdGrowth(struct Unit *unit) { return GetUnitSpdGrowthBonus(unit->pClassData->growthSpd, unit); }
int GetUnitJobBasedLckGrowth(struct Unit *unit) { return GetUnitLckGrowthBonus(unit->pClassData->growthLck, unit); }
int GetUnitJobBasedDefGrowth(struct Unit *unit) { return GetUnitDefGrowthBonus(unit->pClassData->growthDef, unit); }
int GetUnitJobBasedResGrowth(struct Unit *unit) { return GetUnitResGrowthBonus(unit->pClassData->growthRes, unit); }