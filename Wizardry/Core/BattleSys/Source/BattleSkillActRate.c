#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"

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
    /* Check skill Foresight */
    if (SkillTester(&target->unit, SID_Foresight))
        return false;

    if (SkillTester(&actor->unit, sid))
        if (TryAutoActSkill(actor, target) || BattleRoll2RN(rate, false))
            return true;

    return false;
}
