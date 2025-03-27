#include "common-chax.h"
#include "kernel-lib.h"
#include "debuff.h"

enum EventDebuffSubOps {
	EVSUBCMD_DEBUFF_SET_FOR_UNIT = 1,
	EVSUBCMD_DEBUFF_SET_UNIT_SLOT1,
	EVSUBCMD_DEBUFF_SET_UNIT_AT,
	EVSUBCMD_DEBUFF_SET_UNIT_ACTIVE,
	EVSUBCMD_DEBUFF_SET_ALL_PID,
	EVSUBCMD_DEBUFF_SET_ALL_JID,

	EVSUBCMD_DEBUFF_CLEAR_FOR_UNIT,
	EVSUBCMD_DEBUFF_CLEAR_UNIT_SLOT1,
	EVSUBCMD_DEBUFF_CLEAR_UNIT_AT,
	EVSUBCMD_DEBUFF_CLEAR_UNIT_ACTIVE,
	EVSUBCMD_DEBUFF_CLEAR_ALL_PID,
	EVSUBCMD_DEBUFF_CLEAR_ALL_JID,
};

STATIC_DECLAR u8 DemoEventOperation_Debuff_ForUnit(struct EventEngineProc *proc, bool set_or_clear)
{
	u16 argc = EVT_CMD_LEN(proc->pEventCurrent);
	const u16 *argv = proc->pEventCurrent;
	u16 debuff = argv[1];
	u16 duaration = argv[2];
	struct Unit *unit;

	if (argc < 3) {
		Errorf("Event format error at %p", proc->pEventCurrent);
		hang();
	}

	switch (EVT_SUB_CMD(proc->pEventCurrent)) {
	case EVSUBCMD_DEBUFF_SET_FOR_UNIT:
	case EVSUBCMD_DEBUFF_CLEAR_FOR_UNIT:
		unit = GetUnitStructFromEventParameter(argv[3]);
		break;

	case EVSUBCMD_DEBUFF_SET_UNIT_SLOT1:
	case EVSUBCMD_DEBUFF_CLEAR_UNIT_SLOT1:
		unit = GetUnitStructFromEventParameter(gEventSlots[1]);
		break;

	case EVSUBCMD_DEBUFF_SET_UNIT_AT:
	case EVSUBCMD_DEBUFF_CLEAR_UNIT_AT:
		unit = GetUnitAtPosition(argv[3] & 0xFF, (argv[3] & 0xFF00) >> 8);
		break;

	case EVSUBCMD_DEBUFF_SET_UNIT_ACTIVE:
	case EVSUBCMD_DEBUFF_CLEAR_UNIT_ACTIVE:
		unit = gActiveUnit;
		break;

	default:
		unit = NULL;
		break;
	}

	if (UNIT_IS_VALID(unit)) {
		if (set_or_clear == true)
			SetUnitStatusExt(unit, debuff, duaration);
		else {
			SetUnitStatusIndex(unit, 0);
			SetUnitStatusDuration(unit, 0);
		}
	}

	return EVC_ADVANCE_CONTINUE;
}

STATIC_DECLAR u8 DemoEventOperation_DebuffAll(struct EventEngineProc *proc, bool set_or_clear)
{
	const u16 *argv = proc->pEventCurrent;
	u16 debuff;
	u16 duaration;
	u16 pid;
	u16 jid;

	debuff = argv[1];
	duaration = argv[2];
	pid = jid = argv[3];

	if (duaration > 4)
		duaration = 4;

	switch (EVT_SUB_CMD(proc->pEventCurrent)) {
	case EVSUBCMD_DEBUFF_SET_ALL_PID:
	case EVSUBCMD_DEBUFF_CLEAR_ALL_PID:
		FOR_UNITS_ONMAP_ALL(unit, {
			if (UNIT_CHAR_ID(unit) == pid) {
				if (set_or_clear == true)
					SetUnitStatusExt(unit, debuff, duaration);
				else {
					SetUnitStatusIndex(unit, 0);
					SetUnitStatusDuration(unit, 0);
				}
			}
		})
		break;

	case EVSUBCMD_DEBUFF_SET_ALL_JID:
	case EVSUBCMD_DEBUFF_CLEAR_ALL_JID:
		FOR_UNITS_ONMAP_ALL(unit, {
			if (UNIT_CLASS_ID(unit) == jid) {
				if (set_or_clear == true)
					SetUnitStatusExt(unit, debuff, duaration);
				else {
					SetUnitStatusIndex(unit, 0);
					SetUnitStatusDuration(unit, 0);
				}
			}
		})
		break;

	default:
		break;
	};

	return EVC_ADVANCE_CONTINUE;
}

u8 EventDebuffOperation(struct EventEngineProc *proc)
{
	bool set_or_clear;

	switch (EVT_SUB_CMD(proc->pEventCurrent)) {
	case EVSUBCMD_DEBUFF_SET_FOR_UNIT:
	case EVSUBCMD_DEBUFF_SET_UNIT_SLOT1:
	case EVSUBCMD_DEBUFF_SET_UNIT_AT:
	case EVSUBCMD_DEBUFF_SET_UNIT_ACTIVE:
	case EVSUBCMD_DEBUFF_SET_ALL_PID:
	case EVSUBCMD_DEBUFF_SET_ALL_JID:
		set_or_clear = true;
		break;

	case EVSUBCMD_DEBUFF_CLEAR_FOR_UNIT:
	case EVSUBCMD_DEBUFF_CLEAR_UNIT_SLOT1:
	case EVSUBCMD_DEBUFF_CLEAR_UNIT_AT:
	case EVSUBCMD_DEBUFF_CLEAR_UNIT_ACTIVE:
	case EVSUBCMD_DEBUFF_CLEAR_ALL_PID:
	case EVSUBCMD_DEBUFF_CLEAR_ALL_JID:
		set_or_clear = false;
		break;

	default:
		Errorf("Event format error at %p", proc->pEventCurrent);
		hang();
	}

	switch (EVT_SUB_CMD(proc->pEventCurrent)) {
	case EVSUBCMD_DEBUFF_SET_FOR_UNIT:
	case EVSUBCMD_DEBUFF_SET_UNIT_SLOT1:
	case EVSUBCMD_DEBUFF_SET_UNIT_AT:
	case EVSUBCMD_DEBUFF_SET_UNIT_ACTIVE:
	case EVSUBCMD_DEBUFF_CLEAR_FOR_UNIT:
	case EVSUBCMD_DEBUFF_CLEAR_UNIT_SLOT1:
	case EVSUBCMD_DEBUFF_CLEAR_UNIT_AT:
	case EVSUBCMD_DEBUFF_CLEAR_UNIT_ACTIVE:
		return DemoEventOperation_Debuff_ForUnit(proc, set_or_clear);

	case EVSUBCMD_DEBUFF_SET_ALL_PID:
	case EVSUBCMD_DEBUFF_SET_ALL_JID:
	case EVSUBCMD_DEBUFF_CLEAR_ALL_PID:
	case EVSUBCMD_DEBUFF_CLEAR_ALL_JID:
		return DemoEventOperation_DebuffAll(proc, set_or_clear);

	default:
		Errorf("Event format error at %p", proc->pEventCurrent);
		hang();
	};

	return EVC_ADVANCE_CONTINUE;
}
