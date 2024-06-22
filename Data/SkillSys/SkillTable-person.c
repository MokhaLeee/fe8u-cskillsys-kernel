#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u8 gConstSkillTable_Person[0x100][2] = {
    [CHARACTER_EIRIKA] = {
        SKILL_INDEX_REAL(SID_Supply),
        SKILL_INDEX_REAL(SID_Inori),
    },

    [CHARACTER_LYON_CC] = {
        SKILL_INDEX_REAL(SID_AversaNight),
    },

    [CHARACTER_LYON] = {
        SKILL_INDEX_REAL(SID_AversaNight),
    },
};
