#include <common-chax.h>
#include <gaiden-magic.h>

const u8 gGaidenWeaponCostList[0x100] = {
	[ITEM_SWORD_KILLER] = 2,
	[ITEM_ANIMA_FIRE] = 2,
};

struct GaidenPinfoConfigEnt const *const gGaidenPinfoConfigList[] = {
	[CHARACTER_EIRIKA] = (const struct GaidenPinfoConfigEnt []) {
		{ 1, ITEM_SWORD_KILLER },
		{ 0 }
	},
};

const struct GaidenChaxConfigEnt gGaidenChaxConfigs[] = {
	{
		.iid = ITEM_ANIMA_FIRE,
	},
	{
		.iid = ITEM_DARK_NOSFERATU,
		.evflag = 101, // lol
	},
	{
		.iid = ITEM_ANIMA_THUNDER,
		.pid = CHARACTER_EIRIKA,
		.jid = CLASS_EIRIKA_LORD,
		.level = 1,
	},
	{ 0 },
};
