#include "common-chax.h"
#include "skill-system.h"
#include "unit-expa.h"

/* Only one lagendary skill is valid */
STATIC_DECLAR u8 GetUnitLegendarySkill(struct Unit *unit)
{
	u16 sid;
	int i;

	for (i = 0; ; i++) {
		sid = gpLegendSkillPool[i];
		if (!COMMON_SKILL_VALID(sid))
			break;

		if (SkillTester(unit, sid))
			return sid;
	}
	return 0;
}

int TryActivateLegendSkill(struct Unit *unit, const u16 sid)
{
	/**
	 * Legend skill can only be
	 * activated once per chapter
	 */
	if (sid != GetUnitLegendarySkill(unit))
		return -1;

	if (CheckBitUES(unit, UES_BIT_LEGENDARY_SKILL_USED))
		return -1;

	SetBitUES(unit, UES_BIT_LEGENDARY_SKILL_USED);
	SetBitUES(unit, UES_BIT_LEGENDARY_SKILL_ACTIVE);
	return 0;
}

bool SkillTesterLegendActivated(struct Unit *unit, const u16 sid)
{
	if (sid != GetUnitLegendarySkill(unit))
		return false;

	if (!CheckBitUES(unit, UES_BIT_LEGENDARY_SKILL_ACTIVE))
		return false;

	return true;
}

bool PrePhase_UpdateLengendSkillStatus(ProcPtr proc)
{
	/**
	 * Legend skill can only be activated for one turn
	 */
	FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
		if (UNIT_IS_VALID(unit))
			ClearBitUES(unit, UES_BIT_LEGENDARY_SKILL_ACTIVE);
	})
	return false;
}
