#include <common-chax.h>
#include <kernel-lib.h>
#include <battle-system.h>

#define LOCAL_TRACE 0

struct ProcEfxHpCost {
	PROC_HEADER;
	int hpcur, hpend, diff;
	int timer, step;
	int cost;
	struct Anim *anim;
};

/**
 * Actually it is better to directly modify function: ParseBattleHitToBanimCmd() to inset hp changes.
 * But well I wanna not to touch that function as that function has been touched too much times in external patches.
 * Thus I directly use some side ways to directly hook at the end of C07!!!!
 */

STATIC_DECLAR void NewEfxHpCost(struct Anim *anim, int cost);

void BanimC07_UpdateHpCost(struct Anim *anim)
{
	int cost, round;

	if (GetAISLayerId(anim) != 0)
		return;

	if (!IsAttackerAnim(anim))
		return;

	round = anim->nextRoundId - 1;
	if (round < 0 || round >= NEW_BATTLE_HIT_MAX)
		return;

	cost = gBattleHpCostArray[round].cost;
	if (cost > 0)
		NewEfxHpCost(anim, cost);
}

STATIC_DECLAR void EfxHpCost_Loop(struct ProcEfxHpCost *proc)
{
	int pos;

	if (proc->hpcur == proc->hpend) {
		Proc_Break(proc);
		return;
	}

	if (proc->timer > 0) {
		proc->timer--;
		return;
	}

	proc->timer = proc->step;
	proc->hpcur += proc->diff;

	pos = GetAnimPosition(proc->anim);
	gEkrGaugeHp[pos] = proc->hpcur;
}

STATIC_DECLAR void EfxHpCost_End(struct ProcEfxHpCost *proc)
{
	int i, pos = GetAnimPosition(proc->anim);

	/* Yeah, directly flush the whole array! */
	for (i = gEfxHpLutOff[pos]; i < NEW_BATTLE_HIT_MAX; i++)
		prEfxHpLutRe[i * 2 + pos] -= proc->cost;
}

STATIC_DECLAR const struct ProcCmd ProcScr_EfxHpCost[] = {
	PROC_YIELD,
	PROC_REPEAT(EfxHpCost_Loop),
	PROC_CALL(EfxHpCost_End),
	PROC_END
};

STATIC_DECLAR void NewEfxHpCost(struct Anim *anim, int cost)
{
	struct ProcEfxHpCost *proc;
	int pos = GetAnimPosition(anim);

	proc = Proc_Start(ProcScr_EfxHpCost, PROC_TREE_3);
	proc->step = 2;
	proc->timer = proc->step;
	proc->hpcur = gEkrGaugeHp[pos];
	proc->hpend = proc->hpcur - cost;
	proc->diff = -1;
	proc->cost = cost;
	proc->anim = anim;
}
