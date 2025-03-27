#include "common-chax.h"
#include "kernel-lib.h"
#include "battle-system.h"

int GetDefenderNegatedDefense(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	const struct NegateDefConfEnt *it;
	int rate = 0;
	int iid = ITEM_INDEX(attacker->weapon);
	int status = defender->battleDefense;

	if (!(attacker->weaponAttributes & IA_NEGATE_DEFENSE))
		return status;

	if (iid == ITEM_NONE)
		return status;

	for (it = gpNegateDefConf; it->item != ITEM_NONE; it++) {
		if (it->item == iid && (it->evt_flag == 0 || CheckFlag(it->evt_flag))) {
			rate = it->rate;
			break;
		}
	}

	LIMIT_AREA(rate, 0, 100);

	if (rate == 0 || rate == 100)
		return 0;

	return perc_of(status, rate);
}
