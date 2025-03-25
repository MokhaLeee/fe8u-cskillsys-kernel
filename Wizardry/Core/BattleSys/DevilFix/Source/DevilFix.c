#include "common-chax.h"
#include "battle-system.h"

extern const u8 DevilMaxRef;
extern const u8 DevilUnitStructRefOffset;

bool RollBaseDevilCheck(struct BattleUnit *bu)
{
	int devil_max, devil_base, devil_rate;

	devil_max  = DevilMaxRef;
	devil_base = *((u8 *)bu + DevilUnitStructRefOffset);
	devil_rate = devil_max - devil_base;

	if (devil_rate > 0 && BattleRoll1RN(devil_rate, FALSE))
		return true;

	return false;
}
