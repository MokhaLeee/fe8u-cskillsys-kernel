#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

struct ProcYuneWhisper {
	PROC_HEADER;

	int uid;
	struct Unit *unit;
};

STATIC_DECLAR void YuneWhisper_Init(struct ProcYuneWhisper *proc)
{
	proc->uid = gPlaySt.faction + 1;
	proc->unit = NULL;
}

STATIC_DECLAR void YuneWhisper_Loop(struct ProcYuneWhisper *proc)
{
	int i;
	int res1, res2;

	// FOR_UNITS_ONMAP_FACTION
	while (proc->uid < (gPlaySt.faction + 0x40)) {
		struct Unit *unit = proc->unit = GetUnit(proc->uid++);

		if (!UNIT_IS_VALID(unit))
			continue;

#if (defined(SID_YuneWhispers) && (COMMON_SKILL_VALID(SID_YuneWhispers)))
		if (!SkillTester(unit, SID_YuneWhispers))
#else
		if (1)
#endif
			continue;

		res1 = GetUnitResistance(unit);
		InitTargets(unit->xPos, unit->yPos);

		for (i = 1; i < 0xC0; i++) {
			struct Unit *unit_tar;

			if (AreUnitsAllied(unit->index, i))
				continue;

			unit_tar = GetUnit(i);
			if (!UNIT_IS_VALID(unit_tar))
				continue;

			res2 = GetUnitResistance(unit_tar);

#if (defined(SID_YuneWhispers) && (COMMON_SKILL_VALID(SID_YuneWhispers)))
			if (res1 > (res2 + SKILL_EFF0(SID_YuneWhispers)))
#else
			if (res1 > (res2 + 3))
#endif
			{
				AddTarget(unit_tar->xPos, unit_tar->yPos, i, 0);
				LTRACEF("[act=0x%02X, tar=0x%02X] x=%d, y=%d", unit->index & 0xFF, unit_tar->index & 0xFF, unit_tar->xPos, unit_tar->yPos);
			}
		}

		if (GetSelectTargetCount() != 0) {
			Proc_Break(proc);
			return;
		}
	}
}

STATIC_DECLAR void YuneWhisper_Exec(struct ProcYuneWhisper *proc)
{
	int i;

	if (!UNIT_IS_VALID(proc->unit) || proc->uid >= (gPlaySt.faction + 0x40)) {
		Proc_Goto(proc, 99);
		return;
	}

	for (i = 0; i < GetSelectTargetCount(); i++) {
		struct SelectTarget *target = GetTarget(i);
		struct Unit *unit_tar = GetUnit(target->uid);

		if (UNIT_IS_VALID(unit_tar)) {
			/* For now, no anim is introduced */
			SetUnitStatDebuff(unit_tar, UNIT_STAT_DEBUFF_YuneWhispers);
		}
	}

	Proc_Break(proc);
}

FORCE_DECLARE STATIC_DECLAR const struct ProcCmd ProcScr_PrePhaseYuneWhisper[] = {
	PROC_CALL(YuneWhisper_Init),

PROC_LABEL(0),
	PROC_CALL(YuneWhisper_Loop),
	PROC_CALL(YuneWhisper_Exec),
	PROC_GOTO(0),

PROC_LABEL(99),
	PROC_END
};

bool PrePhsae_ExecYuneWhisperSkills(ProcPtr proc)
{
#if (defined(SID_YuneWhispers) && (COMMON_SKILL_VALID(SID_YuneWhispers)))
	Proc_StartBlocking(ProcScr_PrePhaseYuneWhisper, proc);
	return true;
#else
	return false;
#endif
}
