#include "common-chax.h"
#include "strmag.h"
#include "constants/texts.h"

const struct ItemData ItemData_MagBooster = {
	.nameTextId = MSG_ITEM_MAGBOOSTER_NAME,
	.descTextId = MSG_ITEM_MAGBOOSTER_DESC,
	.useDescTextId = MSG_ITEM_MAGBOOSTER_USEDESC,
	.number = CONFIG_ITEM_INDEX_MAG_BOOSTER,
	.weaponType = ITYPE_ITEM,
	.pStatBonuses = &ItemBonus_MagBooster,
	.maxUses = 1,
	.encodedRange = 0x11,
	.costPerUse = 8000,
	.iconId = CONFIG_ICON_INDEX_MAG_BOOSTER,
	.useEffectId = 17,
};
