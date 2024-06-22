#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "constants/gfx.h"

const struct SkillInfo gSkillInfos_Generic[0x100] = {
#if (defined(SID_HpBonus) && GENERIC_SKILL_VALID(SID_HpBonus))
    [SKILL_INDEX_REAL(SID_HpBonus)] = {
        .desc = MSG_SKILL_HpBonus,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_StrBonus) && GENERIC_SKILL_VALID(SID_StrBonus))
    [SKILL_INDEX_REAL(SID_StrBonus)] = {
        .desc = MSG_SKILL_StrBonus,
        .icon = GFX_SkillIcon_StrBonus,
    },
#endif

#if (defined(SID_MagBonus) && GENERIC_SKILL_VALID(SID_MagBonus))
    [SKILL_INDEX_REAL(SID_MagBonus)] = {
        .desc = MSG_SKILL_MagBonus,
        .icon = GFX_SkillIcon_MagBonus,
    },
#endif

#if (defined(SID_SklBonus) && GENERIC_SKILL_VALID(SID_SklBonus))
    [SKILL_INDEX_REAL(SID_SklBonus)] = {
        .desc = MSG_SKILL_SklBonus,
        .icon = GFX_SkillIcon_SklBonus,
    },
#endif

#if (defined(SID_SpdBonus) && GENERIC_SKILL_VALID(SID_SpdBonus))
    [SKILL_INDEX_REAL(SID_SpdBonus)] = {
        .desc = MSG_SKILL_SpdBonus,
        .icon = GFX_SkillIcon_SpdBonus,
    },
#endif

#if (defined(SID_LckBonus) && GENERIC_SKILL_VALID(SID_LckBonus))
    [SKILL_INDEX_REAL(SID_LckBonus)] = {
        .desc = MSG_SKILL_LukBonus,
        .icon = GFX_SkillIcon_LukBonus,
    },
#endif

#if (defined(SID_DefBonus) && GENERIC_SKILL_VALID(SID_DefBonus))
    [SKILL_INDEX_REAL(SID_DefBonus)] = {
        .desc = MSG_SKILL_DefBonus,
        .icon = GFX_SkillIcon_DefBonus,
    },
#endif

#if (defined(SID_ResBonus) && GENERIC_SKILL_VALID(SID_ResBonus))
    [SKILL_INDEX_REAL(SID_ResBonus)] = {
        .desc = MSG_SKILL_ResBonus,
        .icon = GFX_SkillIcon_ResBonus,
    },
#endif

#if (defined(SID_MovBonus) && GENERIC_SKILL_VALID(SID_MovBonus))
    [SKILL_INDEX_REAL(SID_MovBonus)] = {
        .desc = MSG_SKILL_MovBonus,
        .icon = GFX_SkillIcon_MovBonus,
    },
#endif

#if (defined(SID_DefiantStr) && GENERIC_SKILL_VALID(SID_DefiantStr))
    [SKILL_INDEX_REAL(SID_DefiantStr)] = {
        .desc = MSG_SKILL_DefiantStr,
        .icon = GFX_SkillIcon_DefiantStr,
    },
#endif

#if (defined(SID_DefiantMag) && GENERIC_SKILL_VALID(SID_DefiantMag))
    [SKILL_INDEX_REAL(SID_DefiantMag)] = {
        .desc = MSG_SKILL_DefiantMag,
        .icon = GFX_SkillIcon_DefiantMag,
    },
#endif

#if (defined(SID_DefiantSkl) && GENERIC_SKILL_VALID(SID_DefiantSkl))
    [SKILL_INDEX_REAL(SID_DefiantSkl)] = {
        .desc = MSG_SKILL_DefiantSkl,
        .icon = GFX_SkillIcon_DefiantSkl,
    },
#endif

#if (defined(SID_DefiantSpd) && GENERIC_SKILL_VALID(SID_DefiantSpd))
    [SKILL_INDEX_REAL(SID_DefiantSpd)] = {
        .desc = MSG_SKILL_DefiantSpd,
        .icon = GFX_SkillIcon_DefiantSpd,
    },
#endif

#if (defined(SID_DefiantLck) && GENERIC_SKILL_VALID(SID_DefiantLck))
    [SKILL_INDEX_REAL(SID_DefiantLck)] = {
        .desc = MSG_SKILL_DefiantLck,
        .icon = GFX_SkillIcon_DefiantLck,
    },
#endif

#if (defined(SID_DefiantDef) && GENERIC_SKILL_VALID(SID_DefiantDef))
    [SKILL_INDEX_REAL(SID_DefiantDef)] = {
        .desc = MSG_SKILL_DefiantDef,
        .icon = GFX_SkillIcon_DefiantDef,
    },
#endif

#if (defined(SID_DefiantRes) && GENERIC_SKILL_VALID(SID_DefiantRes))
    [SKILL_INDEX_REAL(SID_DefiantRes)] = {
        .desc = MSG_SKILL_DefiantRes,
        .icon = GFX_SkillIcon_DefiantRes,
    },
#endif

#if (defined(SID_DefiantCrit) && GENERIC_SKILL_VALID(SID_DefiantCrit))
    [SKILL_INDEX_REAL(SID_DefiantCrit)] = {
        .desc = MSG_SKILL_DefiantCrit,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_DefiantAvoid) && GENERIC_SKILL_VALID(SID_DefiantAvoid))
    [SKILL_INDEX_REAL(SID_DefiantAvoid)] = {
        .desc = MSG_SKILL_DefiantAvoid,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Fury) && GENERIC_SKILL_VALID(SID_Fury))
    [SKILL_INDEX_REAL(SID_Fury)] = {
        .desc = MSG_SKILL_Fury,
        .icon = GFX_SkillIcon_Fury,
    },
#endif

#if (defined(SID_FuryPlus) && GENERIC_SKILL_VALID(SID_FuryPlus))
    [SKILL_INDEX_REAL(SID_FuryPlus)] = {
        .desc = MSG_SKILL_FuryPlus,
        .icon = GFX_SkillIcon_FuryPlus,
    },
#endif

#if (defined(SID_FortressDef) && GENERIC_SKILL_VALID(SID_FortressDef))
    [SKILL_INDEX_REAL(SID_FortressDef)] = {
        .desc = MSG_SKILL_FortressDef,
        .icon = GFX_SkillIcon_FortressDef,
    },
#endif

#if (defined(SID_FortressRes) && GENERIC_SKILL_VALID(SID_FortressRes))
    [SKILL_INDEX_REAL(SID_FortressRes)] = {
        .desc = MSG_SKILL_FortressRes,
        .icon = GFX_SkillIcon_FortressRes,
    },
#endif

#if (defined(SID_LifeAndDeath) && GENERIC_SKILL_VALID(SID_LifeAndDeath))
    [SKILL_INDEX_REAL(SID_LifeAndDeath)] = {
        .desc = MSG_SKILL_LifeAndDeath,
        .icon = GFX_SkillIcon_LifeAndDeath,
    },
#endif

#if (defined(SID_LuckySeven) && GENERIC_SKILL_VALID(SID_LuckySeven))
    [SKILL_INDEX_REAL(SID_LuckySeven)] = {
        .desc = MSG_SKILL_LuckySeven,
        .icon = GFX_SkillIcon_LuckySeven,
    },
#endif

#if (defined(SID_Nullify) && GENERIC_SKILL_VALID(SID_Nullify))
    [SKILL_INDEX_REAL(SID_Nullify)] = {
        .desc = MSG_SKILL_Nullify,
        .icon = GFX_SkillIcon_Nullify,
    },
#endif

#if (defined(SID_Slayer) && GENERIC_SKILL_VALID(SID_Slayer))
    [SKILL_INDEX_REAL(SID_Slayer)] = {
        .desc = MSG_SKILL_Slayer,
        .icon = GFX_SkillIcon_Slayer,
    },
#endif

#if (defined(SID_Skybreaker) && GENERIC_SKILL_VALID(SID_Skybreaker))
    [SKILL_INDEX_REAL(SID_Skybreaker)] = {
        .name = MSG_SKILL_Skybreaker_NAME,
        .desc = MSG_SKILL_Skybreaker,
        .icon = GFX_SkillIcon_Skybreaker,
    },
#endif

#if (defined(SID_Resourceful) && GENERIC_SKILL_VALID(SID_Resourceful))
    [SKILL_INDEX_REAL(SID_Resourceful)] = {
        .name = MSG_SKILL_Resourceful_NAME,
        .desc = MSG_SKILL_Resourceful,
        .icon = GFX_SkillIcon_Resourceful,
    },
#endif

#if (defined(SID_Lethality) && GENERIC_SKILL_VALID(SID_Lethality))
    [SKILL_INDEX_REAL(SID_Lethality)] = {
        .desc = MSG_SKILL_Lethality,
        .icon = GFX_SkillIcon_Lethality,
    },
#endif

#if (defined(SID_Crit) && GENERIC_SKILL_VALID(SID_Crit))
    [SKILL_INDEX_REAL(SID_Crit)] = {
        .desc = MSG_SKILL_Crit,
        .icon = GFX_SkillIcon_Crit,
    },
#endif

#if (defined(SID_WatchfulEye) && GENERIC_SKILL_VALID(SID_WatchfulEye))
    [SKILL_INDEX_REAL(SID_WatchfulEye)] = {
        .desc = MSG_SKILL_WatchfulEye,
        .icon = GFX_SkillIcon_WatchfulEye,
    },
