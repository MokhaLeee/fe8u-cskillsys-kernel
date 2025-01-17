#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "gaiden-magic.h"
#include "combat-art.h"
#include "combo-attack.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

STATIC_DECLAR bool CheckCanContinueAttack(struct BattleUnit *bu)
{
	if (!bu->weapon)
		return false;

	if (CheckGaidenMagicAttack(bu))
		if (bu->unit.curHP <= GetGaidenWeaponHpCost(&bu->unit, bu->weapon))
			return false;

	return true;
}

/* This function should also be called by BKSEL, so non static */
bool CheckCanTwiceAttackOrder(struct BattleUnit *actor, struct BattleUnit *target)
{
	FORCE_DECLARE bool followup_nullified_en = true;
	FORCE_DECLARE bool ref_actor_hp_above_half  = ((actor->hpInitial  * 2) > actor->unit.maxHP);
	FORCE_DECLARE bool ref_target_hp_above_half = ((target->hpInitial * 2) > target->unit.maxHP);

	if (target->battleSpeed > 250)
		return false;

	if (!actor->weapon)
		return false;

	if (GetItemWeaponEffect(actor->weaponBefore) == WPN_EFFECT_HPHALVE)
		return false;

	if (GetItemIndex(actor->weapon) == ITEM_MONSTER_STONE)
		return false;

	/* Check combat-art */
	if (&gBattleActor == actor) {
		int cid = GetCombatArtInForce(&actor->unit);

		if (COMBART_VALID(cid)) {
			switch (GetCombatArtInfo(cid)->double_attack) {
			case COMBART_DOUBLE_DISABLED:
				return false;

			case COMBART_DOUBLE_FORCE_ENABLED:
				return true;

			case COMBART_DOUBLE_ENABLED:
			default:
				break;
			}
		}
	}

	/* Check can prevent the follow-up attack */
	followup_nullified_en = true;
	if (&gBattleActor == actor) {
#if defined(SID_YngviAscendant) && (COMMON_SKILL_VALID(SID_YngviAscendant))
		if (BattleFastSkillTester(actor, SID_YngviAscendant))
			followup_nullified_en = false;
#endif
	} else {
#if defined(SID_DragonWrath) && (COMMON_SKILL_VALID(SID_DragonWrath))
		if (BattleFastSkillTester(actor, SID_DragonWrath))
			followup_nullified_en = false;
#endif
	}

	if (followup_nullified_en) {
		if (&gBattleActor == actor) {
#if defined(SID_WaryFighter) && (COMMON_SKILL_VALID(SID_WaryFighter))
			if (BattleFastSkillTester(target, SID_WaryFighter))
				if (ref_target_hp_above_half)
					return false;
#endif

#if defined(SID_Moonlight) && (COMMON_SKILL_VALID(SID_Moonlight))
			if (BattleFastSkillTester(actor, SID_Moonlight))
				return false;
#endif
		}
	}

	/* Check attacker */
	if (&gBattleActor == actor) {
		gBattleTemporaryFlag.act_force_twice_order = false;

#if defined(SID_BoldFighter) && (COMMON_SKILL_VALID(SID_BoldFighter))
		if (BattleFastSkillTester(actor, SID_BoldFighter) && ref_actor_hp_above_half) {
			gBattleTemporaryFlag.act_force_twice_order = true;
			RegisterBattleOrderSkill(SID_BoldFighter, BORDER_ACT_TWICE);
			return true;
		}
#endif

#if defined(SID_RecklessFighter) && (COMMON_SKILL_VALID(SID_RecklessFighter))
		if (BattleFastSkillTester(actor, SID_RecklessFighter) && ref_actor_hp_above_half) {
			RegisterBattleOrderSkill(SID_RecklessFighter, BORDER_ACT_TWICE);
			gBattleTemporaryFlag.act_force_twice_order = true;
			return true;
		} else if (BattleFastSkillTester(target, SID_RecklessFighter) && (target->hpInitial * 2) >= target->unit.maxHP) {
			gBattleTemporaryFlag.act_force_twice_order = true;
			return true;
		}
#endif

#if defined(SID_BidingBlow) && (COMMON_SKILL_VALID(SID_BidingBlow))
		if (BattleFastSkillTester(actor, SID_BidingBlow)) {
			if (target->canCounter == false) {
				gBattleTemporaryFlag.act_force_twice_order = true;
				RegisterBattleOrderSkill(SID_BidingBlow, BORDER_ACT_TWICE);
				return true;
			}
		}
#endif

#if defined(SID_AdvantageChaser) && (COMMON_SKILL_VALID(SID_AdvantageChaser))
		if (BattleFastSkillTester(actor, SID_AdvantageChaser)) {
			if (actor->wTriangleDmgBonus > 0 || actor->wTriangleHitBonus > 0) {
				gBattleTemporaryFlag.act_force_twice_order = true;
				RegisterBattleOrderSkill(SID_AdvantageChaser, BORDER_ACT_TWICE);
				return true;
			}
		}
#endif

#if defined(SID_PridefulWarrior) && (COMMON_SKILL_VALID(SID_PridefulWarrior))
		if (BattleFastSkillTester(actor, SID_PridefulWarrior)) {
			gBattleTemporaryFlag.act_force_twice_order = true;
			RegisterBattleOrderSkill(SID_PridefulWarrior, BORDER_ACT_TWICE);
			return true;
		}
#endif

#if defined(SID_PassionsFlow) && (COMMON_SKILL_VALID(SID_PassionsFlow))
		if (BattleFastSkillTester(actor, SID_PassionsFlow)) {
			struct SupportBonuses bonuses;

			if (GetUnitSupportBonuses(GetUnit(actor->unit.index), &bonuses) != 0) {
				gBattleTemporaryFlag.act_force_twice_order = true;
				RegisterBattleOrderSkill(SID_PassionsFlow, BORDER_ACT_TWICE);
				return true;
			}
		}
#endif

#if defined(SID_BrashAssault) && (COMMON_SKILL_VALID(SID_BrashAssault))
		if (BattleFastSkillTester(actor, SID_BrashAssault)) {
			if (target->canCounter) {
				gBattleTemporaryFlag.act_force_twice_order = true;
				RegisterBattleOrderSkill(SID_BrashAssault, BORDER_ACT_TWICE);
				return true;
			}
		}
#endif

	} else if (&gBattleTarget == actor) {
		gBattleTemporaryFlag.tar_force_twice_order = false;

#if defined(SID_VengefulFighter) && (COMMON_SKILL_VALID(SID_VengefulFighter))
		if (BattleFastSkillTester(actor, SID_VengefulFighter) && ref_actor_hp_above_half) {
			gBattleTemporaryFlag.tar_force_twice_order = true;
			RegisterBattleOrderSkill(SID_VengefulFighter, BORDER_TAR_TWICE);
			return true;
		}
#endif

#if defined(SID_DragonWrath) && (COMMON_SKILL_VALID(SID_DragonWrath))
		if (BattleFastSkillTester(actor, SID_DragonWrath) && ref_actor_hp_above_half) {
			gBattleTemporaryFlag.tar_force_twice_order = true;
			RegisterBattleOrderSkill(SID_DragonWrath, BORDER_TAR_TWICE);
			return true;
		}
#endif

#if defined(SID_RecklessFighter) && (COMMON_SKILL_VALID(SID_RecklessFighter))
		if (BattleFastSkillTester(actor, SID_RecklessFighter)) {
			RegisterBattleOrderSkill(SID_RecklessFighter, BORDER_TAR_TWICE);
			gBattleTemporaryFlag.tar_force_twice_order = true;
			return true;
		} else if (BattleFastSkillTester(target, SID_RecklessFighter)) {
			gBattleTemporaryFlag.tar_force_twice_order = true;
			return true;
		}
#endif

#if defined(SID_QuickRiposte) && (COMMON_SKILL_VALID(SID_QuickRiposte))
		if (BattleFastSkillTester(actor, SID_QuickRiposte)) {
			if (ref_actor_hp_above_half) {
				gBattleTemporaryFlag.tar_force_twice_order = true;
				RegisterBattleOrderSkill(SID_QuickRiposte, BORDER_TAR_TWICE);
				return true;
			}
		}
#endif

#if defined(SID_AdvantageChaser) && (COMMON_SKILL_VALID(SID_AdvantageChaser))
		if (BattleFastSkillTester(actor, SID_AdvantageChaser)) {
			if (actor->wTriangleDmgBonus > 0 || actor->wTriangleHitBonus > 0) {
				gBattleTemporaryFlag.tar_force_twice_order = true;
				RegisterBattleOrderSkill(SID_AdvantageChaser, BORDER_TAR_TWICE);
				return true;
			}
		}
#endif

#if defined(SID_PassionsFlow) && (COMMON_SKILL_VALID(SID_PassionsFlow))
		if (BattleFastSkillTester(actor, SID_PassionsFlow)) {
			struct SupportBonuses bonuses;

			if (GetUnitSupportBonuses(GetUnit(actor->unit.index), &bonuses) != 0) {
				gBattleTemporaryFlag.tar_force_twice_order = true;
				RegisterBattleOrderSkill(SID_PassionsFlow, BORDER_TAR_TWICE);
				return true;
			}
		}
#endif
	}

	/* Basic judgement */
	return ((actor->battleSpeed - target->battleSpeed) >= BATTLE_FOLLOWUP_SPEED_THRESHOLD);
}

