#include "common-chax.h"
#include "item-sys.h"

LYN_REPLACE_CHECK(CanUnitUseItemPrepScreen);
bool CanUnitUseItemPrepScreen(struct Unit *unit, int item)
{
	PrepItemUsabilityFunc_t it = gpPrepItemUsabilityFuncs[ITEM_INDEX(item)];

	if (it)
		return it(unit, item);

	return false;
}

bool CanUnitUseMetiStome(struct Unit *unit, int item)
{
	if (unit->state & US_GROWTH_BOOST)
		return false;

	return true;
}
