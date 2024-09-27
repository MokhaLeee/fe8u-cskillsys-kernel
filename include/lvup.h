#pragma once

#include "common-chax.h"

extern const u8 MetisTomeGrowthBonus;

/* Growth getter */
int GetUnitHpGrowth(struct Unit *unit);
int GetUnitPowGrowth(struct Unit *unit);
int GetUnitMagGrowth(struct Unit *unit);
int GetUnitSklGrowth(struct Unit *unit);
int GetUnitSpdGrowth(struct Unit *unit);
int GetUnitLckGrowth(struct Unit *unit);
int GetUnitDefGrowth(struct Unit *unit);
int GetUnitResGrowth(struct Unit *unit);
