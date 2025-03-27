#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitDefense(struct Unit *unit)
{
	const StatusGetterFunc_t *it;
	int status = unit->def;

	for (it = gpDefGetters; *it; it++)
		status = (*it)(status, unit);

	if (gpExternalDefGetters)
		status = gpExternalDefGetters(status, unit);

	return status;
}

/* Hooks */
int DefGetterWeaponBonus(int status, struct Unit *unit)
{
	u16 weapon = GetUnitEquippedWeapon(unit);

	status += GetItemDefBonus(weapon);
	return status;
}

int DefGetterSkills(int status, struct Unit *unit)
{
	int cur_hp = GetUnitCurrentHp(unit);
	int max_hp = GetUnitMaxHp(unit);

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
	if (SkillTester(unit, SID_LifeAndDeath))
		status += SKILL_EFF1(SID_LifeAndDeath);
#endif

#if defined(SID_DefBonus) && (COMMON_SKILL_VALID(SID_DefBonus))
	if (SkillTester(unit, SID_DefBonus))
		status += SKILL_EFF0(SID_DefBonus);
#endif

#if defined(SID_DefiantDef) && (COMMON_SKILL_VALID(SID_DefiantDef))
	if (SkillTester(unit, SID_DefiantDef))
		if ((cur_hp * 4) < max_hp)
			status += SKILL_EFF0(SID_DefiantDef);
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
	if (SkillTester(unit, SID_Fury))
		status += SKILL_EFF0(SID_Fury);
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
	if (SkillTester(unit, SID_FuryPlus))
		status += SKILL_EFF0(SID_FuryPlus);
#endif

#if defined(SID_FortressDef) && (COMMON_SKILL_VALID(SID_FortressDef))
	if (SkillTester(unit, SID_FortressDef))
		status += SKILL_EFF1(SID_FortressDef);
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
	if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_DEF)
		status += SKILL_EFF0(SID_LuckySeven);
#endif

#if defined(SID_DefenseBoost) && (COMMON_SKILL_VALID(SID_DefenseBoost))
	if (SkillTester(unit, SID_DefenseBoost)) {
		int __buf = SKILL_EFF0(SID_DefenseBoost);

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
#if defined(SID_PushDefense) && (COMMON_SKILL_VALID(SID_PushDefense))
		if (SkillTester(unit, SID_PushDefense))
			status += SKILL_EFF0(SID_PushDefense);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
		if (SkillTester(unit, SID_PushSpectrum))
			status += SKILL_EFF0(SID_PushSpectrum);
#endif
	}

	return status;
}
