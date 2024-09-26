#pragma once

#include "gbafe.h"

extern struct {
	u32 refrain_action : 1;
	u32 flag1 : 31;
	u32 flag2, flag3, flag4;
} gActionDataExpa;

typedef bool (*UnitActionFunc_t)(ProcPtr proc);
// extern const UnitActionFunc_t gUnitActionTable[CONFIG_UNIT_ACTION_AMT];
extern UnitActionFunc_t const *const gpUnitActionTable;
