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

extern StatusGetterFunc_t const gpExternalHpGetters;
extern StatusGetterFunc_t const gpExternalPowGetters;
extern StatusGetterFunc_t const gpExternalMagGetters;
extern StatusGetterFunc_t const gpExternalSklGetters;
extern StatusGetterFunc_t const gpExternalSpdGetters;
extern StatusGetterFunc_t const gpExternalLckGetters;
extern StatusGetterFunc_t const gpExternalDefGetters;
extern StatusGetterFunc_t const gpExternalResGetters;
extern StatusGetterFunc_t const gpExternalMovGetters;
extern StatusGetterFunc_t const gpExternalConGetters;
extern StatusGetterFunc_t const gpExternalAidGetters;

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

struct UnitMaxStatusConfig {
	s8 hp, mag, pow, skl, spd, def, res, lck, con, mov;
	u8 _pad_[2];
};

extern const struct UnitMaxStatusConfig gUnitMaxStatusConfigTable[0x100];
extern struct UnitMaxStatusConfig const * const gpUnitMaxStatusConfigTable;

int GetUnitMaxStatusHp(struct Unit *unit);
int GetUnitMaxStatusPow(struct Unit *unit);
int GetUnitMaxStatusMag(struct Unit *unit);
int GetUnitMaxStatusSkl(struct Unit *unit);
int GetUnitMaxStatusSpd(struct Unit *unit);
int GetUnitMaxStatusLck(struct Unit *unit);
int GetUnitMaxStatusDef(struct Unit *unit);
int GetUnitMaxStatusRes(struct Unit *unit);
int GetUnitMaxStatusMov(struct Unit *unit);
int GetUnitMaxStatusCon(struct Unit *unit);
