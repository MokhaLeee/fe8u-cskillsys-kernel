#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "constants/gfx.h"

const struct SkillInfo gSkillInfos[0x100] = {
#if (defined(SID_HpBonus) && (SID_HpBonus < MAX_SKILL_NUM))
    [SID_HpBonus] = {
        .desc = MSG_SKILL_HpBonus,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_StrBonus) && (SID_StrBonus < MAX_SKILL_NUM))
    [SID_StrBonus] = {
        .desc = MSG_SKILL_StrBonus,
        .icon = GFX_SkillIcon_StrBonus,
    },
#endif

#if (defined(SID_MagBonus) && (SID_MagBonus < MAX_SKILL_NUM))
    [SID_MagBonus] = {
        .desc = MSG_SKILL_MagBonus,
        .icon = GFX_SkillIcon_MagBonus,
    },
#endif

#if (defined(SID_SklBonus) && (SID_SklBonus < MAX_SKILL_NUM))
    [SID_SklBonus] = {
        .desc = MSG_SKILL_SklBonus,
        .icon = GFX_SkillIcon_SklBonus,
    },
#endif

#if (defined(SID_SpdBonus) && (SID_SpdBonus < MAX_SKILL_NUM))
    [SID_SpdBonus] = {
        .desc = MSG_SKILL_SpdBonus,
        .icon = GFX_SkillIcon_SpdBonus,
    },
#endif

#if (defined(SID_LckBonus) && (SID_LckBonus < MAX_SKILL_NUM))
    [SID_LckBonus] = {
        .desc = MSG_SKILL_LukBonus,
        .icon = GFX_SkillIcon_LukBonus,
    },
#endif

#if (defined(SID_DefBonus) && (SID_DefBonus < MAX_SKILL_NUM))
    [SID_DefBonus] = {
        .desc = MSG_SKILL_DefBonus,
        .icon = GFX_SkillIcon_DefBonus,
    },
#endif

#if (defined(SID_ResBonus) && (SID_ResBonus < MAX_SKILL_NUM))
    [SID_ResBonus] = {
        .desc = MSG_SKILL_ResBonus,
        .icon = GFX_SkillIcon_ResBonus,
    },
#endif

#if (defined(SID_MovBonus) && (SID_MovBonus < MAX_SKILL_NUM))
    [SID_MovBonus] = {
        .desc = MSG_SKILL_MovBonus,
        .icon = GFX_SkillIcon_MovBonus,
    },
#endif

#if (defined(SID_DefiantStr) && (SID_DefiantStr < MAX_SKILL_NUM))
    [SID_DefiantStr] = {
        .desc = MSG_SKILL_DefiantStr,
        .icon = GFX_SkillIcon_DefiantStr,
    },
#endif

#if (defined(SID_DefiantMag) && (SID_DefiantMag < MAX_SKILL_NUM))
    [SID_DefiantMag] = {
        .desc = MSG_SKILL_DefiantMag,
        .icon = GFX_SkillIcon_DefiantMag,
    },
#endif

#if (defined(SID_DefiantSkl) && (SID_DefiantSkl < MAX_SKILL_NUM))
    [SID_DefiantSkl] = {
        .desc = MSG_SKILL_DefiantSkl,
        .icon = GFX_SkillIcon_DefiantSkl,
    },
#endif

#if (defined(SID_DefiantSpd) && (SID_DefiantSpd < MAX_SKILL_NUM))
    [SID_DefiantSpd] = {
        .desc = MSG_SKILL_DefiantSpd,
        .icon = GFX_SkillIcon_DefiantSpd,
    },
#endif

#if (defined(SID_DefiantLck) && (SID_DefiantLck < MAX_SKILL_NUM))
    [SID_DefiantLck] = {
        .desc = MSG_SKILL_DefiantLck,
        .icon = GFX_SkillIcon_DefiantLck,
    },
#endif

#if (defined(SID_DefiantDef) && (SID_DefiantDef < MAX_SKILL_NUM))
    [SID_DefiantDef] = {
        .desc = MSG_SKILL_DefiantDef,
        .icon = GFX_SkillIcon_DefiantDef,
    },
#endif

#if (defined(SID_DefiantRes) && (SID_DefiantRes < MAX_SKILL_NUM))
    [SID_DefiantRes] = {
        .desc = MSG_SKILL_DefiantRes,
        .icon = GFX_SkillIcon_DefiantRes,
    },
#endif

#if (defined(SID_DefiantCrit) && (SID_DefiantCrit < MAX_SKILL_NUM))
    [SID_DefiantCrit] = {
        .desc = MSG_SKILL_DefiantCrit,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_DefiantAvoid) && (SID_DefiantAvoid < MAX_SKILL_NUM))
    [SID_DefiantAvoid] = {
        .desc = MSG_SKILL_DefiantAvoid,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Fury) && (SID_Fury < MAX_SKILL_NUM))
    [SID_Fury] = {
        .desc = MSG_SKILL_Fury,
        .icon = GFX_SkillIcon_Fury,
    },
#endif

#if (defined(SID_FuryPlus) && (SID_FuryPlus < MAX_SKILL_NUM))
    [SID_FuryPlus] = {
        .desc = MSG_SKILL_FuryPlus,
        .icon = GFX_SkillIcon_FuryPlus,
    },
#endif

#if (defined(SID_FortressDef) && (SID_FortressDef < MAX_SKILL_NUM))
    [SID_FortressDef] = {
        .desc = MSG_SKILL_FortressDef,
        .icon = GFX_SkillIcon_FortressDef,
    },
