#pragma once

#include "common-chax.h"

struct UnitListHeader {
    u32 ck32;
};

extern bool (* _JudgeUnitList)(struct Unit * unit, struct UnitListHeader * ref);
extern void (* _WriteUnitList)(struct Unit * unit, struct UnitListHeader * out);

#define JudgeUnitList _JudgeUnitList
#define WriteUnitList _WriteUnitList
