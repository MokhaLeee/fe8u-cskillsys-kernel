#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/efx-skills.h"

const struct SkillAnimInfo gSkillAnimInfos_Job[0x100] = {

#if (defined(SID_GreatShield) && JOB_SKILL_VALID(SID_GreatShield))
    [SKILL_INDEX_REAL(SID_GreatShield)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

};
