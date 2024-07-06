#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

extern u8 ARM_MapFloodCoreRe[];

struct MovMapFillStateRe {

    u8 _pad_vanilla_[0xC]; // gMovMapFillState

    /* 0C */ u8 flag;
    /* 0D */ u8 _pad_[3];
};

enum MovMapFillStateRe_flags {
    FMOVSTRE_PASS = 1 << 0,
};

static struct MovMapFillStateRe * const sMovMapFillStateRe = (void *)0x03004E60; // gMovMapFillState

STATIC_DECLAR void PreGenerateMovementMap(void)
{
    struct Unit * unit = GetUnit(gMovMapFillState.unitId);

    sMovMapFillStateRe->flag = 0;

    if (gMovMapFillState.hasUnit && SkillTester(unit, SID_Pass))
        sMovMapFillStateRe->flag |= FMOVSTRE_PASS;
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
