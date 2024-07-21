#include "common-chax.h"
#include "kernel-lib.h"
#include "event-rework.h"

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
    KernelCallEvent(EventScr_ActionTeleportation, EV_EXEC_CUTSCENE, parent);
    return false;
}
