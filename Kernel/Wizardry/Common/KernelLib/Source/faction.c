#include <common-chax.h>
#include <kernel-lib.h>

int GetUidFaction(u8 uid)
{
	switch (uid) {
	case 0 ... 0x3F:
	default:
		return FACTION_BLUE;

	case 0x40 ... 0x7F:
		return FACTION_RED;

	case 0x80 ... 0xBF:
		return FACTION_GREEN;

	case 0xC0 ... 0xFF:
		return FACTION_PURPLE;
	}
}

int GetUnitFaction(struct Unit *unit)
{
	return GetUidFaction(unit->index);
}
