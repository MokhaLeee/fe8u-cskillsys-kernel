#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void _SetOathStatDebuf(struct Unit *unit)
{
#if (defined(SID_OathStrength) && COMMON_SKILL_VALID(SID_OathStrength))
	if (SkillTester(unit, SID_OathStrength))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_POW);
#endif

#if (defined(SID_OathMagic) && COMMON_SKILL_VALID(SID_OathMagic))
	if (SkillTester(unit, SID_OathMagic))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_MAG);
#endif

#if (defined(SID_OathSkill) && COMMON_SKILL_VALID(SID_OathSkill))
	if (SkillTester(unit, SID_OathSkill))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_SKL);
#endif

#if (defined(SID_OathSpeed) && COMMON_SKILL_VALID(SID_OathSpeed))
	if (SkillTester(unit, SID_OathSpeed))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_SPD);
#endif

#if (defined(SID_OathLuck) && COMMON_SKILL_VALID(SID_OathLuck))
	if (SkillTester(unit, SID_OathLuck))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_LCK);
#endif

#if (defined(SID_OathDefense) && COMMON_SKILL_VALID(SID_OathDefense))
	if (SkillTester(unit, SID_OathDefense))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_DEF);
#endif

#if (defined(SID_OathResistance) && COMMON_SKILL_VALID(SID_OathResistance))
	if (SkillTester(unit, SID_OathResistance))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_RES);
#endif

#if (defined(SID_OathMovement) && COMMON_SKILL_VALID(SID_OathMovement))
	if (SkillTester(unit, SID_OathMovement))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_MOV);
#endif
}

static void _SetRouseStatDebuf(struct Unit *unit)
{
#if (defined(SID_RouseStrength) && COMMON_SKILL_VALID(SID_RouseStrength))
	if (SkillTester(unit, SID_RouseStrength))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_POW);
#endif

#if (defined(SID_RouseMagic) && COMMON_SKILL_VALID(SID_RouseMagic))
	if (SkillTester(unit, SID_RouseMagic))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_MAG);
#endif

#if (defined(SID_RouseSkill) && COMMON_SKILL_VALID(SID_RouseSkill))
	if (SkillTester(unit, SID_RouseSkill))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_SKL);
#endif

#if (defined(SID_RouseSpeed) && COMMON_SKILL_VALID(SID_RouseSpeed))
	if (SkillTester(unit, SID_RouseSpeed))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_SPD);
#endif

#if (defined(SID_RouseLuck) && COMMON_SKILL_VALID(SID_RouseLuck))
	if (SkillTester(unit, SID_RouseLuck))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_LCK);
#endif

#if (defined(SID_RouseDefense) && COMMON_SKILL_VALID(SID_RouseDefense))
	if (SkillTester(unit, SID_RouseDefense))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_DEF);
#endif

#if (defined(SID_RouseResistance) && COMMON_SKILL_VALID(SID_RouseResistance))
	if (SkillTester(unit, SID_RouseResistance))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_RES);
#endif

#if (defined(SID_RouseMovement) && COMMON_SKILL_VALID(SID_RouseMovement))
	if (SkillTester(unit, SID_RouseMovement))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_OATHROUSE_MOV);
#endif
}

bool PrePhsae_TickOathRouthSkillStatus(ProcPtr proc)
{
	int j;

	FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
		bool ally_in_range = false;

		for (j = 0; j < ARRAY_COUNT_RANGE1x1; j++) {
			int _x = unit->xPos + gVecs_1x1[j].x;
			int _y = unit->yPos + gVecs_1x1[j].y;
			struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

			if (!UNIT_IS_VALID(unit_ally))
				continue;

			if (unit_ally->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
				continue;

			if (AreUnitsAllied(unit->index, unit_ally->index)) {
				ally_in_range = true;
				break;
			}
		}

		if (ally_in_range)
			_SetOathStatDebuf(unit);
		else
			_SetRouseStatDebuf(unit);
	})
	return false;
}
