#include "common-chax.h"
#include "action-expa.h"

STATIC_DECLAR bool _ActionWait(ProcPtr proc)
{
    gActiveUnit->state |= US_HAS_MOVED;
    return true;
}

STATIC_DECLAR bool _ActionCombat(ProcPtr proc)
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

STATIC_DECLAR bool _ActionStaffDoorChestUseItem(ProcPtr proc)
{
    ActionStaffDoorChestUseItem(proc);
    return false;
}

STATIC_DECLAR bool _ActionPick(ProcPtr proc)
{
    ActionPick(proc);
    return false;
}

const UnitActionFunc_t gUnitActionTable[CONFIG_UNIT_ACTION_AMT] = {
    [UNIT_ACTION_WAIT] = _ActionWait,
    [UNIT_ACTION_TRAPPED] = _ActionWait,
    [UNIT_ACTION_RESCUE] = ActionRescue,
    [UNIT_ACTION_DROP] = ActionDrop,
    [UNIT_ACTION_VISIT] = ActionVisitAndSeize,
    [UNIT_ACTION_SEIZE] = ActionVisitAndSeize,
    [UNIT_ACTION_COMBAT] = _ActionCombat,
    [UNIT_ACTION_DANCE] = ActionDance,
    [UNIT_ACTION_TALK] = ActionTalk,
    [UNIT_ACTION_SUPPORT] = ActionSupport,
    [UNIT_ACTION_STEAL] = ActionSteal,
    [UNIT_ACTION_SUMMON] = ActionSummon,
    [UNIT_ACTION_SUMMON_DK] = ActionSummonDK,
    [UNIT_ACTION_ARENA] = ActionArena,
    [UNIT_ACTION_STAFF] = _ActionStaffDoorChestUseItem,
    [UNIT_ACTION_DOOR] = _ActionStaffDoorChestUseItem,
    [UNIT_ACTION_CHEST] = _ActionStaffDoorChestUseItem,
    [UNIT_ACTION_USE_ITEM] = _ActionStaffDoorChestUseItem,
    [UNIT_ACTION_PICK] = _ActionPick,
};
