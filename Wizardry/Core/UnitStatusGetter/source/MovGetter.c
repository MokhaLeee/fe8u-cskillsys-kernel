#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "debuff.h"
#include "constants/skills.h"

int _GetUnitMov(struct Unit *unit)
{
	const StatusGetterFunc_t *it;
	int status = UNIT_MOV(unit);

	for (it = gpMovGetters; *it; it++)
		status = (*it)(status, unit);

	if (gpExternalMovGetters)
		status = gpExternalMovGetters(status, unit);

	/* Some special effects */
	if (gpDebuffInfos[GetUnitStatusIndex(unit)].cannot_move)
		status = 0;

	return status;
}

int MovGetterSkills(int status, struct Unit *unit)
{
	FORCE_DECLARE int cur_hp = GetUnitCurrentHp(unit);
	FORCE_DECLARE int max_hp = GetUnitMaxHp(unit);

#if defined(SID_MovBonus) && (COMMON_SKILL_VALID(SID_MovBonus))
	if (SkillTester(unit, SID_MovBonus))
		status += SKILL_EFF0(SID_MovBonus);
#endif

#if defined(SID_LuckySeven) && (COMMON_SKILL_VALID(SID_LuckySeven))
	if (SkillTester(unit, SID_LuckySeven) && (gPlaySt.chapterTurnNumber & 0x7) == LUCKY7_MOV)
		status += SKILL_EFF0(SID_LuckySeven);
#endif

	if (cur_hp == max_hp) {
#if defined(SID_PushMovement) && (COMMON_SKILL_VALID(SID_PushMovement))
		if (SkillTester(unit, SID_PushMovement))
			status += SKILL_EFF0(SID_PushMovement);
#endif

#if defined(SID_PushSpectrum) && (COMMON_SKILL_VALID(SID_PushSpectrum))
		if (SkillTester(unit, SID_PushSpectrum))
			status += SKILL_EFF0(SID_PushSpectrum);
#endif
	}

#if defined(SID_EvenFooted) && (COMMON_SKILL_VALID(SID_EvenFooted))
	if (SkillTester(unit, SID_EvenFooted) && (gPlaySt.chapterTurnNumber % 2) == 0)
		status += SKILL_EFF0(SID_EvenFooted);
#endif

#if defined(SID_OddFooted) && (COMMON_SKILL_VALID(SID_OddFooted))
	if (SkillTester(unit, SID_OddFooted) && (gPlaySt.chapterTurnNumber % 2) == 1)
		status += SKILL_EFF0(SID_OddFooted);
#endif

#if defined(SID_MoveBoost) && (COMMON_SKILL_VALID(SID_MoveBoost))
	if (SkillTester(unit, SID_MoveBoost)) {
		int __buf = SKILL_EFF0(SID_MoveBoost);

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

#if defined(SID_Poise) && (COMMON_SKILL_VALID(SID_Poise))
	if (SkillTester(unit, SID_Poise))
		status += SKILL_EFF0(SID_Poise);
#endif

	return status;
}
