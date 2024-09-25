// SPDX-License-Identifier: GPL-2.0-only

#include "common-chax.h"
#include "strmag.h"

void UnitLoadStatsFromCharacterMagic(struct Unit *unit, const struct CharacterData * character)
{
    UNIT_MAG(unit) = GetUnitBaseMagic(unit);
}
