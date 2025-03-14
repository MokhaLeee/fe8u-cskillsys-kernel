#include "common-chax.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "kernel-lib.h"
#include "battle-system.h"
#include "gaiden-magic.h"

bool PrePhase_ClearUnitsActedBit(ProcPtr proc)
{
	FOR_UNITS_VALID_ALL(unit, {
		ClearBitUES(unit, UES_BIT_ACTED);
	})
	return false;
}

STATIC_DECLAR void TryChangeAction(void)
{
	int slot = gActionData.itemSlotIndex;

	/* Modify for special battle */
	if (!CheckUnbreakableSpecialSlot(slot))
		return;

	switch (slot) {
	case CHAX_BUISLOT_GAIDEN_BMAG1 ... CHAX_BUISLOT_GAIDEN_BMAG7:
	case CHAX_BUISLOT_GAIDEN_WMAG1 ... CHAX_BUISLOT_GAIDEN_WMAG7:
		TryChangeGaidenMagicAction();
		break;

	default:
		break;
	}
}

LYN_REPLACE_CHECK(ApplyUnitAction);
unsigned int ApplyUnitAction(ProcPtr proc)
{
	UnitActionFunc_t it;

	gActiveUnit = GetUnit(gActionData.subjectIndex);

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case UNIT_ACTION_STAFF:
		TryChangeAction();
		break;
	}

	if (gActionData.unitActionType >= CONFIG_UNIT_ACTION_AMT)
		return true;

	SetBitUES(gActiveUnit, UES_BIT_ACTED);

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
		int itemIdx = GetItemIndex(GetItemFromSlot(gActiveUnit, gActionData.itemSlotIndex));

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
