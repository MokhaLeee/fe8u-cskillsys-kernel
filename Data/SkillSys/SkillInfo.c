#include "global.h"

#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "constants/gfx.h"

const struct SkillInfo gSkillInfos[0x100] = {
#if (MAX_SKILL_NUM != SID_HpBonus)
    [SID_HpBonus] = {
        .desc = MSG_SKILL_HpBonus,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_StrBonus)
    [SID_StrBonus] = {
        .desc = MSG_SKILL_StrBonus,
        .icon = GFX_SkillIcon_StrBonus,
    },
#endif

#if (MAX_SKILL_NUM != SID_MagBonus)
    [SID_MagBonus] = {
        .desc = MSG_SKILL_MagBonus,
        .icon = GFX_SkillIcon_MagBonus,
    },
#endif

#if (MAX_SKILL_NUM != SID_SklBonus)
    [SID_SklBonus] = {
        .desc = MSG_SKILL_SklBonus,
        .icon = GFX_SkillIcon_SklBonus,
    },
#endif

#if (MAX_SKILL_NUM != SID_SpdBonus)
    [SID_SpdBonus] = {
        .desc = MSG_SKILL_SpdBonus,
        .icon = GFX_SkillIcon_SpdBonus,
    },
#endif

#if (MAX_SKILL_NUM != SID_LckBonus)
    [SID_LckBonus] = {
        .desc = MSG_SKILL_LukBonus,
        .icon = GFX_SkillIcon_LukBonus,
    },
#endif

#if (MAX_SKILL_NUM != SID_DefBonus)
    [SID_DefBonus] = {
        .desc = MSG_SKILL_DefBonus,
        .icon = GFX_SkillIcon_DefBonus,
    },
#endif

#if (MAX_SKILL_NUM != SID_ResBonus)
    [SID_ResBonus] = {
        .desc = MSG_SKILL_ResBonus,
        .icon = GFX_SkillIcon_ResBonus,
    },
#endif

#if (MAX_SKILL_NUM != SID_MovBonus)
    [SID_MovBonus] = {
        .desc = MSG_SKILL_MovBonus,
        .icon = GFX_SkillIcon_MovBonus,
    },
#endif

#if (MAX_SKILL_NUM != SID_DefiantStr)
    [SID_DefiantStr] = {
        .desc = MSG_SKILL_DefiantStr,
        .icon = GFX_SkillIcon_DefiantStr,
    },
#endif

#if (MAX_SKILL_NUM != SID_DefiantMag)
    [SID_DefiantMag] = {
        .desc = MSG_SKILL_DefiantMag,
        .icon = GFX_SkillIcon_DefiantMag,
    },
#endif

#if (MAX_SKILL_NUM != SID_DefiantSkl)
    [SID_DefiantSkl] = {
        .desc = MSG_SKILL_DefiantSkl,
        .icon = GFX_SkillIcon_DefiantSkl,
    },
#endif

#if (MAX_SKILL_NUM != SID_DefiantSpd)
    [SID_DefiantSpd] = {
        .desc = MSG_SKILL_DefiantSpd,
        .icon = GFX_SkillIcon_DefiantSpd,
    },
#endif

#if (MAX_SKILL_NUM != SID_DefiantLck)
    [SID_DefiantLck] = {
        .desc = MSG_SKILL_DefiantLck,
        .icon = GFX_SkillIcon_DefiantLck,
    },
#endif

#if (MAX_SKILL_NUM != SID_DefiantDef)
    [SID_DefiantDef] = {
        .desc = MSG_SKILL_DefiantDef,
        .icon = GFX_SkillIcon_DefiantDef,
    },
#endif

#if (MAX_SKILL_NUM != SID_DefiantRes)
    [SID_DefiantRes] = {
        .desc = MSG_SKILL_DefiantRes,
        .icon = GFX_SkillIcon_DefiantRes,
    },
#endif

#if (MAX_SKILL_NUM != SID_DefiantCrit)
    [SID_DefiantCrit] = {
        .desc = MSG_SKILL_DefiantCrit,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_DefiantAvoid)
    [SID_DefiantAvoid] = {
        .desc = MSG_SKILL_DefiantAvoid,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_Fury)
    [SID_Fury] = {
        .desc = MSG_SKILL_Fury,
        .icon = GFX_SkillIcon_Fury,
    },
#endif

#if (MAX_SKILL_NUM != SID_FuryPlus)
    [SID_FuryPlus] = {
        .desc = MSG_SKILL_FuryPlus,
        .icon = GFX_SkillIcon_FuryPlus,
    },
#endif

#if (MAX_SKILL_NUM != SID_FortressDef)
    [SID_FortressDef] = {
        .desc = MSG_SKILL_FortressDef,
        .icon = GFX_SkillIcon_FortressDef,
    },
#endif

#if (MAX_SKILL_NUM != SID_FortressRes)
    [SID_FortressRes] = {
        .desc = MSG_SKILL_FortressRes,
        .icon = GFX_SkillIcon_FortressRes,
    },
#endif

#if (MAX_SKILL_NUM != SID_LifeAndDeath)
    [SID_LifeAndDeath] = {
        .desc = MSG_SKILL_LifeAndDeath,
        .icon = GFX_SkillIcon_LifeAndDeath,
    },
#endif

#if (MAX_SKILL_NUM != SID_LuckySeven)
    [SID_LuckySeven] = {
        .desc = MSG_SKILL_LuckySeven,
        .icon = GFX_SkillIcon_LuckySeven,
    },
#endif

#if (MAX_SKILL_NUM != SID_Nullify)
    [SID_Nullify] = {
        .desc = MSG_SKILL_Nullify,
        .icon = GFX_SkillIcon_Nullify,
    },
#endif

#if (MAX_SKILL_NUM != SID_Slayer)
    [SID_Slayer] = {
        .desc = MSG_SKILL_Slayer,
        .icon = GFX_SkillIcon_Slayer,
    },
#endif

#if (MAX_SKILL_NUM != SID_Skybreaker)
    [SID_Skybreaker] = {
        .desc = MSG_SKILL_Skybreaker,
        .icon = GFX_SkillIcon_Skybreaker,
    },
#endif

#if (MAX_SKILL_NUM != SID_Resourceful)
    [SID_Resourceful] = {
        .desc = MSG_SKILL_Resourceful,
        .icon = GFX_SkillIcon_Resourceful,
    },
#endif

#if (MAX_SKILL_NUM != SID_Lethality)
    [SID_Lethality] = {
        .desc = MSG_SKILL_Lethality,
        .icon = GFX_SkillIcon_Lethality,
    },
#endif

#if (MAX_SKILL_NUM != SID_Crit)
    [SID_Crit] = {
        .desc = MSG_SKILL_Crit,
        .icon = GFX_SkillIcon_Crit,
    },
#endif

#if (MAX_SKILL_NUM != SID_WatchfulEye)
    [SID_WatchfulEye] = {
        .desc = MSG_SKILL_WatchfulEye,
        .icon = GFX_SkillIcon_WatchfulEye,
    },
#endif

#if (MAX_SKILL_NUM != SID_CritSword)
    [SID_CritSword] = {
        .desc = MSG_SKILL_CritSword,
        .icon = GFX_SkillIcon_CritSword,
    },
#endif

#if (MAX_SKILL_NUM != SID_CritAxe)
    [SID_CritAxe] = {
        .desc = MSG_SKILL_CritAxe,
        .icon = GFX_SkillIcon_CritAxe,
    },
#endif

#if (MAX_SKILL_NUM != SID_CritLance)
    [SID_CritLance] = {
        .desc = MSG_SKILL_CritLance,
        .icon = GFX_SkillIcon_CritLance,
    },
#endif

#if (MAX_SKILL_NUM != SID_CritBow)
    [SID_CritBow] = {
        .desc = MSG_SKILL_CritBow,
        .icon = GFX_SkillIcon_CritBow,
    },
#endif

#if (MAX_SKILL_NUM != SID_FaireSword)
    [SID_FaireSword] = {
        .desc = MSG_SKILL_FaireSword,
        .icon = GFX_SkillIcon_FaireSword,
    },
#endif

#if (MAX_SKILL_NUM != SID_FaireLance)
    [SID_FaireLance] = {
        .desc = MSG_SKILL_FaireLance,
        .icon = GFX_SkillIcon_FaireLance,
    },
#endif

#if (MAX_SKILL_NUM != SID_FaireAxe)
    [SID_FaireAxe] = {
        .desc = MSG_SKILL_FaireAxe,
        .icon = GFX_SkillIcon_FaireAxe,
    },
#endif

#if (MAX_SKILL_NUM != SID_FaireBow)
    [SID_FaireBow] = {
        .desc = MSG_SKILL_FaireBow,
        .icon = GFX_SkillIcon_FaireBow,
    },
#endif

#if (MAX_SKILL_NUM != SID_FaireBMag)
    [SID_FaireBMag] = {
        .desc = MSG_SKILL_FaireBMag,
        .icon = GFX_SkillIcon_FaireBMag,
    },
#endif

#if (MAX_SKILL_NUM != SID_Avoid)
    [SID_Avoid] = {
        .desc = MSG_SKILL_Avoid,
        .icon = GFX_SkillIcon_Avoid,
    },
#endif

#if (MAX_SKILL_NUM != SID_AvoidSword)
    [SID_AvoidSword] = {
        .desc = MSG_SKILL_AvoidSword,
        .icon = GFX_SkillIcon_AvoidSword,
    },
#endif

#if (MAX_SKILL_NUM != SID_RuinedBlade)
    [SID_RuinedBlade] = {
        .desc = MSG_SKILL_RuinedBlade,
        .icon = GFX_SkillIcon_RuinedBlade,
    },
#endif

#if (MAX_SKILL_NUM != SID_RuinedBladePlus)
    [SID_RuinedBladePlus] = {
        .desc = MSG_SKILL_RuinedBladePlus,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_InfinityEdge)
    [SID_InfinityEdge] = {
        .desc = MSG_SKILL_InfinityEdge,
        .icon = GFX_SkillIcon_InfinityEdge,
    },
#endif

#if (MAX_SKILL_NUM != SID_HeavyBlade)
    [SID_HeavyBlade] = {
        .desc = MSG_SKILL_HeavyBlade,
        .icon = GFX_SkillIcon_HeavyBlade,
    },
#endif

#if (MAX_SKILL_NUM != SID_FlashingBlade)
    [SID_FlashingBlade] = {
        .desc = MSG_SKILL_FlashingBlade,
        .icon = GFX_SkillIcon_FlashingBlade,
    },
#endif

#if (MAX_SKILL_NUM != SID_HeavyBladePlus)
    [SID_HeavyBladePlus] = {
        .desc = MSG_SKILL_HeavyBladePlus,
        .icon = GFX_SkillIcon_HeavyBladePlus,
    },
#endif

#if (MAX_SKILL_NUM != SID_FlashingBladePlus)
    [SID_FlashingBladePlus] = {
        .desc = MSG_SKILL_FlashingBladePlus,
        .icon = GFX_SkillIcon_FlashingBladePlus,
    },
#endif

#if (MAX_SKILL_NUM != SID_LunaAttack)
    [SID_LunaAttack] = {
        .desc = MSG_SKILL_LunaAttack,
        .icon = GFX_SkillIcon_LunaAttack,
    },
#endif

#if (MAX_SKILL_NUM != SID_SorceryBlade)
    [SID_SorceryBlade] = {
        .desc = MSG_SKILL_SorceryBlade,
        .icon = GFX_SkillIcon_SorceryBlade,
    },
#endif

#if (MAX_SKILL_NUM != SID_Fortune)
    [SID_Fortune] = {
        .desc = MSG_SKILL_Fortune,
        .icon = GFX_SkillIcon_Fortune,
    },
#endif

#if (MAX_SKILL_NUM != SID_BlueFlame)
    [SID_BlueFlame] = {
        .desc = MSG_SKILL_BlueFlame,
        .icon = GFX_SkillIcon_BlueFlame,
    },
#endif

#if (MAX_SKILL_NUM != SID_SwordBreaker)
    [SID_SwordBreaker] = {
        .desc = MSG_SKILL_SwordBreaker,
        .icon = GFX_SkillIcon_SwordBreaker,
    },
#endif

#if (MAX_SKILL_NUM != SID_AxeBreaker)
    [SID_AxeBreaker] = {
        .desc = MSG_SKILL_AxeBreaker,
        .icon = GFX_SkillIcon_AxeBreaker,
    },
#endif

#if (MAX_SKILL_NUM != SID_LanceBreaker)
    [SID_LanceBreaker] = {
        .desc = MSG_SKILL_LanceBreaker,
        .icon = GFX_SkillIcon_LanceBreaker,
    },
#endif

#if (MAX_SKILL_NUM != SID_BowBreaker)
    [SID_BowBreaker] = {
        .desc = MSG_SKILL_BowBreaker,
        .icon = GFX_SkillIcon_BowBreaker,
    },
#endif

#if (MAX_SKILL_NUM != SID_BMagBreaker)
    [SID_BMagBreaker] = {
        .desc = MSG_SKILL_BMagBreaker,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_Vantage)
    [SID_Vantage] = {
        .desc = MSG_SKILL_Vantage,
        .icon = GFX_SkillIcon_Vantage,
    },
#endif

#if (MAX_SKILL_NUM != SID_Desperation)
    [SID_Desperation] = {
        .desc = MSG_SKILL_Desperation,
        .icon = GFX_SkillIcon_Desperation,
    },
#endif

#if (MAX_SKILL_NUM != SID_QuickRiposte)
    [SID_QuickRiposte] = {
        .desc = MSG_SKILL_QuickRiposte,
        .icon = GFX_SkillIcon_QuickRiposte,
    },
#endif

#if (MAX_SKILL_NUM != SID_WaryFighter)
    [SID_WaryFighter] = {
        .desc = MSG_SKILL_WaryFighter,
        .icon = GFX_SkillIcon_WaryFighter,
    },
#endif

#if (MAX_SKILL_NUM != SID_DoubleLion)
    [SID_DoubleLion] = {
        .desc = MSG_SKILL_DoubleLion,
        .icon = GFX_SkillIcon_DoubleLion,
    },
#endif

#if (MAX_SKILL_NUM != SID_Adept)
    [SID_Adept] = {
        .desc = MSG_SKILL_Adept,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_Inori)
    [SID_Inori] = {
        .desc = MSG_SKILL_Inori,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_Astra)
    [SID_Astra] = {
        .desc = MSG_SKILL_Astra,
        .icon = GFX_SkillIcon_Astra,
    },
#endif

#if (MAX_SKILL_NUM != SID_Aegis)
    [SID_Aegis] = {
        .desc = MSG_SKILL_Aegis,
        .icon = GFX_SkillIcon_Aegis,
    },
#endif

#if (MAX_SKILL_NUM != SID_Pavise)
    [SID_Pavise] = {
        .desc = MSG_SKILL_Pavise,
        .icon = GFX_SkillIcon_Pavise,
    },
#endif

#if (MAX_SKILL_NUM != SID_Bane)
    [SID_Bane] = {
        .desc = MSG_SKILL_Bane,
        .icon = GFX_SkillIcon_Bane,
    },
#endif

#if (MAX_SKILL_NUM != SID_Aether)
    [SID_Aether] = {
        .desc = MSG_SKILL_Aether,
        .icon = GFX_SkillIcon_Aether,
    },
#endif

#if (MAX_SKILL_NUM != SID_Corona)
    [SID_Corona] = {
        .desc = MSG_SKILL_Corona,
        .icon = GFX_SkillIcon_Corona,
    },
#endif

#if (MAX_SKILL_NUM != SID_Luna)
    [SID_Luna] = {
        .desc = MSG_SKILL_Luna,
        .icon = GFX_SkillIcon_Luna,
    },
#endif

#if (MAX_SKILL_NUM != SID_Flare)
    [SID_Flare] = {
        .desc = MSG_SKILL_Flare,
        .icon = GFX_SkillIcon_Flare,
    },
#endif

#if (MAX_SKILL_NUM != SID_Sol)
    [SID_Sol] = {
        .desc = MSG_SKILL_Sol,
        .icon = GFX_SkillIcon_Sol,
    },
#endif

#if (MAX_SKILL_NUM != SID_DragonFang)
    [SID_DragonFang] = {
        .desc = MSG_SKILL_DragonFang,
        .icon = GFX_SkillIcon_DragonFang,
    },
#endif

#if (MAX_SKILL_NUM != SID_Foresight)
    [SID_Foresight] = {
        .desc = MSG_SKILL_Foresight,
        .icon = GFX_SkillIcon_Foresight,
    },
#endif

#if (MAX_SKILL_NUM != SID_BlowDarting)
    [SID_BlowDarting] = {
        .desc = MSG_SKILL_BlowDarting,
        .icon = GFX_SkillIcon_BlowDarting,
    },
#endif

#if (MAX_SKILL_NUM != SID_BlowDeath)
    [SID_BlowDeath] = {
        .desc = MSG_SKILL_BlowDeath,
        .icon = GFX_SkillIcon_BlowDeath,
    },
#endif

#if (MAX_SKILL_NUM != SID_BlowArmored)
    [SID_BlowArmored] = {
        .desc = MSG_SKILL_BlowArmored,
        .icon = GFX_SkillIcon_BlowArmored,
    },
#endif

#if (MAX_SKILL_NUM != SID_BlowFiendish)
    [SID_BlowFiendish] = {
        .desc = MSG_SKILL_BlowFiendish,
        .icon = GFX_SkillIcon_BlowFiendish,
    },
#endif

#if (MAX_SKILL_NUM != SID_BlowWarding)
    [SID_BlowWarding] = {
        .desc = MSG_SKILL_BlowWarding,
        .icon = GFX_SkillIcon_BlowWarding,
    },
#endif

#if (MAX_SKILL_NUM != SID_BlowDuelist)
    [SID_BlowDuelist] = {
        .desc = MSG_SKILL_BlowDuelist,
        .icon = GFX_SkillIcon_BlowDuelist,
    },
#endif

#if (MAX_SKILL_NUM != SID_BlowUncanny)
    [SID_BlowUncanny] = {
        .desc = MSG_SKILL_BlowUncanny,
        .icon = GFX_SkillIcon_BlowUncanny,
    },
#endif

#if (MAX_SKILL_NUM != SID_StanceBracing)
    [SID_StanceBracing] = {
        .desc = MSG_SKILL_StanceBracing,
        .icon = GFX_SkillIcon_StanceBracing,
    },
#endif

#if (MAX_SKILL_NUM != SID_StanceDarting)
    [SID_StanceDarting] = {
        .desc = MSG_SKILL_StanceDarting,
        .icon = GFX_SkillIcon_StanceDarting,
    },
#endif

#if (MAX_SKILL_NUM != SID_StanceFierce)
    [SID_StanceFierce] = {
        .desc = MSG_SKILL_StanceFierce,
        .icon = GFX_SkillIcon_StanceFierce,
    },
#endif

#if (MAX_SKILL_NUM != SID_StanceKestrel)
    [SID_StanceKestrel] = {
        .desc = MSG_SKILL_StanceKestrel,
        .icon = GFX_SkillIcon_StanceKestrel,
    },
#endif

#if (MAX_SKILL_NUM != SID_StanceMirror)
    [SID_StanceMirror] = {
        .desc = MSG_SKILL_StanceMirror,
        .icon = GFX_SkillIcon_StanceMirror,
    },
#endif

#if (MAX_SKILL_NUM != SID_StanceReady)
    [SID_StanceReady] = {
        .desc = MSG_SKILL_StanceReady,
        .icon = GFX_SkillIcon_StanceReady,
    },
#endif

#if (MAX_SKILL_NUM != SID_StanceSteady)
    [SID_StanceSteady] = {
        .desc = MSG_SKILL_StanceSteady,
        .icon = GFX_SkillIcon_StanceSteady,
    },
#endif

#if (MAX_SKILL_NUM != SID_StanceSturdy)
    [SID_StanceSturdy] = {
        .desc = MSG_SKILL_StanceSturdy,
        .icon = GFX_SkillIcon_StanceSturdy,
    },
#endif

#if (MAX_SKILL_NUM != SID_StanceSwift)
    [SID_StanceSwift] = {
        .desc = MSG_SKILL_StanceSwift,
        .icon = GFX_SkillIcon_StanceSwift,
    },
#endif

#if (MAX_SKILL_NUM != SID_StanceWarding)
    [SID_StanceWarding] = {
        .desc = MSG_SKILL_StanceWarding,
        .icon = GFX_SkillIcon_StanceWarding,
    },
#endif

#if (MAX_SKILL_NUM != SID_RangeBonusBMag1)
    [SID_RangeBonusBMag1] = {
        .desc = MSG_SKILL_RangeBonusBMag1,
        .icon = GFX_SkillIcon_RangeBonusBMag1,
    },
#endif

#if (MAX_SKILL_NUM != SID_RangeBonusBMag2)
    [SID_RangeBonusBMag2] = {
        .desc = MSG_SKILL_RangeBonusBMag2,
        .icon = GFX_SkillIcon_RangeBonusBMag2,
    },
#endif

#if (MAX_SKILL_NUM != SID_RangeBonusBow1)
    [SID_RangeBonusBow1] = {
        .desc = MSG_SKILL_RangeBonusBow1,
        .icon = GFX_SkillIcon_RangeBonusBow1,
    },
#endif

#if (MAX_SKILL_NUM != SID_RangeBonusBow2)
    [SID_RangeBonusBow2] = {
        .desc = MSG_SKILL_RangeBonusBow2,
        .icon = GFX_SkillIcon_RangeBonusBow2,
    },
#endif

#if (MAX_SKILL_NUM != SID_Canto)
    [SID_Canto] = {
        .desc = MSG_SKILL_Canto,
        .icon = GFX_SkillIcon_Canto,
    },
#endif

#if (MAX_SKILL_NUM != SID_CantoPlus)
    [SID_CantoPlus] = {
        .desc = MSG_SKILL_CantoPlus,
        .icon = GFX_SkillIcon_CantoPlus,
    },
#endif

#if (MAX_SKILL_NUM != SID_AlertStance)
    [SID_AlertStance] = {
        .desc = MSG_SKILL_AlertStance,
        .icon = GFX_SkillIcon_AlertStance,
    },
#endif

#if (MAX_SKILL_NUM != SID_AlertStancePlus)
    [SID_AlertStancePlus] = {
        .desc = MSG_SKILL_AlertStancePlus,
        .icon = GFX_SkillIcon_AlertStancePlus,
    },
#endif

#if (MAX_SKILL_NUM != SID_Galeforce)
    [SID_Galeforce] = {
        .desc = MSG_SKILL_Galeforce,
        .icon = GFX_SkillIcon_Galeforce,
    },
#endif

#if (MAX_SKILL_NUM != SID_SavageBlow)
    [SID_SavageBlow] = {
        .desc = MSG_SKILL_SavageBlow,
        .icon = GFX_SkillIcon_SavageBlow,
    },
#endif

#if (MAX_SKILL_NUM != SID_BreathOfLife)
    [SID_BreathOfLife] = {
        .desc = MSG_SKILL_BreathOfLife,
        .icon = GFX_SkillIcon_BreathOfLife,
    },
#endif

#if (MAX_SKILL_NUM != SID_Renewal)
    [SID_Renewal] = {
        .desc = MSG_SKILL_Renewal,
        .icon = GFX_SkillIcon_Renewal,
    },
#endif

#if (MAX_SKILL_NUM != SID_Anathema)
    [SID_Anathema] = {
        .desc = MSG_SKILL_Anathema,
        .icon = GFX_SkillIcon_Anathema,
    },
#endif

#if (MAX_SKILL_NUM != SID_Bond)
    [SID_Bond] = {
        .desc = MSG_SKILL_Bond,
        .icon = GFX_SkillIcon_Bond,
    },
#endif

#if (MAX_SKILL_NUM != SID_Charm)
    [SID_Charm] = {
        .desc = MSG_SKILL_Charm,
        .icon = GFX_SkillIcon_Charm,
    },
#endif

#if (MAX_SKILL_NUM != SID_Intimidate)
    [SID_Intimidate] = {
        .desc = MSG_SKILL_Intimidate,
        .icon = GFX_SkillIcon_Intimidate,
    },
#endif

#if (MAX_SKILL_NUM != SID_Tantivy)
    [SID_Tantivy] = {
        .desc = MSG_SKILL_Tantivy,
        .icon = GFX_SkillIcon_Tantivy,
    },
#endif

#if (MAX_SKILL_NUM != SID_Focus)
    [SID_Focus] = {
        .desc = MSG_SKILL_Focus,
        .icon = GFX_SkillIcon_Focus,
    },
#endif

#if (MAX_SKILL_NUM != SID_Hex)
    [SID_Hex] = {
        .desc = MSG_SKILL_Hex,
        .icon = GFX_SkillIcon_Hex,
    },
#endif

#if (MAX_SKILL_NUM != SID_Infiltrator)
    [SID_Infiltrator] = {
        .desc = MSG_SKILL_Infiltrator,
        .icon = GFX_SkillIcon_Infiltrator,
    },
#endif

#if (MAX_SKILL_NUM != SID_Inspiration)
    [SID_Inspiration] = {
        .desc = MSG_SKILL_Inspiration,
        .icon = GFX_SkillIcon_Inspiration,
    },
#endif

#if (MAX_SKILL_NUM != SID_DivinelyInspiring)
    [SID_DivinelyInspiring] = {
        .desc = MSG_SKILL_DivinelyInspiring,
        .icon = GFX_SkillIcon_DivinelyInspiring,
    },
#endif

#if (MAX_SKILL_NUM != SID_VoiceOfPeace)
    [SID_VoiceOfPeace] = {
        .desc = MSG_SKILL_VoiceOfPeace,
        .icon = GFX_SkillIcon_VoiceOfPeace,
    },
#endif

#if (MAX_SKILL_NUM != SID_Shade)
    [SID_Shade] = {
        .desc = MSG_SKILL_Shade,
        .icon = GFX_SkillIcon_Shade,
    },
#endif

#if (MAX_SKILL_NUM != SID_ShadePlus)
    [SID_ShadePlus] = {
        .desc = MSG_SKILL_ShadePlus,
        .icon = GFX_SkillIcon_ShadePlus,
    },
#endif

#if (MAX_SKILL_NUM != SID_Provoke)
    [SID_Provoke] = {
        .desc = MSG_SKILL_Provoke,
        .icon = GFX_SkillIcon_Provoke,
    },
#endif

#if (MAX_SKILL_NUM != SID_Dance)
    [SID_Dance] = {
        .desc = MSG_SKILL_Dance,
        .icon = GFX_SkillIcon_Dance,
    },
#endif

#if (MAX_SKILL_NUM != SID_LockTouch)
    [SID_LockTouch] = {
        .desc = MSG_SKILL_LockTouch,
        .icon = GFX_SkillIcon_LockTouch,
    },
#endif

#if (MAX_SKILL_NUM != SID_Summon)
    [SID_Summon] = {
        .desc = MSG_SKILL_Summon,
        .icon = GFX_SkillIcon_Summon,
    },
#endif

#if (MAX_SKILL_NUM != SID_Supply)
    [SID_Supply] = {
        .desc = MSG_SKILL_Supply,
        .icon = GFX_SkillIcon_Supply,
    },
#endif

#if (MAX_SKILL_NUM != SID_Steal)
    [SID_Steal] = {
        .desc = MSG_SKILL_Steal,
        .icon = GFX_SkillIcon_Steal,
    },
#endif

#if (MAX_SKILL_NUM != SID_Blossom)
    [SID_Blossom] = {
        .desc = MSG_SKILL_Blossom,
        .icon = GFX_SkillIcon_Blossom,
    },
#endif

#if (MAX_SKILL_NUM != SID_Paragon)
    [SID_Paragon] = {
        .desc = MSG_SKILL_Paragon,
        .icon = GFX_SkillIcon_Paragon,
    },
#endif

#if (MAX_SKILL_NUM != SID_VoidCurse)
    [SID_VoidCurse] = {
        .desc = MSG_SKILL_VoidCurse,
        .icon = GFX_SkillIcon_VoidCurse,
    },
#endif

#if (MAX_SKILL_NUM != SID_Aptitude)
    [SID_Aptitude] = {
        .desc = MSG_SKILL_Aptitude,
        .icon = GFX_SkillIcon_Aptitude,
    },
#endif

#if (MAX_SKILL_NUM != SID_LEGEND_InoriAtk)
    [SID_LEGEND_InoriAtk] = {
        .desc = MSG_SKILL_LEGEND_InoriAtk,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_LEGEND_InoriAvo)
    [SID_LEGEND_InoriAvo] = {
        .desc = MSG_SKILL_LEGEND_InoriAvo,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_LEGEND_InoriDef)
    [SID_LEGEND_InoriDef] = {
        .desc = MSG_SKILL_LEGEND_InoriDef,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_LEGEND_4)
    [SID_LEGEND_4] = {
        .desc = MSG_WIP,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_LEGEND_5)
    [SID_LEGEND_5] = {
        .desc = MSG_WIP,
        .icon = GFX_SkillIcon_WIP,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_Grounder)
    [SID_COMBAT_Grounder] = {
        .desc = MSG_SKILL_COMBAT_Grounder,
        .icon = GFX_CombatArtIcon_SwordArtAtk,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_BaneOfMonsters)
    [SID_COMBAT_BaneOfMonsters] = {
        .desc = MSG_SKILL_COMBAT_BaneOfMonsters,
        .icon = GFX_CombatArtIcon_SwordArtAtk,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_Hexblade)
    [SID_COMBAT_Hexblade] = {
        .desc = MSG_SKILL_COMBAT_Hexblade,
        .icon = GFX_CombatArtIcon_SwordArtMag,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_FoudroyantStrike)
    [SID_COMBAT_FoudroyantStrike] = {
        .desc = MSG_SKILL_COMBAT_FoudroyantStrike,
        .icon = GFX_CombatArtIcon_SwordArtCrit,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_TempestLance)
    [SID_COMBAT_TempestLance] = {
        .desc = MSG_SKILL_COMBAT_TempestLance,
        .icon = GFX_CombatArtIcon_LanceArtAtk,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_SwiftStrikes)
    [SID_COMBAT_SwiftStrikes] = {
        .desc = MSG_SKILL_COMBAT_SwiftStrikes,
        .icon = GFX_CombatArtIcon_LanceArtAtk,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_FrozenLance)
    [SID_COMBAT_FrozenLance] = {
        .desc = MSG_SKILL_COMBAT_FrozenLance,
        .icon = GFX_CombatArtIcon_LanceArtMag,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_Vengeance)
    [SID_COMBAT_Vengeance] = {
        .desc = MSG_SKILL_COMBAT_Vengeance,
        .icon = GFX_CombatArtIcon_LanceArtCrit,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_HelmSplitter)
    [SID_COMBAT_HelmSplitter] = {
        .desc = MSG_SKILL_COMBAT_HelmSplitter,
        .icon = GFX_CombatArtIcon_AxeArtAtk,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_LightningAxe)
    [SID_COMBAT_LightningAxe] = {
        .desc = MSG_SKILL_COMBAT_LightningAxe,
        .icon = GFX_CombatArtIcon_AxeArtMag,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_WarMasterStrike)
    [SID_COMBAT_WarMasterStrike] = {
        .desc = MSG_SKILL_COMBAT_WarMasterStrike,
        .icon = GFX_CombatArtIcon_AxeArtCrit,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_CurvedShot)
    [SID_COMBAT_CurvedShot] = {
        .desc = MSG_SKILL_COMBAT_CurvedShot,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_Deadeye)
    [SID_COMBAT_Deadeye] = {
        .desc = MSG_SKILL_COMBAT_Deadeye,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_Encloser)
    [SID_COMBAT_Encloser] = {
        .desc = MSG_SKILL_COMBAT_Encloser,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_EncloserPlus)
    [SID_COMBAT_EncloserPlus] = {
        .desc = MSG_SKILL_COMBAT_EncloserPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_MonsterBlast)
    [SID_COMBAT_MonsterBlast] = {
        .desc = MSG_SKILL_COMBAT_MonsterBlast,
        .icon = GFX_CombatArtIcon_BowArtCrit,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_SchismShot)
    [SID_COMBAT_SchismShot] = {
        .desc = MSG_SKILL_COMBAT_SchismShot,
        .icon = GFX_CombatArtIcon_BowArtCrit,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_BreakShot)
    [SID_COMBAT_BreakShot] = {
        .desc = MSG_SKILL_COMBAT_BreakShot,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_BreakShotPlus)
    [SID_COMBAT_BreakShotPlus] = {
        .desc = MSG_SKILL_COMBAT_BreakShotPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_WaningShot)
    [SID_COMBAT_WaningShot] = {
        .desc = MSG_SKILL_COMBAT_WaningShot,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_WaningShotPlus)
    [SID_COMBAT_WaningShotPlus] = {
        .desc = MSG_SKILL_COMBAT_WaningShotPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (MAX_SKILL_NUM != SID_COMBAT_HuntersVolley)
    [SID_COMBAT_HuntersVolley] = {
        .desc = MSG_SKILL_COMBAT_HuntersVolley,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif
};

FEB_IDENTIFIER(gSkillInfos);
