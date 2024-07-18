#pragma once

#include "common-chax.h"

#ifdef CONFIG_USE_RAND_C

void InitRandC(void);
bool Roll1RandC(int threshold, int amount);
bool Roll2RandC(int threshold, int amount);

void SaveRandC(u8 * dst, const u32 size);
void LoadRandC(u8 * src, const u32 size);

#endif /* CONFIG_USE_RAND_C */
