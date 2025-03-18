#include <common-chax.h>
#include <status-getter.h>

const struct UnitMaxStatusConfig gUnitMaxStatusConfigTable[0x100] = {
	[CHARACTER_EIRIKA] = {
		.pow = -2,
		.mag = -2,
		.spd = 5,
		.skl = 5,
		.lck = 5,
		.def = -2,
		.res = -2,
	},
};
