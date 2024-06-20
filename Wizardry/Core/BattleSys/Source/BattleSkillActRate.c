#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

bool CheckBattleSkillActivte(struct BattleUnit * actor, struct BattleUnit * target, int sid, int rate)
{
    if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
        return false;

#if (defined(SID_Foresight) && (SID_Foresight < MAX_SKILL_NUM))
    if (SkillTester(&target->unit, SID_Foresight))
        return false;
#endif

#if (defined(SID_RightfulKing) && (SID_RightfulKing < MAX_SKILL_NUM))
    if (SkillTester(&actor->unit, SID_RightfulKing))
        rate += 10;
#endif

#if (defined(SID_RightfulGod) && (SID_RightfulGod < MAX_SKILL_NUM))
    if (SkillTester(&actor->unit, SID_RightfulGod))
        rate += 30;
#endif

#if (defined(SID_Hero) && (SID_Hero < MAX_SKILL_NUM))
    if (SkillTester(&actor->unit, SID_RightfulGod))
        if ((actor->hpInitial * 2) < actor->unit.maxHP)
            rate += 30;
#endif

#if (defined(SID_RightfulArch) && (SID_RightfulArch < MAX_SKILL_NUM))
    if (SkillTester(&actor->unit, SID_RightfulArch))
        rate = 100;
#endif

    LIMIT_AREA(rate, 0, 100);

    if (SkillTester(&actor->unit, sid) && Roll2RN(rate))
        return true;

    return false;
}
