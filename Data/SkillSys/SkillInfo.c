#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "constants/gfx.h"

const struct SkillInfo gSkillInfos[MAX_SKILL_NUM + 1] = {
#if (defined(SID_HpBonus) && COMMON_SKILL_VALID(SID_HpBonus))
    [SID_HpBonus] = {
        .desc = MSG_SKILL_HpBonus,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_StrBonus) && COMMON_SKILL_VALID(SID_StrBonus))
    [SID_StrBonus] = {
        .desc = MSG_SKILL_StrBonus,
        .icon = GFX_SkillIcon_StrBonus,
    },
#endif

#if (defined(SID_MagBonus) && COMMON_SKILL_VALID(SID_MagBonus))
    [SID_MagBonus] = {
        .desc = MSG_SKILL_MagBonus,
        .icon = GFX_SkillIcon_MagBonus,
    },
#endif

#if (defined(SID_SklBonus) && COMMON_SKILL_VALID(SID_SklBonus))
    [SID_SklBonus] = {
        .desc = MSG_SKILL_SklBonus,
        .icon = GFX_SkillIcon_SklBonus,
    },
#endif

#if (defined(SID_SpdBonus) && COMMON_SKILL_VALID(SID_SpdBonus))
    [SID_SpdBonus] = {
        .desc = MSG_SKILL_SpdBonus,
        .icon = GFX_SkillIcon_SpdBonus,
    },
#endif

#if (defined(SID_LckBonus) && COMMON_SKILL_VALID(SID_LckBonus))
    [SID_LckBonus] = {
        .desc = MSG_SKILL_LukBonus,
        .icon = GFX_SkillIcon_LukBonus,
    },
#endif

#if (defined(SID_DefBonus) && COMMON_SKILL_VALID(SID_DefBonus))
    [SID_DefBonus] = {
        .desc = MSG_SKILL_DefBonus,
        .icon = GFX_SkillIcon_DefBonus,
    },
#endif

#if (defined(SID_ResBonus) && COMMON_SKILL_VALID(SID_ResBonus))
    [SID_ResBonus] = {
        .desc = MSG_SKILL_ResBonus,
        .icon = GFX_SkillIcon_ResBonus,
    },
#endif

#if (defined(SID_MovBonus) && COMMON_SKILL_VALID(SID_MovBonus))
    [SID_MovBonus] = {
        .desc = MSG_SKILL_MovBonus,
        .icon = GFX_SkillIcon_MovBonus,
    },
#endif

#if (defined(SID_DefiantStr) && COMMON_SKILL_VALID(SID_DefiantStr))
    [SID_DefiantStr] = {
        .desc = MSG_SKILL_DefiantStr,
        .icon = GFX_SkillIcon_DefiantStr,
    },
#endif

#if (defined(SID_DefiantMag) && COMMON_SKILL_VALID(SID_DefiantMag))
    [SID_DefiantMag] = {
        .desc = MSG_SKILL_DefiantMag,
        .icon = GFX_SkillIcon_DefiantMag,
    },
#endif

#if (defined(SID_DefiantSkl) && COMMON_SKILL_VALID(SID_DefiantSkl))
    [SID_DefiantSkl] = {
        .desc = MSG_SKILL_DefiantSkl,
        .icon = GFX_SkillIcon_DefiantSkl,
    },
#endif

#if (defined(SID_DefiantSpd) && COMMON_SKILL_VALID(SID_DefiantSpd))
    [SID_DefiantSpd] = {
        .desc = MSG_SKILL_DefiantSpd,
        .icon = GFX_SkillIcon_DefiantSpd,
    },
#endif

#if (defined(SID_DefiantLck) && COMMON_SKILL_VALID(SID_DefiantLck))
    [SID_DefiantLck] = {
        .desc = MSG_SKILL_DefiantLck,
        .icon = GFX_SkillIcon_DefiantLck,
    },
#endif

#if (defined(SID_DefiantDef) && COMMON_SKILL_VALID(SID_DefiantDef))
    [SID_DefiantDef] = {
        .desc = MSG_SKILL_DefiantDef,
        .icon = GFX_SkillIcon_DefiantDef,
    },
#endif

#if (defined(SID_DefiantRes) && COMMON_SKILL_VALID(SID_DefiantRes))
    [SID_DefiantRes] = {
        .desc = MSG_SKILL_DefiantRes,
        .icon = GFX_SkillIcon_DefiantRes,
    },
#endif

#if (defined(SID_DefiantCrit) && COMMON_SKILL_VALID(SID_DefiantCrit))
    [SID_DefiantCrit] = {
        .desc = MSG_SKILL_DefiantCrit,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_DefiantAvoid) && COMMON_SKILL_VALID(SID_DefiantAvoid))
    [SID_DefiantAvoid] = {
        .desc = MSG_SKILL_DefiantAvoid,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Fury) && COMMON_SKILL_VALID(SID_Fury))
    [SID_Fury] = {
        .desc = MSG_SKILL_Fury,
        .icon = GFX_SkillIcon_Fury,
    },
#endif

#if (defined(SID_FuryPlus) && COMMON_SKILL_VALID(SID_FuryPlus))
    [SID_FuryPlus] = {
        .desc = MSG_SKILL_FuryPlus,
        .icon = GFX_SkillIcon_FuryPlus,
    },
#endif

#if (defined(SID_FortressDef) && COMMON_SKILL_VALID(SID_FortressDef))
    [SID_FortressDef] = {
        .desc = MSG_SKILL_FortressDef,
        .icon = GFX_SkillIcon_FortressDef,
    },
#endif

#if (defined(SID_FortressRes) && COMMON_SKILL_VALID(SID_FortressRes))
    [SID_FortressRes] = {
        .desc = MSG_SKILL_FortressRes,
        .icon = GFX_SkillIcon_FortressRes,
    },
#endif

#if (defined(SID_LifeAndDeath) && COMMON_SKILL_VALID(SID_LifeAndDeath))
    [SID_LifeAndDeath] = {
        .desc = MSG_SKILL_LifeAndDeath,
        .icon = GFX_SkillIcon_LifeAndDeath,
    },
#endif

#if (defined(SID_LuckySeven) && COMMON_SKILL_VALID(SID_LuckySeven))
    [SID_LuckySeven] = {
        .desc = MSG_SKILL_LuckySeven,
        .icon = GFX_SkillIcon_LuckySeven,
    },
#endif

#if (defined(SID_Nullify) && COMMON_SKILL_VALID(SID_Nullify))
    [SID_Nullify] = {
        .desc = MSG_SKILL_Nullify,
        .icon = GFX_SkillIcon_Nullify,
    },
#endif

#if (defined(SID_Slayer) && COMMON_SKILL_VALID(SID_Slayer))
    [SID_Slayer] = {
        .desc = MSG_SKILL_Slayer,
        .icon = GFX_SkillIcon_Slayer,
    },
#endif

#if (defined(SID_Skybreaker) && COMMON_SKILL_VALID(SID_Skybreaker))
    [SID_Skybreaker] = {
        .name = MSG_SKILL_Skybreaker_NAME,
        .desc = MSG_SKILL_Skybreaker,
        .icon = GFX_SkillIcon_Skybreaker,
    },
#endif

#if (defined(SID_Resourceful) && COMMON_SKILL_VALID(SID_Resourceful))
    [SID_Resourceful] = {
        .name = MSG_SKILL_Resourceful_NAME,
        .desc = MSG_SKILL_Resourceful,
        .icon = GFX_SkillIcon_Resourceful,
    },
#endif

#if (defined(SID_CritSword) && COMMON_SKILL_VALID(SID_CritSword))
    [SID_CritSword] = {
        .desc = MSG_SKILL_CritSword,
        .icon = GFX_SkillIcon_CritSword,
    },
#endif

#if (defined(SID_CritAxe) && COMMON_SKILL_VALID(SID_CritAxe))
    [SID_CritAxe] = {
        .desc = MSG_SKILL_CritAxe,
        .icon = GFX_SkillIcon_CritAxe,
    },
#endif

#if (defined(SID_CritLance) && COMMON_SKILL_VALID(SID_CritLance))
    [SID_CritLance] = {
        .desc = MSG_SKILL_CritLance,
        .icon = GFX_SkillIcon_CritLance,
    },
#endif

#if (defined(SID_CritBow) && COMMON_SKILL_VALID(SID_CritBow))
    [SID_CritBow] = {
        .desc = MSG_SKILL_CritBow,
        .icon = GFX_SkillIcon_CritBow,
    },
#endif

#if (defined(SID_FaireSword) && COMMON_SKILL_VALID(SID_FaireSword))
    [SID_FaireSword] = {
        .desc = MSG_SKILL_FaireSword,
        .icon = GFX_SkillIcon_FaireSword,
    },
#endif

#if (defined(SID_FaireLance) && COMMON_SKILL_VALID(SID_FaireLance))
    [SID_FaireLance] = {
        .desc = MSG_SKILL_FaireLance,
        .icon = GFX_SkillIcon_FaireLance,
    },
#endif

#if (defined(SID_FaireAxe) && COMMON_SKILL_VALID(SID_FaireAxe))
    [SID_FaireAxe] = {
        .desc = MSG_SKILL_FaireAxe,
        .icon = GFX_SkillIcon_FaireAxe,
    },
#endif

#if (defined(SID_FaireBow) && COMMON_SKILL_VALID(SID_FaireBow))
    [SID_FaireBow] = {
        .desc = MSG_SKILL_FaireBow,
        .icon = GFX_SkillIcon_FaireBow,
    },
#endif

#if (defined(SID_FaireBMag) && COMMON_SKILL_VALID(SID_FaireBMag))
    [SID_FaireBMag] = {
        .desc = MSG_SKILL_FaireBMag,
        .icon = GFX_SkillIcon_FaireBMag,
    },
#endif

#if (defined(SID_Avoid) && COMMON_SKILL_VALID(SID_Avoid))
    [SID_Avoid] = {
        .desc = MSG_SKILL_Avoid,
        .icon = GFX_SkillIcon_Avoid,
    },
#endif

#if (defined(SID_AvoidSword) && COMMON_SKILL_VALID(SID_AvoidSword))
    [SID_AvoidSword] = {
        .desc = MSG_SKILL_AvoidSword,
        .icon = GFX_SkillIcon_AvoidSword,
    },
#endif

#if (defined(SID_RuinedBlade) && COMMON_SKILL_VALID(SID_RuinedBlade))
    [SID_RuinedBlade] = {
        .desc = MSG_SKILL_RuinedBlade,
        .icon = GFX_SkillIcon_RuinedBlade,
    },
#endif

#if (defined(SID_HeavyBlade) && COMMON_SKILL_VALID(SID_HeavyBlade))
    [SID_HeavyBlade] = {
        .desc = MSG_SKILL_HeavyBlade,
        .icon = GFX_SkillIcon_HeavyBlade,
    },
#endif

#if (defined(SID_FlashingBlade) && COMMON_SKILL_VALID(SID_FlashingBlade))
    [SID_FlashingBlade] = {
        .desc = MSG_SKILL_FlashingBlade,
        .icon = GFX_SkillIcon_FlashingBlade,
    },
#endif

