#include "common-chax.h"
#include "efx-skill.h"
#include "battle-system.h"

/**
 * C07: Start attack animation
 */
void Banim_C07(struct Anim *anim)
{
	/* We need to block the script */
	anim->pScrCurrent = anim->pScrCurrent - 1;

	if (!(anim->state3 & ANIM_BIT3_BLOCKING)) {
		anim->state3 |= ANIM_BIT3_BLOCKING;

		if (GetAISLayerId(anim) == 0) {
			NewEkrSkill(anim);
			BanimC07_UpdateHpCost(anim);
		}
	}

	if ((anim->state3 & ANIM_BIT3_BLOCKING) && !EfxHpCostExists()) {
		if (anim->state3 & ANIM_BIT3_BLOCKEND) {
			anim->state3 &= ~ANIM_BIT3_BLOCKING;
			anim->state3 &= ~ANIM_BIT3_BLOCKEND;
			anim->pScrCurrent = anim->pScrCurrent + 1;
		}
	}
}
