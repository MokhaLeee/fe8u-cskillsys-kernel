#include "common-chax.h"
#include "weapon-range.h"

/* LynJump */
void MakeTargetListForAdjacentHeal(struct Unit * unit)
{
    gSubjectUnit = unit;
    InitTargets(unit->xPos, unit->yPos);

    BmMapFill(gBmMapRange, 0);
    AddMapForItem(unit, ITEM_STAFF_HEAL);
    ForEachUnit(TryAddUnitToHealTargetList, gBmMapRange, 0);
}

/* LynJump */
void MakeTargetListForRangedHeal(struct Unit * unit)
{
    gSubjectUnit = unit;
    InitTargets(unit->xPos, unit->yPos);

    BmMapFill(gBmMapRange, 0);
    AddMapForItem(unit, ITEM_STAFF_PHYSIC);
    ForEachUnit(TryAddUnitToHealTargetList, gBmMapRange, 0);
}

/* LynJump */
void MakeTargetListForRestore(struct Unit * unit)
{
    gSubjectUnit = unit;
    InitTargets(unit->xPos, unit->yPos);

    BmMapFill(gBmMapRange, 0);
    AddMapForItem(unit, ITEM_STAFF_RESTORE);
    ForEachUnit(TryAddUnitToRestoreTargetList, gBmMapRange, 0);
}

/* LynJump */
void MakeTargetListForBarrier(struct Unit * unit)
{
    gSubjectUnit = unit;
    InitTargets(unit->xPos, unit->yPos);

    BmMapFill(gBmMapRange, 0);
    AddMapForItem(unit, ITEM_STAFF_BARRIER);
    ForEachUnit(TryAddUnitToBarrierTargetList, gBmMapRange, 0);
}

/* LynJump */
void MakeTargetListForRescueStaff(struct Unit * unit)
{
    gSubjectUnit = unit;
    InitTargets(unit->xPos, unit->yPos);

    BmMapFill(gBmMapRange, 0);
    AddMapForItem(unit, ITEM_STAFF_RESCUE);
    ForEachUnit(TryAddUnitToRescueStaffTargetList, gBmMapRange, 0);
}

/* LynJump */
void MakeTargetListForSilence(struct Unit * unit)
{
    gSubjectUnit = unit;
    InitTargets(unit->xPos, unit->yPos);

    BmMapFill(gBmMapRange, 0);
    AddMapForItem(unit, ITEM_STAFF_SILENCE);
    ForEachUnit(TryAddUnitToSilenceTargetList, gBmMapRange, 0);
}

/* LynJump */
void MakeTargetListForSleep(struct Unit * unit)
{
    gSubjectUnit = unit;
    InitTargets(unit->xPos, unit->yPos);

    BmMapFill(gBmMapRange, 0);
    AddMapForItem(unit, ITEM_STAFF_SLEEP);
    ForEachUnit(TryAddUnitToSleepTargetList, gBmMapRange, 0);
}

/* LynJump */
void MakeTargetListForBerserk(struct Unit * unit)
{
    gSubjectUnit = unit;
    InitTargets(unit->xPos, unit->yPos);

    BmMapFill(gBmMapRange, 0);
    AddMapForItem(unit, ITEM_STAFF_BERSERK);
    ForEachUnit(TryAddUnitToBerserkTargetList, gBmMapRange, 0);
}

/* LynJump */
void MakeTargetListForWarp(struct Unit * unit)
{
    gSubjectUnit = unit;
    InitTargets(unit->xPos, unit->yPos);

    BmMapFill(gBmMapRange, 0);
    AddMapForItem(unit, ITEM_STAFF_WARP);
    ForEachUnit(TryAddUnitToWarpTargetList, gBmMapRange, 0);
}

/* LynJump */
void MakeTargetListForHammerne(struct Unit * unit)
{
    gSubjectUnit = unit;
    InitTargets(unit->xPos, unit->yPos);

    BmMapFill(gBmMapRange, 0);
    AddMapForItem(unit, ITEM_STAFF_REPAIR);
    ForEachUnit(TryAddUnitToHammerneTargetList, gBmMapRange, 0);
}
