#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    [CHARACTER_EIRIKA] = {
        .skills = {
            [0] = SKILL_INDEX_REAL(SID_SpdBonus),
            [1] = SKILL_INDEX_REAL(SID_PosReturn),
        },
    },
    [CHARACTER_EPHRAIM] = {
        .skills = {
            [0] = SKILL_INDEX_REAL(SID_StrBonus),
        },
    },

    [CHARACTER_SETH] = {
        .skills = {
            [0] = SKILL_INDEX_REAL(SID_Vantage),
        },
    },

    [CHARACTER_INNES] = {
        .skills = {
            [0] = SKILL_INDEX_REAL(SID_RangeBonusBow2),
            [1] = SKILL_INDEX_REAL(SID_PosSwap),
        },
    },

    [CHARACTER_RIEV] = {
        .skills = {
            [0] = SKILL_INDEX_REAL(SID_Thunderstorm),
        },
    },
};

const struct SkillPreloadJConf gSkillPreloadJData[0x100] = {
    [CLASS_SNIPER] = {
        .skills = {
            [10] = SKILL_INDEX_REAL(SID_RangeBonusBow2),
        },
    },

    [CLASS_SNIPER_F] = {
        .skills = {
            [10] = SKILL_INDEX_REAL(SID_RangeBonusBow2),
        },
    },
};
