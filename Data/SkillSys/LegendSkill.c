#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

u8 const gLegendSkillPool[] = {
#if defined(SID_LEGEND_MiracleAtk) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleAtk))
    SID_LEGEND_MiracleAtk,
#endif

#if defined(SID_LEGEND_MiracleAvo) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleAvo))
    SID_LEGEND_MiracleAvo,
#endif

#if defined(SID_LEGEND_MiracleDef) && (COMMON_SKILL_VALID(SID_LEGEND_MiracleDef))
    SID_LEGEND_MiracleDef,
#endif

#if defined(SID_LEGEND_4) && (COMMON_SKILL_VALID(SID_LEGEND_4))
    SID_LEGEND_4,
#endif

#if defined(SID_LEGEND_5) && (COMMON_SKILL_VALID(SID_LEGEND_5))
    SID_LEGEND_5,
#endif

    0
};
