#include <common-chax.h>
#include <gaiden-magic.h>

const u8 gGaidenWeaponCostList[0x100] = {
	[ITEM_SWORD_KILLER] = 2,
	[ITEM_ANIMA_FIRE] = 2,
	[ITEM_ANIMA_THUNDER] = 2,
	[ITEM_DARK_FLUX] = 2,
	[ITEM_DARK_NOSFERATU] = 2,
};

struct GaidenPinfoConfigEnt const *const gGaidenPinfoConfigList[] = {
	[CHARACTER_EIRIKA] = (const struct GaidenPinfoConfigEnt []) {
		{ 1, ITEM_SWORD_KILLER },
		{ 0 }
	},
};

const struct GaidenChaxConfigEnt gGaidenChaxConfigs[] = {
	{
		.iid = ITEM_DARK_FLUX,
		.jid = CLASS_SHAMAN,
		.faction = FACTION_RED,
	},
	{
		.iid = ITEM_DARK_NOSFERATU,
		.faction = FACTION_BLUE,
	},
	{
		.iid = ITEM_ANIMA_THUNDER,
		.pid = CHARACTER_EIRIKA,
		.jid = CLASS_EIRIKA_LORD,
		.level = 1,
		.faction = FACTION_BLUE,
	},
	{ 0 },
};
