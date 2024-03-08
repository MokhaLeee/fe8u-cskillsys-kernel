#pragma once

#include "global.h"
#include "bmunit.h"

typedef int (* StatusGetterFunc_t)(int old, struct Unit * unit);

extern const StatusGetterFunc_t gHpGetters[],  gPowGetters[], gMagGetters[];
extern const StatusGetterFunc_t gSklGetters[], gSpdGetters[], gLckGetters[], gDefGetters[], gResGetters[];
extern const StatusGetterFunc_t gMovGetters[], gConGetters[], gAidGetters[];

int HpMaxGetter(struct Unit * unit);
int PowGetter(struct Unit * unit);
int MagGetter(struct Unit * unit);
int SklGetter(struct Unit * unit);
int SpdGetter(struct Unit * unit);
int DefGetter(struct Unit * unit);
int ResGetter(struct Unit * unit);
int LckGetter(struct Unit * unit);
int AidGetter(struct Unit * unit);
int ConGetter(struct Unit * unit);
int MovGetter(struct Unit * unit);
