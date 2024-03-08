#include "global.h"
#include "bmunit.h"

#include "common-chax.h"
#include "unit-expa.h"

void SetBitUES(struct Unit * unit, int bit)
{
    if (bit < 8)
        unit->_u3A |= 1 << bit;

    if (bit < 16)
        unit->_u3B |= 1 << bit;
}

void ClearBitUES(struct Unit * unit, int bit)
{
    if (bit < 8)
        unit->_u3A &= ~(1 << bit);
    else if (bit < 16)
        unit->_u3B &= ~(1 << bit);
}

bool CheckBitUES(struct Unit * unit, int bit)
{
    if (bit < 8)
        return !!(unit->_u3A & (1 << bit));
    else if (bit < 16)
        return !!(unit->_u3B & (1 << bit));

    return false;
}

static void reset_expa(s8 uid)
{
    struct Unit * unit = GetUnit(uid);
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
