#include "common-chax.h"
#include "map-anims.h"
#include "kernel-lib.h"
#include "battle-system.h"

struct ProcMapAnimHurt {
    PROC_HEADER;
    u32 lock;
    struct Unit * unit;
    int damage;
};

STATIC_DECLAR void MapAnimHurt_Init(struct ProcMapAnimHurt * proc)
{
    MapAnim_CommonInit();
}

STATIC_DECLAR void MapAnimHurt_MoveCamera(struct ProcMapAnimHurt * proc)
{
    EnsureCameraOntoPosition(proc, proc->unit->xPos, proc->unit->yPos);
}

STATIC_DECLAR void MapAnimHurt_ExecAnim(struct ProcMapAnimHurt * proc)
{
    proc->lock = GetGameLock();

    HideUnitSprite(proc->unit);
    BeginUnitCritDamageAnim(proc->unit, proc->damage);
}

STATIC_DECLAR void MapAnimHurt_Idle(struct ProcMapAnimHurt * proc)
{
    if (proc->lock == GetGameLock())
        Proc_Break(proc);
}

STATIC_DECLAR void MapAnimHurt_ExecBmHeal(struct ProcMapAnimHurt * proc)
{
    AddUnitHp(proc->unit, -proc->damage);
}

STATIC_DECLAR void MapAnimHurt_End(struct ProcMapAnimHurt * proc)
{
    MapAnim_CommonEnd();
    RestoreBattleRoundInfo();
}

STATIC_DECLAR const struct ProcCmd ProcScr_MapAnimHurt[] = {
    PROC_CALL(MapAnimHurt_Init),
    PROC_YIELD,
    PROC_CALL(MapAnimHurt_MoveCamera),
    PROC_YIELD,
    PROC_CALL(MapAnimHurt_ExecAnim),
    PROC_YIELD,
    PROC_REPEAT(MapAnimHurt_Idle),
    PROC_YIELD,
    PROC_CALL(MapAnimHurt_ExecBmHeal),
    PROC_CALL(MapAnimHurt_End),
    PROC_END
};

void CallMapAnim_Hurt(ProcPtr parent, struct Unit * unit, int damage)
{
    struct ProcMapAnimHurt * proc;

    if (!parent)
        proc = Proc_Start(ProcScr_MapAnimHurt, PROC_TREE_3);
    else
        proc = Proc_StartBlocking(ProcScr_MapAnimHurt, parent);

    proc->unit = unit;
    proc->damage = damage;
}

bool MapAnimHurtExists(void)
{
    return Proc_Exists(ProcScr_MapAnimHurt);
}
