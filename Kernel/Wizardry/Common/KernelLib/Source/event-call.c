#include "common-chax.h"
#include "kernel-lib.h"

struct ProcKernelEventCaller {
	PROC_HEADER;

	u8 exec_type;
	const EventScr *eventscr;
};

STATIC_DECLAR void KernelEventCaller_Ext(struct ProcKernelEventCaller *proc)
{
	CallEvent((const void *)proc->eventscr, proc->exec_type);
}

STATIC_DECLAR const struct ProcCmd ProcScr_KernelEventCaller[] = {
	PROC_YIELD,
	PROC_CALL(KernelEventCaller_Ext),
	PROC_YIELD,
	PROC_WHILE(EventEngineExists),
	PROC_END
};

ProcPtr KernelCallEvent(const EventScr *eventscr, u8 exec_type, ProcPtr parent)
{
	struct ProcKernelEventCaller *proc;

	if (!parent) {
		CallEvent((const void *)eventscr, exec_type);
		return NULL;
	}

	proc = Proc_StartBlocking(ProcScr_KernelEventCaller, parent);
	proc->eventscr = eventscr;
	proc->exec_type = exec_type;
	return proc;
}
