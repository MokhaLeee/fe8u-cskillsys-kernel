#include "common-chax.h"
#include "unit-expa.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"

struct ProcEndTurnFreeSpirit {
	PROC_HEADER;

	int uid;
};

static bool Local_SkillTester(struct Unit *unit)
{
#if (defined(SID_FreeSpirit) && COMMON_SKILL_VALID(SID_FreeSpirit))
	if (SkillTester(unit, SID_FreeSpirit))
		return true;
#endif

	return false;
}

static bool CheckHasActed(struct Unit *unit)
{
	if (CheckBitUES(unit, UES_BIT_ACTED))
		return true;

	if (unit->state & (US_HAS_MOVED | US_UNSELECTABLE))
		return true;

	return false;
}

STATIC_DECLAR void EndTurnFreeSpirit_Init(struct ProcEndTurnFreeSpirit *proc)
{
	proc->uid = gPlaySt.faction;
}

FORCE_DECLARE static void callback2(ProcPtr proc)
{
	SetUnitStatDebuff(gActiveUnit, UNIT_STAT_BUFF_FREESPIRIT);
}

STATIC_DECLAR void EndTurnFreeSpirit_Loop(struct ProcEndTurnFreeSpirit *proc)
{
	proc->uid++;

	if (proc->uid >= (gPlaySt.faction + 0x40)) {
		Proc_Goto(proc, 1);
		return;
	}

	for (; proc->uid < (gPlaySt.faction + 0x40); proc->uid++) {
		struct Unit *unit = GetUnit(proc->uid);

		if (!UNIT_IS_VALID(unit))
			continue;

		if (!UnitOnMapAvaliable(unit))
			continue;

		if (!Local_SkillTester(unit))
			continue;

		if (CheckHasActed(unit))
			continue;

		gActiveUnit = unit;

#if (defined(SID_FreeSpirit) && COMMON_SKILL_VALID(SID_FreeSpirit))
		if (!UnitOnMapAvaliable(unit))
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_FREESPIRIT);
		else
			NewMuSkillAnimOnActiveUnitWithDeamon(proc, SID_FreeSpirit, NULL, callback2);
#endif
		return;
	}

	if (proc->uid >= (gPlaySt.faction + 0x40)) {
		Proc_Goto(proc, 1);
		return;
	}
}

void EndTurnFreeSpirit_End(ProcPtr proc)
{
	RemoveMuForActiveUnit(5);
}

STATIC_DECLAR const struct ProcCmd ProcScr_EndTurnFreeSpirit[] = {
	PROC_CALL(EndTurnFreeSpirit_Init),
	PROC_YIELD,

PROC_LABEL(0),
	PROC_CALL(EndTurnFreeSpirit_Loop),
	PROC_YIELD,
	PROC_GOTO(0),

PROC_LABEL(1),
	PROC_CALL(EndTurnFreeSpirit_End),
	PROC_END,
};

bool EndTurnSkill_FreeSpirit(ProcPtr parent)
{
#if (defined(SID_FreeSpirit) && COMMON_SKILL_VALID(SID_FreeSpirit))
	if (CheckKernelHookSkippingFlag()) {
		FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
			if (!Local_SkillTester(unit))
				continue;

			if (!CheckHasActed(unit))
				SetUnitStatDebuff(unit, UNIT_STAT_BUFF_FREESPIRIT);
		})
		return false;
	}

	Proc_StartBlocking(ProcScr_EndTurnFreeSpirit, parent);
	return true;
#endif

	return false;
}
