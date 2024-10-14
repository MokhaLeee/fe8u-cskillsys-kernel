#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "combat-art.h"

#define LOCAL_TRACE 0

struct EfxSkillRoundData {
	u16 sid_actor;
	u16 sid_target;
};

extern struct EfxSkillRoundData sEfxSkillRoundData[NEW_BATTLE_HIT_MAX];

/* This function should be placed at: ClearBattleHits() */
void InitEfxSkillRoundData(void)
{
	CpuFill16(0, sEfxSkillRoundData, sizeof(sEfxSkillRoundData));
}

void RegisterActorEfxSkill(int round, const u16 sid)
{
	u16 sid_old = sEfxSkillRoundData[round].sid_actor;

	if (GetEfxSkillPriority(sid_old) >= GetEfxSkillPriority(sid))
		return;

	LTRACEF("Skill %#x at round %d", sid, round);
	sEfxSkillRoundData[round].sid_actor = sid;
}

void RegisterTargetEfxSkill(int round, const u16 sid)
{
	u16 sid_old = sEfxSkillRoundData[round].sid_target;

	if (GetEfxSkillPriority(sid_old) >= GetEfxSkillPriority(sid))
		return;

	LTRACEF("Skill %#x at round %d", sid, round);
	sEfxSkillRoundData[round].sid_target = sid;
}

u16 GetActorEfxSkill(int round)
{
	if (round < NEW_BATTLE_HIT_MAX)
		return sEfxSkillRoundData[round].sid_actor;

	return 0;
}

u16 GetTargetEfxSkill(int round)
{
	if (round < NEW_BATTLE_HIT_MAX)
		return sEfxSkillRoundData[round].sid_target;

	return 0;
}
