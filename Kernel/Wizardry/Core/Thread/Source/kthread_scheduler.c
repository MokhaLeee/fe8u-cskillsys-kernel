#include "common-chax.h"
#include "kthread.h"
#include "kernel-lib.h"

extern EWRAM_DATA volatile u32 gThreadTestCounter;

STATIC_DECLAR void kthread_idle(void)
{
	while (1) {
		if (gThreadSchedulerInfo.func)
			gThreadSchedulerInfo.func();

		YieldSubThread();
	}
}

void RegisterKthreadSchedulerTask(thread_task_func func)
{
	if (gThreadSchedulerInfo.func) {
		Warnf("Scheduler func replaced, old=0x%p, new=0x%p",
			gThreadSchedulerInfo.func, func);
	}

	gThreadSchedulerInfo.func = func;
}

void RemoveKthreadSchedulerTask(thread_task_func func)
{
	gThreadSchedulerInfo.func = NULL;
}

_maybe_unused STATIC_DECLAR void kthread_scheduler_test_task(void)
{
	gThreadTestCounter = 0;

	while (1) {
		gThreadTestCounter++;

		if (k_umod(gThreadTestCounter, 512) == 0) {
			// printf is banned, why?
			NoCashGBAPrint("(kthread_scheduler_test_task): 512 loop done");
		}

		if (k_umod(gThreadTestCounter, 32) == 0) {
			YieldSubThread();
		}
	}
}

void GameInit_SetupKthreadScheduler(void)
{
	gThreadSchedulerInfo.func = NULL;

	CreateSubThread(kthread_idle);

#ifdef CONFIG_DEBUG_THREAD_SCHEDULER
	FPrintf("Thread test counter at 0x%08X", &gThreadTestCounter);
	RegisterKthreadSchedulerTask(kthread_scheduler_test_task);
#endif
}
