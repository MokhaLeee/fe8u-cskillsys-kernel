#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void TileCheck(struct Unit *unit)
{
#if defined(SID_IndoorMarch) && (COMMON_SKILL_VALID(SID_IndoorMarch))
	if (SkillTester(unit, SID_IndoorMarch))
		switch (gBmMapTerrain[unit->yPos][unit->xPos]) {
		case TERRAIN_FORT:
		case TERRAIN_FLOOR_17:
		case TERRAIN_FLOOR_18:
		case TERRAIN_WALL_1A:
		case TERRAIN_WALL_1B:
		case TERRAIN_RUBBLE:
		case TERRAIN_PILLAR:
		case TERRAIN_DOOR:
		case TERRAIN_THRONE:
		case TERRAIN_CHEST_20:
		case TERRAIN_CHEST_21:
		case TERRAIN_ROOF:
		case TERRAIN_STAIRS:
		case TERRAIN_TILE_2E:
		case TERRAIN_GLACIER:
		case TERRAIN_GUNNELS:
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INDOOR_MARCH_MOV);
			break;

		default:
			break;
		}
#endif

#if defined(SID_NatureRush) && (COMMON_SKILL_VALID(SID_NatureRush))
	if (SkillTester(unit, SID_NatureRush))
		switch (gBmMapTerrain[unit->yPos][unit->xPos]) {
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
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_NATURE_RUSH_MOV);
			break;

		default:
			break;
		}
#endif
}

bool PrePhase_CheckTile(ProcPtr proc)
{
	FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
		if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

		TileCheck(unit);
	})

	return false;
}
