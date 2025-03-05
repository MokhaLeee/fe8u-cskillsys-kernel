#include "common-chax.h"
#include "kernel-lib.h"

extern HookProcFunc_t const *const gpEndTurnFuncs;

STATIC_DECLAR void EndTurnHook(ProcPtr proc)
{
	KernelStartBlockingHookProc(gpEndTurnFuncs, proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_PlayerPhaseEndTurnHook[] = {
	PROC_YIELD,
	PROC_CALL(EndTurnHook),
	PROC_YIELD,

	// vanilla
	PROC_WHILE(DoesBMXFADEExist),
	PROC_END,
};

STATIC_DECLAR const struct ProcCmd ProcScr_AiPhaseEndTurnHook[] = {
	PROC_YIELD,
	PROC_CALL(EndTurnHook),
	PROC_YIELD,

	// vanilla
	PROC_CALL(AiPhaseCleanup),
	PROC_END,
};

/**
 * External hook
 */
bool EndTurnHook_PlayerPhase(ProcPtr proc)
{
	Proc_StartBlocking(ProcScr_PlayerPhaseEndTurnHook, proc);
	return false;
}

bool EndTurnHook_AiPhase(ProcPtr proc)
{
	Proc_StartBlocking(ProcScr_AiPhaseEndTurnHook, proc);
	return false;
}

extern struct ProcCmd *prProcScr_PlayerPhase;

LYN_REPLACE_CHECK(CommandEffectEndPlayerPhase);
u8 CommandEffectEndPlayerPhase(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
#if CHAX
	ProcPtr proc = Proc_Find(prProcScr_PlayerPhase);

	if (proc)
		Proc_Goto(proc, 3);
#else
    Proc_EndEach(prProcScr_PlayerPhase);
#endif

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
