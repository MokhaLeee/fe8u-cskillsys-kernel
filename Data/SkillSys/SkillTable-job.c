#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u8 gConstSkillTable_Job[0x100][2] = {
    [CLASS_PALADIN] = {
#if (defined(SID_HoneCavalry) && JOB_SKILL_VALID(SID_HoneCavalry))
        SKILL_INDEX_REAL(SID_HoneCavalry)
#endif
    },

    [CLASS_PALADIN_F] = {
#if (defined(SID_HoneCavalry) && JOB_SKILL_VALID(SID_HoneCavalry))
        SKILL_INDEX_REAL(SID_HoneCavalry)
#endif
    },

    [CLASS_FIGHTER] = {
#if (defined(SID_Teleportation) && JOB_SKILL_VALID(SID_Teleportation))
        SKILL_INDEX_REAL(SID_Teleportation)
#endif
    },

    [CLASS_PEGASUS_KNIGHT] = {
#if (defined(SID_HoneFlier) && JOB_SKILL_VALID(SID_HoneFlier))
        SKILL_INDEX_REAL(SID_HoneFlier)
#endif
    },

    [CLASS_ASSASSIN] = {
#if (defined(SID_Lethality) && JOB_SKILL_VALID(SID_Lethality))
        SKILL_INDEX_REAL(SID_Lethality)
#endif
    },

    [CLASS_ASSASSIN_F] = {
#if (defined(SID_Lethality) && JOB_SKILL_VALID(SID_Lethality))
        SKILL_INDEX_REAL(SID_Lethality)
#endif
    },
};