#endif

#if (defined(SID_FortressRes) && (SID_FortressRes < MAX_SKILL_NUM))
    [SID_FortressRes] = {
        .desc = MSG_SKILL_FortressRes,
        .icon = GFX_SkillIcon_FortressRes,
    },
#endif

#if (defined(SID_LifeAndDeath) && (SID_LifeAndDeath < MAX_SKILL_NUM))
    [SID_LifeAndDeath] = {
        .desc = MSG_SKILL_LifeAndDeath,
        .icon = GFX_SkillIcon_LifeAndDeath,
    },
#endif

#if (defined(SID_LuckySeven) && (SID_LuckySeven < MAX_SKILL_NUM))
    [SID_LuckySeven] = {
        .desc = MSG_SKILL_LuckySeven,
        .icon = GFX_SkillIcon_LuckySeven,
    },
#endif

#if (defined(SID_Nullify) && (SID_Nullify < MAX_SKILL_NUM))
    [SID_Nullify] = {
        .desc = MSG_SKILL_Nullify,
        .icon = GFX_SkillIcon_Nullify,
    },
#endif

#if (defined(SID_Slayer) && (SID_Slayer < MAX_SKILL_NUM))
    [SID_Slayer] = {
        .desc = MSG_SKILL_Slayer,
        .icon = GFX_SkillIcon_Slayer,
    },
#endif

#if (defined(SID_Skybreaker) && (SID_Skybreaker < MAX_SKILL_NUM))
    [SID_Skybreaker] = {
        .name = MSG_SKILL_Skybreaker_NAME,
        .desc = MSG_SKILL_Skybreaker,
        .icon = GFX_SkillIcon_Skybreaker,
    },
#endif

#if (defined(SID_Resourceful) && (SID_Resourceful < MAX_SKILL_NUM))
    [SID_Resourceful] = {
        .name = MSG_SKILL_Resourceful_NAME,
        .desc = MSG_SKILL_Resourceful,
        .icon = GFX_SkillIcon_Resourceful,
    },
#endif

#if (defined(SID_Lethality) && (SID_Lethality < MAX_SKILL_NUM))
    [SID_Lethality] = {
        .desc = MSG_SKILL_Lethality,
        .icon = GFX_SkillIcon_Lethality,
    },
#endif

#if (defined(SID_Crit) && (SID_Crit < MAX_SKILL_NUM))
    [SID_Crit] = {
        .desc = MSG_SKILL_Crit,
        .icon = GFX_SkillIcon_Crit,
    },
#endif

#if (defined(SID_WatchfulEye) && (SID_WatchfulEye < MAX_SKILL_NUM))
    [SID_WatchfulEye] = {
        .desc = MSG_SKILL_WatchfulEye,
        .icon = GFX_SkillIcon_WatchfulEye,
    },
#endif

#if (defined(SID_CritSword) && (SID_CritSword < MAX_SKILL_NUM))
    [SID_CritSword] = {
        .desc = MSG_SKILL_CritSword,
        .icon = GFX_SkillIcon_CritSword,
    },
#endif

#if (defined(SID_CritAxe) && (SID_CritAxe < MAX_SKILL_NUM))
    [SID_CritAxe] = {
        .desc = MSG_SKILL_CritAxe,
        .icon = GFX_SkillIcon_CritAxe,
    },
#endif

#if (defined(SID_CritLance) && (SID_CritLance < MAX_SKILL_NUM))
    [SID_CritLance] = {
        .desc = MSG_SKILL_CritLance,
        .icon = GFX_SkillIcon_CritLance,
    },
#endif

#if (defined(SID_CritBow) && (SID_CritBow < MAX_SKILL_NUM))
    [SID_CritBow] = {
        .desc = MSG_SKILL_CritBow,
        .icon = GFX_SkillIcon_CritBow,
    },
#endif

#if (defined(SID_FaireSword) && (SID_FaireSword < MAX_SKILL_NUM))
    [SID_FaireSword] = {
        .desc = MSG_SKILL_FaireSword,
        .icon = GFX_SkillIcon_FaireSword,
    },
#endif

#if (defined(SID_FaireLance) && (SID_FaireLance < MAX_SKILL_NUM))
    [SID_FaireLance] = {
        .desc = MSG_SKILL_FaireLance,
        .icon = GFX_SkillIcon_FaireLance,
    },
#endif

#if (defined(SID_FaireAxe) && (SID_FaireAxe < MAX_SKILL_NUM))
    [SID_FaireAxe] = {
        .desc = MSG_SKILL_FaireAxe,
        .icon = GFX_SkillIcon_FaireAxe,
    },
#endif

#if (defined(SID_FaireBow) && (SID_FaireBow < MAX_SKILL_NUM))
    [SID_FaireBow] = {
        .desc = MSG_SKILL_FaireBow,
        .icon = GFX_SkillIcon_FaireBow,
    },
#endif

#if (defined(SID_FaireBMag) && (SID_FaireBMag < MAX_SKILL_NUM))
    [SID_FaireBMag] = {
        .desc = MSG_SKILL_FaireBMag,
        .icon = GFX_SkillIcon_FaireBMag,
    },
#endif

#if (defined(SID_Avoid) && (SID_Avoid < MAX_SKILL_NUM))
    [SID_Avoid] = {
        .desc = MSG_SKILL_Avoid,
        .icon = GFX_SkillIcon_Avoid,
    },
#endif

