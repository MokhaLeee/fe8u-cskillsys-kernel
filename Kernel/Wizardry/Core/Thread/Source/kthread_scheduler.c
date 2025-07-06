#include "common-chax.h"
#include "kthread.h"

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

void GameInit_SetupKthreadScheduler(void)
{
	gThreadSchedulerInfo.func = NULL;

	CreateSubThread(kthread_idle);
}
