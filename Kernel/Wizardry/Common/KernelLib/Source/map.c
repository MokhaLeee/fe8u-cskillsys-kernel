#include "common-chax.h"
#include "kernel-lib.h"

inline bool IsPositionValid(s8 x, s8 y)
{
	if (x < 0)
		return false;

	if (y < 0)
		return false;

	if (x > gBmMapSize.x)
		return false;

	if (y > gBmMapSize.y)
		return false;

	return true;
}

struct Unit *GetUnitAtPosition(s8 x, s8 y)
{
	s8 uid;

	if (!IsPositionValid(x, y))
		return NULL;

	uid = gBmMapUnit[y][x];
	if (!uid)
		return NULL;

	return GetUnit(uid);
}

bool CheckOutdoorTerrain(int terrain)
{
	switch (terrain) {
	case TERRAIN_PLAINS:
	case TERRAIN_ROAD:
	case TERRAIN_VILLAGE_03:
	case TERRAIN_VILLAGE_04:
	case TERRIAN_HOUSE:
	case TERRAIN_ARMORY:
	case TERRAIN_VENDOR:
	case TERRAIN_ARENA_08:
	case TERRAIN_C_ROOM_09:
	case TERRAIN_GATE_0B:
	case TERRAIN_FOREST:
	case TERRAIN_THICKET:
	case TERRAIN_SAND:
	case TERRAIN_DESERT:
	case TERRAIN_RIVER:
	case TERRAIN_MOUNTAIN:
	case TERRAIN_PEAK:
	case TERRAIN_BRIDGE_13:
	case TERRAIN_BRIDGE_14:
	case TERRAIN_SEA:
	case TERRAIN_LAKE:
	case TERRAIN_GATE_23:
	case TERRAIN_CHURCH:
	case TERRAIN_RUINS_25:
	case TERRAIN_CLIFF:
	case TERRAIN_BALLISTA_REGULAR:
	case TERRAIN_BALLISTA_LONG:
	case TERRAIN_BALLISTA_KILLER:
	case TERRAIN_SHIP_FLAT:
	case TERRAIN_SHIP_WRECK:
	case TERRAIN_TILE_2C:
	case TERRAIN_ARENA_30:
	case TERRAIN_VALLEY:
	case TERRAIN_FENCE_32:
	case TERRAIN_SNAG:
	case TERRAIN_BRIDGE_34:
	case TERRAIN_SKY:
	case TERRAIN_DEEPS:
	case TERRAIN_RUINS_37:
	case TERRAIN_INN:
	case TERRAIN_BARREL:
	case TERRAIN_BONE:
	case TERRAIN_DARK:
	case TERRAIN_WATER:
	case TERRAIN_DECK:
	case TERRAIN_BRACE:
	case TERRAIN_MAST:
		return true;

	default:
		return false;
	}
}
