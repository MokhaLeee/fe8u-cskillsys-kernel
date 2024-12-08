#include <common-chax.h>
#include <battle-system.h>

#include <gaiden-magic.h>

void AddBattleHpCost(int round, int cost)
{
	GetExtBattleHit(round)->hp_cost += cost;
}

void BattleGenerateHitHpCost(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	struct ExtBattleHit *hit = GetCurrentExtBattleHit();

	BattleGenerateHitHpCostForGaidenMagic(attacker, defender);

	/* On end */
	if (hit->hp_cost >= attacker->unit.curHP)
		hit->hp_cost = attacker->unit.curHP - 1;

	attacker->unit.curHP -= hit->hp_cost;
}
