#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"

extern u16 *CONST_DATA sPoisonIconSprites[12];
extern u16 *CONST_DATA sSleepIconSprites[7];
extern u16 *CONST_DATA sBerserkIconSprites[9];
extern u16 *CONST_DATA sSilenceIconSprites[18];

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

void PutUnitPoisonDebuffOAM(struct Unit *unit, int x, int y)
{
	int frame = k_umod(GetGameClock() >> 3, ARRAY_COUNT(sPoisonIconSprites));

	CallARM_PushToSecondaryOAM(
		OAM1_X(0x200 + x - 2),
		OAM0_Y(0x100 + y - 4),
		sPoisonIconSprites[frame],
		0);
}

void PutUnitSleepDebuffOAM(struct Unit *unit, int x, int y)
{
	int frame = k_umod(GetGameClock() / 16, ARRAY_COUNT(sSleepIconSprites));

	CallARM_PushToSecondaryOAM(
		OAM1_X(0x200 + x + 2),
		OAM0_Y(0x100 + y + 0),
		sSleepIconSprites[frame],
		0);
}

void PutUnitBerserkDebuffOAM(struct Unit *unit, int x, int y)
{
	int frame = k_umod(GetGameClock() / 8, ARRAY_COUNT(sBerserkIconSprites));

	CallARM_PushToSecondaryOAM(
		OAM1_X(0x200 + x + 1),
		OAM0_Y(0x100 + y - 5),
		sBerserkIconSprites[frame],
		0);
}

void PutUnitSilenceDebuffOAM(struct Unit *unit, int x, int y)
{
	int frame = k_umod(GetGameClock() / 4, ARRAY_COUNT(sSilenceIconSprites));

	CallARM_PushToSecondaryOAM(
		OAM1_X(0x200 + x - 2),
		OAM0_Y(0x100 + y - 4),
		sSilenceIconSprites[frame],
		0);
}
