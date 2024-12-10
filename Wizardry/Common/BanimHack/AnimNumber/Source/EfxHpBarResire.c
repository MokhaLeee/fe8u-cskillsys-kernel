#include <common-chax.h>
#include <banim-hack.h>
#include <battle-system.h>

void EfxHpBarResire_DisplayAnimNumber(struct ProcEfxHpBar *proc)
{
	struct Anim *anim = proc->anim_main_other;

	if (++proc->timer < 4)
		return;

	NewEfxAnimNumber(
		anim,
		GetExtBattleHit(anim->nextRoundId - 1)->hp_drain);

	proc->timer = 0;
	Proc_Break(proc);
}

/* External hook */
const struct ProcCmd ProcScr_EfxHpBarResireRework[] = {
	PROC_NAME("efxHPBarResire"),
	PROC_REPEAT(EfxHpBarResire_WaitOnCurrentSide),
	PROC_REPEAT(EfxHpBarResire_SetAnotherSide),

#if CHAX
	PROC_REPEAT(EfxHpBarResire_DisplayAnimNumber),
#endif

	PROC_REPEAT(EfxHpBarResire_DeclineToDeath),
	PROC_REPEAT(EfxHpBar_MoveCameraOnEnd),
	PROC_REPEAT(EfxHpBar_WaitCameraMove),
	PROC_END
};
