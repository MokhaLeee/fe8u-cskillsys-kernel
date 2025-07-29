#include "common-chax.h"
#include "strmag.h"
#include "unit-expa.h"
#include "battle-system.h"
#include "skill-system.h"
#include "constants/skills.h"

void BattleDamageCalc_AttackerSkills(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	_maybe_unused int tmp0, tmp1;
	int _skill_list_cnt;
	struct SkillList *list;

	gBattleTemporaryFlag.skill_activated_aether = false;

	/**
	 * Skip arena judgement
	 */
	if (gBattleStats.config & BATTLE_CONFIG_ARENA)
		return;

	list = GetUnitSkillList(&attacker->unit);
	for (_skill_list_cnt = 0; _skill_list_cnt < list->amt; _skill_list_cnt++) {
		switch (list->sid[_skill_list_cnt]) {
#if (defined(SID_FatalTen) && (COMMON_SKILL_VALID(SID_FatalTen)))
		case SID_FatalTen:
			if (GetBattleGlobalFlags(attacker)->round_cnt_hit >= SKILL_EFF0(SID_FatalTen)) {
				/* WON! */
				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_FatalTen);

				gBattleHitIterator->attributes |= (BATTLE_HIT_ATTR_CRIT | BATTLE_HIT_ATTR_SILENCER);
				gBattleStats.attack = BATTLE_MAX_DAMAGE;
			}
			break;
#endif

#if (defined(SID_Flare) && (COMMON_SKILL_VALID(SID_Flare)))
		case SID_Flare:
			if (CheckBattleSkillActivate(attacker, defender, SID_Flare, attacker->unit.skl)) {
				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Flare);
				gBattleStats.defense = gBattleStats.defense / 2;
			}
			break;
#endif

#if (defined(SID_Corona) && (COMMON_SKILL_VALID(SID_Corona)))
		case SID_Corona:
			if (IsMagicAttack(attacker) && CheckBattleSkillActivate(attacker, defender, SID_Corona, attacker->unit.skl)) {
				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corona);
				gBattleStats.defense = 0;
			}
			break;
#endif

#if (defined(SID_Luna) && (COMMON_SKILL_VALID(SID_Luna)))
		case SID_Luna:
			if (!IsMagicAttack(attacker) && CheckBattleSkillActivate(attacker, defender, SID_Luna, attacker->unit.skl)) {
				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Luna);
				gBattleStats.defense = 0;
			}
			break;
#endif

#if (defined(SID_Ignis) && (COMMON_SKILL_VALID(SID_Ignis)))
		case SID_Ignis:
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
			break;
#endif

#if (defined(SID_Aether) && (COMMON_SKILL_VALID(SID_Aether)))
		case SID_Aether:
			if (CheckBattleSkillActivate(attacker, defender, SID_Aether, attacker->unit.skl)) {
				gBattleTemporaryFlag.skill_activated_aether = true;
				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aether);
				gDmg.correction += perc_of(defender->battleDefense, SKILL_EFF0(SID_Aether));
			}
			break;
#endif

#if (defined(SID_BloodSurge) && COMMON_SKILL_VALID(SID_BloodSurge))
		case SID_BloodSurge:
			if (TryBattleHpCost(attacker, SKILL_EFF0(SID_BloodSurge))) {
				int perc = SKILL_EFF1(SID_BloodSurge);

				AddBattleHpCost(attacker, GetBattleHitRound(gBattleHitIterator), SKILL_EFF0(SID_BloodSurge));
				gDmg.correction += Div(attacker->unit.maxHP * perc, 100);

				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_BloodSurge);
			}
			break;
#endif

#if (defined(SID_BloodReaver) && COMMON_SKILL_VALID(SID_BloodReaver))
		case SID_BloodReaver:
			if (TryBattleHpCost(attacker, SKILL_EFF0(SID_BloodReaver))) {
				int perc = SKILL_EFF1(SID_BloodReaver);

				AddBattleHpCost(attacker, GetBattleHitRound(gBattleHitIterator), SKILL_EFF0(SID_BloodReaver));
				gDmg.correction += Div(defender->unit.maxHP * perc, 100);

				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_BloodReaver);
			}
			break;
#endif

