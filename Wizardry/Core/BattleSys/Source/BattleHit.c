#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "combat-art.h"
#include "gaiden-magic.h"
#include "shield.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(BattleUpdateBattleStats);
void BattleUpdateBattleStats(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	int attack = attacker->battleAttack;
	int defense = defender->battleDefense;
	int hitRate = attacker->battleEffectiveHitRate;
	int critRate = attacker->battleEffectiveCritRate;
	int silencerRate = attacker->battleSilencerRate;

	/* Fasten simulation */
	if (gBattleStats.config & BATTLE_CONFIG_SIMULATE) {
		gBattleStats.attack = attack;
		gBattleStats.defense = defense;
		gBattleStats.hitRate = hitRate;
		gBattleStats.critRate = critRate;
		gBattleStats.silencerRate = silencerRate;
		return;
	}

#if defined(SID_AxeFaith) && (COMMON_SKILL_VALID(SID_AxeFaith))
	if (attacker->weaponType == ITYPE_AXE && CheckBattleSkillActivate(attacker, defender, SID_AxeFaith, attacker->battleAttack)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_AxeFaith);
		hitRate += attacker->battleAttack;
	}
#endif

	gBattleTemporaryFlag.skill_activated_sure_shoot = false;

#if (defined(SID_SureShot) && (COMMON_SKILL_VALID(SID_SureShot)))
	if (CheckBattleSkillActivate(attacker, defender, SID_SureShot, attacker->unit.skl)) {
		gBattleTemporaryFlag.skill_activated_sure_shoot = true;
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_SureShot);
		hitRate = 100;
	}
#endif

	gBattleTemporaryFlag.skill_activated_dead_eye = false;

#if defined(SID_Deadeye) && (COMMON_SKILL_VALID(SID_Deadeye))
	if (CheckBattleSkillActivate(attacker, defender, SID_Deadeye, attacker->unit.skl)) {
		gBattleTemporaryFlag.skill_activated_dead_eye = true;
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Deadeye);
		hitRate *= 2;
	}
#endif

#if defined(SID_Momentum) && (COMMON_SKILL_VALID(SID_Momentum))
	if (BattleFastSkillTester(attacker, SID_Momentum))
		critRate += SKILL_EFF0(SID_Momentum) * GetBattleGlobalFlags(attacker)->round_cnt_hit;
#endif

#if defined(SID_LimitBreak) && (COMMON_SKILL_VALID(SID_LimitBreak))
	if (BattleFastSkillTester(attacker, SID_LimitBreak)) {
		if (GetBattleGlobalFlags(attacker)->round_cnt_hit == SKILL_EFF0(SID_LimitBreak)) {
			RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LimitBreak);
			hitRate = 100;
		}
	}
#endif

#if (defined(SID_ImmovableObject) && COMMON_SKILL_VALID(SID_ImmovableObject))
	if (BattleFastSkillTester(attacker, SID_ImmovableObject) || BattleFastSkillTester(defender, SID_ImmovableObject))
		hitRate = 100;
#endif

#if (defined(SID_UnstoppableForce) && COMMON_SKILL_VALID(SID_UnstoppableForce))
	if (BattleFastSkillTester(attacker, SID_UnstoppableForce) || BattleFastSkillTester(defender, SID_UnstoppableForce))
		hitRate = 100;
#endif

	LIMIT_AREA(gBattleStats.attack, 0, 255);
	LIMIT_AREA(gBattleStats.defense, 0, 255);
	LIMIT_AREA(gBattleStats.hitRate, 0, 100);
	LIMIT_AREA(gBattleStats.critRate, 0, 100);
	LIMIT_AREA(gBattleStats.silencerRate, 0, 100);

	gBattleStats.attack = attack;
	gBattleStats.defense = defense;
	gBattleStats.hitRate = hitRate;
	gBattleStats.critRate = critRate;
	gBattleStats.silencerRate = silencerRate;
}

LYN_REPLACE_CHECK(BattleGenerateHitAttributes);
void BattleGenerateHitAttributes(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	gBattleStats.damage = 0;

	/* Fasten simulation */
	if (!BattleRoll2RN(gBattleStats.hitRate, FALSE)) {
#if (defined(SID_DivinePulse) && (COMMON_SKILL_VALID(SID_DivinePulse)))
		if (BattleRoll2RN(gBattleStats.hitRate, FALSE) &&
			CheckBattleSkillActivate(attacker, defender, SID_DivinePulse, SKILL_EFF0(SID_DivinePulse) + attacker->unit.lck))
			RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DivinePulse);
		else {
			RegisterHitCnt(attacker, true);
			gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
			return;
		}
#else
		RegisterHitCnt(attacker, true);
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
		return;
#endif
	}

	RegisterHitCnt(attacker, false);

	gBattleStats.damage = BattleHit_CalcDamage(attacker, defender);

	if (gBattleStats.config & BATTLE_CONFIG_REAL) {
		if (gDmg.real_damage > 0)
			TriggerKtutorial(KTUTORIAL_REAL_DAMAGE);
	}

	BattleCheckPetrify(attacker, defender);

	if (gBattleStats.damage != 0)
		attacker->nonZeroDamage = TRUE;
}

