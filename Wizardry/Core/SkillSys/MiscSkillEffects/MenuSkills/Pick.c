#include "common-chax.h"
#include "skill-system.h"

LYN_REPLACE_CHECK(PickCommandUsability);
u8 PickCommandUsability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

#if !CHAX
	if (gActiveUnit->pClassData->number != CLASS_ROGUE)
		return MENU_NOTSHOWN;
#endif

	MakeTargetListForPick(gActiveUnit);
	if (GetSelectTargetCount() == 0)
		return MENU_NOTSHOWN;

	return MENU_ENABLED;
}
