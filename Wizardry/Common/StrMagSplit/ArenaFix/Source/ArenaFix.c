#include "common-chax.h"
#include "strmag.h"
#include "kernel-lib.h"

int PowGetterFix_ArenaPlayer(struct Unit *unit)
{
	return (gArenaState.playerIsMagic)
		  ? GetUnitMagic(unit)
		  : GetUnitPower(unit);
}

int PowGetterFix_ArenaOpponent(struct Unit *unit)
{
	return (gArenaState.opponentIsMagic)
		  ? GetUnitMagic(unit)
		  : GetUnitPower(unit);
}

bool ArenaAdjustOpponentPowerRankingRework(void)
{
	bool ret = ArenaAdjustOpponentPowerRanking();

	if (ret) {
		if (gArenaState.playerPowerWeight < gArenaState.opponentPowerWeight) {
			if (UNIT_MAG(gArenaState.opponentUnit) != 0)
				UNIT_MAG(gArenaState.opponentUnit) -= 1;
		} else {
			if (UNIT_MAG(gArenaState.opponentUnit) < 30)
				UNIT_MAG(gArenaState.opponentUnit) += 1;
		}
	}
	return ret;
}
