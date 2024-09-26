#include "common-chax.h"
#include "map-anims.h"
#include "kernel-lib.h"
#include "battle-system.h"

struct ProcMapAnimHeal {
	PROC_HEADER;
	u32 lock;
	struct Unit *unit;
	int heal;
	void (*callback1)(ProcPtr proc);
	void (*callback2)(ProcPtr proc);
};

STATIC_DECLAR void MapAnimHeal_Init(struct ProcMapAnimHeal *proc)
{
	MapAnim_CommonInit();
}

STATIC_DECLAR void MapAnimHeal_MoveCamera(struct ProcMapAnimHeal *proc)
{
	EnsureCameraOntoPosition(proc, proc->unit->xPos, proc->unit->yPos);
}

STATIC_DECLAR void MapAnimHeal_ExecCallBack1(struct ProcMapAnimHeal *proc)
{
	if (proc->callback1)
		proc->callback1(proc);
}

STATIC_DECLAR void MapAnimHeal_ExecCallBack2(struct ProcMapAnimHeal *proc)
{
	if (proc->callback2)
		proc->callback2(proc);
}

STATIC_DECLAR void MapAnimHeal_ExecAnim(struct ProcMapAnimHeal *proc)
{
	proc->lock = GetGameLock();

	HideUnitSprite(proc->unit);
	BeginUnitHealAnim(proc->unit, proc->heal);
}

STATIC_DECLAR void MapAnimHeal_Idle(struct ProcMapAnimHeal *proc)
{
	if (proc->lock == GetGameLock())
		Proc_Break(proc);
}

STATIC_DECLAR void MapAnimHeal_ExecBmHeal(struct ProcMapAnimHeal *proc)
{
	AddUnitHp(proc->unit, proc->heal);
}

STATIC_DECLAR void MapAnimHeal_End(struct ProcMapAnimHeal *proc)
{
	MapAnim_CommonEnd();
}

STATIC_DECLAR const struct ProcCmd ProcScr_MapAnimHeal[] = {
	PROC_CALL(MapAnimHeal_Init),
	PROC_YIELD,
	PROC_CALL(MapAnimHeal_MoveCamera),
	PROC_YIELD,
	PROC_CALL(MapAnimHeal_ExecCallBack1),
	PROC_YIELD,
	PROC_CALL(MapAnimHeal_ExecAnim),
	PROC_YIELD,
	PROC_REPEAT(MapAnimHeal_Idle),
	PROC_YIELD,
	PROC_CALL(MapAnimHeal_ExecBmHeal),
	PROC_YIELD,
	PROC_CALL(MapAnimHeal_ExecCallBack2),
	PROC_YIELD,
	PROC_CALL(MapAnimHeal_End),
	PROC_END
};

void CallMapAnim_HealExt(ProcPtr parent, struct Unit *unit, int heal, void (*callback1)(ProcPtr proc), void (*callback2)(ProcPtr proc))
{
	struct ProcMapAnimHeal *proc;

	if (!parent)
		proc = Proc_Start(ProcScr_MapAnimHeal, PROC_TREE_3);
	else
		proc = Proc_StartBlocking(ProcScr_MapAnimHeal, parent);

	proc->unit = unit;
	proc->heal = heal;
	proc->callback1 = callback1;
	proc->callback2 = callback2;
}

void CallMapAnim_Heal(ProcPtr parent, struct Unit *unit, int heal)
{
	CallMapAnim_HealExt(parent, unit, heal, NULL, NULL);
}

bool MapAnimHealExists(void)
{
	return Proc_Exists(ProcScr_MapAnimHeal);
}
