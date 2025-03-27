#include <common-chax.h>
#include <kernel-lib.h>

bool UnitAvaliable(struct Unit *unit)
{
	return (UNIT_IS_VALID(unit) && !(unit->state & US_UNAVAILABLE));
}

bool UnitOnMapAvaliable(struct Unit *unit)
{
	return (UNIT_IS_VALID(unit) && !(unit->state & (US_UNAVAILABLE | US_HIDDEN)));
}

bool IsUnitStruct(struct Unit *ptr)
{
	if (!IS_IWRAM_PTR(ptr) && !IS_EWRAM_PTR(ptr))
		return false;

	if (!IS_ROM_PTR(ptr->pCharacterData) || !IS_ROM_PTR(ptr->pClassData))
		return false;

	return true;
}
