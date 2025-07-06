#include "common-chax.h"
#include "kthread.h"
#include "kernel-lib.h"

// For test
#define LOCAL_TRACE 1

_maybe_unused STATIC_DECLAR void SubTask_PlayPhaseTest(void)
{
	enum { LOCAL_TRIGG = 0xFFFF };

	u32 total = 0;
	u32 cnt = 0;
	u32 rn = NextRN_N(LOCAL_TRIGG);

	while (1) {
		total++;
		cnt++;

		if (cnt > rn) {
			LTRACEF("trace loop 0x%04X, total 0x%08X", cnt, total);

			cnt = 0;
			rn = NextRN_N(LOCAL_TRIGG);
			YieldSubThread();
		}
	}
}

// external hook
void PlayerPhase_InitUnitMovementSelectRework(void)
{
#ifdef CONFIG_DEBUG_THREAD_SCHEDULER
	CreateSubThread(SubTask_PlayPhaseTest);
#endif

	PlayerPhase_InitUnitMovementSelect();
}

// external hook
void PlayerPhase_RangeDisplayIdleRework(struct Proc *proc)
{
	PlayerPhase_RangeDisplayIdle(proc);

	if (proc->proc_idleCb == NULL) {
#ifdef CONFIG_DEBUG_THREAD_SCHEDULER
		RemoveSubThread(SubTask_PlayPhaseTest);
#endif
	}
}
