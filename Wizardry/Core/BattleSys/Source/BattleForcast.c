#include "common-chax.h"
#include "battle-system.h"
#include "combat-art.h"

extern const u8 Gfx_BKSEL[12][0x80];

LYN_REPLACE_CHECK(BattleForecastHitCountUpdate);
void BattleForecastHitCountUpdate(struct BattleUnit *bu, u8 *hitsCounter, int *usesCounter)
{
	int i;
	struct BattleUnit *target = (bu == &gBattleActor) ? &gBattleTarget : &gBattleActor;
	int count  = CheckCanTwiceAttackOrder(bu, target) ? GetBattleUnitHitCount(bu) * 2 : GetBattleUnitHitCount(bu);

	for (i = 0; i < count; i++) {
		int _i;

		if (*usesCounter <= 0)
			break;

		*hitsCounter = *hitsCounter + 1;

		for (_i = 0; _i < GetWeaponCost(bu, bu->weapon); _i++)
			*usesCounter = *usesCounter - 1;
	}
}

LYN_REPLACE_CHECK(InitBattleForecastBattleStats);
void InitBattleForecastBattleStats(struct BattleForecastProc *proc)
{
	int usesA = GetItemUses(gBattleActor.weaponBefore);
	int usesB = GetItemUses(gBattleTarget.weaponBefore);

	proc->hitCountA = 0;
	proc->isEffectiveA = false;

	if ((gBattleActor.weapon != 0) || (gBattleActor.weaponBroke)) {
		BattleForecastHitCountUpdate(&gBattleActor, (u8 *)&proc->hitCountA, &usesA);

		if (IsUnitEffectiveAgainst(&gBattleActor.unit, &gBattleTarget.unit))
			proc->isEffectiveA = true;

		if (IsItemEffectiveAgainst(gBattleActor.weaponBefore, &gBattleTarget.unit))
			proc->isEffectiveA = true;
	}

	proc->hitCountB = 0;
	proc->isEffectiveB = false;

	if ((gBattleTarget.weapon != 0) || (gBattleTarget.weaponBroke)) {
		BattleForecastHitCountUpdate(&gBattleTarget, (u8 *)&proc->hitCountB, &usesB);

		if (IsUnitEffectiveAgainst(&gBattleTarget.unit, &gBattleActor.unit))
			proc->isEffectiveB = true;

		if (IsItemEffectiveAgainst(gBattleTarget.weaponBefore, &gBattleActor.unit))
			proc->isEffectiveB = true;
	}

	/* Prepare GFX here */
	switch (proc->hitCountA) {
	case 0 ... 9:
		Copy2dChr(Gfx_BKSEL[proc->hitCountA], OBJ_VRAM0 + 0x5D00, 2, 2);
		break;

	default:
		Copy2dChr(Gfx_BKSEL[10], OBJ_VRAM0 + 0x5D00, 2, 2);
		break;
	}

	switch (proc->hitCountB) {
	case 0 ... 9:
		Copy2dChr(Gfx_BKSEL[proc->hitCountB], OBJ_VRAM0 + 0x5D40, 2, 2);
		break;

	default:
		Copy2dChr(Gfx_BKSEL[10], OBJ_VRAM0 + 0x5D40, 2, 2);
		break;
	}
}
