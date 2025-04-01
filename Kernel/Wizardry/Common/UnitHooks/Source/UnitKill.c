#include "common-chax.h"

typedef int (*KillUnitFunc_t)(struct Unit *unit);
// extern const KillUnitFunc_t gKillUnitHooks[];
extern KillUnitFunc_t const *const gpKillUnitHooks;

LYN_REPLACE_CHECK(UnitKill);
void UnitKill(struct Unit *unit)
{
#if CHAX
	const KillUnitFunc_t *it;
	for (it = gpKillUnitHooks; *it; it++)
		(*it)(unit);
#endif

	if (UNIT_FACTION(unit) == FACTION_BLUE) {
		if (UNIT_IS_PHANTOM(unit))
			unit->pCharacterData = NULL;
		else {
			unit->state |= US_DEAD | US_HIDDEN;
			InitUnitsupports(unit);
		}
	} else
		unit->pCharacterData = NULL;
}
