#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void _SetPlusAndMinusStatDebuff(struct Unit *unit)
{
	SetUnitStatDebuff(unit, UNIT_STAT_BUFF_PLUSMINUS);
}

bool PrePhsae_TickPlusAndMinusSkillStatus(ProcPtr proc)
{
	int j;

	FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
		FORCE_DECLARE bool act_plus  = false;
		FORCE_DECLARE bool act_minus = false;

		if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

#if (defined(SID_Plus) && (COMMON_SKILL_VALID(SID_Plus)))
		if (!SkillTester(unit, SID_Plus))
			act_plus = true;
#endif

#if (defined(SID_Minus) && (COMMON_SKILL_VALID(SID_Minus)))
		if (!SkillTester(unit, SID_Minus))
			act_minus = true;
#endif

		if (!act_minus && !act_plus)
			continue;

		for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++) {
			int _x = unit->xPos + gVecs_1x1[j].x;
			int _y = unit->yPos + gVecs_1x1[j].y;
			struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

			if (!UNIT_IS_VALID(unit_ally))
				continue;

			if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
				continue;

			if (!AreUnitsAllied(unit->index, unit_ally->index))
				continue;

			if
			(
#if (defined(SID_Plus) && (COMMON_SKILL_VALID(SID_Plus)))
				(SkillTester(unit_ally, SID_Plus) && act_minus)
#else
				false
#endif
				||
#if (defined(SID_Minus) && (COMMON_SKILL_VALID(SID_Minus)))
				(SkillTester(unit_ally, SID_Minus) && act_plus)
#else
				false
#endif
			) {
				_SetPlusAndMinusStatDebuff(unit);
				_SetPlusAndMinusStatDebuff(unit_ally);
			}
		}
	})
	return false;
}
