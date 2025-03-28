#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "battle-system.h"
#include "item-sys.h"
#include "kernel-lib.h"
#include "constants/skills.h"

int HealAmountGetter(int base, struct Unit *actor, struct Unit *target)
{
	const HealAmountGetterFunc_t *it;
	int status = base;

	for (it = gpHealAmountGetters; *it; it++)
		status = (*it)(status, actor, target);

	LIMIT_AREA(status, 0, 80);
	return status;
}

LYN_REPLACE_CHECK(ExecStandardHeal);
void ExecStandardHeal(ProcPtr proc)
{
	int amount;

	struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
	struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

	BattleInitItemEffect(unit_act, gActionData.itemSlotIndex);

	BattleInitItemEffectTarget(unit_tar);

	amount = GetUnitItemHealAmount(
		unit_act,
		GetItemFromSlot(unit_act, gActionData.itemSlotIndex)
	);

#if CHAX
	amount = HealAmountGetter(amount, unit_act, unit_tar);
#endif

	AddUnitHp(unit_tar, amount);
	gBattleHitIterator->hpChange = gBattleTarget.unit.curHP - GetUnitCurrentHp(unit_tar);
	gBattleTarget.unit.curHP = GetUnitCurrentHp(unit_tar);

	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}

LYN_REPLACE_CHECK(ExecFortify);
void ExecFortify(ProcPtr proc)
{
	int i;
	int amount;
	int targetCount;

	struct Unit *unit_act = GetUnit(gActionData.subjectIndex);

	BattleInitItemEffect(unit_act, gActionData.itemSlotIndex);

	BattleInitItemEffectTarget(GetUnitFromCharId(GetPlayerLeaderUnitId()));
	MakeTargetListForRangedHeal(unit_act);

	amount = GetUnitItemHealAmount(
		unit_act,
		GetItemFromSlot(unit_act, gActionData.itemSlotIndex)
	);

	targetCount = GetSelectTargetCount();

	for (i = 0; i < targetCount; i++) {
#if CHAX
		struct Unit *unit_tar = GetUnit(GetTarget(i)->uid);
		int amound_real = HealAmountGetter(amount, unit_act, unit_tar);

		AddUnitHp(unit_tar, amound_real);
#else
		AddUnitHp(GetUnit(GetTarget(i)->uid), amount);
#endif
	}

	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}
