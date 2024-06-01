#include "common-chax.h"
#include "weapon-range.h"
#include "status-getter.h"

#include "ai-hack.h"

#define LOCAL_TRACE 1

static inline int GetEnemyFaction(int faction)
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

bool AiTryTeleportation(void)
{
    #define RETRY_CNT 100

    int i, ix, iy;
    int enemy_faction;

    if (GetUnitEquippedWeapon(gActiveUnit) == 0)
        return false;

    if (0)
        return false;

    enemy_faction = GetEnemyFaction(UNIT_FACTION(gActiveUnit));

    for (i = 1; i < RETRY_CNT; i++)
    {
        struct Unit * unit = GetUnit((enemy_faction + 1 + NextRN_N(CONFIG_UNIT_AMT_ALLY - 1)));
        if (!UNIT_IS_VALID(unit))
                continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

        BmMapFill(gBmMapMovement, -1);
        SetWorkingMoveCosts(GetUnitMovementCost(unit));
        GenerateMovementMap(
            unit->xPos, unit->yPos,
            GetUnitMaxRange(gActiveUnit) + MovGetter(gActiveUnit),
            unit->index);

        break;
    }

    if (i == RETRY_CNT)
        return false;

    /* Polling a target position */
    for (i = 0; i < RETRY_CNT; i++)
    {
        ix = NextRN_N(gBmMapSize.x);
        iy = NextRN_N(gBmMapSize.y);

        if (gBmMapUnit[iy][ix] == 0 && (s8) gBmMapMovement[iy][ix] >= 0)
        {
            /* Found it! */
            AiSetDecision(gActiveUnit->xPos, gActiveUnit->yPos, CONFIG_AI_ACTION_EXPA_Teleportation, 0, 0, ix, iy);

            return true;
        }
    }
    return false;
}

/* LynJump */
bool AiTryExecScriptA(void)
{
    gpAiScriptCurrent = gpAi1Table[0][gActiveUnit->ai1];
    gpAiScriptCurrent = gpAiScriptCurrent + gActiveUnit->ai_a_pc;

    gAiScriptEnded = 1;
    gAiScriptKind = AI_SCRIPT_AI1;

    LTRACEF("uid=%x, ai1=%d, pc=%d", gActiveUnit->index & 0xFF, gActiveUnit->ai1, gActiveUnit->ai_a_pc);

#if CHAX
    if (!(gActiveUnit->state & US_HAS_MOVED))
    {
        switch (gActiveUnit->ai1) {
        case 0:
        case 1:
        case 2:
        case 11:
            if (AiTryTeleportation())
            {
                LTRACEF("Ai1: Teleportation to x=%d, y=%d", 
                            gAiDecision.xTarget, gAiDecision.yTarget);

                gActiveUnit->state |= US_HAS_MOVED;
                return true;
            }
            break;
        }
    }
#endif

    AiScript_Exec(&gActiveUnit->ai_a_pc);
    return gAiScriptEnded;
}

/* LynJump */
bool AiTryExecScriptB(void)
{
    gpAiScriptCurrent = gpAi2Table[0][gActiveUnit->ai2];
    gpAiScriptCurrent = gpAiScriptCurrent + gActiveUnit->ai_b_pc;

    gAiScriptEnded = 1;
    gAiScriptKind = AI_SCRIPT_AI2;

    LTRACEF("uid=%x, ai2=%d, pc=%d", gActiveUnit->index & 0xFF, gActiveUnit->ai2, gActiveUnit->ai_a_pc);

#if CHAX
    if (!(gActiveUnit->state & US_HAS_MOVED))
    {
        switch (gActiveUnit->ai2) {
        case 0:
        case 1:
        case 2:
        case 6:
        case 7:
            if (AiTryTeleportation())
            {
                LTRACEF("Ai2: Teleportation to x=%d, y=%d", 
                            gAiDecision.xTarget, gAiDecision.yTarget);

                gActiveUnit->state |= US_HAS_MOVED;
                return true;
            }
            break;
        }
    }
#endif

    AiScript_Exec(&gActiveUnit->ai_b_pc);
    return gAiScriptEnded;
}
