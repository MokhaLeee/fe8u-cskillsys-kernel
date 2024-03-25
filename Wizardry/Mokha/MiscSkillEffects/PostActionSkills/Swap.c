#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/texts.h"
#include "constants/skills.h"
#include "battle-system.h"

STATIC_DECLAR void PostActionSwapSkipMenuIfNotAlly(struct EventEngineProc * proc)
{
    gEventSlots[EVT_SLOT_C] = false;
    if (UNIT_FACTION(gActiveUnit) == FACTION_BLUE)
        gEventSlots[EVT_SLOT_C] = true;
}

STATIC_DECLAR void PreparePositionSwap(void)
{
    MU_EndAll();
    RefreshUnitSprites();
}

static void set_target_unit(void)
{
    gEventSlots[EVT_SLOT_2] = UNIT_CHAR_ID(&gBattleTarget.unit);
}

static void set_actor_unit(void)
{
    gEventSlots[EVT_SLOT_2] = UNIT_CHAR_ID(gActiveUnit);
}

static void set_position(void)
{
    struct Unit * unita = gActiveUnit;
    struct Unit * unitb = GetUnit(gBattleTarget.unit.index);

    int x = unita->xPos;
    int y = unita->yPos;

    unita->xPos = unitb->xPos;
    unita->yPos = unitb->yPos;

    gActionData.xMove = unitb->xPos;
    gActionData.yMove = unitb->yPos;

    unitb->xPos = x;
    unitb->yPos = y;
}

STATIC_DECLAR const EventScr EventScr_PostActionPositionSwap[] = {
    EVBIT_MODIFY(0x4)
    STAL(20)
    ASMC(PostActionSwapSkipMenuIfNotAlly)
    BEQ(0, EVT_SLOT_C, EVT_SLOT_0)

    TUTORIALTEXTBOXSTART
    SVAL(EVT_SLOT_B, 0xffffffff)
    TEXTSHOW(MSG_SkillEffect_PosSwap_Confirm)
    TEXTEND
    REMA
    SVAL(EVT_SLOT_7, 0x1)
    BNE(99, EVT_SLOT_C, EVT_SLOT_7)

LABEL(0)
    ASMC(PreparePositionSwap)
    ASMC(set_actor_unit)
    CALL(EventScr_UnitWarpOUT)
    STAL(20)
    ASMC(set_target_unit)
    CALL(EventScr_UnitFlushingOUT)
    STAL(60)
    ASMC(set_position)
    ASMC(set_target_unit)
    CALL(EventScr_UnitFlushingIN)
    STAL(20)
    ASMC(set_actor_unit)
    CALL(EventScr_UnitWarpIN)
    STAL(20)

LABEL(99)
    NoFade
    ENDA
};

bool PostActionSwap(ProcPtr proc)
{
    struct Unit * unit = gActiveUnit;
    if (!SkillTester(unit, SID_PosSwap))
        return false;

    if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
        return false;

    if (!UNIT_IS_VALID(unit) || unit->state & US_UNAVAILABLE)
        return false;

#if 0
    if (gBattleActorGlobalFlag.hitted == false)
        return false;
#endif

    KernelCallEvent(EventScr_PostActionPositionSwap, EV_EXEC_CUTSCENE, proc);
    return true;
}
