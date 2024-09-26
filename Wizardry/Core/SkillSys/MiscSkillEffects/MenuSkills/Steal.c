#include "common-chax.h"
#include "skill-system.h"

LYN_REPLACE_CHECK(StealCommandUsability);
u8 StealCommandUsability(const struct MenuItemDef *def, int number)
{
#if !CHAX
	if (!(UNIT_CATTRIBUTES(gActiveUnit) & CA_STEAL))
		return MENU_NOTSHOWN;
#endif

	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	MakeTargetListForSteal(gActiveUnit);
	if (GetSelectTargetCount() == 0)
		return MENU_NOTSHOWN;

	if (GetUnitItemCount(gActiveUnit) == UNIT_ITEM_COUNT)
		return MENU_DISABLED;

	return MENU_ENABLED;
}
