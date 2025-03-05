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
