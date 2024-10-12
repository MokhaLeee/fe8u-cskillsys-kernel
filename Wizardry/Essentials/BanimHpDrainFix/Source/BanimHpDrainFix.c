#include "common-chax.h"
#include "battle-system.h"

LYN_REPLACE_CHECK(StartBattleAnimHitEffectsDefault);
void StartBattleAnimHitEffectsDefault(struct Anim *anim, int type)
{
	int round = anim->nextRoundId - 1;

	if (gBattleHitArrayRe[round].attributes & BATTLE_HIT_ATTR_HPSTEAL) {
		StartBattleAnimResireHitEffects(anim, type);
		return;
	}

	StartBattleAnimHitEffects(anim, type, 3, 4);
}
