#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

extern EWRAM_OVERLAY(0) s8 sTmpMovCostTable[0x41];

STATIC_DECLAR void CopyTerrainTable(s8 *dst, const s8 *src)
{
	int i;

	for (i = 0; i < TERRAIN_COUNT; i++)
		dst[i] = src[i];
}

LYN_REPLACE_CHECK(GetUnitMovementCost);
const s8 *GetUnitMovementCost(struct Unit *unit)
{
	const s8 *cost_src;

	/**
	 * Well I wanna not to touch ballista
	 */
    if (unit->state & US_IN_BALLISTA)
        return Unk_TerrainTable_0880BC18;

    switch (gPlaySt.chapterWeatherId) {
    case WEATHER_RAIN:
		cost_src = unit->pClassData->pMovCostTable[1];
		break;

    case WEATHER_SNOW:
    case WEATHER_SNOWSTORM:
        cost_src = unit->pClassData->pMovCostTable[2];
		break;

    default:
        cost_src = unit->pClassData->pMovCostTable[0];
		break;
    }

	CopyTerrainTable(sTmpMovCostTable, cost_src);

#if (defined(SID_SeaWays) && COMMON_SKILL_VALID(SID_SeaWays))
	if (SkillTester(unit, SID_SeaWays)) {
		if (sTmpMovCostTable[TERRAIN_RIVER] < 0)
			sTmpMovCostTable[TERRAIN_RIVER] = 2;

		if (sTmpMovCostTable[TERRAIN_SEA] < 0)
			sTmpMovCostTable[TERRAIN_SEA] = 2;

		if (sTmpMovCostTable[TERRAIN_LAKE] < 0)
			sTmpMovCostTable[TERRAIN_LAKE] = 2;
	}
#endif

	return sTmpMovCostTable;
}
