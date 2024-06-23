#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/efx-skills.h"

const struct SkillAnimInfo gSkillAnimInfos_Person[0x100] = {

#if (defined(SID_Inori) && PERSON_SKILL_VALID(SID_Inori))
    [SKILL_INDEX_REAL(SID_Inori)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Armsthrift) && PERSON_SKILL_VALID(SID_Armsthrift))
    [SKILL_INDEX_REAL(SID_Armsthrift)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif
};
