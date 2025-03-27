#include "common-chax.h"
#include "kernel-lib.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"

bool PrePhaseFunc_AversaNight(ProcPtr proc)
{
#if defined(SID_AversaNight) && (COMMON_SKILL_VALID(SID_AversaNight))
	int max_hp = 0;
	bool AversaNight_eff = false;

	FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
		if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

		if (SkillTester(unit, SID_AversaNight)) {
			AversaNight_eff = true;

			if (max_hp > GetUnitMaxHp(unit))
				max_hp = GetUnitMaxHp(unit);
		}
	})

	if (AversaNight_eff == true) {
		u32 i, tuid;
		struct Unit *tunit;

		for (tuid = 1; tuid < 0xC0; tuid++) {
			if (AreUnitsAllied(tuid, gPlaySt.faction + 1))
				continue;

			tunit = GetUnit(tuid);
			if (!UNIT_IS_VALID(tunit))
				continue;

			if (tunit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
				continue;

			if (tunit->curHP >= (max_hp - SKILL_EFF0(SID_AversaNight)))
				continue;

			for (i = 0; i < ARRAY_COUNT_RANGE2x2; i++) {
				int x = tunit->xPos + gVecs_2x2[i].x;
				int y = tunit->yPos + gVecs_2x2[i].y;
				struct Unit *tunit2 = GetUnitAtPosition(x, y);

				if (!tunit2)
					continue;

				if (tunit2->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
					continue;

				if (AreUnitsAllied(tunit->index, tunit2->index)) {
					SetUnitStatusIndex(tunit, NEW_UNIT_STATUS_PANIC);
					SetUnitStatDebuff(tunit, UNIT_STAT_DEBUFF_AversaNight);
					break;
				}
			}
		}
	}
#endif

	return false;
}