STATIC_DECLAR bool CheckDesperationOrder(void)
{
	gBattleTemporaryFlag.desperation_order = false;

	if (!gBattleActor.weapon)
		return false;

#if defined(SID_Desperation) && (COMMON_SKILL_VALID(SID_Desperation))
	if (BattleFastSkillTester(&gBattleActor, SID_Desperation)) {
		if ((gBattleActor.hpInitial * 2) < gBattleActor.unit.maxHP) {
			gBattleTemporaryFlag.desperation_order = true;
			RegisterBattleOrderSkill(SID_Desperation, BORDER_DESPERATION);
			return true;
		}
	}
#endif

#if defined(SID_CloseCombat) && (COMMON_SKILL_VALID(SID_CloseCombat))
	if (BattleFastSkillTester(&gBattleActor, SID_CloseCombat) && gBattleStats.range == 1) {
		gBattleTemporaryFlag.desperation_order = true;
		RegisterBattleOrderSkill(SID_CloseCombat, BORDER_DESPERATION);
		return true;
	}
#endif

#if defined(SID_Pursuer) && (COMMON_SKILL_VALID(SID_Pursuer))
	if (BattleFastSkillTester(&gBattleActor, SID_Pursuer)) {
		gBattleTemporaryFlag.desperation_order = true;
		RegisterBattleOrderSkill(SID_Pursuer, BORDER_DESPERATION);
		return true;
	}
#endif

#if defined(SID_YngviAscendant) && (COMMON_SKILL_VALID(SID_YngviAscendant))
	if (BattleFastSkillTester(&gBattleActor, SID_YngviAscendant)) {
		gBattleTemporaryFlag.desperation_order = true;
		RegisterBattleOrderSkill(SID_YngviAscendant, BORDER_DESPERATION);
		return true;
	}
#endif

	return false;
}

