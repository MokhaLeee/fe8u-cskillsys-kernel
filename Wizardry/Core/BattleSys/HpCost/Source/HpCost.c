#include <common-chax.h>
#include <battle-system.h>
#include <combat-art.h>
#include <skill-system.h>
#include <constants/combat-arts.h>
#include <constants/skills.h>

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

STATIC_DECLAR NOINLINE void BattleGenerateHitHpCostCombatArt(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	if (attacker != &gBattleActor)
		return;

	switch (GetCombatArtInForce(&attacker->unit)) {
	case CID_BloodTribute:
#if (defined(SID_COMBAT_BloodTribute) && (COMMON_SKILL_VALID(SID_COMBAT_BloodTribute)))
		AddBattleHpCost(
			attacker,
			GetCurrentBattleHitRound(),
			simple_div(
				simple_div(attacker->hpInitial * SKILL_EFF0(SID_COMBAT_BloodTribute), 100)
					* SKILL_EFF1(SID_COMBAT_BloodTribute),
				100));
#endif
		break;

	case CID_CrimsonStrike:
#if (defined(SID_COMBAT_CrimsonStrike) && (COMMON_SKILL_VALID(SID_COMBAT_CrimsonStrike)))
		AddBattleHpCost(
			attacker,
			GetCurrentBattleHitRound(),
			simple_div(
				simple_div(attacker->hpInitial * SKILL_EFF0(SID_COMBAT_CrimsonStrike), 100)
					* SKILL_EFF1(SID_COMBAT_CrimsonStrike),
				100));
#endif
		break;

	case CID_VitalReckoning:
#if (defined(SID_COMBAT_VitalReckoning) && (COMMON_SKILL_VALID(SID_COMBAT_VitalReckoning)))
		AddBattleHpCost(
			attacker,
			GetCurrentBattleHitRound(),
			simple_div(
				simple_div(attacker->hpInitial * SKILL_EFF0(SID_COMBAT_VitalReckoning), 100)
					* SKILL_EFF1(SID_COMBAT_VitalReckoning),
				100));
#endif
		break;

	default:
		break;
	}
}

void BattleGenerateHitHpCost(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	BattleGenerateHitHpCostForGaidenMagic(attacker, defender);
	BattleGenerateHitHpCostCombatArt(attacker, defender);
}
