#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "bwl.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(GetUnitExpLevel);
int GetUnitExpLevel(struct Unit *unit)
{
	int base, bonus;

	base = unit->level;

	if (CheckHasBwl(UNIT_CHAR_ID(unit)))
		bonus = GetUnitHiddenLevel(unit);
	else
		bonus = gpClassPreLoadHiddenLevel[UNIT_CLASS_ID(unit)];

	return base + bonus;
}

STATIC_DECLAR int KernelModifyBattleUnitExp(int base, struct BattleUnit *actor, struct BattleUnit *target)
{
	int status = base;

#if defined(SID_Blossom) && (COMMON_SKILL_VALID(SID_Blossom))
	if (BattleFastSkillTester(actor, SID_Blossom))
		status = status / 2;
#endif

#if defined(SID_Paragon) && (COMMON_SKILL_VALID(SID_Paragon))
	if (BattleFastSkillTester(actor, SID_Paragon))
		status = status * 2;
#endif

	/* Check last */
#if defined(SID_VoidCurse) && (COMMON_SKILL_VALID(SID_VoidCurse))
	if (BattleFastSkillTester(target, SID_VoidCurse))
		status = 0;
#endif

	LIMIT_AREA(status, 0, 100);
	if (base > 0 && status <= 0)
		status = 1;

	return status;
}

int GetBattleUnitExpGainRework(struct BattleUnit *actor, struct BattleUnit *target)
{
	int result = GetBattleUnitExpGain(actor, target);

	result = KernelModifyBattleUnitExp(result, actor, target);
	ModifyUnitSpecialExp(&actor->unit, &target->unit, &result);

	return result;
}

LYN_REPLACE_CHECK(BattleApplyMiscActionExpGains);
void BattleApplyMiscActionExpGains(void)
{
	int exp;

	if (UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE)
		return;

	if (!CanBattleUnitGainLevels(&gBattleActor))
		return;

	if (gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)
		return;

	exp = 10;
	exp = KernelModifyBattleUnitExp(exp, &gBattleActor, &gBattleTarget);

	gBattleActor.expGain = exp;
	gBattleActor.unit.exp += exp;

#if CHAX
	ResetPopupSkillStack();
#endif

	CheckBattleUnitLevelUp(&gBattleActor);
}

int GetBattleUnitStaffExpRework(struct BattleUnit *bu)
{
	int result = GetBattleUnitStaffExp(bu);

	result = KernelModifyBattleUnitExp(
		result,
		bu,
		bu == &gBattleActor
			? &gBattleTarget
			: &gBattleActor);

	if (result > 100)
		result = 100;

	return result;
}

LYN_REPLACE_CHECK(BattleApplyItemExpGains);
void BattleApplyItemExpGains(void)
{
	if (gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)
		return;

#if CHAX
	ResetPopupSkillStack();
#endif

	if (gBattleActor.weaponAttributes & IA_STAFF) {
		if (UNIT_FACTION(&gBattleActor.unit) == FACTION_BLUE)
			gBattleActor.wexpMultiplier++;

		gBattleActor.expGain = GetBattleUnitStaffExpRework(&gBattleActor);
		gBattleActor.unit.exp += gBattleActor.expGain;

		CheckBattleUnitLevelUp(&gBattleActor);
	} else if ((gBattleActor.weaponType == ITYPE_12) && (gBattleActor.unit.exp != UNIT_EXP_DISABLED)) {
		gBattleActor.expGain = 20;
		gBattleActor.unit.exp += 20;

		CheckBattleUnitLevelUp(&gBattleActor);
	}
}

LYN_REPLACE_CHECK(BattleApplyExpGains);
void BattleApplyExpGains(void)
{
	if ((UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE) || (UNIT_FACTION(&gBattleTarget.unit) != FACTION_BLUE)) {
		if (!(gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)) {
			gBattleActor.expGain  = GetBattleUnitExpGainRework(&gBattleActor, &gBattleTarget);
			gBattleTarget.expGain = GetBattleUnitExpGainRework(&gBattleTarget, &gBattleActor);

			gBattleActor.unit.exp  += gBattleActor.expGain;
			gBattleTarget.unit.exp += gBattleTarget.expGain;

#if CHAX
			ResetPopupSkillStack();
#endif

			CheckBattleUnitLevelUp(&gBattleActor);
			CheckBattleUnitLevelUp(&gBattleTarget);
		}
	}
}