STATIC_DECLAR bool CheckVantageOrder(void)
{
	gBattleTemporaryFlag.vantage_order = false;

	/* Combat art will not allow vantage */
	if (COMBART_VALID(GetCombatArtInForce(&gBattleActor.unit)))
		return false;

	if (!gBattleTarget.weapon)
		return false;

#if defined(SID_Vantage) && (COMMON_SKILL_VALID(SID_Vantage))
	if (BattleFastSkillTester(&gBattleTarget, SID_Vantage)) {
		if ((gBattleTarget.hpInitial * 2) < gBattleTarget.unit.maxHP) {
			RegisterBattleOrderSkill(SID_Vantage, BORDER_VANTAGE);
			gBattleTemporaryFlag.vantage_order = true;
			return true;
		}
	}
#endif

#if defined(SID_GaleWings) && (COMMON_SKILL_VALID(SID_GaleWings))
	if (BattleFastSkillTester(&gBattleTarget, SID_GaleWings)) {
		if (gBattleTarget.hpInitial == gBattleTarget.unit.maxHP) {
			RegisterBattleOrderSkill(SID_GaleWings, BORDER_VANTAGE);
			gBattleTemporaryFlag.vantage_order = true;
			return true;
		}
	}
#endif

#if defined(SID_PridefulWarrior) && (COMMON_SKILL_VALID(SID_PridefulWarrior))
	if (BattleFastSkillTester(&gBattleActor, SID_PridefulWarrior)) {
		/* actor can enable the foe to attack first */
		gBattleTemporaryFlag.vantage_order = true;
		return true;
	}
#endif

	return false;
}

