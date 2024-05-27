#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    [CHARACTER_EIRIKA] = {
        .skills = {
#if defined(SID_SpdBonus) && (SID_SpdBonus < MAX_SKILL_NUM)
            [0] = SID_SpdBonus,
#endif

#if defined(SID_PosReturn) && (SID_PosReturn < MAX_SKILL_NUM)
            [1] = SID_PosReturn,
#endif

#if defined(SID_DefiantAvoid) && (SID_DefiantAvoid < MAX_SKILL_NUM)
            [5] = SID_DefiantAvoid,
#endif
        },
    },
    [CHARACTER_EPHRAIM] = {
        .skills = {
#if defined(SID_StrBonus) && (SID_StrBonus < MAX_SKILL_NUM)
            [0] = SID_StrBonus,
#endif

#if defined(SID_DefiantCrit) && (SID_DefiantCrit < MAX_SKILL_NUM)
            [5] = SID_DefiantCrit,
#endif
        },
    },

    [CHARACTER_INNES] = {
        .skills = {
#if defined(SID_RangeBonusBow2) && (SID_RangeBonusBow2 < MAX_SKILL_NUM)
            [0] = SID_RangeBonusBow2,
#endif

#if defined(SID_PosSwap) && (SID_PosSwap < MAX_SKILL_NUM)
            [1] = SID_PosSwap,
#endif
        },
    },

    [CHARACTER_RIEV] = {
        .skills = {
#if defined(SID_Thunderstorm) && (SID_Thunderstorm < MAX_SKILL_NUM)
            [0] = SID_Thunderstorm,
#endif
        },
    },
};

const struct SkillPreloadJConf gSkillPreloadJData[0x100] = {
    [CLASS_ASSASSIN] = {
        .skills = {
#if defined(SID_Lethality) && (SID_Lethality < MAX_SKILL_NUM)
            [0]  = SID_Lethality,
#endif

#if defined(SID_DefiantCrit) && (SID_DefiantCrit < MAX_SKILL_NUM)
            [10] = SID_DefiantCrit,
#endif
        },
    },

    [CLASS_ASSASSIN_F] = {
        .skills = {
#if defined(SID_Lethality) && (SID_Lethality < MAX_SKILL_NUM)
            [0]  = SID_Lethality,
#endif

#if defined(SID_DefiantCrit) && (SID_DefiantCrit < MAX_SKILL_NUM)
            [10] = SID_DefiantCrit,
#endif
        },
    },

    [CLASS_SNIPER] = {
        .skills = {
 #if defined(SID_RangeBonusBow2) && (SID_RangeBonusBow2 < MAX_SKILL_NUM)
            [10] = SID_RangeBonusBow2,
#endif
        },
    },

    [CLASS_SNIPER_F] = {
        .skills = {
#if defined(SID_RangeBonusBow2) && (SID_RangeBonusBow2 < MAX_SKILL_NUM)
            [10] = SID_RangeBonusBow2,
#endif
        },
    },
};