#if (defined(SID_HeavyBladePlus) && COMMON_SKILL_VALID(SID_HeavyBladePlus))
    [SID_HeavyBladePlus] = {
        .desc = MSG_SKILL_HeavyBladePlus,
        .icon = GFX_SkillIcon_HeavyBladePlus,
    },
#endif

#if (defined(SID_FlashingBladePlus) && COMMON_SKILL_VALID(SID_FlashingBladePlus))
    [SID_FlashingBladePlus] = {
        .desc = MSG_SKILL_FlashingBladePlus,
        .icon = GFX_SkillIcon_FlashingBladePlus,
    },
#endif

#if (defined(SID_LunaAttack) && COMMON_SKILL_VALID(SID_LunaAttack))
    [SID_LunaAttack] = {
        .desc = MSG_SKILL_LunaAttack,
        .icon = GFX_SkillIcon_LunaAttack,
    },
#endif

#if (defined(SID_SorceryBlade) && COMMON_SKILL_VALID(SID_SorceryBlade))
    [SID_SorceryBlade] = {
        .desc = MSG_SKILL_SorceryBlade,
        .icon = GFX_SkillIcon_SorceryBlade,
    },
#endif

#if (defined(SID_Fortune) && COMMON_SKILL_VALID(SID_Fortune))
    [SID_Fortune] = {
        .desc = MSG_SKILL_Fortune,
        .icon = GFX_SkillIcon_Fortune,
    },
#endif

#if (defined(SID_BlueFlame) && COMMON_SKILL_VALID(SID_BlueFlame))
    [SID_BlueFlame] = {
        .desc = MSG_SKILL_BlueFlame,
        .icon = GFX_SkillIcon_BlueFlame,
    },
#endif

#if (defined(SID_SwordBreaker) && COMMON_SKILL_VALID(SID_SwordBreaker))
    [SID_SwordBreaker] = {
        .desc = MSG_SKILL_SwordBreaker,
        .icon = GFX_SkillIcon_SwordBreaker,
    },
#endif

#if (defined(SID_AxeBreaker) && COMMON_SKILL_VALID(SID_AxeBreaker))
    [SID_AxeBreaker] = {
        .desc = MSG_SKILL_AxeBreaker,
        .icon = GFX_SkillIcon_AxeBreaker,
    },
#endif

#if (defined(SID_LanceBreaker) && COMMON_SKILL_VALID(SID_LanceBreaker))
    [SID_LanceBreaker] = {
        .desc = MSG_SKILL_LanceBreaker,
        .icon = GFX_SkillIcon_LanceBreaker,
    },
#endif

#if (defined(SID_BowBreaker) && COMMON_SKILL_VALID(SID_BowBreaker))
    [SID_BowBreaker] = {
        .desc = MSG_SKILL_BowBreaker,
        .icon = GFX_SkillIcon_BowBreaker,
    },
#endif

#if (defined(SID_BMagBreaker) && COMMON_SKILL_VALID(SID_BMagBreaker))
    [SID_BMagBreaker] = {
        .desc = MSG_SKILL_BMagBreaker,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Vantage) && COMMON_SKILL_VALID(SID_Vantage))
    [SID_Vantage] = {
        .desc = MSG_SKILL_Vantage,
        .icon = GFX_SkillIcon_Vantage,
    },
#endif

#if (defined(SID_Desperation) && COMMON_SKILL_VALID(SID_Desperation))
    [SID_Desperation] = {
        .desc = MSG_SKILL_Desperation,
        .icon = GFX_SkillIcon_Desperation,
    },
#endif

#if (defined(SID_QuickRiposte) && COMMON_SKILL_VALID(SID_QuickRiposte))
    [SID_QuickRiposte] = {
        .name = MSA_SKILL_QuickRiposte_NAME,
        .desc = MSG_SKILL_QuickRiposte,
        .icon = GFX_SkillIcon_QuickRiposte,
    },
#endif

#if (defined(SID_WaryFighter) && COMMON_SKILL_VALID(SID_WaryFighter))
    [SID_WaryFighter] = {
        .desc = MSG_SKILL_WaryFighter,
        .icon = GFX_SkillIcon_WaryFighter,
    },
#endif

#if (defined(SID_DoubleLion) && COMMON_SKILL_VALID(SID_DoubleLion))
    [SID_DoubleLion] = {
        .name = MSG_SKILL_DoubleLion_NAME,
        .desc = MSG_SKILL_DoubleLion,
        .icon = GFX_SkillIcon_DoubleLion,
    },
#endif

#if (defined(SID_Adept) && COMMON_SKILL_VALID(SID_Adept))
    [SID_Adept] = {
        .desc = MSG_SKILL_Adept,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Astra) && COMMON_SKILL_VALID(SID_Astra))
    [SID_Astra] = {
        .desc = MSG_SKILL_Astra,
        .icon = GFX_SkillIcon_Astra,
    },
#endif

#if (defined(SID_Aegis) && COMMON_SKILL_VALID(SID_Aegis))
    [SID_Aegis] = {
        .desc = MSG_SKILL_Aegis,
        .icon = GFX_SkillIcon_Aegis,
    },
#endif

#if (defined(SID_Pavise) && COMMON_SKILL_VALID(SID_Pavise))
    [SID_Pavise] = {
        .desc = MSG_SKILL_Pavise,
        .icon = GFX_SkillIcon_Pavise,
    },
#endif

#if (defined(SID_Bane) && COMMON_SKILL_VALID(SID_Bane))
    [SID_Bane] = {
        .desc = MSG_SKILL_Bane,
        .icon = GFX_SkillIcon_Bane,
    },
#endif

#if (defined(SID_Mercy) && COMMON_SKILL_VALID(SID_Mercy))
    [SID_Mercy] = {
        .desc = MSG_SKILL_Mercy,
        .icon = GFX_SkillIcon_Mercy,
    },
#endif

#if (defined(SID_Aether) && COMMON_SKILL_VALID(SID_Aether))
    [SID_Aether] = {
        .desc = MSG_SKILL_Aether,
        .icon = GFX_SkillIcon_Aether,
    },
#endif

#if (defined(SID_Corona) && COMMON_SKILL_VALID(SID_Corona))
    [SID_Corona] = {
        .desc = MSG_SKILL_Corona,
        .icon = GFX_SkillIcon_Corona,
    },
#endif

#if (defined(SID_Luna) && COMMON_SKILL_VALID(SID_Luna))
    [SID_Luna] = {
        .desc = MSG_SKILL_Luna,
        .icon = GFX_SkillIcon_Luna,
    },
#endif

#if (defined(SID_Flare) && COMMON_SKILL_VALID(SID_Flare))
    [SID_Flare] = {
        .desc = MSG_SKILL_Flare,
        .icon = GFX_SkillIcon_Flare,
    },
#endif

#if (defined(SID_Sol) && COMMON_SKILL_VALID(SID_Sol))
    [SID_Sol] = {
        .desc = MSG_SKILL_Sol,
        .icon = GFX_SkillIcon_Sol,
    },
#endif

#if (defined(SID_DragonFang) && COMMON_SKILL_VALID(SID_DragonFang))
    [SID_DragonFang] = {
        .desc = MSG_SKILL_DragonFang,
        .icon = GFX_SkillIcon_DragonFang,
    },
#endif

#if (defined(SID_Foresight) && COMMON_SKILL_VALID(SID_Foresight))
    [SID_Foresight] = {
        .desc = MSG_SKILL_Foresight,
        .icon = GFX_SkillIcon_Foresight,
    },
#endif

#if (defined(SID_BlowDarting) && COMMON_SKILL_VALID(SID_BlowDarting))
    [SID_BlowDarting] = {
        .desc = MSG_SKILL_BlowDarting,
        .icon = GFX_SkillIcon_BlowDarting,
    },
#endif

#if (defined(SID_BlowDeath) && COMMON_SKILL_VALID(SID_BlowDeath))
    [SID_BlowDeath] = {
        .desc = MSG_SKILL_BlowDeath,
        .icon = GFX_SkillIcon_BlowDeath,
    },
#endif

#if (defined(SID_BlowArmored) && COMMON_SKILL_VALID(SID_BlowArmored))
    [SID_BlowArmored] = {
        .desc = MSG_SKILL_BlowArmored,
        .icon = GFX_SkillIcon_BlowArmored,
    },
#endif

#if (defined(SID_BlowFiendish) && COMMON_SKILL_VALID(SID_BlowFiendish))
    [SID_BlowFiendish] = {
        .desc = MSG_SKILL_BlowFiendish,
        .icon = GFX_SkillIcon_BlowFiendish,
    },
#endif

#if (defined(SID_BlowWarding) && COMMON_SKILL_VALID(SID_BlowWarding))
    [SID_BlowWarding] = {
        .desc = MSG_SKILL_BlowWarding,
        .icon = GFX_SkillIcon_BlowWarding,
    },
#endif

#if (defined(SID_BlowDuelist) && COMMON_SKILL_VALID(SID_BlowDuelist))
    [SID_BlowDuelist] = {
        .desc = MSG_SKILL_BlowDuelist,
        .icon = GFX_SkillIcon_BlowDuelist,
    },
#endif

#if (defined(SID_BlowUncanny) && COMMON_SKILL_VALID(SID_BlowUncanny))
    [SID_BlowUncanny] = {
        .desc = MSG_SKILL_BlowUncanny,
        .icon = GFX_SkillIcon_BlowUncanny,
    },
#endif

#if (defined(SID_BlowKilling) && COMMON_SKILL_VALID(SID_BlowKilling))
    [SID_BlowKilling] = {
        .desc = MSG_SKILL_BlowKilling,
        .icon = GFX_SkillIcon_BlowKilling,
    },
#endif

#if (defined(SID_StanceBracing) && COMMON_SKILL_VALID(SID_StanceBracing))
    [SID_StanceBracing] = {
        .desc = MSG_SKILL_StanceBracing,
        .icon = GFX_SkillIcon_StanceBracing,
    },
#endif

#if (defined(SID_StanceDarting) && COMMON_SKILL_VALID(SID_StanceDarting))
    [SID_StanceDarting] = {
        .desc = MSG_SKILL_StanceDarting,
        .icon = GFX_SkillIcon_StanceDarting,
    },
#endif

#if (defined(SID_StanceFierce) && COMMON_SKILL_VALID(SID_StanceFierce))
    [SID_StanceFierce] = {
        .desc = MSG_SKILL_StanceFierce,
        .icon = GFX_SkillIcon_StanceFierce,
    },
#endif

#if (defined(SID_StanceKestrel) && COMMON_SKILL_VALID(SID_StanceKestrel))
    [SID_StanceKestrel] = {
        .desc = MSG_SKILL_StanceKestrel,
        .icon = GFX_SkillIcon_StanceKestrel,
    },
#endif

#if (defined(SID_StanceMirror) && COMMON_SKILL_VALID(SID_StanceMirror))
    [SID_StanceMirror] = {
        .desc = MSG_SKILL_StanceMirror,
        .icon = GFX_SkillIcon_StanceMirror,
    },
#endif

#if (defined(SID_StanceReady) && COMMON_SKILL_VALID(SID_StanceReady))
    [SID_StanceReady] = {
        .desc = MSG_SKILL_StanceReady,
        .icon = GFX_SkillIcon_StanceReady,
    },
#endif

#if (defined(SID_StanceSteady) && COMMON_SKILL_VALID(SID_StanceSteady))
    [SID_StanceSteady] = {
        .desc = MSG_SKILL_StanceSteady,
        .icon = GFX_SkillIcon_StanceSteady,
    },
