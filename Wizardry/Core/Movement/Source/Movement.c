#include "common-chax.h"
#include "skill-system.h"
#include "arm-func.h"
#include "kernel-lib.h"
#include "map-movement.h"
#include "constants/skills.h"

STATIC_DECLAR void PreGenerateMovementMap(void)
{
    FORCE_DECLARE struct Unit * unit;

    KernelMoveMapFlags = 0;

    /* Fasten calc! */
    if (!gMovMapFillState.hasUnit)
        return;

    unit = GetUnit(gMovMapFillState.unitId);

#if (defined(SID_Pass) && COMMON_SKILL_VALID(SID_Pass))
    if (SkillTester(unit, SID_Pass) && ((GetUnitCurrentHp(unit) * 4) >= GetUnitMaxHp(unit)))
        KernelMoveMapFlags |= FMOVSTRE_PASS;
#endif
}

/* LynJump */
void GenerateMovementMap(int x, int y, int movement, int uid)
{
    gMovMapFillState.dst = gMovMapFillStPool1;
    gMovMapFillState.src = gMovMapFillStPool2;

    gMovMapFillState.movement = movement;

    if (uid == 0)
    {
        gMovMapFillState.hasUnit = false;
    }
    else
    {
        gMovMapFillState.hasUnit = true;
        gMovMapFillState.unitId = uid;
    }

    gMovMapFillState.maxMovementValue = MAP_MOVEMENT_MAX;

    BmMapFill(gWorkingBmMap, -1);

    gMovMapFillState.dst->xPos = x;
    gMovMapFillState.dst->yPos = y;
    gMovMapFillState.dst->connexion = 5;
    gMovMapFillState.dst->leastMoveCost = 0;

    gWorkingBmMap[y][x] = 0;

    gMovMapFillState.dst++;
    gMovMapFillState.dst->connexion = 4;

#if CHAX
    PreGenerateMovementMap();
#endif

    CallARM_FillMovementMap();
}

void GameInit_RedirectMapFloodFunc(void)
{
    gUnknown_03003128 = (void *)ARM_MapFloodCoreRe;
}
