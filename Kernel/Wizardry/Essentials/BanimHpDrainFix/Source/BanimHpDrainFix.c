#include "common-chax.h"

LYN_REPLACE_CHECK(EfxHpBarResire_WaitOnCurrentSide);
void EfxHpBarResire_WaitOnCurrentSide(struct ProcEfxHpBar *proc)
{
	GetAnimPosition(proc->anim_main_this);
	GetAnimPosition(proc->anim_main_this);

	if (proc->finished == false) {
		if (++proc->timer == 2) {
			proc->timer = 0;
			proc->cur += proc->diff;
			gEkrGaugeHp[GetAnimPosition(proc->anim_main_this)] += proc->diff;

			if (proc->cur == proc->next)
				proc->finished = true;
		}
	}

#if CHAX
	if (gEfxHpBarResireFlag == 3)
		proc->timer2 = 0x54;
#endif

	if (proc->timer2 >= 0x54 && proc->finished == true) {
		gEfxHpLutOff[GetAnimPosition(proc->anim_main_this)]++;
		gEkrHitNow[GetAnimPosition(proc->anim_main_this)] = 0;

		if (proc->next == 0)
			proc->death = true;

		proc->timer = 0;
		proc->cur = 10;
		gEfxHpBarResireFlag = 1;

		Proc_Break(proc);
		return;
	}

	if (++proc->timer2 >= 0x54)
		proc->timer2 = 0x54;
}
