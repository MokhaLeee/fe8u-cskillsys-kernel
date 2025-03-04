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
	int result;

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

	/**
	 * Step0: Roll critical and silencer attack
	 */
	gDmg.crit_atk = false;

#if (defined(SID_FatalTen) && (COMMON_SKILL_VALID(SID_FatalTen)))
	if (BattleFastSkillTester(attacker, SID_FatalTen)) {
		if (act_flags->round_cnt_hit >= SKILL_EFF0(SID_FatalTen)) {
			/* WON! */
			RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_FatalTen);

			gBattleHitIterator->attributes |= (BATTLE_HIT_ATTR_CRIT | BATTLE_HIT_ATTR_SILENCER);
			return BATTLE_MAX_DAMAGE;
		}
	}
#endif

	if (
#if defined(SID_Fortune) && (COMMON_SKILL_VALID(SID_Fortune))
		!BattleFastSkillTester(defender, SID_Fortune)
#else
		(1)
#endif
		&&
#if defined(SID_Foresight) && (COMMON_SKILL_VALID(SID_Foresight))
		!BattleFastSkillTester(defender, SID_Foresight)
#else
		(1)
#endif
	) {
		if (BattleRoll1RN(gBattleStats.critRate, false)) {
			gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
			gDmg.crit_atk = true;

			if (BattleRoll1RN(gBattleStats.silencerRate, false)) {
				/* Directly return on silencer attack to fasten calc */
				gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_SILENCER;
				return BATTLE_MAX_DAMAGE;
			}
		}
	}

	/**
	 * Step1: Calculate real damage
	 */
	gDmg.real_damage = CalcBattleRealDamage(attacker, defender);

	/**
	 * Step2: Calculate base damage (atk + gDmg.correction - def)
	 */
	gDmg.attack = gBattleStats.attack;
	gDmg.defense = gBattleStats.defense;
	gDmg.correction = 0;

	if (attacker == &gBattleActor) {
		/**
		 * I don't think it's a good idea to put calculation here.
		 *
		 * Also for the combat-art, it is better to judge on round count,
		 * so that it can only take effect on first strike.
		 *
		 * Later we may optimize it.
		 */
		switch (GetCombatArtInForce(&attacker->unit)) {
		case CID_Detonate:
			if (!(GetItemAttributes(attacker->weapon) & IA_UNBREAKABLE))
				gDmg.defense = 0;

			break;

		case CID_BloodTribute:
#if (defined(SID_COMBAT_BloodTribute) && (COMMON_SKILL_VALID(SID_COMBAT_BloodTribute)))
			if (gBattleStats.config & BATTLE_CONFIG_REAL) {
				int _hp_cost = perc_of(attacker->unit.curHP, SKILL_EFF0(SID_COMBAT_BloodTribute));
				int _dmg_ext = perc_of(_hp_cost, SKILL_EFF1(SID_COMBAT_BloodTribute));

				if (AddBattleHpCost(attacker, GetCurrentBattleHitRound(), _dmg_ext)) {
					RegisterActorEfxSkill(GetCurrentBattleHitRound(), SID_COMBAT_BloodTribute);
					gDmg.attack += _dmg_ext;
				}
			}
#endif
			break;

		case CID_CrimsonStrike:
#if (defined(SID_COMBAT_CrimsonStrike) && (COMMON_SKILL_VALID(SID_COMBAT_CrimsonStrike)))
			if (gBattleStats.config & BATTLE_CONFIG_REAL) {
				int _hp_cost = perc_of(attacker->unit.curHP, SKILL_EFF0(SID_COMBAT_CrimsonStrike));
				int _dmg_ext = perc_of(_hp_cost, SKILL_EFF1(SID_COMBAT_CrimsonStrike));

				if (AddBattleHpCost(attacker, GetCurrentBattleHitRound(), _dmg_ext)) {
					RegisterActorEfxSkill(GetCurrentBattleHitRound(), SID_COMBAT_CrimsonStrike);
					gDmg.attack += _dmg_ext;
				}
			}
#endif
			break;

		case CID_VitalReckoning:
#if (defined(SID_COMBAT_VitalReckoning) && (COMMON_SKILL_VALID(SID_COMBAT_VitalReckoning)))
			if (gBattleStats.config & BATTLE_CONFIG_REAL) {
				int _hp_cost = perc_of(attacker->unit.curHP, SKILL_EFF0(SID_COMBAT_VitalReckoning));
				int _dmg_ext = perc_of(_hp_cost, SKILL_EFF1(SID_COMBAT_VitalReckoning));

				if (AddBattleHpCost(attacker, GetCurrentBattleHitRound(), _dmg_ext)) {
					RegisterActorEfxSkill(GetCurrentBattleHitRound(), SID_COMBAT_VitalReckoning);
					gDmg.attack += _dmg_ext;
				}
			}
#endif
			break;

		default:
			break;
		}
	}

