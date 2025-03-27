#include <common-chax.h>
#include <battle-system.h>
#include <skill-system.h>
#include <constants/skills.h>

#include <gaiden-magic.h>

int GetGaidenWeaponHpCost(struct Unit *unit, int item)
{
#if (defined(SID_Saint) && COMMON_SKILL_VALID(SID_Saint))
	if (SkillTester(unit, SID_Saint))
		return 0;
#endif

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
