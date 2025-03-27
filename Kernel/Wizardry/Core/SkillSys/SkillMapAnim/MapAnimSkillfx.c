#include "common-chax.h"
#include "skill-system.h"

STATIC_DECLAR void MapAnimSkillfx_Initfx(struct ProcMapAnimSkillfx *proc)
{
	proc->timer = 0;

	if (proc->pos == POS_L) {
		LoadIconObjectGraphics(proc->icon_idx, MAPANIMFX_CHR_L);

		proc->x -= 12;
		proc->y += 2;
	} else {
		LoadIconObjectGraphics(proc->icon_idx, MAPANIMFX_CHR_R);

		proc->x += 10;
		proc->y += 2;
	}
	LoadIconPalette(0, 0x10 + MAPANIMFX_PAL);
}

STATIC_DECLAR void MapAnimSkillfx_DrawIconIDLE(struct ProcMapAnimSkillfx *proc)
{
	int chr, oam2;

	int ix = proc->x * 8;
	int iy = proc->y * 8 + 4;

	if (proc->pos == POS_L)
		ix = ix - 1;
	else
		ix = ix + 2;

	if (ix < -16 || ix > DISPLAY_WIDTH)
		return;

	if (iy < -16 || iy > DISPLAY_HEIGHT)
		return;

	chr = proc->pos == POS_L
		? MAPANIMFX_CHR_L
		: MAPANIMFX_CHR_R;

	oam2 = OAM2_PAL(MAPANIMFX_PAL) +
					OAM2_LAYER(0b01) +
					OAM2_CHR(chr);

	PutSprite(4, OAM1_X(ix), OAM0_Y(iy) + ATTR0_TYPE_BLENDED, gObject_16x16, oam2);

	if (++proc->timer == 0x44)
		Proc_Break(proc);
}

const struct ProcCmd ProcScr_MapAnimSkillfx[] = {
	PROC_YIELD,
	PROC_CALL(MapAnimSkillfx_Initfx),
	PROC_REPEAT(MapAnimSkillfx_DrawIconIDLE),
	PROC_END
};
