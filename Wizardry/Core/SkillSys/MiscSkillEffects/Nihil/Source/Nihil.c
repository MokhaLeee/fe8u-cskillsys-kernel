#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "constants/skills.h"

/**
 * This is a special skill as it is tightly coupled to the basic skill-system kernel.
 * Since I am not sure whether the skill system kernel has possibilities to be expanded
 * in the future, this part of the code needs to be reviewed frequently.
 */

STATIC_DECLAR void ExecNihilSkills(struct BattleUnit * actor, struct BattleUnit * target)
{
#if (defined(SID_Nihil) && COMMON_SKILL_VALID(SID_Nihil))
    gBattleTemporaryFlag.nihil_on_actor  = BattleFastSkillTester(target, SID_Nihil);
    gBattleTemporaryFlag.nihil_on_target = BattleFastSkillTester(actor, SID_Nihil);

    if (gBattleTemporaryFlag.nihil_on_actor)
        DisableUnitSkilLList(&gBattleActor.unit);

    if (gBattleTemporaryFlag.nihil_on_target)
        DisableUnitSkilLList(&gBattleTarget.unit);
#endif
}

void UnitToBattle_ExecNihilSkills(struct Unit * unit, struct BattleUnit * bu)
{
    ForceUpdateUnitSkillList(&bu->unit);

    /**
     * Here we hold 3 assumption:
     *
     * 1. UnitToBattle routine stands at the very beginning of battle-generate
     * 2. Battle target initialization is behind actor.
     * 3. No skill activcated before during function: InitBattleUnit()
     */
    if (bu == &gBattleTarget)
        ExecNihilSkills(&gBattleActor, &gBattleTarget);
}
