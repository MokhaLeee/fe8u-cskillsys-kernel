#include "common-chax.h"
#include "kernel-lib.h"

extern u8 const *const pr_SummonConfig;

/**
 * 0:  not exists
 * 1:  exists
 * -1: invalid
 */
static int check_creature_exists(struct Unit *unit)
{
	const u8 *it;
	int pid = UNIT_CHAR_ID(unit);

	/* find the summoner config */
	for (it = pr_SummonConfig; ; it = it + 2) {
		u8 summoner = it[0];
		u8 creature = it[1];

		if (summoner == CHARACTER_NONE)
			return -1;

		if (creature == CHARACTER_NONE)
			continue;

		if (summoner == pid)
			break;
	}

	/* find the creature */
	FOR_UNITS_VALID_FACTION(FACTION_BLUE, creature, {
		if (UNIT_CHAR_ID(creature) == it[1])
			return 1;
	})

	FOR_UNITS_VALID_FACTION(FACTION_GREEN, creature, {
		if (UNIT_CHAR_ID(creature) == it[1])
			return 1;
	})

	return 0;
}

LYN_REPLACE_CHECK(SummonCommandUsability);
u8 SummonCommandUsability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	switch (check_creature_exists(gActiveUnit)) {
	case -1:
		return MENU_NOTSHOWN;

	case 1:
		return MENU_NOTSHOWN;

	case 0:
		break;
	}

	MakeTargetListForSummon(gActiveUnit);
	if (GetSelectTargetCount() == 0)
		return MENU_NOTSHOWN;

	return MENU_ENABLED;
}