#if (defined(SID_AvoidSword) && (SID_AvoidSword < MAX_SKILL_NUM))
    [SID_AvoidSword] = {
        .desc = MSG_SKILL_AvoidSword,
        .icon = GFX_SkillIcon_AvoidSword,
    },
#endif

#if (defined(SID_RuinedBlade) && (SID_RuinedBlade < MAX_SKILL_NUM))
    [SID_RuinedBlade] = {
        .desc = MSG_SKILL_RuinedBlade,
        .icon = GFX_SkillIcon_RuinedBlade,
    },
#endif

#if (defined(SID_RuinedBladePlus) && (SID_RuinedBladePlus < MAX_SKILL_NUM))
    [SID_RuinedBladePlus] = {
        .desc = MSG_SKILL_RuinedBladePlus,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_InfinityEdge) && (SID_InfinityEdge < MAX_SKILL_NUM))
    [SID_InfinityEdge] = {
        .desc = MSG_SKILL_InfinityEdge,
        .icon = GFX_SkillIcon_InfinityEdge,
    },
#endif

#if (defined(SID_HeavyBlade) && (SID_HeavyBlade < MAX_SKILL_NUM))
    [SID_HeavyBlade] = {
        .desc = MSG_SKILL_HeavyBlade,
        .icon = GFX_SkillIcon_HeavyBlade,
    },
#endif

#if (defined(SID_FlashingBlade) && (SID_FlashingBlade < MAX_SKILL_NUM))
    [SID_FlashingBlade] = {
        .desc = MSG_SKILL_FlashingBlade,
        .icon = GFX_SkillIcon_FlashingBlade,
    },
#endif

#if (defined(SID_HeavyBladePlus) && (SID_HeavyBladePlus < MAX_SKILL_NUM))
    [SID_HeavyBladePlus] = {
        .desc = MSG_SKILL_HeavyBladePlus,
        .icon = GFX_SkillIcon_HeavyBladePlus,
    },
#endif

#if (defined(SID_FlashingBladePlus) && (SID_FlashingBladePlus < MAX_SKILL_NUM))
    [SID_FlashingBladePlus] = {
        .desc = MSG_SKILL_FlashingBladePlus,
        .icon = GFX_SkillIcon_FlashingBladePlus,
    },
#endif

#if (defined(SID_LunaAttack) && (SID_LunaAttack < MAX_SKILL_NUM))
    [SID_LunaAttack] = {
        .desc = MSG_SKILL_LunaAttack,
        .icon = GFX_SkillIcon_LunaAttack,
    },
#endif

#if (defined(SID_SorceryBlade) && (SID_SorceryBlade < MAX_SKILL_NUM))
    [SID_SorceryBlade] = {
        .desc = MSG_SKILL_SorceryBlade,
        .icon = GFX_SkillIcon_SorceryBlade,
    },
#endif

#if (defined(SID_Fortune) && (SID_Fortune < MAX_SKILL_NUM))
    [SID_Fortune] = {
        .desc = MSG_SKILL_Fortune,
        .icon = GFX_SkillIcon_Fortune,
    },
#endif

#if (defined(SID_BlueFlame) && (SID_BlueFlame < MAX_SKILL_NUM))
    [SID_BlueFlame] = {
        .desc = MSG_SKILL_BlueFlame,
        .icon = GFX_SkillIcon_BlueFlame,
    },
#endif

#if (defined(SID_SwordBreaker) && (SID_SwordBreaker < MAX_SKILL_NUM))
    [SID_SwordBreaker] = {
        .desc = MSG_SKILL_SwordBreaker,
        .icon = GFX_SkillIcon_SwordBreaker,
    },
#endif

#if (defined(SID_AxeBreaker) && (SID_AxeBreaker < MAX_SKILL_NUM))
    [SID_AxeBreaker] = {
        .desc = MSG_SKILL_AxeBreaker,
        .icon = GFX_SkillIcon_AxeBreaker,
    },
#endif

#if (defined(SID_LanceBreaker) && (SID_LanceBreaker < MAX_SKILL_NUM))
    [SID_LanceBreaker] = {
        .desc = MSG_SKILL_LanceBreaker,
        .icon = GFX_SkillIcon_LanceBreaker,
    },
#endif

#if (defined(SID_BowBreaker) && (SID_BowBreaker < MAX_SKILL_NUM))
    [SID_BowBreaker] = {
        .desc = MSG_SKILL_BowBreaker,
        .icon = GFX_SkillIcon_BowBreaker,
    },
#endif

#if (defined(SID_BMagBreaker) && (SID_BMagBreaker < MAX_SKILL_NUM))
    [SID_BMagBreaker] = {
        .desc = MSG_SKILL_BMagBreaker,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Vantage) && (SID_Vantage < MAX_SKILL_NUM))
    [SID_Vantage] = {
        .desc = MSG_SKILL_Vantage,
        .icon = GFX_SkillIcon_Vantage,
    },
#endif

#if (defined(SID_Desperation) && (SID_Desperation < MAX_SKILL_NUM))
    [SID_Desperation] = {
        .desc = MSG_SKILL_Desperation,
        .icon = GFX_SkillIcon_Desperation,
    },
#endif

#if (defined(SID_QuickRiposte) && (SID_QuickRiposte < MAX_SKILL_NUM))
    [SID_QuickRiposte] = {
        .name = MSA_SKILL_QuickRiposte_NAME,
        .desc = MSG_SKILL_QuickRiposte,
        .icon = GFX_SkillIcon_QuickRiposte,
    },
