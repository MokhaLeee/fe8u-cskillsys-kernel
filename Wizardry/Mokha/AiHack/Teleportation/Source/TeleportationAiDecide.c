#include "common-chax.h"
#include "weapon-range.h"
#include "status-getter.h"

#include "ai-hack.h"

bool AiTryTeleportation(void)
{
    int i, ix, iy;
    int max_rng;

    if (GetUnitEquippedWeapon(gActiveUnit) == 0)
        return false;

    if (0)
        return false;

    BmMapFill(gBmMapMovement, -1);

    max_rng = GetUnitMaxRange(gActiveUnit) + MovGetter(gActiveUnit);

    for (iy = 0; iy < gBmMapSize.y; iy++)
    {
        for (ix = 0; ix < gBmMapSize.x; ix++)
        {
            s8 uid = gBmMapUnit[iy][ix];
            struct Unit * unit;

            if (uid == 0)
                continue;

            if (AreUnitsAllied(gActiveUnitId, gBmMapUnit[iy][ix]) == 1)
                continue;

            unit = GetUnit(uid);
            if (!UNIT_IS_VALID(unit))
                continue;

            if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            MapAddInRange(ix, iy, max_rng, 1);
        }
    }

    /* Polling a target position */
    for (i = 0; i < 100; i++)
    {
        ix = NextRN_N(gBmMapSize.x);
        iy = NextRN_N(gBmMapSize.y);

        if (gBmMapUnit[iy][ix] != 0 && (s8) gBmMapMovement[iy][ix] >= 0)
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

#if CHAX
    if (!(gActiveUnit->ai_config & AI_UNIT_CONFIG_EXTFLAG_TELEPORTATION))
    {
        Error("AiTryTeleportation");
        switch (gActiveUnit->ai1) {
        case 0:
        case 1:
        case 2:
        case 11:
            if (AiTryTeleportation())
            {
                gActiveUnit->ai_config |= AI_UNIT_CONFIG_EXTFLAG_TELEPORTATION;
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

#if CHAX
    if (!(gActiveUnit->ai_config & AI_UNIT_CONFIG_EXTFLAG_TELEPORTATION))
    {
        Error("AiTryTeleportation2");
        switch (gActiveUnit->ai2) {
        case 0:
        case 1:
        case 2:
        case 6:
        case 7:
            if (AiTryTeleportation())
            {
                gActiveUnit->ai_config |= AI_UNIT_CONFIG_EXTFLAG_TELEPORTATION;
                return true;
            }
            break;
        }
    }
#endif

    AiScript_Exec(&gActiveUnit->ai_b_pc);
    return gAiScriptEnded;
}
