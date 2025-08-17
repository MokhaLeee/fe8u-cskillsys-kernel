#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"

#ifndef INT8_MAX
#define INT8_MAX 127
#endif

LYN_REPLACE_CHECK(UnitAutolevelRealistic);
void UnitAutolevelRealistic(struct Unit *unit)
{
	int lvup_level = unit->level - unit->pCharacterData->baseLevel;

	unit->maxHP += GetAutoleveledStatIncrease(GetUnitHpGrowth(unit), lvup_level);
	unit->pow   += GetAutoleveledStatIncrease(GetUnitPowGrowth(unit), lvup_level);
	unit->skl   += GetAutoleveledStatIncrease(GetUnitSklGrowth(unit), lvup_level);
	unit->spd   += GetAutoleveledStatIncrease(GetUnitSpdGrowth(unit), lvup_level);
	unit->lck   += GetAutoleveledStatIncrease(GetUnitLckGrowth(unit), lvup_level);
	unit->def   += GetAutoleveledStatIncrease(GetUnitDefGrowth(unit), lvup_level);
	unit->res   += GetAutoleveledStatIncrease(GetUnitResGrowth(unit), lvup_level);

	UNIT_MAG(unit) += GetAutoleveledStatIncrease(GetUnitMagGrowth(unit), lvup_level);

	UnitCheckStatCaps(unit);
	unit->curHP = GetUnitMaxHp(unit);
}

LYN_REPLACE_CHECK(UnitAutolevelCore);
void UnitAutolevelCore(struct Unit *unit, u8 classId, int levelCount)
{
	if (levelCount) {
		int mhp = unit->maxHP + GetAutoleveledStatIncrease(unit->pClassData->growthHP,  levelCount);
		int pow = unit->pow + GetAutoleveledStatIncrease(unit->pClassData->growthPow, levelCount);
		int skl = unit->skl + GetAutoleveledStatIncrease(unit->pClassData->growthSkl, levelCount);
		int spd = unit->spd + GetAutoleveledStatIncrease(unit->pClassData->growthSpd, levelCount);
		int def = unit->def + GetAutoleveledStatIncrease(unit->pClassData->growthDef, levelCount);
		int res = unit->res + GetAutoleveledStatIncrease(unit->pClassData->growthRes, levelCount);
		int lck = unit->lck + GetAutoleveledStatIncrease(unit->pClassData->growthLck, levelCount);
		int mag = UNIT_MAG(unit) + GetAutoleveledStatIncrease(GetUnitJobBasedBasicMagGrowth(unit), levelCount);

		if (mhp > INT8_MAX)
			mhp = INT8_MAX;

		if (pow > INT8_MAX)
			pow = INT8_MAX;

		if (skl > INT8_MAX)
			skl = INT8_MAX;

		if (spd > INT8_MAX)
			spd = INT8_MAX;

		if (lck > INT8_MAX)
			lck = INT8_MAX;

		if (def > INT8_MAX)
			def = INT8_MAX;

		if (res > INT8_MAX)
			res = INT8_MAX;

		if (mag > INT8_MAX)
			mag = INT8_MAX;

		unit->maxHP = mhp;
		unit->pow = pow;
		unit->skl = skl;
		unit->spd = spd;
		unit->lck = lck;
		unit->def = def;
		unit->res = res;
		UNIT_MAG(unit) = mag;
	}
}

LYN_REPLACE_CHECK(UnitAutolevelPenalty);
void UnitAutolevelPenalty(struct Unit *unit, u8 classId, int levelCount)
{
	int level = unit->level;

	if (levelCount && level > unit->pCharacterData->baseLevel) {
		levelCount = level - levelCount;

		unit->maxHP = unit->pCharacterData->baseHP  + unit->pClassData->baseHP;
		unit->pow   = unit->pCharacterData->basePow + unit->pClassData->basePow;
		unit->skl   = unit->pCharacterData->baseSkl + unit->pClassData->baseSkl;
		unit->spd   = unit->pCharacterData->baseSpd + unit->pClassData->baseSpd;
		unit->def   = unit->pCharacterData->baseDef + unit->pClassData->baseDef;
		unit->res   = unit->pCharacterData->baseRes + unit->pClassData->baseRes;
		unit->lck   = unit->pCharacterData->baseLck;

		/* Hook here */
		UNIT_MAG(unit) = GetUnitBaseMagic(unit);

		if (levelCount > unit->pCharacterData->baseLevel) {
			unit->level = levelCount;
			UnitAutolevel(unit);
			unit->level = level;
		}
	}
}
