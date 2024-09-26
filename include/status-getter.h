#pragma once

#include "common-chax.h"

typedef int (*StatusGetterFunc_t)(int old, struct Unit *unit);

extern StatusGetterFunc_t const *const gpHpGetters;
extern StatusGetterFunc_t const *const gpPowGetters;
extern StatusGetterFunc_t const *const gpMagGetters;
extern StatusGetterFunc_t const *const gpSklGetters;
extern StatusGetterFunc_t const *const gpSpdGetters;
extern StatusGetterFunc_t const *const gpLckGetters;
extern StatusGetterFunc_t const *const gpDefGetters;
extern StatusGetterFunc_t const *const gpResGetters;
extern StatusGetterFunc_t const *const gpMovGetters;
extern StatusGetterFunc_t const *const gpConGetters;
extern StatusGetterFunc_t const *const gpAidGetters;

int HpMaxGetter(struct Unit *unit);
int PowGetter(struct Unit *unit);
int MagGetter(struct Unit *unit);
int SklGetter(struct Unit *unit);
int SpdGetter(struct Unit *unit);
int DefGetter(struct Unit *unit);
int ResGetter(struct Unit *unit);
int LckGetter(struct Unit *unit);
int AidGetter(struct Unit *unit);
int ConGetter(struct Unit *unit);
int MovGetter(struct Unit *unit);

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