#endif

#if (defined(SID_CritSword) && GENERIC_SKILL_VALID(SID_CritSword))
    [SKILL_INDEX_REAL(SID_CritSword)] = {
        .desc = MSG_SKILL_CritSword,
        .icon = GFX_SkillIcon_CritSword,
    },
#endif

#if (defined(SID_CritAxe) && GENERIC_SKILL_VALID(SID_CritAxe))
    [SKILL_INDEX_REAL(SID_CritAxe)] = {
        .desc = MSG_SKILL_CritAxe,
        .icon = GFX_SkillIcon_CritAxe,
    },
#endif

#if (defined(SID_CritLance) && GENERIC_SKILL_VALID(SID_CritLance))
    [SKILL_INDEX_REAL(SID_CritLance)] = {
        .desc = MSG_SKILL_CritLance,
        .icon = GFX_SkillIcon_CritLance,
    },
#endif

#if (defined(SID_CritBow) && GENERIC_SKILL_VALID(SID_CritBow))
    [SKILL_INDEX_REAL(SID_CritBow)] = {
        .desc = MSG_SKILL_CritBow,
        .icon = GFX_SkillIcon_CritBow,
    },
#endif

#if (defined(SID_FaireSword) && GENERIC_SKILL_VALID(SID_FaireSword))
    [SKILL_INDEX_REAL(SID_FaireSword)] = {
        .desc = MSG_SKILL_FaireSword,
        .icon = GFX_SkillIcon_FaireSword,
    },
#endif

#if (defined(SID_FaireLance) && GENERIC_SKILL_VALID(SID_FaireLance))
    [SKILL_INDEX_REAL(SID_FaireLance)] = {
        .desc = MSG_SKILL_FaireLance,
        .icon = GFX_SkillIcon_FaireLance,
    },
#endif

#if (defined(SID_FaireAxe) && GENERIC_SKILL_VALID(SID_FaireAxe))
    [SKILL_INDEX_REAL(SID_FaireAxe)] = {
        .desc = MSG_SKILL_FaireAxe,
        .icon = GFX_SkillIcon_FaireAxe,
    },
#endif

#if (defined(SID_FaireBow) && GENERIC_SKILL_VALID(SID_FaireBow))
    [SKILL_INDEX_REAL(SID_FaireBow)] = {
        .desc = MSG_SKILL_FaireBow,
        .icon = GFX_SkillIcon_FaireBow,
    },
#endif

#if (defined(SID_FaireBMag) && GENERIC_SKILL_VALID(SID_FaireBMag))
    [SKILL_INDEX_REAL(SID_FaireBMag)] = {
        .desc = MSG_SKILL_FaireBMag,
        .icon = GFX_SkillIcon_FaireBMag,
    },
#endif

#if (defined(SID_Avoid) && GENERIC_SKILL_VALID(SID_Avoid))
    [SKILL_INDEX_REAL(SID_Avoid)] = {
        .desc = MSG_SKILL_Avoid,
        .icon = GFX_SkillIcon_Avoid,
    },
#endif

#if (defined(SID_AvoidSword) && GENERIC_SKILL_VALID(SID_AvoidSword))
    [SKILL_INDEX_REAL(SID_AvoidSword)] = {
        .desc = MSG_SKILL_AvoidSword,
        .icon = GFX_SkillIcon_AvoidSword,
    },
#endif

#if (defined(SID_RuinedBlade) && GENERIC_SKILL_VALID(SID_RuinedBlade))
    [SKILL_INDEX_REAL(SID_RuinedBlade)] = {
        .desc = MSG_SKILL_RuinedBlade,
        .icon = GFX_SkillIcon_RuinedBlade,
    },
#endif

#if (defined(SID_InfinityEdge) && GENERIC_SKILL_VALID(SID_InfinityEdge))
    [SKILL_INDEX_REAL(SID_InfinityEdge)] = {
        .desc = MSG_SKILL_InfinityEdge,
        .icon = GFX_SkillIcon_InfinityEdge,
    },
#endif

#if (defined(SID_HeavyBlade) && GENERIC_SKILL_VALID(SID_HeavyBlade))
    [SKILL_INDEX_REAL(SID_HeavyBlade)] = {
        .desc = MSG_SKILL_HeavyBlade,
        .icon = GFX_SkillIcon_HeavyBlade,
    },
#endif

#if (defined(SID_FlashingBlade) && GENERIC_SKILL_VALID(SID_FlashingBlade))
    [SKILL_INDEX_REAL(SID_FlashingBlade)] = {
        .desc = MSG_SKILL_FlashingBlade,
        .icon = GFX_SkillIcon_FlashingBlade,
    },
#endif

#if (defined(SID_HeavyBladePlus) && GENERIC_SKILL_VALID(SID_HeavyBladePlus))
    [SKILL_INDEX_REAL(SID_HeavyBladePlus)] = {
        .desc = MSG_SKILL_HeavyBladePlus,
        .icon = GFX_SkillIcon_HeavyBladePlus,
    },
#endif

#if (defined(SID_FlashingBladePlus) && GENERIC_SKILL_VALID(SID_FlashingBladePlus))
    [SKILL_INDEX_REAL(SID_FlashingBladePlus)] = {
        .desc = MSG_SKILL_FlashingBladePlus,
        .icon = GFX_SkillIcon_FlashingBladePlus,
    },
#endif

#if (defined(SID_LunaAttack) && GENERIC_SKILL_VALID(SID_LunaAttack))
    [SKILL_INDEX_REAL(SID_LunaAttack)] = {
        .desc = MSG_SKILL_LunaAttack,
        .icon = GFX_SkillIcon_LunaAttack,
    },
#endif

#if (defined(SID_SorceryBlade) && GENERIC_SKILL_VALID(SID_SorceryBlade))
    [SKILL_INDEX_REAL(SID_SorceryBlade)] = {
        .desc = MSG_SKILL_SorceryBlade,
        .icon = GFX_SkillIcon_SorceryBlade,
    },
#endif

#if (defined(SID_Fortune) && GENERIC_SKILL_VALID(SID_Fortune))
    [SKILL_INDEX_REAL(SID_Fortune)] = {
        .desc = MSG_SKILL_Fortune,
        .icon = GFX_SkillIcon_Fortune,
    },
#endif

#if (defined(SID_BlueFlame) && GENERIC_SKILL_VALID(SID_BlueFlame))
    [SKILL_INDEX_REAL(SID_BlueFlame)] = {
        .desc = MSG_SKILL_BlueFlame,
        .icon = GFX_SkillIcon_BlueFlame,
    },
#endif

#if (defined(SID_SwordBreaker) && GENERIC_SKILL_VALID(SID_SwordBreaker))
    [SKILL_INDEX_REAL(SID_SwordBreaker)] = {
        .desc = MSG_SKILL_SwordBreaker,
        .icon = GFX_SkillIcon_SwordBreaker,
    },
#endif

#if (defined(SID_AxeBreaker) && GENERIC_SKILL_VALID(SID_AxeBreaker))
    [SKILL_INDEX_REAL(SID_AxeBreaker)] = {
        .desc = MSG_SKILL_AxeBreaker,
        .icon = GFX_SkillIcon_AxeBreaker,
    },
#endif

#if (defined(SID_LanceBreaker) && GENERIC_SKILL_VALID(SID_LanceBreaker))
    [SKILL_INDEX_REAL(SID_LanceBreaker)] = {
        .desc = MSG_SKILL_LanceBreaker,
        .icon = GFX_SkillIcon_LanceBreaker,
    },
#endif

#if (defined(SID_BowBreaker) && GENERIC_SKILL_VALID(SID_BowBreaker))
    [SKILL_INDEX_REAL(SID_BowBreaker)] = {
        .desc = MSG_SKILL_BowBreaker,
        .icon = GFX_SkillIcon_BowBreaker,
    },
#endif

#if (defined(SID_BMagBreaker) && GENERIC_SKILL_VALID(SID_BMagBreaker))
    [SKILL_INDEX_REAL(SID_BMagBreaker)] = {
        .desc = MSG_SKILL_BMagBreaker,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Vantage) && GENERIC_SKILL_VALID(SID_Vantage))
    [SKILL_INDEX_REAL(SID_Vantage)] = {
        .desc = MSG_SKILL_Vantage,
        .icon = GFX_SkillIcon_Vantage,
    },
#endif

#if (defined(SID_Desperation) && GENERIC_SKILL_VALID(SID_Desperation))
    [SKILL_INDEX_REAL(SID_Desperation)] = {
        .desc = MSG_SKILL_Desperation,
        .icon = GFX_SkillIcon_Desperation,
    },
#endif

#if (defined(SID_QuickRiposte) && GENERIC_SKILL_VALID(SID_QuickRiposte))
    [SKILL_INDEX_REAL(SID_QuickRiposte)] = {
        .name = MSA_SKILL_QuickRiposte_NAME,
        .desc = MSG_SKILL_QuickRiposte,
        .icon = GFX_SkillIcon_QuickRiposte,
    },
#endif

#if (defined(SID_WaryFighter) && GENERIC_SKILL_VALID(SID_WaryFighter))
    [SKILL_INDEX_REAL(SID_WaryFighter)] = {
        .desc = MSG_SKILL_WaryFighter,
        .icon = GFX_SkillIcon_WaryFighter,
    },
#endif

