#include "common-chax.h"
#include "battle-system.h"

bool IsAttackerAnim(struct Anim *anim)
{
	if (GetAnimPosition(anim) == POS_L) {
		if (gpEkrBattleUnitLeft == &gBattleActor)
			return true;
	} else {
		if (gpEkrBattleUnitRight == &gBattleActor)
			return true;
	}
	return false;
}

bool IsActorAnim(struct Anim *anim)
{
	switch (GetAnimRoundType(anim)) {
	case ANIM_ROUND_HIT_CLOSE:
	case ANIM_ROUND_CRIT_CLOSE:
	case ANIM_ROUND_NONCRIT_FAR:
	case ANIM_ROUND_CRIT_FAR:
	case ANIM_ROUND_MISS_CLOSE:
		return true;

	case ANIM_ROUND_TAKING_MISS_CLOSE:
	case ANIM_ROUND_TAKING_MISS_FAR:
	case ANIM_ROUND_TAKING_HIT_CLOSE:
	case ANIM_ROUND_STANDING:
	case ANIM_ROUND_TAKING_HIT_FAR:
	default:
		return false;
	}
}
