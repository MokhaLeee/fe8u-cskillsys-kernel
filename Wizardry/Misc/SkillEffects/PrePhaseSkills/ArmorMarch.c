#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "class-types.h"
#include "skill-system.h"
#include "constants/skills.h"

#include "pre-phase.h"

FORCE_DECLARE static void _SetArmorMarchStatDebuff(struct Unit *unit)
{
	SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ARMOR_MARCH);
}

/**
 * Exec in common, FOR_UNITS_FACTION:
 */
void ExecArmorMarch(struct Unit *unit)
{
#if (defined(SID_ArmorMarch) && (COMMON_SKILL_VALID(SID_ArmorMarch)))
	if (SkillTester(unit, SID_ArmorMarch) && CheckClassArmor(UNIT_CLASS_ID(unit))) {
		int j;

		for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++) {
			int _x = unit->xPos + gVecs_1x1[j].x;
			int _y = unit->yPos + gVecs_1x1[j].y;

			struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

			if (!UNIT_ALIVE(unit_ally))
				continue;

			if (!AreUnitsAllied(unit->index, unit_ally->index))
				continue;

			if (CheckClassArmor(UNIT_CLASS_ID(unit_ally))) {
				_SetArmorMarchStatDebuff(unit);
				_SetArmorMarchStatDebuff(unit_ally);
			}
		}
	}
#endif
}