#if (defined(SID_DoubleLion) && GENERIC_SKILL_VALID(SID_DoubleLion))
    [SKILL_INDEX_REAL(SID_DoubleLion)] = {
        .name = MSG_SKILL_DoubleLion_NAME,
        .desc = MSG_SKILL_DoubleLion,
        .icon = GFX_SkillIcon_DoubleLion,
    },
#endif

#if (defined(SID_Adept) && GENERIC_SKILL_VALID(SID_Adept))
    [SKILL_INDEX_REAL(SID_Adept)] = {
        .desc = MSG_SKILL_Adept,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Astra) && GENERIC_SKILL_VALID(SID_Astra))
    [SKILL_INDEX_REAL(SID_Astra)] = {
        .desc = MSG_SKILL_Astra,
        .icon = GFX_SkillIcon_Astra,
    },
#endif

#if (defined(SID_Aegis) && GENERIC_SKILL_VALID(SID_Aegis))
    [SKILL_INDEX_REAL(SID_Aegis)] = {
        .desc = MSG_SKILL_Aegis,
        .icon = GFX_SkillIcon_Aegis,
    },
#endif

#if (defined(SID_Pavise) && GENERIC_SKILL_VALID(SID_Pavise))
    [SKILL_INDEX_REAL(SID_Pavise)] = {
        .desc = MSG_SKILL_Pavise,
        .icon = GFX_SkillIcon_Pavise,
    },
#endif

#if (defined(SID_Bane) && GENERIC_SKILL_VALID(SID_Bane))
    [SKILL_INDEX_REAL(SID_Bane)] = {
        .desc = MSG_SKILL_Bane,
        .icon = GFX_SkillIcon_Bane,
    },
#endif

#if (defined(SID_Mercy) && GENERIC_SKILL_VALID(SID_Mercy))
    [SKILL_INDEX_REAL(SID_Mercy)] = {
        .desc = MSG_SKILL_Mercy,
        .icon = GFX_SkillIcon_Mercy,
    },
#endif

#if (defined(SID_Aether) && GENERIC_SKILL_VALID(SID_Aether))
    [SKILL_INDEX_REAL(SID_Aether)] = {
        .desc = MSG_SKILL_Aether,
        .icon = GFX_SkillIcon_Aether,
    },
#endif

#if (defined(SID_Corona) && GENERIC_SKILL_VALID(SID_Corona))
    [SKILL_INDEX_REAL(SID_Corona)] = {
        .desc = MSG_SKILL_Corona,
        .icon = GFX_SkillIcon_Corona,
    },
#endif

#if (defined(SID_Luna) && GENERIC_SKILL_VALID(SID_Luna))
    [SKILL_INDEX_REAL(SID_Luna)] = {
        .desc = MSG_SKILL_Luna,
        .icon = GFX_SkillIcon_Luna,
    },
#endif

#if (defined(SID_Flare) && GENERIC_SKILL_VALID(SID_Flare))
    [SKILL_INDEX_REAL(SID_Flare)] = {
        .desc = MSG_SKILL_Flare,
        .icon = GFX_SkillIcon_Flare,
    },
#endif

#if (defined(SID_Sol) && GENERIC_SKILL_VALID(SID_Sol))
    [SKILL_INDEX_REAL(SID_Sol)] = {
        .desc = MSG_SKILL_Sol,
        .icon = GFX_SkillIcon_Sol,
    },
#endif

#if (defined(SID_DragonFang) && GENERIC_SKILL_VALID(SID_DragonFang))
    [SKILL_INDEX_REAL(SID_DragonFang)] = {
        .desc = MSG_SKILL_DragonFang,
        .icon = GFX_SkillIcon_DragonFang,
    },
#endif

#if (defined(SID_Foresight) && GENERIC_SKILL_VALID(SID_Foresight))
    [SKILL_INDEX_REAL(SID_Foresight)] = {
        .desc = MSG_SKILL_Foresight,
        .icon = GFX_SkillIcon_Foresight,
    },
#endif

#if (defined(SID_BlowDarting) && GENERIC_SKILL_VALID(SID_BlowDarting))
    [SKILL_INDEX_REAL(SID_BlowDarting)] = {
        .desc = MSG_SKILL_BlowDarting,
        .icon = GFX_SkillIcon_BlowDarting,
    },
#endif

#if (defined(SID_BlowDeath) && GENERIC_SKILL_VALID(SID_BlowDeath))
    [SKILL_INDEX_REAL(SID_BlowDeath)] = {
        .desc = MSG_SKILL_BlowDeath,
        .icon = GFX_SkillIcon_BlowDeath,
    },
#endif

#if (defined(SID_BlowArmored) && GENERIC_SKILL_VALID(SID_BlowArmored))
    [SKILL_INDEX_REAL(SID_BlowArmored)] = {
        .desc = MSG_SKILL_BlowArmored,
        .icon = GFX_SkillIcon_BlowArmored,
    },
#endif

#if (defined(SID_BlowFiendish) && GENERIC_SKILL_VALID(SID_BlowFiendish))
    [SKILL_INDEX_REAL(SID_BlowFiendish)] = {
        .desc = MSG_SKILL_BlowFiendish,
        .icon = GFX_SkillIcon_BlowFiendish,
    },
#endif

#if (defined(SID_BlowWarding) && GENERIC_SKILL_VALID(SID_BlowWarding))
    [SKILL_INDEX_REAL(SID_BlowWarding)] = {
        .desc = MSG_SKILL_BlowWarding,
        .icon = GFX_SkillIcon_BlowWarding,
    },
#endif

#if (defined(SID_BlowDuelist) && GENERIC_SKILL_VALID(SID_BlowDuelist))
    [SKILL_INDEX_REAL(SID_BlowDuelist)] = {
        .desc = MSG_SKILL_BlowDuelist,
        .icon = GFX_SkillIcon_BlowDuelist,
    },
#endif

#if (defined(SID_BlowUncanny) && GENERIC_SKILL_VALID(SID_BlowUncanny))
    [SKILL_INDEX_REAL(SID_BlowUncanny)] = {
        .desc = MSG_SKILL_BlowUncanny,
        .icon = GFX_SkillIcon_BlowUncanny,
    },
#endif

#if (defined(SID_BlowKilling) && GENERIC_SKILL_VALID(SID_BlowKilling))
    [SKILL_INDEX_REAL(SID_BlowKilling)] = {
        .desc = MSG_SKILL_BlowKilling,
        .icon = GFX_SkillIcon_BlowKilling,
    },
#endif

#if (defined(SID_StanceBracing) && GENERIC_SKILL_VALID(SID_StanceBracing))
    [SKILL_INDEX_REAL(SID_StanceBracing)] = {
        .desc = MSG_SKILL_StanceBracing,
        .icon = GFX_SkillIcon_StanceBracing,
    },
#endif

#if (defined(SID_StanceDarting) && GENERIC_SKILL_VALID(SID_StanceDarting))
    [SKILL_INDEX_REAL(SID_StanceDarting)] = {
        .desc = MSG_SKILL_StanceDarting,
        .icon = GFX_SkillIcon_StanceDarting,
    },
#endif

#if (defined(SID_StanceFierce) && GENERIC_SKILL_VALID(SID_StanceFierce))
    [SKILL_INDEX_REAL(SID_StanceFierce)] = {
        .desc = MSG_SKILL_StanceFierce,
        .icon = GFX_SkillIcon_StanceFierce,
    },
#endif

#if (defined(SID_StanceKestrel) && GENERIC_SKILL_VALID(SID_StanceKestrel))
    [SKILL_INDEX_REAL(SID_StanceKestrel)] = {
        .desc = MSG_SKILL_StanceKestrel,
        .icon = GFX_SkillIcon_StanceKestrel,
    },
#endif

#if (defined(SID_StanceMirror) && GENERIC_SKILL_VALID(SID_StanceMirror))
    [SKILL_INDEX_REAL(SID_StanceMirror)] = {
        .desc = MSG_SKILL_StanceMirror,
        .icon = GFX_SkillIcon_StanceMirror,
    },
#endif

#if (defined(SID_StanceReady) && GENERIC_SKILL_VALID(SID_StanceReady))
    [SKILL_INDEX_REAL(SID_StanceReady)] = {
        .desc = MSG_SKILL_StanceReady,
        .icon = GFX_SkillIcon_StanceReady,
    },
#endif

#if (defined(SID_StanceSteady) && GENERIC_SKILL_VALID(SID_StanceSteady))
    [SKILL_INDEX_REAL(SID_StanceSteady)] = {
        .desc = MSG_SKILL_StanceSteady,
        .icon = GFX_SkillIcon_StanceSteady,
    },
#endif

#if (defined(SID_StanceSturdy) && GENERIC_SKILL_VALID(SID_StanceSturdy))
    [SKILL_INDEX_REAL(SID_StanceSturdy)] = {
        .desc = MSG_SKILL_StanceSturdy,
        .icon = GFX_SkillIcon_StanceSturdy,
    },
#endif

#if (defined(SID_StanceSwift) && GENERIC_SKILL_VALID(SID_StanceSwift))
    [SKILL_INDEX_REAL(SID_StanceSwift)] = {
        .desc = MSG_SKILL_StanceSwift,
        .icon = GFX_SkillIcon_StanceSwift,
    },
#endif

