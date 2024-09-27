#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

/* AiActionConf::exec */
void AiAction_Teleportation(ProcPtr parent)
{
	gActionData.xMove = gAiDecision.xTarget;
	gActionData.yMove = gAiDecision.yTarget;

#if defined(SID_Teleportation) && (COMMON_SKILL_VALID(SID_Teleportation))
	gActionData.subjectIndex = gActiveUnit->index;
	gActionData.unk08 = SID_Teleportation;
	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

	ApplyUnitAction(parent);
#endif
}

/* AiActionConf:idle */
STATIC_DECLAR bool AiAction_TeleportationIdleExt(struct CpPerformProc *proc)
{
	gAiDecision.xMove = gAiDecision.xTarget;
	gAiDecision.yMove = gAiDecision.yTarget;

	gActiveUnit->state &= ~US_HAS_MOVED_AI;
	gAiState.unitIt--;
	return true;
}

bool AiAction_TeleportationIdle(struct CpPerformProc *proc)
{
	/* Force introduce a YIELD */
	proc->func = AiAction_TeleportationIdleExt;
	return false;
}
