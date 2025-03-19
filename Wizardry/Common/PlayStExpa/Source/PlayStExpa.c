#include "common-chax.h"
#include "kernel-lib.h"
#include "playst-expa.h"

void NewGame_InitPlayStExpa(void)
{
	memset(gPlayStExpa, 0, sizeof(gPlayStExpa));
}

void ChapterInit_ResetPlayStExpa(void)
{
	memset(gPlayStExpa, 0, sizeof(gPlayStExpa));
}

void EMS_SavePlayStExpa(u8 *dst, const u32 size)
{
	gPlaySt.time_saved = GetGameClock();
	WriteAndVerifySramFast(gPlayStExpa, dst, size);
}

void EMS_LoadPlayStExpa(u8 *src, const u32 size)
{
	ReadSramFast(src, gPlayStExpa, size);
}
