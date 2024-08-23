#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    [CHARACTER_EIRIKA] = {
        .skills = {
            [0] = SID_SpdBonus,
            [1] = SID_PosReturn,
            [2] = SID_HpBonus,
            [3] = SID_Desperation,
        },
    },
    [CHARACTER_EPHRAIM] = {
        .skills = {
            [0] = SID_StrBonus,
        },
    },

    [CHARACTER_SETH] = {
        .skills = {
            [0] = SID_Vantage,
        },
    },

    [CHARACTER_INNES] = {
        .skills = {
            [0] = SID_RangeBonusBow2,
            [1] = SID_Lunge,
        },
    },

    [CHARACTER_SALEH] = {
        .skills = {
            [0] = SID_RangeBonusBMag1,
            [1] = SID_RangeBonusBMag2,
            [2] = SID_Lunge,
        },
    },

    [CHARACTER_RIEV] = {
        .skills = {
            [0] = SID_Thunderstorm,
        },
    },
};

const struct SkillPreloadJConf gSkillPreloadJData[0x100] = {
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