#if (defined(SID_StanceWarding) && GENERIC_SKILL_VALID(SID_StanceWarding))
    [SKILL_INDEX_REAL(SID_StanceWarding)] = {
        .desc = MSG_SKILL_StanceWarding,
        .icon = GFX_SkillIcon_StanceWarding,
    },
#endif

#if (defined(SID_RangeBonusBMag1) && GENERIC_SKILL_VALID(SID_RangeBonusBMag1))
    [SKILL_INDEX_REAL(SID_RangeBonusBMag1)] = {
        .desc = MSG_SKILL_RangeBonusBMag1,
        .icon = GFX_SkillIcon_RangeBonusBMag1,
    },
#endif

#if (defined(SID_RangeBonusBMag2) && GENERIC_SKILL_VALID(SID_RangeBonusBMag2))
    [SKILL_INDEX_REAL(SID_RangeBonusBMag2)] = {
        .desc = MSG_SKILL_RangeBonusBMag2,
        .icon = GFX_SkillIcon_RangeBonusBMag2,
    },
#endif

#if (defined(SID_RangeBonusBow1) && GENERIC_SKILL_VALID(SID_RangeBonusBow1))
    [SKILL_INDEX_REAL(SID_RangeBonusBow1)] = {
        .desc = MSG_SKILL_RangeBonusBow1,
        .icon = GFX_SkillIcon_RangeBonusBow1,
    },
#endif

#if (defined(SID_RangeBonusBow2) && GENERIC_SKILL_VALID(SID_RangeBonusBow2))
    [SKILL_INDEX_REAL(SID_RangeBonusBow2)] = {
        .desc = MSG_SKILL_RangeBonusBow2,
        .icon = GFX_SkillIcon_RangeBonusBow2,
    },
#endif

#if (defined(SID_RunningStart) && GENERIC_SKILL_VALID(SID_RunningStart))
    [SKILL_INDEX_REAL(SID_RunningStart)] = {
        .name = MSG_SKILL_RunningStart_NAME,
        .desc = MSG_SKILL_RunningStart,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Canto) && GENERIC_SKILL_VALID(SID_Canto))
    [SKILL_INDEX_REAL(SID_Canto)] = {
        .desc = MSG_SKILL_Canto,
        .icon = GFX_SkillIcon_Canto,
    },
#endif

#if (defined(SID_CantoPlus) && GENERIC_SKILL_VALID(SID_CantoPlus))
    [SKILL_INDEX_REAL(SID_CantoPlus)] = {
        .desc = MSG_SKILL_CantoPlus,
        .icon = GFX_SkillIcon_CantoPlus,
    },
#endif

#if (defined(SID_AlertStance) && GENERIC_SKILL_VALID(SID_AlertStance))
    [SKILL_INDEX_REAL(SID_AlertStance)] = {
        .desc = MSG_SKILL_AlertStance,
        .icon = GFX_SkillIcon_AlertStance,
    },
#endif

#if (defined(SID_AlertStancePlus) && GENERIC_SKILL_VALID(SID_AlertStancePlus))
    [SKILL_INDEX_REAL(SID_AlertStancePlus)] = {
        .desc = MSG_SKILL_AlertStancePlus,
        .icon = GFX_SkillIcon_AlertStancePlus,
    },
#endif

#if (defined(SID_Galeforce) && GENERIC_SKILL_VALID(SID_Galeforce))
    [SKILL_INDEX_REAL(SID_Galeforce)] = {
        .desc = MSG_SKILL_Galeforce,
        .icon = GFX_SkillIcon_Galeforce,
    },
#endif

#if (defined(SID_SavageBlow) && GENERIC_SKILL_VALID(SID_SavageBlow))
    [SKILL_INDEX_REAL(SID_SavageBlow)] = {
        .desc = MSG_SKILL_SavageBlow,
        .icon = GFX_SkillIcon_SavageBlow,
    },
#endif

#if (defined(SID_BreathOfLife) && GENERIC_SKILL_VALID(SID_BreathOfLife))
    [SKILL_INDEX_REAL(SID_BreathOfLife)] = {
        .desc = MSG_SKILL_BreathOfLife,
        .icon = GFX_SkillIcon_BreathOfLife,
    },
#endif

#if (defined(SID_Thunderstorm) && GENERIC_SKILL_VALID(SID_Thunderstorm))
    [SKILL_INDEX_REAL(SID_Thunderstorm)] = {
        .desc = MSG_SKILL_Thunderstorm,
        .icon = GFX_SkillIcon_Thunderstorm,
    },
#endif

#if (defined(SID_PosReturn) && GENERIC_SKILL_VALID(SID_PosReturn))
    [SKILL_INDEX_REAL(SID_PosReturn)] = {
        .desc = MSG_SKILL_PosReturn,
        .icon = GFX_SkillIcon_PosReturn,
    },
#endif

#if (defined(SID_PosSwap) && GENERIC_SKILL_VALID(SID_PosSwap))
    [SKILL_INDEX_REAL(SID_PosSwap)] = {
        .desc = MSG_SKILL_PosSwap,
        .icon = GFX_SkillIcon_PosSwap,
    },
#endif

#if (defined(SID_Renewal) && GENERIC_SKILL_VALID(SID_Renewal))
    [SKILL_INDEX_REAL(SID_Renewal)] = {
        .desc = MSG_SKILL_Renewal,
        .icon = GFX_SkillIcon_Renewal,
    },
#endif

#if (defined(SID_Imbue) && GENERIC_SKILL_VALID(SID_Imbue))
    [SKILL_INDEX_REAL(SID_Imbue)] = {
        .desc = MSG_SKILL_Imbue,
        .icon = GFX_SkillIcon_Imbue,
    },
#endif

#if (defined(SID_Forager) && GENERIC_SKILL_VALID(SID_Forager))
    [SKILL_INDEX_REAL(SID_Forager)] = {
        .desc = MSG_SKILL_Forager,
        .icon = GFX_SkillIcon_Forager,
    },
#endif

#if (defined(SID_Anathema) && GENERIC_SKILL_VALID(SID_Anathema))
    [SKILL_INDEX_REAL(SID_Anathema)] = {
        .desc = MSG_SKILL_Anathema,
        .icon = GFX_SkillIcon_Anathema,
    },
#endif

#if (defined(SID_Bond) && GENERIC_SKILL_VALID(SID_Bond))
    [SKILL_INDEX_REAL(SID_Bond)] = {
        .desc = MSG_SKILL_Bond,
        .icon = GFX_SkillIcon_Bond,
    },
#endif

#if (defined(SID_Charm) && GENERIC_SKILL_VALID(SID_Charm))
    [SKILL_INDEX_REAL(SID_Charm)] = {
        .desc = MSG_SKILL_Charm,
        .icon = GFX_SkillIcon_Charm,
    },
#endif

#if (defined(SID_Intimidate) && GENERIC_SKILL_VALID(SID_Intimidate))
    [SKILL_INDEX_REAL(SID_Intimidate)] = {
        .desc = MSG_SKILL_Intimidate,
        .icon = GFX_SkillIcon_Intimidate,
    },
#endif

#if (defined(SID_Tantivy) && GENERIC_SKILL_VALID(SID_Tantivy))
    [SKILL_INDEX_REAL(SID_Tantivy)] = {
        .desc = MSG_SKILL_Tantivy,
        .icon = GFX_SkillIcon_Tantivy,
    },
#endif

#if (defined(SID_Focus) && GENERIC_SKILL_VALID(SID_Focus))
    [SKILL_INDEX_REAL(SID_Focus)] = {
        .desc = MSG_SKILL_Focus,
        .icon = GFX_SkillIcon_Focus,
    },
#endif

#if (defined(SID_Hex) && GENERIC_SKILL_VALID(SID_Hex))
    [SKILL_INDEX_REAL(SID_Hex)] = {
        .desc = MSG_SKILL_Hex,
        .icon = GFX_SkillIcon_Hex,
    },
#endif

#if (defined(SID_Infiltrator) && GENERIC_SKILL_VALID(SID_Infiltrator))
    [SKILL_INDEX_REAL(SID_Infiltrator)] = {
        .desc = MSG_SKILL_Infiltrator,
        .icon = GFX_SkillIcon_Infiltrator,
    },
#endif

#if (defined(SID_Inspiration) && GENERIC_SKILL_VALID(SID_Inspiration))
    [SKILL_INDEX_REAL(SID_Inspiration)] = {
        .desc = MSG_SKILL_Inspiration,
        .icon = GFX_SkillIcon_Inspiration,
    },
#endif

#if (defined(SID_DivinelyInspiring) && GENERIC_SKILL_VALID(SID_DivinelyInspiring))
    [SKILL_INDEX_REAL(SID_DivinelyInspiring)] = {
        .desc = MSG_SKILL_DivinelyInspiring,
        .icon = GFX_SkillIcon_DivinelyInspiring,
    },
#endif

#if (defined(SID_VoiceOfPeace) && GENERIC_SKILL_VALID(SID_VoiceOfPeace))
    [SKILL_INDEX_REAL(SID_VoiceOfPeace)] = {
        .desc = MSG_SKILL_VoiceOfPeace,
        .icon = GFX_SkillIcon_VoiceOfPeace,
    },
#endif

#if (defined(SID_PowHone) && GENERIC_SKILL_VALID(SID_PowHone))
    [SKILL_INDEX_REAL(SID_PowHone)] = {
        .name = MSG_SKILL_PowHone_NAME,
        .desc = MSG_SKILL_PowHone,
        .icon = GFX_SkillIcon_HonePow
    },
