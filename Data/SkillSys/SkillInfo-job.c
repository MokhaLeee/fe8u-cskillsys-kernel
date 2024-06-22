#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "constants/gfx.h"

const struct SkillInfo gSkillInfos_Job[0x100] = {
#if (defined(SID_GreatShield) && JOB_SKILL_VALID(SID_GreatShield)) 
    [SKILL_INDEX_REAL(SID_GreatShield)] = {
        .desc = MSG_SKILL_GreatShield,
        .icon = GFX_SkillIcon_GreatShield,
    },
#endif

#if (defined(SID_HoneCavalry) && JOB_SKILL_VALID(SID_HoneCavalry))
    [SKILL_INDEX_REAL(SID_HoneCavalry)] = {
        .name = MSG_SKILL_HoneCavalry_NAME,
        .desc = MSG_SKILL_HoneCavalry,
        .icon = GFX_SkillIcon_HoneCavalry
    },
#endif

#if (defined(SID_HoneFlier) && JOB_SKILL_VALID(SID_HoneFlier))
    [SKILL_INDEX_REAL(SID_HoneFlier)] = {
        .name = MSG_SKILL_HoneFlier_NAME,
        .desc = MSG_SKILL_HoneFlier,
        .icon = GFX_SkillIcon_HoneFlier
    },
#endif

#if (defined(SID_HoneArmor) && JOB_SKILL_VALID(SID_HoneArmor))
    [SKILL_INDEX_REAL(SID_HoneArmor)] = {
        .name = MSG_SKILL_HoneArmor_NAME,
        .desc = MSG_SKILL_HoneArmor,
        .icon = GFX_SkillIcon_HoneArmor
    },
#endif

#if (defined(SID_FortifyArmor) && JOB_SKILL_VALID(SID_FortifyArmor))
    [SKILL_INDEX_REAL(SID_FortifyArmor)] = {
        .name = MSG_SKILL_FortifyArmor_NAME,
        .desc = MSG_SKILL_FortifyArmor,
        .icon = GFX_SkillIcon_WIP
    },
#endif

#if (defined(SID_Shade) && JOB_SKILL_VALID(SID_Shade))
    [SKILL_INDEX_REAL(SID_Shade)] = {
        .desc = MSG_SKILL_Shade,
        .icon = GFX_SkillIcon_Shade,
    },
#endif

#if (defined(SID_ShadePlus) && JOB_SKILL_VALID(SID_ShadePlus))
    [SKILL_INDEX_REAL(SID_ShadePlus)] = {
        .desc = MSG_SKILL_ShadePlus,
        .icon = GFX_SkillIcon_ShadePlus,
    },
#endif

#if (defined(SID_Provoke) && JOB_SKILL_VALID(SID_Provoke))
    [SKILL_INDEX_REAL(SID_Provoke)] = {
        .desc = MSG_SKILL_Provoke,
        .icon = GFX_SkillIcon_Provoke,
    },
#endif

#if (defined(SID_Teleportation) && JOB_SKILL_VALID(SID_Teleportation))
    [SKILL_INDEX_REAL(SID_Teleportation)] = {
        .name = MSG_SKILL_Teleportation_NAME,
        .desc = MSG_SKILL_Teleportation,
        .icon = GFX_SkillIcon_Teleportation,
    },
#endif

#if (defined(SID_Dance) && JOB_SKILL_VALID(SID_Dance))
    [SKILL_INDEX_REAL(SID_Dance)] = {
        .desc = MSG_SKILL_Dance,
        .icon = GFX_SkillIcon_Dance,
    },
#endif

#if (defined(SID_LockTouch) && JOB_SKILL_VALID(SID_LockTouch))
    [SKILL_INDEX_REAL(SID_LockTouch)] = {
        .desc = MSG_SKILL_LockTouch,
        .icon = GFX_SkillIcon_LockTouch,
    },
#endif

#if (defined(SID_Summon) && JOB_SKILL_VALID(SID_Summon))
    [SKILL_INDEX_REAL(SID_Summon)] = {
        .desc = MSG_SKILL_Summon,
        .icon = GFX_SkillIcon_Summon,
    },
#endif
};
