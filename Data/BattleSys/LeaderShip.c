#include "common-chax.h"
#include "battle-system.h"

const struct LeaderShipConf gLeaderShipConf = {
#ifdef CONFIG_LEADER_SHIP_EN
	.en = true,
#endif
	.cancel_out_opposing = true,
	.ally_bonus  = { 5, 3 },
	.enemy_bonus = { 5, 3 },
	.npc_bonus   = { 5, 3 },
};

const u8 gLeaderShipPConf[0x100] = {
	[CHARACTER_EIRIKA] = 1,
};

const u8 gLeaderShipJConf[0x100] = {
	[CLASS_PALADIN] = 1,
};
