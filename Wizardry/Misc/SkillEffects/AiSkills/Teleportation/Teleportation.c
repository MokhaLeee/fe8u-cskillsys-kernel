#include "common-chax.h"
#include "status-getter.h"

static int GetEnemyFaction(int faction)
{
	switch (faction) {
	case FACTION_BLUE:
	case FACTION_GREEN:
		return FACTION_RED;

	case FACTION_RED:
		return FACTION_BLUE;

	default:
		return FACTION_BLUE;
	}
}

bool GetTeleportationRandomPosition(struct Unit *unit, struct Vec2 *out)
{
	#define RETRY_CNT 100

	int i, ix, iy;
	int enemy_faction;

	enemy_faction = GetEnemyFaction(UNIT_FACTION(gActiveUnit));

	for (i = 1; i < RETRY_CNT; i++) {
		struct Unit *unit = GetUnit((enemy_faction + 1 + NextRN_N(CONFIG_UNIT_AMT_ALLY - 1)));

		if (!UNIT_IS_VALID(unit))
			continue;

		if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

		BmMapFill(gBmMapMovement, -1);
		SetWorkingMoveCosts(GetUnitMovementCost(unit));
		GenerateMovementMap(
			unit->xPos, unit->yPos,
			MovGetter(gActiveUnit),
			unit->index);

		break;
	}

	if (i == RETRY_CNT)
		return false;

	/* Polling a target position */
	for (i = 0; i < RETRY_CNT; i++) {
		ix = NextRN_N(gBmMapSize.x);
		iy = NextRN_N(gBmMapSize.y);

		if (gBmMapUnit[iy][ix] == 0 && (s8) gBmMapMovement[iy][ix] >= 0 && CanUnitCrossTerrain(gActiveUnit, gBmMapTerrain[iy][ix])) {
			/* Found it! */
			out->x = ix;
			out->y = iy;
			return true;
		}
	}
	return false;
}
