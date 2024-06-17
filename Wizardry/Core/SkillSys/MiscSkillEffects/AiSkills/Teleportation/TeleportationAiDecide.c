#include "common-chax.h"
#include "weapon-range.h"
#include "status-getter.h"
#include "skill-system.h"
#include "constants/skills.h"

#include "ai-hack.h"

#define LOCAL_TRACE 0

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

STATIC_DECLAR bool AiTryTeleportationExt(void)
{
    #define RETRY_CNT 100

    int i, ix, iy;
    int enemy_faction;

    if (GetUnitEquippedWeapon(gActiveUnit) == 0)
        return false;

#if defined(SID_Teleportation) && (SID_Teleportation < MAX_SKILL_NUM)
    if (!SkillTester(gActiveUnit, SID_Teleportation))
#else
    if (1)
#endif
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
            MovGetter(gActiveUnit),
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
bool Ai2Decide_TryTeleportation(void)
{
    /**
     * This function may just hook at AI-2 decided
     */
    if (gAiScriptKind != AI_SCRIPT_AI2)
        return false;

    if (gActiveUnit->state & US_HAS_MOVED)
        return false;

    switch (gActiveUnit->ai2) {
    case 0:
    case 1:
    case 2:
    case 9:
    case 10:
    case 11:
        break;

    default:
        return false;
    }

    if (AiTryTeleportationExt())
    {
        LTRACEF("[uid=%x] Ai1 Teleportation to x=%d, y=%d", 
                    gActiveUnit->index & 0xFF, gAiDecision.xTarget, gAiDecision.yTarget);

        gActiveUnit->state |= US_HAS_MOVED;
        return true;
    }
    return false;
}
