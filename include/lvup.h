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


static inline int GetUnitBaseHpGrowth(struct Unit * unit)
{
    return unit->pCharacterData->growthHP;
}

static inline int GetUnitBasePowGrowth(struct Unit * unit)
{
    return unit->pCharacterData->growthPow;
}

static inline int GetUnitBaseSklGrowth(struct Unit * unit)
{
    return unit->pCharacterData->growthSkl;
}

static inline int GetUnitBaseSpdGrowth(struct Unit * unit)
{
    return unit->pCharacterData->growthSpd;
}

static inline int GetUnitBaseLckGrowth(struct Unit * unit)
{
    return unit->pCharacterData->growthLck;
}

static inline int GetUnitBaseDefGrowth(struct Unit * unit)
{
    return unit->pCharacterData->growthDef;
}

static inline int GetUnitBaseResGrowth(struct Unit * unit)
{
    return unit->pCharacterData->growthRes;
}
