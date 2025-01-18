#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "event-rework.h"

#define LOCAL_TRACE 0

STATIC_DECLAR u8 EventAddSkill(struct EventEngineProc *proc)
{
	u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
	const u16 *argv = proc->pEventCurrent;
	u16 sid = argv[1];
	u8 pid = argv[2];
	struct Unit *unit = GetUnitFromCharId(pid);

	LTRACEF("sid %#x, pid %#x, unit %p", sid, pid, unit);

	if (argc < 3) {
		Errorf("No enough argument at %p", proc->pEventCurrent);
		hang();
	}

	if (UNIT_IS_VALID(unit) && EQUIPE_SKILL_VALID(sid))
		AddSkill(unit, sid);

	return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventAddSkillOnActive(struct EventEngineProc *proc)
{
	u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
	const u16 *argv = proc->pEventCurrent;

	u16 sid = argv[1];
	struct Unit *unit = gActiveUnit;

	if (argc < 2) {
		Errorf("Event format error at %p", proc->pEventCurrent);
		hang();
	}

	if (UNIT_IS_VALID(unit) && EQUIPE_SKILL_VALID(sid))
		AddSkill(unit, sid);

	return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventAddSkillAt(struct EventEngineProc *proc)
{
	u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
	const u16 *argv = proc->pEventCurrent;

	u16 sid = argv[1];
	s16 x = argv[2];
	s16 y = argv[3];
	struct Unit *unit = GetUnitAtPosition(x, y);

	if (argc < 4) {
		Errorf("Event format error at %p", proc->pEventCurrent);
		hang();
	}

	if (UNIT_IS_VALID(unit) && EQUIPE_SKILL_VALID(sid))
		AddSkill(unit, sid);

	return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventAddSkillBySlotC(struct EventEngineProc *proc)
{
	u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
	const u16 *argv = proc->pEventCurrent;

	u16 sid = argv[1];
	u8 pid = gEventSlots[0xC];
	struct Unit *unit = GetUnitFromCharId(pid);

	if (argc < 2) {
		Errorf("Event format error at %p", proc->pEventCurrent);
		hang();
	}

	if (UNIT_IS_VALID(unit) && EQUIPE_SKILL_VALID(sid))
		AddSkill(unit, sid);

	return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventRemoveSkill(struct EventEngineProc *proc)
{
	u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
	const u16 *argv = proc->pEventCurrent;

	u16 sid = argv[1];
	u8 pid = argv[2];
	struct Unit *unit = GetUnitFromCharId(pid);

	if (argc < 3) {
		Errorf("No enough argument at %p", proc->pEventCurrent);
		hang();
	}

	if (UNIT_IS_VALID(unit) && EQUIPE_SKILL_VALID(sid)) {
		ForgetSkill(unit, sid);
		RemoveSkill(unit, sid);
	}
	return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventRemoveSkillOnActive(struct EventEngineProc *proc)
{
	u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
	const u16 *argv = proc->pEventCurrent;

	u16 sid = argv[1];
	struct Unit *unit = gActiveUnit;

	if (argc < 2) {
		Errorf("Event format error at %p", proc->pEventCurrent);
		hang();
	}

	if (UNIT_IS_VALID(unit) && EQUIPE_SKILL_VALID(sid)) {
		ForgetSkill(unit, sid);
		RemoveSkill(unit, sid);
	}

	return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventRemoveSkillAt(struct EventEngineProc *proc)
{
	u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
	const u16 *argv = proc->pEventCurrent;

	u16 sid = argv[1];
	s16 x = argv[2];
	s16 y = argv[3];
	struct Unit *unit = GetUnitAtPosition(x, y);

	if (argc < 4) {
		Errorf("Event format error at %p", proc->pEventCurrent);
		hang();
	}

	if (UNIT_IS_VALID(unit) && EQUIPE_SKILL_VALID(sid)) {
		ForgetSkill(unit, sid);
		RemoveSkill(unit, sid);
	}

	return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 EventRemoveSkillBySlotC(struct EventEngineProc *proc)
{
	u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
	const u16 *argv = proc->pEventCurrent;
	u16 sid = argv[1];
	u8 pid = gEventSlots[0xC];
	struct Unit *unit = GetUnitFromCharId(pid);

	if (argc < 2) {
		Errorf("Event format error at %p", proc->pEventCurrent);
		hang();
	}

	if (UNIT_IS_VALID(unit) && EQUIPE_SKILL_VALID(sid)) {
		ForgetSkill(unit, sid);
		RemoveSkill(unit, sid);
	}

	return EVC_ADVANCE_CONTINUE;
}

/* External hook */
u8 EventSkillOperation(struct EventEngineProc *proc)
{
	switch (EVT_SUB_CMD(proc->pEventCurrent)) {
	case EVSUBCMD_ADD_SKILL:
		return EventAddSkill(proc);

	case EVSUBCMD_ADD_SKILL_ACTIVE:
		return EventAddSkillOnActive(proc);

	case EVSUBCMD_ADD_SKILL_AT:
		return EventAddSkillAt(proc);

	case EVSUBCMD_ADD_SKILL_SC:
		return EventAddSkillBySlotC(proc);

	case EVSUBCMD_REMOVE_SKILL:
		return EventRemoveSkill(proc);

	case EVSUBCMD_REMOVE_SKILL_ACTIVE:
		return EventRemoveSkillOnActive(proc);

	case EVSUBCMD_REMOVE_SKILL_AT:
		return EventRemoveSkillAt(proc);

	case EVSUBCMD_REMOVE_SKILL_SC:
		return EventRemoveSkillBySlotC(proc);

	default:
		Errorf("Event format error at %p", proc->pEventCurrent);
		hang();
	};
}
