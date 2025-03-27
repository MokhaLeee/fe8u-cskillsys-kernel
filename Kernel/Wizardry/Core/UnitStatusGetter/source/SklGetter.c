#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitSkill(struct Unit *unit)
{
	const StatusGetterFunc_t *it;
	int status = unit->skl;

	if (unit->state & US_RESCUING)
		status = unit->skl / 2;

	for (it = gpSklGetters; *it; it++)
		status = (*it)(status, unit);

	if (gpExternalSklGetters)
		status = gpExternalSklGetters(status, unit);

	return status;
}

/* Hooks */
int SklGetterWeaponBonus(int status, struct Unit *unit)
{
	u16 weapon = GetUnitEquippedWeapon(unit);

	status += GetItemSklBonus(weapon);
	return status;
}

int SklGetterSkills(int status, struct Unit *unit)
{
	int cur_hp = GetUnitCurrentHp(unit);
	int max_hp = GetUnitMaxHp(unit);

#if defined(SID_LifeAndDeath) && (COMMON_SKILL_VALID(SID_LifeAndDeath))
	if (SkillTester(unit, SID_LifeAndDeath))
		status += SKILL_EFF0(SID_LifeAndDeath);
#endif

#if defined(SID_SklBonus) && (COMMON_SKILL_VALID(SID_SklBonus))
	if (SkillTester(unit, SID_SklBonus))
		status += SKILL_EFF0(SID_SklBonus);
#endif

#if defined(SID_DefiantSkl) && (COMMON_SKILL_VALID(SID_DefiantSkl))
	if (SkillTester(unit, SID_DefiantSkl))
		if ((cur_hp * 4) < max_hp)
			status += SKILL_EFF0(SID_DefiantSkl);
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
	if (SkillTester(unit, SID_Fury))
		status += SKILL_EFF0(SID_Fury);
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
	if (SkillTester(unit, SID_FuryPlus))
		status += SKILL_EFF0(SID_FuryPlus);
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
	if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_SKL)
		status += SKILL_EFF0(SID_LuckySeven);
#endif

#if defined(SID_SkillBoost) && (COMMON_SKILL_VALID(SID_SkillBoost))
	if (SkillTester(unit, SID_SkillBoost)) {
		int __buf = SKILL_EFF0(SID_SkillBoost);

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
#if defined(SID_PushSkill) && (COMMON_SKILL_VALID(SID_PushSkill))
		if (SkillTester(unit, SID_PushSkill))
			status += SKILL_EFF0(SID_PushSkill);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
		if (SkillTester(unit, SID_PushSpectrum))
			status += SKILL_EFF0(SID_PushSpectrum);
#endif
	}

#if (defined(SID_Resolve) && (COMMON_SKILL_VALID(SID_Resolve)))
	if (SkillTester(unit, SID_Resolve)) {
		if ((cur_hp * 2) < max_hp)
			status += status / 2;
	}
#endif

	return status;
}
