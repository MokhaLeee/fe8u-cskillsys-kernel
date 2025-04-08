#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "unit-expa.h"
#include "kernel-lib.h"
#include "combat-art.h"
#include "efx-skill.h"
#include "strmag.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

#define LOCAL_TRACE 0

typedef void (*BattleDamageCalcFunc) (struct BattleUnit *buA, struct BattleUnit *buB);
extern BattleDamageCalcFunc const *const gpBattleDamageCalcFuncs;

typedef int (*BattleRealDamageCalcFunc) (int old, struct BattleUnit *buA, struct BattleUnit *buB);
extern BattleRealDamageCalcFunc const *const gpBattleRealDamageCalcFuncs;

int CalcBattleRealDamage(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	const BattleRealDamageCalcFunc *it;

	int damage = 0;

#if defined(SID_RuinedBlade) && (COMMON_SKILL_VALID(SID_RuinedBlade))
	if (BattleFastSkillTester(attacker, SID_RuinedBlade))
		damage += SKILL_EFF2(SID_RuinedBlade);
#endif

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
	if (BattleFastSkillTester(attacker, SID_RuinedBladePlus))
		damage += SKILL_EFF1(SID_RuinedBladePlus);
#endif

#if defined(SID_LunaAttack) && (COMMON_SKILL_VALID(SID_LunaAttack))
	if (BattleFastSkillTester(attacker, SID_LunaAttack))
		damage += defender->battleDefense / 4;
#endif

#if defined(SID_Bushido) && (COMMON_SKILL_VALID(SID_Bushido))
	if (BattleFastSkillTester(attacker, SID_Bushido))
		damage += SKILL_EFF1(SID_Bushido);
#endif

#if defined(SID_Scendscale) && (COMMON_SKILL_VALID(SID_Scendscale))
	if (BattleFastSkillTester(attacker, SID_Scendscale))
		damage += Div(attacker->battleAttack * SKILL_EFF0(SID_Bushido), 100);
#endif

	for (it = gpBattleRealDamageCalcFuncs; *it; it++)
		damage = (*it)(damage, attacker, defender);

	return damage;
}

int BattleHit_CalcDamage(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	const BattleDamageCalcFunc *it;

	FORCE_DECLARE bool barricadePlus_activated;
	int base_damage, crit_correction, result;

	FORCE_DECLARE struct BattleGlobalFlags *act_flags, *tar_flags;

	/**
	 * result = ([atk + gDmg.correction - def])
	 *			  * (100% + increase%)
	 *			  * (100% + crit_correction%)
	 *			  / (100% + decrease%)
	 *			  + real_damage
	 */

	if (attacker == &gBattleActor) {
		act_flags = &gBattleActorGlobalFlag;
		tar_flags = &gBattleTargetGlobalFlag;
	} else {
		act_flags = &gBattleTargetGlobalFlag;
		tar_flags = &gBattleActorGlobalFlag;
	}

	gDmg.crit_atk = false;
	gDmg.correction = 0;
	gDmg.increase = 100;
	gDmg.decrease = 0x100;
	gDmg.crit_correction = gpKernelBattleDesignerConfig->crit_correction;
	gDmg.real_damage = CalcBattleRealDamage(attacker, defender);

	/**
	 * Roll critical and silencer attack
	 */
#if defined(SID_Fortune) && (COMMON_SKILL_VALID(SID_Fortune))
	if (BattleFastSkillTester(defender, SID_Fortune))
		gBattleStats.critRate = 0;
#endif

#if defined(SID_Foresight) && (COMMON_SKILL_VALID(SID_Foresight))
	if (BattleFastSkillTester(defender, SID_Foresight))
		gBattleStats.critRate = 0;
#endif

	if (BattleRoll1RN(gBattleStats.critRate, false)) {
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
		gDmg.crit_atk = true;

		if (BattleRoll1RN(gBattleStats.silencerRate, false)) {
			/* Directly return on silencer attack to fasten calc */
			gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_SILENCER;
			gDmg.result = BATTLE_MAX_DAMAGE;
			return BATTLE_MAX_DAMAGE;
		}
	}


	/**
	 * calc-loop
	 */
	for (it = gpBattleDamageCalcFuncs; *it; it++)
		(*it)(attacker, defender);

	/**
	 * Calculate result
	 */
	if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_SILENCER) {
		gDmg.result = BATTLE_MAX_DAMAGE;
		return BATTLE_MAX_DAMAGE;
	} else if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_GREATSHLD) {
		gDmg.result = 0;
		return 0;
	}

	base_damage = gBattleStats.attack + gDmg.correction - gBattleStats.defense;
	crit_correction = (gDmg.crit_atk) ? gDmg.crit_correction : 100;

	if (base_damage <= 0)
		base_damage = 0;

	if (base_damage <= 0 && gDmg.real_damage <= 0) {
		/* If no damage taken, directly end the damage calculation */
		gDmg.result = 0;
		return 0;
	}

	/**
	 * Step6: Calculate result
	 */
	{
		u32 dividend, divisor, quotient;

		dividend = base_damage * gDmg.increase * crit_correction * 0x100;
		divisor  = 100 * 100 * gDmg.decrease;

		// quotient = k_udiv(dividend, divisor);
		quotient = DIV_ROUND_CLOSEST(dividend, divisor);

		LTRACEF("dividend=%ld, divisor=%ld, quotient=%ld", dividend, divisor, quotient);
		result = quotient;
	}

	if (result == 0 && base_damage > 0)
		result = 1; // at least 1 damage left.

	result += gDmg.real_damage;

	LTRACEF("[round %d] dmg=%d: base=%d (atk=%d, def=%d, cor=%d), inc=%d, crt=%d, dec=%d, real=%d",
					GetBattleHitRound(gBattleHitIterator), result, base_damage,
					gBattleStats.attack, gBattleStats.defense, gDmg.correction, gDmg.increase, crit_correction, gDmg.decrease, gDmg.real_damage);

	if (result > BATTLE_MAX_DAMAGE)
		result = BATTLE_MAX_DAMAGE;

	gDmg.result = result;
	return result;
}
