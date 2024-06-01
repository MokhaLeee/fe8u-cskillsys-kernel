#include "common-chax.h"
#include "kernel-lib.h"

#define LOCAL_TRACE 0

STATIC_DECLAR void clear_mus(void)
{
    MU_EndAll();
    RefreshUnitSprites();
}

static void set_actor_unit(void)
{
    gEventSlots[EVT_SLOT_2] = UNIT_CHAR_ID(gActiveUnit);
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
    ASMC(clear_mus)
    ASMC(set_actor_unit)
    CALL(EventScr_UnitWarpOUT)
    STAL(60)
    ASMC(set_position)
    ASMC(set_actor_unit)
    CALL(EventScr_UnitWarpIN)
    STAL(20)

LABEL(99)
    NoFade
    ENDA
};

/* Action */
bool Action_Teleportation(ProcPtr parent)
{
	LTRACEF("Teleportation AiAction (uid=%x) to x=%d, y=%d",
		gActiveUnit->index & 0xFF, gActionData.xMove, gActionData.xMove);

    KernelCallEvent(EventScr_ActionTeleportation, EV_EXEC_CUTSCENE, parent);
    return false;
}

/* AiActionConf::exec */
void AiAction_Teleportation(ProcPtr parent)
{
    gActionData.xMove = gAiDecision.xTarget;
    gActionData.yMove = gAiDecision.yTarget;

	Action_Teleportation(parent);
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
