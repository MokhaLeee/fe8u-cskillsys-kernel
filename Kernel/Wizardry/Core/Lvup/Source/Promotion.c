#include "common-chax.h"
#include "strmag.h"
#include "lvup.h"
#include "bwl.h"
#include "skill-system.h"

STATIC_DECLAR NOINLINE void ApplyUnitPromotionVanilla(struct Unit *unit, u8 classId)
{
	const struct ClassData *promotedClass = GetClassData(classId);

	int i;

	// Remove base class' base wexp from unit wexp
	for (i = 0; i < 8; ++i)
		unit->ranks[i] -= unit->pClassData->baseRanks[i];

	// Update unit class
	unit->pClassData = promotedClass;

	// Add promoted class' base wexp to unit wexp
	for (i = 0; i < 8; ++i) {
		int wexp = unit->ranks[i];

		wexp += unit->pClassData->baseRanks[i];

		if (wexp > WPN_EXP_S)
			wexp = WPN_EXP_S;

		unit->ranks[i] = wexp;
	}

	// Apply stat ups

	unit->maxHP += promotedClass->promotionHp;
	unit->pow += promotedClass->promotionPow;
	unit->skl += promotedClass->promotionSkl;
	unit->spd += promotedClass->promotionSpd;
	unit->def += promotedClass->promotionDef;
	unit->res += promotedClass->promotionRes;

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

	unit->level = 1;
	unit->exp   = 0;

	unit->curHP += promotedClass->promotionHp;

	if (unit->curHP > GetUnitMaxHp(unit))
		unit->curHP = GetUnitMaxHp(unit);
}

void GenerateBattleUnitStatGainsComparativelyVanilla(struct BattleUnit *bu, struct Unit *unit)
{
	bu->changeHP  = bu->unit.maxHP - unit->maxHP;
	bu->changePow = bu->unit.pow   - unit->pow;
	bu->changeSkl = bu->unit.skl   - unit->skl;
	bu->changeSpd = bu->unit.spd   - unit->spd;
	bu->changeDef = bu->unit.def   - unit->def;
	bu->changeRes = bu->unit.res   - unit->res;
	bu->changeLck = bu->unit.lck   - unit->lck;

	if (bu->unit.conBonus != unit->conBonus)
		bu->changeCon = bu->unit.conBonus - unit->conBonus;
	else {
		bu->changeCon = 0;
		bu->unit.conBonus = unit->conBonus;
	}
}

LYN_REPLACE_CHECK(ApplyUnitPromotion);
void ApplyUnitPromotion(struct Unit *unit, u8 jid)
{
	NewBwlRecordHiddenLevel(unit);
	ApplyUnitPromotionVanilla(unit, jid);
	TryAddSkillPromotion(unit, jid);

	/* Hooks */
	UNIT_MAG(unit) += GetClassChgMagicBonus(jid);
	if (UNIT_MAG(unit) > GetUnitMaxStatusMag(unit))
		UNIT_MAG(unit) = GetUnitMaxStatusMag(unit);
}

LYN_REPLACE_CHECK(ApplyUnitDefaultPromotion);
void ApplyUnitDefaultPromotion(struct Unit *unit)
{
	ApplyUnitPromotion(unit, GetClassData(unit->pClassData->promotion)->number);
}

LYN_REPLACE_CHECK(GenerateBattleUnitStatGainsComparatively);
void GenerateBattleUnitStatGainsComparatively(struct BattleUnit *bu, struct Unit *unit)
{
	GenerateBattleUnitStatGainsComparativelyVanilla(bu, unit);

	/* Hooks */
	BU_CHG_MAG(bu) = UNIT_MAG(&bu->unit) - UNIT_MAG(unit);
}
