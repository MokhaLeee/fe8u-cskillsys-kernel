#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "kernel-lib.h"
#include "strmag.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

void PreBattleCalcWeaponTriangle(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	const struct WeaponTriangleConf *it;
	const struct WeaponTriangleRule *vanilla_it;
	const struct WeaponTriangleItemConf *item_it;
	bool invert = false;

	bool poise_self = false;
	bool poise_foo  = false;

	int ui, atk, def, hit, avo, crt, sil;

	ui  = 0;
	atk = 0;
	def = 0;
	hit = 0;
	avo = 0;
	crt = 0;
	sil = 0;

#if (defined(SID_Nonconforming) && (COMMON_SKILL_VALID(SID_Nonconforming)))
	if (BattleFastSkillTester(attacker, SID_Nonconforming))
		invert = !invert;
	if (BattleFastSkillTester(defender, SID_Nonconforming))
		invert = !invert;
#endif

#if (defined(SID_Poise) && (COMMON_SKILL_VALID(SID_Poise)))
	if (BattleFastSkillTester(attacker, SID_Poise))
		poise_self = true;
	if (BattleFastSkillTester(defender, SID_Poise))
		poise_foo  = true;
#endif

	item_it = &gpWeaponTriangleItemConf[ITEM_INDEX(attacker->weaponBefore)];
	if (item_it->valid && item_it->wtype == defender->weaponType) {
		if ((item_it->is_buff && !poise_foo) || (!item_it->is_buff && !poise_self)) {
			atk += item_it->battle_status.atk;
			def += item_it->battle_status.def;
			hit += item_it->battle_status.hit;
			avo += item_it->battle_status.avo;
			crt += item_it->battle_status.crit;
			sil += item_it->battle_status.silencer;
		}
	} else {
		/* vanilla */
		for (vanilla_it = sWeaponTriangleRules; vanilla_it->attackerWeaponType >= 0; ++vanilla_it) {
			if ((attacker->weaponType == vanilla_it->attackerWeaponType) && (defender->weaponType == vanilla_it->defenderWeaponType)) {
				if ((vanilla_it->atkBonus > 0 && !poise_foo) || (!(vanilla_it->atkBonus > 0) && !poise_self)) {
					atk += vanilla_it->atkBonus;
					hit += vanilla_it->hitBonus;
				}
				break;
			}
		}
	}

	for (it = gpWeaponTriangleConfs; it->wtype_a != it->wtype_b; it++) {
		if (it->wtype_a == attacker->weaponType && it->wtype_b == defender->weaponType) {
			if (it->sid == 0 || BattleFastSkillTester(attacker, it->sid)) {
				if ((item_it->is_buff && !poise_foo) || (!item_it->is_buff && !poise_self)) {
					atk += it->bonus_atk;
					def += it->bonus_def;
					hit += it->bonus_hit;
					avo += it->bonus_avoid;
					crt += it->bonus_crit;
					sil += it->bonus_silencer;
				}
			}
			break;
		}
	}

	if
	(
#if (defined(SID_TriangleAdept) && (COMMON_SKILL_VALID(SID_TriangleAdept)))
		BattleFastSkillTester(attacker, SID_TriangleAdept)
#else
		0
#endif
		||
#if (defined(SID_TriangleAdeptPlus) && (COMMON_SKILL_VALID(SID_TriangleAdeptPlus)))
		BattleFastSkillTester(attacker, SID_TriangleAdeptPlus) || BattleFastSkillTester(defender, SID_TriangleAdeptPlus)
#else
		0
#endif
	) {
		atk *= 2;
		def *= 2;
		hit *= 2;
		avo *= 2;
		crt *= 2;
		sil *= 2;
	}

	ui = (atk + def) * 10 + hit + avo + crt + sil;

	ui = ui / 8;
	if (ui > 63)
		ui = 63;

	if (!invert) {
		attacker->battleAttack       += atk;
		attacker->battleDefense      += def;
		attacker->battleHitRate      += hit;
		attacker->battleAvoidRate    += avo;
		attacker->battleCritRate     += crt;
		attacker->battleSilencerRate += sil;

		attacker->wTriangleHitBonus  += ui;
		attacker->wTriangleDmgBonus  += ui;
		defender->wTriangleHitBonus  -= ui;
		defender->wTriangleDmgBonus  -= ui;
	} else {
		attacker->battleAttack       -= atk;
		attacker->battleDefense      -= def;
		attacker->battleHitRate      -= hit;
		attacker->battleAvoidRate    -= avo;
		attacker->battleCritRate     -= crt;
		attacker->battleSilencerRate -= sil;

		attacker->wTriangleHitBonus  -= ui;
		attacker->wTriangleDmgBonus  -= ui;
		defender->wTriangleHitBonus  += ui;
		defender->wTriangleDmgBonus  += ui;
	}
}

#if 0
/**
 * On consideration of FEB WTA config,
 * Here we should not use LynJump as this may use 0x10 length
 **/
LYN_UNUSED_REPLACE_CHECK(BattleApplyWeaponTriangleEffect);
void BattleApplyWeaponTriangleEffect(struct BattleUnit *attacker, struct BattleUnit *defender)
#else
void _BattleApplyWeaponTriangleEffect(struct BattleUnit *attacker, struct BattleUnit *defender)
#endif
{
	/*
	 * Idea:
	 * Since vanilla WTA bonus can only get hit & dmg bonus.
	 * So here we null the vanilla WTA bonus
	 * But just calculate in Pre-Battle calc (PreBattleCalcWeaponTriangle)
	 * now WTA-bonus in BattleUnit struct is just for BkSel UI.
	 */
	return;
}
