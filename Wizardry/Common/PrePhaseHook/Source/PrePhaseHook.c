#include "common-chax.h"

struct ProcPrePhase {
	PROC_HEADER;
	int index;
};

typedef bool (*PrePhaseFunc_t)(struct ProcPrePhase *proc);
extern const PrePhaseFunc_t gPrePhaseFuncs[];

static void PrePhaseHook_Init(struct ProcPrePhase *proc);
static void PrePhaseHook_Main(struct ProcPrePhase *proc);

const struct ProcCmd ProcScr_PrePhaseHook[] = {
	PROC_YIELD,
	PROC_CALL(PrePhaseHook_Init),

PROC_LABEL(1),
	PROC_CALL(PrePhaseHook_Main),
	PROC_YIELD,
	PROC_GOTO(1),

	/* Vanilla */
PROC_LABEL(2),
	PROC_START_CHILD_BLOCKING(gProcScr_GorgonEggHatchDisplay),
	PROC_END
};

static void PrePhaseHook_Init(struct ProcPrePhase *proc)
{
	proc->index = 0;
}

static void PrePhaseHook_Main(struct ProcPrePhase *proc)
{
	int ret;
	PrePhaseFunc_t it;

	while (1) {
		it = gPrePhaseFuncs[proc->index++];
		if (!it)
			goto post_action_done;

		ret = it(proc);
		if (ret != false)
			return;
	}

post_action_done:
	Proc_Goto(proc, 2);
}