LYN_REPLACE_CHECK(BattleUnwind);
void BattleUnwind(void)
{
	int i, ret;
#ifdef CONFIG_USE_COMBO_ATTACK
	bool combo_atk_done = false;
#endif
	u8 round_mask = 0;
	const u8 *config;

	/* Identifier to record attack amount for skill anim triger */
	int actor_count = 0;
	int target_count = 0;

	ClearBattleHits();
	gBattleHitIterator->info |= BATTLE_HIT_INFO_BEGIN;

	/* fasten calc! */
	if (!(gBattleStats.config & BATTLE_CONFIG_REAL)) {
		gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
		return;
	}

	if (CheckDesperationOrder())
		round_mask |= UNWIND_DESPERA;

	if (CheckVantageOrder())
		round_mask |= UNWIND_VANTAGE;

	if (CheckCanTwiceAttackOrder(&gBattleActor, &gBattleTarget))
		round_mask |= UNWIND_DOUBLE_ACT;

	if (CheckCanTwiceAttackOrder(&gBattleTarget, &gBattleActor))
		round_mask |= UNWIND_DOUBLE_TAR;

	config = BattleUnwindConfig[round_mask];

	for (i = 0; i < 4; i++) {
		struct BattleHit *old = gBattleHitIterator;

		if (config[i] == ACT_ATTACK) {
#ifdef CONFIG_USE_COMBO_ATTACK
			/* Combo-attack first */
			if (!combo_atk_done) {
				combo_atk_done = true;
				ret = BattleComboGenerateHits();
				if (ret)
					break;

				/* Reload battle-hit */
				old = gBattleHitIterator;
				LTRACEF("Combo end at round %d", GetBattleHitRound(old));
			}
#endif
			ret = BattleGenerateRoundHits(&gBattleActor, &gBattleTarget);
			actor_count++;
		} else if (config[i] == TAR_ATTACK) {
			gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_RETALIATE;
			ret = BattleGenerateRoundHits(&gBattleTarget, &gBattleActor);
			target_count++;
		} else if (config[i] == NOP_ATTACK)
			break;

		/* Combat art first */
		if (i == 0) {
			int cid = GetCombatArtInForce(&gBattleActor.unit);

			if (COMBART_VALID(cid))
				RegisterEfxSkillCombatArt(GetBattleHitRound(old), cid);
		}

		if (i != 0 && config[i - 1] == config[i])
			gBattleHitIterator->attributes = BATTLE_HIT_ATTR_FOLLOWUP;

		/* Vantage */
		if (i == 0 && (round_mask & UNWIND_VANTAGE))
			if (gBattleTemporaryFlag.vantage_order)
				RegisterActorEfxSkill(GetBattleHitRound(old), BattleOrderSkills[BORDER_VANTAGE]);

		/* Desperation */
		if (i == 1 && (round_mask & UNWIND_DESPERA))
			if (config[0] == ACT_ATTACK && config[1] == ACT_ATTACK && config[2] == TAR_ATTACK)
				if (gBattleTemporaryFlag.desperation_order)
					RegisterActorEfxSkill(GetBattleHitRound(old), BattleOrderSkills[BORDER_DESPERATION]);

		/* Target double attack */
		if (target_count > 1 && config[i] == TAR_ATTACK)
			if (gBattleTemporaryFlag.tar_force_twice_order)
				RegisterActorEfxSkill(GetBattleHitRound(old), BattleOrderSkills[BORDER_TAR_TWICE]);

		/* Actor double attack */
		if (actor_count > 1 && config[i] == ACT_ATTACK)
			if (gBattleTemporaryFlag.act_force_twice_order)
				RegisterActorEfxSkill(GetBattleHitRound(old), BattleOrderSkills[BORDER_ACT_TWICE]);

		if (ret)
			break;
	}
	gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
}

