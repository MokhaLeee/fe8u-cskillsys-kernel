#include "common-chax.h"
#include "strmag.h"
#include "debuff.h"
#include "battle-system.h"
#include "skill-system.h"
#include "constants/skills.h"

#if (defined(SID_LightAndDark) && (COMMON_SKILL_VALID(SID_LightAndDark)))
STATIC_DECLAR FORCE_DECLARE void ModifyBattleUnitStatus_LightAndDark(struct BattleUnit *actor, struct BattleUnit *target)
{
	target->unit.pow -= SKILL_EFF0(SID_LightAndDark);
	UNIT_MAG(&target->unit) -= SKILL_EFF0(SID_LightAndDark);
	target->unit.spd -= SKILL_EFF1(SID_LightAndDark);
	target->unit.def -= SKILL_EFF2(SID_LightAndDark);
	target->unit.res -= SKILL_EFF3(SID_LightAndDark);

	RemoveUnitNegativeStatus(&actor->unit);
	RemoveUnitPositiveStatus(&target->unit);
}
#endif

void PreBattleGenerate_LightAndDark(void)
{
#if (defined(SID_LightAndDark) && (COMMON_SKILL_VALID(SID_LightAndDark)))
	struct BattleUnit *actor  = &gBattleActor;
	struct BattleUnit *target = &gBattleTarget;

	if (BattleFastSkillTester(actor, SID_LightAndDark))
		ModifyBattleUnitStatus_LightAndDark(actor, target);

	if (BattleFastSkillTester(target, SID_LightAndDark))
		ModifyBattleUnitStatus_LightAndDark(target, actor);
#endif
}
