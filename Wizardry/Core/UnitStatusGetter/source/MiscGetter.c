#include "global.h"
#include "bmunit.h"
#include "bmitem.h"
#include "constants/characters.h"
#include "constants/items.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int StatusGetterPadFunc(int status, struct Unit * unit)
{
    return status;
}

int StatusGetterCheckCpas(int status, struct Unit * unit)
{
    LIMIT_AREA(status, 0, 127);
    return status;
}

/* Vanilla hooks */
int GetUnitMagBy2Range(struct Unit * unit)
{
    int range;

    if (unit->pCharacterData->number == CHARACTER_FOMORTIIS)
        return GetItemMaxRange(ITEM_NIGHTMARE);

    range = MagGetter(unit) / 2;

    if (range < 5)
        range = 5;

    return range;
}

int GetUnitCurrentHp(struct Unit * unit)
{
    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);

    return unit->curHP;
}
