#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"

#include "common-chax.h"
#include "skill-system.h"
#include "bwl.h"
#include "constants/skills.h"

/* LynJump */
int GetUnitExpLevel(struct Unit * unit)
{
    int base, bonus;
    base = unit->level;

    if (CheckHasBwl(UNIT_CHAR_ID(unit)))
        bonus = GetUnitHiddenLevel(unit);
    else
        bonus = gClassPreLoadHiddenLevel[UNIT_CLASS_ID(unit)];

    return base + bonus;
}

/* LynJump */
int GetBattleUnitExpGain(struct BattleUnit * actor, struct BattleUnit * target)
{
    int result;

    if (!CanBattleUnitGainLevels(actor) || (actor->unit.curHP == 0) || UNIT_CATTRIBUTES(&target->unit) & CA_NEGATE_LETHALITY)
        return 0;

    if (!actor->nonZeroDamage)
        return 1;

    result = GetUnitRoundExp(&actor->unit, &target->unit);
    result += GetUnitKillExpBonus(&actor->unit, &target->unit);

#if CHAX
    if (SkillTester(&actor->unit, SID_Blossom))
        result = result / 2;

    if (SkillTester(&actor->unit, SID_Paragon))
        result = result * 2;

    /* Check last */
    if (SkillTester(&target->unit, SID_VoidCurse))
        result = 0;
#endif

    if (result > 100)
        result = 100;

    if (result < 1)
        result = 1;

#if !CHAX
    ModifyUnitSpecialExp(&actor->unit, &target->unit, &result);
#endif

    return result;
}
