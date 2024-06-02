#pragma once

#include "common-chax.h"

typedef bool (* UnitActionFunc_t)(ProcPtr);
// extern const UnitActionFunc_t gUnitActionTable[CONFIG_UNIT_ACTION_AMT];
extern UnitActionFunc_t const * const gpUnitActionTable;
