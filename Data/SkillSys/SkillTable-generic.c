#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    [CHARACTER_EIRIKA] = {
        // .skills = {
        //     [0] = SID_Resurrection,
        // },
    },
    [CHARACTER_EPHRAIM] = {
    },

    [CHARACTER_SETH] = {
        .skills = {
            [0] = SID_Canto,
        },
    },

    [CHARACTER_ONEILL] = {
        // .skills = {
        //     [0] = SID_Resurrection,
        // },
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
