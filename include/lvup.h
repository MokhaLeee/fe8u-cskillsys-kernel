#pragma once

#include "common-chax.h"
#include "kernel-lib.h"

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

int GetUnitJobBasedHpGrowth(struct Unit *unit);
int GetUnitJobBasedPowGrowth(struct Unit *unit);
int GetUnitJobBasedMagGrowth(struct Unit *unit);
int GetUnitJobBasedSklGrowth(struct Unit *unit);
int GetUnitJobBasedSpdGrowth(struct Unit *unit);
int GetUnitJobBasedLckGrowth(struct Unit *unit);
int GetUnitJobBasedDefGrowth(struct Unit *unit);
int GetUnitJobBasedResGrowth(struct Unit *unit);

/* Job growth */
struct JobGrowthEnt { s8 st[UNIT_STATUS_MAX - 1]; };
// extern struct JobGrowthEnt const gJobGrowthList[0x100];
extern struct JobGrowthEnt const *const gpJobGrowthList;

int GetUnitHpGrowthJobBonus(int status, struct Unit *unit);
int GetUnitPowGrowthJobBonus(int status, struct Unit *unit);
int GetUnitMagGrowthJobBonus(int status, struct Unit *unit);
int GetUnitSklGrowthJobBonus(int status, struct Unit *unit);
int GetUnitSpdGrowthJobBonus(int status, struct Unit *unit);
int GetUnitLckGrowthJobBonus(int status, struct Unit *unit);
int GetUnitDefGrowthJobBonus(int status, struct Unit *unit);
int GetUnitResGrowthJobBonus(int status, struct Unit *unit);
