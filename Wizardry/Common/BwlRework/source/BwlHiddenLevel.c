#include "global.h"
#include "common-chax.h"
#include "bwl.h"

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

    bwl->levelGain = 127;
}

int GetUnitHiddenLevel(struct Unit * unit)
{
    struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(unit));
    if (bwl)
        return bwl->levelGain;

    return 0;
}

void UnitHiddenLevelPreLoad(struct Unit * unit)
{
    struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(unit));
    if (!bwl)
        return;

    bwl->levelGain = gClassPreLoadHiddenLevel[UNIT_CLASS_ID(unit)];
}
