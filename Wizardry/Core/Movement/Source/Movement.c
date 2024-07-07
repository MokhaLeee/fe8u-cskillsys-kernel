#include "common-chax.h"
#include "skill-system.h"
#include "arm-func.h"
#include "kernel-lib.h"
#include "map-movement.h"
#include "constants/skills.h"

STATIC_DECLAR void PreGenerateMovementMap(void)
{
    struct Unit * unit;
    int ix, iy;

    KernelMoveMapFlags = 0;

    /* Fasten calc! */
    if (!gMovMapFillState.hasUnit)
        return;

    unit = GetUnit(gMovMapFillState.unitId);

#if (defined(SID_Pass) && COMMON_SKILL_VALID(SID_Pass))
    if (SkillTester(unit, SID_Pass) && ((GetUnitCurrentHp(unit) * 4) >= GetUnitMaxHp(unit)))
        KernelMoveMapFlags |= FMOVSTRE_PASS;
#endif

    /**
     * Here we use generic buffer as a external cost buffer
     */
    BmMapInit(gGenericBuffer, &KernelExtMoveCostMap, gBmMapSize.x, gBmMapSize.y);
    BmMapFill(KernelExtMoveCostMap, 0);

    for (iy = 0; iy < gBmMapSize.y; ++iy)
        for (ix = 0; ix < gBmMapSize.x; ++ix)
        {
            struct Unit * _unit = GetUnitAtPosition(ix, iy);
            if (!UNIT_ALIVE(_unit))
                continue;

#if (defined(SID_Obstruct) && COMMON_SKILL_VALID(SID_Obstruct))
            if
            (
                !AreUnitsAllied(unit->index, _unit->index) &&
                !(KernelMoveMapFlags & FMOVSTRE_PASS) && // Obstruct is not effective on Pass skill
                SkillTester(_unit, SID_Obstruct)
            )
            {
                int _i;
                for (_i = 0; _i < ARRAY_COUNT_RANGE1x1; _i++)
                {
                    int _x = _unit->xPos + gVecs_1x1[_i].x;
                    int _y = _unit->yPos + gVecs_1x1[_i].y;

                    if (IsPositionValid(_x, _y))
                        KernelExtMoveCostMap[_y][_x] = -1;
                }

                KernelExtMoveCostMap[_unit->yPos][_unit->xPos] = -1;
                KernelMoveMapFlags |= FMOVSTRE_OBSTRUCT;
            }
#endif
        }
}

#if 0
void PostGenerateMovementMap(void)
{
    struct Unit * unit;
    int ix, iy;

    /* Fasten calc! */
    if (!gMovMapFillState.hasUnit)
        return;

    unit = GetUnit(gMovMapFillState.unitId);

    for (iy = 0; iy < gBmMapSize.y; ++iy)
        for (ix = 0; ix < gBmMapSize.x; ++ix)
        {
            int cost;
            struct Unit * _unit;

            cost = gWorkingBmMap[iy][ix];
            if (cost == 0xFF)
                continue;

            _unit = GetUnitAtPosition(ix, iy);
            if (!UNIT_ALIVE(_unit))
                continue;

            if (AreUnitsAllied(unit->index, _unit->index))
            {
                int _i;
                for (_i = 0; _i < ARRAY_COUNT_RANGE1x1; _i++)
                {
                    int _x = _unit->xPos + gVecs_1x1[_i].x;
                    int _y = _unit->yPos + gVecs_1x1[_i].y;

                    if (IsPositionValid(_x, _y) && gWorkingBmMap[_y][_x] == 0xFF)
                        gWorkingBmMap[_y][_x] = cost;
                }
            }
        }
}
#endif

/* LynJump */
void GenerateMovementMap(int x, int y, int movement, int uid)
{
    u8 ** working_map = gWorkingBmMap;

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
    SetWorkingBmMap(working_map);
#endif

    CallARM_FillMovementMap();

#if 0
    PostGenerateMovementMap();
    SetWorkingBmMap(working_map);
#endif
}

void GameInit_RedirectMapFloodFunc(void)
{
    gUnknown_03003128 = _MapFloodCoreRe;
}
