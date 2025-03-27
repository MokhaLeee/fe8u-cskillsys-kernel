#include "common-chax.h"
#include "weapon-range.h"

LYN_REPLACE_CHECK(MakeTargetListForAdjacentHeal);
void MakeTargetListForAdjacentHeal(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_HEAL);
	ForEachUnit(TryAddUnitToHealTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForRangedHeal);
void MakeTargetListForRangedHeal(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_PHYSIC);
	ForEachUnit(TryAddUnitToHealTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForRestore);
void MakeTargetListForRestore(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_RESTORE);
	ForEachUnit(TryAddUnitToRestoreTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForBarrier);
void MakeTargetListForBarrier(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_BARRIER);
	ForEachUnit(TryAddUnitToBarrierTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForRescueStaff);
void MakeTargetListForRescueStaff(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_RESCUE);
	ForEachUnit(TryAddUnitToRescueStaffTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForSilence);
void MakeTargetListForSilence(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_SILENCE);
	ForEachUnit(TryAddUnitToSilenceTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForSleep);
void MakeTargetListForSleep(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_SLEEP);
	ForEachUnit(TryAddUnitToSleepTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForBerserk);
void MakeTargetListForBerserk(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_BERSERK);
	ForEachUnit(TryAddUnitToBerserkTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForWarp);
void MakeTargetListForWarp(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_WARP);
	ForEachUnit(TryAddUnitToWarpTargetList, gBmMapRange, 0);
}

LYN_REPLACE_CHECK(MakeTargetListForHammerne);
void MakeTargetListForHammerne(struct Unit *unit)
{
	gSubjectUnit = unit;
	InitTargets(unit->xPos, unit->yPos);

	BmMapFill(gBmMapRange, 0);
	AddMapForItem(unit, ITEM_STAFF_REPAIR);
	ForEachUnit(TryAddUnitToHammerneTargetList, gBmMapRange, 0);
}
