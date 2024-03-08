#include "global.h"
#include "anime.h"

#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "combat-art.h"

struct EfxSkillRoundData {
    u8 sid_actor;
    u8 sid_target;
};

extern struct EfxSkillRoundData sEfxSkillRoundData[NEW_BATTLE_HIT_MAX];

/* This function should be placed at: ClearBattleHits() */
void InitEfxSkillRoundData(void)
{
    CpuFill16(0, sEfxSkillRoundData, sizeof(sEfxSkillRoundData));
}

void RegisterActorEfxSkill(int round, const u8 sid)
{
    if (round < NEW_BATTLE_HIT_MAX)
    {
        u8 sid_old = sEfxSkillRoundData[round].sid_actor;
        if (SKILL_VALID(sid_old) && GetEfxSkillPriority(sid_old) >= GetEfxSkillPriority(sid))
            return;

        Debugf("Skill %#x at round %d", sid, round);
        sEfxSkillRoundData[round].sid_actor = sid;
    }
}

void RegisterTargetEfxSkill(int round, const u8 sid)
{
    if (round < NEW_BATTLE_HIT_MAX)
    {
        u8 sid_old = sEfxSkillRoundData[round].sid_target;
        if (SKILL_VALID(sid_old) && GetEfxSkillPriority(sid_old) >= GetEfxSkillPriority(sid))
            return;

        Debugf("Skill %#x at round %d", sid, round);
        sEfxSkillRoundData[round].sid_target = sid;
    }
}

u8 GetActorEfxSkill(int round)
{
    if (round < NEW_BATTLE_HIT_MAX)
        return sEfxSkillRoundData[round].sid_actor;

    return 0;
}

u8 GetTargetEfxSkill(int round)
{
    if (round < NEW_BATTLE_HIT_MAX)
        return sEfxSkillRoundData[round].sid_target;

    return 0;
}
