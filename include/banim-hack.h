#pragma once

#include <common-chax.h>

enum chax_banim_video_alloc {
	OBCHR_CHAX_ANIMNUM = (0x2000 / CHR_SIZE),
	OBPAL_CHAX_ANIMNUM = 0xF,

	// Img order
	ANIMNUMBER_CHR_0 = 0,
	ANIMNUMBER_CHR_PLUS  = 10 * 2,
	ANIMNUMBER_CHR_MINUS = 11 * 2,
	ANIMNUMBER_CHR_BLANK = 12 * 2,
	ANIMNUMBER_CHR_MAX = 13 * 2,
};

ProcPtr NewEfxAnimNumberExt(int number, int x, int y);
ProcPtr NewEfxAnimNumber(struct Anim *anim, int number);
void DisplayEfxAnimNumberForHpDrain(struct Anim *anim);

extern const u8 Img_EfxAnimNumber[];
extern const u16 Pal_EfxAnimNumber[];
extern AnimScr AnimScr_AnimNumber[];
