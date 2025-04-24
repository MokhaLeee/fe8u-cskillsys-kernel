#include "common-chax.h"
#include "unit-expa.h"
#include "kernel-lib.h"

void SetBitUES(struct Unit *unit, int bit)
{
	Assert(bit >= 0 && bit < 16);
	_BIT_SET(&unit->_u3A, bit);
}

void ClearBitUES(struct Unit *unit, int bit)
{
	Assert(bit >= 0 && bit < 16);
	_BIT_CLR(&unit->_u3A, bit);
}

bool CheckBitUES(struct Unit *unit, int bit)
{
	Assert(bit >= 0 && bit < 16);
	return _BIT_CHK(&unit->_u3A, bit);
}

static void reset_expa(s8 uid)
{
	struct Unit *unit = GetUnit(uid);

	if (!unit)
		return;

	unit->_u3A = 0;
	unit->_u3B = 0;
}

void ResetUnitsExpaSus(void)
{
	int i;

	for (i = FACTION_BLUE + 1; i < (FACTION_BLUE + 0x40); i++)
		reset_expa(i);

	for (i = FACTION_RED + 1; i < (FACTION_RED + 0x40); i++)
		reset_expa(i);

	for (i = FACTION_GREEN + 1; i < (FACTION_GREEN + 0x40); i++)
		reset_expa(i);
}
