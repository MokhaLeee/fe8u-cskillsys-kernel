#include "common-chax.h"
#include "bwl.h"

STATIC_DECLAR int GetUnitStaticHiddenLevel(struct Unit * unit)
{
    if (!UNIT_IS_VALID(unit))
        return 0;

    return gpClassPreLoadHiddenLevel[UNIT_CLASS_ID(unit)];
}

void NewBwlRecordHiddenLevel(struct Unit * unit)
{
    int level;
    struct NewBwl * bwl;
    bwl = GetNewBwl(UNIT_CHAR_ID(unit));
    if (!bwl)
        return;

    level = unit->level + bwl->levelGain;
    if (level > 127)
        level = 127;

    bwl->levelGain = level;
}

int GetUnitHiddenLevel(struct Unit * unit)
{
    struct NewBwl * bwl;
    bwl = GetNewBwl(UNIT_CHAR_ID(unit));
    if (bwl)
        return bwl->levelGain;

    return GetUnitStaticHiddenLevel(unit);
}

void UnitHiddenLevelPreLoad(struct Unit * unit)
{
    struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(unit));
    if (!bwl)
        return;

    bwl->levelGain = GetUnitStaticHiddenLevel(unit);
}
