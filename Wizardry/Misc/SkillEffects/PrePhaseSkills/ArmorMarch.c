#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "class-types.h"
#include "skill-system.h"
#include "constants/skills.h"

FORCE_DECLARE static void _SetArmorMarchStatDebuff(struct Unit *unit)
{
	SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ARMOR_MARCH);
}

bool PrePhase_TickArmorMarchSkillStatus(ProcPtr proc)
{
#if (defined(SID_ArmorMarch) && (COMMON_SKILL_VALID(SID_ArmorMarch)))
	int j;

	FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
		if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

		if (!SkillTester(unit, SID_ArmorMarch))
			continue;

		if (!CheckClassArmor(UNIT_CLASS_ID(unit)))
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

			if (CheckClassArmor(UNIT_CLASS_ID(unit_ally))) {
				_SetArmorMarchStatDebuff(unit);
				_SetArmorMarchStatDebuff(unit_ally);
			}
		}
	})
#endif
	return false;
}
