#include "common-chax.h"
#include "kernel-lib.h"

/* LynJump */
struct Unit * GetFreeUnit(int faction)
{
    int i, amount = GetFactionUnitAmount(faction);

    for (i = 1; i <= amount; i++)
    {
        struct Unit * unit = GetUnit(faction + i);

        if (unit->pCharacterData == NULL)
            return unit;
    }
    return NULL;
}

/* LynJump */
struct Unit * GetFreeBlueUnit(const struct UnitDefinition * uDef)
{
    return GetFreeUnit(FACTION_BLUE);
}
