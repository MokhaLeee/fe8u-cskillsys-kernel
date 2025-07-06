#include "common-chax.h"
#include "kthread.h"

#define LOCAL_TRACE 1

extern void yield_sub_thread(void);
extern void get_sub_thread_stack(u32 *buf);

void dump_sub_thread_stack(void)
{
	_maybe_unused u32 *sp = gThreadInfo.sub_thread_sp;

	Print("sub thread");
	Printf("[pc] 0x%08X [lr] 0x%08X [sp] 0x%08X",  sp[14], sp[12], (uintptr_t)sp);
	Printf("[r0] 0x%08X [r1] 0x%08X [r2] 0x%08X [r3] 0x%08X", sp[0],  sp[1],  sp[2],  sp[3]);
	Printf("[r4] 0x%08X [r5] 0x%08X [r6] 0x%08X [r7] 0x%08X", sp[13], sp[5],  sp[6],  sp[7]);
	Print("done");
}

/* API */
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

	LTRACE("yield");
	yield_sub_thread();
}
