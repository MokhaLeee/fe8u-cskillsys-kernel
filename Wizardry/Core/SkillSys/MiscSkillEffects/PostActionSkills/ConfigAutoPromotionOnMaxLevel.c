#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "battle-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "strmag.h"
#include "bmunit.h"
#include "classchg.h"
#include "status-getter.h"
#include "action-expa.h"
#include "unit-expa.h"

STATIC_DECLAR const EventScr EventScr_PostActionPromote[] = {
    EVBIT_MODIFY(0x4)
    STAL(20)
    PROM(4, 7, 0)
    // So PROM expects a chaarcter ID, expected class ID to promote into and an item ID (which can be 0)
    //
    LABEL(99)
    NOFADE
    ENDA
};

void ConfigAutoPromotionOnMaxLevel(ProcPtr proc)
{   
    // if ((UNIT_CATTRIBUTES(gActiveUnit) & CA_MAXLEVEL10) && gActiveUnit->level == 10)

    /* These don't work and I don't know how to make thmm work */
    // EVT_CMD_ARGV(proc->pEventCurrent)[0] = 4;
    // EVT_CMD_ARGV(proc->pEventCurrent)[1] = 7;
    // EVT_CMD_ARGV(proc->pEventCurrent)[2] = 0;


    KernelCallEvent(EventScr_PostActionPromote, EV_EXEC_CUTSCENE, proc);
    return;
}