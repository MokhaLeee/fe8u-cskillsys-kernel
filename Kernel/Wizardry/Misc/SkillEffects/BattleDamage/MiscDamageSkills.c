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
}
