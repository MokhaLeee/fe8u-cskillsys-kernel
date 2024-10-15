#include "common-chax.h"

LYN_REPLACE_CHECK(SummonCommandUsability);
u8 SummonCommandUsability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	MakeTargetListForSummon(gActiveUnit);
	if (GetSelectTargetCount() == 0)
		return MENU_NOTSHOWN;

	return MENU_ENABLED;
}