#endif

#if (defined(SID_MagHone) && GENERIC_SKILL_VALID(SID_MagHone))
    [SKILL_INDEX_REAL(SID_MagHone)] = {
        .name = MSG_SKILL_MagHone_NAME,
        .desc = MSG_SKILL_MagHone,
        .icon = GFX_SkillIcon_HoneMag
    },
#endif

#if (defined(SID_SklHone) && GENERIC_SKILL_VALID(SID_SklHone))
    [SKILL_INDEX_REAL(SID_SklHone)] = {
        .name = MSG_SKILL_SklHone_NAME,
        .desc = MSG_SKILL_SklHone,
        .icon = GFX_SkillIcon_HoneSkl
    },
#endif

#if (defined(SID_SpdHone) && GENERIC_SKILL_VALID(SID_SpdHone))
    [SKILL_INDEX_REAL(SID_SpdHone)] = {
        .name = MSG_SKILL_SpdHone_NAME,
        .desc = MSG_SKILL_SpdHone,
        .icon = GFX_SkillIcon_HoneSpd
    },
#endif

#if (defined(SID_LckHone) && GENERIC_SKILL_VALID(SID_LckHone))
    [SKILL_INDEX_REAL(SID_LckHone)] = {
        .name = MSG_SKILL_LckHone_NAME,
        .desc = MSG_SKILL_LckHone,
        .icon = GFX_SkillIcon_HoneLuk
    },
#endif

#if (defined(SID_DefHone) && GENERIC_SKILL_VALID(SID_DefHone))
    [SKILL_INDEX_REAL(SID_DefHone)] = {
        .name = MSG_SKILL_DefHone_NAME,
        .desc = MSG_SKILL_DefHone,
        .icon = GFX_SkillIcon_HoneDef
    },
#endif

#if (defined(SID_ResHone) && GENERIC_SKILL_VALID(SID_ResHone))
    [SKILL_INDEX_REAL(SID_ResHone)] = {
        .name = MSG_SKILL_ResHone_NAME,
        .desc = MSG_SKILL_ResHone,
        .icon = GFX_SkillIcon_HoneRes
    },
#endif

#if (defined(SID_BattleRange_Todo1) && GENERIC_SKILL_VALID(SID_BattleRange_Todo1))
    [SKILL_INDEX_REAL(SID_BattleRange_Todo1)] = {
        .desc = MSG_SKILL_BattleRange1,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_BattleRange_Todo2) && GENERIC_SKILL_VALID(SID_BattleRange_Todo2))
    [SKILL_INDEX_REAL(SID_BattleRange_Todo2)] = {
        .desc = MSG_SKILL_BattleRange2,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_BattleRange_Todo3) && GENERIC_SKILL_VALID(SID_BattleRange_Todo3))
    [SKILL_INDEX_REAL(SID_BattleRange_Todo3)] = {
        .desc = MSG_SKILL_BattleRange3,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Steal) && GENERIC_SKILL_VALID(SID_Steal))
    [SKILL_INDEX_REAL(SID_Steal)] = {
        .desc = MSG_SKILL_Steal,
        .icon = GFX_SkillIcon_Steal,
    },
#endif

#if (defined(SID_Blossom) && GENERIC_SKILL_VALID(SID_Blossom))
    [SKILL_INDEX_REAL(SID_Blossom)] = {
        .desc = MSG_SKILL_Blossom,
        .icon = GFX_SkillIcon_Blossom,
    },
#endif

#if (defined(SID_Paragon) && GENERIC_SKILL_VALID(SID_Paragon))
    [SKILL_INDEX_REAL(SID_Paragon)] = {
        .desc = MSG_SKILL_Paragon,
        .icon = GFX_SkillIcon_Paragon,
    },
#endif

#if (defined(SID_VoidCurse) && GENERIC_SKILL_VALID(SID_VoidCurse))
    [SKILL_INDEX_REAL(SID_VoidCurse)] = {
        .desc = MSG_SKILL_VoidCurse,
        .icon = GFX_SkillIcon_VoidCurse,
    },
#endif

#if (defined(SID_Aptitude) && GENERIC_SKILL_VALID(SID_Aptitude))
    [SKILL_INDEX_REAL(SID_Aptitude)] = {
        .desc = MSG_SKILL_Aptitude,
        .icon = GFX_SkillIcon_Aptitude,
    },
#endif

#if (defined(SID_Peacebringer) && GENERIC_SKILL_VALID(SID_VoiceOfPeace))
    [SKILL_INDEX_REAL(SID_Peacebringer)] = {
        .desc = MSG_SKILL_Peacebringer,
        .icon = GFX_SkillIcon_Peacebringer,
    },
#endif

#if (defined(SID_LEGEND_InoriAtk) && GENERIC_SKILL_VALID(SID_LEGEND_InoriAtk))
    [SKILL_INDEX_REAL(SID_LEGEND_InoriAtk)] = {
        .desc = MSG_SKILL_LEGEND_InoriAtk,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_InoriAvo) && GENERIC_SKILL_VALID(SID_LEGEND_InoriAvo))
    [SKILL_INDEX_REAL(SID_LEGEND_InoriAvo)] = {
        .desc = MSG_SKILL_LEGEND_InoriAvo,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_InoriDef) && GENERIC_SKILL_VALID(SID_LEGEND_InoriDef))
    [SKILL_INDEX_REAL(SID_LEGEND_InoriDef)] = {
        .desc = MSG_SKILL_LEGEND_InoriDef,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_4) && GENERIC_SKILL_VALID(SID_LEGEND_4))
    [SKILL_INDEX_REAL(SID_LEGEND_4)] = {
        .desc = MSG_WIP,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_5) && GENERIC_SKILL_VALID(SID_LEGEND_5))
    [SKILL_INDEX_REAL(SID_LEGEND_5)] = {
        .desc = MSG_WIP,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Frenzy) && GENERIC_SKILL_VALID(SID_Frenzy))
    [SKILL_INDEX_REAL(SID_Frenzy)] = {
        .desc = MSG_SKILL_Frenzy,
        .icon = GFX_SkillIcon_Frenzy,
    },
#endif

#if (defined(SID_WonderGuard) && GENERIC_SKILL_VALID(SID_WonderGuard))
    [SKILL_INDEX_REAL(SID_WonderGuard)] = {
        .name = MSG_SKILL_WonderGuard_NAME,
        .desc = MSG_SKILL_WonderGuard,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_PoisonPoint) && GENERIC_SKILL_VALID(SID_PoisonPoint))
    [SKILL_INDEX_REAL(SID_PoisonPoint)] = {
        .name = MSG_SKILL_PoisonPoint_NAME,
        .desc = MSG_SKILL_PoisonPoint,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Merciless) && GENERIC_SKILL_VALID(SID_Merciless))
    [SKILL_INDEX_REAL(SID_Merciless)] = {
        .desc = MSG_SKILL_Merciless,
        .icon = GFX_SkillIcon_Merciless,
    },
#endif

#if (defined(SID_CriticalPierce) && GENERIC_SKILL_VALID(SID_CriticalPierce))
    [SKILL_INDEX_REAL(SID_CriticalPierce)] = {
        .desc = MSG_SKILL_CriticalPierce,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_KillingMachine) && GENERIC_SKILL_VALID(SID_KillingMachine))
    [SKILL_INDEX_REAL(SID_KillingMachine)] = {
        .name = MSG_SKILL_KillingMachine_NAME,
        .desc = MSG_SKILL_KillingMachine,
        .icon = GFX_SkillIcon_KillingMachine,
    },
#endif

#if (defined(SID_HeavyStrikes) && GENERIC_SKILL_VALID(SID_HeavyStrikes))
    [SKILL_INDEX_REAL(SID_HeavyStrikes)] = {
        .name = MSG_SKILL_HeavyStrikes_NAME,
        .desc = MSG_SKILL_HeavyStrikes,
        .icon = GFX_SkillIcon_HeavyStrikes,
    },
#endif

#if (defined(SID_QuickBurn) && GENERIC_SKILL_VALID(SID_QuickBurn))
    [SKILL_INDEX_REAL(SID_QuickBurn)] = {
        .desc = MSG_SKILL_QuickBurn,
        .icon = GFX_SkillIcon_QuickBurn,
    },
#endif

#if (defined(SID_SlowBurn) && GENERIC_SKILL_VALID(SID_SlowBurn))
    [SKILL_INDEX_REAL(SID_SlowBurn)] = {
        .desc = MSG_SKILL_SlowBurn,
        .icon = GFX_SkillIcon_SlowBurn,
    },
#endif

#if (defined(SID_StanceSpectrum) && GENERIC_SKILL_VALID(SID_StanceSpectrum))
    [SKILL_INDEX_REAL(SID_StanceSpectrum)] = {
        .name = MSG_SKILL_StanceSpectrum_NAME,
        .desc = MSG_SKILL_StanceSpectrum,
        .icon = GFX_SkillIcon_StanceSpectrum,
    },
#endif

#if (defined(SID_Technician) && GENERIC_SKILL_VALID(SID_Technician))
    [SKILL_INDEX_REAL(SID_Technician)] = {
        .desc = MSG_SKILL_Technician,
        .icon = GFX_SkillIcon_Technician,
    },
#endif

