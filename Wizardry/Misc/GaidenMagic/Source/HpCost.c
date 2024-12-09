#include <common-chax.h>
#include <battle-system.h>

#include <gaiden-magic.h>

int GetGaidenWeaponHpCost(struct Unit *unit, int item)
{
	return gpGaidenWeaponCostList[ITEM_INDEX(item)];
}

void BattleGenerateHitHpCostForGaidenMagic(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	if (CheckGaidenMagicAttack(attacker)) {
		int cost = GetGaidenWeaponHpCost(&attacker->unit, attacker->weapon);
		int round = GetCurrentBattleHitRound();

		AddBattleHpCost(attacker, round, cost);
	}
}