#endif

#if (defined(SID_StanceSturdy) && COMMON_SKILL_VALID(SID_StanceSturdy))
    [SID_StanceSturdy] = {
        .desc = MSG_SKILL_StanceSturdy,
        .icon = GFX_SkillIcon_StanceSturdy,
    },
#endif

#if (defined(SID_StanceSwift) && COMMON_SKILL_VALID(SID_StanceSwift))
    [SID_StanceSwift] = {
        .desc = MSG_SKILL_StanceSwift,
        .icon = GFX_SkillIcon_StanceSwift,
    },
#endif

#if (defined(SID_StanceWarding) && COMMON_SKILL_VALID(SID_StanceWarding))
    [SID_StanceWarding] = {
        .desc = MSG_SKILL_StanceWarding,
        .icon = GFX_SkillIcon_StanceWarding,
    },
#endif

#if (defined(SID_RangeBonusBMag1) && COMMON_SKILL_VALID(SID_RangeBonusBMag1))
    [SID_RangeBonusBMag1] = {
        .desc = MSG_SKILL_RangeBonusBMag1,
        .icon = GFX_SkillIcon_RangeBonusBMag1,
    },
#endif

#if (defined(SID_RangeBonusBMag2) && COMMON_SKILL_VALID(SID_RangeBonusBMag2))
    [SID_RangeBonusBMag2] = {
        .desc = MSG_SKILL_RangeBonusBMag2,
        .icon = GFX_SkillIcon_RangeBonusBMag2,
    },
#endif

#if (defined(SID_RangeBonusBow1) && COMMON_SKILL_VALID(SID_RangeBonusBow1))
    [SID_RangeBonusBow1] = {
        .desc = MSG_SKILL_RangeBonusBow1,
        .icon = GFX_SkillIcon_RangeBonusBow1,
    },
#endif

#if (defined(SID_RangeBonusBow2) && COMMON_SKILL_VALID(SID_RangeBonusBow2))
    [SID_RangeBonusBow2] = {
        .desc = MSG_SKILL_RangeBonusBow2,
        .icon = GFX_SkillIcon_RangeBonusBow2,
    },
#endif

#if (defined(SID_RunningStart) && COMMON_SKILL_VALID(SID_RunningStart))
    [SID_RunningStart] = {
        .name = MSG_SKILL_RunningStart_NAME,
        .desc = MSG_SKILL_RunningStart,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Canto) && COMMON_SKILL_VALID(SID_Canto))
    [SID_Canto] = {
        .desc = MSG_SKILL_Canto,
        .icon = GFX_SkillIcon_Canto,
    },
#endif

#if (defined(SID_CantoPlus) && COMMON_SKILL_VALID(SID_CantoPlus))
    [SID_CantoPlus] = {
        .desc = MSG_SKILL_CantoPlus,
        .icon = GFX_SkillIcon_CantoPlus,
    },
#endif

#if (defined(SID_AlertStance) && COMMON_SKILL_VALID(SID_AlertStance))
    [SID_AlertStance] = {
        .desc = MSG_SKILL_AlertStance,
        .icon = GFX_SkillIcon_AlertStance,
    },
#endif

#if (defined(SID_AlertStancePlus) && COMMON_SKILL_VALID(SID_AlertStancePlus))
    [SID_AlertStancePlus] = {
        .desc = MSG_SKILL_AlertStancePlus,
        .icon = GFX_SkillIcon_AlertStancePlus,
    },
#endif

#if (defined(SID_Galeforce) && COMMON_SKILL_VALID(SID_Galeforce))
    [SID_Galeforce] = {
        .desc = MSG_SKILL_Galeforce,
        .icon = GFX_SkillIcon_Galeforce,
    },
#endif

#if (defined(SID_SavageBlow) && COMMON_SKILL_VALID(SID_SavageBlow))
    [SID_SavageBlow] = {
        .desc = MSG_SKILL_SavageBlow,
        .icon = GFX_SkillIcon_SavageBlow,
    },
#endif

#if (defined(SID_BreathOfLife) && COMMON_SKILL_VALID(SID_BreathOfLife))
    [SID_BreathOfLife] = {
        .desc = MSG_SKILL_BreathOfLife,
        .icon = GFX_SkillIcon_BreathOfLife,
    },
#endif

#if (defined(SID_Thunderstorm) && COMMON_SKILL_VALID(SID_Thunderstorm))
    [SID_Thunderstorm] = {
        .name = MSG_SKILL_Thunderstorm_NAME,
        .desc = MSG_SKILL_Thunderstorm,
        .icon = GFX_SkillIcon_Thunderstorm,
    },
#endif

#if (defined(SID_PosReturn) && COMMON_SKILL_VALID(SID_PosReturn))
    [SID_PosReturn] = {
        .desc = MSG_SKILL_PosReturn,
        .icon = GFX_SkillIcon_PosReturn,
    },
#endif

#if (defined(SID_PosSwap) && COMMON_SKILL_VALID(SID_PosSwap))
    [SID_PosSwap] = {
        .desc = MSG_SKILL_PosSwap,
        .icon = GFX_SkillIcon_PosSwap,
    },
#endif

#if (defined(SID_Renewal) && COMMON_SKILL_VALID(SID_Renewal))
    [SID_Renewal] = {
        .desc = MSG_SKILL_Renewal,
        .icon = GFX_SkillIcon_Renewal,
    },
#endif

#if (defined(SID_Imbue) && COMMON_SKILL_VALID(SID_Imbue))
    [SID_Imbue] = {
        .desc = MSG_SKILL_Imbue,
        .icon = GFX_SkillIcon_Imbue,
    },
#endif

#if (defined(SID_Forager) && COMMON_SKILL_VALID(SID_Forager))
    [SID_Forager] = {
        .desc = MSG_SKILL_Forager,
        .icon = GFX_SkillIcon_Forager,
    },
#endif

#if (defined(SID_Anathema) && COMMON_SKILL_VALID(SID_Anathema))
    [SID_Anathema] = {
        .desc = MSG_SKILL_Anathema,
        .icon = GFX_SkillIcon_Anathema,
    },
#endif

#if (defined(SID_Bond) && COMMON_SKILL_VALID(SID_Bond))
    [SID_Bond] = {
        .desc = MSG_SKILL_Bond,
        .icon = GFX_SkillIcon_Bond,
    },
#endif

#if (defined(SID_Charm) && COMMON_SKILL_VALID(SID_Charm))
    [SID_Charm] = {
        .desc = MSG_SKILL_Charm,
        .icon = GFX_SkillIcon_Charm,
    },
#endif

#if (defined(SID_Intimidate) && COMMON_SKILL_VALID(SID_Intimidate))
    [SID_Intimidate] = {
        .desc = MSG_SKILL_Intimidate,
        .icon = GFX_SkillIcon_Intimidate,
    },
#endif

#if (defined(SID_Tantivy) && COMMON_SKILL_VALID(SID_Tantivy))
    [SID_Tantivy] = {
        .desc = MSG_SKILL_Tantivy,
        .icon = GFX_SkillIcon_Tantivy,
    },
#endif

#if (defined(SID_Focus) && COMMON_SKILL_VALID(SID_Focus))
    [SID_Focus] = {
        .desc = MSG_SKILL_Focus,
        .icon = GFX_SkillIcon_Focus,
    },
#endif

#if (defined(SID_Hex) && COMMON_SKILL_VALID(SID_Hex))
    [SID_Hex] = {
        .desc = MSG_SKILL_Hex,
        .icon = GFX_SkillIcon_Hex,
    },
#endif

#if (defined(SID_Infiltrator) && COMMON_SKILL_VALID(SID_Infiltrator))
    [SID_Infiltrator] = {
        .desc = MSG_SKILL_Infiltrator,
        .icon = GFX_SkillIcon_Infiltrator,
    },
#endif

#if (defined(SID_Inspiration) && COMMON_SKILL_VALID(SID_Inspiration))
    [SID_Inspiration] = {
        .desc = MSG_SKILL_Inspiration,
        .icon = GFX_SkillIcon_Inspiration,
    },
#endif

#if (defined(SID_DivinelyInspiring) && COMMON_SKILL_VALID(SID_DivinelyInspiring))
    [SID_DivinelyInspiring] = {
        .desc = MSG_SKILL_DivinelyInspiring,
        .icon = GFX_SkillIcon_DivinelyInspiring,
    },
#endif

#if (defined(SID_VoiceOfPeace) && COMMON_SKILL_VALID(SID_VoiceOfPeace))
    [SID_VoiceOfPeace] = {
        .desc = MSG_SKILL_VoiceOfPeace,
        .icon = GFX_SkillIcon_VoiceOfPeace,
    },
#endif

#if (defined(SID_PowHone) && COMMON_SKILL_VALID(SID_PowHone))
    [SID_PowHone] = {
        .name = MSG_SKILL_PowHone_NAME,
        .desc = MSG_SKILL_PowHone,
        .icon = GFX_SkillIcon_HonePow
    },
#endif

#if (defined(SID_MagHone) && COMMON_SKILL_VALID(SID_MagHone))
    [SID_MagHone] = {
        .name = MSG_SKILL_MagHone_NAME,
        .desc = MSG_SKILL_MagHone,
        .icon = GFX_SkillIcon_HoneMag
    },
#endif

#if (defined(SID_SklHone) && COMMON_SKILL_VALID(SID_SklHone))
    [SID_SklHone] = {
        .name = MSG_SKILL_SklHone_NAME,
        .desc = MSG_SKILL_SklHone,
        .icon = GFX_SkillIcon_HoneSkl
    },
#endif

#if (defined(SID_SpdHone) && COMMON_SKILL_VALID(SID_SpdHone))
    [SID_SpdHone] = {
        .name = MSG_SKILL_SpdHone_NAME,
        .desc = MSG_SKILL_SpdHone,
        .icon = GFX_SkillIcon_HoneSpd
    },
#endif

#if (defined(SID_LckHone) && COMMON_SKILL_VALID(SID_LckHone))
    [SID_LckHone] = {
        .name = MSG_SKILL_LckHone_NAME,
        .desc = MSG_SKILL_LckHone,
        .icon = GFX_SkillIcon_HoneLuk
    },
#endif

#if (defined(SID_DefHone) && COMMON_SKILL_VALID(SID_DefHone))
    [SID_DefHone] = {
        .name = MSG_SKILL_DefHone_NAME,
        .desc = MSG_SKILL_DefHone,
        .icon = GFX_SkillIcon_HoneDef
    },
#endif

#if (defined(SID_ResHone) && COMMON_SKILL_VALID(SID_ResHone))
    [SID_ResHone] = {
        .name = MSG_SKILL_ResHone_NAME,
        .desc = MSG_SKILL_ResHone,
        .icon = GFX_SkillIcon_HoneRes
    },
#endif

