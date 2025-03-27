#include <common-chax.h>
#include <constants/texts.h>

#include "Reorganize.h"

// External replacement of: gItemSubMenuItems
const struct MenuItemDef gItemSubMenuItemsRework[] = {
	{"使う", 0x696, 0x6DB, 0, 0x34, ItemSubMenu_IsUseAvailable, 0, ItemSubMenu_UseItem, 0, 0, 0}, // Use >
	{"装備", 0x697, 0x6DA, 4, 0x35, ItemSubMenu_IsEquipAvailable, 0, ItemSubMenu_EquipItem, 0, 0, 0}, // Equip >
	{"交換", 0x698, 0x6D4, 4, 0x36, ItemSubMenu_IsTradeAvailable, 0, ItemSubMenu_TradeItem, 0, 0, 0}, // Trade >
	{"捨てる", 0x699, 0x6DC, 4, 0x37, ItemSubMenu_IsDiscardAvailable, 0, ItemSubMenu_DiscardItem, 0, 0, 0}, // Discard >
	{
		"置位",
		MSG_ItemSubOption_Reorganize_NAME,
		MSG_ItemSubOption_Reorganize_DESC,
		TEXT_COLOR_SYSTEM_GREEN,
		0x38,
		ItemSubMenu_ReorganizeUsability,
		0,
		ItemSubMenu_ReorganizeEffect,
		0, 0, 0
	},
	MenuItemsEnd
};
