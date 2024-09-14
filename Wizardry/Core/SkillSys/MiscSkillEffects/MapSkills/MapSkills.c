#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "mu.h"
#include "debuff.h"
#include "class-pairs.h"

/*
** This array is static in the decomp, but it wouldn't compile for me otherwise
** I also had to define it in the vanilla.h file
*/
EWRAM_DATA u16 sTilesetConfig[0x1000 + 0x200] = {};

LYN_REPLACE_CHECK(RefreshUnitsOnBmMap);
void RefreshUnitsOnBmMap(void)
{
    struct Unit *unit;
    int i;

    // 1. Blue & Green units

    for (i = 1; i < FACTION_RED; ++i)
    {
        unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_HIDDEN)
            continue;

#if defined(SID_MagicSeal) && (COMMON_SKILL_VALID(SID_MagicSeal))
        if (SkillTester(unit, SID_MagicSeal))
        {
            MapAddInRange(unit->xPos, unit->yPos, 10, -1);
            gBmMapUnit[unit->yPos][unit->xPos] = i;

            if (unit->state & US_BIT9)
                unit->state = (unit->state & ~US_BIT9) | US_BIT8;
        }
#endif

        // Put unit on unit map
        gBmMapUnit[unit->yPos][unit->xPos] = i;

        // If fog is enabled, apply unit vision to fog map
        if (gPlaySt.chapterVisionRange)
            MapAddInRange(unit->xPos, unit->yPos, GetUnitFogViewRange(unit), 1);
    }

    // 2. Red (& Purple) units

    if (gPlaySt.faction != FACTION_RED)
    {
        // 2.1. No red phase

        for (i = FACTION_RED + 1; i < FACTION_PURPLE + 6; ++i)
        {
            unit = GetUnit(i);

            if (!UNIT_IS_VALID(unit))
                continue;

            if (unit->state & US_HIDDEN)
                continue;

            // If unit is magic seal, set fog in range 0-10.
            // Magic seal set the fog map probably solely for the alternate map palette.
            if (UNIT_CATTRIBUTES(unit) & CA_MAGICSEAL)
                MapAddInRange(unit->xPos, unit->yPos, 10, -1);

            if (gPlaySt.chapterVisionRange && !gBmMapFog[unit->yPos][unit->xPos])
            {
                // If in fog, set unit bit on the hidden map, and set the "hidden in fog" state

                gBmMapHidden[unit->yPos][unit->xPos] |= HIDDEN_BIT_UNIT;
                unit->state = unit->state | US_BIT9;
            }
            else
            {
                // If not in fog, put unit on the map, and update state accordingly

                gBmMapUnit[unit->yPos][unit->xPos] = i;

                if (unit->state & US_BIT9)
                    unit->state = (unit->state & ~US_BIT9) | US_BIT8;
            }
        }
    }
    else
    {
        // 2.2. Yes red phase

        // This does mostly the same as the "No red phase" loop, except:
        // - It always puts the units on the unit map
        // - It never sets the "spotted" unit state bit (even if unit is seen)

        for (i = FACTION_RED + 1; i < FACTION_PURPLE + 6; ++i)
        {
            unit = GetUnit(i);

            if (!UNIT_IS_VALID(unit))
                continue;

            if (unit->state & US_HIDDEN)
                continue;

            // See above
            if (UNIT_CATTRIBUTES(unit) & CA_MAGICSEAL)
                MapAddInRange(unit->xPos, unit->yPos, 10, -1);

            if (gPlaySt.chapterVisionRange)
            {
                // Update unit state according to fog level

                if (!gBmMapFog[unit->yPos][unit->xPos])
                    unit->state = unit->state | US_BIT9;
                else
                    unit->state = unit->state & ~US_BIT9;
            }

            // Put on unit map
            gBmMapUnit[unit->yPos][unit->xPos] = i;
        }
    }
}

LYN_REPLACE_CHECK(DisplayBmTile);
void DisplayBmTile(u16 *bg, int xTileMap, int yTileMap, int xBmMap, int yBmMap)
{
    u16 *out = bg + yTileMap * 0x40 + xTileMap * 2; // TODO: BG_LOCATED_TILE?
    u16 *tile = sTilesetConfig + gBmMapBaseTiles[yBmMap][xBmMap];

    // TODO: palette id constants
    u16 base = gBmMapFog[yBmMap][xBmMap] ? (6 << 12) : (11 << 12);

    out[0x00 + 0] = base + *tile++;
    out[0x00 + 1] = base + *tile++;
    out[0x20 + 0] = base + *tile++;
    out[0x20 + 1] = base + *tile++;
}

LYN_REPLACE_CHECK(GetUnitFogViewRange);
int GetUnitFogViewRange(struct Unit *unit)
{
    int result = gPlaySt.chapterVisionRange;

    if (UNIT_CATTRIBUTES(unit) & CA_THIEF)
        result += 5;
    else
    {
#if defined(SID_HazeHunter) && (COMMON_SKILL_VALID(SID_HazeHunter))
        if (SkillTester(unit, SID_HazeHunter))
            result += 5;
#endif
    }

    return result + unit->torchDuration;
}

