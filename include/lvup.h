#pragma once

#include "common-chax.h"

/* Growth getter */
int GetUnitHpGrowth(struct Unit * unit);
int GetUnitPowGrowth(struct Unit * unit);
int GetUnitMagGrowth(struct Unit * unit);
int GetUnitSklGrowth(struct Unit * unit);
int GetUnitSpdGrowth(struct Unit * unit);
int GetUnitLckGrowth(struct Unit * unit);
int GetUnitDefGrowth(struct Unit * unit);
int GetUnitResGrowth(struct Unit * unit);

/* Port for FEB patch: SetHPClassLimit */
extern const u8 pr_SetHPClassLimitJudgement[];

#define CheckUnlockAllyMhpLimit() \
( \
    pr_SetHPClassLimitJudgement[0] == 0x00 && \
    pr_SetHPClassLimitJudgement[1] == 0x4A && \
    pr_SetHPClassLimitJudgement[2] == 0x97 && \
    pr_SetHPClassLimitJudgement[3] == 0x46 \
)

#define KUNIT_MHP_MAX(unit) (CheckUnlockAllyMhpLimit() ? 120 : UNIT_MHP_MAX(unit))
