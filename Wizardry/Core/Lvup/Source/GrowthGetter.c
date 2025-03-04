#include "common-chax.h"
#include "lvup.h"
#include "skill-system.h"
#include "strmag.h"
#include "shield.h"
#include "constants/skills.h"

STATIC_DECLAR int GetUnitCommonGrowthBonus(int status, struct Unit *unit)
{
	int new = status;

#if defined(SID_Blossom) && (COMMON_SKILL_VALID(SID_Blossom))
	if (SkillTester(unit, SID_Blossom))
		new = new + status;
#endif

#if defined(SID_Aptitude) && (COMMON_SKILL_VALID(SID_Aptitude))
	if (SkillTester(unit, SID_Aptitude))
		new = new + SKILL_EFF0(SID_Aptitude);
#endif

	return new;
}

int GetUnitHpGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitHpGrowthJobBonus(status, unit);
	return status;
}

int GetUnitPowGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitPowGrowthJobBonus(status, unit);
	status = PowGrowthGetterShield(status, unit);
	return status;
}

int GetUnitMagGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitMagGrowthJobBonus(status, unit);
	status = MagGrowthGetterShield(status, unit);
	return status;
}

int GetUnitSklGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitSklGrowthJobBonus(status, unit);
	status = SklGrowthGetterShield(status, unit);
	return status;
}

int GetUnitSpdGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitSpdGrowthJobBonus(status, unit);
	status = SpdGrowthGetterShield(status, unit);
	return status;
}

int GetUnitLckGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitLckGrowthJobBonus(status, unit);
	status = LckGrowthGetterShield(status, unit);
	return status;
}

int GetUnitDefGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitDefGrowthJobBonus(status, unit);
	status = DefGrowthGetterShield(status, unit);
	return status;
}

int GetUnitResGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitResGrowthJobBonus(status, unit);
	status = ResGrowthGetterShield(status, unit);
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
