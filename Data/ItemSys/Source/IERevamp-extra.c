#include "common-chax.h"
#include "item-sys.h"
#include "constants/texts.h"

const struct IER_PromoConfig IER_PromotionItemTable[] = {
	{
		.item = ITEM_HEROCREST,
		.job_list = gItemUseJidList_HeroCrest
	},
	{
		.item = ITEM_KNIGHTCREST,
		.job_list = gItemUseJidList_KnightCrest,
	},
	{
		.item = ITEM_ORIONSBOLT,
		.job_list = gItemUseJidList_OrionsBolt,
	},
	{
		.item = ITEM_ELYSIANWHIP,
		.job_list = gItemUseJidList_ElysianWhip,
	},
	{
		.item = ITEM_GUIDINGRING,
		.job_list = gItemUseJidList_GuidRing,
	},
	{
		.item = ITEM_MASTERSEAL,
		.job_list = gItemUseJidList_MasterSeal,
	},
	{
		.item = ITEM_OCEANSEAL,
		.job_list = gItemUseJidList_OceanSeal,
	},
	{
		.item = ITEM_LUNARBRACE,
		.job_list = gItemUseJidList_LunarBrace,
	},
	{
		.item = ITEM_SOLARBRACE,
		.job_list = gItemUseJidList_SolarBrace,
	},
	{
		.item = ITEM_UNK_C1,
		.job_list = gItemUseJidList_C1,
	},

	// end
	// { 0 }
	{
		.item = 0xFFFF // WTF
	},
};

struct IER_PrepStatBoosterMsg const IER_StatBoosterTextTable[] = {
	{
		.item = ITEM_BOOSTER_HP,
		.msg  = MSG_001C,
	},
	{
		.item = ITEM_BOOSTER_POW,
		.msg  = MSG_0013,
	},
	{
		.item = CONFIG_ITEM_INDEX_MAG_BOOSTER,
		.msg  = MSG_0014,
	},
	{
		.item = ITEM_BOOSTER_SKL,
		.msg  = MSG_0015,
	},
	{
		.item = ITEM_BOOSTER_SPD,
		.msg  = MSG_0016,
	},
	{
		.item = ITEM_BOOSTER_LCK,
		.msg  = MSG_0017,
	},
	{
		.item = ITEM_BOOSTER_DEF,
		.msg  = MSG_0018,
	},
	{
		.item = ITEM_BOOSTER_RES,
		.msg  = MSG_0019,
	},
	{
		.item = ITEM_BOOSTER_MOV,
		.msg  = MSG_001A,
	},
	{
		.item = ITEM_BOOSTER_CON,
		.msg  = MSG_001B,
	},
	{
		.item = ITEM_METISSTOME,
		.msg  = MSG_001D,
	},
	{}
};

/**
 * Useless actually
 */
const u8 IER_SelfHealItemEffectList[] = {
	IER_VULNERARY,
	IER_ELIXIR,
	IER_VULNERARY_2,

	IER_INVALID
};

/**
 * Useless actually
 */
const u8 IER_ChestKeyEffectList[] = {
	IER_CHESTKEY,
	IER_LOCKPICK,
	IER_CHESTKEY_BUNDLE,

	IER_INVALID
};

/**
 * Useless actually
 */
const u8 IER_PromoEffectList[] = {
	IER_HEROCREST,
	IER_KNIGHTCREST,
	IER_ORIONSBOLT,
	IER_ELYSIANWHIP,
	IER_GUIDINGRING,
	IER_MASTERSEAL,
	IER_OCEANSEAL,
	IER_LUNARBRACE,
	IER_SOLARBRACE,

	IER_INVALID
};
