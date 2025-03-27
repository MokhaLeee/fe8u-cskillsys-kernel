#include <common-chax.h>
#include <lvup.h>

int GetUnitHpGrowthJobBonus(int status, struct Unit *unit)
{
	// W.I.P.
	return status;
}

int GetUnitPowGrowthJobBonus(int status, struct Unit *unit)
{
	return status + gpJobGrowthList[UNIT_CLASS_ID(unit)].st[UNIT_STATUS_POW];
}

int GetUnitMagGrowthJobBonus(int status, struct Unit *unit)
{
	return status + gpJobGrowthList[UNIT_CLASS_ID(unit)].st[UNIT_STATUS_MAG];
}

int GetUnitSklGrowthJobBonus(int status, struct Unit *unit)
{
	return status + gpJobGrowthList[UNIT_CLASS_ID(unit)].st[UNIT_STATUS_SKL];
}

int GetUnitSpdGrowthJobBonus(int status, struct Unit *unit)
{
	return status + gpJobGrowthList[UNIT_CLASS_ID(unit)].st[UNIT_STATUS_SPD];
}

int GetUnitLckGrowthJobBonus(int status, struct Unit *unit)
{
	return status + gpJobGrowthList[UNIT_CLASS_ID(unit)].st[UNIT_STATUS_LCK];
}

int GetUnitDefGrowthJobBonus(int status, struct Unit *unit)
{
	return status + gpJobGrowthList[UNIT_CLASS_ID(unit)].st[UNIT_STATUS_DEF];
}

int GetUnitResGrowthJobBonus(int status, struct Unit *unit)
{
	return status + gpJobGrowthList[UNIT_CLASS_ID(unit)].st[UNIT_STATUS_RES];
}
