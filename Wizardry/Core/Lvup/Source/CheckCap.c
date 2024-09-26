#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"

STATIC_DECLAR void CheckBattleUnitStatCapsVanilla(struct Unit *unit, struct BattleUnit *bu)
{
	if ((unit->maxHP + bu->changeHP) > KUNIT_MHP_MAX(unit))
		bu->changeHP = KUNIT_MHP_MAX(unit) - unit->maxHP;

	if ((unit->pow + bu->changePow) > UNIT_POW_MAX(unit))
		bu->changePow = UNIT_POW_MAX(unit) - unit->pow;

	if ((unit->skl + bu->changeSkl) > UNIT_SKL_MAX(unit))
		bu->changeSkl = UNIT_SKL_MAX(unit) - unit->skl;

	if ((unit->spd + bu->changeSpd) > UNIT_SPD_MAX(unit))
		bu->changeSpd = UNIT_SPD_MAX(unit) - unit->spd;

	if ((unit->def + bu->changeDef) > UNIT_DEF_MAX(unit))
		bu->changeDef = UNIT_DEF_MAX(unit) - unit->def;

	if ((unit->res + bu->changeRes) > UNIT_RES_MAX(unit))
		bu->changeRes = UNIT_RES_MAX(unit) - unit->res;

	if ((unit->lck + bu->changeLck) > UNIT_LCK_MAX(unit))
		bu->changeLck = UNIT_LCK_MAX(unit) - unit->lck;
}

STATIC_DECLAR void UnitCheckStatCapsVanilla(struct Unit *unit)
{
	if (unit->maxHP > KUNIT_MHP_MAX(unit))
		unit->maxHP = KUNIT_MHP_MAX(unit);

	if (unit->pow > UNIT_POW_MAX(unit))
		unit->pow = UNIT_POW_MAX(unit);

	if (unit->skl > UNIT_SKL_MAX(unit))
		unit->skl = UNIT_SKL_MAX(unit);

	if (unit->spd > UNIT_SPD_MAX(unit))
		unit->spd = UNIT_SPD_MAX(unit);

	if (unit->def > UNIT_DEF_MAX(unit))
		unit->def = UNIT_DEF_MAX(unit);

	if (unit->res > UNIT_RES_MAX(unit))
		unit->res = UNIT_RES_MAX(unit);

	if (unit->lck > UNIT_LCK_MAX(unit))
		unit->lck = UNIT_LCK_MAX(unit);

	if (unit->conBonus > (UNIT_CON_MAX(unit) - UNIT_CON_BASE(unit)))
		unit->conBonus = (UNIT_CON_MAX(unit) - UNIT_CON_BASE(unit));

	if (unit->movBonus > (UNIT_MOV_MAX(unit) - UNIT_MOV_BASE(unit)))
		unit->movBonus = (UNIT_MOV_MAX(unit) - UNIT_MOV_BASE(unit));
}

LYN_REPLACE_CHECK(CheckBattleUnitStatCaps);
void CheckBattleUnitStatCaps(struct Unit *unit, struct BattleUnit *bu)
{
	CheckBattleUnitStatCapsVanilla(unit, bu);

	/* Hooks */
	if ((UNIT_MAG(unit) + BU_CHG_MAG(bu)) > GetUnitMaxMagic(unit))
		BU_CHG_MAG(bu) = GetUnitMaxMagic(unit) - UNIT_MAG(unit);
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
	if (UNIT_MAG(unit) > GetUnitMaxMagic(unit))
		UNIT_MAG(unit) = GetUnitMaxMagic(unit);
}
