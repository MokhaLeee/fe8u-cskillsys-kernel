#include "common-chax.h"
#include "battle-system.h"

/**
 * Fix some value displayed for UI
 *
 * Although I think it is better to put
 * at the very beginning of sProcScr_CombatAction[],
 * but it is also okay to stay at tail of battle-generate
 */
void ModifyBattleStatusForUI(void)
{
	/* Fix for real-damage */
	if (gBattleActor.battleAttack < gBattleTarget.battleDefense)
		gBattleActor.battleAttack = gBattleTarget.battleDefense;

	if (gBattleTarget.battleAttack < gBattleActor.battleDefense)
		gBattleTarget.battleAttack = gBattleActor.battleDefense;

	gBattleActor.battleAttack += CalcBattleRealDamage(&gBattleActor, &gBattleTarget);
	gBattleTarget.battleAttack += CalcBattleRealDamage(&gBattleTarget, &gBattleActor);
}
