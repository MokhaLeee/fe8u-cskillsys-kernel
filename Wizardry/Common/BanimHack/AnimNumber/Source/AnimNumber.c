#include <common-chax.h>
#include <kernel-lib.h>
#include <banim-hack.h>
#include <battle-system.h>

extern u8 sAnimNumberSlot;

NOINLINE void VramCopyAnimNumber(const void *src, void *dst)
{
	VramCopy(src,                                     dst,         CHR_SIZE * 2);
	VramCopy(src + CHR_SIZE * IMG_ANIMNUMBER_LEN * 2, dst + 0x400, CHR_SIZE * 2);
}

void SetupEfxAnimNumberGfx(int number, int slot)
{
	int i, abs_num;
	int base_chr;

	void *vram_base = OBJ_VRAM0 + (OBCHR_CHAX_ANIMNUM + slot * 8) * CHR_SIZE;

	CpuFastFill(0, vram_base,         CHR_SIZE * 8);
	CpuFastFill(0, vram_base + 0x400, CHR_SIZE * 8);

	if (number > 0) {
		abs_num = number;
		base_chr = ANIMNUMBER_CHR_BASE_G;
	} else {
		abs_num = -number;
		base_chr = ANIMNUMBER_CHR_BASE_R;
	}

	LIMIT_AREA(abs_num, 0, 999);

	/**
	 * Number image
	 */
	for (i = 3; i > 0;) {
		int lo = simple_mod(abs_num, 10);

		VramCopyAnimNumber(
			Img_EfxAnimNumber + (base_chr + lo * 2) * CHR_SIZE,
			vram_base + i * 2 * CHR_SIZE
		);

		abs_num = simple_div(abs_num, 10);

		i--;
		if (abs_num <= 0)
			break;
	}

	/**
	 * Prefix image
	 */
	VramCopyAnimNumber(
		Img_EfxAnimNumber + (base_chr + 10 * 2) * CHR_SIZE,
		vram_base + i * 2 * CHR_SIZE
	);

	/**
	 * Pal
	 */
	ApplyPalette(Pal_EfxAnimNumber, 0x10 + OBPAL_CHAX_ANIMNUM);
}

ProcPtr NewEfxAnimNumberExt(int x, int y, int number)
{
	ProcPtr proc;

	sAnimNumberSlot += 1;
	sAnimNumberSlot &= 1;

	SetupEfxAnimNumberGfx(number, sAnimNumberSlot);

	proc = NewEkrsubAnimeEmulator(
		OAM1_X(x), OAM0_Y(y),
		AnimScr_AnimNumber,
		0,
		OAM2_PAL(OBPAL_CHAX_ANIMNUM) + OAM2_LAYER(1) + OAM2_CHR(OBCHR_CHAX_ANIMNUM + sAnimNumberSlot * 8),
		0,
		PROC_TREE_3
	);

	return proc;
}

ProcPtr NewEfxAnimNumber(struct Anim *anim, int number)
{
	int y, x = OAM1_X(anim->xPosition);

	x -= 0x34;
	if (x < 0)
		x = 0;

	y = 0x25;

	return NewEfxAnimNumberExt(x, y, number);
}
