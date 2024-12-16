#include <common-chax.h>
#include <kernel-tutorial.h>

#include "Reorganize.h"

u8 ItemSubMenu_ReorganizeUsability(const struct MenuItemDef *def, int number)
{
	if (number == 0)
		return MENU_NOTSHOWN;

	return MENU_ENABLED;
}

u8 ItemSubMenu_ReorganizeEffect(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	EquipUnitItemSlot(gActiveUnit, gActionData.itemSlotIndex);
	return sub_8023550(menu);
}
