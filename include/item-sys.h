#pragma once

#include "common-chax.h"

typedef void (*PrepItemEffectFunc_t)(struct ProcPrepItemUse * proc, u16 item);
extern PrepItemEffectFunc_t const gPrepItemEffectFunc[0x100];

bool CanUnitUseMetiStome(struct Unit * unit, int item);
typedef bool (*PrepItemUsabilityFunc_t)(struct Unit * unit, int item);
extern PrepItemUsabilityFunc_t const gPrepItemUsabilityFuncs[0x100];

/* DuraItem */
bool IsDuraItem(int item);
char * GetDuraItemName(int item);
int GetDuraItemDescId(int item);
int GetDuraItemUseDescId(int item);
int GetDuraItemIconId(int item);

/* Misc API functions */
void PrepItemEffect_StatBooster(struct ProcPrepItemUse * proc, u16 item);
void PrepItemEffect_Promotion(struct ProcPrepItemUse * proc, u16 item);
void PrepItemEffect_JunaFruit(struct ProcPrepItemUse * proc, u16 item);

// Struct for key-value pairs
typedef struct
{
    const char * key;
    int value;
} KeyValuePair;

// int compare(const void * a, const void * b);
// int lookup(KeyValuePair * array, int size, const char * key);
int binary_search(const KeyValuePair *array, int size, const char *key);
int getArraySize(const KeyValuePair *array);
