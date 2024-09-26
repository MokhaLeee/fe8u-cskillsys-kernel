#include "common-chax.h"
#include "map-task.h"

const struct MapTaskConf gMapTaskConfs[] = {
	[MTSKCONF_HPBAR]        = {  0,  9, gObject_16x8 },
	[MTSKCONF_COMM_BUFF]    = { -1,  0, sSprite_SysUpArrowA },
	[MTSKCONF_COMM_DEBUFF]  = { -1,  0, sSprite_SysDownArrowA },
	[MTSKCONF_RESCUING]     = {  9,  7, gObject_8x8 },
	[MTSKCONF_BOSS]         = {  9,  7, gObject_8x8 },
	[MTSKCONF_PROTECTION]   = {  9,  7, gObject_8x8 },
};

const u8 gHpBarChr[] = {
	      0x74, 0x74,
	0x56, 0x54, 0x52,
	0x36, 0x34, 0x32,
	0x16, 0x14, 0x12
};
