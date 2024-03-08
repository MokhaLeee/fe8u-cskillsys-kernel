#include "global.h"
#include "bmunit.h"
#include "event.h"

#include "common-chax.h"
#include "chax-glb.h"
#include "skill-system.h"

enum EventSkillSubOps {
    EVSUBCMD_ADD_SKILL = 1,
    EVSUBCMD_ADD_SKILL_AT,
    EVSUBCMD_ADD_SKILL_SC,

    EVSUBCMD_REMOVE_SKILL,
    EVSUBCMD_REMOVE_SKILL_AT,
    EVSUBCMD_REMOVE_SKILL_SC,
};

STATIC_DECLAR u8 EventAddSkill(struct EventEngineProc * proc)
{
    u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
    const u16 * argv = proc->pEventCurrent;

    u8 sid = argv[1];
    u8 pid = argv[2];
    struct Unit * unit = GetUnitFromCharId(pid);

    Debugf("sid %#x, pid %#x, unit %p", sid, pid, unit);

    if (argc < 3)
    {
        Errorf("No enough argument at %p", proc->pEventCurrent);
        return EVC_ERROR;
    }

    if (unit && SKILL_VALID(sid))
        AddSkill(unit, sid);

    return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventAddSkillAt(struct EventEngineProc * proc)
{
    u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
    const u16 * argv = proc->pEventCurrent;

    u8 sid = argv[1];
    s16 x = argv[2];
    s16 y = argv[3];
    struct Unit * unit = GetUnitAtPosition(x, y);

    if (argc < 4)
    {
        Errorf("Event format error at %p", proc->pEventCurrent);
        return EVC_ERROR;
    }

    if (unit && SKILL_VALID(sid))
        AddSkill(unit, sid);

    return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventAddSkillBySlotC(struct EventEngineProc * proc)
{
    u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
    const u16 * argv = proc->pEventCurrent;

    u8 sid = argv[1];
    u8 pid = gEventSlots[0xC];
    struct Unit * unit = GetUnitFromCharId(pid);

    if (argc < 2)
    {
        Errorf("Event format error at %p", proc->pEventCurrent);
        return EVC_ERROR;
    }

    if (unit && SKILL_VALID(sid))
        AddSkill(unit, sid);

    return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventRemoveSkill(struct EventEngineProc * proc)
{
    u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
    const u16 * argv = proc->pEventCurrent;

    u8 sid = argv[1];
    u8 pid = argv[2];
    struct Unit * unit = GetUnitFromCharId(pid);

    if (argc < 3)
    {
        Errorf("No enough argument at %p", proc->pEventCurrent);
        return EVC_ERROR;
    }

    if (unit && SKILL_VALID(sid))
        RemoveSkill(unit, sid);

    return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventRemoveSkillAt(struct EventEngineProc * proc)
{
    u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
    const u16 * argv = proc->pEventCurrent;

    u8 sid = argv[1];
    s16 x = argv[2];
    s16 y = argv[3];
    struct Unit * unit = GetUnitAtPosition(x, y);

    if (argc < 4)
    {
        Errorf("Event format error at %p", proc->pEventCurrent);
        return EVC_ERROR;
    }

    if (unit && SKILL_VALID(sid))
        RemoveSkill(unit, sid);

    return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventRemoveSkillBySlotC(struct EventEngineProc * proc)
{
    u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
    const u16 * argv = proc->pEventCurrent;

    u8 sid = argv[1];
    u8 pid = gEventSlots[0xC];
    struct Unit * unit = GetUnitFromCharId(pid);

    if (argc < 2)
    {
        Errorf("Event format error at %p", proc->pEventCurrent);
        return EVC_ERROR;
    }

    if (unit && SKILL_VALID(sid))
        RemoveSkill(unit, sid);

    return EVC_ADVANCE_CONTINUE;
}

/* LynJump */
u8 EventSkillOperation(struct EventEngineProc * proc)
{
    switch (EVT_SUB_CMD(proc->pEventCurrent)) {
    case EVSUBCMD_ADD_SKILL:
        return EventAddSkill(proc);

    case EVSUBCMD_ADD_SKILL_AT:
        return EventAddSkillAt(proc);

    case EVSUBCMD_ADD_SKILL_SC:
        return EventAddSkillBySlotC(proc);

    case EVSUBCMD_REMOVE_SKILL:
        return EventRemoveSkill(proc);

    case EVSUBCMD_REMOVE_SKILL_AT:
        return EventRemoveSkillAt(proc);

    case EVSUBCMD_REMOVE_SKILL_SC:
        return EventRemoveSkillBySlotC(proc);

    default:
        Errorf("Event format error at %p", proc->pEventCurrent);
        return EVC_ERROR;
    };
}