LYN_REPLACE_CHECK(BattleGenerateHitEffects);
void BattleGenerateHitEffects(struct BattleUnit *attacker, struct BattleUnit *defender)
{
#if (defined(SID_Discipline) && (COMMON_SKILL_VALID(SID_Discipline)))
		if (BattleFastSkillTester(attacker, SID_Discipline))
			attacker->wexpMultiplier += 2;
		else
			attacker->wexpMultiplier++;
#else
		attacker->wexpMultiplier++;
#endif

	if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS)) {
		if (CheckBattleHpHalve(attacker, defender)) {
			gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPHALVE;
			gBattleStats.damage = defender->unit.curHP / 2;
		}

		if (CheckDevilAttack(attacker, defender)) {
			gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_DEVIL;
			if (gBattleStats.damage > attacker->unit.curHP)
				gBattleStats.damage = attacker->unit.curHP;

			attacker->unit.curHP -= gBattleStats.damage;

			if (attacker->unit.curHP < 0)
				attacker->unit.curHP = 0;
		} else {
			if (gBattleStats.damage > defender->unit.curHP)
				gBattleStats.damage = defender->unit.curHP;

#if defined(SID_Bane) && (COMMON_SKILL_VALID(SID_Bane))
			if (gBattleStats.damage < (defender->unit.curHP - 1)) {
				if (CheckBattleSkillActivate(attacker, defender, SID_Bane, attacker->unit.skl)) {
					RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Bane);
					gBattleStats.damage = defender->unit.curHP - 1;
					gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
				}
			}
#endif
			defender->unit.curHP -= gBattleStats.damage;

			if (defender->unit.curHP < 0)
				defender->unit.curHP = 0;
		}

#if CHAX
		BattleHit_CalcHpDrain(attacker, defender);
#else
		if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN) {
			if (attacker->unit.maxHP < (attacker->unit.curHP + gBattleStats.damage))
				attacker->unit.curHP = attacker->unit.maxHP;
			else
				attacker->unit.curHP += gBattleStats.damage;

			gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
		}
#endif

		BattleHit_InjectNegativeStatus(attacker, defender);
	}

	gBattleHitIterator->hpChange = gBattleStats.damage;

	BattleHit_ConsumeWeapon(attacker, defender);
	BattleHit_ConsumeShield(attacker, defender);
}

LYN_REPLACE_CHECK(BattleGenerateHit);
bool BattleGenerateHit(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	if (attacker == &gBattleTarget)
		gBattleHitIterator->info |= BATTLE_HIT_INFO_RETALIATION;

	BattleUpdateBattleStats(attacker, defender);

#if CHAX
	/**
	 * Gaiden magic needs hp-cost
	 */
	if (CheckGaidenMagicAttack(attacker)) {
		int hp_cost = GetGaidenWeaponHpCost(&attacker->unit, attacker->weapon);

		if (!TryBattleHpCost(attacker, hp_cost)) {
			gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
			gBattleHitIterator++;
			return true;
		}
	}
#endif

	/**
	 * Hp cost must be calculated first
	 */
	BattleGenerateHitHpCost(attacker, defender);

	BattleGenerateHitTriangleAttack(attacker, defender);
	BattleGenerateHitAttributes(attacker, defender);
	BattleGenerateHitEffects(attacker, defender);

	if (attacker->unit.curHP == 0 || defender->unit.curHP == 0) {
#if (defined(SID_Discipline) && (COMMON_SKILL_VALID(SID_Discipline)))
		if (BattleFastSkillTester(attacker, SID_Discipline))
			attacker->wexpMultiplier += 2;
		else
			attacker->wexpMultiplier++;
#else
		attacker->wexpMultiplier++;
#endif

		gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;

#if CHAX
		if (defender->unit.curHP == 0) {
			if (CheckBattleInori(attacker, defender)) {
				gBattleStats.damage = gBattleStats.damage - 1;
				gBattleHitIterator->hpChange = gBattleStats.damage;
				defender->unit.curHP = 1;

				gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
				gBattleHitIterator++;
				return true;
			}

#if (defined(SID_OverKill) && (COMMON_SKILL_VALID(SID_OverKill)))
			if (BattleFastSkillTester(attacker, SID_OverKill)) {
				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_OverKill);
				AppendHpDrain(attacker, defender, gDmg.result - gBattleStats.damage);
			}
#endif
		}
#endif

		if (gBattleTarget.unit.curHP == 0) {
			gBattleActorGlobalFlag.enimy_defeated = true;

#if (defined(SID_Galeforce) && (COMMON_SKILL_VALID(SID_Galeforce)))
			if (CheckBattleSkillActivate(&gBattleActor, &gBattleTarget, SID_Galeforce, gBattleActor.unit.skl))
				gBattleActorGlobalFlag.skill_activated_galeforce = true;
#endif

#if (defined(SID_Pickup) && (COMMON_SKILL_VALID(SID_Pickup)))
			if (CheckBattleSkillActivate(&gBattleActor, &gBattleTarget, SID_Pickup, gBattleActor.unit.lck)) {
				struct Unit *unit_tar = &gBattleTarget.unit;

				unit_tar->state |= US_DROP_ITEM;
			}
#endif
			gBattleHitIterator->info |= BATTLE_HIT_INFO_KILLS_TARGET;
		}

		gBattleHitIterator++;
		return true;
	} else if (defender->statusOut == UNIT_STATUS_PETRIFY || defender->statusOut == UNIT_STATUS_13 || defender->statusOut == UNIT_STATUS_SLEEP) {
		gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
		gBattleHitIterator++;
		return true;
	}

	gBattleHitIterator++;
	return false;
}
