#include "common-chax.h"
#include "kernel-lib.h"
#include "event-rework.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

static void set_actor_unit(void)
{
    gEventSlots[EVT_SLOT_2] = gActiveUnit->index;
}

static void set_position(void)
{
    gActiveUnit->xPos = gActionData.xMove;
    gActiveUnit->yPos = gActionData.yMove;
}

STATIC_DECLAR const EventScr EventScr_ActionTeleportation[] = {
    EVBIT_MODIFY(0x4)
    STAL(20)

LABEL(0)
    ASMC(MapAnim_CommonInit)
    ASMC(set_actor_unit)
    CALL(EventScr_UidWarpOUT)
    STAL(60)
    ASMC(set_position)
    ASMC(set_actor_unit)
    CALL(EventScr_UidWarpIN)
    STAL(20)

LABEL(99)
    NoFade
    ENDA
};

/* Action */
bool Action_Teleportation(ProcPtr parent)
{
    LTRACEF("[uid=%x] Teleportation AiAction to x=%d, y=%d",
        gActiveUnit->index & 0xFF, gActionData.xMove, gActionData.xMove);

    KernelCallEvent(EventScr_ActionTeleportation, EV_EXEC_CUTSCENE, parent);
    return false;
}

/* AiActionConf::exec */
void AiAction_Teleportation(ProcPtr parent)
{
    gActionData.xMove = gAiDecision.xTarget;
    gActionData.yMove = gAiDecision.yTarget;

    gActionData.subjectIndex = gActiveUnit->index;
    gActionData.unk08 = SID_Teleportation;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    ApplyUnitAction(parent);
}

/* AiActionConf:idle */
STATIC_DECLAR bool AiAction_TeleportationIdleExt(struct CpPerformProc * proc)
{
    gAiDecision.xMove = gAiDecision.xTarget;
    gAiDecision.yMove = gAiDecision.yTarget;

    gActiveUnit->state &= ~US_HAS_MOVED_AI;
    gAiState.unitIt--;
    return true;
}

bool AiAction_TeleportationIdle(struct CpPerformProc * proc)
{
    /* Force introduce a YIELD */
    proc->func = AiAction_TeleportationIdleExt;
    return false;
}
