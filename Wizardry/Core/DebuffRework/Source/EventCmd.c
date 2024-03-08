#include "global.h"
#include "bmunit.h"
#include "event.h"

#include "common-chax.h"
#include "chax-glb.h"
#include "debuff.h"

enum EventDebuffSubOps {
    EVSUBCMD_SET_DEBUFF = 1,
    EVSUBCMD_SET_DEBUFF_AT,
    EVSUBCMD_CLEAR_DEBUFF,
    EVSUBCMD_CLEAR_DEBUFF_AT,
};

u8 EventDebuffOperation(struct EventEngineProc * proc)
{
    const u16 * argv = proc->pEventCurrent;
    u8 subcmd = EVT_SUB_CMD(proc->pEventCurrent);

    int x, y, pid, debuff;
    struct Unit * unit;

    switch (subcmd) {
    case EVSUBCMD_SET_DEBUFF:
    case EVSUBCMD_CLEAR_DEBUFF:
        pid = argv[1];
        debuff = argv[2];
        unit = GetUnitFromCharId(pid);
        break;

    case EVSUBCMD_SET_DEBUFF_AT:
    case EVSUBCMD_CLEAR_DEBUFF_AT:
        x = argv[1];
        y = argv[2];
        debuff = argv[3];
        unit = GetUnitAtPosition(x, y);
        break;

    default:
        Errorf("Event format error at %p", proc->pEventCurrent);
        return EVC_ERROR;
    }

    if (!IS_UNIT_PTR(unit))
    {
        Errorf("Unit %p invalid at event %p", unit, proc->pEventCurrent);
        return EVC_ERROR;
    }

    switch (subcmd) {
    case EVSUBCMD_SET_DEBUFF:
    case EVSUBCMD_SET_DEBUFF_AT:
        SetUnitStatus(unit, debuff);
        break;

    case EVSUBCMD_CLEAR_DEBUFF:
    case EVSUBCMD_CLEAR_DEBUFF_AT:
        SetUnitStatusIndex(unit, 0);
        SetUnitStatusDuration(unit, 0);
        break;
    };

    return EVC_ADVANCE_CONTINUE;
}
