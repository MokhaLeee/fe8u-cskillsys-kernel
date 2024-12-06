#include <common-chax.h>
#include <battle-system.h>

#include <gaiden-magic.h>

int GetGaidenWeaponHpCost(struct Unit *unit, int item)
{
	return gGaidenWeaponCostList[ITEM_INDEX(item)];
}

void BattleGenerateHitHpCostForGaidenMagic(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	if (CheckGaidenMagicAttack(attacker))
		AddBattleHpCost(GetCurrentBattleHitRound(), GetGaidenWeaponHpCost(&attacker->unit, attacker->weapon));
}
