#include "common-chax.h"
#include "combat-art.h"
#include "skill-system.h"
#include "battle-system.h"

extern HookProcFunc_t const *const gpPostActionFuncs;

void PostActionHook(ProcPtr proc)
{
	KernelStartBlockingHookProc(gpPostActionFuncs, proc);
}

bool PostActionEnd(ProcPtr proc)
{
	/* Some other proc-free routine */
	ResetCombatArtStatus();
	ResetSkillLists();
	ResetCombatArtList();

	/* Vanilla function at the hack entry */
	HandlePostActionTraps(proc);

	return true;
}

bool PostActionPadFunc(ProcPtr proc)
{
	return false;
}

/* CHAX hooks */
const struct ProcCmd ProcScr_PostActionHookPlayPhase[] = {
	PROC_YIELD,
	PROC_CALL(PostActionHook),
	PROC_YIELD,
	PROC_JUMP((const struct ProcCmd *)0x0859ABE8),
	PROC_END
};

const struct ProcCmd ProcScr_PostActionHookCpPerform[] = {
	PROC_YIELD,
	PROC_CALL(PostActionHook),
	PROC_YIELD,
	PROC_JUMP((const struct ProcCmd *)0x085A807C),
	PROC_END
};

extern bool (*gpExternalPostActionHook)(ProcPtr proc);

bool CallExternalPostActionHook(ProcPtr proc)
{
	if (gpExternalPostActionHook)
		return gpExternalPostActionHook(proc);

	return false;
}
