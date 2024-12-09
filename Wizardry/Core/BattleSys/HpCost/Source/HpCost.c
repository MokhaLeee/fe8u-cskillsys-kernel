#include <common-chax.h>
#include <battle-system.h>

#include <gaiden-magic.h>

bool TryBattleHpCost(struct BattleUnit *bu, int hp_cost)
{
	if (hp_cost >= bu->unit.curHP)
		return false;

	return true;
}

bool AddBattleHpCost(struct BattleUnit *bu, int round, int hp_cost)
{
	if (!TryBattleHpCost(bu, hp_cost))
		return false;

	bu->unit.curHP -= hp_cost;
	GetExtBattleHit(round)->hp_cost += hp_cost;
	return true;
}

void BattleGenerateHitHpCost(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	BattleGenerateHitHpCostForGaidenMagic(attacker, defender);
}
