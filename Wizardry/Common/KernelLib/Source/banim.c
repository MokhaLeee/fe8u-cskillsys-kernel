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
