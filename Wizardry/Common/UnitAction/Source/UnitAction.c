#include "common-chax.h"
#include "action-expa.h"

/* LynJump */
unsigned int ApplyUnitAction(ProcPtr proc)
{
    const UnitActionFunc_t * it;

    gActiveUnit = GetUnit(gActionData.subjectIndex);

    if (gActionData.unitActionType >= CONFIG_UNIT_ACTION_AMT)
        return true;

    it = gpUnitActionTable + gActionData.unitActionType;
    if (!it)
        return true;

    return (*it)(proc);
}

/* Misc action functions */
bool _ActionWait(ProcPtr proc)
{
    gActiveUnit->state |= US_HAS_MOVED;
    return true;
}

bool _ActionCombat(ProcPtr proc)
{
    if (gActionData.unitActionType == UNIT_ACTION_COMBAT)
    {
        int itemIdx = GetItemIndex(gActiveUnit->items[gActionData.itemSlotIndex]);
        if (itemIdx == ITEM_NIGHTMARE)
        {
            ActionStaffDoorChestUseItem(proc);
            return false;
        }
    }
    return ActionCombat(proc);
}

bool _ActionStaffDoorChestUseItem(ProcPtr proc)
{
    ActionStaffDoorChestUseItem(proc);
    return false;
}

bool _ActionPick(ProcPtr proc)
{
    ActionPick(proc);
    return false;
}
