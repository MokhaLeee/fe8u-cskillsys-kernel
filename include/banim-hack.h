#pragma once

#include <common-chax.h>

enum chax_banim_video_alloc {
	OBCHR_CHAX_ANIMNUM = (0x2000 / CHR_SIZE),
	OBPAL_CHAX_ANIMNUM = 0xF,

	// Img order
	IMG_ANIMNUMBER_LEN = 11,

	ANIMNUMBER_CHR_BASE_G = 0,
	ANIMNUMBER_CHR_BASE_R = 0 + 2 * 2 * IMG_ANIMNUMBER_LEN,
};

ProcPtr NewEfxAnimNumberExt(int number, int x, int y);
ProcPtr NewEfxAnimNumber(struct Anim *anim, int number);

extern const u8 Img_EfxAnimNumber[];
extern const u16 Pal_EfxAnimNumber[];
extern AnimScr AnimScr_AnimNumber[];