#if (defined(SID_BattleRange_Todo1) && COMMON_SKILL_VALID(SID_BattleRange_Todo1))
    [SID_BattleRange_Todo1] = {
        .desc = MSG_SKILL_BattleRange1,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_BattleRange_Todo2) && COMMON_SKILL_VALID(SID_BattleRange_Todo2))
    [SID_BattleRange_Todo2] = {
        .desc = MSG_SKILL_BattleRange2,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_BattleRange_Todo3) && COMMON_SKILL_VALID(SID_BattleRange_Todo3))
    [SID_BattleRange_Todo3] = {
        .desc = MSG_SKILL_BattleRange3,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Steal) && COMMON_SKILL_VALID(SID_Steal))
    [SID_Steal] = {
        .desc = MSG_SKILL_Steal,
        .icon = GFX_SkillIcon_Steal,
    },
#endif

#if (defined(SID_Blossom) && COMMON_SKILL_VALID(SID_Blossom))
    [SID_Blossom] = {
        .desc = MSG_SKILL_Blossom,
        .icon = GFX_SkillIcon_Blossom,
    },
#endif

#if (defined(SID_Paragon) && COMMON_SKILL_VALID(SID_Paragon))
    [SID_Paragon] = {
        .desc = MSG_SKILL_Paragon,
        .icon = GFX_SkillIcon_Paragon,
    },
#endif

#if (defined(SID_VoidCurse) && COMMON_SKILL_VALID(SID_VoidCurse))
    [SID_VoidCurse] = {
        .desc = MSG_SKILL_VoidCurse,
        .icon = GFX_SkillIcon_VoidCurse,
    },
#endif

#if (defined(SID_Aptitude) && COMMON_SKILL_VALID(SID_Aptitude))
    [SID_Aptitude] = {
        .desc = MSG_SKILL_Aptitude,
        .icon = GFX_SkillIcon_Aptitude,
    },
#endif

#if (defined(SID_Peacebringer) && COMMON_SKILL_VALID(SID_VoiceOfPeace))
    [SID_Peacebringer] = {
        .desc = MSG_SKILL_Peacebringer,
        .icon = GFX_SkillIcon_Peacebringer,
    },
#endif

#if (defined(SID_LEGEND_InoriAtk) && COMMON_SKILL_VALID(SID_LEGEND_InoriAtk))
    [SID_LEGEND_InoriAtk] = {
        .desc = MSG_SKILL_LEGEND_InoriAtk,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_InoriAvo) && COMMON_SKILL_VALID(SID_LEGEND_InoriAvo))
    [SID_LEGEND_InoriAvo] = {
        .desc = MSG_SKILL_LEGEND_InoriAvo,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_InoriDef) && COMMON_SKILL_VALID(SID_LEGEND_InoriDef))
    [SID_LEGEND_InoriDef] = {
        .desc = MSG_SKILL_LEGEND_InoriDef,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_4) && COMMON_SKILL_VALID(SID_LEGEND_4))
    [SID_LEGEND_4] = {
        .desc = MSG_WIP,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_5) && COMMON_SKILL_VALID(SID_LEGEND_5))
    [SID_LEGEND_5] = {
        .desc = MSG_WIP,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Frenzy) && COMMON_SKILL_VALID(SID_Frenzy))
    [SID_Frenzy] = {
        .desc = MSG_SKILL_Frenzy,
        .icon = GFX_SkillIcon_Frenzy,
    },
#endif

#if (defined(SID_WonderGuard) && COMMON_SKILL_VALID(SID_WonderGuard))
    [SID_WonderGuard] = {
        .name = MSG_SKILL_WonderGuard_NAME,
        .desc = MSG_SKILL_WonderGuard,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_PoisonPoint) && COMMON_SKILL_VALID(SID_PoisonPoint))
    [SID_PoisonPoint] = {
        .name = MSG_SKILL_PoisonPoint_NAME,
        .desc = MSG_SKILL_PoisonPoint,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Merciless) && COMMON_SKILL_VALID(SID_Merciless))
    [SID_Merciless] = {
        .desc = MSG_SKILL_Merciless,
        .icon = GFX_SkillIcon_Merciless,
    },
#endif

#if (defined(SID_CriticalPierce) && COMMON_SKILL_VALID(SID_CriticalPierce))
    [SID_CriticalPierce] = {
        .desc = MSG_SKILL_CriticalPierce,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_KillingMachine) && COMMON_SKILL_VALID(SID_KillingMachine))
    [SID_KillingMachine] = {
        .name = MSG_SKILL_KillingMachine_NAME,
        .desc = MSG_SKILL_KillingMachine,
        .icon = GFX_SkillIcon_KillingMachine,
    },
#endif

#if (defined(SID_HeavyStrikes) && COMMON_SKILL_VALID(SID_HeavyStrikes))
    [SID_HeavyStrikes] = {
        .name = MSG_SKILL_HeavyStrikes_NAME,
        .desc = MSG_SKILL_HeavyStrikes,
        .icon = GFX_SkillIcon_HeavyStrikes,
    },
#endif

#if (defined(SID_QuickBurn) && COMMON_SKILL_VALID(SID_QuickBurn))
    [SID_QuickBurn] = {
        .desc = MSG_SKILL_QuickBurn,
        .icon = GFX_SkillIcon_QuickBurn,
    },
#endif

#if (defined(SID_SlowBurn) && COMMON_SKILL_VALID(SID_SlowBurn))
    [SID_SlowBurn] = {
        .desc = MSG_SKILL_SlowBurn,
        .icon = GFX_SkillIcon_SlowBurn,
    },
#endif

#if (defined(SID_StanceSpectrum) && COMMON_SKILL_VALID(SID_StanceSpectrum))
    [SID_StanceSpectrum] = {
        .name = MSG_SKILL_StanceSpectrum_NAME,
        .desc = MSG_SKILL_StanceSpectrum,
        .icon = GFX_SkillIcon_StanceSpectrum,
    },
#endif

#if (defined(SID_Technician) && COMMON_SKILL_VALID(SID_Technician))
    [SID_Technician] = {
        .desc = MSG_SKILL_Technician,
        .icon = GFX_SkillIcon_Technician,
    },
#endif

#if (defined(SID_CatchingUp) && COMMON_SKILL_VALID(SID_CatchingUp))
    [SID_CatchingUp] = {
        .desc = MSG_SKILL_CatchingUp,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_BloodTide) && COMMON_SKILL_VALID(SID_BloodTide))
    [SID_BloodTide] = {
        .desc = MSG_SKILL_BloodTide,
        .icon = GFX_SkillIcon_BloodTide,
    },
#endif

#if (defined(SID_WhitePool) && COMMON_SKILL_VALID(SID_WhitePool))
    [SID_WhitePool] = {
        .desc = MSG_SKILL_WhitePool,
        .icon = GFX_SkillIcon_WhitePool,
    },
#endif

#if (defined(SID_NightTide) && COMMON_SKILL_VALID(SID_NightTide))
    [SID_NightTide] = {
        .desc = MSG_SKILL_NightTide,
        .icon = GFX_SkillIcon_NightTide,
    },
#endif

#if (defined(SID_DriveStr) && COMMON_SKILL_VALID(SID_DriveStr))
    [SID_DriveStr] = {
        .desc = MSG_SKILL_DriveStr,
        .icon = GFX_SkillIcon_DriveStr,
    },
#endif

#if (defined(SID_DriveMag) && COMMON_SKILL_VALID(SID_DriveMag))
    [SID_DriveMag] = {
        .desc = MSG_SKILL_DriveMag,
        .icon = GFX_SkillIcon_DriveMag,
    },
#endif

#if (defined(SID_DriveSpd) && COMMON_SKILL_VALID(SID_DriveSpd))
    [SID_DriveSpd] = {
        .desc = MSG_SKILL_DriveSpd,
        .icon = GFX_SkillIcon_DriveSpd,
    },
#endif

#if (defined(SID_DriveDef) && COMMON_SKILL_VALID(SID_DriveDef))
    [SID_DriveDef] = {
        .desc = MSG_SKILL_DriveDef,
        .icon = GFX_SkillIcon_DriveDef,
    },
#endif

#if (defined(SID_DriveRes) && COMMON_SKILL_VALID(SID_DriveRes))
    [SID_DriveRes] = {
        .desc = MSG_SKILL_DriveRes,
        .icon = GFX_SkillIcon_DriveRes,
    },
#endif

#if (defined(SID_Charisma) && COMMON_SKILL_VALID(SID_Charisma))
    [SID_Charisma] = {
        .desc = MSG_SKILL_Charisma,
        .icon = GFX_SkillIcon_Charisma,
    },
#endif

#if (defined(SID_LilysPoise) && COMMON_SKILL_VALID(SID_LilysPoise))
    [SID_LilysPoise] = {
        .desc = MSG_SKILL_LilysPoise,
        .icon = GFX_SkillIcon_LilysPoise,
    },
#endif

#if (defined(SID_Daunt) && COMMON_SKILL_VALID(SID_Daunt))
    [SID_Daunt] = {
        .desc = MSG_SKILL_Daunt,
        .icon = GFX_SkillIcon_Daunt,
    },
#endif

#if (defined(SID_SpurStr) && COMMON_SKILL_VALID(SID_SpurStr))
    [SID_SpurStr] = {
        .desc = MSG_SKILL_SpurStr,
        .icon = GFX_SkillIcon_SpurStr,
    },
#endif

#if (defined(SID_SpurMag) && COMMON_SKILL_VALID(SID_SpurMag))
    [SID_SpurMag] = {
        .desc = MSG_SKILL_SpurMag,
        .icon = GFX_SkillIcon_SpurMag,
    },
#endif

#if (defined(SID_SpurDef) && COMMON_SKILL_VALID(SID_SpurDef))
    [SID_SpurDef] = {
        .desc = MSG_SKILL_SpurDef,
        .icon = GFX_SkillIcon_SpurDef,
    },
#endif

#if (defined(SID_SpurRes) && COMMON_SKILL_VALID(SID_SpurRes))
    [SID_SpurRes] = {
        .desc = MSG_SKILL_SpurRes,
        .icon = GFX_SkillIcon_SpurRes,
    },
#endif

#if (defined(SID_SpurSpd) && COMMON_SKILL_VALID(SID_SpurSpd))
    [SID_SpurSpd] = {
        .desc = MSG_SKILL_SpurSpd,
        .icon = GFX_SkillIcon_SpurSpd,
    },
#endif

#if (defined(SID_BattleVeteran) && COMMON_SKILL_VALID(SID_BattleVeteran))
    [SID_BattleVeteran] = {
        .name = MSG_SKILL_BattleVeteran_NAME,
        .desc = MSG_SKILL_BattleVeteran,
        .icon = GFX_SkillIcon_BattleVeteran,
    },
#endif

#if (defined(SID_Gentilhomme) && COMMON_SKILL_VALID(SID_Gentilhomme))
    [SID_Gentilhomme] = {
        .desc = MSG_SKILL_Gentilhomme,
        .icon = GFX_SkillIcon_Gentilhomme,
    },
#endif

#if (defined(SID_Demoiselle) && COMMON_SKILL_VALID(SID_Demoiselle))
    [SID_Demoiselle] = {
        .desc = MSG_SKILL_Demoiselle,
        .icon = GFX_SkillIcon_Demoiselle,
    },
#endif

#if (defined(SID_Solidarity) && COMMON_SKILL_VALID(SID_Solidarity))
    [SID_Solidarity] = {
        .desc = MSG_SKILL_Solidarity,
        .icon = GFX_SkillIcon_Solidarity,
    },
#endif

