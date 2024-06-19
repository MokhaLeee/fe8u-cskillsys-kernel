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

const struct SkillAnimInfo gSkillAnimInfos[0x100] = {
#if defined(SID_RuinedBlade) && (SID_RuinedBlade < MAX_SKILL_NUM)
    [SID_RuinedBlade] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_RuinedBladePlus) && (SID_RuinedBladePlus < MAX_SKILL_NUM)
    [SID_RuinedBladePlus] = {
        .aid = EFX_SKILL_MOKHA,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Vantage) && (SID_Vantage < MAX_SKILL_NUM)
    [SID_Vantage] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_DragonSkin) && (SID_DragonSkin < MAX_SKILL_NUM)
    [SID_DragonSkin] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_KeenFighter) && (SID_KeenFighter < MAX_SKILL_NUM)
    [SID_KeenFighter] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Desperation) && (SID_Desperation < MAX_SKILL_NUM)
    [SID_Desperation] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_QuickRiposte) && (SID_QuickRiposte < MAX_SKILL_NUM)
    [SID_QuickRiposte] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_WaryFighter) && (SID_WaryFighter < MAX_SKILL_NUM)
    [SID_WaryFighter] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_DoubleLion) && (SID_DoubleLion < MAX_SKILL_NUM)
    [SID_DoubleLion] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Adept) && (SID_Adept < MAX_SKILL_NUM)
    [SID_Adept] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Astra) && (SID_Astra < MAX_SKILL_NUM)
    [SID_Astra] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGH,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Aegis) && (SID_Aegis < MAX_SKILL_NUM)
    [SID_Aegis] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Pavise) && (SID_Pavise < MAX_SKILL_NUM)
    [SID_Pavise] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Bane) && (SID_Bane < MAX_SKILL_NUM)
    [SID_Bane] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_HIGH,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Mercy) && (SID_Mercy < MAX_SKILL_NUM)
    [SID_Mercy] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGH,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Aether) && (SID_Aether < MAX_SKILL_NUM)
    [SID_Aether] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Corona) && (SID_Corona < MAX_SKILL_NUM)
    [SID_Corona] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Luna) && (SID_Luna < MAX_SKILL_NUM)
    [SID_Luna] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Flare) && (SID_Flare < MAX_SKILL_NUM)
    [SID_Flare] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Sol) && (SID_Sol < MAX_SKILL_NUM)
    [SID_Sol] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_DragonFang) && (SID_DragonFang < MAX_SKILL_NUM)
    [SID_DragonFang] = {
        .aid = EFX_SKILL_DRAGON_FANG,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Inori) && (SID_Inori < MAX_SKILL_NUM)
    [SID_Inori] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Colossus) && (SID_Colossus < MAX_SKILL_NUM)
    [SID_Colossus] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Corrosion) && (SID_Corrosion < MAX_SKILL_NUM)
    [SID_Corrosion] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Ignis) && (SID_Ignis < MAX_SKILL_NUM)
    [SID_Ignis] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if defined(SID_Impale) && (SID_Impale < MAX_SKILL_NUM)
    [SID_Impale] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif
};
