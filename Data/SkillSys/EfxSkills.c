#include "global.h"
#include "common-chax.h"

#include "efx-anim.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/efx-skills.h"

struct EfxAnimConf const * const gEfxSkillAnims[0x100] = {
    [EFX_SKILL_DEFAULT] = &EfxSkillVanilla,
    [EFX_COMBAT_DEFAULT] = &EfxSkillCombat,
    [EFX_SKILL_MOKHA] = &EfxSkillMokha,
    [EFX_SKILL_AETHER] = &EfxSkillAether,
    [EFX_SKILL_DRAGON_FANG] = &EfxSkillDragonFang,
};

FEB_IDENTIFIER(gEfxSkillAnims);

const struct SkillAnimInfo gSkillAnimInfos[0x100] = {
    [SID_RuinedBlade] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
    [SID_RuinedBladePlus] = {
        .aid = EFX_SKILL_MOKHA,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
    [SID_Vantage] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Desperation] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_QuickRiposte] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_WaryFighter] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_DoubleLion] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Adept] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Astra] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGH,
        .sfx = 0x3D1,
    },
    [SID_Aegis] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Pavise] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Bane] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Aether] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Corona] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Luna] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Flare] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Sol] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_DragonFang] = {
        .aid = EFX_SKILL_DRAGON_FANG,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
    [SID_Inori] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
};

FEB_IDENTIFIER(gSkillAnimInfos);