#if (defined(SID_CatchingUp) && GENERIC_SKILL_VALID(SID_CatchingUp))
    [SKILL_INDEX_REAL(SID_CatchingUp)] = {
        .desc = MSG_SKILL_CatchingUp,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_BloodTide) && GENERIC_SKILL_VALID(SID_BloodTide))
    [SKILL_INDEX_REAL(SID_BloodTide)] = {
        .desc = MSG_SKILL_BloodTide,
        .icon = GFX_SkillIcon_BloodTide,
    },
#endif

#if (defined(SID_WhitePool) && GENERIC_SKILL_VALID(SID_WhitePool))
    [SKILL_INDEX_REAL(SID_WhitePool)] = {
        .desc = MSG_SKILL_WhitePool,
        .icon = GFX_SkillIcon_WhitePool,
    },
#endif

#if (defined(SID_NightTide) && GENERIC_SKILL_VALID(SID_NightTide))
    [SKILL_INDEX_REAL(SID_NightTide)] = {
        .desc = MSG_SKILL_NightTide,
        .icon = GFX_SkillIcon_NightTide,
    },
#endif

#if (defined(SID_DriveStr) && GENERIC_SKILL_VALID(SID_DriveStr))
    [SKILL_INDEX_REAL(SID_DriveStr)] = {
        .desc = MSG_SKILL_DriveStr,
        .icon = GFX_SkillIcon_DriveStr,
    },
#endif

#if (defined(SID_DriveMag) && GENERIC_SKILL_VALID(SID_DriveMag))
    [SKILL_INDEX_REAL(SID_DriveMag)] = {
        .desc = MSG_SKILL_DriveMag,
        .icon = GFX_SkillIcon_DriveMag,
    },
#endif

#if (defined(SID_DriveSpd) && GENERIC_SKILL_VALID(SID_DriveSpd))
    [SKILL_INDEX_REAL(SID_DriveSpd)] = {
        .desc = MSG_SKILL_DriveSpd,
        .icon = GFX_SkillIcon_DriveSpd,
    },
#endif

#if (defined(SID_DriveDef) && GENERIC_SKILL_VALID(SID_DriveDef))
    [SKILL_INDEX_REAL(SID_DriveDef)] = {
        .desc = MSG_SKILL_DriveDef,
        .icon = GFX_SkillIcon_DriveDef,
    },
#endif

#if (defined(SID_DriveRes) && GENERIC_SKILL_VALID(SID_DriveRes))
    [SKILL_INDEX_REAL(SID_DriveRes)] = {
        .desc = MSG_SKILL_DriveRes,
        .icon = GFX_SkillIcon_DriveRes,
    },
#endif

#if (defined(SID_Charisma) && GENERIC_SKILL_VALID(SID_Charisma))
    [SKILL_INDEX_REAL(SID_Charisma)] = {
         .desc = MSG_SKILL_Charisma,
         .icon = GFX_SkillIcon_Charisma,
    },
#endif

#if (defined(SID_LilysPoise) && GENERIC_SKILL_VALID(SID_LilysPoise))
    [SKILL_INDEX_REAL(SID_LilysPoise)] = {
        .desc = MSG_SKILL_LilysPoise,
        .icon = GFX_SkillIcon_LilysPoise,
    },
#endif

#if (defined(SID_Daunt) && GENERIC_SKILL_VALID(SID_Daunt))
    [SKILL_INDEX_REAL(SID_Daunt)] = {
         .desc = MSG_SKILL_Daunt,
         .icon = GFX_SkillIcon_Daunt,
    },
#endif

#if (defined(SID_SpurStr) && GENERIC_SKILL_VALID(SID_SpurStr))
    [SKILL_INDEX_REAL(SID_SpurStr)] = {
         .desc = MSG_SKILL_SpurStr,
         .icon = GFX_SkillIcon_SpurStr,
    },
#endif

#if (defined(SID_SpurMag) && GENERIC_SKILL_VALID(SID_SpurMag))
    [SKILL_INDEX_REAL(SID_SpurMag)] = {
         .desc = MSG_SKILL_SpurMag,
         .icon = GFX_SkillIcon_SpurMag,
    },
#endif

#if (defined(SID_SpurDef) && GENERIC_SKILL_VALID(SID_SpurDef))
    [SKILL_INDEX_REAL(SID_SpurDef)] = {
         .desc = MSG_SKILL_SpurDef,
         .icon = GFX_SkillIcon_SpurDef,
    },
#endif

#if (defined(SID_SpurRes) && GENERIC_SKILL_VALID(SID_SpurRes))
    [SKILL_INDEX_REAL(SID_SpurRes)] = {
         .desc = MSG_SKILL_SpurRes,
         .icon = GFX_SkillIcon_SpurRes,
    },
#endif

#if (defined(SID_SpurSpd) && GENERIC_SKILL_VALID(SID_SpurSpd))
    [SKILL_INDEX_REAL(SID_SpurSpd)] = {
         .desc = MSG_SKILL_SpurSpd,
         .icon = GFX_SkillIcon_SpurSpd,
    },
#endif

#if (defined(SID_BattleVeteran) && GENERIC_SKILL_VALID(SID_BattleVeteran))
    [SKILL_INDEX_REAL(SID_BattleVeteran)] = {
         .name = MSG_SKILL_BattleVeteran_NAME,
         .desc = MSG_SKILL_BattleVeteran,
         .icon = GFX_SkillIcon_BattleVeteran,
    },
#endif

#if (defined(SID_Gentilhomme) && GENERIC_SKILL_VALID(SID_Gentilhomme))
    [SKILL_INDEX_REAL(SID_Gentilhomme)] = {
         .desc = MSG_SKILL_Gentilhomme,
         .icon = GFX_SkillIcon_Gentilhomme,
    },
#endif

#if (defined(SID_Demoiselle) && GENERIC_SKILL_VALID(SID_Demoiselle))
    [SKILL_INDEX_REAL(SID_Demoiselle)] = {
         .desc = MSG_SKILL_Demoiselle,
         .icon = GFX_SkillIcon_Demoiselle,
    },
#endif

#if (defined(SID_Solidarity) && GENERIC_SKILL_VALID(SID_Solidarity))
    [SKILL_INDEX_REAL(SID_Solidarity)] = {
         .desc = MSG_SKILL_Solidarity,
         .icon = GFX_SkillIcon_Solidarity,
    },
#endif

#if (defined(SID_StoneBody) && GENERIC_SKILL_VALID(SID_StoneBody))
    [SKILL_INDEX_REAL(SID_StoneBody)] = {
        .desc = MSG_SKILL_StoneBody,
        .icon = GFX_SkillIcon_StoneBody,
    },
#endif

#if (defined(SID_Chivalry) && GENERIC_SKILL_VALID(SID_Chivalry))
    [SKILL_INDEX_REAL(SID_Chivalry)] = {
         .desc = MSG_SKILL_Chivalry,
         .icon = GFX_SkillIcon_Chivalry,
    },
#endif

#if (defined(SID_Pragmatic) && GENERIC_SKILL_VALID(SID_Pragmatic))
    [SKILL_INDEX_REAL(SID_Pragmatic)] = {
         .desc = MSG_SKILL_Pragmatic,
         .icon = GFX_SkillIcon_Pragmatic,
    },
#endif

#if (defined(SID_WindDisciple) && GENERIC_SKILL_VALID(SID_WindDisciple))
    [SKILL_INDEX_REAL(SID_WindDisciple)] = {
         .desc = MSG_SKILL_WindDisciple,
         .icon = GFX_SkillIcon_WindDisciple,
    },
#endif

#if (defined(SID_Perfectionist) && GENERIC_SKILL_VALID(SID_Perfectionist))
    [SKILL_INDEX_REAL(SID_Perfectionist)] = {
         .desc = MSG_SKILL_Perfectionist,
         .icon = GFX_SkillIcon_Perfectionist,
    },
#endif

#if (defined(SID_CriticalForce) && GENERIC_SKILL_VALID(SID_CriticalForce))
    [SKILL_INDEX_REAL(SID_CriticalForce)] = {
         .name = MSG_SKILL_CriticalForce_NAME,
         .desc = MSG_SKILL_CriticalForce,
         .icon = GFX_SkillIcon_CriticalForce,
    },
#endif

#if (defined(SID_StrongRiposte) && GENERIC_SKILL_VALID(SID_StrongRiposte))
    [SKILL_INDEX_REAL(SID_StrongRiposte)] = {
         .name = MSG_SKILL_StrongRiposte_NAME,
         .desc = MSG_SKILL_StrongRiposte,
         .icon = GFX_SkillIcon_StrongRiposte,
    },
#endif

#if (defined(SID_Patience) && GENERIC_SKILL_VALID(SID_Patience))
    [SKILL_INDEX_REAL(SID_Patience)] = {
         .desc = MSG_SKILL_Patience,
         .icon = GFX_SkillIcon_Patience,
    },
#endif

#if (defined(SID_Pursuit) && GENERIC_SKILL_VALID(SID_Pursuit))
    [SKILL_INDEX_REAL(SID_Pursuit)] = {
         .desc = MSG_SKILL_Pursuit,
         .icon = GFX_SkillIcon_Pursuit,
    },
#endif

#if (defined(SID_DragonSkin) && GENERIC_SKILL_VALID(SID_DragonSkin))
    [SKILL_INDEX_REAL(SID_DragonSkin)] = {
         .name = MSG_SKILL_DragonSkin_NAME,
         .desc = MSG_SKILL_DragonSkin,
         .icon = GFX_SkillIcon_DragonSkin,
    },
