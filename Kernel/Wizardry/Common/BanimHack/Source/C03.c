#include "common-chax.h"

void Banim_C03(struct Anim *anim)
{
#ifndef CONFIG_C03_NOT_FLUSH_EFXSTATUS
	if (GetUnitEfxDebuff(gAnims[0]) & 0xC)
		SetUnitEfxDebuff(gAnims[0], 0);

	if (GetUnitEfxDebuff(gAnims[1]) & 0xC)
		SetUnitEfxDebuff(gAnims[1], 0);

	if (GetUnitEfxDebuff(gAnims[2]) & 0xC)
		SetUnitEfxDebuff(gAnims[2], 0);

	if (GetUnitEfxDebuff(gAnims[3]) & 0xC)
		SetUnitEfxDebuff(gAnims[3], 0);
#endif

	if (!(anim->state3 & ANIM_BIT3_BLOCKING)) {
		anim->state3 |= ANIM_BIT3_BLOCKING;
		if (GetAISLayerId(anim) == 0)
			NewEfxSpecalEffect(anim);
	}

	if (anim->state3 & ANIM_BIT3_BLOCKEND) {
		anim->state3 &= ~ANIM_BIT3_BLOCKING;
		anim->state3 &= ~ANIM_BIT3_BLOCKEND;
		anim->pScrCurrent = anim->pScrCurrent + 1;
	}
}