#endif

#if (defined(SID_WaryFighter) && (SID_WaryFighter < MAX_SKILL_NUM))
    [SID_WaryFighter] = {
        .desc = MSG_SKILL_WaryFighter,
        .icon = GFX_SkillIcon_WaryFighter,
    },
#endif

#if (defined(SID_DoubleLion) && (SID_DoubleLion < MAX_SKILL_NUM))
    [SID_DoubleLion] = {
        .name = MSG_SKILL_DoubleLion_NAME,
        .desc = MSG_SKILL_DoubleLion,
        .icon = GFX_SkillIcon_DoubleLion,
    },
#endif

#if (defined(SID_Adept) && (SID_Adept < MAX_SKILL_NUM))
    [SID_Adept] = {
        .desc = MSG_SKILL_Adept,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Inori) && (SID_Inori < MAX_SKILL_NUM))
    [SID_Inori] = {
        .desc = MSG_SKILL_Inori,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Astra) && (SID_Astra < MAX_SKILL_NUM))
    [SID_Astra] = {
        .desc = MSG_SKILL_Astra,
        .icon = GFX_SkillIcon_Astra,
    },
#endif

#if (defined(SID_Aegis) && (SID_Aegis < MAX_SKILL_NUM))
    [SID_Aegis] = {
        .desc = MSG_SKILL_Aegis,
        .icon = GFX_SkillIcon_Aegis,
    },
#endif

#if (defined(SID_Pavise) && (SID_Pavise < MAX_SKILL_NUM))
    [SID_Pavise] = {
        .desc = MSG_SKILL_Pavise,
        .icon = GFX_SkillIcon_Pavise,
    },
#endif

#if (defined(SID_Bane) && (SID_Bane < MAX_SKILL_NUM))
    [SID_Bane] = {
        .desc = MSG_SKILL_Bane,
        .icon = GFX_SkillIcon_Bane,
    },
#endif

#if (defined(SID_Aether) && (SID_Aether < MAX_SKILL_NUM))
    [SID_Aether] = {
        .desc = MSG_SKILL_Aether,
        .icon = GFX_SkillIcon_Aether,
    },
#endif

#if (defined(SID_Corona) && (SID_Corona < MAX_SKILL_NUM))
    [SID_Corona] = {
        .desc = MSG_SKILL_Corona,
        .icon = GFX_SkillIcon_Corona,
    },
#endif

#if (defined(SID_Luna) && (SID_Luna < MAX_SKILL_NUM))
    [SID_Luna] = {
        .desc = MSG_SKILL_Luna,
        .icon = GFX_SkillIcon_Luna,
    },
#endif

#if (defined(SID_Flare) && (SID_Flare < MAX_SKILL_NUM))
    [SID_Flare] = {
        .desc = MSG_SKILL_Flare,
        .icon = GFX_SkillIcon_Flare,
    },
#endif

#if (defined(SID_Sol) && (SID_Sol < MAX_SKILL_NUM))
    [SID_Sol] = {
        .desc = MSG_SKILL_Sol,
        .icon = GFX_SkillIcon_Sol,
    },
#endif

#if (defined(SID_DragonFang) && (SID_DragonFang < MAX_SKILL_NUM))
    [SID_DragonFang] = {
        .desc = MSG_SKILL_DragonFang,
        .icon = GFX_SkillIcon_DragonFang,
    },
#endif

#if (defined(SID_Foresight) && (SID_Foresight < MAX_SKILL_NUM))
    [SID_Foresight] = {
        .desc = MSG_SKILL_Foresight,
        .icon = GFX_SkillIcon_Foresight,
    },
#endif

#if (defined(SID_BlowDarting) && (SID_BlowDarting < MAX_SKILL_NUM))
    [SID_BlowDarting] = {
        .desc = MSG_SKILL_BlowDarting,
        .icon = GFX_SkillIcon_BlowDarting,
    },
#endif

#if (defined(SID_BlowDeath) && (SID_BlowDeath < MAX_SKILL_NUM))
    [SID_BlowDeath] = {
        .desc = MSG_SKILL_BlowDeath,
        .icon = GFX_SkillIcon_BlowDeath,
    },
#endif

#if (defined(SID_BlowArmored) && (SID_BlowArmored < MAX_SKILL_NUM))
    [SID_BlowArmored] = {
        .desc = MSG_SKILL_BlowArmored,
        .icon = GFX_SkillIcon_BlowArmored,
    },
#endif

#if (defined(SID_BlowFiendish) && (SID_BlowFiendish < MAX_SKILL_NUM))
    [SID_BlowFiendish] = {
        .desc = MSG_SKILL_BlowFiendish,
        .icon = GFX_SkillIcon_BlowFiendish,
    },
#endif

#if (defined(SID_BlowWarding) && (SID_BlowWarding < MAX_SKILL_NUM))
    [SID_BlowWarding] = {
        .desc = MSG_SKILL_BlowWarding,
        .icon = GFX_SkillIcon_BlowWarding,
    },
#endif

#if (defined(SID_BlowDuelist) && (SID_BlowDuelist < MAX_SKILL_NUM))
    [SID_BlowDuelist] = {
        .desc = MSG_SKILL_BlowDuelist,
        .icon = GFX_SkillIcon_BlowDuelist,
    },
#endif

#if (defined(SID_BlowUncanny) && (SID_BlowUncanny < MAX_SKILL_NUM))
    [SID_BlowUncanny] = {
        .desc = MSG_SKILL_BlowUncanny,
        .icon = GFX_SkillIcon_BlowUncanny,
    },
