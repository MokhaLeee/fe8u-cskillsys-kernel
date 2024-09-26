#include "common-chax.h"
#include "debuff.h"

/* External on-draw functions */
void PutUnitDanceRingBuffIcon(struct Unit *unit, int ix, int iy)
{
	const u16 obj[] = {
		2,
		0x4000, 0x0000, OAM2_PAL(0x1) + OAM2_LAYER(0x2) + OAM2_CHR(0x600 / 0x20),
		0x0008, 0x0000, OAM2_PAL(0x1) + OAM2_LAYER(0x2) + OAM2_CHR(0xA00 / 0x20),
	};

	CallARM_PushToSecondaryOAM(
		OAM1_X(0x200 + ix - 1),
		OAM0_Y(0x100 + iy - 5),
		obj,
		0);
}
