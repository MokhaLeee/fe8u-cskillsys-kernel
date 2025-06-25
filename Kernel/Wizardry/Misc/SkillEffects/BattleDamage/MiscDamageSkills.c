#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "constants/skills.h"

void BattleDamageCalc_MiscSkills(struct BattleUnit *attacker, struct BattleUnit *defender)
{
#if defined(SID_ImmovableObject) && (COMMON_SKILL_VALID(SID_ImmovableObject))
	if (BattleFastSkillTester(attacker, SID_ImmovableObject))
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_ImmovableObject));
	else if (BattleFastSkillTester(defender, SID_ImmovableObject))
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_ImmovableObject));
#endif

#if defined(SID_UnstoppableForce) && (COMMON_SKILL_VALID(SID_UnstoppableForce))
	if (BattleFastSkillTester(attacker, SID_UnstoppableForce))
		gDmg.increase += SKILL_EFF0(SID_UnstoppableForce);
	else if (BattleFastSkillTester(defender, SID_UnstoppableForce))
		gDmg.increase += SKILL_EFF0(SID_UnstoppableForce);
#endif
}
