#include "common-chax.h"
#include "strmag.h"

void UnitLoadStatsFromCharacterMagic(struct Unit *unit, const struct CharacterData *character)
{
	UNIT_MAG(unit) = GetUnitBaseMagic(unit);
}
