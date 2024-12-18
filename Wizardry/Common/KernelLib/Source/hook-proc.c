#include "common-chax.h"
#include "kernel-lib.h"

#define KERNEL_SKIPPING_LISTENER_VALID_MAGIC 0x83

extern u8 sKernelHookSkippingFlag;

/**
 * Terminator listener
 */
static void ListenKernelHookSkippingFlag(void)
{
	if ((gKeyStatusPtr->newKeys | gKeyStatusPtr->repeatedKeys | gKeyStatusPtr->heldKeys) & START_BUTTON)
		sKernelHookSkippingFlag = KERNEL_SKIPPING_LISTENER_VALID_MAGIC;
}

static void ResetKernelHookSkippingFlag(void)
{
	sKernelHookSkippingFlag = 0;
}

static const struct ProcCmd ProcScr_SkippingFlagListener[] = {
	PROC_NAME("SkippingFlagListener"),
	PROC_SET_END_CB(ResetKernelHookSkippingFlag),
	PROC_CALL(ResetKernelHookSkippingFlag),
	PROC_YIELD,
	PROC_REPEAT(ListenKernelHookSkippingFlag),
	PROC_END
};

static ProcPtr StartHookListener(ProcPtr parent)
{
	return Proc_Start(ProcScr_SkippingFlagListener, parent);
}

bool CheckKernelHookSkippingFlag(void)
{
	ListenKernelHookSkippingFlag();

	return sKernelHookSkippingFlag == KERNEL_SKIPPING_LISTENER_VALID_MAGIC;
}

/**
 * Hook proc
 */
struct HookProc {
	PROC_HEADER;
	HookProcFunc_t const *hook_list;
	int index;
};

STATIC_DECLAR void HookProc_Main(struct HookProc *proc)
{
	int ret;
	HookProcFunc_t it;

	while (1) {
		it = proc->hook_list[proc->index++];
		if (!it) {
			Proc_Goto(proc, 2);
			return;
		}

		ret = it(proc);
		if (ret != false)
			return;
	}
}

STATIC_DECLAR const struct ProcCmd ProcScr_HookCommon[] = {
	PROC_SET_END_CB(EndAllProcChildren),
	PROC_YIELD,

PROC_LABEL(1),
	PROC_CALL(HookProc_Main),
	PROC_YIELD,
	PROC_GOTO(1),

PROC_LABEL(2),
	PROC_YIELD,
	PROC_END
};

void KernelStartBlockingHookProc(HookProcFunc_t const *hook_list, ProcPtr parent)
{
	struct HookProc *proc;

	proc = Proc_StartBlocking(ProcScr_HookCommon, parent);
	proc->index = 0;
	proc->hook_list = hook_list;

	StartHookListener(proc);
}
