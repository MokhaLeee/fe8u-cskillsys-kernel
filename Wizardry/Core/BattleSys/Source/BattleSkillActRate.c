#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

STATIC_DECLAR bool TryAutoActSkill(struct BattleUnit * actor, struct BattleUnit * target)
{
    return false;
}

bool CheckBattleSkillActivte(struct BattleUnit * actor, struct BattleUnit * target, int sid, int rate)
{
#if (defined(SID_Foresight) && (SID_Foresight < MAX_SKILL_NUM))
    /* Check skill Foresight */
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

    if (SkillTester(&actor->unit, sid))
        if (TryAutoActSkill(actor, target) || BattleRoll2RN(rate, false))
            return true;

    return false;
}
