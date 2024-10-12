#include "common-chax.h"

extern void * const prBattleHitArray;
extern u8 BattleHitArrayWidth;

LYN_REPLACE_CHECK(StartBattleAnimHitEffectsDefault);
void StartBattleAnimHitEffectsDefault(struct Anim *anim, int type)
{
	int round = anim->nextRoundId - 1;
	struct BattleHit *hit = (prBattleHitArray + BattleHitArrayWidth * round);

	if (hit->attributes & BATTLE_HIT_ATTR_HPSTEAL) {
		StartBattleAnimResireHitEffects(anim, type);
		gEfxHpBarResireFlag = 3;
		return;
	}

	StartBattleAnimHitEffects(anim, type, 3, 4);
}

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

	if (proc->timer2 == 0x54 && proc->finished == true) {
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
