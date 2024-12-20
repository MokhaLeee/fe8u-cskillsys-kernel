#pragma once

#include "common-chax.h"

typedef void (*PrepItemEffectFunc_t)(struct ProcPrepItemUse *proc, u16 item);
// extern PrepItemEffectFunc_t const gPrepItemEffectFunc[0x100];
extern PrepItemEffectFunc_t const *const gpPrepItemEffectFunc;

bool CanUnitUseMetiStome(struct Unit *unit, int item);
typedef bool (*PrepItemUsabilityFunc_t)(struct Unit *unit, int item);
// extern PrepItemUsabilityFunc_t const gPrepItemUsabilityFuncs[0x100];
extern PrepItemUsabilityFunc_t const *const gpPrepItemUsabilityFuncs;

/* DuraItem */
bool IsDuraItem(int item);
char *GetDuraItemName(int item);
int GetDuraItemDescId(int item);
int GetDuraItemUseDescId(int item);
int GetDuraItemIconId(int item);

/* Misc API functions */
void PrepItemEffect_StatBooster(struct ProcPrepItemUse *proc, u16 item);
void PrepItemEffect_Promotion(struct ProcPrepItemUse *proc, u16 item);
void PrepItemEffect_JunaFruit(struct ProcPrepItemUse *proc, u16 item);

typedef int (*HealAmountGetterFunc_t)(int old, struct Unit *actor, struct Unit *target);
extern HealAmountGetterFunc_t const *const gpHealAmountGetters;
