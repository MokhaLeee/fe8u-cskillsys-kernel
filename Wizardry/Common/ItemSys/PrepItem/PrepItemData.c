#include "common-chax.h"
#include "item-sys.h"

PrepItemEffectFunc_t const gPrepItemEffectFunc[0x100] = {
	[ITEM_HEAVENSEAL]  = PrepItemEffect_Promotion,
	[ITEM_HEROCREST]   = PrepItemEffect_Promotion,
	[ITEM_KNIGHTCREST] = PrepItemEffect_Promotion,
	[ITEM_ORIONSBOLT]  = PrepItemEffect_Promotion,
	[ITEM_ELYSIANWHIP] = PrepItemEffect_Promotion,
	[ITEM_GUIDINGRING] = PrepItemEffect_Promotion,
	[ITEM_MASTERSEAL]  = PrepItemEffect_Promotion,
	[ITEM_OCEANSEAL]   = PrepItemEffect_Promotion,
	[ITEM_LUNARBRACE]  = PrepItemEffect_Promotion,
	[ITEM_SOLARBRACE]  = PrepItemEffect_Promotion,
	[ITEM_UNK_C1]      = PrepItemEffect_Promotion,

	[ITEM_JUNAFRUIT] = PrepItemEffect_JunaFruit,

	[ITEM_BOOSTER_HP]  = PrepItemEffect_StatBooster,
	[ITEM_BOOSTER_POW] = PrepItemEffect_StatBooster,
	[ITEM_BOOSTER_SKL] = PrepItemEffect_StatBooster,
	[ITEM_BOOSTER_SPD] = PrepItemEffect_StatBooster,
	[ITEM_BOOSTER_LCK] = PrepItemEffect_StatBooster,
	[ITEM_BOOSTER_DEF] = PrepItemEffect_StatBooster,
	[ITEM_BOOSTER_RES] = PrepItemEffect_StatBooster,
	[ITEM_BOOSTER_MOV] = PrepItemEffect_StatBooster,
	[ITEM_BOOSTER_CON] = PrepItemEffect_StatBooster,
};

PrepItemUsabilityFunc_t const gPrepItemUsabilityFuncs[0x100] = {
	[ITEM_BOOSTER_HP]  = CanUnitUseStatGainItem,
	[ITEM_BOOSTER_POW] = CanUnitUseStatGainItem,
	[ITEM_BOOSTER_SKL] = CanUnitUseStatGainItem,
	[ITEM_BOOSTER_SPD] = CanUnitUseStatGainItem,
	[ITEM_BOOSTER_LCK] = CanUnitUseStatGainItem,
	[ITEM_BOOSTER_DEF] = CanUnitUseStatGainItem,
	[ITEM_BOOSTER_RES] = CanUnitUseStatGainItem,
	[ITEM_BOOSTER_MOV] = CanUnitUseStatGainItem,
	[ITEM_BOOSTER_CON] = CanUnitUseStatGainItem,

	[ITEM_HEROCREST]   = CanUnitUsePromotionItem,
	[ITEM_KNIGHTCREST] = CanUnitUsePromotionItem,
	[ITEM_ORIONSBOLT]  = CanUnitUsePromotionItem,
	[ITEM_ELYSIANWHIP] = CanUnitUsePromotionItem,
	[ITEM_GUIDINGRING] = CanUnitUsePromotionItem,
	[ITEM_MASTERSEAL]  = CanUnitUsePromotionItem,
	[ITEM_HEAVENSEAL]  = CanUnitUsePromotionItem,
	[ITEM_OCEANSEAL]   = CanUnitUsePromotionItem,
	[ITEM_LUNARBRACE]  = CanUnitUsePromotionItem,
	[ITEM_SOLARBRACE]  = CanUnitUsePromotionItem,
	[ITEM_UNK_C1]      = CanUnitUsePromotionItem,

	[ITEM_METISSTOME] = CanUnitUseMetiStome,
	[ITEM_JUNAFRUIT]  = (void *)CanUnitUseFruitItem,
};
