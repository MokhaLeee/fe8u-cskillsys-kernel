#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

void PreBattleCalcLegendSkills(struct BattleUnit *attacker, struct BattleUnit *defender)
{
#if defined(SID_LEGEND_InoriAtk) && (COMMON_SKILL_VALID(SID_LEGEND_InoriAtk))
	if (SkillTesterLegendActivated(&attacker->unit, SID_LEGEND_InoriAtk)) {
		attacker->battleAttack += 10;
		attacker->battleCritRate += 100;
	}
#endif

#if defined(SID_LEGEND_InoriAvo) && (COMMON_SKILL_VALID(SID_LEGEND_InoriAvo))
	if (SkillTesterLegendActivated(&attacker->unit, SID_LEGEND_InoriAvo))
		attacker->battleAvoidRate += 100;
#endif
}

int SpdGetterLegendSkills(int status, struct Unit *unit)
{
#if defined(SID_LEGEND_InoriAvo) && (COMMON_SKILL_VALID(SID_LEGEND_InoriAvo))
	if (SkillTesterLegendActivated(unit, SID_LEGEND_InoriAvo))
		status += 10;
#endif

	return status;
}

int DefGetterLegendSkills(int status, struct Unit *unit)
{
#if defined(SID_LEGEND_InoriDef) && (COMMON_SKILL_VALID(SID_LEGEND_InoriDef))
	if (SkillTesterLegendActivated(unit, SID_LEGEND_InoriDef))
		status += 10;
#endif

	return status;
}

int ResGetterLegendSkills(int status, struct Unit *unit)
{
#if defined(SID_LEGEND_InoriDef) && (COMMON_SKILL_VALID(SID_LEGEND_InoriDef))
	if (SkillTesterLegendActivated(unit, SID_LEGEND_InoriDef))
		status += 10;
#endif

	return status;
}
