#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

void PreBattleCalcLegendSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
#if defined(SID_LEGEND_MiracleAtk) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleAtk))
    if (SkillTesterLegendActivated(&attacker->unit, SID_LEGEND_MiracleAtk))
    {
        attacker->battleAttack += 10;
        attacker->battleCritRate += 100;
    }
#endif

#if defined(SID_LEGEND_MiracleAvo) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleAvo))
    if (SkillTesterLegendActivated(&attacker->unit, SID_LEGEND_MiracleAvo))
    {
        attacker->battleAvoidRate += 100;
    }
#endif
}

int SpdGetterLegendSkills(int status, struct Unit * unit)
{
#if defined(SID_LEGEND_MiracleAvo) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleAvo))
    if (SkillTesterLegendActivated(unit, SID_LEGEND_MiracleAvo))
        status += 10;
#endif

    return status;
}

int DefGetterLegendSkills(int status, struct Unit * unit)
{
#if defined(SID_LEGEND_MiracleDef) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleDef))
    if (SkillTesterLegendActivated(unit, SID_LEGEND_MiracleDef))
        status += 10;
#endif

    return status;
}

int ResGetterLegendSkills(int status, struct Unit * unit)
{
#if defined(SID_LEGEND_MiracleDef) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleDef))
    if (SkillTesterLegendActivated(unit, SID_LEGEND_MiracleDef))
        status += 10;
#endif

    return status;
}
