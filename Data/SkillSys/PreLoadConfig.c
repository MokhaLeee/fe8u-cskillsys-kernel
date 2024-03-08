#include "global.h"
#include "constants/characters.h"
#include "constants/classes.h"

#include "skill-system.h"
#include "constants/skills.h"

const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    [CHARACTER_EIRIKA] = {
        .skills = {
            [0] = SID_SpdBonus,
            [5] = SID_DefiantAvoid,
        },
    },
};

FEB_IDENTIFIER(gSkillPData);

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
};

FEB_IDENTIFIER(gSkillJData);
