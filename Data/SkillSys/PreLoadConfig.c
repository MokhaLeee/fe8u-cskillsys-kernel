#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    [CHARACTER_EIRIKA] = {
        .skills = {
            [0] = SID_SpdBonus,
            [1] = SID_PosReturn,
            [5] = SID_DefiantAvoid,
        },
    },
    [CHARACTER_EPHRAIM] = {
        .skills = {
            [0] = SID_StrBonus,
            [5] = SID_DefiantCrit,
        },
    },

    [CHARACTER_INNES] = {
        .skills = {
            [0] = SID_RangeBonusBow2,
            [1] = SID_Thunderstorm,
        },
    },

    [CHARACTER_RIEV] = {
        .skills = {
            [0] = SID_Thunderstorm,
        },
    },
};

const struct SkillPreloadJConf gSkillPreloadJData[0x100] = {
    [CLASS_ASSASSIN] = {
        .skills = {
            [0]  = SID_Lethality,
            [10] = SID_DefiantCrit,
        },
    },

    [CLASS_ASSASSIN_F] = {
        .skills = {
            [0]  = SID_Lethality,
            [10] = SID_DefiantCrit,
        },
    },

    [CLASS_SNIPER] = {
        .skills = {
            [10] = SID_RangeBonusBow2,
        },
    },

    [CLASS_SNIPER_F] = {
        .skills = {
            [10] = SID_RangeBonusBow2,
        },
    },
};
