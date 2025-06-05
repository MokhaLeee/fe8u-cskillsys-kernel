#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "kernel-lib.h"
#include "strmag.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0
#define ENABLE_ALL_WTA_HANDLER 1

STATIC_DECLAR bool WtaHandler_Skill(struct BattleUnit *attacker, struct BattleUnit *defender, struct WtaStatus *status)
{
	const struct WeaponTriangleConf *it;

	for (it = gpWeaponTriangleConfs; it->wtype_a != it->wtype_b; it++) {
		if (it->wtype_a == attacker->weaponType && it->wtype_b == defender->weaponType) {
			if (it->sid == 0 || BattleFastSkillTester(attacker, it->sid)) {
				if (it->is_buff) {
					status->bonus.atk += it->bonus_atk;
					status->bonus.def += it->bonus_def;
					status->bonus.hit += it->bonus_hit;
					status->bonus.avo += it->bonus_avoid;
					status->bonus.crt += it->bonus_crit;
					status->bonus.sil += it->bonus_silencer;
				} else {
					status->minus.atk += it->bonus_atk;
					status->minus.def += it->bonus_def;
					status->minus.hit += it->bonus_hit;
					status->minus.avo += it->bonus_avoid;
					status->minus.crt += it->bonus_crit;
					status->minus.sil += it->bonus_silencer;
				}
				return true;
			}
		}
	}
	return false;
}

STATIC_DECLAR bool WtaHandler_Weapon(struct BattleUnit *attacker, struct BattleUnit *defender, struct WtaStatus *status)
{
	const struct WeaponTriangleItemConf *it;

	it = &gpWeaponTriangleItemConf[ITEM_INDEX(attacker->weaponBefore)];
	if (it->valid && it->wtype == defender->weaponType) {
		if (it->is_buff) {
			status->bonus.atk += it->battle_status.atk;
			status->bonus.def += it->battle_status.def;
			status->bonus.hit += it->battle_status.hit;
			status->bonus.avo += it->battle_status.avo;
			status->bonus.crt += it->battle_status.crit;
			status->bonus.sil += it->battle_status.silencer;
		} else {
			status->minus.atk += it->battle_status.atk;
			status->minus.def += it->battle_status.def;
			status->minus.hit += it->battle_status.hit;
			status->minus.avo += it->battle_status.avo;
			status->minus.crt += it->battle_status.crit;
			status->minus.sil += it->battle_status.silencer;
		}
		return true;
	}
	return false;
}

STATIC_DECLAR bool WtaHandler_Vanilla(struct BattleUnit *attacker, struct BattleUnit *defender, struct WtaStatus *status)
{
	const struct WeaponTriangleRule *it;

	for (it = pr_WeaponTriangleRules; it->attackerWeaponType >= 0; ++it) {
		if ((attacker->weaponType == it->attackerWeaponType) && (defender->weaponType == it->defenderWeaponType)) {
			if (it->atkBonus > 0) {
				status->bonus.atk += it->atkBonus;
				status->bonus.hit += it->hitBonus;
			} else {
				status->minus.atk += it->atkBonus;
				status->minus.hit += it->hitBonus;
			}
			return true;
		}
	}
	return false;
}

STATIC_DECLAR void WtaHandler(void)
{
	_maybe_unused bool handled = false;
	struct BattleUnit *attacker = &gBattleActor;
	struct BattleUnit *defender = &gBattleTarget;

	/**
	 * Attacker
	 */
	handled = WtaHandler_Skill(attacker, defender, &gWtaStatus_act);
	if (!handled || ENABLE_ALL_WTA_HANDLER)
		handled = WtaHandler_Weapon(attacker, defender, &gWtaStatus_act);
	if (!handled || ENABLE_ALL_WTA_HANDLER)
		handled = WtaHandler_Vanilla(attacker, defender, &gWtaStatus_act);

	/**
	 * Defender
	 */
	handled = WtaHandler_Skill(defender, attacker, &gWtaStatus_tar);
	if (!handled || ENABLE_ALL_WTA_HANDLER)
		handled = WtaHandler_Weapon(defender, attacker, &gWtaStatus_tar);
	if (!handled || ENABLE_ALL_WTA_HANDLER)
		handled = WtaHandler_Vanilla(defender, attacker, &gWtaStatus_tar);
}

