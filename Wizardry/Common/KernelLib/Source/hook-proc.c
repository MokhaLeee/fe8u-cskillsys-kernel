#include "common-chax.h"
#include "kernel-lib.h"

#define KERNEL_SKIPPING_LISTENER_VALID_MAGIC 0x83

extern u8 sKernelHookSkippingFlag;

/**
 * Terminator listener
 */
static void KernelHookSkippingFlagListener_End(ProcPtr proc)
{
	sKernelHookSkippingFlag = 0;
}

static void KernelHookSkippingFlagListener_Loop(ProcPtr proc)
{
	if (gKeyStatusPtr->newKeys & START_BUTTON)
		sKernelHookSkippingFlag = KERNEL_SKIPPING_LISTENER_VALID_MAGIC;
}

static const struct ProcCmd ProcScr_SkippingFlagListener[] = {
	PROC_NAME("SkippingFlagListener"),
	PROC_SET_END_CB(KernelHookSkippingFlagListener_End),
	PROC_YIELD,
	PROC_REPEAT(KernelHookSkippingFlagListener_Loop),
	PROC_END
};

static ProcPtr StartHookListener(ProcPtr parent)
{
	sKernelHookSkippingFlag = 0;
	return Proc_Start(ProcScr_SkippingFlagListener, parent);
}

bool CheckKernelHookSkippingFlag(void)
{
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
