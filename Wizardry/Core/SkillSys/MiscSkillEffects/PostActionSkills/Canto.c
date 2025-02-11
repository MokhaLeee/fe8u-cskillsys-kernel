#include "common-chax.h"
#include "status-getter.h"
#include "battle-system.h"
#include "action-expa.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "unit-expa.h"

STATIC_DECLAR bool CheckCanto(void)
{
    bool canto, cantop, canter;

    if (gActiveUnit->state & (US_DEAD | US_HAS_MOVED | US_BIT16))
        return false;

#if defined(SID_CantoPlus) && (COMMON_SKILL_VALID(SID_CantoPlus))
    cantop = SkillTester(gActiveUnit, SID_CantoPlus);
#else
    cantop = false;
#endif

#if defined(SID_Canto) && (COMMON_SKILL_VALID(SID_Canto))
    canto = SkillTester(gActiveUnit, SID_Canto);
#else
    canto = false;
#endif

#if defined(SID_Canter) && (COMMON_SKILL_VALID(SID_Canter))
    canter = SkillTester(gActiveUnit, SID_Canter);
#else
    canter = false;
#endif

    if (!canto || !cantop || !canter)
    {
        int i;
        for (i = 0; i < ARRAY_COUNT_RANGE3x3; i++)
        {
            int _x = gActiveUnit->xPos + gVecs_3x3[i].x;
            int _y = gActiveUnit->yPos + gVecs_3x3[i].y;

            struct Unit *unit_ally = GetUnitAtPosition(_x, _y);
            if (!UNIT_IS_VALID(unit_ally))
                continue;

            if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

            if (!AreUnitsAllied(gActiveUnit->index, unit_ally->index))
                continue;

#if (defined(SID_Pathfinder) && (COMMON_SKILL_VALID(SID_Pathfinder)))
            if (SkillTester(unit_ally, SID_Pathfinder))
            {
                canto = true;
                break;
            }
#endif
        }
    }

    if (!canto && !cantop && !canter)
        return false;

    switch (gActionData.unitActionType)
    {
    case UNIT_ACTION_WAIT:
        return false;

    case UNIT_ACTION_COMBAT:
    case UNIT_ACTION_STAFF:
        if (!cantop && !canter)
            return false;

        break;
    }

    if ((MovGetter(gActiveUnit) <= gActionData.moveCount) && !canter)
        return false;

    if (!CanUnitMove() && !canter)
        return false;

    /* Canter calculation to ensure an additional 2 movement */
    if (canter)
        gActionData.moveCount = MovGetter(gActiveUnit) - 2;

    return true;
}

LYN_REPLACE_CHECK(TryMakeCantoUnit);
bool TryMakeCantoUnit(ProcPtr proc)
{
    bool canCanto = false;

    if (gActionData.unitActionType == UNIT_ACTION_WAIT)
        return false;

#if defined(SID_GridMasterAtk) && (COMMON_SKILL_VALID(SID_GridMasterAtk))
    if (SkillTester(gActiveUnit, SID_GridMasterAtk))
    {
        if(gActionData.unitActionType == UNIT_ACTION_COMBAT)
            return false;

        canCanto = true;
        gActionData.moveCount = MovGetter(gActiveUnit);
    }
#endif

#if defined(SID_GridMaster) && (COMMON_SKILL_VALID(SID_GridMaster))
    if (SkillTester(gActiveUnit, SID_GridMaster))
        canCanto = true;
#endif

    if (CheckCanto())
        canCanto = true;

    if (!canCanto)
        return false;

    BmMapFill(gBmMapRange, 0);

    UnitBeginCantoAction(gActiveUnit);

    gActiveUnit->state |= US_HAS_MOVED;
    gActiveUnit->state &= ~US_UNSELECTABLE;

    EndAllMus();
    StartMu(gActiveUnit);
    SetAutoMuDefaultFacing();

    if (gPlaySt.chapterVisionRange != 0)
        Proc_Goto(proc, 4);
    else
        Proc_Goto(proc, 1);

    return true;
}

LYN_REPLACE_CHECK(PlayerPhase_FinishAction);
void PlayerPhase_FinishAction(ProcPtr proc)
{
#if defined(SID_TwinCrests) && (COMMON_SKILL_VALID(SID_TwinCrests))
    if (SkillTester(gActiveUnit, SID_TwinCrests) && !CheckBitUES(gActiveUnit, UES_BIT_TWIN_CRESTS_SKILL_USED))
    {
        SetBitUES(gActiveUnit, UES_BIT_TWIN_CRESTS_SKILL_USED);
        gActionDataExpa.refrain_action = true;
        EndAllMus();
    }
#endif

    if (gPlaySt.chapterVisionRange != 0)
    {
        RenderBmMapOnBg2();
        MoveActiveUnit(gActionData.xMove, gActionData.yMove);
        RefreshEntityBmMaps();
        RenderBmMap();
        NewBMXFADE(0);
        RefreshUnitSprites();
    }
    else
    {
        MoveActiveUnit(gActionData.xMove, gActionData.yMove);
        RefreshEntityBmMaps();
        RenderBmMap();
    }

    SetCursorMapPosition(gActiveUnit->xPos, gActiveUnit->yPos);

    gPlaySt.xCursor = gBmSt.playerCursor.x;
    gPlaySt.yCursor = gBmSt.playerCursor.y;

#if CHAX
    if (gActionDataExpa.refrain_action)
    {
        gActiveUnit->state &= ~(US_UNSELECTABLE | US_CANTOING);
    }
    else if (TryMakeCantoUnit(proc))
    {
        HideUnitSprite(gActiveUnit);
        return;
    }
#else
    if (TryMakeCantoUnit(proc))
    {
        HideUnitSprite(gActiveUnit);
        return;
    }
#endif

    if (ShouldCallEndEvent())
    {
        EndAllMus();
        RefreshEntityBmMaps();
        RenderBmMap();
        RefreshUnitSprites();
        MaybeCallEndEvent_();
        Proc_Goto(proc, 8);
        return;
    }
    EndAllMus();
}