#if (defined(SID_StoneBody) && COMMON_SKILL_VALID(SID_StoneBody))
    [SID_StoneBody] = {
        .desc = MSG_SKILL_StoneBody,
        .icon = GFX_SkillIcon_StoneBody,
    },
#endif

#if (defined(SID_Chivalry) && COMMON_SKILL_VALID(SID_Chivalry))
    [SID_Chivalry] = {
        .desc = MSG_SKILL_Chivalry,
        .icon = GFX_SkillIcon_Chivalry,
    },
#endif

#if (defined(SID_Pragmatic) && COMMON_SKILL_VALID(SID_Pragmatic))
    [SID_Pragmatic] = {
        .desc = MSG_SKILL_Pragmatic,
        .icon = GFX_SkillIcon_Pragmatic,
    },
#endif

#if (defined(SID_WindDisciple) && COMMON_SKILL_VALID(SID_WindDisciple))
    [SID_WindDisciple] = {
        .desc = MSG_SKILL_WindDisciple,
        .icon = GFX_SkillIcon_WindDisciple,
    },
#endif

#if (defined(SID_Perfectionist) && COMMON_SKILL_VALID(SID_Perfectionist))
    [SID_Perfectionist] = {
        .desc = MSG_SKILL_Perfectionist,
        .icon = GFX_SkillIcon_Perfectionist,
    },
#endif

#if (defined(SID_CriticalForce) && COMMON_SKILL_VALID(SID_CriticalForce))
    [SID_CriticalForce] = {
        .name = MSG_SKILL_CriticalForce_NAME,
        .desc = MSG_SKILL_CriticalForce,
        .icon = GFX_SkillIcon_CriticalForce,
    },
#endif

#if (defined(SID_StrongRiposte) && COMMON_SKILL_VALID(SID_StrongRiposte))
    [SID_StrongRiposte] = {
        .name = MSG_SKILL_StrongRiposte_NAME,
        .desc = MSG_SKILL_StrongRiposte,
        .icon = GFX_SkillIcon_StrongRiposte,
    },
#endif

#if (defined(SID_Patience) && COMMON_SKILL_VALID(SID_Patience))
    [SID_Patience] = {
        .desc = MSG_SKILL_Patience,
        .icon = GFX_SkillIcon_Patience,
    },
#endif

#if (defined(SID_Pursuit) && COMMON_SKILL_VALID(SID_Pursuit))
    [SID_Pursuit] = {
        .desc = MSG_SKILL_Pursuit,
        .icon = GFX_SkillIcon_Pursuit,
    },
#endif

#if (defined(SID_DragonSkin) && COMMON_SKILL_VALID(SID_DragonSkin))
    [SID_DragonSkin] = {
        .name = MSG_SKILL_DragonSkin_NAME,
        .desc = MSG_SKILL_DragonSkin,
        .icon = GFX_SkillIcon_DragonSkin,
    },
#endif

#if (defined(SID_Hawkeye) && COMMON_SKILL_VALID(SID_Hawkeye))
    [SID_Hawkeye] = {
        .desc = MSG_SKILL_Hawkeye,
        .icon = GFX_SkillIcon_Hawkeye,
    },
#endif

#if (defined(SID_KeenFighter) && COMMON_SKILL_VALID(SID_KeenFighter))
    [SID_KeenFighter] = {
        .name = MSG_SKILL_KeenFighter_NAME,
        .desc = MSG_SKILL_KeenFighter,
        .icon = GFX_SkillIcon_KeenFighter,
    },
#endif

#if (defined(SID_FireBoost) && COMMON_SKILL_VALID(SID_FireBoost))
    [SID_FireBoost] = {
        .desc = MSG_SKILL_FireBoost,
        .icon = GFX_SkillIcon_FireBoost,
    },
#endif

#if (defined(SID_WindBoost) && COMMON_SKILL_VALID(SID_WindBoost))
    [SID_WindBoost] = {
        .desc = MSG_SKILL_WindBoost,
        .icon = GFX_SkillIcon_WindBoost,
    },
#endif

#if (defined(SID_EarthBoost) && COMMON_SKILL_VALID(SID_EarthBoost))
    [SID_EarthBoost] = {
        .desc = MSG_SKILL_EarthBoost,
        .icon = GFX_SkillIcon_EarthBoost,
    },
#endif

#if (defined(SID_WaterBoost) && COMMON_SKILL_VALID(SID_WaterBoost))
    [SID_WaterBoost] = {
        .desc = MSG_SKILL_WaterBoost,
        .icon = GFX_SkillIcon_WaterBoost,
    },
#endif

#if (defined(SID_FieryBlood) && COMMON_SKILL_VALID(SID_FieryBlood))
    [SID_FieryBlood] = {
        .name = MSG_SKILL_FieryBlood_NAME,
        .desc = MSG_SKILL_FieryBlood,
        .icon = GFX_SkillIcon_FieryBlood,
    },
#endif

#if (defined(SID_Colossus) && COMMON_SKILL_VALID(SID_Colossus))
    [SID_Colossus] = {
        .desc = MSG_SKILL_Colossus,
        .icon = GFX_SkillIcon_Colossus,
    },
#endif

#if (defined(SID_LightWeight) && COMMON_SKILL_VALID(SID_LightWeight))
    [SID_LightWeight] = {
        .desc = MSG_SKILL_LightWeight,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_EvenFooted) && COMMON_SKILL_VALID(SID_EvenFooted))
    [SID_EvenFooted] = {
        .desc = MSG_SKILL_EvenFooted,
        .icon = GFX_SkillIcon_EvenFooted,
    },
#endif

#if (defined(SID_QuickDraw) && COMMON_SKILL_VALID(SID_QuickDraw))
    [SID_QuickDraw] = {
        .desc = MSG_SKILL_QuickDraw,
        .icon = GFX_SkillIcon_QuickDraw,
    },
#endif

#if (defined(SID_ArcaneBlade) && COMMON_SKILL_VALID(SID_ArcaneBlade))
    [SID_ArcaneBlade] = {
        .desc = MSG_SKILL_ArcaneBlade,
        .icon = GFX_SkillIcon_ArcaneBlade,
    },
#endif

#if (defined(SID_ElbowRoom) && COMMON_SKILL_VALID(SID_ElbowRoom))
    [SID_ElbowRoom] = {
        .desc = MSG_SKILL_ElbowRoom,
        .icon = GFX_SkillIcon_ElbowRoom,
    },
#endif

#if (defined(SID_NaturalCover) && COMMON_SKILL_VALID(SID_NaturalCover))
    [SID_NaturalCover] = {
        .name = MSG_SKILL_NaturalCover_NAME,
        .desc = MSG_SKILL_NaturalCover,
        .icon = GFX_SkillIcon_NaturalCover,
    },
#endif

#if (defined(SID_ChaosStyle) && COMMON_SKILL_VALID(SID_ChaosStyle))
    [SID_ChaosStyle] = {
        .desc = MSG_SKILL_ChaosStyle,
        .icon = GFX_SkillIcon_ChaosStyle,
    },
#endif

#if (defined(SID_PushDefense) && COMMON_SKILL_VALID(SID_PushDefense))
    [SID_PushDefense] = {
        .name = MSG_SKILL_PushDefense_NAME,
        .desc = MSG_SKILL_PushDefense,
        .icon = GFX_SkillIcon_PushDefense,
    },
#endif

#if (defined(SID_PushMagic) && COMMON_SKILL_VALID(SID_PushMagic))
    [SID_PushMagic] = {
        .name = MSG_SKILL_PushMagic_NAME,
        .desc = MSG_SKILL_PushMagic,
        .icon = GFX_SkillIcon_PushMagic,
    },
#endif

#if (defined(SID_PushResistance) && COMMON_SKILL_VALID(SID_PushResistance))
    [SID_PushResistance] = {
        .name = MSG_SKILL_PushResistance_NAME,
        .desc = MSG_SKILL_PushResistance,
        .icon = GFX_SkillIcon_PushResistance,
    },
#endif

#if (defined(SID_PushSkill) && COMMON_SKILL_VALID(SID_PushSkill))
    [SID_PushSkill] = {
        .name = MSG_SKILL_PushSkill_NAME,
        .desc = MSG_SKILL_PushSkill,
        .icon = GFX_SkillIcon_PushSkill,
    },
#endif

#if (defined(SID_PushSpeed) && COMMON_SKILL_VALID(SID_PushSpeed))
    [SID_PushSpeed] = {
        .name = MSG_SKILL_PushSpeed_NAME,
        .desc = MSG_SKILL_PushSpeed,
        .icon = GFX_SkillIcon_PushSpeed,
    },
#endif

#if (defined(SID_PushStrength) && COMMON_SKILL_VALID(SID_PushStrength))
    [SID_PushStrength] = {
        .name = MSG_SKILL_PushStrength_NAME,
        .desc = MSG_SKILL_PushStrength,
        .icon = GFX_SkillIcon_PushStrength,
    },
#endif

#if (defined(SID_PushLuck) && COMMON_SKILL_VALID(SID_PushLuck))
    [SID_PushLuck] = {
        .name = MSG_SKILL_PushLuck_NAME,
        .desc = MSG_SKILL_PushLuck,
        .icon = GFX_SkillIcon_PushLuck,
    },
#endif

#if (defined(SID_PushMovement) && COMMON_SKILL_VALID(SID_PushMovement))
    [SID_PushMovement] = {
        .name = MSG_SKILL_PushMovement_NAME,
        .desc = MSG_SKILL_PushMovement,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_PushSpectrum) && COMMON_SKILL_VALID(SID_PushSpectrum))
    [SID_PushSpectrum] = {
        .name = MSG_SKILL_PushSpectrum_NAME,
        .desc = MSG_SKILL_PushSpectrum,
        .icon = GFX_SkillIcon_PushSpectrum,
    },
#endif

#if (defined(SID_Charge) && COMMON_SKILL_VALID(SID_Charge))
    [SID_Charge] = {
        .desc = MSG_SKILL_Charge,
        .icon = GFX_SkillIcon_Charge,
    },
#endif

#if (defined(SID_Wrath) && COMMON_SKILL_VALID(SID_Wrath))
    [SID_Wrath] = {
        .desc = MSG_SKILL_Wrath,
        .icon = GFX_SkillIcon_Wrath,
    },
#endif

#if (defined(SID_Vigilance) && COMMON_SKILL_VALID(SID_Vigilance))
    [SID_Vigilance] = {
        .desc = MSG_SKILL_Vigilance,
        .icon = GFX_SkillIcon_Vigilance,
    },
#endif

#if (defined(SID_OutdoorFighter) && COMMON_SKILL_VALID(SID_OutdoorFighter))
    [SID_OutdoorFighter] = {
        .name = MSG_SKILL_OutdoorFighter_NAME,
        .desc = MSG_SKILL_OutdoorFighter,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_DancingBlade) && COMMON_SKILL_VALID(SID_DancingBlade))
    [SID_DancingBlade] = {
        .name = MSG_SKILL_DancingBlade_NAME,
        .desc = MSG_SKILL_DancingBlade,
        .icon = GFX_SkillIcon_DancingBlade,
    },
#endif

#if (defined(SID_KnightAspirant) && COMMON_SKILL_VALID(SID_KnightAspirant))
    [SID_KnightAspirant] = {
        .name = MSG_SKILL_KnightAspirant_NAME,
        .desc = MSG_SKILL_KnightAspirant,
        .icon = GFX_SkillIcon_KnightAspirant,
    },
