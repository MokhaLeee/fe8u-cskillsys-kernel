#pragma once

#include "common-chax.h"

enum play_st_expa_bits {
	PLAYSTEXPA_BIT_DivineReprieve,

	/* MAX */
	PLAYSTEXPA_BIT_MAX = 128,
};

extern u32 gPlayStExpa[4];

void NewGame_InitPlayStExpa(void);
void ChapterInit_ResetPlayStExpa(void);

void EMS_SavePlayStExpa(u8 *dst, const u32 size);
void EMS_LoadPlayStExpa(u8 *src, const u32 size);
