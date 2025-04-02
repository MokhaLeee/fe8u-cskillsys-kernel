#include "common-chax.h"

enum { MAX_UDEFS_ON_ONE_LOAD = 20 };

LYN_REPLACE_CHECK(GetLoadUnitsAmount);
u16 GetLoadUnitsAmount(const struct UnitDefinition *unitDefinition)
{
	u16 result = 0;

#if CHAX
	if (!unitDefinition)
		return 0;
#endif

	while (unitDefinition->charIndex) {
		++result;
		++unitDefinition;
	}

#if CHAX
	/**
	 * This is a detection on: FreeDemoRamSpaceTop
	 * FreeDemoRamSpaceTop = gLoadUnitBuffer + 0x14 * count
	 *
	 * But actually this is a bug fix LOL
	 */
	if (result > MAX_UDEFS_ON_ONE_LOAD)
		result = MAX_UDEFS_ON_ONE_LOAD;
#endif

	return result;
}

/* external call */
struct UnitDefinition *CHAX_GetUnitDefinitionFormEventScr(struct UnitDefinition *source, short count, u8 arg2, s8 arg3, s8 arg4)
{
	if (count > MAX_UDEFS_ON_ONE_LOAD)
		count = MAX_UDEFS_ON_ONE_LOAD;

	return GetUnitDefinitionFormEventScr(source, count, arg2, arg3, arg4);
}
