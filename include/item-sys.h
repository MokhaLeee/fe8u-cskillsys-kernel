#pragma once

#include "common-chax.h"

typedef void (* PrepItemEffectFunc_t)(struct ProcPrepItemUse * proc, u16 item);
extern PrepItemEffectFunc_t const gPrepItemEffectFunc[0x100];

bool CanUnitUseMetiStome(struct Unit * unit, int item);
typedef bool (* PrepItemUsabilityFunc_t)(struct Unit * unit, int item);
extern PrepItemUsabilityFunc_t const gPrepItemUsabilityFuncs[0x100];

/* Misc API functions */
void PrepItemEffect_StatBooster(struct ProcPrepItemUse * proc, u16 item);
void PrepItemEffect_Promotion(struct ProcPrepItemUse * proc, u16 item);
void PrepItemEffect_JunaFruit(struct ProcPrepItemUse * proc, u16 item);
