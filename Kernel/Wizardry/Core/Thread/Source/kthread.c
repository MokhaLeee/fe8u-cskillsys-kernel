#include "gbafe.h"
#include "kthread.h"

extern void yield_sub_thread(void);

void CreateSubThread(thread_task_func func)
{
	gThreadInfo.func = func;
	gThreadInfo.sub_thread_state = SUBTHREAD_PENDING;
    gThreadInfo.sub_thread_break_reason = SUBTHRED_BREAK_FROM_NORMAL;
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
