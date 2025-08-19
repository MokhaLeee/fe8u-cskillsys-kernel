#include "common-chax.h"
#include "item-sys.h"
#include "battle-system.h"
#include "constants/texts.h"

/**
 * Usability
 */
bool IER_Usability_AdjacentHeal(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForAdjacentHeal);
}

bool IER_Usability_RangedHeal(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForRangedHeal);
}

bool IER_Usability_Restore(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForRestore);
}

bool IER_Usability_Rescue(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForRescueStaff);
}

bool IER_Usability_Barrier(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForBarrier);
}

bool IER_Usability_Silence(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForSilence);
}

bool IER_Usability_Sleep(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForSleep);
}

bool IER_Usability_Berserk(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForBerserk);
}

bool IER_Usability_Warp(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForWarp);
}

bool IER_Usability_Hammerne(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForHammerne);
}

bool IER_Usability_Unlock(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForUnlock);
}

bool IER_Usability_Booster(struct Unit *unit, int item)
{
	return CanUnitUseStatGainItem(unit, item);
}

bool IER_Usability_Promotion(struct Unit *unit, int item)
{
	return CanUnitUsePromotionItem(unit, item);
}

bool IER_Usability_Vulnerary(struct Unit *unit, int item)
{
	return CanUnitUseHealItem(unit);
}

bool IER_Usability_PureWater(struct Unit *unit, int item)
{
	return CanUnitUsePureWaterItem(unit);
}

bool IER_Usability_Torch(struct Unit *unit, int item)
{
	return CanUnitUseTorchItem(unit);
}

bool IER_Usability_Antitoxin(struct Unit *unit, int item)
{
	return CanUnitUseAntitoxinItem(unit);
}

bool IER_Usability_ChestKey(struct Unit *unit, int item)
{
	return CanUnitUseChestKeyItem(unit);
}

bool IER_Usability_DoorKey(struct Unit *unit, int item)
{
	return CanUnitUseDoorKeyItem(unit);
}

bool IER_Usability_LockPick(struct Unit *unit, int item)
{
	return CanUnitUseLockpickItem(unit);
}

bool IER_Usability_Latona(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForLatona);
}

bool IER_Usability_Mine(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForMine);
}

bool IER_Usability_LightRune(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForLightRune);
}

bool IER_Usability_StaffTorch(struct Unit *unit, int item)
{
	return gPlaySt.chapterVisionRange != 0;
}

bool IER_Usability_DanceRing(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForDanceRing);
}

bool IER_Usability_MetisStone(struct Unit *unit, int item)
{
	if (unit->state & US_GROWTH_BOOST)
		return false;

	return true;
}

bool IER_Usability_JunaFruit(struct Unit *unit, int item)
{
	return CanUnitUseFruitItem(unit);
}

bool IER_Usability_NightMare(struct Unit *unit, int item)
{
	return HasSelectTarget(unit, MakeTargetListForFuckingNightmare);
}

/**
 * Effects
 */
void IER_Effect_AdjacentHeal(struct Unit *unit, int item)
{
	DoUseHealStaff(unit, MakeTargetListForAdjacentHeal);
}

void IER_Effect_RangedHeal(struct Unit *unit, int item)
{
	DoUseHealStaff(unit, MakeTargetListForRangedHeal);
}

void IER_Effect_RescueStaff(struct Unit *unit, int item)
{
	DoUseRescueStaff(unit, MakeTargetListForRescueStaff);
}

void IER_Effect_Restore(struct Unit *unit, int item)
{
	DoUseRestoreStaff(unit, MakeTargetListForRestore);
}

void IER_Effect_Silence(struct Unit *unit, int item)
{
	DoUseAttackStaff(unit, MakeTargetListForSilence);
}

void IER_Effect_Sleep(struct Unit *unit, int item)
{
	DoUseAttackStaff(unit, MakeTargetListForSleep);
}

void IER_Effect_Berserk(struct Unit *unit, int item)
{
	DoUseAttackStaff(unit, MakeTargetListForBerserk);
}

void IER_Effect_Barrier(struct Unit *unit, int item)
{
	DoUseBarrierStaff(unit);
}

void IER_Effect_Unlock(struct Unit *unit, int item)
{
	DoUsePutTrap(unit, MakeTargetListForUnlock, MSG_087A);
}

void IER_Effect_Warp(struct Unit *unit, int item)
{
	DoUseWarpStaff(unit);
}

void IER_Effect_Repair(struct Unit *unit, int item)
{
	DoUseRepairStaff(unit);
}

void IER_Effect_CommonStaff(struct Unit *unit, int item)
{
	SetStaffUseAction(unit);
}

void IER_Effect_Mine(struct Unit *unit, int item)
{
	DoUsePutTrap(unit, MakeTargetListForMine, MSG_087D);
}

void IER_Effect_LightRune(struct Unit *unit, int item)
{
	DoUsePutTrap(unit, MakeTargetListForLightRune, MSG_087E);
}

void IER_Effect_Torch(struct Unit *unit, int item)
{
	DoUseTorchStaff(unit);
}

