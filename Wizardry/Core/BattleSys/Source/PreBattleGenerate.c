#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "debuff.h"
#include "strmag.h"
#include "battle-system.h"
#include "constants/skills.h"

STATIC_DECLAR FORCE_DECLARE void ModifyBattleUnitStatus_LightAndDark(struct BattleUnit * actor, struct BattleUnit * target)
{
    int i;

    const struct DebuffInfo * debuff_act = &gpDebuffInfos[GetUnitStatusIndex(&actor->unit)];
    const struct DebuffInfo * debuff_tar = &gpDebuffInfos[GetUnitStatusIndex(&target->unit)];

    struct StatDebuffStatus * sdebuff_act = GetUnitStatDebuffStatus(&actor->unit);
    struct StatDebuffStatus * sdebuff_tar = GetUnitStatDebuffStatus(&target->unit);

    target->unit.pow -= 5;
    UNIT_MAG(&target->unit) -= 5;
    target->unit.spd -= 5;
    target->unit.def -= 5;
    target->unit.res -= 5;

    /* Debuff */
    if (debuff_act->positive_type == STATUS_DEBUFF_NEGATIVE)
        SetUnitStatus(&actor->unit, UNIT_STATUS_NONE);

    if (debuff_tar->positive_type == STATUS_DEBUFF_POSITIVE)
        SetUnitStatus(&target->unit, UNIT_STATUS_NONE);

    /* StatDebuff */
    for (i = UNIT_STAT_DEBUFF_IDX_START; i < UNIT_STAT_DEBUFF_MAX; i++)
    {
        if (!_BIT_CHK(sdebuff_act->st.bitmask, i))
            continue;

        if (gpStatDebuffInfos[i].positive_type == STATUS_DEBUFF_NEGATIVE)
            _BIT_CLR(sdebuff_act->st.bitmask, i);
    }

    for (i = UNIT_STAT_DEBUFF_IDX_START; i < UNIT_STAT_DEBUFF_MAX; i++)
    {
        if (!_BIT_CHK(sdebuff_tar->st.bitmask, i))
            continue;

        if (gpStatDebuffInfos[i].positive_type == STATUS_DEBUFF_POSITIVE)
            _BIT_CLR(sdebuff_tar->st.bitmask, i);
    }
}

/**
 * This is set an addition routine on start of function: `BattleGenerate()`
 * The goal of introducing this function is to make potential modification on battle unit status.
 * This function will be called once per battle calc.
 * As a comparison, pre-battle calc and battle-calc real routine will exec twice for both foe.
 */
void PreBattleGenerateHook(void)
{
    FORCE_DECLARE struct BattleUnit * actor  = &gBattleActor;
    FORCE_DECLARE struct BattleUnit * target = &gBattleTarget;

#if (defined(SID_LightAndDark) && (COMMON_SKILL_VALID(SID_LightAndDark)))
    if (SkillTester(&actor->unit, SID_LightAndDark))
        ModifyBattleUnitStatus_LightAndDark(actor, target);

    if (SkillTester(&target->unit, SID_LightAndDark))
        ModifyBattleUnitStatus_LightAndDark(target, actor);
#endif
}
