#include "common-chax.h"
#include "skill-system.h"

LYN_REPLACE_CHECK(DanceCommandUsability);
u8 DanceCommandUsability(const struct MenuItemDef *def, int number)
{
#if !CHAX
	if (!(UNIT_CATTRIBUTES(gActiveUnit) & CA_DANCE))
		return MENU_NOTSHOWN;
#endif

	gBmSt.um_tmp_item = ITEM_DANCE;
	return sub_80230F0(def);
}
