#include <common-chax.h>
#include <icon-rework.h>
#include <popup-reowrk.h>

LYN_REPLACE_CHECK(ParsePopupInstAndGetLen);
int ParsePopupInstAndGetLen(struct PopupProc *proc)
{
	const struct PopupInstruction *inst;

	proc->xGfxSize = 0;

	for (inst = proc->pDefinition; inst->opcode != POPUP_OP_END; inst++) {
		if (gPopupComponents[inst->opcode].get_len == NULL)
			continue;

		proc->xGfxSize += gPopupComponents[inst->opcode].get_len(proc, inst);
	}

	return proc->xGfxSize;
}

LYN_REPLACE_CHECK(GeneratePopupText);
void GeneratePopupText(const struct PopupInstruction *inst, struct Text th)
{
	for (; inst->opcode != POPUP_OP_END; inst++) {
		if (gPopupComponents[inst->opcode].display == NULL)
			continue;

		gPopupComponents[inst->opcode].display(&th, inst);
	}
	BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);
}

/**
 * Bm popup main loop
 */
void BmPopR_Init(struct ProcPopupHanlder *proc)
{
	proc->config = gBattlePopupConfigs;
}

void BmPopR_Loop(struct ProcPopupHanlder *proc)
{
	const struct PopupConfig *config = proc->config++;

	if (!config || !config->setup || !config->inst) {
		Proc_Goto(proc, 99);
		return;
	}

	if (config->setup()) {
		NewPopup_Simple(config->inst, config->duration, 0, proc);
		Proc_Break(proc);
	}
}

STATIC_DECLAR const struct ProcCmd ProcScr_BmPopR[] = {
	PROC_CALL(BmPopR_Init),
	PROC_YIELD,

PROC_LABEL(0),
	PROC_REPEAT(BmPopR_Loop),
	PROC_YIELD,
	PROC_GOTO(0),

PROC_LABEL(99),
	PROC_END,
};

LYN_REPLACE_CHECK(DisplayWpnBrokePopup);
void DisplayWpnBrokePopup(ProcPtr proc) { Proc_StartBlocking(ProcScr_BmPopR, proc); }

LYN_REPLACE_CHECK(DisplayWRankUpPopup);
void DisplayWRankUpPopup(ProcPtr proc) {}
