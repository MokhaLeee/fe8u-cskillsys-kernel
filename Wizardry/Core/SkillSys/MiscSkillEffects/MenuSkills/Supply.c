#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(SupplyUsability);
u8 SupplyUsability(const struct MenuItemDef *def, int number)
{
	int i;

#if 0
	if (!HasConvoyAccess())
		return MENU_NOTSHOWN;
#endif

	if (UNIT_CLASS_ID(gActiveUnit) == CLASS_PHANTOM)
		return MENU_NOTSHOWN;

#if defined(SID_Supply) && (COMMON_SKILL_VALID(SID_Supply))
	if (SkillTester(gActiveUnit, SID_Supply))
#else
	if (0)
#endif
		return MENU_ENABLED;

	for (i = 0; i < ARRAY_COUNT_RANGE1x1; i++) {
		struct Unit *unit = GetUnitAtPosition(
			gActiveUnit->xPos + gVecs_1x1[i].x,
			gActiveUnit->yPos + gVecs_1x1[i].y
		);

		if (!UNIT_IS_VALID(unit) || !AreUnitsAllied(gActiveUnit->index, unit->index))
			continue;

#if defined(SID_Supply) && (COMMON_SKILL_VALID(SID_Supply))
		if (SkillTester(unit, SID_Supply))
#else
		if (0)
#endif
			return MENU_ENABLED;
	}
	return MENU_NOTSHOWN;
}
