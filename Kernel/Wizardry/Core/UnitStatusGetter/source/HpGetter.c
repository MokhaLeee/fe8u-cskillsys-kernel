#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitMaxHp(struct Unit *unit)
{
	const StatusGetterFunc_t *it;
	int status = unit->maxHP;

	for (it = gpHpGetters; *it; it++)
		status = (*it)(status, unit);

	if (gpExternalHpGetters)
		status = gpExternalHpGetters(status, unit);

	return status;
}

/* Hooks */
int HpGetterWeaponBonus(int status, struct Unit *unit)
{
	u16 weapon = GetUnitEquippedWeapon(unit);

	status += GetItemHpBonus(weapon);
	return status;
}

int HpGetterSkills(int status, struct Unit *unit)
{
#if defined(SID_HpBonus) && (COMMON_SKILL_VALID(SID_HpBonus))
	if (SkillTester(unit, SID_HpBonus))
		status += SKILL_EFF0(SID_HpBonus);
#endif

	return status;
}
