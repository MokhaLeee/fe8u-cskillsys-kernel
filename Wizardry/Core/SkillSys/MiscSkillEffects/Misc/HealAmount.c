#include "common-chax.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

int HealAmount_SkillsGetter(int status, struct Unit *actor, struct Unit *target)
{
	FORCE_DECLARE int i, tmp;

#if (defined(SID_GentleFlower) && COMMON_SKILL_VALID(SID_GentleFlower))
	tmp = false;

	/* Judge on actor and target themself */
	if (SkillTester(actor, SID_GentleFlower) || SkillTester(target, SID_GentleFlower)) {
		tmp = true;
		goto L_SID_GentleFlower_done;
	}

	/* Judge on actor range 2x2 */
	for (i = 0; i < ARRAY_COUNT_RANGE2x2; i++) {
		int _x = actor->xPos + gVecs_2x2[i].x;
		int _y = actor->yPos + gVecs_2x2[i].y;

		struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

		if (!UNIT_IS_VALID(unit_ally))
			continue;

#if 0
		if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

		if (!AreUnitsAllied(actor->index, unit_ally->index))
			continue;
#endif

		if (SkillTester(unit_ally, SID_GentleFlower)) {
			tmp = true;
			goto L_SID_GentleFlower_done;
		}
	}

	/* Judge on target range 2x2 */
	for (i = 0; i < ARRAY_COUNT_RANGE2x2; i++) {
		int _x = target->xPos + gVecs_2x2[i].x;
		int _y = target->yPos + gVecs_2x2[i].y;

		struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

		if (!UNIT_IS_VALID(unit_ally))
			continue;

#if 0
		if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

		if (!AreUnitsAllied(target->index, unit_ally->index))
			continue;
#endif

		if (SkillTester(unit_ally, SID_GentleFlower)) {
			tmp = true;
			goto L_SID_GentleFlower_done;
		}
	}

L_SID_GentleFlower_done:
	if (tmp)
		status += status / 2;
#endif

	return status;
}
