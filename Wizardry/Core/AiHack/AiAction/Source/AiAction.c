#include "common-chax.h"
#include "AiAction.h"
#include "battle-system.h"

LYN_REPLACE_CHECK(CpPerform_PerformAction);
void CpPerform_PerformAction(struct CpPerformProc *proc)
{
	const struct AiActionConf *it;
	void (*exec)(ProcPtr proc);

	proc->unk_30 = 0;
	if (gActionData.unitActionType == UNIT_ACTION_TRAPPED) {
		proc->func = AiDummyAction;
		return;
	}

#if CHAX
	if (gAiDecision.actionId >= CONFIG_AI_ACTION_AMT) {
		proc->func = AiDummyAction;
		return;
	}

	it = gpAiActionTable + gAiDecision.actionId;
	if (it->idle)
		proc->func = (void *)((uintptr_t)it->idle | 0x08000000);
	else
		proc->func = AiDummyAction;

	if (it->exec) {
		exec = (void *)((uintptr_t)it->exec | 0x08000000);
		exec(proc);
	}
#endif
}

LYN_REPLACE_CHECK(AiStartCombatAction);
void AiStartCombatAction(struct CpPerformProc *proc)
{
	gActionData.subjectIndex = gActiveUnitId;
	gActionData.unitActionType = UNIT_ACTION_COMBAT;
	gActionData.targetIndex = gAiDecision.targetId;

	gActiveUnit->xPos = gAiDecision.xMove;
	gActiveUnit->yPos = gAiDecision.yMove;

	if (gAiDecision.targetId == 0) {
		struct Trap *trap = GetTrapAt(gAiDecision.xTarget, gAiDecision.yTarget);

		gActionData.xOther = gAiDecision.xTarget;
		gActionData.yOther = gAiDecision.yTarget;
		gActionData.trapType = trap->extra;
	}

	switch (gAiDecision.itemSlot) {
	case 0xFF:
		gActionData.itemSlotIndex = BU_ISLOT_BALLISTA;
		break;

	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		EquipUnitItemSlot(gActiveUnit, gAiDecision.itemSlot);
		gActionData.itemSlotIndex = 0;
		break;

#if CHAX
	case CHAX_BUISLOT_GAIDEN_BMAG1 ... CHAX_BUISLOT_GAIDEN_BMAG7:
	case CHAX_BUISLOT_GAIDEN_WMAG1 ... CHAX_BUISLOT_GAIDEN_WMAG7:
		gActionData.itemSlotIndex = gAiDecision.itemSlot;
		gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat;
		break;
#endif
	}
	ApplyUnitAction(proc);
}

LYN_REPLACE_CHECK(AiStaffAction);
bool AiStaffAction(struct CpPerformProc *proc)
{
	gActiveUnit->xPos = gAiDecision.xMove;
	gActiveUnit->yPos = gAiDecision.yMove;

	gActionData.unitActionType = UNIT_ACTION_STAFF;

	gActionData.targetIndex = gAiDecision.targetId;
	gActionData.itemSlotIndex = gAiDecision.itemSlot;

#if CHAX
	ApplyUnitAction(proc);
#else
	ActionStaffDoorChestUseItem(proc);
#endif

	return true;
}
