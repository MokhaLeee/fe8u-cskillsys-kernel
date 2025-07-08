#include "common-chax.h"
#include "battle-system.h"

#define LOCAL_TRACE 1

/**
 * Fix some value displayed for UI
 *
 * Although I think it is better to put
 * at the very beginning of sProcScr_CombatAction[],
 * but it is also okay to stay at tail of battle-generate
 */
STATIC_DECLAR void Local_ModifyBattleStatusExt(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	struct BaseDmg *base_dmg = GetBaseDmg(attacker);

	if (attacker->battleAttack > defender->battleDefense) {
		u32 simu_dmg, dividend, divisor, quotient;

		simu_dmg = attacker->battleAttack - defender->battleDefense;

		dividend = simu_dmg * base_dmg->increase * 0x100;
		divisor  = 1 * 100 * base_dmg->decrease;
		quotient = k_udiv(dividend, divisor);

		LTRACEF("dividend=%ld, divisor=%ld, quotient=%ld", dividend, divisor, quotient);
		attacker->battleAttack = defender->battleDefense + quotient;
	}

	if (base_dmg->real_damage > 0) {
		if (attacker->battleAttack < defender->battleDefense)
			attacker->battleAttack = defender->battleDefense;

		attacker->battleAttack += base_dmg->real_damage;
	}
}

void ModifyBattleStatusForUI(void)
{
	if (gBattleStats.config & BATTLE_CONFIG_SIMULATE) {
		Local_ModifyBattleStatusExt(&gBattleActor, &gBattleTarget);
		Local_ModifyBattleStatusExt(&gBattleTarget, &gBattleActor);
	}
}
