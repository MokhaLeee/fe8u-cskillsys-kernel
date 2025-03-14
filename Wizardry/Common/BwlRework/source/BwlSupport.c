#include "common-chax.h"
#include "bwl.h"
#include "skill-system.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

STATIC_DECLAR u8 *GetUnitBwlSupports(u8 pid)
{
	struct NewBwl *bwl = GetNewBwl(pid);

	if (bwl)
		return bwl->supports;

	return NULL;
}

LYN_REPLACE_CHECK(ActionSupport);
s8 ActionSupport(ProcPtr proc)
{
	int subjectExp;
	int targetExp;
	u8 *supp1, *supp2;

	struct Unit *target = GetUnit(gActionData.targetIndex);

	int targetSupportNum = GetUnitSupporterNum(gActiveUnit, target->pCharacterData->number);
	int subjectSupportNum = GetUnitSupporterNum(target, gActiveUnit->pCharacterData->number);

	CanUnitSupportNow(target, subjectSupportNum);

	UnitGainSupportLevel(gActiveUnit, targetSupportNum);
	UnitGainSupportLevel(target, subjectSupportNum);

	StartSupportTalk(
		gActiveUnit->pCharacterData->number,
		target->pCharacterData->number,
		GetUnitSupportLevel(gActiveUnit, targetSupportNum)
	);

	supp1 = GetUnitBwlSupports(UNIT_CHAR_ID(gActiveUnit));
	supp2 = GetUnitBwlSupports(UNIT_CHAR_ID(target));

	if (supp1 && supp2) {
		subjectExp = supp1[targetSupportNum];
		targetExp = supp2[subjectSupportNum];

		if (subjectExp != targetExp) {
			if (subjectExp > targetExp)
				supp2[subjectSupportNum] = subjectExp;

			if (subjectExp < targetExp)
				supp1[targetSupportNum] = targetExp;
		}
	}

	return 0;
}

LYN_REPLACE_CHECK(GetUnitSupportLevel);
int GetUnitSupportLevel(struct Unit *unit, int num)
{
	int exp;
	u8 *supp;

	if (!UNIT_IS_VALID(unit))
		return SUPPORT_LEVEL_NONE;

	supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
	if (supp) {
		exp = supp[num];
		LTRACEF("[pid=%d, num=%d] exp=%d at BWL", UNIT_CHAR_ID(unit), num, exp);
	} else {
		/**
		 * for none-BWL characters,
		 * directly judge on its rom data
		 */
		exp = unit->pCharacterData->pSupportData->supportExpBase[num];
		LTRACEF("[pid=%d, num=%d] exp=%d at ROM", UNIT_CHAR_ID(unit), num, exp);
	}

	if (exp > 240)
		return SUPPORT_LEVEL_A;

	if (exp > 160)
		return SUPPORT_LEVEL_B;

	if (exp > 80)
		return SUPPORT_LEVEL_C;

	return SUPPORT_LEVEL_NONE;
}

LYN_REPLACE_CHECK(UnitGainSupportExp);
void UnitGainSupportExp(struct Unit *unit, int num)
{
	u8 *supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

	if (UNIT_SUPPORT_DATA(unit) && supp) {
		int gain = UNIT_SUPPORT_DATA(unit)->supportExpGrowth[num];
		int currentExp = supp[num];
		int maxExp = sSupportMaxExpLookup[GetUnitSupportLevel(unit, num)];

		FORCE_DECLARE struct Unit *other = GetUnitSupporterUnit(unit, num);

#if defined(SID_SocialButterfly) && (COMMON_SKILL_VALID(SID_SocialButterfly))
		if (SkillTester(unit, SID_SocialButterfly) || SkillTester(other, SID_SocialButterfly))
			gain *= 2;
#endif

		if (currentExp + gain > maxExp)
			gain = maxExp - currentExp;

		supp[num] = currentExp + gain;
		gPlaySt.chapterTotalSupportGain += gain;
	}
}

LYN_REPLACE_CHECK(UnitGainSupportLevel);
void UnitGainSupportLevel(struct Unit *unit, int num)
{
	u8 *supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

	if (supp)
		supp[num]++;

	gPlaySt.chapterTotalSupportGain++;

	SetSupportLevelGained(unit->pCharacterData->number, GetUnitSupporterCharacter(unit, num));
}

LYN_REPLACE_CHECK(CanUnitSupportNow);
s8 CanUnitSupportNow(struct Unit *unit, int num)
{
	int exp, maxExp;
	u8 *supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

	if (gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)
		return FALSE;

	if (gPlaySt.chapterStateBits & PLAY_FLAG_TUTORIAL)
		return FALSE;

	if (HasUnitGainedSupportLevel(unit, num))
		return FALSE;

	if (GetUnitTotalSupportLevel(unit) >= MAX_SIMULTANEOUS_SUPPORT_COUNT)
		return FALSE;

	if (GetUnitTotalSupportLevel(GetUnitSupporterUnit(unit, num)) >= MAX_SIMULTANEOUS_SUPPORT_COUNT)
		return FALSE;

	if (!supp)
		return FALSE;

	exp	= supp[num];
	maxExp = sSupportMaxExpLookup[GetUnitSupportLevel(unit, num)];

	if (exp == SUPPORT_EXP_A)
		return FALSE;

	return (exp == maxExp) ? TRUE : FALSE;
}

LYN_REPLACE_CHECK(InitUnitsupports);
void InitUnitsupports(struct Unit *unit)
{
	u8 *supp1, *supp2;
	int i, count = GetUnitSupporterCount(unit);

	for (i = 0; i < count; ++i) {
		struct Unit *other = GetUnitSupporterUnit(unit, i);

		if (!other)
			continue;

		supp1 = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
		supp2 = GetUnitBwlSupports(UNIT_CHAR_ID(other));

		if (!supp1 || !supp2)
			continue;

		supp2[GetUnitSupporterNum(other, unit->pCharacterData->number)] = 0;
		supp1[i] = 0;
	}
}

LYN_REPLACE_CHECK(UnitLoadSupports);
void UnitLoadSupports(struct Unit *unit)
{
	int i, count = GetUnitSupporterCount(unit);
	u8 *supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

	if (supp) {
		CpuFill16(0, supp, UNIT_SUPPORT_MAX_COUNT);

		for (i = 0; i < count; ++i)
			supp[i] = GetUnitSupporterInitialExp(unit, i);
	}
}
