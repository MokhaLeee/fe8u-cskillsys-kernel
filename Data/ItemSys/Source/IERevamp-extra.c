#include "common-chax.h"
#include "item-sys.h"

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
	{ 0 }
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
