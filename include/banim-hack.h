#pragma once

#include <common-chax.h>

enum chax_banim_video_alloc {
	OBCHR_CHAX_ANIMNUM = (0x2000 / CHR_SIZE),
	OBPAL_CHAX_ANIMNUM = 0xF, // ovrwrite EfxSkill name

	// Img order
	IMG_ANIMNUMBER_LEN = 11,

	ANIMNUMBER_CHR_BASE_G = 0,
	ANIMNUMBER_CHR_BASE_R = 0 + 2 * 2 * IMG_ANIMNUMBER_LEN,
};

struct ProcEfxAnimNumber {
	PROC_HEADER;

	struct Anim *anim;
	int number;
	int slot;
	struct Anim *anim_num;
};

void NewEfxAnimNumber(struct Anim *anim, int number);
bool EfxAnimNumberExists(void);

extern u8 sAnimNumberSlot;

extern u16 const *const gpPAL_EfxMoji;
// extern const u8 Img_EfxAnimNumber[];
// extern const u16 Pal_EfxAnimNumber[];
extern u8 const *const gpImg_EfxAnimNumber;
extern u16 const *const gpPal_EfxAnimNumber;
// extern AnimScr AnimScr_AnimNumber[];
extern AnimScr *gpAnimScr_AnimNumber;
