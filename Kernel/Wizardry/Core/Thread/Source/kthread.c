#include "common-chax.h"
#include "kthread.h"

extern void yield_sub_thread(void);

void CreateSubThread(thread_task_func func)
{
	if (gThreadInfo.func) {
		Warnf("SubThread replaced, old=0x%p, new=0x%p",
			gThreadInfo.func, func);
	}

	gThreadInfo.func = func;
	gThreadInfo.sub_thread_state = SUBTHREAD_PENDING;
    gThreadInfo.sub_thread_break_reason = SUBTHRED_BREAK_FROM_NORMAL;
}

void RemoveSubThread(thread_task_func func)
{
	gThreadInfo.func = NULL;
	gThreadInfo.sub_thread_state = SUBTHREAD_NONE;
}

void YieldSubThread(void)
{
	u32 vcount = REG_VCOUNT;

	if (vcount >= 160) {
		/* It is risky that vblank INTR may come */
		return;
	}

	yield_sub_thread();
}