#endif

#if (defined(SID_Hawkeye) && GENERIC_SKILL_VALID(SID_Hawkeye))
    [SKILL_INDEX_REAL(SID_Hawkeye)] = {
         .desc = MSG_SKILL_Hawkeye,
         .icon = GFX_SkillIcon_Hawkeye,
    },
#endif

#if (defined(SID_KeenFighter) && GENERIC_SKILL_VALID(SID_KeenFighter))
    [SKILL_INDEX_REAL(SID_KeenFighter)] = {
         .name = MSG_SKILL_KeenFighter_NAME,
         .desc = MSG_SKILL_KeenFighter,
         .icon = GFX_SkillIcon_KeenFighter,
    },
#endif

#if (defined(SID_FireBoost) && GENERIC_SKILL_VALID(SID_FireBoost))
    [SKILL_INDEX_REAL(SID_FireBoost)] = {
         .desc = MSG_SKILL_FireBoost,
         .icon = GFX_SkillIcon_FireBoost,
    },
#endif

#if (defined(SID_WindBoost) && GENERIC_SKILL_VALID(SID_WindBoost))
    [SKILL_INDEX_REAL(SID_WindBoost)] = {
         .desc = MSG_SKILL_WindBoost,
         .icon = GFX_SkillIcon_WindBoost,
    },
#endif

#if (defined(SID_EarthBoost) && GENERIC_SKILL_VALID(SID_EarthBoost))
    [SKILL_INDEX_REAL(SID_EarthBoost)] = {
         .desc = MSG_SKILL_EarthBoost,
         .icon = GFX_SkillIcon_EarthBoost,
    },
#endif

#if (defined(SID_WaterBoost) && GENERIC_SKILL_VALID(SID_WaterBoost))
    [SKILL_INDEX_REAL(SID_WaterBoost)] = {
         .desc = MSG_SKILL_WaterBoost,
         .icon = GFX_SkillIcon_WaterBoost,
    },
#endif

#if (defined(SID_FieryBlood) && GENERIC_SKILL_VALID(SID_FieryBlood))
    [SKILL_INDEX_REAL(SID_FieryBlood)] = {
         .name = MSG_SKILL_FieryBlood_NAME,
         .desc = MSG_SKILL_FieryBlood,
         .icon = GFX_SkillIcon_FieryBlood,
    },
#endif

#if (defined(SID_Colossus) && GENERIC_SKILL_VALID(SID_Colossus))
    [SKILL_INDEX_REAL(SID_Colossus)] = {
        .desc = MSG_SKILL_Colossus,
        .icon = GFX_SkillIcon_Colossus,
    },
#endif

#if (defined(SID_LightWeight) && GENERIC_SKILL_VALID(SID_LightWeight))
    [SKILL_INDEX_REAL(SID_LightWeight)] = {
         .desc = MSG_SKILL_LightWeight,
         .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_EvenFooted) && GENERIC_SKILL_VALID(SID_EvenFooted))
    [SKILL_INDEX_REAL(SID_EvenFooted)] = {
         .desc = MSG_SKILL_EvenFooted,
         .icon = GFX_SkillIcon_EvenFooted,
    },
#endif

#if (defined(SID_QuickDraw) && GENERIC_SKILL_VALID(SID_QuickDraw))
    [SKILL_INDEX_REAL(SID_QuickDraw)] = {
         .desc = MSG_SKILL_QuickDraw,
         .icon = GFX_SkillIcon_QuickDraw,
    },
#endif

#if (defined(SID_ArcaneBlade) && GENERIC_SKILL_VALID(SID_ArcaneBlade))
    [SKILL_INDEX_REAL(SID_ArcaneBlade)] = {
         .desc = MSG_SKILL_ArcaneBlade,
         .icon = GFX_SkillIcon_ArcaneBlade,
    },
#endif

#if (defined(SID_ElbowRoom) && GENERIC_SKILL_VALID(SID_ElbowRoom))
    [SKILL_INDEX_REAL(SID_ElbowRoom)] = {
         .desc = MSG_SKILL_ElbowRoom,
         .icon = GFX_SkillIcon_ElbowRoom,
    },
#endif

#if (defined(SID_NaturalCover) && GENERIC_SKILL_VALID(SID_NaturalCover))
    [SKILL_INDEX_REAL(SID_NaturalCover)] = {
         .name = MSG_SKILL_NaturalCover_NAME,
         .desc = MSG_SKILL_NaturalCover,
         .icon = GFX_SkillIcon_NaturalCover,
    },
#endif

#if (defined(SID_ChaosStyle) && GENERIC_SKILL_VALID(SID_ChaosStyle))
    [SKILL_INDEX_REAL(SID_ChaosStyle)] = {
         .desc = MSG_SKILL_ChaosStyle,
         .icon = GFX_SkillIcon_ChaosStyle,
    },
#endif

#if (defined(SID_PushDefense) && GENERIC_SKILL_VALID(SID_PushDefense))
    [SKILL_INDEX_REAL(SID_PushDefense)] = {
         .name = MSG_SKILL_PushDefense_NAME,
         .desc = MSG_SKILL_PushDefense,
         .icon = GFX_SkillIcon_PushDefense,
    },
#endif

#if (defined(SID_PushMagic) && GENERIC_SKILL_VALID(SID_PushMagic))
    [SKILL_INDEX_REAL(SID_PushMagic)] = {
         .name = MSG_SKILL_PushMagic_NAME,
         .desc = MSG_SKILL_PushMagic,
         .icon = GFX_SkillIcon_PushMagic,
    },
#endif

#if (defined(SID_PushResistance) && GENERIC_SKILL_VALID(SID_PushResistance))
    [SKILL_INDEX_REAL(SID_PushResistance)] = {
         .name = MSG_SKILL_PushResistance_NAME,
         .desc = MSG_SKILL_PushResistance,
         .icon = GFX_SkillIcon_PushResistance,
    },
#endif

#if (defined(SID_PushSkill) && GENERIC_SKILL_VALID(SID_PushSkill))
    [SKILL_INDEX_REAL(SID_PushSkill)] = {
         .name = MSG_SKILL_PushSkill_NAME,
         .desc = MSG_SKILL_PushSkill,
         .icon = GFX_SkillIcon_PushSkill,
    },
#endif

#if (defined(SID_PushSpeed) && GENERIC_SKILL_VALID(SID_PushSpeed))
    [SKILL_INDEX_REAL(SID_PushSpeed)] = {
         .name = MSG_SKILL_PushSpeed_NAME,
         .desc = MSG_SKILL_PushSpeed,
         .icon = GFX_SkillIcon_PushSpeed,
    },
#endif

#if (defined(SID_PushStrength) && GENERIC_SKILL_VALID(SID_PushStrength))
    [SKILL_INDEX_REAL(SID_PushStrength)] = {
         .name = MSG_SKILL_PushStrength_NAME,
         .desc = MSG_SKILL_PushStrength,
         .icon = GFX_SkillIcon_PushStrength,
    },
#endif

#if (defined(SID_PushLuck) && GENERIC_SKILL_VALID(SID_PushLuck))
    [SKILL_INDEX_REAL(SID_PushLuck)] = {
         .name = MSG_SKILL_PushLuck_NAME,
         .desc = MSG_SKILL_PushLuck,
         .icon = GFX_SkillIcon_PushLuck,
    },
#endif

#if (defined(SID_PushMovement) && GENERIC_SKILL_VALID(SID_PushMovement))
    [SKILL_INDEX_REAL(SID_PushMovement)] = {
         .name = MSG_SKILL_PushMovement_NAME,
         .desc = MSG_SKILL_PushMovement,
         .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_PushSpectrum) && GENERIC_SKILL_VALID(SID_PushSpectrum))
    [SKILL_INDEX_REAL(SID_PushSpectrum)] = {
         .name = MSG_SKILL_PushSpectrum_NAME,
         .desc = MSG_SKILL_PushSpectrum,
         .icon = GFX_SkillIcon_PushSpectrum,
    },
#endif

#if (defined(SID_Charge) && GENERIC_SKILL_VALID(SID_Charge))
    [SKILL_INDEX_REAL(SID_Charge)] = {
         .desc = MSG_SKILL_Charge,
         .icon = GFX_SkillIcon_Charge,
    },
#endif

#if (defined(SID_Wrath) && GENERIC_SKILL_VALID(SID_Wrath))
    [SKILL_INDEX_REAL(SID_Wrath)] = {
         .desc = MSG_SKILL_Wrath,
         .icon = GFX_SkillIcon_Wrath,
    },
#endif

#if (defined(SID_Vigilance) && GENERIC_SKILL_VALID(SID_Vigilance))
    [SKILL_INDEX_REAL(SID_Vigilance)] = {
         .desc = MSG_SKILL_Vigilance,
         .icon = GFX_SkillIcon_Vigilance,
    },
#endif

#if (defined(SID_OutdoorFighter) && GENERIC_SKILL_VALID(SID_OutdoorFighter))
    [SKILL_INDEX_REAL(SID_OutdoorFighter)] = {
         .name = MSG_SKILL_OutdoorFighter_NAME,
         .desc = MSG_SKILL_OutdoorFighter,
         .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_DancingBlade) && GENERIC_SKILL_VALID(SID_DancingBlade))
    [SKILL_INDEX_REAL(SID_DancingBlade)] = {
         .name = MSG_SKILL_DancingBlade_NAME,
         .desc = MSG_SKILL_DancingBlade,
         .icon = GFX_SkillIcon_DancingBlade,
    },
