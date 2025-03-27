#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "map-anims.h"
#include "kernel-lib.h"
#include "constants/skills.h"

STATIC_DECLAR void ExecSkillSavageBlowEffectAnim(ProcPtr proc)
{
	int i;
	struct Unit *unit = gActiveUnit;
	struct Unit *target = GetUnit(gActionData.targetIndex);

	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);
	GenerateUnitStandingReachRange(target, 0b11);

	InitTargets(unit->xPos, unit->yPos);
	ForEachUnitInRange(AddUnitToTargetListIfNotAllied);

	for (i = 0; i < GetSelectTargetCount(); i++) {
		struct SelectTarget *starget = GetTarget(i);

		CallMapAnim_HeavyGravity(proc, starget->x, starget->y);
	}
}

STATIC_DECLAR void SkillSavageBlowPostAnimEffect(ProcPtr proc)
{
	int i;
#ifdef SID_SavageBlow
	int perc = SKILL_EFF0(SID_SavageBlow);
#else
	int perc = 20;
#endif
	int damage = Div(gBattleStats.damage * perc, 100);

	for (i = 0; i < GetSelectTargetCount(); i++) {
		struct SelectTarget *starget = GetTarget(i);
		struct Unit *tunit = GetUnit(starget->uid);

		if (tunit->curHP <= damage)
			tunit->curHP = 1;
		else
			tunit->curHP = tunit->curHP - damage;
	}
}

STATIC_DECLAR const struct ProcCmd ProcScr_PostActionSkillSavageBlow[] = {
	PROC_NAME("PostActionSkillSavageBlow"),
	PROC_YIELD,
	PROC_CALL(ExecSkillSavageBlowEffectAnim),
	PROC_WHILE(MapAnimHeavyGravityExists),
	PROC_CALL(SkillSavageBlowPostAnimEffect),
	PROC_END
};

bool PostActionSkillSavageBlow(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *unit = gActiveUnit;
	FORCE_DECLARE struct Unit *target = GetUnit(gActionData.targetIndex);

	if (!UNIT_IS_VALID(unit))
		return false;

#if defined(SID_SavageBlow) && (COMMON_SKILL_VALID(SID_SavageBlow))
	if (!SkillListTester(unit, SID_SavageBlow) || !UNIT_IS_VALID(target))
#else
	if (1)
#endif
		return false;

	if (!UnitAvaliable(gActiveUnit) || UNIT_STONED(gActiveUnit))
		return false;

	Proc_StartBlocking(ProcScr_PostActionSkillSavageBlow, parent);
	return true;
}
