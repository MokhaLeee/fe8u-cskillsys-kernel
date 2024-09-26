#include "common-chax.h"
#include "AiAction.h"

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
