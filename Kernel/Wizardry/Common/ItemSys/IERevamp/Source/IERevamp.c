#include "common-chax.h"
#include "item-sys.h"

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
		return FALSE;

	return TRUE;
}

bool IER_Usability_JunaFruit(struct Unit *unit, int item)
{
	return CanUnitUseFruitItem(unit);
}
