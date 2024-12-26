#include "common-chax.h"
#include "kernel-lib.h"

extern const HookProcFunc_t gPrePhaseFuncs[];

bool PrePhaseHook_VanillaEnd(ProcPtr proc)
{
	Proc_StartBlocking(gProcScr_GorgonEggHatchDisplay, proc);
	return true;
}

bool PrePhaseHook(ProcPtr proc)
{
	KernelStartBlockingHookProc(gPrePhaseFuncs, proc);
	return false;
}
