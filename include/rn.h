#pragma once

#include "global.h"

void InitRandC(void);
bool Roll1RandC(int threshold, int amount);
bool Roll2RandC(int threshold, int amount);

void SaveRandC(u8 * dst, const u32 size);
void LoadRandC(u8 * src, const u32 size);
