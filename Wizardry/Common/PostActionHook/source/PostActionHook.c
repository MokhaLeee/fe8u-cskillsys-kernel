#include "global.h"
#include "proc.h"
#include "bmtrap.h"

#include "common-chax.h"
#include "combat-art.h"
#include "skill-system.h"
#include "battle-system.h"

struct ProcPostAction {
    PROC_HEADER;
    int index;
};

typedef bool (* PostActionFunc_t)(struct ProcPostAction * proc);
extern const PostActionFunc_t gPostActionFuncs[];

STATIC_DECLAR void PostActionExecHooks(struct ProcPostAction * proc);
STATIC_DECLAR void PostActionExecVanilla(struct ProcPostAction * proc);

STATIC_DECLAR const struct ProcCmd ProcScr_PostActionHook[] = {
    PROC_YIELD,

PROC_LABEL(1),
    PROC_CALL(PostActionExecHooks),
    PROC_YIELD,
    PROC_GOTO(1),

PROC_LABEL(2),
    PROC_CALL(PostActionExecVanilla),
    PROC_YIELD,
    PROC_END
};

void PostActionHook(ProcPtr parent)
{
    struct ProcPostAction * proc;
    proc = Proc_StartBlocking(ProcScr_PostActionHook, parent);
    proc->index = 0;
}

STATIC_DECLAR void PostActionExecHooks(struct ProcPostAction * proc)
{
    int ret;
    PostActionFunc_t it;

    while (1)
    {
        it = gPostActionFuncs[proc->index++];
        if (!it)
            goto post_action_done;

        ret = it(proc);
        if (ret != false)
            return;
    }

post_action_done:

    /* Some other proc-free routine */
    ClearBattleGlobalFlags();
    ResetCombatArtStatus();
    ResetSkillLists();
    ResetCombatArtList();

    Proc_Goto(proc, 2);
}

STATIC_DECLAR void PostActionExecVanilla(struct ProcPostAction * proc)
{
    /* Vanilla function at the hack entry */
    HandlePostActionTraps(proc);
}

bool PostActionPadFunc(struct ProcPostAction * proc)
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
