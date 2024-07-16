#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/efx-skills.h"

const struct SkillAnimInfo gSkillAnimInfos[MAX_SKILL_NUM + 1] = {
#if (defined(SID_RuinedBlade) && COMMON_SKILL_VALID(SID_RuinedBlade))
    [SID_RuinedBlade] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Vantage) && COMMON_SKILL_VALID(SID_Vantage))
    [SID_Vantage] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_DragonSkin) && COMMON_SKILL_VALID(SID_DragonSkin))
    [SID_DragonSkin] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_KeenFighter) && COMMON_SKILL_VALID(SID_KeenFighter))
    [SID_KeenFighter] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Desperation) && COMMON_SKILL_VALID(SID_Desperation))
    [SID_Desperation] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_QuickRiposte) && COMMON_SKILL_VALID(SID_QuickRiposte))
    [SID_QuickRiposte] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_WaryFighter) && COMMON_SKILL_VALID(SID_WaryFighter))
    [SID_WaryFighter] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_DoubleLion) && COMMON_SKILL_VALID(SID_DoubleLion))
    [SID_DoubleLion] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Adept) && COMMON_SKILL_VALID(SID_Adept))
    [SID_Adept] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Astra) && COMMON_SKILL_VALID(SID_Astra))
    [SID_Astra] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Aegis) && COMMON_SKILL_VALID(SID_Aegis))
    [SID_Aegis] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Pavise) && COMMON_SKILL_VALID(SID_Pavise))
    [SID_Pavise] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Bane) && COMMON_SKILL_VALID(SID_Bane))
    [SID_Bane] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_HIGH,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Mercy) && COMMON_SKILL_VALID(SID_Mercy))
    [SID_Mercy] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGH,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Aether) && COMMON_SKILL_VALID(SID_Aether))
    [SID_Aether] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Corona) && COMMON_SKILL_VALID(SID_Corona))
    [SID_Corona] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Luna) && COMMON_SKILL_VALID(SID_Luna))
    [SID_Luna] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Flare) && COMMON_SKILL_VALID(SID_Flare))
    [SID_Flare] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Sol) && COMMON_SKILL_VALID(SID_Sol))
    [SID_Sol] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_DragonFang) && COMMON_SKILL_VALID(SID_DragonFang))
    [SID_DragonFang] = {
        .aid = EFX_SKILL_DRAGON_FANG,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Colossus) && COMMON_SKILL_VALID(SID_Colossus))
    [SID_Colossus] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Corrosion) && COMMON_SKILL_VALID(SID_Corrosion))
    [SID_Corrosion] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Ignis) && COMMON_SKILL_VALID(SID_Ignis))
    [SID_Ignis] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Impale) && COMMON_SKILL_VALID(SID_Impale))
    [SID_Impale] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Glacies) && COMMON_SKILL_VALID(SID_Glacies))
    [SID_Glacies] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Vengeance) && COMMON_SKILL_VALID(SID_Vengeance))
    [SID_Vengeance] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Deadeye) && COMMON_SKILL_VALID(SID_Deadeye))
    [SID_Deadeye] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_AxeFaith) && COMMON_SKILL_VALID(SID_AxeFaith))
    [SID_AxeFaith] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Inori) && COMMON_SKILL_VALID(SID_Inori))
    [SID_Inori] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Armsthrift) && COMMON_SKILL_VALID(SID_Armsthrift))
    [SID_Armsthrift] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_GreatShield) && COMMON_SKILL_VALID(SID_GreatShield))
    [SID_GreatShield] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_SureShot) && COMMON_SKILL_VALID(SID_SureShot))
    [SID_SureShot] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
    [SID_RuinedBladePlus] = {
        .aid = EFX_SKILL_MOKHA,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_GuardBearing) && COMMON_SKILL_VALID(SID_GuardBearing))
    [SID_GuardBearing] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_VengefulFighter) && COMMON_SKILL_VALID(SID_VengefulFighter))
    [SID_VengefulFighter] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_RecklessFighter) && COMMON_SKILL_VALID(SID_RecklessFighter))
    [SID_RecklessFighter] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_BoldFighter) && COMMON_SKILL_VALID(SID_BoldFighter))
    [SID_BoldFighter] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Counter) && COMMON_SKILL_VALID(SID_Counter))
    [SID_Counter] = {
        .aid = EFX_SKILL_AETHER,
        .priority = EFX_PRIORITY_HIGH,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_Multiscale) && COMMON_SKILL_VALID(SID_Multiscale))
    [SID_Multiscale] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif

#if (defined(SID_TowerShieldPlus) && COMMON_SKILL_VALID(SID_TowerShieldPlus))
    [SID_TowerShieldPlus] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_NORMAL,
        .sfx = 0x3D1,
    },
#endif
};