void IER_Effect_SpeicalDance(struct Unit *unit, int item)
{
	DoUseSpecialDance(unit, MakeTargetListForDanceRing, MSG_087F);
}

/**
 * Actions
 */
void IER_Action_StandardHeal(ProcPtr proc, struct Unit *unit, int item)
{
	ExecStandardHeal(proc);
}

void IER_Action_StatusStaff(ProcPtr proc, struct Unit *unit, int item)
{
	ExecStatusStaff(proc);
}

void IER_Action_NightMare(ProcPtr proc, struct Unit *unit, int item)
{
	// ExecNightmare(proc);
	{
		BattleInitItemEffect(GetUnit(gActionData.subjectIndex), gActionData.itemSlotIndex);
		BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));
		BattleApplyItemEffect(proc);

		// BeginBattleAnimations();
		BG_Fill(gBG2TilemapBuffer, 0);
		BG_EnableSyncByMask(1 << 2);
		gPaletteBuffer[PAL_BACKDROP_OFFSET] = 0;
		EnablePaletteSync();
		RenderBmMap();
		EndAllMus();
		RenderBmMap();
		BeginBattleMapAnims();
		gBattleStats.config |= BATTLE_CONFIG_MAPANIMS;
	}

	Proc_StartBlocking(ProcScr_ExecNightmareStaff, proc);
}

void IER_Action_Fortify(ProcPtr proc, struct Unit *unit, int item)
{
	ExecFortify(proc);
}

void IER_Action_Restore(ProcPtr proc, struct Unit *unit, int item)
{
	ExecRestore(proc);
}

void IER_Action_RescueStaff(ProcPtr proc, struct Unit *unit, int item)
{
	ExecRescueStaff(proc);
}

void IER_Action_BarrierStaff(ProcPtr proc, struct Unit *unit, int item)
{
	ExecBarrierStaff(proc);
}

void IER_Action_WarpStaff(ProcPtr proc, struct Unit *unit, int item)
{
	ExecWarpStaff(proc);
}

void IER_Action_UnlockStaff(ProcPtr proc, struct Unit *unit, int item)
{
	ExecUnlockStaff(proc);
}

void IER_Action_Hammerne(ProcPtr proc, struct Unit *unit, int item)
{
	ExecHammerne(proc);
}

void IER_Action_TorchItem(ProcPtr proc, struct Unit *unit, int item)
{
	ExecTorchItem(proc);
}

void IER_Action_VulneraryItem(ProcPtr proc, struct Unit *unit, int item)
{
	int amount;
	
	amount = GetUnitItemHealAmount(unit, item);
	amount = HealAmountGetter(amount, NULL, unit);

	ExecVulneraryItem(proc, amount);
}

void IER_Action_ElixirItem(ProcPtr proc, struct Unit *unit, int item)
{
	ExecElixirItem(proc);
}

void IER_Action_PureWaterItem(ProcPtr proc, struct Unit *unit, int item)
{
	ExecPureWaterItem(proc);
}

void IER_Action_AntitoxinItem(ProcPtr proc, struct Unit *unit, int item)
{
	ExecAntitoxinItem(proc);
}

void IER_Action_KeyItem(ProcPtr proc, struct Unit *unit, int item)
{
	ExecKeyItem(proc);
}

void IER_Action_Promotion(ProcPtr proc, struct Unit *unit, int item)
{
	gBattleActor.weaponBefore = gBattleTarget.weaponBefore = item;
	gBattleActor.weapon = gBattleTarget.weapon = GetUnitEquippedWeapon(unit);
	gBattleTarget.statusOut = -1;

	StartBmPromotion(proc);
}

void IER_Action_Booster(ProcPtr proc, struct Unit *unit, int item)
{
	ExecStatBoostItem(proc);
}

void IER_Action_Latona(ProcPtr proc, struct Unit *unit, int item)
{
	ExecLatona(proc);
}

void IER_Action_Mine(ProcPtr proc, struct Unit *unit, int item)
{
	ExecMine(proc);
}

void IER_Action_LightRune(ProcPtr proc, struct Unit *unit, int item)
{
	ExecLightRune(proc);
}

void IER_Action_TorchStaff(ProcPtr proc, struct Unit *unit, int item)
{
	ExecTorchStaff(proc);
}

void IER_Action_DanceRing(ProcPtr proc, struct Unit *unit, int item)
{
	ExecDanceRing(proc);
}

void IER_Action_JunaFruitItem(ProcPtr proc, struct Unit *unit, int item)
{
	ExecJunaFruitItem(proc);
}

/**
 * Prepscreen effects
 */
void IER_PrepEffect_StatBooster(struct ProcPrepItemUse *proc, u16 item)
{
	Proc_StartBlocking(ProcScr_PrepItemUseBooster, proc);
}

void IER_PrepEffect_Promotion(struct ProcPrepItemUse *proc, u16 item)
{
	PlaySoundEffect(0x6A);
	Proc_Goto(proc, PROC_LABEL_PREPITEMUSE_EXEC_PROMO);
}

void IER_PrepEffect_JunaFruit(struct ProcPrepItemUse *proc, u16 item)
{
	Proc_Goto(proc, PROC_LABEL_PREPITEMUSE_EXEC_JUNA);
}
