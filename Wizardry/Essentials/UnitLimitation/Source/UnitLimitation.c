#include "common-chax.h"
#include "kernel-lib.h"

LYN_REPLACE_CHECK(GetFreeUnit);
struct Unit *GetFreeUnit(int faction)
{
	int i, amount = GetFactionUnitAmount(faction);

	for (i = 1; i <= amount; i++) {
		struct Unit *unit = GetUnit(faction + i);

		if (unit->pCharacterData == NULL)
			return unit;
	}
	return NULL;
}

LYN_REPLACE_CHECK(GetFreeBlueUnit);
struct Unit *GetFreeBlueUnit(const struct UnitDefinition *uDef)
{
	return GetFreeUnit(FACTION_BLUE);
}
