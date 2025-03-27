#include <common-chax.h>
#include <strmag.h>
#include <lvup.h>

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