#endif

#if (defined(SID_StanceBracing) && (SID_StanceBracing < MAX_SKILL_NUM))
    [SID_StanceBracing] = {
        .desc = MSG_SKILL_StanceBracing,
        .icon = GFX_SkillIcon_StanceBracing,
    },
#endif

#if (defined(SID_StanceDarting) && (SID_StanceDarting < MAX_SKILL_NUM))
    [SID_StanceDarting] = {
        .desc = MSG_SKILL_StanceDarting,
        .icon = GFX_SkillIcon_StanceDarting,
    },
#endif

#if (defined(SID_StanceFierce) && (SID_StanceFierce < MAX_SKILL_NUM))
    [SID_StanceFierce] = {
        .desc = MSG_SKILL_StanceFierce,
        .icon = GFX_SkillIcon_StanceFierce,
    },
#endif

#if (defined(SID_StanceKestrel) && (SID_StanceKestrel < MAX_SKILL_NUM))
    [SID_StanceKestrel] = {
        .desc = MSG_SKILL_StanceKestrel,
        .icon = GFX_SkillIcon_StanceKestrel,
    },
#endif

#if (defined(SID_StanceMirror) && (SID_StanceMirror < MAX_SKILL_NUM))
    [SID_StanceMirror] = {
        .desc = MSG_SKILL_StanceMirror,
        .icon = GFX_SkillIcon_StanceMirror,
    },
#endif

#if (defined(SID_StanceReady) && (SID_StanceReady < MAX_SKILL_NUM))
    [SID_StanceReady] = {
        .desc = MSG_SKILL_StanceReady,
        .icon = GFX_SkillIcon_StanceReady,
    },
#endif

#if (defined(SID_StanceSteady) && (SID_StanceSteady < MAX_SKILL_NUM))
    [SID_StanceSteady] = {
        .desc = MSG_SKILL_StanceSteady,
        .icon = GFX_SkillIcon_StanceSteady,
    },
#endif

#if (defined(SID_StanceSturdy) && (SID_StanceSturdy < MAX_SKILL_NUM))
    [SID_StanceSturdy] = {
        .desc = MSG_SKILL_StanceSturdy,
        .icon = GFX_SkillIcon_StanceSturdy,
    },
#endif

#if (defined(SID_StanceSwift) && (SID_StanceSwift < MAX_SKILL_NUM))
    [SID_StanceSwift] = {
        .desc = MSG_SKILL_StanceSwift,
        .icon = GFX_SkillIcon_StanceSwift,
    },
#endif

#if (defined(SID_StanceWarding) && (SID_StanceWarding < MAX_SKILL_NUM))
    [SID_StanceWarding] = {
        .desc = MSG_SKILL_StanceWarding,
        .icon = GFX_SkillIcon_StanceWarding,
    },
#endif

#if (defined(SID_RangeBonusBMag1) && (SID_RangeBonusBMag1 < MAX_SKILL_NUM))
    [SID_RangeBonusBMag1] = {
        .desc = MSG_SKILL_RangeBonusBMag1,
        .icon = GFX_SkillIcon_RangeBonusBMag1,
    },
#endif

#if (defined(SID_RangeBonusBMag2) && (SID_RangeBonusBMag2 < MAX_SKILL_NUM))
    [SID_RangeBonusBMag2] = {
        .desc = MSG_SKILL_RangeBonusBMag2,
        .icon = GFX_SkillIcon_RangeBonusBMag2,
    },
#endif

#if (defined(SID_RangeBonusBow1) && (SID_RangeBonusBow1 < MAX_SKILL_NUM))
    [SID_RangeBonusBow1] = {
        .desc = MSG_SKILL_RangeBonusBow1,
        .icon = GFX_SkillIcon_RangeBonusBow1,
    },
#endif

#if (defined(SID_RangeBonusBow2) && (SID_RangeBonusBow2 < MAX_SKILL_NUM))
    [SID_RangeBonusBow2] = {
        .desc = MSG_SKILL_RangeBonusBow2,
        .icon = GFX_SkillIcon_RangeBonusBow2,
    },
#endif

#if (defined(SID_RunningStart) && (SID_RunningStart < MAX_SKILL_NUM))
    [SID_RunningStart] = {
        .name = MSG_SKILL_RunningStart_NAME,
        .desc = MSG_SKILL_RunningStart,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Canto) && (SID_Canto < MAX_SKILL_NUM))
    [SID_Canto] = {
        .desc = MSG_SKILL_Canto,
        .icon = GFX_SkillIcon_Canto,
    },
#endif

#if (defined(SID_CantoPlus) && (SID_CantoPlus < MAX_SKILL_NUM))
    [SID_CantoPlus] = {
        .desc = MSG_SKILL_CantoPlus,
        .icon = GFX_SkillIcon_CantoPlus,
    },
#endif

#if (defined(SID_AlertStance) && (SID_AlertStance < MAX_SKILL_NUM))
    [SID_AlertStance] = {
        .desc = MSG_SKILL_AlertStance,
        .icon = GFX_SkillIcon_AlertStance,
    },
#endif

#if (defined(SID_AlertStancePlus) && (SID_AlertStancePlus < MAX_SKILL_NUM))
    [SID_AlertStancePlus] = {
        .desc = MSG_SKILL_AlertStancePlus,
        .icon = GFX_SkillIcon_AlertStancePlus,
    },
