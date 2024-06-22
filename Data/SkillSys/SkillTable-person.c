#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u8 gConstSkillTable_Person[0x100][2] = {
    [CHARACTER_EIRIKA] = {
#if (defined(SID_Supply) && PERSON_SKILL_VALID(SID_Supply))
        SKILL_INDEX_REAL(SID_Supply),
#endif
#if (defined(SID_Bane) && PERSON_SKILL_VALID(SID_Bane))
        SKILL_INDEX_REAL(SID_Bane)
#endif
    },

    [CHARACTER_SETH] = {
#if (defined(SID_Vantage) && PERSON_SKILL_VALID(SID_Vantage))
        SKILL_INDEX_REAL(SID_Vantage)
#endif
    },
};
