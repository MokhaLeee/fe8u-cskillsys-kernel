#include <common-chax.h>
#include <banim-hack.h>
#include <battle-system.h>

STATIC_DECLAR void EfxMojiObjRework_HittedWithDamage(struct Anim *anim);

/**
 * NewEfxDamageMojiEffect has been called in:
 *
 * MISS: NewEfxAvoid: NewEfxDamageMojiEffect(anim, 1)
 * NODAMAGE: NewEfxNoDamage: NewEfxDamageMojiEffect(anim, 0)
 * HITTED: need hack
 */
LYN_REPLACE_CHECK(NewEfxFlashHPBar);
void NewEfxFlashHPBar(struct Anim *anim, int duartion, int duartion2)
{
	struct ProcEfxFlashing *proc;

	proc = Proc_Start(ProcScr_efxFlashHPBar, PROC_TREE_4);
	proc->anim = anim;
	proc->timer = 0;
	proc->terminator = duartion;
	proc->terminator2 = duartion2;

#if CHAX
	/**
	 * FlashHpBar will always display in hitted.
	 * So we inject hook here.
	 */
	NewEfxDamageMojiEffect(anim, 2);
#endif

	if (duartion == 0)
		Proc_Break(proc);
}

LYN_REPLACE_CHECK(NewEfxDamageMojiEffectOBJ);
void NewEfxDamageMojiEffectOBJ(struct Anim *anim, int hitted)
{
	u16 val1;
	u32 *anim_scr;
	struct ProcEfxDamageMojiEffectOBJ *proc;

#if CHAX
	if (hitted == 2) {
		/**
		 * CHAX: hitted with damage
		 */
		EfxMojiObjRework_HittedWithDamage(anim);
		return;
	}
#endif

	proc = Proc_Start(ProcScr_efxDamageMojiEffectOBJ, PROC_TREE_3);
	proc->anim = anim;
	proc->timer = 0;

	if (hitted == 0) {
		proc->terminator = 0x32;
		anim_scr = AnimScr_NoDamage;
	} else {
		proc->terminator = 0x32;
		anim_scr = AnimScr_Miss;
	}

	val1 = GetAnimPosition(anim) == EKR_POS_L ? 0x6100 : 0x5100;
	proc->sub_proc = NewEkrsubAnimeEmulator(
		anim->xPosition,
		anim->yPosition - 0x28,
		anim_scr,
		2, val1, 0, PROC_TREE_3
	);
}

NOINLINE STATIC_DECLAR void EfxMojiObjRework_HittedWithDamage(struct Anim *anim)
{
	int round = anim->nextRoundId - 1;
	struct BattleHit *hit = (prBattleHitArray + BattleHitArrayWidth * round);

	/**
	 * Only called when hitted with damage
	 */
	NewEfxAnimNumber(anim, -hit->hpChange);
}
