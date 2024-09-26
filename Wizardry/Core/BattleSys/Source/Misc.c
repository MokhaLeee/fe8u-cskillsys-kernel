#include "common-chax.h"
#include "battle-system.h"

void ClearBattleGlobalFlags(void)
{
	Assert(sizeof(gBattleActorGlobalFlag) <= 0x10);
	Assert(sizeof(gBattleTargetGlobalFlag) <= 0x10);

	CpuFill16(0, &gBattleActorGlobalFlag, sizeof(gBattleActorGlobalFlag));
	CpuFill16(0, &gBattleTargetGlobalFlag, sizeof(gBattleTargetGlobalFlag));
}

void RegisterHitCnt(struct BattleUnit *bu, bool miss)
{
	struct BattleGlobalFlags *flags;

	if (!(gBattleStats.config & BATTLE_CONFIG_REAL))
		return;

	flags = GetBattleGlobalFlags(bu);
	flags->round_cnt++;
	if (!miss) {
		flags->hitted = true;
		flags->round_cnt_hit++;
	} else
		flags->round_cnt_avo++;
}

void ResetRoundEfxSkills(void)
{
	memset(&sEfxSkillQueue, 0, sizeof(sEfxSkillQueue));
}

void EnqueueRoundEfxSkill(u16 sid)
{
	if (sEfxSkillQueue.max < (sizeof(sEfxSkillQueue.skill_pool) - 1))
		sEfxSkillQueue.skill_pool[sEfxSkillQueue.max++] = sid;
}

u16 DequeueRoundEfxSkill(void)
{
	if (sEfxSkillQueue.cur < sEfxSkillQueue.max)
		return sEfxSkillQueue.skill_pool[sEfxSkillQueue.cur++];

	return 0;
}