#endif

#if (defined(SID_KnightAspirant) && GENERIC_SKILL_VALID(SID_KnightAspirant))
    [SKILL_INDEX_REAL(SID_KnightAspirant)] = {
         .name = MSG_SKILL_KnightAspirant_NAME,
         .desc = MSG_SKILL_KnightAspirant,
         .icon = GFX_SkillIcon_KnightAspirant,
    },
#endif

#if (defined(SID_Outrider) && GENERIC_SKILL_VALID(SID_Outrider))
    [SKILL_INDEX_REAL(SID_Outrider)] = {
         .desc = MSG_SKILL_Outrider,
         .icon = GFX_SkillIcon_Outrider
    },
#endif

#if (defined(SID_EvenRhythm) && GENERIC_SKILL_VALID(SID_EvenRhythm))
    [SKILL_INDEX_REAL(SID_EvenRhythm)] = {
         .desc = MSG_SKILL_EvenRhythm,
         .icon = GFX_SkillIcon_EvenRhythm
    },
#endif

#if (defined(SID_OddRhythm) && GENERIC_SKILL_VALID(SID_OddRhythm))
    [SKILL_INDEX_REAL(SID_OddRhythm)] = {
         .desc = MSG_SKILL_OddRhythm,
         .icon = GFX_SkillIcon_OddRhythm
    },
#endif

#if (defined(SID_OddFooted) && GENERIC_SKILL_VALID(SID_OddFooted))
    [SKILL_INDEX_REAL(SID_OddFooted)] = {
         .desc = MSG_SKILL_OddFooted,
         .icon = GFX_SkillIcon_OddFooted,
    },
#endif

#if (defined(SID_Corrosion) && GENERIC_SKILL_VALID(SID_Corrosion))
    [SKILL_INDEX_REAL(SID_Corrosion)] = {
        .desc = MSG_SKILL_Corrosion,
        .icon = GFX_SkillIcon_Corrosion,
    },
#endif

#if (defined(SID_NoGuard) && GENERIC_SKILL_VALID(SID_NoGuard))
    [SKILL_INDEX_REAL(SID_NoGuard)] = {
        .desc = MSG_SKILL_NoGuard,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Puissance) && GENERIC_SKILL_VALID(SID_Puissance))
    [SKILL_INDEX_REAL(SID_Puissance)] = {
        .desc = MSG_SKILL_Puissance,
        .icon = GFX_SkillIcon_Puissance,
    },
#endif

#if (defined(SID_Prescience) && GENERIC_SKILL_VALID(SID_Prescience))
    [SKILL_INDEX_REAL(SID_Prescience)] = {
        .desc = MSG_SKILL_Prescience,
        .icon = GFX_SkillIcon_Prescience,
    },
#endif

#if (defined(SID_Resolve) && GENERIC_SKILL_VALID(SID_Resolve))
    [SKILL_INDEX_REAL(SID_Resolve)] = {
        .desc = MSG_SKILL_Resolve,
        .icon = GFX_SkillIcon_Resolve,
    },
#endif

#if (defined(SID_SilentPride) && GENERIC_SKILL_VALID(SID_SilentPride))
    [SKILL_INDEX_REAL(SID_SilentPride)] = {
        .desc = MSG_SKILL_SilentPride,
        .icon = GFX_SkillIcon_SilentPride,
    },
#endif

#if (defined(SID_RightfulKing) && GENERIC_SKILL_VALID(SID_RightfulKing))
    [SKILL_INDEX_REAL(SID_RightfulKing)] = {
        .desc = MSG_SKILL_RightfulKing,
        .icon = GFX_SkillIcon_RightfulKing,
    },
#endif

#if (defined(SID_RightfulGod) && GENERIC_SKILL_VALID(SID_RightfulGod))
    [SKILL_INDEX_REAL(SID_RightfulGod)] = {
        .desc = MSG_SKILL_RightfulGod,
        .icon = GFX_SkillIcon_RightfulGod,
    },
#endif

#if (defined(SID_RightfulArch) && GENERIC_SKILL_VALID(SID_RightfulArch))
    [SKILL_INDEX_REAL(SID_RightfulArch)] = {
        .desc = MSG_SKILL_RightfulArch,
        .icon = GFX_SkillIcon_RightfulArch,
    },
#endif

#if (defined(SID_Guts) && GENERIC_SKILL_VALID(SID_Guts))
    [SKILL_INDEX_REAL(SID_Guts)] = {
        .desc = MSG_SKILL_Guts,
        .icon = GFX_SkillIcon_Guts,
    },
#endif

#if (defined(SID_Impale) && GENERIC_SKILL_VALID(SID_Impale))
    [SKILL_INDEX_REAL(SID_Impale)] = {
        .desc = MSG_SKILL_Impale,
        .icon = GFX_SkillIcon_Impale,
    },
#endif

#if (defined(SID_Hero) && GENERIC_SKILL_VALID(SID_Hero))
    [SKILL_INDEX_REAL(SID_Hero)] = {
        .desc = MSG_SKILL_Hero,
        .icon = GFX_SkillIcon_Hero,
    },
#endif

#if (defined(SID_Ignis) && GENERIC_SKILL_VALID(SID_Ignis))
    [SKILL_INDEX_REAL(SID_Ignis)] = {
        .desc = MSG_SKILL_Ignis,
        .icon = GFX_SkillIcon_Ignis,
    },
#endif

#if (defined(SID_HolyAura) && GENERIC_SKILL_VALID(SID_HolyAura))
    [SKILL_INDEX_REAL(SID_HolyAura)] = {
        .desc = MSG_SKILL_HolyAura,
        .icon = GFX_SkillIcon_HolyAura,
    },
#endif

#if (defined(SID_Loyalty) && GENERIC_SKILL_VALID(SID_Loyalty))
    [SKILL_INDEX_REAL(SID_Loyalty)] = {
        .desc = MSG_SKILL_Loyalty,
        .icon = GFX_SkillIcon_Loyalty,
    },
#endif

#if (defined(SID_TowerShield) && GENERIC_SKILL_VALID(SID_TowerShield))
    [SKILL_INDEX_REAL(SID_TowerShield)] = {
        .desc = MSG_SKILL_TowerShield,
        .icon = GFX_SkillIcon_TowerShield,
    },
#endif

#if (defined(SID_StunningSmile) && GENERIC_SKILL_VALID(SID_StunningSmile))
    [SKILL_INDEX_REAL(SID_StunningSmile)] = {
        .name = MSG_SKILL_StunningSmile_NAME,
        .desc = MSG_SKILL_StunningSmile,
        .icon = GFX_SkillIcon_StunningSmile,
    },
#endif

#if (defined(SID_Trample) && GENERIC_SKILL_VALID(SID_Trample))
    [SKILL_INDEX_REAL(SID_Trample)] = {
        .desc = MSG_SKILL_Trample,
        .icon = GFX_SkillIcon_Trample,
    },
#endif

#if (defined(SID_Opportunist) && GENERIC_SKILL_VALID(SID_Opportunist))
    [SKILL_INDEX_REAL(SID_Opportunist)] = {
        .desc = MSG_SKILL_Opportunist,
        .icon = GFX_SkillIcon_Opportunist,
    },
#endif

#if (defined(SID_SuperLuck) && GENERIC_SKILL_VALID(SID_SuperLuck))
    [SKILL_INDEX_REAL(SID_SuperLuck)] = {
        .desc = MSG_SKILL_SuperLuck,
        .icon = GFX_SkillIcon_SuperLuck,
    },
#endif

#if (defined(SID_ShortShield) && GENERIC_SKILL_VALID(SID_ShortShield))
    [SKILL_INDEX_REAL(SID_ShortShield)] = {
        .desc = MSG_SKILL_ShortShield,
        .icon = GFX_SkillIcon_ShortShield,
    },
#endif

#if (defined(SID_Vanity) && GENERIC_SKILL_VALID(SID_Vanity))
    [SKILL_INDEX_REAL(SID_Vanity)] = {
        .desc = MSG_SKILL_Vanity,
        .icon = GFX_SkillIcon_Vanity,
    },
#endif

#if (defined(SID_SpeedBoost) && GENERIC_SKILL_VALID(SID_SpeedBoost))
    [SKILL_INDEX_REAL(SID_SpeedBoost)] = {
        .desc = MSG_SKILL_SpeedBoost,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Glacies) && GENERIC_SKILL_VALID(SID_Glacies))
    [SKILL_INDEX_REAL(SID_Glacies)] = {
        .desc = MSG_SKILL_Glacies,
        .icon = GFX_SkillIcon_Glacies,
    },
#endif

#if (defined(SID_Vengeance) && GENERIC_SKILL_VALID(SID_Vengeance))
    [SKILL_INDEX_REAL(SID_Vengeance)] = {
        .desc = MSG_SKILL_Vengeance,
        .icon = GFX_SkillIcon_Vengeance,
    },
#endif

#if (defined(SID_Deadeye) && GENERIC_SKILL_VALID(SID_Deadeye))
    [SKILL_INDEX_REAL(SID_Deadeye)] = {
        .desc = MSG_SKILL_Deadeye,
        .icon = GFX_SkillIcon_Deadeye,
    },
#endif
};
