#include "common-chax.h"
#include "help-box.h"
#include "wrank-bonus.h"
#include "constants/texts.h"

#define LOCAL_TRACE 1

const struct WrankBonusConfEnt *GetWrankBonusConf(struct Unit *unit, int wtype, int wrank)
{
	const struct WrankBonusConfEnt *it;

	for (it = gpWrankBonusConf; it->wrank != WPN_LEVEL_0; it++)
		if (it->wtype == wtype && it->wrank == wrank)
			return it;

	return NULL;
}

int GetWtypeFromRTextMsg(u16 msg)
{
	const struct WrankRtextConfEnt *it;

	for (it = gpWrankRtextConf; it->msg != 0; it++) {
		if (it->msg == msg)
			return it->wtype;
	}
	return -1;
}

/**
 * pre-battle
 */
void PreBattleCalc_WrankBonus(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	int wtype = attacker->weaponType;
	int wrank = GetWeaponLevelFromExp(UNIT_WRANK(&attacker->unit, wtype));
	const struct WrankBonusConfEnt *conf = GetWrankBonusConf(&attacker->unit, wtype, wrank);

	if (conf) {
		attacker->battleAttack       += conf->bonus[BATTLE_STATUS_ATK];
		attacker->battleDefense      += conf->bonus[BATTLE_STATUS_DEF];
		attacker->battleSpeed        += conf->bonus[BATTLE_STATUS_AS];
		attacker->battleHitRate      += conf->bonus[BATTLE_STATUS_HIT];
		attacker->battleAvoidRate    += conf->bonus[BATTLE_STATUS_AVO];
		attacker->battleCritRate     += conf->bonus[BATTLE_STATUS_CRIT];
		attacker->battleDodgeRate    += conf->bonus[BATTLE_STATUS_DODGE];
		attacker->battleSilencerRate += conf->bonus[BATTLE_STATUS_SILENCER];
	}
}

/**
 * help-box
 */
void HbPopuplate_WrankBonus(struct HelpBoxProc *proc)
{
	int wtype;
	const struct WrankBonusConfEnt *conf;

	sHelpBoxType = NEW_HB_DEFAULT;

	if (!gpKernelDesigerConfig->wrank_bonux_rtext_auto_gen)
		return;

	wtype = GetWtypeFromRTextMsg(proc->info->mid);

	if (wtype < 0)
		return;

	conf = GetWrankBonusConf(gStatScreen.unit, wtype, GetWeaponLevelFromExp(UNIT_WRANK(gStatScreen.unit, wtype)));
	if (!conf)
		return;

	sHelpBoxType = NEW_HB_WRANK_STATSCREEN;
}

void DrawHelpBoxLabels_WrankBonus(void)
{
	Text_InsertDrawString(&gHelpBoxSt.text[0], 0x00, TEXT_COLOR_47CF, GetStringFromIndex(MSG_04F3)); // atk
	Text_InsertDrawString(&gHelpBoxSt.text[0], 0x30, TEXT_COLOR_47CF, GetStringFromIndex(MSG_04EF)); // def
	Text_InsertDrawString(&gHelpBoxSt.text[0], 0x60, TEXT_COLOR_47CF, GetStringFromIndex(MSG_0504)); // as

	Text_InsertDrawString(&gHelpBoxSt.text[1], 0x00, TEXT_COLOR_47CF, GetStringFromIndex(MSG_04F4)); // hit
	Text_InsertDrawString(&gHelpBoxSt.text[1], 0x30, TEXT_COLOR_47CF, GetStringFromIndex(MSG_04F5));  // avo
	Text_InsertDrawString(&gHelpBoxSt.text[1], 0x60, TEXT_COLOR_47CF, GetStringFromIndex(MSG_0501));  // crit

	Text_InsertDrawString(&gHelpBoxSt.text[2], 0x00, TEXT_COLOR_47CF, GetStringFromIndex(MSG_51E)); // dodge
	Text_InsertDrawString(&gHelpBoxSt.text[2], 0x30, TEXT_COLOR_47CF, GetStringFromIndex(MSG_SILENCER));  // dodge
	Text_InsertDrawString(&gHelpBoxSt.text[2], 0x70, TEXT_COLOR_47CF, GetStringFromIndex(MSG_MSS_SupportBonus));  // bonus
}

void DrawHelpBoxStats_WrankBonus(struct ProcHelpBoxIntro *proc)
{
	int wtype = GetWtypeFromRTextMsg(proc->msg);
	int wrank = GetWeaponLevelFromExp(UNIT_WRANK(gStatScreen.unit, wtype));
	const struct WrankBonusConfEnt _conf = { 0 };
	const struct WrankBonusConfEnt *conf = GetWrankBonusConf(gStatScreen.unit, wtype, wrank);

	if (!conf) {
		Errorf("failed to get conf: wtype=%d, wrank=%d", wtype, wrank);
		conf = &_conf;
	}

	Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[0], 0x20, TEXT_COLOR_456F, conf->bonus[BATTLE_STATUS_ATK]);
	Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[0], 0x50, TEXT_COLOR_456F, conf->bonus[BATTLE_STATUS_DEF]);
	Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[0], 0x80, TEXT_COLOR_456F, conf->bonus[BATTLE_STATUS_AS]);

	Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 0x20, TEXT_COLOR_456F, conf->bonus[BATTLE_STATUS_HIT]);
	Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 0x50, TEXT_COLOR_456F, conf->bonus[BATTLE_STATUS_AVO]);
	Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 0x80, TEXT_COLOR_456F, conf->bonus[BATTLE_STATUS_CRIT]);

	Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[2], 0x20, TEXT_COLOR_456F, conf->bonus[BATTLE_STATUS_DODGE]);
	Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[2], 0x50, TEXT_COLOR_456F, conf->bonus[BATTLE_STATUS_SILENCER]);
}
