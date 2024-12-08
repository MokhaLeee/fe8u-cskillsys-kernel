#include "common-chax.h"
#include "action-expa.h"
#include "battle-system.h"
#include "gaiden-magic.h"

NOINLINE static void TryChangeAction(void)
{
	int slot = gActionData.itemSlotIndex;

	/* Modify for special battle */
	if (!CheckUnbreakableSpecialSlot(slot))
		return;

	switch (slot) {
	case CHAX_BUISLOT_GAIDEN_BMAG1:
	case CHAX_BUISLOT_GAIDEN_BMAG2:
	case CHAX_BUISLOT_GAIDEN_BMAG3:
	case CHAX_BUISLOT_GAIDEN_BMAG4:
	case CHAX_BUISLOT_GAIDEN_BMAG5:
	case CHAX_BUISLOT_GAIDEN_BMAG6:
	case CHAX_BUISLOT_GAIDEN_BMAG7:
	case CHAX_BUISLOT_GAIDEN_WMAG1:
	case CHAX_BUISLOT_GAIDEN_WMAG2:
	case CHAX_BUISLOT_GAIDEN_WMAG3:
	case CHAX_BUISLOT_GAIDEN_WMAG4:
	case CHAX_BUISLOT_GAIDEN_WMAG5:
	case CHAX_BUISLOT_GAIDEN_WMAG6:
	case CHAX_BUISLOT_GAIDEN_WMAG7:
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

	TryChangeAction();
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
