#include "common-chax.h"
#include "icon-rework.h"
#include "constants/gfx.h"

u8 const *const gAffinIcons[0x100] = {
	[UNIT_AFFIN_FIRE] = GFX_AffinIcon_Fire,
	[UNIT_AFFIN_THUNDER] = GFX_AffinIcon_Thunder,
	[UNIT_AFFIN_WIND] = GFX_AffinIcon_Wind,
	[UNIT_AFFIN_ICE] = GFX_AffinIcon_Ice,
	[UNIT_AFFIN_DARK] = GFX_AffinIcon_Dark,
	[UNIT_AFFIN_LIGHT] = GFX_AffinIcon_Light,
	[UNIT_AFFIN_ANIMA] = GFX_AffinIcon_Anima,
};

u8 const *const gMountIcons[0x100] = {
	[1] = GFX_MountIcon_Mount,
	[2] = GFX_MountIcon_Wyvern,
	[3] = GFX_MountIcon_Pegasus,
};

u8 const *const gWTypeIcons[0x100] = {
	[ITYPE_SWORD] = GFX_WtypeIcon_Sword,
	[ITYPE_AXE] = GFX_WtypeIcon_Axe,
	[ITYPE_LANCE] = GFX_WtypeIcon_Lance,
	[ITYPE_BOW] = GFX_WtypeIcon_Bow,
	[ITYPE_ANIMA] = GFX_WtypeIcon_Anima,
	[ITYPE_LIGHT] = GFX_WtypeIcon_Light,
	[ITYPE_DARK] = GFX_WtypeIcon_Dark,
	[ITYPE_STAFF] = GFX_WtypeIcon_Staff,
};