#endif

#if (defined(SID_Outrider) && COMMON_SKILL_VALID(SID_Outrider))
    [SID_Outrider] = {
        .desc = MSG_SKILL_Outrider,
        .icon = GFX_SkillIcon_Outrider
    },
#endif

#if (defined(SID_EvenRhythm) && COMMON_SKILL_VALID(SID_EvenRhythm))
    [SID_EvenRhythm] = {
        .desc = MSG_SKILL_EvenRhythm,
        .icon = GFX_SkillIcon_EvenRhythm
    },
#endif

#if (defined(SID_OddRhythm) && COMMON_SKILL_VALID(SID_OddRhythm))
    [SID_OddRhythm] = {
        .desc = MSG_SKILL_OddRhythm,
        .icon = GFX_SkillIcon_OddRhythm
    },
#endif

#if (defined(SID_OddFooted) && COMMON_SKILL_VALID(SID_OddFooted))
    [SID_OddFooted] = {
        .desc = MSG_SKILL_OddFooted,
        .icon = GFX_SkillIcon_OddFooted,
    },
#endif

#if (defined(SID_Corrosion) && COMMON_SKILL_VALID(SID_Corrosion))
    [SID_Corrosion] = {
        .desc = MSG_SKILL_Corrosion,
        .icon = GFX_SkillIcon_Corrosion,
    },
#endif

#if (defined(SID_NoGuard) && COMMON_SKILL_VALID(SID_NoGuard))
    [SID_NoGuard] = {
        .desc = MSG_SKILL_NoGuard,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Puissance) && COMMON_SKILL_VALID(SID_Puissance))
    [SID_Puissance] = {
        .desc = MSG_SKILL_Puissance,
        .icon = GFX_SkillIcon_Puissance,
    },
#endif

#if (defined(SID_Prescience) && COMMON_SKILL_VALID(SID_Prescience))
    [SID_Prescience] = {
        .desc = MSG_SKILL_Prescience,
        .icon = GFX_SkillIcon_Prescience,
    },
#endif

#if (defined(SID_Resolve) && COMMON_SKILL_VALID(SID_Resolve))
    [SID_Resolve] = {
        .desc = MSG_SKILL_Resolve,
        .icon = GFX_SkillIcon_Resolve,
    },
#endif

#if (defined(SID_SilentPride) && COMMON_SKILL_VALID(SID_SilentPride))
    [SID_SilentPride] = {
        .desc = MSG_SKILL_SilentPride,
        .icon = GFX_SkillIcon_SilentPride,
    },
#endif

#if (defined(SID_RightfulKing) && COMMON_SKILL_VALID(SID_RightfulKing))
    [SID_RightfulKing] = {
        .desc = MSG_SKILL_RightfulKing,
        .icon = GFX_SkillIcon_RightfulKing,
    },
#endif

#if (defined(SID_RightfulGod) && COMMON_SKILL_VALID(SID_RightfulGod))
    [SID_RightfulGod] = {
        .desc = MSG_SKILL_RightfulGod,
        .icon = GFX_SkillIcon_RightfulGod,
    },
#endif

#if (defined(SID_RightfulArch) && COMMON_SKILL_VALID(SID_RightfulArch))
    [SID_RightfulArch] = {
        .desc = MSG_SKILL_RightfulArch,
        .icon = GFX_SkillIcon_RightfulArch,
    },
#endif

#if (defined(SID_Guts) && COMMON_SKILL_VALID(SID_Guts))
    [SID_Guts] = {
        .desc = MSG_SKILL_Guts,
        .icon = GFX_SkillIcon_Guts,
    },
#endif

#if (defined(SID_Impale) && COMMON_SKILL_VALID(SID_Impale))
    [SID_Impale] = {
        .desc = MSG_SKILL_Impale,
        .icon = GFX_SkillIcon_Impale,
    },
#endif

#if (defined(SID_Hero) && COMMON_SKILL_VALID(SID_Hero))
    [SID_Hero] = {
        .desc = MSG_SKILL_Hero,
        .icon = GFX_SkillIcon_Hero,
    },
#endif

#if (defined(SID_Ignis) && COMMON_SKILL_VALID(SID_Ignis))
    [SID_Ignis] = {
        .desc = MSG_SKILL_Ignis,
        .icon = GFX_SkillIcon_Ignis,
    },
#endif

#if (defined(SID_HolyAura) && COMMON_SKILL_VALID(SID_HolyAura))
    [SID_HolyAura] = {
        .desc = MSG_SKILL_HolyAura,
        .icon = GFX_SkillIcon_HolyAura,
    },
#endif

#if (defined(SID_Loyalty) && COMMON_SKILL_VALID(SID_Loyalty))
    [SID_Loyalty] = {
        .desc = MSG_SKILL_Loyalty,
        .icon = GFX_SkillIcon_Loyalty,
    },
#endif

#if (defined(SID_TowerShield) && COMMON_SKILL_VALID(SID_TowerShield))
    [SID_TowerShield] = {
        .desc = MSG_SKILL_TowerShield,
        .icon = GFX_SkillIcon_TowerShield,
    },
#endif

#if (defined(SID_StunningSmile) && COMMON_SKILL_VALID(SID_StunningSmile))
    [SID_StunningSmile] = {
        .name = MSG_SKILL_StunningSmile_NAME,
        .desc = MSG_SKILL_StunningSmile,
        .icon = GFX_SkillIcon_StunningSmile,
    },
#endif

#if (defined(SID_Trample) && COMMON_SKILL_VALID(SID_Trample))
    [SID_Trample] = {
        .desc = MSG_SKILL_Trample,
        .icon = GFX_SkillIcon_Trample,
    },
#endif

#if (defined(SID_Opportunist) && COMMON_SKILL_VALID(SID_Opportunist))
    [SID_Opportunist] = {
        .desc = MSG_SKILL_Opportunist,
        .icon = GFX_SkillIcon_Opportunist,
    },
#endif

#if (defined(SID_SuperLuck) && COMMON_SKILL_VALID(SID_SuperLuck))
    [SID_SuperLuck] = {
        .desc = MSG_SKILL_SuperLuck,
        .icon = GFX_SkillIcon_SuperLuck,
    },
#endif

#if (defined(SID_ShortShield) && COMMON_SKILL_VALID(SID_ShortShield))
    [SID_ShortShield] = {
        .desc = MSG_SKILL_ShortShield,
        .icon = GFX_SkillIcon_ShortShield,
    },
#endif

#if (defined(SID_Vanity) && COMMON_SKILL_VALID(SID_Vanity))
    [SID_Vanity] = {
        .desc = MSG_SKILL_Vanity,
        .icon = GFX_SkillIcon_Vanity,
    },
#endif

#if (defined(SID_SpeedBoost) && COMMON_SKILL_VALID(SID_SpeedBoost))
    [SID_SpeedBoost] = {
        .desc = MSG_SKILL_SpeedBoost,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Glacies) && COMMON_SKILL_VALID(SID_Glacies))
    [SID_Glacies] = {
        .desc = MSG_SKILL_Glacies,
        .icon = GFX_SkillIcon_Glacies,
    },
#endif

#if (defined(SID_Vengeance) && COMMON_SKILL_VALID(SID_Vengeance))
    [SID_Vengeance] = {
        .desc = MSG_SKILL_Vengeance,
        .icon = GFX_SkillIcon_Vengeance,
    },
#endif

#if (defined(SID_Deadeye) && COMMON_SKILL_VALID(SID_Deadeye))
    [SID_Deadeye] = {
        .desc = MSG_SKILL_Deadeye,
        .icon = GFX_SkillIcon_Deadeye,
    },
#endif

#if (defined(SID_AxeFaith) && COMMON_SKILL_VALID(SID_AxeFaith))
    [SID_AxeFaith] = {
        .desc = MSG_SKILL_AxeFaith,
        .icon = GFX_SkillIcon_AxeFaith,
    },
#endif

#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
    [SID_RuinedBladePlus] = {
        .desc = MSG_SKILL_RuinedBladePlus,
        .icon = GFX_SkillIcon_RuinedBlade,
    },
#endif

#if (defined(SID_COMBAT_Grounder) && COMMON_SKILL_VALID(SID_COMBAT_Grounder))
    [SID_COMBAT_Grounder] = {
        .desc = MSG_SKILL_COMBAT_Grounder,
        .icon = GFX_CombatArtIcon_SwordArtAtk,
    },
#endif

#if (defined(SID_COMBAT_BaneOfMonsters) && COMMON_SKILL_VALID(SID_COMBAT_BaneOfMonsters))
    [SID_COMBAT_BaneOfMonsters] = {
        .desc = MSG_SKILL_COMBAT_BaneOfMonsters,
        .icon = GFX_CombatArtIcon_SwordArtAtk,
    },
#endif

#if (defined(SID_COMBAT_Hexblade) && COMMON_SKILL_VALID(SID_COMBAT_Hexblade))
    [SID_COMBAT_Hexblade] = {
        .desc = MSG_SKILL_COMBAT_Hexblade,
        .icon = GFX_CombatArtIcon_SwordArtMag,
    },
#endif

#if (defined(SID_COMBAT_FoudroyantStrike) && COMMON_SKILL_VALID(SID_COMBAT_FoudroyantStrike))
    [SID_COMBAT_FoudroyantStrike] = {
        .desc = MSG_SKILL_COMBAT_FoudroyantStrike,
        .icon = GFX_CombatArtIcon_SwordArtCrit,
    },
#endif

#if (defined(SID_COMBAT_TempestLance) && COMMON_SKILL_VALID(SID_COMBAT_TempestLance))
    [SID_COMBAT_TempestLance] = {
        .desc = MSG_SKILL_COMBAT_TempestLance,
        .icon = GFX_CombatArtIcon_LanceArtAtk,
    },
#endif

#if (defined(SID_COMBAT_SwiftStrikes) && COMMON_SKILL_VALID(SID_COMBAT_SwiftStrikes))
    [SID_COMBAT_SwiftStrikes] = {
        .desc = MSG_SKILL_COMBAT_SwiftStrikes,
        .icon = GFX_CombatArtIcon_LanceArtAtk,
    },
#endif

#if (defined(SID_COMBAT_FrozenLance) && COMMON_SKILL_VALID(SID_COMBAT_FrozenLance))
    [SID_COMBAT_FrozenLance] = {
        .desc = MSG_SKILL_COMBAT_FrozenLance,
        .icon = GFX_CombatArtIcon_LanceArtMag,
    },
#endif

#if (defined(SID_COMBAT_Vengeance) && COMMON_SKILL_VALID(SID_COMBAT_Vengeance))
    [SID_COMBAT_Vengeance] = {
        .desc = MSG_SKILL_COMBAT_Vengeance,
        .icon = GFX_CombatArtIcon_LanceArtCrit,
    },
#endif

#if (defined(SID_COMBAT_HelmSplitter) && COMMON_SKILL_VALID(SID_COMBAT_HelmSplitter))
    [SID_COMBAT_HelmSplitter] = {
        .desc = MSG_SKILL_COMBAT_HelmSplitter,
        .icon = GFX_CombatArtIcon_AxeArtAtk,
    },
#endif