#if (defined(SID_Flare) && (COMMON_SKILL_VALID(SID_Flare)))
	if (CheckBattleSkillActivate(attacker, defender, SID_Flare, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Flare);
		gDmg.defense = gDmg.defense / 2;
	}
#endif

	if (IsMagicAttack(attacker)) {
#if (defined(SID_Corona) && (COMMON_SKILL_VALID(SID_Corona)))
		if (CheckBattleSkillActivate(attacker, defender, SID_Corona, attacker->unit.skl)) {
			RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corona);
			gDmg.defense = 0;
		}
#endif
	} else {
#if (defined(SID_Luna) && (COMMON_SKILL_VALID(SID_Luna)))
		if (CheckBattleSkillActivate(attacker, defender, SID_Luna, attacker->unit.skl)) {
			RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Luna);
			gDmg.defense = 0;
		}
#endif
	}

#if (defined(SID_Ignis) && (COMMON_SKILL_VALID(SID_Ignis)))
	if (CheckBattleSkillActivate(attacker, defender, SID_Ignis, attacker->unit.skl)) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Ignis);
		switch (attacker->weaponType) {
		case ITYPE_SWORD:
		case ITYPE_LANCE:
		case ITYPE_AXE:
		case ITYPE_BOW:
			gDmg.correction += attacker->unit.def / 2;
			break;

		case ITYPE_ANIMA:
		case ITYPE_LIGHT:
		case ITYPE_DARK:
			gDmg.correction += attacker->unit.res / 2;
			break;
		default:
			break;
		}
	}
#endif

	gBattleTemporaryFlag.skill_activated_aether = false;

#if (defined(SID_Aether) && (COMMON_SKILL_VALID(SID_Aether)))
	if (CheckBattleSkillActivate(attacker, defender, SID_Aether, attacker->unit.skl)) {
		gBattleTemporaryFlag.skill_activated_aether = true;
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aether);
		gDmg.correction += defender->battleDefense * 4 / 5;
	}
#endif

#if (defined(SID_LunarBrace) && (COMMON_SKILL_VALID(SID_LunarBrace)))
	if (BattleFastSkillTester(attacker, SID_LunarBrace)) {
		if (&gBattleActor == attacker) {
			RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LunarBrace);
			gDmg.correction += defender->battleDefense * 1 / 4;
		}
	}
#endif

#if (defined(SID_BloodSurge) && COMMON_SKILL_VALID(SID_BloodSurge))
	if (BattleFastSkillTester(attacker, SID_BloodSurge)) {
		int hp_cost = SKILL_EFF0(SID_BloodSurge);

		if (TryBattleHpCost(attacker, hp_cost)) {
			int perc = SKILL_EFF1(SID_BloodSurge);

			AddBattleHpCost(attacker, GetBattleHitRound(gBattleHitIterator), hp_cost);
			gDmg.correction += Div(attacker->unit.maxHP * perc, 100);

			RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_BloodSurge);
		}
	}
#endif

#if (defined(SID_BloodReaver) && COMMON_SKILL_VALID(SID_BloodReaver))
	if (BattleFastSkillTester(attacker, SID_BloodReaver)) {
		int hp_cost = SKILL_EFF0(SID_BloodReaver);

		if (TryBattleHpCost(attacker, hp_cost)) {
			int perc = SKILL_EFF1(SID_BloodReaver);

			AddBattleHpCost(attacker, GetBattleHitRound(gBattleHitIterator), hp_cost);
			gDmg.correction += Div(defender->unit.maxHP * perc, 100);

			RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_BloodReaver);
		}
	}
#endif

#if defined(SID_Glacies) && (COMMON_SKILL_VALID(SID_Glacies))
	if (CheckBattleSkillActivate(attacker, defender, SID_Glacies, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Glacies);
		gDmg.correction += attacker->unit.res;
	}
#endif

#if defined(SID_Vengeance) && (COMMON_SKILL_VALID(SID_Vengeance))
	if (CheckBattleSkillActivate(attacker, defender, SID_Vengeance, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Vengeance);
		gDmg.correction += (attacker->unit.maxHP - attacker->unit.curHP);
	}
#endif

#if (defined(SID_Spurn) && (COMMON_SKILL_VALID(SID_Spurn)))
		if (BattleFastSkillTester(attacker, SID_Spurn) && gDmg.crit_atk && (attacker->hpInitial * 4) < (attacker->unit.maxHP * 3))
			gDmg.correction += SKILL_EFF0(SID_Spurn);
