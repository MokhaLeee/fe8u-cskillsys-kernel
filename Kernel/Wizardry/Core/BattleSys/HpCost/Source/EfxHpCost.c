#include <common-chax.h>
#include <kernel-lib.h>
#include <battle-system.h>
#include <banim-hack.h>
#include <efx-skill.h>

#define LOCAL_TRACE 0

struct ProcEfxHpCost {
	PROC_HEADER;
	int hpcur, hpend, diff;
	int timer, step;
	int sound_timer;
	struct Anim *anim;
};

STATIC_DECLAR void EfxHpCost_Start(struct ProcEfxHpCost *proc) {}

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

	EfxPlaySE(SONG_75, 0x100);
	M4aPlayWithPostionCtrl(SONG_75, proc->anim->xPosition, 1);
}

STATIC_DECLAR void EfxHpCost_End(struct ProcEfxHpCost *proc)
{
	gEfxHpLutOff[GetAnimPosition(proc->anim)]++;
}

STATIC_DECLAR const struct ProcCmd ProcScr_EfxHpCost[] = {
	PROC_YIELD,
	PROC_CALL(EfxHpCost_Start),
	PROC_REPEAT(EfxHpCost_Loop),

	// We can fasten the banim not to waiting anim-number done
	// PROC_WHILE(EfxAnimNumberExists),

	PROC_CALL(EfxHpCost_End),
	PROC_END
};

void NewEfxHpCost(struct Anim *anim)
{
	struct ProcEfxHpCost *proc;
	int pos = GetAnimPosition(anim);

	proc = Proc_Start(ProcScr_EfxHpCost, PROC_TREE_3);
	proc->step = 4;
	proc->timer = proc->step;
	proc->sound_timer = 0;
	proc->hpcur = GetEfxHp((gEfxHpLutOff[pos] + 0) * 2 + pos);
	proc->hpend = GetEfxHp((gEfxHpLutOff[pos] + 1) * 2 + pos);
	proc->diff = -1;
	proc->anim = anim;

	NewEfxAnimNumber(
		anim,
		-GetExtBattleHit(anim->nextRoundId - 1)->hp_cost);

	LTRACEF("round=%d, off=%d, cur=%d, next=%d", anim->nextRoundId - 1, gEfxHpLutOff[pos], proc->hpcur, proc->hpend);
}

bool EfxHpCostExists(void)
{
	return Proc_Exists(ProcScr_EfxHpCost);
}
