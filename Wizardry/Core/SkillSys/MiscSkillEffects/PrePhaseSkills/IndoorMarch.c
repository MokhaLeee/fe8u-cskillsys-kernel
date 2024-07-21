#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"

static void IndoorMarchCheck(struct Unit * unit)
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
}

bool PrePhase_CheckIndoorMarch(ProcPtr proc)
{
    int uid;

    switch (gPlaySt.faction) {
    case FACTION_BLUE:
        for (uid = FACTION_BLUE + 1; uid < FACTION_BLUE + 1 + CONFIG_UNIT_AMT_ALLY; uid++)
        {
            struct Unit * unit = GetUnit(uid);

            if (!UNIT_IS_VALID(unit))
                continue;

            if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;
                
            IndoorMarchCheck(unit);
        }
        break;

    case FACTION_GREEN:
        for (uid = FACTION_GREEN + 1; uid < FACTION_GREEN + 1 + CONFIG_UNIT_AMT_NPC; uid++)
        {
            struct Unit * unit = GetUnit(uid);

            if (!UNIT_IS_VALID(unit))
                continue;

            if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;
                
            IndoorMarchCheck(unit);
        }
        break;

    case FACTION_RED:
        for (uid = FACTION_RED + 1; uid < FACTION_RED + 1 + CONFIG_UNIT_AMT_ENEMY; uid++)
        {
            struct Unit * unit = GetUnit(uid);

            if (!UNIT_IS_VALID(unit))
                continue;

            if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;
                
            IndoorMarchCheck(unit);
        }
        break;
    }

    return false;
}
