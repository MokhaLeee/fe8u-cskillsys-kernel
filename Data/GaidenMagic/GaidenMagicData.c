#include <common-chax.h>
#include <gaiden-magic.h>
#include <skill-system.h>
#include <constants/skills.h>

const u8 gGaidenWeaponCostList[0x100] = {
	[ITEM_SWORD_KILLER] = 2,
	[ITEM_ANIMA_FIRE] = 2,
	[ITEM_ANIMA_THUNDER] = 2,
	[ITEM_DARK_FLUX] = 2,
	[ITEM_DARK_LUNA] = 3,
	[ITEM_DARK_NOSFERATU] = 2,
	[ITEM_STAFF_HEAL] = 2,
	[ITEM_STAFF_PHYSIC] = 5,
	[ITEM_STAFF_SLEEP] = 5,
	[ITEM_STAFF_LATONA] = 10,
};

struct GaidenPinfoConfig const gGaidenPinfoConfigList[0x100] = {
	[CHARACTER_EIRIKA] = { .ent = (const struct GaidenPinfoConfigEnt []) {
		{ 1, ITEM_SWORD_KILLER },
		{ 0 }
	}},
	[CHARACTER_NATASHA] = { .ent = (const struct GaidenPinfoConfigEnt []) {
		{ 1, ITEM_STAFF_HEAL },
		{ 1, ITEM_DARK_LUNA },
		{ 1, ITEM_STAFF_SLEEP },
		{ 0 }
	}},
	[CHARACTER_SALEH] = { .ent = (const struct GaidenPinfoConfigEnt []) {
		{ 1, ITEM_DARK_NOSFERATU },
		{ 1, ITEM_STAFF_PHYSIC },
		{ 0 }
	}},
	[CHARACTER_SELENA] = { .ent = (const struct GaidenPinfoConfigEnt []) {
		{ 1, ITEM_STAFF_PHYSIC },
		{ 0 }
	}},
	[CHARACTER_NOVALA] = { .ent = (const struct GaidenPinfoConfigEnt []) {
		{ 1, ITEM_DARK_FLUX },
		{ 0 }
	}},
	[CHARACTER_RIEV] = { .ent = (const struct GaidenPinfoConfigEnt []) {
		{ 1, ITEM_STAFF_SLEEP },
		{ 0 }
	}},
};

// This config is valid only when CONFIG_GAIDEN_EXT_CONF_EN is set!
const struct GaidenChaxConfigEnt gGaidenChaxConfigs[] = {
#if (defined(SID_Latona) && COMMON_SKILL_VALID(SID_Latona))
	{
		.iid = ITEM_STAFF_LATONA,
		.skill = SID_Latona,
	},
#endif
	{ 0 },
};
