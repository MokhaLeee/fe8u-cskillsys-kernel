#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

bool CheckBattleSkillActivate(struct BattleUnit * actor, struct BattleUnit * target, int sid, int rate)
{
    if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
        return false;

#if (defined(SID_Foresight) && (COMMON_SKILL_VALID(SID_Foresight)))
    if (SkillTester(&target->unit, SID_Foresight))
        return false;
#endif

#if (defined(SID_RightfulKing) && (COMMON_SKILL_VALID(SID_RightfulKing)))
    if (SkillTester(&actor->unit, SID_RightfulKing))
        rate += 10;
#endif

#if (defined(SID_RightfulGod) && (COMMON_SKILL_VALID(SID_RightfulGod)))
    if (SkillTester(&actor->unit, SID_RightfulGod))
        rate += 30;
#endif

#if (defined(SID_Hero) && (COMMON_SKILL_VALID(SID_Hero)))
    if (SkillTester(&actor->unit, SID_RightfulGod))
        if ((actor->hpInitial * 2) < actor->unit.maxHP)
            rate += 30;
#endif

#if (defined(SID_RightfulArch) && (COMMON_SKILL_VALID(SID_RightfulArch)))
    if (SkillTester(&actor->unit, SID_RightfulArch))
        rate = 100;
#endif

    LIMIT_AREA(rate, 0, 100);

    if (SkillTester(&actor->unit, sid) && Roll2RN(rate))
        return true;

    return false;
}
