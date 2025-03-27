#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"

STATIC_DECLAR void CheckBattleUnitStatCapsVanilla(struct Unit *unit, struct BattleUnit *bu)
{
	if ((unit->maxHP + bu->changeHP) > GetUnitMaxStatusHp(unit))
		bu->changeHP = GetUnitMaxStatusHp(unit) - unit->maxHP;

	if ((unit->pow + bu->changePow) > GetUnitMaxStatusPow(unit))
		bu->changePow = GetUnitMaxStatusPow(unit) - unit->pow;

	if ((unit->skl + bu->changeSkl) > GetUnitMaxStatusSkl(unit))
		bu->changeSkl = GetUnitMaxStatusSkl(unit) - unit->skl;

	if ((unit->spd + bu->changeSpd) > GetUnitMaxStatusSpd(unit))
		bu->changeSpd = GetUnitMaxStatusSpd(unit) - unit->spd;

	if ((unit->def + bu->changeDef) > GetUnitMaxStatusDef(unit))
		bu->changeDef = GetUnitMaxStatusDef(unit) - unit->def;

	if ((unit->res + bu->changeRes) > GetUnitMaxStatusRes(unit))
		bu->changeRes = GetUnitMaxStatusRes(unit) - unit->res;

	if ((unit->lck + bu->changeLck) > GetUnitMaxStatusLck(unit))
		bu->changeLck = GetUnitMaxStatusLck(unit) - unit->lck;
}

STATIC_DECLAR void UnitCheckStatCapsVanilla(struct Unit *unit)
{
	if (unit->maxHP > GetUnitMaxStatusHp(unit))
		unit->maxHP = GetUnitMaxStatusHp(unit);

	if (unit->pow > GetUnitMaxStatusPow(unit))
		unit->pow = GetUnitMaxStatusPow(unit);

	if (unit->skl > GetUnitMaxStatusSkl(unit))
		unit->skl = GetUnitMaxStatusSkl(unit);

	if (unit->spd > GetUnitMaxStatusSpd(unit))
		unit->spd = GetUnitMaxStatusSpd(unit);

	if (unit->def > GetUnitMaxStatusDef(unit))
		unit->def = GetUnitMaxStatusDef(unit);

	if (unit->res > GetUnitMaxStatusRes(unit))
		unit->res = GetUnitMaxStatusRes(unit);

	if (unit->lck > GetUnitMaxStatusLck(unit))
		unit->lck = GetUnitMaxStatusLck(unit);

	if (unit->conBonus > (GetUnitMaxStatusCon(unit) - UNIT_CON_BASE(unit)))
		unit->conBonus = (GetUnitMaxStatusCon(unit) - UNIT_CON_BASE(unit));

	if (unit->movBonus > (GetUnitMaxStatusMov(unit) - UNIT_MOV_BASE(unit)))
		unit->movBonus = (GetUnitMaxStatusMov(unit) - UNIT_MOV_BASE(unit));
}

LYN_REPLACE_CHECK(CheckBattleUnitStatCaps);
void CheckBattleUnitStatCaps(struct Unit *unit, struct BattleUnit *bu)
{
	CheckBattleUnitStatCapsVanilla(unit, bu);

	/* Hooks */
	if ((UNIT_MAG(unit) + BU_CHG_MAG(bu)) > GetUnitMaxStatusMag(unit))
		BU_CHG_MAG(bu) = GetUnitMaxStatusMag(unit) - UNIT_MAG(unit);
}

#if 0
LYN_UNUSED_REPLACE_CHECK(UnitCheckStatCaps);
void UnitCheckStatCaps(struct Unit *unit)
#else
/* External hook to save spaces */
void _UnitCheckStatCaps(struct Unit *unit)
#endif
{
	UnitCheckStatCapsVanilla(unit);

	/* Hooks */
	if (UNIT_MAG(unit) > GetUnitMaxStatusMag(unit))
		UNIT_MAG(unit) = GetUnitMaxStatusMag(unit);
}
