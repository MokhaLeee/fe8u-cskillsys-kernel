#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    [CHARACTER_EIRIKA] = {
        .skills = {
#if defined(SID_SpdBonus) && GENERIC_SKILL_VALID(SID_SpdBonus)
            [0] = SID_SpdBonus,
#endif

#if defined(SID_PosReturn) && GENERIC_SKILL_VALID(SID_PosReturn)
            [1] = SID_PosReturn,
#endif
        },
    },
    [CHARACTER_EPHRAIM] = {
        .skills = {
#if defined(SID_StrBonus) && GENERIC_SKILL_VALID(SID_StrBonus)
            [0] = SID_StrBonus,
#endif
        },
    },

    [CHARACTER_INNES] = {
        .skills = {
#if defined(SID_RangeBonusBow2) && GENERIC_SKILL_VALID(SID_RangeBonusBow2)
            [0] = SID_RangeBonusBow2,
#endif

#if defined(SID_PosSwap) && GENERIC_SKILL_VALID(SID_PosSwap)
            [1] = SID_PosSwap,
#endif
        },
    },

    [CHARACTER_RIEV] = {
        .skills = {
#if defined(SID_Thunderstorm) && GENERIC_SKILL_VALID(SID_Thunderstorm)
            [0] = SID_Thunderstorm,
#endif
        },
    },
};

const struct SkillPreloadJConf gSkillPreloadJData[0x100] = {
    [CLASS_SNIPER] = {
        .skills = {
 #if defined(SID_RangeBonusBow2) && GENERIC_SKILL_VALID(SID_RangeBonusBow2)
            [10] = SID_RangeBonusBow2,
#endif
        },
    },

    [CLASS_SNIPER_F] = {
        .skills = {
#if defined(SID_RangeBonusBow2) && GENERIC_SKILL_VALID(SID_RangeBonusBow2)
            [10] = SID_RangeBonusBow2,
#endif
        },
    },
};
