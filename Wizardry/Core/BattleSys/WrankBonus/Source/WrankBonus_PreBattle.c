#include "common-chax.h"
#include "battle-system.h"

STATIC_DECLAR const struct WrankBonusConfEnt *GetWrankBonusConf(struct Unit *unit, int wtype, int wrank)
{
	const struct WrankBonusConfEnt *it;

	for (it = gpWrankBonusConf; it->wrank != WPN_LEVEL_0; it++) {
		if (it->wtype == wtype && it->wrank == wrank)
			return it;
	}
	return NULL;
}

void PreBattleCalc_WrankBonus(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	int wtype = attacker->weaponType;
	int wrank = attacker->unit.ranks[wtype];
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
