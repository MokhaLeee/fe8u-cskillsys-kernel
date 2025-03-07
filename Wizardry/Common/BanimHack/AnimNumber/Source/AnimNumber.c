#include <common-chax.h>
#include <kernel-lib.h>
#include <banim-hack.h>
#include <battle-system.h>

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
		int lo = k_umod(abs_num, 10);

		VramCopyAnimNumber(
			gpImg_EfxAnimNumber + (base_chr + lo * 2) * CHR_SIZE,
			vram_base + i * 2 * CHR_SIZE
		);

		abs_num = k_udiv(abs_num, 10);

		i--;
		if (abs_num <= 0)
			break;
	}

	/**
	 * Prefix image
	 */
	VramCopyAnimNumber(
		gpImg_EfxAnimNumber + (base_chr + 10 * 2) * CHR_SIZE,
		vram_base + i * 2 * CHR_SIZE
	);

	/**
	 * Pal
	 */
	ApplyPalette(gpPal_EfxAnimNumber, 0x10 + OBPAL_CHAX_ANIMNUM);
}

void EfxAnimNumber_Start(struct ProcEfxAnimNumber *proc)
{
	struct Anim *anim;
	int y, x = OAM1_X(proc->anim->xPosition);

	x -= 0x34;
	if (x < 0)
		x = 0;

	y = 0x25;

	proc->anim_num = anim = AnimCreate(gpAnimScr_AnimNumber, 0x95);
	anim->xPosition = OAM1_X(x);
	anim->yPosition = OAM0_Y(y);
	anim->oam2Base  = OAM2_PAL(OBPAL_CHAX_ANIMNUM) + OAM2_LAYER(1) + OAM2_CHR(OBCHR_CHAX_ANIMNUM + proc->slot * 8);
}

void EfxAnimNumber_Loop(struct ProcEfxAnimNumber *proc)
{
	if (proc->anim_num->state == 0)
		Proc_Break(proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_EfxAnimNumber[] = {
	PROC_YIELD,
	PROC_CALL(EfxAnimNumber_Start),
	PROC_REPEAT(EfxAnimNumber_Loop),
	PROC_END,
};

void NewEfxAnimNumber(struct Anim *anim, int number)
{
	struct ProcEfxAnimNumber *proc;

	if (number == 0)
		return;

	sAnimNumberSlot += 1;
	sAnimNumberSlot &= 1;

	proc = Proc_Start(ProcScr_EfxAnimNumber, PROC_TREE_3);
	proc->anim = anim;
	proc->number = number;
	proc->slot = sAnimNumberSlot;

	SetupEfxAnimNumberGfx(proc->number, proc->slot);
}

bool EfxAnimNumberExists(void)
{
	return Proc_Exists(ProcScr_EfxAnimNumber);
}
