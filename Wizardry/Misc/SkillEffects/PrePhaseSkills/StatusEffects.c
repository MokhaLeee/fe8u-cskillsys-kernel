#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"
#include "debuff.h"

struct ProcPrePhaseBoon {
	PROC_HEADER;

	u8 uid;
};

STATIC_DECLAR void PrePhaseBoon_FindNextCharacter(struct ProcPrePhaseBoon *proc)
{
	// FOR_UNITS_ONMAP_FACTION
	while (++proc->uid < (gPlaySt.faction + 0x40)) {
		struct Unit *unit = GetUnit(proc->uid);

		if (!UnitOnMapAvaliable(unit))
			continue;

#if defined(SID_Boon) && (COMMON_SKILL_VALID(SID_Boon))
		if (SkillTester(unit, SID_Boon) && UnitHasNegativeStatus(unit)) {
			EnsureCameraOntoPosition(proc, unit->xPos, unit->yPos);
			return;
		}
#endif
	}
	Proc_Goto(proc, 99);
}

STATIC_DECLAR void PrePhaseBoon_ExecAnim(struct ProcPrePhaseBoon *proc)
{
	EndAllMus();

	/**
	 * Try skip anim
	 */
	if (CheckKernelHookSkippingFlag())
		return;

	StartStatusHealEffect(GetUnit(proc->uid), proc);
}

STATIC_DECLAR void PrePhaseBoon_ClearStatus(struct ProcPrePhaseBoon *proc)
{
	RemoveUnitNegativeStatus(GetUnit(proc->uid));
}

FORCE_DECLARE STATIC_DECLAR const struct ProcCmd ProcScr_PrePhaseBoon[] = {
	PROC_YIELD,

PROC_LABEL(1),
	PROC_CALL(PrePhaseBoon_FindNextCharacter),
	PROC_YIELD,
	PROC_CALL(PrePhaseBoon_ExecAnim),
	PROC_YIELD,
	PROC_CALL(PrePhaseBoon_ClearStatus),
	PROC_GOTO(1),

PROC_LABEL(99),
	PROC_END
};

bool PrePhase_UnitStatusEffectSkills(ProcPtr parent)
{
	FORCE_DECLARE struct ProcPrePhaseBoon *proc;

#if defined(SID_Boon) && (COMMON_SKILL_VALID(SID_Boon))
	proc = Proc_StartBlocking(ProcScr_PrePhaseBoon, parent);
	proc->uid = gPlaySt.faction;
	return true;
#endif

	return false;
}
