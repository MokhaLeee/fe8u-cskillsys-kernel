#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

/* Maximum of 7 skills here */
const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    [CHARACTER_EIRIKA] = {
        .skills = {
        [0] = SID_Toxic,
        // [1] = SID_ShedSkin,
        // [2] = SID_Wrestler,
        // [3] = SID_DefiantAvoid,
        // [4] = SID_AdaptiveLunge,
        },
    },
    [CHARACTER_EPHRAIM] = {
    },

    [CHARACTER_SETH] = {
        .skills = {
            // [0] = SID_Canto,
            // [1] = SID_KillStreak,
            // [2] = SID_HyperFocus,
        },
    },

    [CHARACTER_ONEILL] = {
        .skills = {
            // [0] = SID_AbsorbMagic
        },
    },

    [CHARACTER_INNES] = {
    },

    [CHARACTER_SALEH] = {
    },

    [CHARACTER_RIEV] = {
    },
};

const struct SkillPreloadJConf gSkillPreloadJData[0x100] = {
    [CLASS_SNIPER] = {
    },

    [CLASS_SNIPER_F] = {
    },

    [CLASS_PALADIN] = {
    }
};
