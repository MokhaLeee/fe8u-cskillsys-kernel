#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"

#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

void PreBattleCalcLegendSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (SkillTesterLegendActivated(&attacker->unit, SID_LEGEND_InoriAtk))
    {
        attacker->battleAttack += 10;
        attacker->battleCritRate += 100;
    }

    if (SkillTesterLegendActivated(&attacker->unit, SID_LEGEND_InoriAvo))
    {
        attacker->battleAvoidRate += 100;
    }
}

int SpdGetterLegendSkills(int status, struct Unit * unit)
{
    if (SkillTesterLegendActivated(unit, SID_LEGEND_InoriAvo))
        status += 10;

    return status;
}

int DefGetterLegendSkills(int status, struct Unit * unit)
{
    if (SkillTesterLegendActivated(unit, SID_LEGEND_InoriDef))
        status += 10;

    return status;
}

int ResGetterLegendSkills(int status, struct Unit * unit)
{
    if (SkillTesterLegendActivated(unit, SID_LEGEND_InoriDef))
        status += 10;

    return status;
}