#endif

#if (defined(SID_Galeforce) && (SID_Galeforce < MAX_SKILL_NUM))
    [SID_Galeforce] = {
        .desc = MSG_SKILL_Galeforce,
        .icon = GFX_SkillIcon_Galeforce,
    },
#endif

#if (defined(SID_SavageBlow) && (SID_SavageBlow < MAX_SKILL_NUM))
    [SID_SavageBlow] = {
        .desc = MSG_SKILL_SavageBlow,
        .icon = GFX_SkillIcon_SavageBlow,
    },
#endif

#if (defined(SID_BreathOfLife) && (SID_BreathOfLife < MAX_SKILL_NUM))
    [SID_BreathOfLife] = {
        .desc = MSG_SKILL_BreathOfLife,
        .icon = GFX_SkillIcon_BreathOfLife,
    },
#endif

#if (defined(SID_Thunderstorm) && (SID_Thunderstorm < MAX_SKILL_NUM))
    [SID_Thunderstorm] = {
        .desc = MSG_SKILL_Thunderstorm,
        .icon = GFX_SkillIcon_Thunderstorm,
    },
#endif

#if (defined(SID_PosReturn) && (SID_PosReturn < MAX_SKILL_NUM))
    [SID_PosReturn] = {
        .desc = MSG_SKILL_PosReturn,
        .icon = GFX_SkillIcon_PosReturn,
    },
#endif

#if (defined(SID_PosSwap) && (SID_PosSwap < MAX_SKILL_NUM))
    [SID_PosSwap] = {
        .desc = MSG_SKILL_PosSwap,
        .icon = GFX_SkillIcon_PosSwap,
    },
#endif

#if (defined(SID_Renewal) && (SID_Renewal < MAX_SKILL_NUM))
    [SID_Renewal] = {
        .desc = MSG_SKILL_Renewal,
        .icon = GFX_SkillIcon_Renewal,
    },
#endif

#if (defined(SID_Anathema) && (SID_Anathema < MAX_SKILL_NUM))
    [SID_Anathema] = {
        .desc = MSG_SKILL_Anathema,
        .icon = GFX_SkillIcon_Anathema,
    },
#endif

#if (defined(SID_Bond) && (SID_Bond < MAX_SKILL_NUM))
    [SID_Bond] = {
        .desc = MSG_SKILL_Bond,
        .icon = GFX_SkillIcon_Bond,
    },
#endif

#if (defined(SID_Charm) && (SID_Charm < MAX_SKILL_NUM))
    [SID_Charm] = {
        .desc = MSG_SKILL_Charm,
        .icon = GFX_SkillIcon_Charm,
    },
#endif

#if (defined(SID_Intimidate) && (SID_Intimidate < MAX_SKILL_NUM))
    [SID_Intimidate] = {
        .desc = MSG_SKILL_Intimidate,
        .icon = GFX_SkillIcon_Intimidate,
    },
#endif

#if (defined(SID_Tantivy) && (SID_Tantivy < MAX_SKILL_NUM))
    [SID_Tantivy] = {
        .desc = MSG_SKILL_Tantivy,
        .icon = GFX_SkillIcon_Tantivy,
    },
#endif

#if (defined(SID_Focus) && (SID_Focus < MAX_SKILL_NUM))
    [SID_Focus] = {
        .desc = MSG_SKILL_Focus,
        .icon = GFX_SkillIcon_Focus,
    },
#endif

#if (defined(SID_Hex) && (SID_Hex < MAX_SKILL_NUM))
    [SID_Hex] = {
        .desc = MSG_SKILL_Hex,
        .icon = GFX_SkillIcon_Hex,
    },
#endif

#if (defined(SID_Infiltrator) && (SID_Infiltrator < MAX_SKILL_NUM))
    [SID_Infiltrator] = {
        .desc = MSG_SKILL_Infiltrator,
        .icon = GFX_SkillIcon_Infiltrator,
    },
#endif

#if (defined(SID_Inspiration) && (SID_Inspiration < MAX_SKILL_NUM))
    [SID_Inspiration] = {
        .desc = MSG_SKILL_Inspiration,
        .icon = GFX_SkillIcon_Inspiration,
    },
#endif

#if (defined(SID_DivinelyInspiring) && (SID_DivinelyInspiring < MAX_SKILL_NUM))
    [SID_DivinelyInspiring] = {
        .desc = MSG_SKILL_DivinelyInspiring,
        .icon = GFX_SkillIcon_DivinelyInspiring,
    },
#endif

#if (defined(SID_VoiceOfPeace) && (SID_VoiceOfPeace < MAX_SKILL_NUM))
    [SID_VoiceOfPeace] = {
        .desc = MSG_SKILL_VoiceOfPeace,
        .icon = GFX_SkillIcon_VoiceOfPeace,
    },
#endif

#if (defined(SID_BattleRange_Todo1) && (SID_BattleRange_Todo1 < MAX_SKILL_NUM))
    [SID_BattleRange_Todo1] = {
        .desc = MSG_SKILL_BattleRange1,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_BattleRange_Todo2) && (SID_BattleRange_Todo2 < MAX_SKILL_NUM))
    [SID_BattleRange_Todo2] = {
        .desc = MSG_SKILL_BattleRange2,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_BattleRange_Todo3) && (SID_BattleRange_Todo3 < MAX_SKILL_NUM))
    [SID_BattleRange_Todo3] = {
        .desc = MSG_SKILL_BattleRange3,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_Shade) && (SID_Shade < MAX_SKILL_NUM))
    [SID_Shade] = {
        .desc = MSG_SKILL_Shade,
        .icon = GFX_SkillIcon_Shade,
    },