LYN_REPLACE_CHECK(BattleGenerateRoundHits);
bool BattleGenerateRoundHits(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	int i, count;
	u32 attrs;

	if (!CheckCanContinueAttack(attacker))
		return FALSE;

	/* Clear the round related efx skill pool */
	ResetRoundEfxSkills();

	attrs = gBattleHitIterator->attributes;
	count = GetBattleUnitHitCount(attacker);

	LTRACEF("Battle rounds: %d", count);

	for (i = 0; i < count; ++i) {
		int efx_sid, round = GetBattleHitRound(gBattleHitIterator);

		gBattleHitIterator->attributes |= attrs;

		if (i != 0) {
			efx_sid = DequeueRoundEfxSkill();
			if (COMMON_SKILL_VALID(efx_sid)) {
				LTRACEF("Round skill %02x registered at round %d", efx_sid, round);
				RegisterActorEfxSkill(round, efx_sid);
			}
		}

		if (BattleGenerateHit(attacker, defender))
			return true;

		/* Hack here: we need avoid hit array overflow */
		if (CheckBattleHitOverflow()) {
			Error("Battle hit overflowed!");
			gBattleHitIterator = gBattleHitIterator - 1;
			gBattleHitIterator->info |= (BATTLE_HIT_INFO_FINISHES | BATTLE_HIT_INFO_END);
			return true;
		}

		/* I think this is a bug-fix for vanilla */
		if (!CheckCanContinueAttack(attacker))
			return false;
	}
	return false;
}

LYN_REPLACE_CHECK(BattleGetFollowUpOrder);
bool BattleGetFollowUpOrder(struct BattleUnit **outAttacker, struct BattleUnit **outDefender)
{
	if (CheckCanTwiceAttackOrder(&gBattleActor, &gBattleTarget)) {
		*outAttacker = &gBattleActor;
		*outDefender = &gBattleTarget;
		return true;
	} else if (CheckCanTwiceAttackOrder(&gBattleTarget, &gBattleActor)) {
		*outAttacker = &gBattleTarget;
		*outDefender = &gBattleActor;
		return true;
	}
	return false;
}

LYN_REPLACE_CHECK(GetBattleUnitHitCount);
int GetBattleUnitHitCount(struct BattleUnit *actor)
{
	FORCE_DECLARE struct BattleUnit *target = (actor == &gBattleActor)
										    ? &gBattleTarget
										    : &gBattleActor;
	int result = 1;

	if (BattleCheckBraveEffect(actor))
		result = result + 1;

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
	if (BattleFastSkillTester(actor, SID_RuinedBladePlus)) {
		EnqueueRoundEfxSkill(SID_RuinedBladePlus);
		result = result + SKILL_EFF2(SID_RuinedBladePlus);
	}
#endif

#if defined(SID_Astra) && (COMMON_SKILL_VALID(SID_Astra))
	if (actor == &gBattleActor && CheckBattleSkillActivate(actor, target, SID_Astra, actor->unit.spd)) {
		EnqueueRoundEfxSkill(SID_Astra);
		gBattleActorGlobalFlag.skill_activated_astra = true;
		result = result + SKILL_EFF0(SID_Astra);
	}
#endif

#if defined(SID_Adept) && (COMMON_SKILL_VALID(SID_Adept))
	if (BattleFastSkillTester(actor, SID_Adept) && actor->hpInitial == actor->unit.maxHP) {
		EnqueueRoundEfxSkill(SID_Adept);
		result = result + 1;
	}
#endif

#if defined(SID_ChargePlus) && (COMMON_SKILL_VALID(SID_ChargePlus))
		if (BattleFastSkillTester(actor, SID_ChargePlus)) {
			if (MovGetter(gActiveUnit) == gActionData.moveCount)
				result = result + 1;
		}
#endif

#if defined(SID_DoubleLion) && (COMMON_SKILL_VALID(SID_DoubleLion))
	if (actor == &gBattleActor && BattleFastSkillTester(actor, SID_DoubleLion) && actor->hpInitial == actor->unit.maxHP) {
		gBattleActorGlobalFlag.skill_activated_double_lion = true;
		EnqueueRoundEfxSkill(SID_DoubleLion);
		result = result + 1;
	}
#endif

	return result;
}