#endif

#if (defined(SID_DragonWarth) && (COMMON_SKILL_VALID(SID_DragonWarth)))
	if (BattleFastSkillTester(attacker, SID_DragonWarth) && act_flags->round_cnt_hit == 1)
		gDmg.correction += gDmg.attack * SKILL_EFF0(SID_DragonWarth) / 100;
#endif

	/**
	 * Step2.1: fasten calculation!
	 */
	gDmg.damage_base = gDmg.attack + gDmg.correction - gDmg.defense;
	if (gDmg.damage_base <= 0)
		gDmg.damage_base = 0;

	if (gDmg.damage_base <= 0 && gDmg.real_damage <= 0) {
		/* If no damage taken, directly end the damage calculation */
		return 0;
	}

#if (defined(SID_GreatShield) && (COMMON_SKILL_VALID(SID_GreatShield)))
	if (CheckBattleSkillActivate(defender, attacker, SID_GreatShield, defender->unit.skl)) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_GreatShield);
		return 0;
	}
#endif

#if (defined(SID_TowerShieldPlus) && (COMMON_SKILL_VALID(SID_TowerShieldPlus)))
	if (BattleFastSkillTester(defender, SID_TowerShieldPlus)) {
		if (gBattleStats.range > 1) {
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_TowerShieldPlus);
			return 0;
		}
	}
#endif

#if (defined(SID_Dazzling) && (COMMON_SKILL_VALID(SID_Dazzling)))
	if (BattleFastSkillTester(defender, SID_Dazzling)) {
		if (gBattleStats.range >= 3) {
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Dazzling);
			return 0;
		}
	}
#endif
	if (IsMagicAttack(attacker)) {
#if (defined(SID_Aegis) && (COMMON_SKILL_VALID(SID_Aegis)))
		if (CheckBattleSkillActivate(defender, attacker, SID_Aegis, defender->unit.skl)) {
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aegis);
			return 0;
		}
#endif
	} else {
#if (defined(SID_Pavise) && (COMMON_SKILL_VALID(SID_Pavise)))
		if (CheckBattleSkillActivate(defender, attacker, SID_Pavise, defender->unit.skl)) {
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Pavise);
			return 0;
		}
#endif
	}

	/**
	 * Step3: Calculate damage increase amplifier (100% + increase%)
	 */
	gDmg.increase = 100;

#if defined(SID_DragonFang) && (COMMON_SKILL_VALID(SID_DragonFang))
	if (CheckBattleSkillActivate(attacker, defender, SID_DragonFang, attacker->unit.skl * 2)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DragonFang);
		gDmg.increase += SKILL_EFF0(SID_DragonFang);
	}
#endif

#if (defined(SID_Colossus) && (COMMON_SKILL_VALID(SID_Colossus)))
	if (CheckBattleSkillActivate(attacker, defender, SID_Colossus, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Colossus);
		gDmg.increase += 10 * SKILL_EFF0(SID_Colossus);
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
	}
#endif

#if defined(SID_Impale) && (COMMON_SKILL_VALID(SID_Impale))
	if (CheckBattleSkillActivate(attacker, defender, SID_Impale, attacker->unit.skl)) {
		RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Impale);
		gDmg.increase += 10 * SKILL_EFF0(SID_Impale);
		gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
	}
#endif

#if defined(SID_LimitBreak) && (COMMON_SKILL_VALID(SID_LimitBreak))
	if (BattleFastSkillTester(attacker, SID_LimitBreak)) {
		if (GetBattleGlobalFlags(attacker)->round_cnt_hit == SKILL_EFF0(SID_LimitBreak)) {
			// RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LimitBreak);
			gDmg.increase += 10 * SKILL_EFF1(SID_LimitBreak);
			gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
		}
	}
#endif

#if defined(SID_SolarPower) && (COMMON_SKILL_VALID(SID_SolarPower))
	if (BattleFastSkillTester(attacker, SID_SolarPower))
		if (gPlaySt.chapterWeatherId == WEATHER_FLAMES && IsMagicAttack(attacker))
			gDmg.increase += SKILL_EFF0(SID_SolarPower);
#endif

#if defined(SID_UnstoppableForce) && (COMMON_SKILL_VALID(SID_UnstoppableForce))
	if (BattleFastSkillTester(attacker, SID_UnstoppableForce))
		gDmg.increase += SKILL_EFF0(SID_UnstoppableForce);
