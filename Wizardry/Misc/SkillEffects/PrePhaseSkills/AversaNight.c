#include "common-chax.h"
#include "kernel-lib.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"

#include "pre-phase.h"

void SetupAversaNight(struct Unit *unit)
{
	/**
	 * This should lie in PrePhase loop: FOR_UNITS_FACTION
	 */
#if defined(SID_AversaNight) && (COMMON_SKILL_VALID(SID_AversaNight))

	if (SkillTester(unit, SID_AversaNight)) {
		int _max_hp = GetUnitMaxHp(unit);

		if (gPrePhaseSkillBuf.AversaNight_Mhp < _max_hp)
			gPrePhaseSkillBuf.AversaNight_Mhp = _max_hp;
	}
#endif
}

void ExecAversaNight(struct Unit *unit)
{
	/**
	 * This should lie in PrePhase loop: FOR_UNITS_ALL
	 */

#if defined(SID_AversaNight) && (COMMON_SKILL_VALID(SID_AversaNight))
	int i;

	if (unit->curHP >= (gPrePhaseSkillBuf.AversaNight_Mhp - SKILL_EFF0(SID_AversaNight)))
		return;

	for (i = 0; i < ARRAY_COUNT_RANGE2x2; i++) {
		int x = unit->xPos + gVecs_2x2[i].x;
		int y = unit->yPos + gVecs_2x2[i].y;
		struct Unit *unit2 = GetUnitAtPosition(x, y);

		if (!unit2)
			continue;

		if (unit2->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

		if (AreUnitsAllied(unit->index, unit2->index)) {
			SetUnitStatusIndex(unit, NEW_UNIT_STATUS_PANIC);
			SetUnitStatDebuff(unit, UNIT_STAT_DEBUFF_AversaNight);
			break;
		}
	}
#endif
}
