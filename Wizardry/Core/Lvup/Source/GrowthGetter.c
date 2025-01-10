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

int GetUnitHpGrowthBonus(struct Unit *unit)
{
	int status = 0;

	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitHpGrowthJobBonus(status, unit);
	return status;
}

int GetUnitPowGrowthBonus(struct Unit *unit)
{
	int status = 0;

	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitPowGrowthJobBonus(status, unit);
	return status;
}

int GetUnitMagGrowthBonus(struct Unit *unit)
{
	int status = 0;

	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitMagGrowthJobBonus(status, unit);
	return status;
}

int GetUnitSklGrowthBonus(struct Unit *unit)
{
	int status = 0;

	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitSklGrowthJobBonus(status, unit);
	return status;
}

int GetUnitSpdGrowthBonus(struct Unit *unit)
{
	int status = 0;

	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitSpdGrowthJobBonus(status, unit);
	return status;
}

int GetUnitLckGrowthBonus(struct Unit *unit)
{
	int status = 0;

	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitLckGrowthJobBonus(status, unit);
	return status;
}

int GetUnitDefGrowthBonus(struct Unit *unit)
{
	int status = 0;

	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitDefGrowthJobBonus(status, unit);
	return status;
}

int GetUnitResGrowthBonus(struct Unit *unit)
{
	int status = 0;

	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitResGrowthJobBonus(status, unit);
	return status;
}

/* Person based growth */
int GetUnitHpGrowth(struct Unit *unit)  { return unit->pCharacterData->growthHP  + GetUnitHpGrowthBonus(unit); }
int GetUnitPowGrowth(struct Unit *unit) { return unit->pCharacterData->growthPow + GetUnitPowGrowthBonus(unit); }
int GetUnitMagGrowth(struct Unit *unit) { return GetUnitBasicMagGrowth(unit)     + GetUnitMagGrowthBonus(unit); }
int GetUnitSklGrowth(struct Unit *unit) { return unit->pCharacterData->growthSkl + GetUnitSklGrowthBonus(unit); }
int GetUnitSpdGrowth(struct Unit *unit) { return unit->pCharacterData->growthSpd + GetUnitSpdGrowthBonus(unit); }
int GetUnitLckGrowth(struct Unit *unit) { return unit->pCharacterData->growthLck + GetUnitLckGrowthBonus(unit); }
int GetUnitDefGrowth(struct Unit *unit) { return unit->pCharacterData->growthDef + GetUnitDefGrowthBonus(unit); }
int GetUnitResGrowth(struct Unit *unit) { return unit->pCharacterData->growthRes + GetUnitResGrowthBonus(unit); }

/* Job based growth */
int GetUnitJobBasedHpGrowth(struct Unit *unit)  { return unit->pClassData->growthHP  + GetUnitHpGrowthBonus(unit); }
int GetUnitJobBasedPowGrowth(struct Unit *unit) { return unit->pClassData->growthPow + GetUnitPowGrowthBonus(unit); }
int GetUnitJobBasedMagGrowth(struct Unit *unit) { return GetUnitJobBasedBasicMagGrowth(unit) + GetUnitMagGrowthBonus(unit); }
int GetUnitJobBasedSklGrowth(struct Unit *unit) { return unit->pClassData->growthSkl + GetUnitSklGrowthBonus(unit); }
int GetUnitJobBasedSpdGrowth(struct Unit *unit) { return unit->pClassData->growthSpd + GetUnitSpdGrowthBonus(unit); }
int GetUnitJobBasedLckGrowth(struct Unit *unit) { return unit->pClassData->growthLck + GetUnitLckGrowthBonus(unit); }
int GetUnitJobBasedDefGrowth(struct Unit *unit) { return unit->pClassData->growthDef + GetUnitDefGrowthBonus(unit); }
int GetUnitJobBasedResGrowth(struct Unit *unit) { return unit->pClassData->growthRes + GetUnitResGrowthBonus(unit); }