LYN_REPLACE_CHECK(UnitApplyWorkingMovementScript);
void UnitApplyWorkingMovementScript(struct Unit *unit, int x, int y)
{
    u8 *it = gWorkingMovementScript;

    for (;;)
    {
        gActionData.xMove = x;
        gActionData.yMove = y;

        switch (*it)
        {

        case MOVE_CMD_MOVE_UP: // up
            y--;
            break;

        case MOVE_CMD_MOVE_DOWN: // down
            y++;
            break;

        case MOVE_CMD_MOVE_LEFT: // left
            x--;
            break;

        case MOVE_CMD_MOVE_RIGHT: // right
            x++;
            break;

        } // switch (*it)

        if (!(UNIT_CATTRIBUTES(unit) & (CA_THIEF | CA_FLYER | CA_ASSASSIN)))
        {
            if (gBmMapHidden[y][x] & HIDDEN_BIT_TRAP)
            {
                *++it = MOVE_CMD_HALT;

                gActionData.unitActionType = UNIT_ACTION_TRAPPED;
                gActionData.xMove = x;
                gActionData.yMove = y;

                return;
            }
        }
        if (gBmMapHidden[y][x] & HIDDEN_BIT_UNIT)
        {
            *it++ = MOVE_CMD_BUMP;
            *it++ = MOVE_CMD_HALT;

#if defined(SID_Reflex) && (COMMON_SKILL_VALID(SID_Reflex))
            if (!SkillTester(unit, SID_Reflex))
                gActionData.unitActionType = UNIT_ACTION_TRAPPED;
#else
            gActionData.unitActionType = UNIT_ACTION_TRAPPED;
#endif
            return;
        }

        if (*it == MOVE_CMD_HALT)
            break;

        it++;
    }
}

LYN_REPLACE_CHECK(ChapterChangeUnitCleanup);
void ChapterChangeUnitCleanup(void)
{
    int i, j;

    // Clear phantoms
    for (i = 1; i < 0x40; ++i)
    {
        struct Unit *unit = GetUnit(i);

        if (unit && unit->pCharacterData)
            if (UNIT_IS_PHANTOM(unit))
                ClearUnit(unit);
    }

    // Clear all non player units (green & red units)
    for (i = 0x41; i < 0xC0; ++i)
    {
        struct Unit *unit = GetUnit(i);

        if (unit && unit->pCharacterData)
            ClearUnit(unit);
    }

    // Reset player unit "temporary" states (HP, status, some state flags, etc)
    for (j = 1; j < 0x40; ++j)
    {
        struct Unit *unit = GetUnit(j);

    // Reset the transformed state of any units with the skill
#if defined(SID_Transform) && (COMMON_SKILL_VALID(SID_Transform))
        if (SkillTester(unit, SID_Transform))
        {
            for (int i = 0; i < transformationListSize; i++)
            {
                if (gActiveUnit->pClassData->number == transformationPairs[i][1])
                {
                    unit->pClassData = GetClassData(transformationPairs[i][0]);
                    ClearUnitStatDebuff(gActiveUnit, UNIT_STAT_BUFF_TRANSFORM);
                    unit->maxHP -= 7;
                    break;
                }
            }
        }
#endif

    // Reset the doppleganger state of any units with the skill
#if defined(SID_Doppleganger) && (COMMON_SKILL_VALID(SID_Doppleganger))
        if (SkillTester(unit, SID_Doppleganger))
        {
            for (int i = 0; i < dopplegangerListSize; i++)
            {
                if (gActiveUnit->pCharacterData->number == dopplegangerPairs[i][0])
                {
                    unit->pClassData = GetClassData(dopplegangerPairs[i][1]);
                    break;
                }
            }
        }
#endif

        if (unit && unit->pCharacterData)
        {
            SetUnitHp(unit, GetUnitMaxHp(unit));
            SetUnitStatus(unit, UNIT_STATUS_NONE);

            unit->torchDuration = 0;
            unit->barrierDuration = 0;

            if (unit->state & US_NOT_DEPLOYED)
                unit->state = unit->state | US_BIT21;
            else
                unit->state = unit->state & ~US_BIT21;

            unit->state &= (US_DEAD | US_GROWTH_BOOST | US_SOLOANIM_1 | US_SOLOANIM_2 |
                            US_BIT16 | US_BIT20 | US_BIT21 | US_BIT25 | US_BIT26);

            if (UNIT_CATTRIBUTES(unit) & CA_SUPPLY)
                unit->state = unit->state & ~US_DEAD;

            unit->state |= US_HIDDEN | US_NOT_DEPLOYED;

            unit->rescue = 0;
            unit->supportBits = 0;
        }
    }

    gPlaySt.chapterStateBits = gPlaySt.chapterStateBits & ~PLAY_FLAG_PREPSCREEN;
}