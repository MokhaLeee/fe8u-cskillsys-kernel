#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitResistance(struct Unit *unit)
{
	const StatusGetterFunc_t *it;
	int status = unit->res;

	for (it = gpResGetters; *it; it++)
		status = (*it)(status, unit);

	if (gpExternalResGetters)
		status = gpExternalResGetters(status, unit);

	return status;
}

/* Hooks */
int ResGetterWeaponBonus(int status, struct Unit *unit)
{
	u16 weapon = GetUnitEquippedWeapon(unit);

	status += GetItemResBonus(weapon);
	return status;
}

int ResGetterSkills(int status, struct Unit *unit)
{
	int cur_hp = GetUnitCurrentHp(unit);
	int max_hp = GetUnitMaxHp(unit);

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
	if (SkillTester(unit, SID_LifeAndDeath))
		status += SKILL_EFF1(SID_LifeAndDeath);
#endif

#if defined(SID_ResBonus) && (COMMON_SKILL_VALID(SID_ResBonus))
	if (SkillTester(unit, SID_ResBonus))
		status += SKILL_EFF0(SID_ResBonus);
#endif

#if defined(SID_DefiantRes) && (COMMON_SKILL_VALID(SID_DefiantRes))
	if (SkillTester(unit, SID_DefiantRes))
		if ((cur_hp * 4) < max_hp)
			status += SKILL_EFF0(SID_DefiantRes);
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
	if (SkillTester(unit, SID_Fury))
		status += SKILL_EFF0(SID_Fury);
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
	if (SkillTester(unit, SID_FuryPlus))
		status += SKILL_EFF0(SID_FuryPlus);
#endif

#if defined(SID_FortressRes) && (COMMON_SKILL_VALID(SID_FortressRes))
	if (SkillTester(unit, SID_FortressRes))
		status += SKILL_EFF1(SID_FortressRes);
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
	if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_RES)
		status += SKILL_EFF0(SID_LuckySeven);
#endif

#if defined(SID_ResistanceBoost) && (COMMON_SKILL_VALID(SID_ResistanceBoost))
	if (SkillTester(unit, SID_ResistanceBoost)) {
		int __buf = SKILL_EFF0(SID_ResistanceBoost);

		if (gPlaySt.chapterTurnNumber >= __buf)
			status += __buf;
		else
			status += gPlaySt.chapterTurnNumber;
	}
#endif

#if defined(SID_SpectrumBoost) && (COMMON_SKILL_VALID(SID_SpectrumBoost))
	if (SkillTester(unit, SID_SpectrumBoost)) {
		int __buf = SKILL_EFF0(SID_SpectrumBoost);

		if (gPlaySt.chapterTurnNumber >= __buf)
			status += __buf;
		else
			status += gPlaySt.chapterTurnNumber;
	}
#endif

	if (cur_hp == max_hp) {
#if defined(SID_PushResistance) && (COMMON_SKILL_VALID(SID_PushResistance))
		if (SkillTester(unit, SID_PushResistance))
			status += SKILL_EFF0(SID_PushResistance);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
		if (SkillTester(unit, SID_PushSpectrum))
			status += SKILL_EFF0(SID_PushSpectrum);
#endif
	}

	return status;
}

int ResGetterPureWater(int status, struct Unit *unit)
{
	return status + unit->barrierDuration;
}
