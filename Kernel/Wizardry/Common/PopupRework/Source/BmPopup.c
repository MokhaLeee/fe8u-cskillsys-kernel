#include <common-chax.h>
#include <icon-rework.h>
#include <popup-reowrk.h>

/**
 * Bm popup main loop
 */
void BmPopR_Init(struct ProcPopupHanlder *proc)
{
	proc->config = gpBattlePopupConfigs;
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
