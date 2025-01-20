#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

u16 const gLegendSkillPool[] = {
#if defined(SID_LEGEND_InoriAtk) && (COMMON_SKILL_VALID(SID_LEGEND_InoriAtk))
	SID_LEGEND_InoriAtk,
#endif

#if defined(SID_LEGEND_InoriAvo) && (COMMON_SKILL_VALID(SID_LEGEND_InoriAvo))
	SID_LEGEND_InoriAvo,
#endif

#if defined(SID_LEGEND_InoriDef) && (COMMON_SKILL_VALID(SID_LEGEND_InoriDef))
	SID_LEGEND_InoriDef,
#endif

#if defined(SID_LEGEND_4) && (COMMON_SKILL_VALID(SID_LEGEND_4))
	SID_LEGEND_4,
#endif

#if defined(SID_LEGEND_5) && (COMMON_SKILL_VALID(SID_LEGEND_5))
	SID_LEGEND_5,
#endif

	0
};
