#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

/* Maximum of 5 skills here */
const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    [CHARACTER_EIRIKA] = {
        .skills = {
            // [0] = SID_Supply,
            // [1] = SID_GoddessDance,
            // [2] = SID_DefiantStr,
            // [3] = SID_DefiantAvoid,
            // [4] = SID_AdaptiveLunge,
            // [0] = SID_Shove,
            // [1] = SID_GridMasterAtk,
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
            [0] = SID_Counterattack,
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