#if (defined(SID_COMBAT_LightningAxe) && COMMON_SKILL_VALID(SID_COMBAT_LightningAxe))
    [SID_COMBAT_LightningAxe] = {
        .desc = MSG_SKILL_COMBAT_LightningAxe,
        .icon = GFX_CombatArtIcon_AxeArtMag,
    },
#endif

#if (defined(SID_COMBAT_WarMasterStrike) && COMMON_SKILL_VALID(SID_COMBAT_WarMasterStrike))
    [SID_COMBAT_WarMasterStrike] = {
        .desc = MSG_SKILL_COMBAT_WarMasterStrike,
        .icon = GFX_CombatArtIcon_AxeArtCrit,
    },
#endif

#if (defined(SID_COMBAT_CurvedShot) && COMMON_SKILL_VALID(SID_COMBAT_CurvedShot))
    [SID_COMBAT_CurvedShot] = {
        .desc = MSG_SKILL_COMBAT_CurvedShot,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif

#if (defined(SID_COMBAT_Deadeye) && COMMON_SKILL_VALID(SID_COMBAT_Deadeye))
    [SID_COMBAT_Deadeye] = {
        .desc = MSG_SKILL_COMBAT_Deadeye,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif

#if (defined(SID_COMBAT_Encloser) && COMMON_SKILL_VALID(SID_COMBAT_Encloser))
    [SID_COMBAT_Encloser] = {
        .desc = MSG_SKILL_COMBAT_Encloser,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_EncloserPlus) && COMMON_SKILL_VALID(SID_COMBAT_EncloserPlus))
    [SID_COMBAT_EncloserPlus] = {
        .desc = MSG_SKILL_COMBAT_EncloserPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_MonsterBlast) && COMMON_SKILL_VALID(SID_COMBAT_MonsterBlast))
    [SID_COMBAT_MonsterBlast] = {
        .desc = MSG_SKILL_COMBAT_MonsterBlast,
        .icon = GFX_CombatArtIcon_BowArtCrit,
    },
#endif

#if (defined(SID_COMBAT_SchismShot) && COMMON_SKILL_VALID(SID_COMBAT_SchismShot))
    [SID_COMBAT_SchismShot] = {
        .desc = MSG_SKILL_COMBAT_SchismShot,
        .icon = GFX_CombatArtIcon_BowArtCrit,
    },
#endif

#if (defined(SID_COMBAT_BreakShot) && COMMON_SKILL_VALID(SID_COMBAT_BreakShot))
    [SID_COMBAT_BreakShot] = {
        .desc = MSG_SKILL_COMBAT_BreakShot,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_BreakShotPlus) && COMMON_SKILL_VALID(SID_COMBAT_BreakShotPlus))
    [SID_COMBAT_BreakShotPlus] = {
        .desc = MSG_SKILL_COMBAT_BreakShotPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_WaningShot) && COMMON_SKILL_VALID(SID_COMBAT_WaningShot))
    [SID_COMBAT_WaningShot] = {
        .desc = MSG_SKILL_COMBAT_WaningShot,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_WaningShotPlus) && COMMON_SKILL_VALID(SID_COMBAT_WaningShotPlus))
    [SID_COMBAT_WaningShotPlus] = {
        .desc = MSG_SKILL_COMBAT_WaningShotPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_HuntersVolley) && COMMON_SKILL_VALID(SID_COMBAT_HuntersVolley))
    [SID_COMBAT_HuntersVolley] = {
        .desc = MSG_SKILL_COMBAT_HuntersVolley,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif

#if (defined(SID_GreatShield) && COMMON_SKILL_VALID(SID_GreatShield)) 
    [SID_GreatShield] = {
        .desc = MSG_SKILL_GreatShield,
        .icon = GFX_SkillIcon_GreatShield,
    },
#endif

#if (defined(SID_HoneCavalry) && COMMON_SKILL_VALID(SID_HoneCavalry))
    [SID_HoneCavalry] = {
        .name = MSG_SKILL_HoneCavalry_NAME,
        .desc = MSG_SKILL_HoneCavalry,
        .icon = GFX_SkillIcon_HoneCavalry
    },
#endif

#if (defined(SID_HoneFlier) && COMMON_SKILL_VALID(SID_HoneFlier))
    [SID_HoneFlier] = {
        .name = MSG_SKILL_HoneFlier_NAME,
        .desc = MSG_SKILL_HoneFlier,
        .icon = GFX_SkillIcon_HoneFlier
    },
#endif

#if (defined(SID_HoneArmor) && COMMON_SKILL_VALID(SID_HoneArmor))
    [SID_HoneArmor] = {
        .name = MSG_SKILL_HoneArmor_NAME,
        .desc = MSG_SKILL_HoneArmor,
        .icon = GFX_SkillIcon_HoneArmor
    },
#endif

#if (defined(SID_FortifyArmor) && COMMON_SKILL_VALID(SID_FortifyArmor))
    [SID_FortifyArmor] = {
        .name = MSG_SKILL_FortifyArmor_NAME,
        .desc = MSG_SKILL_FortifyArmor,
        .icon = GFX_SkillIcon_WIP
    },
#endif

#if (defined(SID_Shade) && COMMON_SKILL_VALID(SID_Shade))
    [SID_Shade] = {
        .desc = MSG_SKILL_Shade,
        .icon = GFX_SkillIcon_Shade,
    },
#endif

#if (defined(SID_ShadePlus) && COMMON_SKILL_VALID(SID_ShadePlus))
    [SID_ShadePlus] = {
        .desc = MSG_SKILL_ShadePlus,
        .icon = GFX_SkillIcon_ShadePlus,
    },
#endif

#if (defined(SID_Provoke) && COMMON_SKILL_VALID(SID_Provoke))
    [SID_Provoke] = {
        .desc = MSG_SKILL_Provoke,
        .icon = GFX_SkillIcon_Provoke,
    },
#endif

#if (defined(SID_Teleportation) && COMMON_SKILL_VALID(SID_Teleportation))
    [SID_Teleportation] = {
        .name = MSG_SKILL_Teleportation_NAME,
        .desc = MSG_SKILL_Teleportation,
        .icon = GFX_SkillIcon_Teleportation,
    },
#endif

#if (defined(SID_Dance) && COMMON_SKILL_VALID(SID_Dance))
    [SID_Dance] = {
        .desc = MSG_SKILL_Dance,
        .icon = GFX_SkillIcon_Dance,
    },
#endif

#if (defined(SID_LockTouch) && COMMON_SKILL_VALID(SID_LockTouch))
    [SID_LockTouch] = {
        .desc = MSG_SKILL_LockTouch,
        .icon = GFX_SkillIcon_LockTouch,
    },
#endif

#if (defined(SID_Summon) && COMMON_SKILL_VALID(SID_Summon))
    [SID_Summon] = {
        .desc = MSG_SKILL_Summon,
        .icon = GFX_SkillIcon_Summon,
    },
#endif

#if (defined(SID_SureShot) && COMMON_SKILL_VALID(SID_SureShot))
    [SID_SureShot] = {
        .desc = MSG_SKILL_SureShot,
        .icon = GFX_SkillIcon_SureShot,
    },
#endif

#if (defined(SID_Crit) && COMMON_SKILL_VALID(SID_Crit))
    [SID_Crit] = {
        .desc = MSG_SKILL_Crit,
        .icon = GFX_SkillIcon_Crit,
    },
#endif

#if (defined(SID_Lethality) && COMMON_SKILL_VALID(SID_Lethality))
    [SID_Lethality] = {
        .desc = MSG_SKILL_Lethality,
        .icon = GFX_SkillIcon_Lethality,
    },
#endif

#if (defined(SID_WatchfulEye) && COMMON_SKILL_VALID(SID_WatchfulEye))
    [SID_WatchfulEye] = {
        .desc = MSG_SKILL_WatchfulEye,
        .icon = GFX_SkillIcon_WatchfulEye,
    },
#endif

#if (defined(SID_InfinityEdge) && COMMON_SKILL_VALID(SID_InfinityEdge))
    [SID_InfinityEdge] = {
        .desc = MSG_SKILL_InfinityEdge,
        .icon = GFX_SkillIcon_InfinityEdge,
    },
#endif

#if (defined(SID_AversaNight) && COMMON_SKILL_VALID(SID_AversaNight))
    [SID_AversaNight] = {
        .name = MSG_SKILL_DEBUFF_Aversa_NAME,
        .desc = MSG_SKILL_DEBUFF_Aversa,
        .icon = GFX_SkillIcon_AversaNight,
    },
#endif

#if (defined(SID_Supply) && COMMON_SKILL_VALID(SID_Supply))
    [SID_Supply] = {
        .desc = MSG_SKILL_Supply,
        .icon = GFX_SkillIcon_Supply,
    },
#endif

#if (defined(SID_Inori) && COMMON_SKILL_VALID(SID_Inori))
    [SID_Inori] = {
        .desc = MSG_SKILL_Inori,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Armsthrift) && COMMON_SKILL_VALID(SID_Armsthrift))
    [SID_Armsthrift] = {
        .desc = MSG_SKILL_Armsthrift,
        .icon = GFX_SkillIcon_Armsthrift,
    },
#endif

#if (defined(SID_HighGround) && COMMON_SKILL_VALID(SID_HighGround))
    [SID_HighGround] = {
        .desc = MSG_SKILL_HighGround,
        .icon = GFX_SkillIcon_HighGround,
    },
#endif

#if (defined(SID_Airlock) && COMMON_SKILL_VALID(SID_Airlock))
    [SID_Airlock] = {
        .desc = MSG_SKILL_Airlock,
        .icon = GFX_SkillIcon_Airlock,
    },
#endif

#if (defined(SID_Drought) && COMMON_SKILL_VALID(SID_Drought))
    [SID_Drought] = {
        .desc = MSG_SKILL_Drought,
        .icon = GFX_SkillIcon_Drought,
    },
#endif

#if (defined(SID_Drizzle) && COMMON_SKILL_VALID(SID_Drizzle))
    [SID_Drizzle] = {
        .desc = MSG_SKILL_Drizzle,
        .icon = GFX_SkillIcon_Drizzle,
    },
#endif

#if (defined(SID_SnowWarning) && COMMON_SKILL_VALID(SID_SnowWarning))
    [SID_SnowWarning] = {
        .desc = MSG_SKILL_SnowWarning,
        .icon = GFX_SkillIcon_SnowWarning,
    },
#endif

#if (defined(SID_SandStream) && COMMON_SKILL_VALID(SID_SandStream))
    [SID_SandStream] = {
        .desc = MSG_SKILL_SandStream,
        .icon = GFX_SkillIcon_SandStream,
    },
#endif

#if (defined(SID_PointBlank) && COMMON_SKILL_VALID(SID_PointBlank))
    [SID_PointBlank] = {
        .desc = MSG_SKILL_PointBlank,
        .icon = GFX_SkillIcon_PointBlank,
    },
#endif

#if (defined(SID_Synchronize) && COMMON_SKILL_VALID(SID_Synchronize))
    [SID_Synchronize] = {
        .desc = MSG_SKILL_Synchronize,
        .icon = GFX_SkillIcon_Synchronize,
    },
#endif

#if (defined(SID_StaffSavant) && COMMON_SKILL_VALID(SID_StaffSavant))
    [SID_StaffSavant] = {
        .desc = MSG_SKILL_StaffSavant,
        .icon = GFX_SkillIcon_StaffSavant,
    },