#endif

#if (defined(SID_ShadePlus) && (SID_ShadePlus < MAX_SKILL_NUM))
    [SID_ShadePlus] = {
        .desc = MSG_SKILL_ShadePlus,
        .icon = GFX_SkillIcon_ShadePlus,
    },
#endif

#if (defined(SID_Provoke) && (SID_Provoke < MAX_SKILL_NUM))
    [SID_Provoke] = {
        .desc = MSG_SKILL_Provoke,
        .icon = GFX_SkillIcon_Provoke,
    },
#endif

#if (defined(SID_Dance) && (SID_Dance < MAX_SKILL_NUM))
    [SID_Dance] = {
        .desc = MSG_SKILL_Dance,
        .icon = GFX_SkillIcon_Dance,
    },
#endif

#if (defined(SID_LockTouch) && (SID_LockTouch < MAX_SKILL_NUM))
    [SID_LockTouch] = {
        .desc = MSG_SKILL_LockTouch,
        .icon = GFX_SkillIcon_LockTouch,
    },
#endif

#if (defined(SID_Summon) && (SID_Summon < MAX_SKILL_NUM))
    [SID_Summon] = {
        .desc = MSG_SKILL_Summon,
        .icon = GFX_SkillIcon_Summon,
    },
#endif

#if (defined(SID_Supply) && (SID_Supply < MAX_SKILL_NUM))
    [SID_Supply] = {
        .desc = MSG_SKILL_Supply,
        .icon = GFX_SkillIcon_Supply,
    },
#endif

#if (defined(SID_Steal) && (SID_Steal < MAX_SKILL_NUM))
    [SID_Steal] = {
        .desc = MSG_SKILL_Steal,
        .icon = GFX_SkillIcon_Steal,
    },
#endif

#if (defined(SID_Blossom) && (SID_Blossom < MAX_SKILL_NUM))
    [SID_Blossom] = {
        .desc = MSG_SKILL_Blossom,
        .icon = GFX_SkillIcon_Blossom,
    },
#endif

#if (defined(SID_Paragon) && (SID_Paragon < MAX_SKILL_NUM))
    [SID_Paragon] = {
        .desc = MSG_SKILL_Paragon,
        .icon = GFX_SkillIcon_Paragon,
    },
#endif

#if (defined(SID_VoidCurse) && (SID_VoidCurse < MAX_SKILL_NUM))
    [SID_VoidCurse] = {
        .desc = MSG_SKILL_VoidCurse,
        .icon = GFX_SkillIcon_VoidCurse,
    },
#endif

#if (defined(SID_Aptitude) && (SID_Aptitude < MAX_SKILL_NUM))
    [SID_Aptitude] = {
        .desc = MSG_SKILL_Aptitude,
        .icon = GFX_SkillIcon_Aptitude,
    },
#endif

