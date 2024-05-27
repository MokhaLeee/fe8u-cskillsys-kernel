#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

STATIC_DECLAR void PostActionReturnSkipMenuIfNotAlly(struct EventEngineProc * proc)
{
    gEventSlots[EVT_SLOT_C] = false;
    if (UNIT_FACTION(gActiveUnit) == FACTION_BLUE)
        gEventSlots[EVT_SLOT_C] = true;
}

STATIC_DECLAR void PrepareReturnPosition(void)
{
    gEventSlots[EVT_SLOT_2] = UNIT_CHAR_ID(gActiveUnit);

    MU_EndAll();
    RefreshUnitSprites();
}

STATIC_DECLAR void PostActionReturnToOrigin(void)
{
    gActionData.xMove = gActiveUnitMoveOrigin.x;
    gActionData.yMove = gActiveUnitMoveOrigin.y;

    gActiveUnit->xPos = gActiveUnitMoveOrigin.x;
    gActiveUnit->yPos = gActiveUnitMoveOrigin.y;
}

STATIC_DECLAR const EventScr EventScr_PostActionPositionReturn[] = {
    EVBIT_MODIFY(0x4)
    ASMC(PostActionReturnSkipMenuIfNotAlly)
    BEQ(0, EVT_SLOT_C, EVT_SLOT_0)

    TUTORIALTEXTBOXSTART
    SVAL(EVT_SLOT_B, 0xffffffff)
    TEXTSHOW(MSG_SkillEffect_PosReturn_Confirm)
    TEXTEND
    REMA
    SVAL(EVT_SLOT_7, 0x1)
    BNE(99, EVT_SLOT_C, EVT_SLOT_7)

LABEL(0)
    ASMC(PrepareReturnPosition)
    CALL(EventScr_UnitWarpOUT)
    STAL(20)
    ASMC(PostActionReturnToOrigin)
    CALL(EventScr_UnitFlushingIN)
    STAL(20)

LABEL(99)
    NoFade
    ENDA
};

bool PostActionPositionReturn(ProcPtr proc)
{
    if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
        return false;

    if (gActiveUnit->state & US_UNAVAILABLE)
        return false;

#if defined(SID_PosReturn) && (SID_PosReturn < MAX_SKILL_NUM)
    if (!SkillTester(gActiveUnit, SID_PosReturn))
#else
    if (1)
#endif
        return false;

    if (gBattleActorGlobalFlag.enimy_defeated == false)
        return false;

    KernelCallEvent(EventScr_PostActionPositionReturn, EV_EXEC_CUTSCENE, proc);
    return true;
}