#endif

	if (gBattleTemporaryFlag.skill_activated_sure_shoot)
		gDmg.increase += 50;

	/**
	 * Step4: Calculate critial damage amplifier (200%  + crit_correction%)
	 */
	gDmg.crit_correction = 100;
	if (gDmg.crit_atk) {
		gDmg.crit_correction = gpKernelBattleDesignerConfig->crit_correction;

#if defined(SID_InfinityEdge) && (COMMON_SKILL_VALID(SID_InfinityEdge))
		if (BattleFastSkillTester(attacker, SID_InfinityEdge))
			gDmg.crit_correction += SKILL_EFF0(SID_InfinityEdge);
#endif

#if (defined(SID_Gambit) && (COMMON_SKILL_VALID(SID_Gambit)))
		if (BattleFastSkillTester(defender, SID_Gambit) && gBattleStats.range == 1)
			gDmg.crit_correction -= SKILL_EFF0(SID_Gambit);
#endif

#if (defined(SID_MagicGambit) && (COMMON_SKILL_VALID(SID_MagicGambit)))
		if (BattleFastSkillTester(defender, SID_MagicGambit) && gBattleStats.range > 1)
			gDmg.crit_correction -= SKILL_EFF0(SID_MagicGambit);
#endif
	}

	/**
	 * Step5: Calculate damage decrease amplifier (100% + decrease%)
	 */
	gDmg.decrease = 0x100;

#if defined(SID_Astra) && (COMMON_SKILL_VALID(SID_Astra))
	if (attacker == &gBattleActor && BattleFastSkillTester(attacker, SID_Astra) && gBattleActorGlobalFlag.skill_activated_astra)
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_Astra));
#endif

#if (defined(SID_DragonSkin) && (COMMON_SKILL_VALID(SID_DragonSkin)))
	if (BattleFastSkillTester(defender, SID_DragonSkin)) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DragonSkin);
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_DragonSkin));
	}
#endif

#if (defined(SID_Multiscale) && (COMMON_SKILL_VALID(SID_Multiscale)))
	if (BattleFastSkillTester(defender, SID_Multiscale)) {
		if (defender->unit.curHP == defender->unit.maxHP) {
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Multiscale);
			gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_Multiscale));
		}
	}
#endif

#if defined(SID_Expertise) && (COMMON_SKILL_VALID(SID_Expertise))
	if (BattleFastSkillTester(defender, SID_Expertise) && gDmg.crit_atk)
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_Expertise));
#endif

#if (defined(SID_KeenFighter) && (COMMON_SKILL_VALID(SID_KeenFighter)))
	if (BattleFastSkillTester(defender, SID_KeenFighter) && CheckCanTwiceAttackOrder(attacker, defender)) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_KeenFighter);
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_KeenFighter));
	}
#endif

#if defined(SID_GuardBearing) && (COMMON_SKILL_VALID(SID_GuardBearing))
	if (BattleFastSkillTester(defender, SID_GuardBearing)) {
		if (!AreUnitsAllied(defender->unit.index, gPlaySt.faction) &&
			act_flags->round_cnt_hit == 1 &&
			!CheckBitUES(&defender->unit, UES_BIT_GUARDBEAR_SKILL_USED)) {
			RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_GuardBearing);
			SetBitUES(&defender->unit, UES_BIT_GUARDBEAR_SKILL_USED);
			gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_GuardBearing));
		}
	}
#endif

#if (defined(SID_BeastAssault) && (COMMON_SKILL_VALID(SID_BeastAssault)))
	if (BattleFastSkillTester(defender, SID_BeastAssault))
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_BeastAssault));
#endif

#if (defined(SID_Spurn) && (COMMON_SKILL_VALID(SID_Spurn)))
	if (BattleFastSkillTester(defender, SID_Spurn)) {
		int _diff = defender->battleSpeed - attacker->battleSpeed;

		LIMIT_AREA(_diff, 0, 10);
		gDmg.decrease += DAMAGE_DECREASE(_diff * SKILL_EFF1(SID_Spurn));
	}
#endif

#if (defined(SID_Bushido) && (COMMON_SKILL_VALID(SID_Bushido)))
	if (BattleFastSkillTester(defender, SID_Bushido)) {
		int _diff = defender->battleSpeed - attacker->battleSpeed;

		LIMIT_AREA(_diff, 0, 10);
		gDmg.decrease += DAMAGE_DECREASE(_diff * SKILL_EFF0(SID_Bushido));
	}
#endif

