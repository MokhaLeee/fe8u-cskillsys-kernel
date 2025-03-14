#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void _SetKeepUpStatDebuff(struct Unit *unit)
{
	SetUnitStatDebuff(unit, UNIT_STAT_BUFF_KEEPUP);
}

bool PrePhase_TickKeepUpSkillStatus(ProcPtr proc)
{
	int j;

	FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
		FORCE_DECLARE bool keep_up  = false;

		if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

#if (defined(SID_KeepUp) && (COMMON_SKILL_VALID(SID_KeepUp)))
		if (SkillTester(unit, SID_KeepUp))
			keep_up = true;
#endif

		for (j = 0; j < ARRAY_COUNT_RANGE3x3; j++) {
			int _x = unit->xPos + gVecs_3x3[j].x;
			int _y = unit->yPos + gVecs_3x3[j].y;
			struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

			if (!UNIT_IS_VALID(unit_ally))
				continue;

			if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
				continue;

			if (!AreUnitsAllied(unit->index, unit_ally->index))
				continue;

			if
			(
#if (defined(SID_Canto) && (COMMON_SKILL_VALID(SID_Canto)))
				(SkillTester(unit_ally, SID_Canto) && keep_up)
#else
				false
#endif
				||
#if (defined(SID_CantoPlus) && (COMMON_SKILL_VALID(SID_CantoPlus)))
				(SkillTester(unit_ally, SID_CantoPlus) && keep_up)
#else
				false
#endif
			)
				_SetKeepUpStatDebuff(unit);
		}
	})
	return false;
}
