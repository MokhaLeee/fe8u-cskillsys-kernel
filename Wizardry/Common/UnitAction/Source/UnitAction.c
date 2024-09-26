#include "common-chax.h"
#include "action-expa.h"

LYN_REPLACE_CHECK(ApplyUnitAction);
unsigned int ApplyUnitAction(ProcPtr proc)
{
	UnitActionFunc_t it;

	gActiveUnit = GetUnit(gActionData.subjectIndex);

	if (gActionData.unitActionType >= CONFIG_UNIT_ACTION_AMT)
		return true;

	/* Well I think there should be set some data for action-expa during action routine */
	memset(&gActionDataExpa, 0, sizeof(gActionDataExpa));

	it = gpUnitActionTable[gActionData.unitActionType];
	if (!it)
		return true;

	return it(proc);
}

/* Misc action functions */
bool _ActionWait(ProcPtr proc)
{
	gActiveUnit->state |= US_HAS_MOVED;
	return true;
}

bool _ActionCombat(ProcPtr proc)
{
	if (gActionData.unitActionType == UNIT_ACTION_COMBAT) {
		int itemIdx = GetItemIndex(gActiveUnit->items[gActionData.itemSlotIndex]);

		if (itemIdx == ITEM_NIGHTMARE) {
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
