#pragma once

#include "gbafe.h"

extern struct {
    u32 refrain_action : 1;
} gActionDataExpa;

typedef bool (* UnitActionFunc_t)(ProcPtr);
// extern const UnitActionFunc_t gUnitActionTable[CONFIG_UNIT_ACTION_AMT];
extern UnitActionFunc_t const * const gpUnitActionTable;