void PreBattleGenerate_SetupWtaStatus(void)
{
	bool invert = false;

	LTRACEF("sizeof WTA_status: %d", sizeof(struct WtaStatus));
	Assert(sizeof(struct WtaStatus) <= 0x38);

	memset(&gWtaStatus_act, 0, sizeof(gWtaStatus_act));
	memset(&gWtaStatus_tar, 0, sizeof(gWtaStatus_tar));

#if (defined(SID_Poise) && (COMMON_SKILL_VALID(SID_Poise)))
	if (BattleFastSkillTester(&gBattleActor, SID_Poise)) {
		gWtaStatus_act.minus.poise_eff = true;
		gWtaStatus_tar.bonus.poise_eff = true;
	}

	if (BattleFastSkillTester(&gBattleTarget, SID_Poise)) {
		gWtaStatus_act.bonus.poise_eff = true;
		gWtaStatus_tar.minus.poise_eff = true;
	}
#endif

#if (defined(SID_Nonconforming) && (COMMON_SKILL_VALID(SID_Nonconforming)))
	if (BattleFastSkillTester(&gBattleActor, SID_Nonconforming))
		invert = !invert;

	if (BattleFastSkillTester(&gBattleTarget, SID_Nonconforming))
		invert = !invert;
#endif

	gWtaStatus_act.invert_eff = invert;
	gWtaStatus_tar.invert_eff = invert;

#if (defined(SID_TriangleAdept) && (COMMON_SKILL_VALID(SID_TriangleAdept)))
	if (BattleFastSkillTester(&gBattleActor, SID_TriangleAdept))
		gWtaStatus_act.amplifier_eff = true;

	if (BattleFastSkillTester(&gBattleTarget, SID_TriangleAdept))
		gWtaStatus_tar.amplifier_eff = true;
#endif

#if (defined(SID_TriangleAdeptPlus) && (COMMON_SKILL_VALID(SID_TriangleAdeptPlus)))
	if (BattleFastSkillTester(&gBattleActor, SID_TriangleAdeptPlus)) {
		gWtaStatus_act.amplifier_eff = true;
		gWtaStatus_tar.amplifier_eff = true;
	}

	if (BattleFastSkillTester(&gBattleTarget, SID_TriangleAdeptPlus)) {
		gWtaStatus_act.amplifier_eff = true;
		gWtaStatus_tar.amplifier_eff = true;
	}
#endif

	WtaHandler();
}

void PreBattleCalcWeaponTriangle(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	int ui1, ui2, ui;

	struct WtaStatus *status = (attacker == &gBattleActor) ? &gWtaStatus_act : &gWtaStatus_tar;

	if (status->bonus.poise_eff == true) {
		status->bonus.atk = 0;
		status->bonus.def = 0;
		status->bonus.hit = 0;
		status->bonus.avo = 0;
		status->bonus.crt = 0;
		status->bonus.sil = 0;
	}

	if (status->minus.poise_eff == true) {
		status->minus.atk = 0;
		status->minus.def = 0;
		status->minus.hit = 0;
		status->minus.avo = 0;
		status->minus.crt = 0;
		status->minus.sil = 0;
	}

	if (status->amplifier_eff) {
		status->bonus.atk *= 2;
		status->bonus.def *= 2;
		status->bonus.hit *= 2;
		status->bonus.avo *= 2;
		status->bonus.crt *= 2;
		status->bonus.sil *= 2;

		status->minus.atk *= 2;
		status->minus.def *= 2;
		status->minus.hit *= 2;
		status->minus.avo *= 2;
		status->minus.crt *= 2;
		status->minus.sil *= 2;
	}

	if (status->invert_eff) {
		status->bonus.atk = -status->bonus.atk;
		status->bonus.def = -status->bonus.def;
		status->bonus.hit = -status->bonus.hit;
		status->bonus.avo = -status->bonus.avo;
		status->bonus.crt = -status->bonus.crt;
		status->bonus.sil = -status->bonus.sil;

		status->minus.atk = -status->minus.atk;
		status->minus.def = -status->minus.def;
		status->minus.hit = -status->minus.hit;
		status->minus.avo = -status->minus.avo;
		status->minus.crt = -status->minus.crt;
		status->minus.sil = -status->minus.sil;
	}

	ui1 = (status->bonus.atk + status->bonus.def) * 10
		+ status->bonus.hit + status->bonus.avo + status->bonus.crt + status->bonus.sil;
	ui2 = (status->minus.atk + status->minus.def) * 10
		+ status->minus.hit + status->minus.avo + status->minus.crt + status->minus.sil;

	ui = (ui1 + ui2) / 8;

	LTRACEF("[actor=%d] ui=%d invert=%d", attacker == &gBattleActor, ui, status->invert_eff);
	LTRACEF("bonus: atk=%d, def=%d, hit=%d, avo=%d, crt=%d, sil=%d",
		status->bonus.atk, status->bonus.def, status->bonus.hit, status->bonus.avo, status->bonus.crt, status->bonus.sil);
	LTRACEF("minus: atk=%d, def=%d, hit=%d, avo=%d, crt=%d, sil=%d",
		status->minus.atk, status->minus.def, status->minus.hit, status->minus.avo, status->minus.crt, status->minus.sil);

	attacker->battleAttack       += status->bonus.atk + status->minus.atk;
	attacker->battleDefense      += status->bonus.def + status->minus.def;
	attacker->battleHitRate      += status->bonus.hit + status->minus.hit;
	attacker->battleAvoidRate    += status->bonus.avo + status->minus.avo;
	attacker->battleCritRate     += status->bonus.crt + status->minus.crt;
	attacker->battleSilencerRate += status->bonus.sil + status->minus.sil;

	attacker->wTriangleHitBonus  += ui;
	attacker->wTriangleDmgBonus  += ui;
	defender->wTriangleHitBonus  -= ui;
	defender->wTriangleDmgBonus  -= ui;
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
