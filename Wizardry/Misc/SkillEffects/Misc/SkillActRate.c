#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

extern s16 sBattleSkillActivateBuf[2];

void ResetBattleSkillActivateBuf(void)
{
	sBattleSkillActivateBuf[0] = -1;
	sBattleSkillActivateBuf[1] = -1;
}

STATIC_DECLAR s16 CalcBattleSkillActivateBufExt(struct BattleUnit *actor, struct BattleUnit *target)
{
	int rate = 0;

#if (defined(SID_Foresight) && (COMMON_SKILL_VALID(SID_Foresight)))
	if (BattleSkillTesterFast(target, SID_Foresight))
		return 0;
#endif

#if (defined(SID_RightfulArch) && (COMMON_SKILL_VALID(SID_RightfulArch)))
	if (BattleSkillTesterFast(actor, SID_RightfulArch))
		return 100;
#endif

#if (defined(SID_RightfulKing) && (COMMON_SKILL_VALID(SID_RightfulKing)))
	if (BattleSkillTesterFast(actor, SID_RightfulKing))
		rate += SKILL_EFF0(SID_RightfulKing);
#endif

#if (defined(SID_RightfulGod) && (COMMON_SKILL_VALID(SID_RightfulGod)))
	if (BattleSkillTesterFast(actor, SID_RightfulGod))
		rate += SKILL_EFF0(SID_RightfulGod);
#endif

#if (defined(SID_Hero) && (COMMON_SKILL_VALID(SID_Hero)))
	if (BattleSkillTesterFast(actor, SID_Hero))
		if ((actor->hpInitial * 2) < actor->unit.maxHP)
			rate += SKILL_EFF0(SID_Hero);
#endif

	LIMIT_AREA(rate, 0, 100);
	return rate;
}

STATIC_DECLAR void SetupBattleSkillActivateBuf(void)
{
	sBattleSkillActivateBuf[0] = CalcBattleSkillActivateBufExt(&gBattleActor,  &gBattleTarget);
	sBattleSkillActivateBuf[1] = CalcBattleSkillActivateBufExt(&gBattleTarget, &gBattleActor);
}

bool CheckSkillActivate(struct Unit *unit, int sid, int rate)
{
	if (!SkillTester(unit, sid))
		return false;

#if (defined(SID_RightfulArch) && (COMMON_SKILL_VALID(SID_RightfulArch)))
	if (SkillTester(unit, SID_RightfulArch))
		return true;
#endif

#if (defined(SID_RightfulKing) && (COMMON_SKILL_VALID(SID_RightfulKing)))
	if (SkillTester(unit, SID_RightfulKing))
		rate += SKILL_EFF0(SID_RightfulKing);
#endif

#if (defined(SID_RightfulGod) && (COMMON_SKILL_VALID(SID_RightfulGod)))
	if (SkillTester(unit, SID_RightfulGod))
		rate += SKILL_EFF0(SID_RightfulGod);
#endif

#if (defined(SID_Hero) && (COMMON_SKILL_VALID(SID_Hero)))
	if (SkillTester(unit, SID_Hero))
		if ((unit->curHP * 2) < unit->maxHP)
			rate += SKILL_EFF0(SID_Hero);
#endif

	LIMIT_AREA(rate, 0, 100);

	if (Roll2RN(rate))
		return true;

	return false;
}

bool CheckBattleSkillActivate(struct BattleUnit *actor, struct BattleUnit *target, int sid, int rate)
{
	s16 base_rate;

	if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
		return false;

	if (!BattleSkillTesterFast(actor, sid))
		return false;

	base_rate = sBattleSkillActivateBuf[actor - &gBattleActor];
	if (base_rate < 0) {
		SetupBattleSkillActivateBuf();
		base_rate = sBattleSkillActivateBuf[actor - &gBattleActor];
	}

	if (Roll2RN(rate + base_rate))
		return true;

	return false;
}