#if (defined(SID_DragonWall) && (COMMON_SKILL_VALID(SID_DragonWall)))
	if (BattleFastSkillTester(defender, SID_DragonWall)) {
		int _diff = defender->unit.res - attacker->unit.res;

		LIMIT_AREA(_diff, 0, 10);
		gDmg.decrease += DAMAGE_DECREASE(_diff * SKILL_EFF0(SID_DragonWall));
	}
#endif

#if (defined(SID_BlueLionRule) && (COMMON_SKILL_VALID(SID_BlueLionRule)))
	if (BattleFastSkillTester(defender, SID_BlueLionRule)) {
		int _diff = defender->unit.def - attacker->unit.def;

		LIMIT_AREA(_diff, 0, 10);
		gDmg.decrease += DAMAGE_DECREASE(_diff * SKILL_EFF0(SID_BlueLionRule));
	}
#endif

#if (defined(SID_CounterRoar) && (COMMON_SKILL_VALID(SID_CounterRoar)))
	if (BattleFastSkillTester(defender, SID_CounterRoar)) {
		if (act_flags->round_cnt_hit == 1)
			gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_CounterRoar));
		else
			gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_CounterRoar));
	}
#endif

#if (defined(SID_DragonWarth) && (COMMON_SKILL_VALID(SID_DragonWarth)))
	if (BattleFastSkillTester(defender, SID_DragonWarth) && tar_flags->round_cnt_hit == 1)
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_DragonWarth));
#endif

#if (defined(SID_CrusaderWard) && (COMMON_SKILL_VALID(SID_CrusaderWard)))
	if (BattleFastSkillTester(defender, SID_CrusaderWard) &&
		tar_flags->round_cnt_hit > 1 &&
		gBattleStats.range > 1)
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_CrusaderWard));
#endif

#if (defined(SID_ShieldPulse) && (COMMON_SKILL_VALID(SID_ShieldPulse)))
	if (BattleFastSkillTester(defender, SID_ShieldPulse) && gDmg.crit_atk) {
		RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_ShieldPulse);
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_ShieldPulse));
	}
#endif

#if defined(SID_ImmovableObject) && (COMMON_SKILL_VALID(SID_ImmovableObject))
	if (BattleFastSkillTester(attacker, SID_ImmovableObject))
		gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_ImmovableObject));
#endif

	/**
	 * Boolean check to prevent Barricade+ and Barricade from stacking
	 */
	barricadePlus_activated = false;

#if (defined(SID_BarricadePlus) && (COMMON_SKILL_VALID(SID_BarricadePlus)))
	if (BattleFastSkillTester(defender, SID_BarricadePlus)) {
		if (act_flags->round_cnt_hit > 2) {
			int _i, _reduction = SKILL_EFF0(SID_BarricadePlus);
			int _base = _reduction;

			barricadePlus_activated = true;

			for (_i = 0; _i < act_flags->round_cnt_hit - 2; _i++) {
				_base = _base / 2;
				_reduction += _base;
			}
			gDmg.decrease += DAMAGE_DECREASE(_reduction);
		}
	}
#endif

#if (defined(SID_Barricade) && (COMMON_SKILL_VALID(SID_Barricade)))
	if (BattleFastSkillTester(defender, SID_Barricade) && !barricadePlus_activated) {
		if (defender->unit.curHP < defender->hpInitial)
			gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF0(SID_Barricade));
	}
#endif

	/**
	 * External calc!
	 */
	for (it = gpBattleDamageCalcFuncs; *it; it++)
		(*it)(attacker, defender);

	/**
	 * Step6: Calculate result
	 */
	{
		u32 dividend, divisor, quotient;

		dividend = gDmg.damage_base * gDmg.increase * gDmg.crit_correction * 0x100;
		divisor  = 100 * 100 * gDmg.decrease;

		// quotient = k_udiv(dividend, divisor);
		quotient = DIV_ROUND_CLOSEST(dividend, divisor);

		LTRACEF("dividend=%ld, divisor=%ld, quotient=%ld", dividend, divisor, quotient);
		result = quotient;
	}

	if (result == 0 && gDmg.damage_base > 0)
		result = 1; // at least 1 damage left.

	result += gDmg.real_damage;

	LTRACEF("[round %d] dmg=%d: base=%d (atk=%d, def=%d, cor=%d), inc=%d, crt=%d, dec=%d, real=%d",
					GetBattleHitRound(gBattleHitIterator), result, gDmg.damage_base,
					gDmg.attack, gDmg.defense, gDmg.correction, gDmg.increase, gDmg.crit_correction, gDmg.decrease, gDmg.real_damage);

	if (result > BATTLE_MAX_DAMAGE)
		result = BATTLE_MAX_DAMAGE;

	gDmg.result = result;
	return result;
}
