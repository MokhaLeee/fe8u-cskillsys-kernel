#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "constants/gfx.h"

const struct SkillInfo gSkillInfos_Person[0x100] = {
#if (defined(SID_AversaNight) && PERSON_SKILL_VALID(SID_AversaNight))
    [SKILL_INDEX_REAL(SID_AversaNight)] = {
        .name = MSG_SKILL_DEBUFF_Aversa_NAME,
        .desc = MSG_SKILL_DEBUFF_Aversa,
        .icon = GFX_SkillIcon_AversaNight,
    },
#endif

#if (defined(SID_Supply) && PERSON_SKILL_VALID(SID_Supply))
    [SKILL_INDEX_REAL(SID_Supply)] = {
        .desc = MSG_SKILL_Supply,
        .icon = GFX_SkillIcon_Supply,
    },
#endif

#if (defined(SID_Inori) && PERSON_SKILL_VALID(SID_Inori))
    [SKILL_INDEX_REAL(SID_Inori)] = {
        .desc = MSG_SKILL_Inori,
        .icon = GFX_SkillIcon_WIP,
    },
#endif
};