#endif

#if (defined(SID_Skyguard) && COMMON_SKILL_VALID(SID_Skyguard))
    [SID_Skyguard] = {
        .desc = MSG_SKILL_Skyguard,
        .icon = GFX_SkillIcon_Skyguard,
    },
#endif

#if (defined(SID_Horseguard) && COMMON_SKILL_VALID(SID_Horseguard))
    [SID_Horseguard] = {
        .desc = MSG_SKILL_Horseguard,
        .icon = GFX_SkillIcon_Horseguard,
    },
#endif

#if (defined(SID_Armorboost) && COMMON_SKILL_VALID(SID_Armorboost))
    [SID_Armorboost] = {
        .desc = MSG_SKILL_Armorboost,
        .icon = GFX_SkillIcon_Armorboost,
    },
#endif

#if (defined(SID_GentleFlower) && COMMON_SKILL_VALID(SID_GentleFlower))
    [SID_GentleFlower] = {
        .name = MSG_SKILL_GentleFlower_NAME,
        .desc = MSG_SKILL_GentleFlower,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Admiration) && COMMON_SKILL_VALID(SID_Admiration))
    [SID_Admiration] = {
        .desc = MSG_SKILL_Admiration,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_FairyTaleFolk) && COMMON_SKILL_VALID(SID_FairyTaleFolk))
    [SID_FairyTaleFolk] = {
        .name = MSG_SKILL_FairyTaleFolk_NAME,
        .desc = MSG_SKILL_FairyTaleFolk,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_InitCalm) && COMMON_SKILL_VALID(SID_InitCalm))
    [SID_InitCalm] = {
        .name = MSG_SKILL_InitCalm_NAME,
        .desc = MSG_SKILL_InitCalm,
        .icon = GFX_SkillIcon_InitCalm,
    },
#endif

#if (defined(SID_InitClever) && COMMON_SKILL_VALID(SID_InitClever))
    [SID_InitClever] = {
        .name = MSG_SKILL_InitClever_NAME,
        .desc = MSG_SKILL_InitClever,
        .icon = GFX_SkillIcon_InitClever,
    },
#endif

#if (defined(SID_InitDeft) && COMMON_SKILL_VALID(SID_InitDeft))
    [SID_InitDeft] = {
        .name = MSG_SKILL_InitDeft_NAME,
        .desc = MSG_SKILL_InitDeft,
        .icon = GFX_SkillIcon_InitDeft,
    },
#endif

#if (defined(SID_InitLucky) && COMMON_SKILL_VALID(SID_InitLucky))
    [SID_InitLucky] = {
        .name = MSG_SKILL_InitLucky_NAME,
        .desc = MSG_SKILL_InitLucky,
        .icon = GFX_SkillIcon_InitLucky,
    },
#endif

#if (defined(SID_InitNimble) && COMMON_SKILL_VALID(SID_InitNimble))
    [SID_InitNimble] = {
        .name = MSG_SKILL_InitNimble_NAME,
        .desc = MSG_SKILL_InitNimble,
        .icon = GFX_SkillIcon_InitNimble,
    },
#endif

#if (defined(SID_InitQuick) && COMMON_SKILL_VALID(SID_InitQuick))
    [SID_InitQuick] = {
        .name = MSG_SKILL_InitQuick_NAME,
        .desc = MSG_SKILL_InitQuick,
        .icon = GFX_SkillIcon_InitQuick,
    },
#endif

#if (defined(SID_InitSpectrum) && COMMON_SKILL_VALID(SID_InitSpectrum))
    [SID_InitSpectrum] = {
        .name = MSG_SKILL_InitSpectrum_NAME,
        .desc = MSG_SKILL_InitSpectrum,
        .icon = GFX_SkillIcon_InitSpectrum,
    },
#endif

#if (defined(SID_InitStrong) && COMMON_SKILL_VALID(SID_InitStrong))
    [SID_InitStrong] = {
        .name = MSG_SKILL_InitStrong_NAME,
        .desc = MSG_SKILL_InitStrong,
        .icon = GFX_SkillIcon_InitStrong,
    },
#endif

#if (defined(SID_InitSturdy) && COMMON_SKILL_VALID(SID_InitSturdy))
    [SID_InitSturdy] = {
        .name = MSG_SKILL_InitSturdy_NAME,
        .desc = MSG_SKILL_InitSturdy,
        .icon = GFX_SkillIcon_InitSturdy,
    },
#endif

#if (defined(SID_Lifetaker) && COMMON_SKILL_VALID(SID_Lifetaker))
    [SID_Lifetaker] = {
        .desc = MSG_SKILL_Lifetaker,
        .icon = GFX_SkillIcon_Lifetaker,
    },
#endif

#if (defined(SID_Analytic) && COMMON_SKILL_VALID(SID_Analytic))
    [SID_Analytic] = {
        .desc = MSG_SKILL_Analytic,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_OathDefense) && COMMON_SKILL_VALID(SID_OathDefense))
    [SID_OathDefense] = {
        .name = MSG_SKILL_OathDefense_NAME,
        .desc = MSG_SKILL_OathDefense,
        .icon = GFX_SkillIcon_OathDefense,
    },
#endif

#if (defined(SID_OathLuck) && COMMON_SKILL_VALID(SID_OathLuck))
    [SID_OathLuck] = {
        .name = MSG_SKILL_OathLuck_NAME,
        .desc = MSG_SKILL_OathLuck,
        .icon = GFX_SkillIcon_OathLuck,
    },
#endif

#if (defined(SID_OathMagic) && COMMON_SKILL_VALID(SID_OathMagic))
    [SID_OathMagic] = {
        .name = MSG_SKILL_OathMagic_NAME,
        .desc = MSG_SKILL_OathMagic,
        .icon = GFX_SkillIcon_OathMagic,
    },
#endif

#if (defined(SID_CounterRoar) && COMMON_SKILL_VALID(SID_CounterRoar))
    [SID_CounterRoar] = {
        .desc = MSG_SKILL_CounterRoar,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Gambit) && COMMON_SKILL_VALID(SID_Gambit))
    [SID_Gambit] = {
        .desc = MSG_SKILL_Gambit,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_MagicGambit) && COMMON_SKILL_VALID(SID_MagicGambit))
    [SID_MagicGambit] = {
        .desc = MSG_SKILL_MagicGambit,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_BeastAssault) && COMMON_SKILL_VALID(SID_BeastAssault))
    [SID_BeastAssault] = {
        .name = MSG_SKILL_BeastAssault_NAME,
        .desc = MSG_SKILL_BeastAssault,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LightAndDark) && COMMON_SKILL_VALID(SID_LightAndDark))
    [SID_LightAndDark] = {
        .desc = MSG_SKILL_LightAndDark,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_OathMovement) && COMMON_SKILL_VALID(SID_OathMovement))
    [SID_OathMovement] = {
        .name = MSG_SKILL_OathMovement_NAME,
        .desc = MSG_SKILL_OathMovement,
        .icon = GFX_SkillIcon_OathMovement,
    },
#endif

#if (defined(SID_OathResistance) && COMMON_SKILL_VALID(SID_OathResistance))
    [SID_OathResistance] = {
        .name = MSG_SKILL_OathResistance_NAME,
        .desc = MSG_SKILL_OathResistance,
        .icon = GFX_SkillIcon_OathResistance,
    },
#endif

#if (defined(SID_OathSkill) && COMMON_SKILL_VALID(SID_OathSkill))
    [SID_OathSkill] = {
        .name = MSG_SKILL_OathSkill_NAME,
        .desc = MSG_SKILL_OathSkill,
        .icon = GFX_SkillIcon_OathSkill,
    },
#endif

#if (defined(SID_OathSpeed) && COMMON_SKILL_VALID(SID_OathSpeed))
    [SID_OathSpeed] = {
        .name = MSG_SKILL_OathSpeed_NAME,
        .desc = MSG_SKILL_OathSpeed,
        .icon = GFX_SkillIcon_OathSpeed,
    },
#endif

#if (defined(SID_OathStrength) && COMMON_SKILL_VALID(SID_OathStrength))
    [SID_OathStrength] = {
        .name = MSG_SKILL_OathStrength_NAME,
        .desc = MSG_SKILL_OathStrength,
        .icon = GFX_SkillIcon_OathStrength,
    },
#endif

#if (defined(SID_DivinePulse) && COMMON_SKILL_VALID(SID_DivinePulse))
    [SID_DivinePulse] = {
        .name = MSG_SKILL_DivinePulse_NAME,
        .desc = MSG_SKILL_DivinePulse,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Tsuzuku) && COMMON_SKILL_VALID(SID_Tsuzuku))
    [SID_Tsuzuku] = {
        .desc = MSG_SKILL_Tsuzuku,
        .icon = GFX_SkillIcon_Tsuzuku,
    },
#endif

#if (defined(SID_MysticBoost) && COMMON_SKILL_VALID(SID_MysticBoost))
    [SID_MysticBoost] = {
        .desc = MSG_SKILL_MysticBoost,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_GuardBearing) && COMMON_SKILL_VALID(SID_GuardBearing))
    [SID_GuardBearing] = {
        .name = MSG_SKILL_GuardBearing_NAME,
        .desc = MSG_SKILL_GuardBearing,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Spurn) && COMMON_SKILL_VALID(SID_Spurn))
    [SID_Spurn] = {
        .desc = MSG_SKILL_Spurn,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_YuneWhispers) && COMMON_SKILL_VALID(SID_YuneWhispers))
    [SID_YuneWhispers] = {
        .name = MSG_SKILL_YuneWhispers_NAME,
        .desc = MSG_SKILL_YuneWhispers,
        .icon = GFX_SkillIcon_YuneWhispers,
    },
#endif

#if (defined(SID_Plus) && COMMON_SKILL_VALID(SID_Plus))
    [SID_Plus] = {
        .desc = MSG_SKILL_Plus,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

  #if (defined(SID_Minus) && COMMON_SKILL_VALID(SID_Minus))
    [SID_Minus] = {
        .desc = MSG_SKILL_Minus,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Chlorophyll) && COMMON_SKILL_VALID(SID_Chlorophyll))
    [SID_Chlorophyll] = {
        .desc = MSG_SKILL_Chlorophyll,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_SlushRush) && COMMON_SKILL_VALID(SID_SlushRush))
    [SID_SlushRush] = {
        .desc = MSG_SKILL_SlushRush,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_SandRush) && COMMON_SKILL_VALID(SID_SandRush))
    [SID_SandRush] = {
        .desc = MSG_SKILL_SandRush,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_SwiftSwim) && COMMON_SKILL_VALID(SID_SwiftSwim))
    [SID_SwiftSwim] = {
        .desc = MSG_SKILL_SwiftSwim,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_RainDish) && COMMON_SKILL_VALID(SID_RainDish))
    [SID_RainDish] = {
        .desc = MSG_SKILL_RainDish,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Assist) && COMMON_SKILL_VALID(SID_Assist))
    [SID_Assist] = {
        .desc = MSG_SKILL_Assist,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Synergism) && COMMON_SKILL_VALID(SID_Synergism))
    [SID_Synergism] = {
        .desc = MSG_SKILL_Synergism,
        .icon = GFX_SkillIcon_WIP,
    },
#endif
};
