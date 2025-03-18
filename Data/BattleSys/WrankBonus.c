#include "common-chax.h"
#include "battle-system.h"

struct WrankBonusConfEnt const gWrankBonusConf[] = {
	{
		.wtype = ITYPE_SWORD,
		.wrank = WPN_LEVEL_D,
		.bonus = {
			[BATTLE_STATUS_HIT] = 3,
		},
	},
	{
		.wtype = ITYPE_SWORD,
		.wrank = WPN_LEVEL_C,
		.bonus = {
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_SWORD,
		.wrank = WPN_LEVEL_B,
		.bonus = {
			[BATTLE_STATUS_ATK] = 1,
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_SWORD,
		.wrank = WPN_LEVEL_A,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
		},
	},
	{
		.wtype = ITYPE_SWORD,
		.wrank = WPN_LEVEL_S,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
			[BATTLE_STATUS_CRIT] = 10,
			[BATTLE_STATUS_SILENCER] = 5,
		},
	},
	{
		.wtype = ITYPE_LANCE,
		.wrank = WPN_LEVEL_D,
		.bonus = {
			[BATTLE_STATUS_HIT] = 3,
		},
	},
	{
		.wtype = ITYPE_LANCE,
		.wrank = WPN_LEVEL_C,
		.bonus = {
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_LANCE,
		.wrank = WPN_LEVEL_B,
		.bonus = {
			[BATTLE_STATUS_ATK] = 1,
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_LANCE,
		.wrank = WPN_LEVEL_A,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
		},
	},
	{
		.wtype = ITYPE_LANCE,
		.wrank = WPN_LEVEL_S,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
			[BATTLE_STATUS_CRIT] = 10,
			[BATTLE_STATUS_SILENCER] = 5,
		},
	},
	{
		.wtype = ITYPE_AXE,
		.wrank = WPN_LEVEL_D,
		.bonus = {
			[BATTLE_STATUS_HIT] = 3,
		},
	},
	{
		.wtype = ITYPE_AXE,
		.wrank = WPN_LEVEL_C,
		.bonus = {
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_AXE,
		.wrank = WPN_LEVEL_B,
		.bonus = {
			[BATTLE_STATUS_ATK] = 1,
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_AXE,
		.wrank = WPN_LEVEL_A,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
		},
	},
	{
		.wtype = ITYPE_AXE,
		.wrank = WPN_LEVEL_S,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
			[BATTLE_STATUS_CRIT] = 10,
			[BATTLE_STATUS_SILENCER] = 5,
		},
	},
	{
		.wtype = ITYPE_BOW,
		.wrank = WPN_LEVEL_D,
		.bonus = {
			[BATTLE_STATUS_HIT] = 3,
		},
	},
	{
		.wtype = ITYPE_BOW,
		.wrank = WPN_LEVEL_C,
		.bonus = {
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_BOW,
		.wrank = WPN_LEVEL_B,
		.bonus = {
			[BATTLE_STATUS_ATK] = 1,
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_BOW,
		.wrank = WPN_LEVEL_A,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
		},
	},
	{
		.wtype = ITYPE_BOW,
		.wrank = WPN_LEVEL_S,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
			[BATTLE_STATUS_CRIT] = 10,
			[BATTLE_STATUS_SILENCER] = 5,
		},
	},
	{
		.wtype = ITYPE_ANIMA,
		.wrank = WPN_LEVEL_D,
		.bonus = {
			[BATTLE_STATUS_HIT] = 3,
		},
	},
	{
		.wtype = ITYPE_ANIMA,
		.wrank = WPN_LEVEL_C,
		.bonus = {
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_ANIMA,
		.wrank = WPN_LEVEL_B,
		.bonus = {
			[BATTLE_STATUS_ATK] = 1,
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_ANIMA,
		.wrank = WPN_LEVEL_A,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
		},
	},
	{
		.wtype = ITYPE_ANIMA,
		.wrank = WPN_LEVEL_S,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
			[BATTLE_STATUS_CRIT] = 10,
			[BATTLE_STATUS_SILENCER] = 5,
		},
	},
	{
		.wtype = ITYPE_LIGHT,
		.wrank = WPN_LEVEL_D,
		.bonus = {
			[BATTLE_STATUS_HIT] = 3,
		},
	},
	{
		.wtype = ITYPE_LIGHT,
		.wrank = WPN_LEVEL_C,
		.bonus = {
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_LIGHT,
		.wrank = WPN_LEVEL_B,
		.bonus = {
			[BATTLE_STATUS_ATK] = 1,
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_LIGHT,
		.wrank = WPN_LEVEL_A,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
		},
	},
	{
		.wtype = ITYPE_LIGHT,
		.wrank = WPN_LEVEL_S,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
			[BATTLE_STATUS_CRIT] = 10,
			[BATTLE_STATUS_SILENCER] = 5,
		},
	},
	{
		.wtype = ITYPE_DARK,
		.wrank = WPN_LEVEL_D,
		.bonus = {
			[BATTLE_STATUS_HIT] = 3,
		},
	},
	{
		.wtype = ITYPE_DARK,
		.wrank = WPN_LEVEL_C,
		.bonus = {
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_DARK,
		.wrank = WPN_LEVEL_B,
		.bonus = {
			[BATTLE_STATUS_ATK] = 1,
			[BATTLE_STATUS_HIT] = 5,
		},
	},
	{
		.wtype = ITYPE_DARK,
		.wrank = WPN_LEVEL_A,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
		},
	},
	{
		.wtype = ITYPE_DARK,
		.wrank = WPN_LEVEL_S,
		.bonus = {
			[BATTLE_STATUS_ATK] = 3,
			[BATTLE_STATUS_HIT] = 10,
			[BATTLE_STATUS_CRIT] = 10,
			[BATTLE_STATUS_SILENCER] = 5,
		},
	},
	{}
};
