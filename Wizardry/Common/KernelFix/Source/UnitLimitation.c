#include "common-chax.h"

static inline int GetFactionUnitAmount(int faction)
{
    switch (faction) {
    case FACTION_BLUE:
        return CONFIG_UNIT_AMT_ALLY;

    case FACTION_GREEN:
        return CONFIG_UNIT_AMT_NPC;

    case FACTION_RED:
        return CONFIG_UNIT_AMT_ENEMY;

    default:
        return 0;
    }
}

/* LynJump */
struct Unit * GetFreeUnit(int faction)
{
    int i, amount = GetFactionUnitAmount(faction);

    for (i = 0; i < amount; i++)
    {
        struct Unit * unit = GetUnit(faction + i + 1);

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
