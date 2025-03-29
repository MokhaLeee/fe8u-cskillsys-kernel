#include "common-chax.h"
#include "kernel-lib.h"
#include "item-sys.h"
#include "battle-system.h"

const struct IERevamp *GetIERevamp(int item)
{
	struct IERevamp const *const pplist = *pr_gpIERevampTable;
	int iid = ITEM_INDEX(item);
	int ier_id;

#if 1
	if (iid >= ItemDataLengthCache)
		iid = CONFIG_ITEM_INDEX_SKILL_SCROLL;
#endif

	ier_id = GetItemData(ITEM_INDEX(item))->useEffectId;

	if (IER_IS_VALID(ier_id))
		return &pplist[ier_id];

	return NULL;
}

/**
 * Usability
 */
LYN_REPLACE_CHECK(CanUnitUseItem);
bool CanUnitUseItem(struct Unit* unit, int item)
{
	const struct IERevamp *revamp;

	if ((GetItemAttributes(item) & IA_STAFF) && !CanUnitUseStaff(unit, item))
		return false;

	revamp = GetIERevamp(item);
	if (revamp && revamp->usability)
		return revamp->usability(unit, item);

	return false;
}

/**
 * Effect
 */
LYN_REPLACE_CHECK(DoItemUse);
void DoItemUse(struct Unit *unit, int item)
{
	const struct IERevamp *revamp = GetIERevamp(item);

	ClearBg0Bg1();
	EndFaceById(0);

	if (revamp && revamp->effect)
		revamp->effect(unit, item);
	else
		SetItemUseAction(unit); // default
}

/**
 * Action
 */
LYN_REPLACE_CHECK(ActionStaffDoorChestUseItem);
void ActionStaffDoorChestUseItem(ProcPtr proc)
{
	struct Unit *unit = GetUnit(gActionData.subjectIndex);
#if CHAX
	int item = GetItemFromSlot(unit, gActionData.itemSlotIndex);
#else
	int item = unit->items[gActionData.itemSlotIndex];
#endif
	const struct IERevamp *revamp = GetIERevamp(item);

	gBattleActor.hasItemEffectTarget = false;

	if (revamp && revamp->action_effect)
		revamp->action_effect(proc, unit, item);

	if (gBattleTarget.statusOut >= 0)
		Proc_StartBlocking(ProcScr_SetTargetStatus, proc);
}

/**
 * Prep usability
 */
LYN_REPLACE_CHECK(CanUnitUseItemPrepScreen);
bool CanUnitUseItemPrepScreen(struct Unit *unit, int item)
{
	const struct IERevamp *revamp = GetIERevamp(item);

	if (revamp && revamp->prep_usability)
		return revamp->prep_usability(unit, item);

	return false;
}

/**
 * Prep effects
 */
LYN_REPLACE_CHECK(PrepItemUse_HandleItemEffect);
void PrepItemUse_HandleItemEffect(struct ProcPrepItemUse *proc)
{
	u16 item = proc->unit->items[proc->slot];
	const struct IERevamp *revamp = GetIERevamp(item);

	if (revamp && revamp->prep_effect)
		revamp->prep_effect(proc, item);
}
