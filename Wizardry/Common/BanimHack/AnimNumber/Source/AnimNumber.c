#include <common-chax.h>
#include <kernel-lib.h>
#include <banim-hack.h>
#include <battle-system.h>

static void SetupEfxAnimNumberGfx(int number)
{
	int i, abs_num = ABS(number);
	void *dst;
	const void *src;
	int src_chr;

	LIMIT_AREA(abs_num, 0, 99);

	CpuFastFill(0, OBJ_VRAM0 + OBCHR_CHAX_ANIMNUM * CHR_SIZE,         CHR_SIZE * 8);
	CpuFastFill(0, OBJ_VRAM0 + OBCHR_CHAX_ANIMNUM * CHR_SIZE + 0x400, CHR_SIZE * 8);

	/**
	 * Number image
	 */
	for (i = 2; i > 0;) {
		int lo = simple_mod(abs_num, 10);
		int src_chr = ANIMNUMBER_CHR_0 + lo * 2;

		Printf("lo=%d, chr=0x%X", lo, src_chr);

		src = Img_EfxAnimNumber + src_chr * CHR_SIZE;
		dst = OBJ_VRAM0 + OBCHR_CHAX_ANIMNUM * CHR_SIZE + i * 2 * CHR_SIZE;

		VramCopy(src, dst, CHR_SIZE * 2);
		VramCopy(src + ANIMNUMBER_CHR_MAX * CHR_SIZE, dst + 0x400, CHR_SIZE * 2);

		abs_num = simple_div(abs_num, 10);

		i--;
		if (abs_num <= 0)
			break;
	}

	/**
	 * Prefix image
	 */
	if (number > 0)
		src_chr = ANIMNUMBER_CHR_PLUS;
	else
		src_chr = ANIMNUMBER_CHR_MINUS;

	src = Img_EfxAnimNumber + src_chr * CHR_SIZE;
	dst = OBJ_VRAM0 + OBCHR_CHAX_ANIMNUM * CHR_SIZE + i * 2 * CHR_SIZE;

	VramCopy(src, dst, CHR_SIZE * 2);
	VramCopy(src + ANIMNUMBER_CHR_MAX * CHR_SIZE, dst + 0x400, CHR_SIZE * 2);

	/**
	 * Pal
	 */
	if (number > 0)
		ApplyPalette(Pal_EfxAnimNumber, 0x10 + OBPAL_CHAX_ANIMNUM);
	else
		ApplyPalette(Pal_EfxAnimNumber + 0x10, 0x10 + OBPAL_CHAX_ANIMNUM);
}

ProcPtr NewEfxAnimNumberExt(int x, int y, int number)
{
	SetupEfxAnimNumberGfx(number);

	return NewEkrsubAnimeEmulator(
		x, y,
		AnimScr_AnimNumber,
		0,
		OAM2_PAL(OBPAL_CHAX_ANIMNUM) + OAM2_LAYER(1) + OAM2_CHR(OBCHR_CHAX_ANIMNUM),
		0,
		PROC_TREE_3
	);
}

ProcPtr NewEfxAnimNumber(struct Anim *anim, int number)
{
	return NewEfxAnimNumberExt(number, anim->xPosition - 0x28, anim->yPosition - 0x30);
}

void DisplayEfxAnimNumberForHpDrain(struct Anim *anim)
{
	int round = anim->nextRoundId - 1;
	struct BattleHit *hit = (prBattleHitArray + BattleHitArrayWidth * round);

	NewEfxAnimNumber(anim, hit->hpChange);
}
