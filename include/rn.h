#pragma once

#include "common-chax.h"

void InitRandC(void);
bool Roll1RandC(int threshold);
bool Roll2RandC(int threshold);

void SaveRandC(u8 *dst, const u32 size);
void LoadRandC(u8 *src, const u32 size);