#if defined(SID_Glacies) && (COMMON_SKILL_VALID(SID_Glacies))
		case SID_Glacies:
			if (CheckBattleSkillActivate(attacker, defender, SID_Glacies, attacker->unit.skl)) {
				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Glacies);
				gDmg.correction += attacker->unit.res;
			}
			break;
#endif

#if (defined(SID_Spurn) && (COMMON_SKILL_VALID(SID_Spurn)))
		case SID_Spurn:
			if (gDmg.crit_atk && (attacker->hpInitial * 4) < (attacker->unit.maxHP * 3))
				gDmg.correction += SKILL_EFF0(SID_Spurn);

			break;
#endif

#if (defined(SID_DragonWarth) && (COMMON_SKILL_VALID(SID_DragonWarth)))
		case SID_DragonWarth:
			if (GetBattleGlobalFlags(attacker)->round_cnt_hit == 1)
				gDmg.correction += perc_of(BattleUnitOriginalStatus(attacker)->atk, SKILL_EFF0(SID_DragonWarth));

			break;
#endif

#if defined(SID_DragonFang) && (COMMON_SKILL_VALID(SID_DragonFang))
		case SID_DragonFang:
			if (CheckBattleSkillActivate(attacker, defender, SID_DragonFang, attacker->unit.skl * 2)) {
				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DragonFang);
				gDmg.increase += SKILL_EFF0(SID_DragonFang);
			}
			break;
#endif

#if (defined(SID_Colossus) && (COMMON_SKILL_VALID(SID_Colossus)))
		case SID_Colossus:
			if (CheckBattleSkillActivate(attacker, defender, SID_Colossus, attacker->unit.skl)) {
				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Colossus);
				gDmg.increase += 10 * SKILL_EFF0(SID_Colossus);
				gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
			}
			break;
#endif

#if defined(SID_Impale) && (COMMON_SKILL_VALID(SID_Impale))
		case SID_Impale:
			if (CheckBattleSkillActivate(attacker, defender, SID_Impale, attacker->unit.skl)) {
				RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Impale);
				gDmg.increase += 10 * SKILL_EFF0(SID_Impale);
				gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
			}
			break;
#endif

#if defined(SID_LimitBreak) && (COMMON_SKILL_VALID(SID_LimitBreak))
		case SID_LimitBreak:
			if (GetBattleGlobalFlags(attacker)->round_cnt_hit == SKILL_EFF0(SID_LimitBreak)) {
				// RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LimitBreak);
				gDmg.increase += 10 * SKILL_EFF1(SID_LimitBreak);
				gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
			}
			break;
#endif

#if (defined(SID_SureShot) && (COMMON_SKILL_VALID(SID_SureShot)))
		case SID_SureShot:
			if (gBattleTemporaryFlag.skill_activated_sure_shoot)
				gDmg.increase += 50;

			break;
#endif

#if defined(SID_InfinityEdge) && (COMMON_SKILL_VALID(SID_InfinityEdge))
		case SID_InfinityEdge:
			if (gDmg.crit_atk)
				gDmg.crit_correction += SKILL_EFF0(SID_InfinityEdge);

			break;
#endif

#if defined(SID_Astra) && (COMMON_SKILL_VALID(SID_Astra))
		case SID_Astra:
			if (attacker == &gBattleActor && gBattleActorGlobalFlag.skill_activated_astra)
				gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_Astra));

			break;
#endif

#if defined(SID_SteadyBrawler) && (COMMON_SKILL_VALID(SID_SteadyBrawler))
		case SID_SteadyBrawler:
			if (attacker == &gBattleActor) {
				if (gBattleFlagExt.round_mask & UNWIND_DOUBLE_ACT)
					gDmg.increase += SKILL_EFF0(SID_SteadyBrawler);
				else
					gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_SteadyBrawler));
			} else {
				if (gBattleFlagExt.round_mask & UNWIND_DOUBLE_TAR)
					gDmg.increase += SKILL_EFF0(SID_SteadyBrawler);
				else
					gDmg.decrease += DAMAGE_DECREASE(SKILL_EFF1(SID_SteadyBrawler));
			}
			break;
#endif

		case MAX_SKILL_NUM:
			Fatal("ENOSUPP");
			break;

		default:
			break;
		}
	}
}
