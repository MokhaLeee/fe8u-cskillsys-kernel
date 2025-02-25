#include <common-chax.h>
#include <shield.h>

struct ShieldExtEquipableConfig const gShieldExtEquipableConfig[] = {
	{ 0 }
};

struct ShieldItem const gShieldItemTable[0x100] = {
	[ITEM_HOPLON_SHIELD] = {
		.en = true,

		.info = {
			.iid = ITEM_HOPLON_SHIELD,
			.consume_on_defense = true,
			.consume_on_hitted  = true,
			.battle_status_bonus = { [BATTLE_STATUS_DEF] = 2 },
			.unit_status_bonus = {
				[UNIT_STATUS_DEF] = 3,
				[UNIT_STATUS_RES] = 3,
			},
			.unit_growth_bonus = {
				[UNIT_STATUS_DEF] = 10,
				[UNIT_STATUS_RES] = 10,
			},
		},
	},

#ifdef CONFIG_USE_DEBUG
	[ITEM_ELIXIR] = {
		.en = true,

		.info = {
			.iid = ITEM_ELIXIR,
			.consume_on_attack = true,
			.consume_on_hitted = true,
			.battle_status_bonus = { [BATTLE_STATUS_ATK] = 2 },
		},
	},
	[ITEM_VULNERARY] = {
		.en = true,

		.info = {
			.iid = ITEM_VULNERARY,
			.consume_on_attack = true,
			.consume_on_hitted = true,
			.battle_status_bonus = { [BATTLE_STATUS_ATK] = 2 },
		},
	},
#endif
};
