#include "common-chax.h"
#include "battle-system.h"

extern const u8 pr_Bow2DecreaseJdg[];
#define CheckPatch_Bow2Decrease() \
( \
	pr_Bow2DecreaseJdg[0] == 0x00 && \
	pr_Bow2DecreaseJdg[1] == 0x4B && \
	pr_Bow2DecreaseJdg[2] == 0x9F && \
	pr_Bow2DecreaseJdg[3] == 0x46 \
)

bool CheckWeaponCostForMissedBowAttack(struct BattleUnit *actor)
{
	if (CheckPatch_Bow2Decrease() && actor->weaponType == ITYPE_BOW)
		return true;

	return false;
}
