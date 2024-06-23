#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/efx-skills.h"

const struct SkillAnimInfo gSkillAnimInfos_Generic[0x100] = {
#if (defined(SID_RuinedBlade) && GENERIC_SKILL_VALID(SID_RuinedBlade))
    [SKILL_INDEX_REAL(SID_RuinedBlade)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Vantage) && GENERIC_SKILL_VALID(SID_Vantage))
    [SKILL_INDEX_REAL(SID_Vantage)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_DragonSkin) && GENERIC_SKILL_VALID(SID_DragonSkin))
    [SKILL_INDEX_REAL(SID_DragonSkin)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_KeenFighter) && GENERIC_SKILL_VALID(SID_KeenFighter))
    [SKILL_INDEX_REAL(SID_KeenFighter)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Desperation) && GENERIC_SKILL_VALID(SID_Desperation))
    [SKILL_INDEX_REAL(SID_Desperation)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_QuickRiposte) && GENERIC_SKILL_VALID(SID_QuickRiposte))
    [SKILL_INDEX_REAL(SID_QuickRiposte)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_WaryFighter) && GENERIC_SKILL_VALID(SID_WaryFighter))
    [SKILL_INDEX_REAL(SID_WaryFighter)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_DoubleLion) && GENERIC_SKILL_VALID(SID_DoubleLion))
    [SKILL_INDEX_REAL(SID_DoubleLion)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Adept) && GENERIC_SKILL_VALID(SID_Adept))
    [SKILL_INDEX_REAL(SID_Adept)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Astra) && GENERIC_SKILL_VALID(SID_Astra))
    [SKILL_INDEX_REAL(SID_Astra)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Aegis) && GENERIC_SKILL_VALID(SID_Aegis))
    [SKILL_INDEX_REAL(SID_Aegis)] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Pavise) && GENERIC_SKILL_VALID(SID_Pavise))
    [SKILL_INDEX_REAL(SID_Pavise)] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Bane) && GENERIC_SKILL_VALID(SID_Bane))
    [SKILL_INDEX_REAL(SID_Bane)] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_HIGH,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Mercy) && GENERIC_SKILL_VALID(SID_Mercy))
    [SKILL_INDEX_REAL(SID_Mercy)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGH,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Aether) && GENERIC_SKILL_VALID(SID_Aether))
    [SKILL_INDEX_REAL(SID_Aether)] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Corona) && GENERIC_SKILL_VALID(SID_Corona))
    [SKILL_INDEX_REAL(SID_Corona)] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Luna) && GENERIC_SKILL_VALID(SID_Luna))
    [SKILL_INDEX_REAL(SID_Luna)] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Flare) && GENERIC_SKILL_VALID(SID_Flare))
    [SKILL_INDEX_REAL(SID_Flare)] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Sol) && GENERIC_SKILL_VALID(SID_Sol))
    [SKILL_INDEX_REAL(SID_Sol)] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_DragonFang) && GENERIC_SKILL_VALID(SID_DragonFang))
    [SKILL_INDEX_REAL(SID_DragonFang)] = {
        .aid = EFX_SKILL_DRAGON_FANG,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Colossus) && GENERIC_SKILL_VALID(SID_Colossus))
    [SKILL_INDEX_REAL(SID_Colossus)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Corrosion) && GENERIC_SKILL_VALID(SID_Corrosion))
    [SKILL_INDEX_REAL(SID_Corrosion)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Ignis) && GENERIC_SKILL_VALID(SID_Ignis))
    [SKILL_INDEX_REAL(SID_Ignis)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Impale) && GENERIC_SKILL_VALID(SID_Impale))
    [SKILL_INDEX_REAL(SID_Impale)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Glacies) && GENERIC_SKILL_VALID(SID_Glacies))
    [SKILL_INDEX_REAL(SID_Glacies)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Vengeance) && GENERIC_SKILL_VALID(SID_Vengeance))
    [SKILL_INDEX_REAL(SID_Vengeance)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_AxeFaith) && GENERIC_SKILL_VALID(SID_AxeFaith))
    [SKILL_INDEX_REAL(SID_AxeFaith)] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif
};
