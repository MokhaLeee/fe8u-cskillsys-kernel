#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "weapon-range.h"
#include "status-getter.h"

extern const struct AiCombatScoreCoefficients *sCombatScoreCoefficients;

STATIC_DECLAR int AiGetDamageDealtCombatScoreComponentVanilla(void)
{
	int score;

	if (gBattleTarget.unit.curHP == 0)
		return 50;

	score = (gBattleActor.battleAttack - gBattleTarget.battleDefense) * gBattleActor.battleEffectiveHitRate;

	if (score < 0)
		score = 0;

	score = Div(score, 100);
	score = sCombatScoreCoefficients->coeffDamageDealt * score;

	if (score > 40)
		score = 40;

	return score;
}

LYN_REPLACE_CHECK(AiGetDamageDealtCombatScoreComponent);
int AiGetDamageDealtCombatScoreComponent(void)
{
	int score = AiGetDamageDealtCombatScoreComponentVanilla();

#if defined(SID_Provoke) && (COMMON_SKILL_VALID(SID_Provoke))
	if (BattleFastSkillTester(&gBattleTarget, SID_Provoke))
		score += SKILL_EFF0(SID_Provoke);
#endif

	return score;
}