#if (defined(SID_LEGEND_InoriAtk) && (SID_LEGEND_InoriAtk < MAX_SKILL_NUM))
    [SID_LEGEND_InoriAtk] = {
        .desc = MSG_SKILL_LEGEND_InoriAtk,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_InoriAvo) && (SID_LEGEND_InoriAvo < MAX_SKILL_NUM))
    [SID_LEGEND_InoriAvo] = {
        .desc = MSG_SKILL_LEGEND_InoriAvo,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_InoriDef) && (SID_LEGEND_InoriDef < MAX_SKILL_NUM))
    [SID_LEGEND_InoriDef] = {
        .desc = MSG_SKILL_LEGEND_InoriDef,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_4) && (SID_LEGEND_4 < MAX_SKILL_NUM))
    [SID_LEGEND_4] = {
        .desc = MSG_WIP,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_LEGEND_5) && (SID_LEGEND_5 < MAX_SKILL_NUM))
    [SID_LEGEND_5] = {
        .desc = MSG_WIP,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (defined(SID_COMBAT_Grounder) && (SID_COMBAT_Grounder < MAX_SKILL_NUM))
    [SID_COMBAT_Grounder] = {
        .desc = MSG_SKILL_COMBAT_Grounder,
        .icon = GFX_CombatArtIcon_SwordArtAtk,
    },
#endif

#if (defined(SID_COMBAT_BaneOfMonsters) && (SID_COMBAT_BaneOfMonsters < MAX_SKILL_NUM))
    [SID_COMBAT_BaneOfMonsters] = {
        .desc = MSG_SKILL_COMBAT_BaneOfMonsters,
        .icon = GFX_CombatArtIcon_SwordArtAtk,
    },
#endif

#if (defined(SID_COMBAT_Hexblade) && (SID_COMBAT_Hexblade < MAX_SKILL_NUM))
    [SID_COMBAT_Hexblade] = {
        .desc = MSG_SKILL_COMBAT_Hexblade,
        .icon = GFX_CombatArtIcon_SwordArtMag,
    },
#endif

#if (defined(SID_COMBAT_FoudroyantStrike) && (SID_COMBAT_FoudroyantStrike < MAX_SKILL_NUM))
    [SID_COMBAT_FoudroyantStrike] = {
        .desc = MSG_SKILL_COMBAT_FoudroyantStrike,
        .icon = GFX_CombatArtIcon_SwordArtCrit,
    },
#endif

#if (defined(SID_COMBAT_TempestLance) && (SID_COMBAT_TempestLance < MAX_SKILL_NUM))
    [SID_COMBAT_TempestLance] = {
        .desc = MSG_SKILL_COMBAT_TempestLance,
        .icon = GFX_CombatArtIcon_LanceArtAtk,
    },
#endif

#if (defined(SID_COMBAT_SwiftStrikes) && (SID_COMBAT_SwiftStrikes < MAX_SKILL_NUM))
    [SID_COMBAT_SwiftStrikes] = {
        .desc = MSG_SKILL_COMBAT_SwiftStrikes,
        .icon = GFX_CombatArtIcon_LanceArtAtk,
    },
#endif

#if (defined(SID_COMBAT_FrozenLance) && (SID_COMBAT_FrozenLance < MAX_SKILL_NUM))
    [SID_COMBAT_FrozenLance] = {
        .desc = MSG_SKILL_COMBAT_FrozenLance,
        .icon = GFX_CombatArtIcon_LanceArtMag,
    },
#endif

#if (defined(SID_COMBAT_Vengeance) && (SID_COMBAT_Vengeance < MAX_SKILL_NUM))
    [SID_COMBAT_Vengeance] = {
        .desc = MSG_SKILL_COMBAT_Vengeance,
        .icon = GFX_CombatArtIcon_LanceArtCrit,
    },
#endif

#if (defined(SID_COMBAT_HelmSplitter) && (SID_COMBAT_HelmSplitter < MAX_SKILL_NUM))
    [SID_COMBAT_HelmSplitter] = {
        .desc = MSG_SKILL_COMBAT_HelmSplitter,
        .icon = GFX_CombatArtIcon_AxeArtAtk,
    },
#endif

#if (defined(SID_COMBAT_LightningAxe) && (SID_COMBAT_LightningAxe < MAX_SKILL_NUM))
    [SID_COMBAT_LightningAxe] = {
        .desc = MSG_SKILL_COMBAT_LightningAxe,
        .icon = GFX_CombatArtIcon_AxeArtMag,
    },
#endif

#if (defined(SID_COMBAT_WarMasterStrike) && (SID_COMBAT_WarMasterStrike < MAX_SKILL_NUM))
    [SID_COMBAT_WarMasterStrike] = {
        .desc = MSG_SKILL_COMBAT_WarMasterStrike,
        .icon = GFX_CombatArtIcon_AxeArtCrit,
    },
#endif

#if (defined(SID_COMBAT_CurvedShot) && (SID_COMBAT_CurvedShot < MAX_SKILL_NUM))
    [SID_COMBAT_CurvedShot] = {
        .desc = MSG_SKILL_COMBAT_CurvedShot,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif

#if (defined(SID_COMBAT_Deadeye) && (SID_COMBAT_Deadeye < MAX_SKILL_NUM))
    [SID_COMBAT_Deadeye] = {
        .desc = MSG_SKILL_COMBAT_Deadeye,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif

#if (defined(SID_COMBAT_Encloser) && (SID_COMBAT_Encloser < MAX_SKILL_NUM))
    [SID_COMBAT_Encloser] = {
        .desc = MSG_SKILL_COMBAT_Encloser,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_EncloserPlus) && (SID_COMBAT_EncloserPlus < MAX_SKILL_NUM))
    [SID_COMBAT_EncloserPlus] = {
        .desc = MSG_SKILL_COMBAT_EncloserPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_MonsterBlast) && (SID_COMBAT_MonsterBlast < MAX_SKILL_NUM))
    [SID_COMBAT_MonsterBlast] = {
        .desc = MSG_SKILL_COMBAT_MonsterBlast,
        .icon = GFX_CombatArtIcon_BowArtCrit,
    },
#endif

#if (defined(SID_COMBAT_SchismShot) && (SID_COMBAT_SchismShot < MAX_SKILL_NUM))
    [SID_COMBAT_SchismShot] = {
        .desc = MSG_SKILL_COMBAT_SchismShot,
        .icon = GFX_CombatArtIcon_BowArtCrit,
    },
#endif

#if (defined(SID_COMBAT_BreakShot) && (SID_COMBAT_BreakShot < MAX_SKILL_NUM))
    [SID_COMBAT_BreakShot] = {
        .desc = MSG_SKILL_COMBAT_BreakShot,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_BreakShotPlus) && (SID_COMBAT_BreakShotPlus < MAX_SKILL_NUM))
    [SID_COMBAT_BreakShotPlus] = {
        .desc = MSG_SKILL_COMBAT_BreakShotPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_WaningShot) && (SID_COMBAT_WaningShot < MAX_SKILL_NUM))
    [SID_COMBAT_WaningShot] = {
        .desc = MSG_SKILL_COMBAT_WaningShot,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_WaningShotPlus) && (SID_COMBAT_WaningShotPlus < MAX_SKILL_NUM))
    [SID_COMBAT_WaningShotPlus] = {
        .desc = MSG_SKILL_COMBAT_WaningShotPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_HuntersVolley) && (SID_COMBAT_HuntersVolley < MAX_SKILL_NUM))
    [SID_COMBAT_HuntersVolley] = {
        .desc = MSG_SKILL_COMBAT_HuntersVolley,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif

#if (defined(SID_Frenzy) && (SID_Frenzy < MAX_SKILL_NUM))
    [SID_Frenzy] = {
        .desc = MSG_SKILL_Frenzy,
        .icon = GFX_SkillIcon_Frenzy,
    },
#endif
};
