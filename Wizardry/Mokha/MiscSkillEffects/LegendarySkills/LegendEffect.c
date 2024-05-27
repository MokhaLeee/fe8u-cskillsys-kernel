#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"

#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

void PreBattleCalcLegendSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
#if defined(SID_LEGEND_InoriAtk) && (SID_LEGEND_InoriAtk < MAX_SKILL_NUM)
    if (SkillTesterLegendActivated(&attacker->unit, SID_LEGEND_InoriAtk))
    {
        attacker->battleAttack += 10;
        attacker->battleCritRate += 100;
    }
#endif

#if defined(SID_LEGEND_InoriAvo) && (SID_LEGEND_InoriAvo < MAX_SKILL_NUM)
    if (SkillTesterLegendActivated(&attacker->unit, SID_LEGEND_InoriAvo))
    {
        attacker->battleAvoidRate += 100;
    }
#endif
}

int SpdGetterLegendSkills(int status, struct Unit * unit)
{
#if defined(SID_LEGEND_InoriAvo) && (SID_LEGEND_InoriAvo < MAX_SKILL_NUM)
    if (SkillTesterLegendActivated(unit, SID_LEGEND_InoriAvo))
        status += 10;
#endif

    return status;
}

int DefGetterLegendSkills(int status, struct Unit * unit)
{
#if defined(SID_LEGEND_InoriDef) && (SID_LEGEND_InoriDef < MAX_SKILL_NUM)
    if (SkillTesterLegendActivated(unit, SID_LEGEND_InoriDef))
        status += 10;
#endif

    return status;
}

int ResGetterLegendSkills(int status, struct Unit * unit)
{
#if defined(SID_LEGEND_InoriDef) && (SID_LEGEND_InoriDef < MAX_SKILL_NUM)
    if (SkillTesterLegendActivated(unit, SID_LEGEND_InoriDef))
        status += 10;
#endif

    return status;
}
