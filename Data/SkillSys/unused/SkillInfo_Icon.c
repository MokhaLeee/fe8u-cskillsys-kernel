#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/gfx.h"

u8 const * const gSkillIcons[MAX_SKILL_NUM + 1] = {
#if (defined(SID_HpBonus) && COMMON_SKILL_VALID(SID_HpBonus))
    [SID_HpBonus] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_StrBonus) && COMMON_SKILL_VALID(SID_StrBonus))
    [SID_StrBonus] = GFX_SkillIcon_StrBonus,
#endif

#if (defined(SID_MagBonus) && COMMON_SKILL_VALID(SID_MagBonus))
    [SID_MagBonus] = GFX_SkillIcon_MagBonus,
#endif

#if (defined(SID_SklBonus) && COMMON_SKILL_VALID(SID_SklBonus))
    [SID_SklBonus] = GFX_SkillIcon_SklBonus,
#endif

#if (defined(SID_SpdBonus) && COMMON_SKILL_VALID(SID_SpdBonus))
    [SID_SpdBonus] = GFX_SkillIcon_SpdBonus,
#endif

#if (defined(SID_LckBonus) && COMMON_SKILL_VALID(SID_LckBonus))
    [SID_LckBonus] = GFX_SkillIcon_LukBonus,
#endif

#if (defined(SID_DefBonus) && COMMON_SKILL_VALID(SID_DefBonus))
    [SID_DefBonus] = GFX_SkillIcon_DefBonus,
#endif

#if (defined(SID_ResBonus) && COMMON_SKILL_VALID(SID_ResBonus))
    [SID_ResBonus] = GFX_SkillIcon_ResBonus,
#endif

#if (defined(SID_MovBonus) && COMMON_SKILL_VALID(SID_MovBonus))
    [SID_MovBonus] = GFX_SkillIcon_MovBonus,
#endif

#if (defined(SID_DefiantStr) && COMMON_SKILL_VALID(SID_DefiantStr))
    [SID_DefiantStr] = GFX_SkillIcon_DefiantStr,
#endif

#if (defined(SID_DefiantMag) && COMMON_SKILL_VALID(SID_DefiantMag))
    [SID_DefiantMag] = GFX_SkillIcon_DefiantMag,
#endif

#if (defined(SID_DefiantSkl) && COMMON_SKILL_VALID(SID_DefiantSkl))
    [SID_DefiantSkl] = GFX_SkillIcon_DefiantSkl,
#endif

#if (defined(SID_DefiantSpd) && COMMON_SKILL_VALID(SID_DefiantSpd))
    [SID_DefiantSpd] = GFX_SkillIcon_DefiantSpd,
#endif

#if (defined(SID_DefiantLck) && COMMON_SKILL_VALID(SID_DefiantLck))
    [SID_DefiantLck] = GFX_SkillIcon_DefiantLck,
#endif

#if (defined(SID_DefiantDef) && COMMON_SKILL_VALID(SID_DefiantDef))
    [SID_DefiantDef] = GFX_SkillIcon_DefiantDef,
#endif

#if (defined(SID_DefiantRes) && COMMON_SKILL_VALID(SID_DefiantRes))
    [SID_DefiantRes] = GFX_SkillIcon_DefiantRes,
#endif

#if (defined(SID_DefiantCrit) && COMMON_SKILL_VALID(SID_DefiantCrit))
    [SID_DefiantCrit] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_DefiantAvoid) && COMMON_SKILL_VALID(SID_DefiantAvoid))
    [SID_DefiantAvoid] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Fury) && COMMON_SKILL_VALID(SID_Fury))
    [SID_Fury] = GFX_SkillIcon_Fury,
#endif

#if (defined(SID_FuryPlus) && COMMON_SKILL_VALID(SID_FuryPlus))
    [SID_FuryPlus] = GFX_SkillIcon_FuryPlus,
#endif

#if (defined(SID_FortressDef) && COMMON_SKILL_VALID(SID_FortressDef))
    [SID_FortressDef] = GFX_SkillIcon_FortressDef,
#endif

#if (defined(SID_FortressRes) && COMMON_SKILL_VALID(SID_FortressRes))
    [SID_FortressRes] = GFX_SkillIcon_FortressRes,
#endif

#if (defined(SID_LifeAndDeath) && COMMON_SKILL_VALID(SID_LifeAndDeath))
    [SID_LifeAndDeath] = GFX_SkillIcon_LifeAndDeath,
#endif

#if (defined(SID_LuckySeven) && COMMON_SKILL_VALID(SID_LuckySeven))
    [SID_LuckySeven] = GFX_SkillIcon_LuckySeven,
#endif

#if (defined(SID_Nullify) && COMMON_SKILL_VALID(SID_Nullify))
    [SID_Nullify] = GFX_SkillIcon_Nullify,
#endif

#if (defined(SID_Slayer) && COMMON_SKILL_VALID(SID_Slayer))
    [SID_Slayer] = GFX_SkillIcon_Slayer,
#endif

#if (defined(SID_Skybreaker) && COMMON_SKILL_VALID(SID_Skybreaker))
    [SID_Skybreaker] = GFX_SkillIcon_Skybreaker,
#endif

#if (defined(SID_Resourceful) && COMMON_SKILL_VALID(SID_Resourceful))
    [SID_Resourceful] = GFX_SkillIcon_Resourceful,
#endif

#if (defined(SID_Nihil) && COMMON_SKILL_VALID(SID_Nihil))
    [SID_Nihil] = GFX_SkillIcon_Nihil,
#endif

#if (defined(SID_CritSword) && COMMON_SKILL_VALID(SID_CritSword))
    [SID_CritSword] = GFX_SkillIcon_CritSword,
#endif

#if (defined(SID_CritAxe) && COMMON_SKILL_VALID(SID_CritAxe))
    [SID_CritAxe] = GFX_SkillIcon_CritAxe,
#endif

#if (defined(SID_CritLance) && COMMON_SKILL_VALID(SID_CritLance))
    [SID_CritLance] = GFX_SkillIcon_CritLance,
#endif

#if (defined(SID_CritBow) && COMMON_SKILL_VALID(SID_CritBow))
    [SID_CritBow] = GFX_SkillIcon_CritBow,
#endif

#if (defined(SID_FaireSword) && COMMON_SKILL_VALID(SID_FaireSword))
    [SID_FaireSword] = GFX_SkillIcon_FaireSword,
#endif

#if (defined(SID_FaireLance) && COMMON_SKILL_VALID(SID_FaireLance))
    [SID_FaireLance] = GFX_SkillIcon_FaireLance,
#endif

#if (defined(SID_FaireAxe) && COMMON_SKILL_VALID(SID_FaireAxe))
    [SID_FaireAxe] = GFX_SkillIcon_FaireAxe,
#endif

#if (defined(SID_FaireBow) && COMMON_SKILL_VALID(SID_FaireBow))
    [SID_FaireBow] = GFX_SkillIcon_FaireBow,
#endif

#if (defined(SID_FaireBMag) && COMMON_SKILL_VALID(SID_FaireBMag))
    [SID_FaireBMag] = GFX_SkillIcon_FaireBMag,
#endif

#if (defined(SID_Avoid) && COMMON_SKILL_VALID(SID_Avoid))
    [SID_Avoid] = GFX_SkillIcon_Avoid,
#endif

#if (defined(SID_AvoidSword) && COMMON_SKILL_VALID(SID_AvoidSword))
    [SID_AvoidSword] = GFX_SkillIcon_AvoidSword,
#endif

#if (defined(SID_RuinedBlade) && COMMON_SKILL_VALID(SID_RuinedBlade))
    [SID_RuinedBlade] = GFX_SkillIcon_RuinedBlade,
#endif

#if (defined(SID_HeavyBlade) && COMMON_SKILL_VALID(SID_HeavyBlade))
    [SID_HeavyBlade] = GFX_SkillIcon_HeavyBlade,
#endif

#if (defined(SID_FlashingBlade) && COMMON_SKILL_VALID(SID_FlashingBlade))
    [SID_FlashingBlade] = GFX_SkillIcon_FlashingBlade,
#endif

#if (defined(SID_HeavyBladePlus) && COMMON_SKILL_VALID(SID_HeavyBladePlus))
    [SID_HeavyBladePlus] = GFX_SkillIcon_HeavyBladePlus,
#endif

#if (defined(SID_FlashingBladePlus) && COMMON_SKILL_VALID(SID_FlashingBladePlus))
    [SID_FlashingBladePlus] = GFX_SkillIcon_FlashingBladePlus,
#endif

#if (defined(SID_LunaAttack) && COMMON_SKILL_VALID(SID_LunaAttack))
    [SID_LunaAttack] = GFX_SkillIcon_LunaAttack,
#endif

#if (defined(SID_SorceryBlade) && COMMON_SKILL_VALID(SID_SorceryBlade))
    [SID_SorceryBlade] = GFX_SkillIcon_SorceryBlade,
#endif

#if (defined(SID_Fortune) && COMMON_SKILL_VALID(SID_Fortune))
    [SID_Fortune] = GFX_SkillIcon_Fortune,
#endif

#if (defined(SID_BlueFlame) && COMMON_SKILL_VALID(SID_BlueFlame))
    [SID_BlueFlame] = GFX_SkillIcon_BlueFlame,
#endif

#if (defined(SID_SwordBreaker) && COMMON_SKILL_VALID(SID_SwordBreaker))
    [SID_SwordBreaker] = GFX_SkillIcon_SwordBreaker,
#endif

#if (defined(SID_AxeBreaker) && COMMON_SKILL_VALID(SID_AxeBreaker))
    [SID_AxeBreaker] = GFX_SkillIcon_AxeBreaker,
#endif

#if (defined(SID_LanceBreaker) && COMMON_SKILL_VALID(SID_LanceBreaker))
    [SID_LanceBreaker] = GFX_SkillIcon_LanceBreaker,
#endif

#if (defined(SID_BowBreaker) && COMMON_SKILL_VALID(SID_BowBreaker))
    [SID_BowBreaker] = GFX_SkillIcon_BowBreaker,
#endif

#if (defined(SID_BMagBreaker) && COMMON_SKILL_VALID(SID_BMagBreaker))
    [SID_BMagBreaker] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Vantage) && COMMON_SKILL_VALID(SID_Vantage))
    [SID_Vantage] = GFX_SkillIcon_Vantage,
#endif

#if (defined(SID_Desperation) && COMMON_SKILL_VALID(SID_Desperation))
    [SID_Desperation] = GFX_SkillIcon_Desperation,
#endif

#if (defined(SID_QuickRiposte) && COMMON_SKILL_VALID(SID_QuickRiposte))
    [SID_QuickRiposte] = GFX_SkillIcon_QuickRiposte,
#endif

#if (defined(SID_WaryFighter) && COMMON_SKILL_VALID(SID_WaryFighter))
    [SID_WaryFighter] = GFX_SkillIcon_WaryFighter,
#endif

#if (defined(SID_DoubleLion) && COMMON_SKILL_VALID(SID_DoubleLion))
    [SID_DoubleLion] = GFX_SkillIcon_DoubleLion,
#endif

#if (defined(SID_Adept) && COMMON_SKILL_VALID(SID_Adept))
    [SID_Adept] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Astra) && COMMON_SKILL_VALID(SID_Astra))
    [SID_Astra] = GFX_SkillIcon_Astra,
#endif

#if (defined(SID_Aegis) && COMMON_SKILL_VALID(SID_Aegis))
    [SID_Aegis] = GFX_SkillIcon_Aegis,
#endif

#if (defined(SID_Pavise) && COMMON_SKILL_VALID(SID_Pavise))
    [SID_Pavise] = GFX_SkillIcon_Pavise,
#endif

#if (defined(SID_Bane) && COMMON_SKILL_VALID(SID_Bane))
    [SID_Bane] = GFX_SkillIcon_Bane,
#endif

#if (defined(SID_Mercy) && COMMON_SKILL_VALID(SID_Mercy))
    [SID_Mercy] = GFX_SkillIcon_Mercy,
#endif

#if (defined(SID_Aether) && COMMON_SKILL_VALID(SID_Aether))
    [SID_Aether] = GFX_SkillIcon_Aether,
#endif

#if (defined(SID_Corona) && COMMON_SKILL_VALID(SID_Corona))
    [SID_Corona] = GFX_SkillIcon_Corona,
#endif

#if (defined(SID_Luna) && COMMON_SKILL_VALID(SID_Luna))
    [SID_Luna] = GFX_SkillIcon_Luna,
#endif

#if (defined(SID_Flare) && COMMON_SKILL_VALID(SID_Flare))
    [SID_Flare] = GFX_SkillIcon_Flare,
#endif

#if (defined(SID_Sol) && COMMON_SKILL_VALID(SID_Sol))
    [SID_Sol] = GFX_SkillIcon_Sol,
#endif

#if (defined(SID_DragonFang) && COMMON_SKILL_VALID(SID_DragonFang))
    [SID_DragonFang] = GFX_SkillIcon_DragonFang,
#endif

#if (defined(SID_Foresight) && COMMON_SKILL_VALID(SID_Foresight))
    [SID_Foresight] = GFX_SkillIcon_Foresight,
#endif

#if (defined(SID_BlowDarting) && COMMON_SKILL_VALID(SID_BlowDarting))
    [SID_BlowDarting] = GFX_SkillIcon_BlowDarting,
#endif

#if (defined(SID_BlowDeath) && COMMON_SKILL_VALID(SID_BlowDeath))
    [SID_BlowDeath] = GFX_SkillIcon_BlowDeath,
#endif

#if (defined(SID_BlowArmored) && COMMON_SKILL_VALID(SID_BlowArmored))
    [SID_BlowArmored] = GFX_SkillIcon_BlowArmored,
#endif

#if (defined(SID_BlowFiendish) && COMMON_SKILL_VALID(SID_BlowFiendish))
    [SID_BlowFiendish] = GFX_SkillIcon_BlowFiendish,
#endif

#if (defined(SID_BlowWarding) && COMMON_SKILL_VALID(SID_BlowWarding))
    [SID_BlowWarding] = GFX_SkillIcon_BlowWarding,
#endif

#if (defined(SID_BlowDuelist) && COMMON_SKILL_VALID(SID_BlowDuelist))
    [SID_BlowDuelist] = GFX_SkillIcon_BlowDuelist,
#endif

#if (defined(SID_BlowUncanny) && COMMON_SKILL_VALID(SID_BlowUncanny))
    [SID_BlowUncanny] = GFX_SkillIcon_BlowUncanny,
#endif

#if (defined(SID_BlowKilling) && COMMON_SKILL_VALID(SID_BlowKilling))
    [SID_BlowKilling] = GFX_SkillIcon_BlowKilling,
#endif

#if (defined(SID_StanceBracing) && COMMON_SKILL_VALID(SID_StanceBracing))
    [SID_StanceBracing] = GFX_SkillIcon_StanceBracing,
#endif

#if (defined(SID_StanceDarting) && COMMON_SKILL_VALID(SID_StanceDarting))
    [SID_StanceDarting] = GFX_SkillIcon_StanceDarting,
#endif

#if (defined(SID_StanceFierce) && COMMON_SKILL_VALID(SID_StanceFierce))
    [SID_StanceFierce] = GFX_SkillIcon_StanceFierce,
#endif

#if (defined(SID_StanceKestrel) && COMMON_SKILL_VALID(SID_StanceKestrel))
    [SID_StanceKestrel] = GFX_SkillIcon_StanceKestrel,
#endif

#if (defined(SID_StanceMirror) && COMMON_SKILL_VALID(SID_StanceMirror))
    [SID_StanceMirror] = GFX_SkillIcon_StanceMirror,
#endif

#if (defined(SID_StanceReady) && COMMON_SKILL_VALID(SID_StanceReady))
    [SID_StanceReady] = GFX_SkillIcon_StanceReady,
#endif

#if (defined(SID_StanceSteady) && COMMON_SKILL_VALID(SID_StanceSteady))
    [SID_StanceSteady] = GFX_SkillIcon_StanceSteady,
#endif

#if (defined(SID_StanceSturdy) && COMMON_SKILL_VALID(SID_StanceSturdy))
    [SID_StanceSturdy] = GFX_SkillIcon_StanceSturdy,
#endif

#if (defined(SID_StanceSwift) && COMMON_SKILL_VALID(SID_StanceSwift))
    [SID_StanceSwift] = GFX_SkillIcon_StanceSwift,
#endif

#if (defined(SID_StanceWarding) && COMMON_SKILL_VALID(SID_StanceWarding))
    [SID_StanceWarding] = GFX_SkillIcon_StanceWarding,
#endif

#if (defined(SID_RangeBonusBMag1) && COMMON_SKILL_VALID(SID_RangeBonusBMag1))
    [SID_RangeBonusBMag1] = GFX_SkillIcon_RangeBonusBMag1,
#endif

#if (defined(SID_RangeBonusBMag2) && COMMON_SKILL_VALID(SID_RangeBonusBMag2))
    [SID_RangeBonusBMag2] = GFX_SkillIcon_RangeBonusBMag2,
#endif

#if (defined(SID_RangeBonusBow1) && COMMON_SKILL_VALID(SID_RangeBonusBow1))
    [SID_RangeBonusBow1] = GFX_SkillIcon_RangeBonusBow1,
#endif

#if (defined(SID_RangeBonusBow2) && COMMON_SKILL_VALID(SID_RangeBonusBow2))
    [SID_RangeBonusBow2] = GFX_SkillIcon_RangeBonusBow2,
#endif

#if (defined(SID_RunningStart) && COMMON_SKILL_VALID(SID_RunningStart))
    [SID_RunningStart] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Canto) && COMMON_SKILL_VALID(SID_Canto))
    [SID_Canto] = GFX_SkillIcon_Canto,
#endif

#if (defined(SID_CantoPlus) && COMMON_SKILL_VALID(SID_CantoPlus))
    [SID_CantoPlus] = GFX_SkillIcon_CantoPlus,
#endif

#if (defined(SID_AlertStance) && COMMON_SKILL_VALID(SID_AlertStance))
    [SID_AlertStance] = GFX_SkillIcon_AlertStance,
#endif

#if (defined(SID_AlertStancePlus) && COMMON_SKILL_VALID(SID_AlertStancePlus))
    [SID_AlertStancePlus] = GFX_SkillIcon_AlertStancePlus,
#endif

#if (defined(SID_Galeforce) && COMMON_SKILL_VALID(SID_Galeforce))
    [SID_Galeforce] = GFX_SkillIcon_Galeforce,
#endif

#if (defined(SID_COMBAT_Galeforce) && COMMON_SKILL_VALID(SID_COMBAT_Galeforce))
    [SID_COMBAT_Galeforce] = GFX_SkillIcon_Galeforce,
#endif

#if (defined(SID_SavageBlow) && COMMON_SKILL_VALID(SID_SavageBlow))
    [SID_SavageBlow] = GFX_SkillIcon_SavageBlow,
#endif

#if (defined(SID_BreathOfLife) && COMMON_SKILL_VALID(SID_BreathOfLife))
    [SID_BreathOfLife] = GFX_SkillIcon_BreathOfLife,
#endif

#if (defined(SID_Thunderstorm) && COMMON_SKILL_VALID(SID_Thunderstorm))
    [SID_Thunderstorm] = GFX_SkillIcon_Thunderstorm,
#endif

#if (defined(SID_PosReturn) && COMMON_SKILL_VALID(SID_PosReturn))
    [SID_PosReturn] = GFX_SkillIcon_PosReturn,
#endif

#if (defined(SID_Lunge) && COMMON_SKILL_VALID(SID_Lunge))
    [SID_Lunge] = GFX_SkillIcon_Lunge,
#endif

#if (defined(SID_Renewal) && COMMON_SKILL_VALID(SID_Renewal))
    [SID_Renewal] = GFX_SkillIcon_Renewal,
#endif

#if (defined(SID_Imbue) && COMMON_SKILL_VALID(SID_Imbue))
    [SID_Imbue] = GFX_SkillIcon_Imbue,
#endif

#if (defined(SID_Forager) && COMMON_SKILL_VALID(SID_Forager))
    [SID_Forager] = GFX_SkillIcon_Forager,
#endif

#if (defined(SID_Anathema) && COMMON_SKILL_VALID(SID_Anathema))
    [SID_Anathema] = GFX_SkillIcon_Anathema,
#endif

#if (defined(SID_Bond) && COMMON_SKILL_VALID(SID_Bond))
    [SID_Bond] = GFX_SkillIcon_Bond,
#endif

#if (defined(SID_Charm) && COMMON_SKILL_VALID(SID_Charm))
    [SID_Charm] = GFX_SkillIcon_Charm,
#endif

#if (defined(SID_Intimidate) && COMMON_SKILL_VALID(SID_Intimidate))
    [SID_Intimidate] = GFX_SkillIcon_Intimidate,
#endif

#if (defined(SID_Tantivy) && COMMON_SKILL_VALID(SID_Tantivy))
    [SID_Tantivy] = GFX_SkillIcon_Tantivy,
#endif

#if (defined(SID_Focus) && COMMON_SKILL_VALID(SID_Focus))
    [SID_Focus] = GFX_SkillIcon_Focus,
#endif

#if (defined(SID_Hex) && COMMON_SKILL_VALID(SID_Hex))
    [SID_Hex] = GFX_SkillIcon_Hex,
#endif

#if (defined(SID_Infiltrator) && COMMON_SKILL_VALID(SID_Infiltrator))
    [SID_Infiltrator] = GFX_SkillIcon_Infiltrator,
#endif

#if (defined(SID_Inspiration) && COMMON_SKILL_VALID(SID_Inspiration))
    [SID_Inspiration] = GFX_SkillIcon_Inspiration,
#endif

#if (defined(SID_DivinelyInspiring) && COMMON_SKILL_VALID(SID_DivinelyInspiring))
    [SID_DivinelyInspiring] = GFX_SkillIcon_DivinelyInspiring,
#endif

#if (defined(SID_VoiceOfPeace) && COMMON_SKILL_VALID(SID_VoiceOfPeace))
    [SID_VoiceOfPeace] = GFX_SkillIcon_VoiceOfPeace,
#endif

#if (defined(SID_PowHone) && COMMON_SKILL_VALID(SID_PowHone))
    [SID_PowHone] = GFX_SkillIcon_HonePow,
#endif

#if (defined(SID_MagHone) && COMMON_SKILL_VALID(SID_MagHone))
    [SID_MagHone] = GFX_SkillIcon_HoneMag,
#endif

#if (defined(SID_SklHone) && COMMON_SKILL_VALID(SID_SklHone))
    [SID_SklHone] = GFX_SkillIcon_HoneSkl,
#endif

#if (defined(SID_SpdHone) && COMMON_SKILL_VALID(SID_SpdHone))
    [SID_SpdHone] = GFX_SkillIcon_HoneSpd,
#endif

#if (defined(SID_LckHone) && COMMON_SKILL_VALID(SID_LckHone))
    [SID_LckHone] = GFX_SkillIcon_HoneLuk,
#endif

#if (defined(SID_DefHone) && COMMON_SKILL_VALID(SID_DefHone))
    [SID_DefHone] = GFX_SkillIcon_HoneDef,
#endif

#if (defined(SID_ResHone) && COMMON_SKILL_VALID(SID_ResHone))
    [SID_ResHone] = GFX_SkillIcon_HoneRes,
#endif

#if (defined(SID_MovHone) && COMMON_SKILL_VALID(SID_MovHone))
    [SID_MovHone] = GFX_SkillIcon_HoneMov,
#endif

#if (defined(SID_BattleRange_Todo1) && COMMON_SKILL_VALID(SID_BattleRange_Todo1))
    [SID_BattleRange_Todo1] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_BattleRange_Todo2) && COMMON_SKILL_VALID(SID_BattleRange_Todo2))
    [SID_BattleRange_Todo2] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_BattleRange_Todo3) && COMMON_SKILL_VALID(SID_BattleRange_Todo3))
    [SID_BattleRange_Todo3] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Steal) && COMMON_SKILL_VALID(SID_Steal))
    [SID_Steal] = GFX_SkillIcon_Steal,
#endif

#if (defined(SID_HealingFocus) && COMMON_SKILL_VALID(SID_HealingFocus))
    [SID_HealingFocus] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Blossom) && COMMON_SKILL_VALID(SID_Blossom))
    [SID_Blossom] = GFX_SkillIcon_Blossom,
#endif

#if (defined(SID_Paragon) && COMMON_SKILL_VALID(SID_Paragon))
    [SID_Paragon] = GFX_SkillIcon_Paragon,
#endif

#if (defined(SID_VoidCurse) && COMMON_SKILL_VALID(SID_VoidCurse))
    [SID_VoidCurse] = GFX_SkillIcon_VoidCurse,
#endif

#if (defined(SID_Aptitude) && COMMON_SKILL_VALID(SID_Aptitude))
    [SID_Aptitude] = GFX_SkillIcon_Aptitude,
#endif

#if (defined(SID_Peacebringer) && COMMON_SKILL_VALID(SID_VoiceOfPeace))
    [SID_Peacebringer] = GFX_SkillIcon_Peacebringer,
#endif

#if (defined(SID_LEGEND_InoriAtk) && COMMON_SKILL_VALID(SID_LEGEND_InoriAtk))
    [SID_LEGEND_InoriAtk] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_LEGEND_InoriAvo) && COMMON_SKILL_VALID(SID_LEGEND_InoriAvo))
    [SID_LEGEND_InoriAvo] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_LEGEND_InoriDef) && COMMON_SKILL_VALID(SID_LEGEND_InoriDef))
    [SID_LEGEND_InoriDef] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_LEGEND_4) && COMMON_SKILL_VALID(SID_LEGEND_4))
    [SID_LEGEND_4] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_LEGEND_5) && COMMON_SKILL_VALID(SID_LEGEND_5))
    [SID_LEGEND_5] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Frenzy) && COMMON_SKILL_VALID(SID_Frenzy))
    [SID_Frenzy] = GFX_SkillIcon_Frenzy,
#endif

#if (defined(SID_WonderGuard) && COMMON_SKILL_VALID(SID_WonderGuard))
    [SID_WonderGuard] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_PoisonPoint) && COMMON_SKILL_VALID(SID_PoisonPoint))
    [SID_PoisonPoint] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Merciless) && COMMON_SKILL_VALID(SID_Merciless))
    [SID_Merciless] = GFX_SkillIcon_Merciless,
#endif

#if (defined(SID_CriticalPierce) && COMMON_SKILL_VALID(SID_CriticalPierce))
    [SID_CriticalPierce] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_KillingMachine) && COMMON_SKILL_VALID(SID_KillingMachine))
    [SID_KillingMachine] = GFX_SkillIcon_KillingMachine,
#endif

#if (defined(SID_HeavyStrikes) && COMMON_SKILL_VALID(SID_HeavyStrikes))
    [SID_HeavyStrikes] = GFX_SkillIcon_HeavyStrikes,
#endif

#if (defined(SID_QuickBurn) && COMMON_SKILL_VALID(SID_QuickBurn))
    [SID_QuickBurn] = GFX_SkillIcon_QuickBurn,
#endif

#if (defined(SID_SlowBurn) && COMMON_SKILL_VALID(SID_SlowBurn))
    [SID_SlowBurn] = GFX_SkillIcon_SlowBurn,
#endif

#if (defined(SID_StanceSpectrum) && COMMON_SKILL_VALID(SID_StanceSpectrum))
    [SID_StanceSpectrum] = GFX_SkillIcon_StanceSpectrum,
#endif

#if (defined(SID_Technician) && COMMON_SKILL_VALID(SID_Technician))
    [SID_Technician] = GFX_SkillIcon_Technician,
#endif

#if (defined(SID_CatchingUp) && COMMON_SKILL_VALID(SID_CatchingUp))
    [SID_CatchingUp] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_BloodTide) && COMMON_SKILL_VALID(SID_BloodTide))
    [SID_BloodTide] = GFX_SkillIcon_BloodTide,
#endif

#if (defined(SID_WhitePool) && COMMON_SKILL_VALID(SID_WhitePool))
    [SID_WhitePool] = GFX_SkillIcon_WhitePool,
#endif

#if (defined(SID_NightTide) && COMMON_SKILL_VALID(SID_NightTide))
    [SID_NightTide] = GFX_SkillIcon_NightTide,
#endif

#if (defined(SID_DriveStr) && COMMON_SKILL_VALID(SID_DriveStr))
    [SID_DriveStr] = GFX_SkillIcon_DriveStr,
#endif

#if (defined(SID_DriveMag) && COMMON_SKILL_VALID(SID_DriveMag))
    [SID_DriveMag] = GFX_SkillIcon_DriveMag,
#endif

#if (defined(SID_DriveSpd) && COMMON_SKILL_VALID(SID_DriveSpd))
    [SID_DriveSpd] = GFX_SkillIcon_DriveSpd,
#endif

#if (defined(SID_DriveDef) && COMMON_SKILL_VALID(SID_DriveDef))
    [SID_DriveDef] = GFX_SkillIcon_DriveDef,
#endif

#if (defined(SID_DriveRes) && COMMON_SKILL_VALID(SID_DriveRes))
    [SID_DriveRes] = GFX_SkillIcon_DriveRes,
#endif

#if (defined(SID_Charisma) && COMMON_SKILL_VALID(SID_Charisma))
    [SID_Charisma] = GFX_SkillIcon_Charisma,
#endif

#if (defined(SID_LilysPoise) && COMMON_SKILL_VALID(SID_LilysPoise))
    [SID_LilysPoise] = GFX_SkillIcon_LilysPoise,
#endif

#if (defined(SID_Daunt) && COMMON_SKILL_VALID(SID_Daunt))
    [SID_Daunt] = GFX_SkillIcon_Daunt,
#endif

#if (defined(SID_SpurStr) && COMMON_SKILL_VALID(SID_SpurStr))
    [SID_SpurStr] = GFX_SkillIcon_SpurStr,
#endif

#if (defined(SID_SpurMag) && COMMON_SKILL_VALID(SID_SpurMag))
    [SID_SpurMag] = GFX_SkillIcon_SpurMag,
#endif

#if (defined(SID_SpurDef) && COMMON_SKILL_VALID(SID_SpurDef))
    [SID_SpurDef] = GFX_SkillIcon_SpurDef,
#endif

#if (defined(SID_SpurRes) && COMMON_SKILL_VALID(SID_SpurRes))
    [SID_SpurRes] = GFX_SkillIcon_SpurRes,
#endif

#if (defined(SID_SpurSpd) && COMMON_SKILL_VALID(SID_SpurSpd))
    [SID_SpurSpd] = GFX_SkillIcon_SpurSpd,
#endif

#if (defined(SID_BattleVeteran) && COMMON_SKILL_VALID(SID_BattleVeteran))
    [SID_BattleVeteran] = GFX_SkillIcon_BattleVeteran,
#endif

#if (defined(SID_Gentilhomme) && COMMON_SKILL_VALID(SID_Gentilhomme))
    [SID_Gentilhomme] = GFX_SkillIcon_Gentilhomme,
#endif

#if (defined(SID_Demoiselle) && COMMON_SKILL_VALID(SID_Demoiselle))
    [SID_Demoiselle] = GFX_SkillIcon_Demoiselle,
#endif

#if (defined(SID_MaleficAura) && (SID_MaleficAura < MAX_SKILL_NUM))
    [SID_MaleficAura] = GFX_SkillIcon_MaleficAura,
#endif

#if (defined(SID_Solidarity) && COMMON_SKILL_VALID(SID_Solidarity))
    [SID_Solidarity] = GFX_SkillIcon_Solidarity,
#endif

#if (defined(SID_StoneBody) && COMMON_SKILL_VALID(SID_StoneBody))
    [SID_StoneBody] = GFX_SkillIcon_StoneBody,
#endif

#if (defined(SID_Chivalry) && COMMON_SKILL_VALID(SID_Chivalry))
    [SID_Chivalry] = GFX_SkillIcon_Chivalry,
#endif

#if (defined(SID_Pragmatic) && COMMON_SKILL_VALID(SID_Pragmatic))
    [SID_Pragmatic] = GFX_SkillIcon_Pragmatic,
#endif

#if (defined(SID_WindDisciple) && COMMON_SKILL_VALID(SID_WindDisciple))
    [SID_WindDisciple] = GFX_SkillIcon_WindDisciple,
#endif

#if (defined(SID_Perfectionist) && COMMON_SKILL_VALID(SID_Perfectionist))
    [SID_Perfectionist] = GFX_SkillIcon_Perfectionist,
#endif

#if (defined(SID_CriticalForce) && COMMON_SKILL_VALID(SID_CriticalForce))
    [SID_CriticalForce] = GFX_SkillIcon_CriticalForce,
#endif

#if (defined(SID_StrongRiposte) && COMMON_SKILL_VALID(SID_StrongRiposte))
    [SID_StrongRiposte] = GFX_SkillIcon_StrongRiposte,
#endif

#if (defined(SID_Patience) && COMMON_SKILL_VALID(SID_Patience))
    [SID_Patience] = GFX_SkillIcon_Patience,
#endif

#if (defined(SID_Pursuit) && COMMON_SKILL_VALID(SID_Pursuit))
    [SID_Pursuit] = GFX_SkillIcon_Pursuit,
#endif

#if (defined(SID_DragonSkin) && COMMON_SKILL_VALID(SID_DragonSkin))
    [SID_DragonSkin] = GFX_SkillIcon_DragonSkin,
#endif

#if (defined(SID_Hawkeye) && COMMON_SKILL_VALID(SID_Hawkeye))
    [SID_Hawkeye] = GFX_SkillIcon_Hawkeye,
#endif

#if (defined(SID_MagicEye) && COMMON_SKILL_VALID(SID_MagicEye))
    [SID_MagicEye] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_KeenFighter) && COMMON_SKILL_VALID(SID_KeenFighter))
    [SID_KeenFighter] = GFX_SkillIcon_KeenFighter,
#endif

#if (defined(SID_FireBoost) && COMMON_SKILL_VALID(SID_FireBoost))
    [SID_FireBoost] = GFX_SkillIcon_FireBoost,
#endif

#if (defined(SID_WindBoost) && COMMON_SKILL_VALID(SID_WindBoost))
    [SID_WindBoost] = GFX_SkillIcon_WindBoost,
#endif

#if (defined(SID_EarthBoost) && COMMON_SKILL_VALID(SID_EarthBoost))
    [SID_EarthBoost] = GFX_SkillIcon_EarthBoost,
#endif

#if (defined(SID_WaterBoost) && COMMON_SKILL_VALID(SID_WaterBoost))
    [SID_WaterBoost] = GFX_SkillIcon_WaterBoost,
#endif

#if (defined(SID_FieryBlood) && COMMON_SKILL_VALID(SID_FieryBlood))
    [SID_FieryBlood] = GFX_SkillIcon_FieryBlood,
#endif

#if (defined(SID_Colossus) && COMMON_SKILL_VALID(SID_Colossus))
    [SID_Colossus] = GFX_SkillIcon_Colossus,
#endif

#if (defined(SID_LightWeight) && COMMON_SKILL_VALID(SID_LightWeight))
    [SID_LightWeight] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_EvenFooted) && COMMON_SKILL_VALID(SID_EvenFooted))
    [SID_EvenFooted] = GFX_SkillIcon_EvenFooted,
#endif

#if (defined(SID_QuickDraw) && COMMON_SKILL_VALID(SID_QuickDraw))
    [SID_QuickDraw] = GFX_SkillIcon_QuickDraw,
#endif

#if (defined(SID_ArcaneBlade) && COMMON_SKILL_VALID(SID_ArcaneBlade))
    [SID_ArcaneBlade] = GFX_SkillIcon_ArcaneBlade,
#endif

#if (defined(SID_ElbowRoom) && COMMON_SKILL_VALID(SID_ElbowRoom))
    [SID_ElbowRoom] = GFX_SkillIcon_ElbowRoom,
#endif

#if (defined(SID_NaturalCover) && COMMON_SKILL_VALID(SID_NaturalCover))
    [SID_NaturalCover] = GFX_SkillIcon_NaturalCover,
#endif

#if (defined(SID_ChaosStyle) && COMMON_SKILL_VALID(SID_ChaosStyle))
    [SID_ChaosStyle] = GFX_SkillIcon_ChaosStyle,
#endif

#if (defined(SID_PushDefense) && COMMON_SKILL_VALID(SID_PushDefense))
    [SID_PushDefense] = GFX_SkillIcon_PushDefense,
#endif

#if (defined(SID_PushMagic) && COMMON_SKILL_VALID(SID_PushMagic))
    [SID_PushMagic] = GFX_SkillIcon_PushMagic,
#endif

#if (defined(SID_PushResistance) && COMMON_SKILL_VALID(SID_PushResistance))
    [SID_PushResistance] = GFX_SkillIcon_PushResistance,
#endif

#if (defined(SID_PushSkill) && COMMON_SKILL_VALID(SID_PushSkill))
    [SID_PushSkill] = GFX_SkillIcon_PushSkill,
#endif

#if (defined(SID_PushSpeed) && COMMON_SKILL_VALID(SID_PushSpeed))
    [SID_PushSpeed] = GFX_SkillIcon_PushSpeed,
#endif

#if (defined(SID_PushStrength) && COMMON_SKILL_VALID(SID_PushStrength))
    [SID_PushStrength] = GFX_SkillIcon_PushStrength,
#endif

#if (defined(SID_PushLuck) && COMMON_SKILL_VALID(SID_PushLuck))
    [SID_PushLuck] = GFX_SkillIcon_PushLuck,
#endif

#if (defined(SID_PushMovement) && COMMON_SKILL_VALID(SID_PushMovement))
    [SID_PushMovement] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_PushSpectrum) && COMMON_SKILL_VALID(SID_PushSpectrum))
    [SID_PushSpectrum] = GFX_SkillIcon_PushSpectrum,
#endif

#if (defined(SID_Charge) && COMMON_SKILL_VALID(SID_Charge))
    [SID_Charge] = GFX_SkillIcon_Charge,
#endif

#if (defined(SID_ChargePlus) && COMMON_SKILL_VALID(SID_ChargePlus))
    [SID_ChargePlus] = GFX_SkillIcon_ChargePlus,
#endif

#if (defined(SID_Wrath) && COMMON_SKILL_VALID(SID_Wrath))
    [SID_Wrath] = GFX_SkillIcon_Wrath,
#endif

#if (defined(SID_Vigilance) && COMMON_SKILL_VALID(SID_Vigilance))
    [SID_Vigilance] = GFX_SkillIcon_Vigilance,
#endif

#if (defined(SID_OutdoorFighter) && COMMON_SKILL_VALID(SID_OutdoorFighter))
    [SID_OutdoorFighter] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_DancingBlade) && COMMON_SKILL_VALID(SID_DancingBlade))
    [SID_DancingBlade] = GFX_SkillIcon_DancingBlade,
#endif

#if (defined(SID_KnightAspirant) && COMMON_SKILL_VALID(SID_KnightAspirant))
    [SID_KnightAspirant] = GFX_SkillIcon_KnightAspirant,
#endif

#if (defined(SID_Outrider) && COMMON_SKILL_VALID(SID_Outrider))
    [SID_Outrider] = GFX_SkillIcon_Outrider,
#endif

#if (defined(SID_EvenRhythm) && COMMON_SKILL_VALID(SID_EvenRhythm))
    [SID_EvenRhythm] = GFX_SkillIcon_EvenRhythm,
#endif

#if (defined(SID_OddRhythm) && COMMON_SKILL_VALID(SID_OddRhythm))
    [SID_OddRhythm] = GFX_SkillIcon_OddRhythm,
#endif

#if (defined(SID_OddFooted) && COMMON_SKILL_VALID(SID_OddFooted))
    [SID_OddFooted] = GFX_SkillIcon_OddFooted,
#endif

#if (defined(SID_Corrosion) && COMMON_SKILL_VALID(SID_Corrosion))
    [SID_Corrosion] = GFX_SkillIcon_Corrosion,
#endif

#if (defined(SID_NoGuard) && COMMON_SKILL_VALID(SID_NoGuard))
    [SID_NoGuard] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Puissance) && COMMON_SKILL_VALID(SID_Puissance))
    [SID_Puissance] = GFX_SkillIcon_Puissance,
#endif

#if (defined(SID_Prescience) && COMMON_SKILL_VALID(SID_Prescience))
    [SID_Prescience] = GFX_SkillIcon_Prescience,
#endif

#if (defined(SID_Resolve) && COMMON_SKILL_VALID(SID_Resolve))
    [SID_Resolve] = GFX_SkillIcon_Resolve,
#endif

#if (defined(SID_SilentPride) && COMMON_SKILL_VALID(SID_SilentPride))
    [SID_SilentPride] = GFX_SkillIcon_SilentPride,
#endif

#if (defined(SID_RightfulKing) && COMMON_SKILL_VALID(SID_RightfulKing))
    [SID_RightfulKing] = GFX_SkillIcon_RightfulKing,
#endif

#if (defined(SID_RightfulGod) && COMMON_SKILL_VALID(SID_RightfulGod))
    [SID_RightfulGod] = GFX_SkillIcon_RightfulGod,
#endif

#if (defined(SID_RightfulArch) && COMMON_SKILL_VALID(SID_RightfulArch))
    [SID_RightfulArch] = GFX_SkillIcon_RightfulArch,
#endif

#if (defined(SID_Guts) && COMMON_SKILL_VALID(SID_Guts))
    [SID_Guts] = GFX_SkillIcon_Guts,
#endif

#if (defined(SID_StrongCon) && COMMON_SKILL_VALID(SID_StrongCon))
    [SID_StrongCon] = GFX_SkillIcon_StrongCon,
#endif

#if (defined(SID_Impale) && COMMON_SKILL_VALID(SID_Impale))
    [SID_Impale] = GFX_SkillIcon_Impale,
#endif

#if (defined(SID_Hero) && COMMON_SKILL_VALID(SID_Hero))
    [SID_Hero] = GFX_SkillIcon_Hero,
#endif

#if (defined(SID_Ignis) && COMMON_SKILL_VALID(SID_Ignis))
    [SID_Ignis] = GFX_SkillIcon_Ignis,
#endif

#if (defined(SID_HolyAura) && COMMON_SKILL_VALID(SID_HolyAura))
    [SID_HolyAura] = GFX_SkillIcon_HolyAura,
#endif

#if (defined(SID_Loyalty) && COMMON_SKILL_VALID(SID_Loyalty))
    [SID_Loyalty] = GFX_SkillIcon_Loyalty,
#endif

#if (defined(SID_TowerShield) && COMMON_SKILL_VALID(SID_TowerShield))
    [SID_TowerShield] = GFX_SkillIcon_TowerShield,
#endif

#if (defined(SID_StunningSmile) && COMMON_SKILL_VALID(SID_StunningSmile))
    [SID_StunningSmile] = GFX_SkillIcon_StunningSmile,
#endif

#if (defined(SID_Trample) && COMMON_SKILL_VALID(SID_Trample))
    [SID_Trample] = GFX_SkillIcon_Trample,
#endif

#if (defined(SID_Opportunist) && COMMON_SKILL_VALID(SID_Opportunist))
    [SID_Opportunist] = GFX_SkillIcon_Opportunist,
#endif

#if (defined(SID_SuperLuck) && COMMON_SKILL_VALID(SID_SuperLuck))
    [SID_SuperLuck] = GFX_SkillIcon_SuperLuck,
#endif

#if (defined(SID_ShortShield) && COMMON_SKILL_VALID(SID_ShortShield))
    [SID_ShortShield] = GFX_SkillIcon_ShortShield,
#endif

#if (defined(SID_Vanity) && COMMON_SKILL_VALID(SID_Vanity))
    [SID_Vanity] = GFX_SkillIcon_Vanity,
#endif

#if (defined(SID_DefenseBoost) && COMMON_SKILL_VALID(SID_DefenseBoost))
    [SID_DefenseBoost] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_LuckBoost) && COMMON_SKILL_VALID(SID_LuckBoost))
    [SID_LuckBoost] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_MagicBoost) && COMMON_SKILL_VALID(SID_MagicBoost))
    [SID_MagicBoost] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_MoveBoost) && COMMON_SKILL_VALID(SID_MoveBoost))
    [SID_MoveBoost] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_ResistanceBoost) && COMMON_SKILL_VALID(SID_ResistanceBoost))
    [SID_ResistanceBoost] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SkillBoost) && COMMON_SKILL_VALID(SID_SkillBoost))
    [SID_SkillBoost] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SpeedBoost) && COMMON_SKILL_VALID(SID_SpeedBoost))
    [SID_SpeedBoost] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_StrengthBoost) && COMMON_SKILL_VALID(SID_StrengthBoost))
    [SID_StrengthBoost] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SpectrumBoost) && COMMON_SKILL_VALID(SID_SpectrumBoost))
    [SID_SpectrumBoost] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Glacies) && COMMON_SKILL_VALID(SID_Glacies))
    [SID_Glacies] = GFX_SkillIcon_Glacies,
#endif

#if (defined(SID_Vengeance) && COMMON_SKILL_VALID(SID_Vengeance))
    [SID_Vengeance] = GFX_SkillIcon_Vengeance,
#endif

#if (defined(SID_Deadeye) && COMMON_SKILL_VALID(SID_Deadeye))
    [SID_Deadeye] = GFX_SkillIcon_Deadeye,
#endif

#if (defined(SID_AxeFaith) && COMMON_SKILL_VALID(SID_AxeFaith))
    [SID_AxeFaith] = GFX_SkillIcon_AxeFaith,
#endif

#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
    [SID_RuinedBladePlus] = GFX_SkillIcon_RuinedBlade,
#endif

#if (defined(SID_COMBAT_Grounder) && COMMON_SKILL_VALID(SID_COMBAT_Grounder))
    [SID_COMBAT_Grounder] = GFX_CombatArtIcon_SwordArtAtk,
#endif

#if (defined(SID_COMBAT_BaneOfMonsters) && COMMON_SKILL_VALID(SID_COMBAT_BaneOfMonsters))
    [SID_COMBAT_BaneOfMonsters] = GFX_CombatArtIcon_SwordArtAtk,
#endif

#if (defined(SID_COMBAT_Hexblade) && COMMON_SKILL_VALID(SID_COMBAT_Hexblade))
    [SID_COMBAT_Hexblade] = GFX_CombatArtIcon_SwordArtMag,
#endif

#if (defined(SID_COMBAT_FoudroyantStrike) && COMMON_SKILL_VALID(SID_COMBAT_FoudroyantStrike))
    [SID_COMBAT_FoudroyantStrike] = GFX_CombatArtIcon_SwordArtCrit,
#endif

#if (defined(SID_COMBAT_TempestLance) && COMMON_SKILL_VALID(SID_COMBAT_TempestLance))
    [SID_COMBAT_TempestLance] = GFX_CombatArtIcon_LanceArtAtk,
#endif

#if (defined(SID_COMBAT_SwiftStrikes) && COMMON_SKILL_VALID(SID_COMBAT_SwiftStrikes))
    [SID_COMBAT_SwiftStrikes] = GFX_CombatArtIcon_LanceArtAtk,
#endif

#if (defined(SID_COMBAT_FrozenLance) && COMMON_SKILL_VALID(SID_COMBAT_FrozenLance))
    [SID_COMBAT_FrozenLance] = GFX_CombatArtIcon_LanceArtMag,
#endif

#if (defined(SID_COMBAT_Vengeance) && COMMON_SKILL_VALID(SID_COMBAT_Vengeance))
    [SID_COMBAT_Vengeance] = GFX_CombatArtIcon_LanceArtCrit,
#endif

#if (defined(SID_COMBAT_HelmSplitter) && COMMON_SKILL_VALID(SID_COMBAT_HelmSplitter))
    [SID_COMBAT_HelmSplitter] = GFX_CombatArtIcon_AxeArtAtk,
#endif

#if (defined(SID_COMBAT_LightningAxe) && COMMON_SKILL_VALID(SID_COMBAT_LightningAxe))
    [SID_COMBAT_LightningAxe] = GFX_CombatArtIcon_AxeArtMag,
#endif

#if (defined(SID_COMBAT_WarMasterStrike) && COMMON_SKILL_VALID(SID_COMBAT_WarMasterStrike))
    [SID_COMBAT_WarMasterStrike] = GFX_CombatArtIcon_AxeArtCrit,
#endif

#if (defined(SID_COMBAT_CurvedShot) && COMMON_SKILL_VALID(SID_COMBAT_CurvedShot))
    [SID_COMBAT_CurvedShot] = GFX_CombatArtIcon_BowArtAtk,
#endif

#if (defined(SID_COMBAT_Deadeye) && COMMON_SKILL_VALID(SID_COMBAT_Deadeye))
    [SID_COMBAT_Deadeye] = GFX_CombatArtIcon_BowArtAtk,
#endif

#if (defined(SID_COMBAT_Encloser) && COMMON_SKILL_VALID(SID_COMBAT_Encloser))
    [SID_COMBAT_Encloser] = GFX_CombatArtIcon_BowArtDebuff,
#endif

#if (defined(SID_COMBAT_EncloserPlus) && COMMON_SKILL_VALID(SID_COMBAT_EncloserPlus))
    [SID_COMBAT_EncloserPlus] = GFX_CombatArtIcon_BowArtDebuff,
#endif

#if (defined(SID_COMBAT_MonsterBlast) && COMMON_SKILL_VALID(SID_COMBAT_MonsterBlast))
    [SID_COMBAT_MonsterBlast] = GFX_CombatArtIcon_BowArtCrit,
#endif

#if (defined(SID_COMBAT_SchismShot) && COMMON_SKILL_VALID(SID_COMBAT_SchismShot))
    [SID_COMBAT_SchismShot] = GFX_CombatArtIcon_BowArtCrit,
#endif

#if (defined(SID_COMBAT_BreakShot) && COMMON_SKILL_VALID(SID_COMBAT_BreakShot))
    [SID_COMBAT_BreakShot] = GFX_CombatArtIcon_BowArtDebuff,
#endif

#if (defined(SID_COMBAT_BreakShotPlus) && COMMON_SKILL_VALID(SID_COMBAT_BreakShotPlus))
    [SID_COMBAT_BreakShotPlus] = GFX_CombatArtIcon_BowArtDebuff,
#endif

#if (defined(SID_COMBAT_WaningShot) && COMMON_SKILL_VALID(SID_COMBAT_WaningShot))
    [SID_COMBAT_WaningShot] = GFX_CombatArtIcon_BowArtDebuff,
#endif

#if (defined(SID_COMBAT_WaningShotPlus) && COMMON_SKILL_VALID(SID_COMBAT_WaningShotPlus))
    [SID_COMBAT_WaningShotPlus] = GFX_CombatArtIcon_BowArtDebuff,
#endif

#if (defined(SID_COMBAT_HuntersVolley) && COMMON_SKILL_VALID(SID_COMBAT_HuntersVolley))
    [SID_COMBAT_HuntersVolley] = GFX_CombatArtIcon_BowArtAtk,
#endif

#if (defined(SID_GreatShield) && COMMON_SKILL_VALID(SID_GreatShield))
    [SID_GreatShield] = GFX_SkillIcon_GreatShield,
#endif

#if (defined(SID_HoneCavalry) && COMMON_SKILL_VALID(SID_HoneCavalry))
    [SID_HoneCavalry] = GFX_SkillIcon_HoneCavalry,
#endif

#if (defined(SID_HoneFlier) && COMMON_SKILL_VALID(SID_HoneFlier))
    [SID_HoneFlier] = GFX_SkillIcon_HoneFlier,
#endif

#if (defined(SID_HoneArmor) && COMMON_SKILL_VALID(SID_HoneArmor))
    [SID_HoneArmor] = GFX_SkillIcon_HoneArmor,
#endif

#if (defined(SID_FortifyArmor) && COMMON_SKILL_VALID(SID_FortifyArmor))
    [SID_FortifyArmor] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Shade) && COMMON_SKILL_VALID(SID_Shade))
    [SID_Shade] = GFX_SkillIcon_Shade,
#endif

#if (defined(SID_ShadePlus) && COMMON_SKILL_VALID(SID_ShadePlus))
    [SID_ShadePlus] = GFX_SkillIcon_ShadePlus,
#endif

#if (defined(SID_Provoke) && COMMON_SKILL_VALID(SID_Provoke))
    [SID_Provoke] = GFX_SkillIcon_Provoke,
#endif

#if (defined(SID_Teleportation) && COMMON_SKILL_VALID(SID_Teleportation))
    [SID_Teleportation] = GFX_SkillIcon_Teleportation,
#endif

#if (defined(SID_Dance) && COMMON_SKILL_VALID(SID_Dance))
    [SID_Dance] = GFX_SkillIcon_Dance,
#endif

#if (defined(SID_LockTouch) && COMMON_SKILL_VALID(SID_LockTouch))
    [SID_LockTouch] = GFX_SkillIcon_LockTouch,
#endif

#if (defined(SID_Summon) && COMMON_SKILL_VALID(SID_Summon))
    [SID_Summon] = GFX_SkillIcon_Summon,
#endif

#if (defined(SID_SureShot) && COMMON_SKILL_VALID(SID_SureShot))
    [SID_SureShot] = GFX_SkillIcon_SureShot,
#endif

#if (defined(SID_Crit) && COMMON_SKILL_VALID(SID_Crit))
    [SID_Crit] = GFX_SkillIcon_Crit,
#endif

#if (defined(SID_Lethality) && COMMON_SKILL_VALID(SID_Lethality))
    [SID_Lethality] = GFX_SkillIcon_Lethality,
#endif

#if (defined(SID_WatchfulEye) && COMMON_SKILL_VALID(SID_WatchfulEye))
    [SID_WatchfulEye] = GFX_SkillIcon_WatchfulEye,
#endif

#if (defined(SID_InfinityEdge) && COMMON_SKILL_VALID(SID_InfinityEdge))
    [SID_InfinityEdge] = GFX_SkillIcon_InfinityEdge,
#endif

#if (defined(SID_AversaNight) && COMMON_SKILL_VALID(SID_AversaNight))
    [SID_AversaNight] = GFX_SkillIcon_AversaNight,
#endif

#if (defined(SID_Supply) && COMMON_SKILL_VALID(SID_Supply))
    [SID_Supply] = GFX_SkillIcon_Supply,
#endif

#if (defined(SID_LightRune) && COMMON_SKILL_VALID(SID_LightRune))
    [SID_LightRune] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Mine) && COMMON_SKILL_VALID(SID_Mine))
    [SID_Mine] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Inori) && COMMON_SKILL_VALID(SID_Inori))
    [SID_Inori] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Armsthrift) && COMMON_SKILL_VALID(SID_Armsthrift))
    [SID_Armsthrift] = GFX_SkillIcon_Armsthrift,
#endif

#if (defined(SID_HighGround) && COMMON_SKILL_VALID(SID_HighGround))
    [SID_HighGround] = GFX_SkillIcon_HighGround,
#endif

#if (defined(SID_Airlock) && COMMON_SKILL_VALID(SID_Airlock))
    [SID_Airlock] = GFX_SkillIcon_Airlock,
#endif

#if (defined(SID_Drought) && COMMON_SKILL_VALID(SID_Drought))
    [SID_Drought] = GFX_SkillIcon_Drought,
#endif

#if (defined(SID_Drizzle) && COMMON_SKILL_VALID(SID_Drizzle))
    [SID_Drizzle] = GFX_SkillIcon_Drizzle,
#endif

#if (defined(SID_SnowWarning) && COMMON_SKILL_VALID(SID_SnowWarning))
    [SID_SnowWarning] = GFX_SkillIcon_SnowWarning,
#endif

#if (defined(SID_SandStream) && COMMON_SKILL_VALID(SID_SandStream))
    [SID_SandStream] = GFX_SkillIcon_SandStream,
#endif

#if (defined(SID_PointBlank) && COMMON_SKILL_VALID(SID_PointBlank))
    [SID_PointBlank] = GFX_SkillIcon_PointBlank,
#endif

#if (defined(SID_Synchronize) && COMMON_SKILL_VALID(SID_Synchronize))
    [SID_Synchronize] = GFX_SkillIcon_Synchronize,
#endif

#if (defined(SID_StaffSavant) && COMMON_SKILL_VALID(SID_StaffSavant))
    [SID_StaffSavant] = GFX_SkillIcon_StaffSavant,
#endif

#if (defined(SID_Skyguard) && COMMON_SKILL_VALID(SID_Skyguard))
    [SID_Skyguard] = GFX_SkillIcon_Skyguard,
#endif

#if (defined(SID_Horseguard) && COMMON_SKILL_VALID(SID_Horseguard))
    [SID_Horseguard] = GFX_SkillIcon_Horseguard,
#endif

#if (defined(SID_Armorboost) && COMMON_SKILL_VALID(SID_Armorboost))
    [SID_Armorboost] = GFX_SkillIcon_Armorboost,
#endif

#if (defined(SID_GentleFlower) && COMMON_SKILL_VALID(SID_GentleFlower))
    [SID_GentleFlower] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Admiration) && COMMON_SKILL_VALID(SID_Admiration))
    [SID_Admiration] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_FairyTaleFolk) && COMMON_SKILL_VALID(SID_FairyTaleFolk))
    [SID_FairyTaleFolk] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_InitCalm) && COMMON_SKILL_VALID(SID_InitCalm))
    [SID_InitCalm] = GFX_SkillIcon_InitCalm,
#endif

#if (defined(SID_InitClever) && COMMON_SKILL_VALID(SID_InitClever))
    [SID_InitClever] = GFX_SkillIcon_InitClever,
#endif

#if (defined(SID_InitDeft) && COMMON_SKILL_VALID(SID_InitDeft))
    [SID_InitDeft] = GFX_SkillIcon_InitDeft,
#endif

#if (defined(SID_InitLucky) && COMMON_SKILL_VALID(SID_InitLucky))
    [SID_InitLucky] = GFX_SkillIcon_InitLucky,
#endif

#if (defined(SID_InitNimble) && COMMON_SKILL_VALID(SID_InitNimble))
    [SID_InitNimble] = GFX_SkillIcon_InitNimble,
#endif

#if (defined(SID_InitQuick) && COMMON_SKILL_VALID(SID_InitQuick))
    [SID_InitQuick] = GFX_SkillIcon_InitQuick,
#endif

#if (defined(SID_InitSpectrum) && COMMON_SKILL_VALID(SID_InitSpectrum))
    [SID_InitSpectrum] = GFX_SkillIcon_InitSpectrum,
#endif

#if (defined(SID_InitStrong) && COMMON_SKILL_VALID(SID_InitStrong))
    [SID_InitStrong] = GFX_SkillIcon_InitStrong,
#endif

#if (defined(SID_InitSturdy) && COMMON_SKILL_VALID(SID_InitSturdy))
    [SID_InitSturdy] = GFX_SkillIcon_InitSturdy,
#endif

#if (defined(SID_Lifetaker) && COMMON_SKILL_VALID(SID_Lifetaker))
    [SID_Lifetaker] = GFX_SkillIcon_Lifetaker,
#endif

#if (defined(SID_Analytic) && COMMON_SKILL_VALID(SID_Analytic))
    [SID_Analytic] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_OathDefense) && COMMON_SKILL_VALID(SID_OathDefense))
    [SID_OathDefense] = GFX_SkillIcon_OathDefense,
#endif

#if (defined(SID_OathLuck) && COMMON_SKILL_VALID(SID_OathLuck))
    [SID_OathLuck] = GFX_SkillIcon_OathLuck,
#endif

#if (defined(SID_OathMagic) && COMMON_SKILL_VALID(SID_OathMagic))
    [SID_OathMagic] = GFX_SkillIcon_OathMagic,
#endif

#if (defined(SID_CounterRoar) && COMMON_SKILL_VALID(SID_CounterRoar))
    [SID_CounterRoar] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Gambit) && COMMON_SKILL_VALID(SID_Gambit))
    [SID_Gambit] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_MagicGambit) && COMMON_SKILL_VALID(SID_MagicGambit))
    [SID_MagicGambit] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_BeastAssault) && COMMON_SKILL_VALID(SID_BeastAssault))
    [SID_BeastAssault] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_LightAndDark) && COMMON_SKILL_VALID(SID_LightAndDark))
    [SID_LightAndDark] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_OathMovement) && COMMON_SKILL_VALID(SID_OathMovement))
    [SID_OathMovement] = GFX_SkillIcon_OathMovement,
#endif

#if (defined(SID_OathResistance) && COMMON_SKILL_VALID(SID_OathResistance))
    [SID_OathResistance] = GFX_SkillIcon_OathResistance,
#endif

#if (defined(SID_OathSkill) && COMMON_SKILL_VALID(SID_OathSkill))
    [SID_OathSkill] = GFX_SkillIcon_OathSkill,
#endif

#if (defined(SID_OathSpeed) && COMMON_SKILL_VALID(SID_OathSpeed))
    [SID_OathSpeed] = GFX_SkillIcon_OathSpeed,
#endif

#if (defined(SID_OathStrength) && COMMON_SKILL_VALID(SID_OathStrength))
    [SID_OathStrength] = GFX_SkillIcon_OathStrength,
#endif

#if (defined(SID_DivinePulse) && COMMON_SKILL_VALID(SID_DivinePulse))
    [SID_DivinePulse] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Tsuzuku) && COMMON_SKILL_VALID(SID_Tsuzuku))
    [SID_Tsuzuku] = GFX_SkillIcon_Tsuzuku,
#endif

#if (defined(SID_MysticBoost) && COMMON_SKILL_VALID(SID_MysticBoost))
    [SID_MysticBoost] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_GuardBearing) && COMMON_SKILL_VALID(SID_GuardBearing))
    [SID_GuardBearing] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Spurn) && COMMON_SKILL_VALID(SID_Spurn))
    [SID_Spurn] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Bushido) && COMMON_SKILL_VALID(SID_Bushido))
    [SID_Bushido] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_DragonWall) && COMMON_SKILL_VALID(SID_DragonWall))
    [SID_DragonWall] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_DragonWarth) && COMMON_SKILL_VALID(SID_DragonWarth))
    [SID_DragonWarth] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_BlueLionRule) && COMMON_SKILL_VALID(SID_BlueLionRule))
    [SID_BlueLionRule] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_FaerghusAncestry) && COMMON_SKILL_VALID(SID_FaerghusAncestry))
    [SID_FaerghusAncestry] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_CrusaderWard) && COMMON_SKILL_VALID(SID_CrusaderWard))
    [SID_CrusaderWard] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_YuneWhispers) && COMMON_SKILL_VALID(SID_YuneWhispers))
    [SID_YuneWhispers] = GFX_SkillIcon_YuneWhispers,
#endif

#if (defined(SID_Plus) && COMMON_SKILL_VALID(SID_Plus))
    [SID_Plus] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Minus) && COMMON_SKILL_VALID(SID_Minus))
    [SID_Minus] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Chlorophyll) && COMMON_SKILL_VALID(SID_Chlorophyll))
    [SID_Chlorophyll] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SlushRush) && COMMON_SKILL_VALID(SID_SlushRush))
    [SID_SlushRush] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SandRush) && COMMON_SKILL_VALID(SID_SandRush))
    [SID_SandRush] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SwiftSwim) && COMMON_SKILL_VALID(SID_SwiftSwim))
    [SID_SwiftSwim] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_RainDish) && COMMON_SKILL_VALID(SID_RainDish))
    [SID_RainDish] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Assist) && COMMON_SKILL_VALID(SID_Assist))
    [SID_Assist] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Synergism) && COMMON_SKILL_VALID(SID_Synergism))
    [SID_Synergism] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Pass) && COMMON_SKILL_VALID(SID_Pass))
    [SID_Pass] = GFX_SkillIcon_Pass,
#endif

#if (defined(SID_Obstruct) && COMMON_SKILL_VALID(SID_Obstruct))
    [SID_Obstruct] = GFX_SkillIcon_Obstruct,
#endif

#if (defined(SID_Guidance) && COMMON_SKILL_VALID(SID_Guidance))
    [SID_Guidance] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_FlierGuidance) && COMMON_SKILL_VALID(SID_FlierGuidance))
    [SID_FlierGuidance] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SoaringGuidance) && COMMON_SKILL_VALID(SID_SoaringGuidance))
    [SID_SoaringGuidance] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Aerobatics) && COMMON_SKILL_VALID(SID_Aerobatics))
    [SID_Aerobatics] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_FlierFormation) && COMMON_SKILL_VALID(SID_FlierFormation))
    [SID_FlierFormation] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SoaringWings) && COMMON_SKILL_VALID(SID_SoaringWings))
    [SID_SoaringWings] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_DetailedReport) && COMMON_SKILL_VALID(SID_DetailedReport))
    [SID_DetailedReport] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_VengefulFighter) && COMMON_SKILL_VALID(SID_VengefulFighter))
    [SID_VengefulFighter] = GFX_SkillIcon_VengefulFighter,
#endif

#if (defined(SID_RecklessFighter) && COMMON_SKILL_VALID(SID_RecklessFighter))
    [SID_RecklessFighter] = GFX_SkillIcon_RecklessFighter,
#endif

#if (defined(SID_BoldFighter) && COMMON_SKILL_VALID(SID_BoldFighter))
    [SID_BoldFighter] = GFX_SkillIcon_BoldFighter,
#endif

#if (defined(SID_Dazzle) && COMMON_SKILL_VALID(SID_Dazzle))
    [SID_Dazzle] = GFX_SkillIcon_Dazzle,
#endif

#if (defined(SID_Boon) && COMMON_SKILL_VALID(SID_Boon))
    [SID_Boon] = GFX_SkillIcon_Boon,
#endif

#if (defined(SID_BidingBlow) && COMMON_SKILL_VALID(SID_BidingBlow))
    [SID_BidingBlow] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Discipline) && COMMON_SKILL_VALID(SID_Discipline))
    [SID_Discipline] = GFX_SkillIcon_Discipline,
#endif

#if (defined(SID_AdvantageChaser) && COMMON_SKILL_VALID(SID_AdvantageChaser))
    [SID_AdvantageChaser] = GFX_SkillIcon_AdvantageChaser,
#endif

#if (defined(SID_Nonconforming) && COMMON_SKILL_VALID(SID_Nonconforming))
    [SID_Nonconforming] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Moonlight) && COMMON_SKILL_VALID(SID_Moonlight))
    [SID_Moonlight] = GFX_SkillIcon_Moonlight,
#endif

#if (defined(SID_RouseDefense) && COMMON_SKILL_VALID(SID_RouseDefense))
    [SID_RouseDefense] = GFX_SkillIcon_RouseDefense,
#endif

#if (defined(SID_RouseLuck) && COMMON_SKILL_VALID(SID_RouseLuck))
    [SID_RouseLuck] = GFX_SkillIcon_RouseLuck,
#endif

#if (defined(SID_RouseMagic) && COMMON_SKILL_VALID(SID_RouseMagic))
    [SID_RouseMagic] = GFX_SkillIcon_RouseMagic,
#endif

#if (defined(SID_RouseMovement) && COMMON_SKILL_VALID(SID_RouseMovement))
    [SID_RouseMovement] = GFX_SkillIcon_RouseMovement,
#endif

#if (defined(SID_RouseResistance) && COMMON_SKILL_VALID(SID_RouseResistance))
    [SID_RouseResistance] = GFX_SkillIcon_RouseResistance,
#endif

#if (defined(SID_RouseSkill) && COMMON_SKILL_VALID(SID_RouseSkill))
    [SID_RouseSkill] = GFX_SkillIcon_RouseSkill,
#endif

#if (defined(SID_RouseSpeed) && COMMON_SKILL_VALID(SID_RouseSpeed))
    [SID_RouseSpeed] = GFX_SkillIcon_RouseSpeed,
#endif

#if (defined(SID_RouseStrength) && COMMON_SKILL_VALID(SID_RouseStrength))
    [SID_RouseStrength] = GFX_SkillIcon_RouseStrength,
#endif

#if (defined(SID_BlackMagic) && COMMON_SKILL_VALID(SID_BlackMagic))
    [SID_BlackMagic] = GFX_SkillIcon_BlackMagic,
#endif

#if (defined(SID_Relief) && COMMON_SKILL_VALID(SID_Relief))
    [SID_Relief] = GFX_SkillIcon_Relief,
#endif

#if (defined(SID_Camaraderie) && COMMON_SKILL_VALID(SID_Camaraderie))
    [SID_Camaraderie] = GFX_SkillIcon_Camaraderie,
#endif

#if (defined(SID_Amaterasu) && (SID_Amaterasu < MAX_SKILL_NUM))
    [SID_Amaterasu] = GFX_SkillIcon_Amaterasu,
#endif

#if (defined(SID_Counter) && COMMON_SKILL_VALID(SID_Counter))
    [SID_Counter] = GFX_SkillIcon_Counter,
#endif

#if (defined(SID_LastWord) && COMMON_SKILL_VALID(SID_LastWord))
    [SID_LastWord] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_PowerStaff) && COMMON_SKILL_VALID(SID_PowerStaff))
    [SID_PowerStaff] = GFX_SkillIcon_PowerStaff,
#endif

#if (defined(SID_Multiscale) && COMMON_SKILL_VALID(SID_Multiscale))
    [SID_Multiscale] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_PridefulWarrior) && COMMON_SKILL_VALID(SID_PridefulWarrior))
    [SID_PridefulWarrior] = GFX_SkillIcon_PridefulWarrior,
#endif

#if (defined(SID_LiveToServe) && COMMON_SKILL_VALID(SID_LiveToServe))
    [SID_LiveToServe] = GFX_SkillIcon_LiveToServe,
#endif

#if (defined(SID_MageSlayer) && COMMON_SKILL_VALID(SID_MageSlayer))
    [SID_MageSlayer] = GFX_SkillIcon_MageSlayer,
#endif

#if (defined(SID_FailGale) && COMMON_SKILL_VALID(SID_FailGale))
    [SID_FailGale] = GFX_SkillIcon_FailGale,
#endif

#if (defined(SID_Clearance) && COMMON_SKILL_VALID(SID_Clearance))
    [SID_Clearance] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_TriangleAdept) && COMMON_SKILL_VALID(SID_TriangleAdept))
    [SID_TriangleAdept] = GFX_SkillIcon_TriangleAdept,
#endif

#if (defined(SID_TriangleAdeptPlus) && COMMON_SKILL_VALID(SID_TriangleAdeptPlus))
    [SID_TriangleAdeptPlus] = GFX_SkillIcon_TriangleAdeptPlus,
#endif

#if (defined(SID_TowerShieldPlus) && COMMON_SKILL_VALID(SID_TowerShieldPlus))
    [SID_TowerShieldPlus] = GFX_SkillIcon_TowerShieldPlus,
#endif

#if (defined(SID_SolarPower) && COMMON_SKILL_VALID(SID_SolarPower))
    [SID_SolarPower] = GFX_SkillIcon_SolarPower,
#endif

#if (defined(SID_COMBAT_Gamble) && COMMON_SKILL_VALID(SID_COMBAT_Gamble))
    [SID_COMBAT_Gamble] = GFX_SkillIcon_Gamble,
#endif

#if (defined(SID_PoisonStrike) && COMMON_SKILL_VALID(SID_PoisonStrike))
    [SID_PoisonStrike] = GFX_SkillIcon_PoisonStrike,
#endif

#if (defined(SID_GrislyWound) && COMMON_SKILL_VALID(SID_GrislyWound))
    [SID_GrislyWound] = GFX_SkillIcon_GrislyWound,
#endif

#if (defined(SID_Poise) && COMMON_SKILL_VALID(SID_Poise))
    [SID_Poise] = GFX_SkillIcon_Poise,
#endif

#if (defined(SID_COMBAT_Detonate) && COMMON_SKILL_VALID(SID_COMBAT_Detonate))
    [SID_COMBAT_Detonate] = GFX_SkillIcon_Detonate,
#endif

#if (defined(SID_Enrage) && COMMON_SKILL_VALID(SID_Enrage))
    [SID_Enrage] = GFX_SkillIcon_Enrage,
#endif

#if (defined(SID_Petrify) && COMMON_SKILL_VALID(SID_Petrify))
    [SID_Petrify] = GFX_SkillIcon_Petrify,
#endif

#if (defined(SID_AirRaidAttack) && COMMON_SKILL_VALID(SID_AirRaidAttack))
    [SID_AirRaidAttack] = GFX_SkillIcon_AirRaidAttack,
#endif

#if (defined(SID_AirRaidAvoid) && COMMON_SKILL_VALID(SID_AirRaidAvoid))
    [SID_AirRaidAvoid] = GFX_SkillIcon_AirRaidAvoid,
#endif

#if (defined(SID_AirRaidCrit) && COMMON_SKILL_VALID(SID_AirRaidCrit))
    [SID_AirRaidCrit] = GFX_SkillIcon_AirRaidCrit,
#endif

#if (defined(SID_AirRaidDefense) && COMMON_SKILL_VALID(SID_AirRaidDefense))
    [SID_AirRaidDefense] = GFX_SkillIcon_AirRaidDefense,
#endif

#if (defined(SID_AirRaidHit) && COMMON_SKILL_VALID(SID_AirRaidHit))
    [SID_AirRaidHit] = GFX_SkillIcon_AirRaidHit,
#endif

#if (defined(SID_AirRaidResistance) && COMMON_SKILL_VALID(SID_AirRaidResistance))
    [SID_AirRaidResistance] = GFX_SkillIcon_AirRaidResistance,
#endif

#if (defined(SID_AirRaidSpeed) && COMMON_SKILL_VALID(SID_AirRaidSpeed))
    [SID_AirRaidSpeed] = GFX_SkillIcon_AirRaidSpeed,
#endif

#if (defined(SID_FaireAnima) && COMMON_SKILL_VALID(SID_FaireAnima))
    [SID_FaireAnima] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_FaireLight) && COMMON_SKILL_VALID(SID_FaireLight))
    [SID_FaireLight] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_FaireDark) && COMMON_SKILL_VALID(SID_FaireDark))
    [SID_FaireDark] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SealDefense) && COMMON_SKILL_VALID(SID_SealDefense))
    [SID_SealDefense] = GFX_SkillIcon_SealDefense,
#endif

#if (defined(SID_SealLuck) && COMMON_SKILL_VALID(SID_SealLuck))
    [SID_SealLuck] = GFX_SkillIcon_SealLuck,
#endif

#if (defined(SID_SealMagic) && COMMON_SKILL_VALID(SID_SealMagic))
    [SID_SealMagic] = GFX_SkillIcon_SealMagic,
#endif

#if (defined(SID_SealResistance) && COMMON_SKILL_VALID(SID_SealResistance))
    [SID_SealResistance] = GFX_SkillIcon_SealResistance,
#endif

#if (defined(SID_SealSkill) && COMMON_SKILL_VALID(SID_SealSkill))
    [SID_SealSkill] = GFX_SkillIcon_SealSkill,
#endif

#if (defined(SID_SealSpeed) && COMMON_SKILL_VALID(SID_SealSpeed))
    [SID_SealSpeed] = GFX_SkillIcon_SealSpeed,
#endif

#if (defined(SID_SealStrength) && COMMON_SKILL_VALID(SID_SealStrength))
    [SID_SealStrength] = GFX_SkillIcon_SealStrength,
#endif

#if (defined(SID_RallyDefense) && COMMON_SKILL_VALID(SID_RallyDefense))
    [SID_RallyDefense] = GFX_SkillIcon_RallyDefense,
#endif

#if (defined(SID_RallyLuck) && COMMON_SKILL_VALID(SID_RallyLuck))
    [SID_RallyLuck] = GFX_SkillIcon_RallyLuck,
#endif

#if (defined(SID_RallyMagic) && COMMON_SKILL_VALID(SID_RallyMagic))
    [SID_RallyMagic] = GFX_SkillIcon_RallyMagic,
#endif

#if (defined(SID_RallyMovement) && COMMON_SKILL_VALID(SID_RallyMovement))
    [SID_RallyMovement] = GFX_SkillIcon_RallyMovement,
#endif

#if (defined(SID_RallyResistance) && COMMON_SKILL_VALID(SID_RallyResistance))
    [SID_RallyResistance] = GFX_SkillIcon_RallyResistance,
#endif

#if (defined(SID_RallySkill) && COMMON_SKILL_VALID(SID_RallySkill))
    [SID_RallySkill] = GFX_SkillIcon_RallySkill,
#endif

#if (defined(SID_RallySpeed) && COMMON_SKILL_VALID(SID_RallySpeed))
    [SID_RallySpeed] = GFX_SkillIcon_RallySpeed,
#endif

#if (defined(SID_RallyStrength) && COMMON_SKILL_VALID(SID_RallyStrength))
    [SID_RallyStrength] = GFX_SkillIcon_RallyStrength,
#endif

#if (defined(SID_RallySpectrum) && COMMON_SKILL_VALID(SID_RallySpectrum))
    [SID_RallySpectrum] = GFX_SkillIcon_RallySpectrum,
#endif

#if (defined(SID_Despoil) && COMMON_SKILL_VALID(SID_Despoil))
    [SID_Despoil] = GFX_SkillIcon_Despoil,
#endif

#if (defined(SID_Pickup) && COMMON_SKILL_VALID(SID_Pickup))
    [SID_Pickup] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_IndoorMarch) && COMMON_SKILL_VALID(SID_IndoorMarch))
    [SID_IndoorMarch] = GFX_SkillIcon_IndoorMarch,
#endif

#if (defined(SID_DevilsPact) && COMMON_SKILL_VALID(SID_DevilsPact))
    [SID_DevilsPact] = GFX_SkillIcon_DevilsPact,
#endif

#if (defined(SID_DevilsWhim) && COMMON_SKILL_VALID(SID_DevilsWhim))
    [SID_DevilsWhim] = GFX_SkillIcon_DevilsWhim,
#endif

#if (defined(SID_DevilsLuck) && COMMON_SKILL_VALID(SID_DevilsLuck))
    [SID_DevilsLuck] = GFX_SkillIcon_DevilsLuck,
#endif

#if (defined(SID_Expertise) && COMMON_SKILL_VALID(SID_Expertise))
    [SID_Expertise] = GFX_SkillIcon_Expertise,
#endif

#if (defined(SID_PassionsFlow) && COMMON_SKILL_VALID(SID_PassionsFlow))
    [SID_PassionsFlow] = GFX_SkillIcon_PassionsFlow,
#endif

#if (defined(SID_NatureRush) && COMMON_SKILL_VALID(SID_NatureRush))
    [SID_NatureRush] = GFX_SkillIcon_NatureRush,
#endif

#if (defined(SID_CounterMagic) && COMMON_SKILL_VALID(SID_CounterMagic))
    [SID_CounterMagic] = GFX_SkillIcon_CounterMagic,
#endif

#if (defined(SID_LadyBlade) && COMMON_SKILL_VALID(SID_LadyBlade))
    [SID_LadyBlade] = GFX_SkillIcon_LadyBlade,
#endif

#if (defined(SID_LullDefense) && COMMON_SKILL_VALID(SID_LullDefense))
    [SID_LullDefense] = GFX_SkillIcon_LullDefense,
#endif

#if (defined(SID_LullLuck) && COMMON_SKILL_VALID(SID_LullLuck))
    [SID_LullLuck] = GFX_SkillIcon_LullLuck,
#endif

#if (defined(SID_LullMagic) && COMMON_SKILL_VALID(SID_LullMagic))
    [SID_LullMagic] = GFX_SkillIcon_LullMagic,
#endif

#if (defined(SID_LullMovement) && COMMON_SKILL_VALID(SID_LullMovement))
    [SID_LullMovement] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_LullResistance) && COMMON_SKILL_VALID(SID_LullResistance))
    [SID_LullResistance] = GFX_SkillIcon_LullResistance,
#endif

#if (defined(SID_LullSkill) && COMMON_SKILL_VALID(SID_LullSkill))
    [SID_LullSkill] = GFX_SkillIcon_LullSkill,
#endif

#if (defined(SID_LullSpectrum) && COMMON_SKILL_VALID(SID_LullSpectrum))
    [SID_LullSpectrum] = GFX_SkillIcon_LullSpectrum,
#endif

#if (defined(SID_LullSpeed) && COMMON_SKILL_VALID(SID_LullSpeed))
    [SID_LullSpeed] = GFX_SkillIcon_LullSpeed,
#endif

#if (defined(SID_LullStrength) && COMMON_SKILL_VALID(SID_LullStrength))
    [SID_LullStrength] = GFX_SkillIcon_LullStrength,
#endif

#if (defined(SID_ShrewdPotential) && COMMON_SKILL_VALID(SID_ShrewdPotential))
    [SID_ShrewdPotential] = GFX_SkillIcon_ShrewdPotential,
#endif

#if (defined(SID_GoddessDance) && COMMON_SKILL_VALID(SID_GoddessDance))
    [SID_GoddessDance] = GFX_SkillIcon_GoddessDance,
#endif

#if (defined(SID_SteadyBrawler) && COMMON_SKILL_VALID(SID_SteadyBrawler))
    [SID_SteadyBrawler] = GFX_SkillIcon_SteadyBrawler,
#endif

#if (defined(SID_KeepUp) && COMMON_SKILL_VALID(SID_KeepUp))
    [SID_KeepUp] = GFX_SkillIcon_KeepUp,
#endif

#if (defined(SID_LiquidOoze) && COMMON_SKILL_VALID(SID_LiquidOoze))
    [SID_LiquidOoze] = GFX_SkillIcon_LiquidOoze,
#endif

#if (defined(SID_CloseCombat) && COMMON_SKILL_VALID(SID_CloseCombat))
    [SID_CloseCombat] = GFX_SkillIcon_CloseCombat,
#endif

#if (defined(SID_ArmorMarch) && COMMON_SKILL_VALID(SID_ArmorMarch))
    [SID_ArmorMarch] = GFX_SkillIcon_ArmorMarch,
#endif

#if (defined(SID_Eclipse) && COMMON_SKILL_VALID(SID_Eclipse))
    [SID_Eclipse] = GFX_SkillIcon_Eclipse,
#endif

#if (defined(SID_Barricade) && COMMON_SKILL_VALID(SID_Barricade))
    [SID_Barricade] = GFX_SkillIcon_Barricade,
#endif

#if (defined(SID_BarricadePlus) && COMMON_SKILL_VALID(SID_BarricadePlus))
    [SID_BarricadePlus] = GFX_SkillIcon_BarricadePlus,
#endif

#if (defined(SID_LunarBrace) && COMMON_SKILL_VALID(SID_LunarBrace))
    [SID_LunarBrace] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SoulSap) && COMMON_SKILL_VALID(SID_SoulSap))
    [SID_SoulSap] = GFX_SkillIcon_SoulSap,
#endif

#if (defined(SID_Pursuer) && COMMON_SKILL_VALID(SID_Pursuer))
    [SID_Pursuer] = GFX_SkillIcon_Pursuer,
#endif

#if (defined(SID_Stride) && COMMON_SKILL_VALID(SID_Stride))
    [SID_Stride] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_QuickLearner) && COMMON_SKILL_VALID(SID_QuickLearner))
    [SID_QuickLearner] = GFX_SkillIcon_QuickLearner,
#endif

#if (defined(SID_GoldDigger) && COMMON_SKILL_VALID(SID_GoldDigger))
    [SID_GoldDigger] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_EffectSpore) && COMMON_SKILL_VALID(SID_EffectSpore))
    [SID_EffectSpore] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_DominantArm) && COMMON_SKILL_VALID(SID_DominantArm))
    [SID_DominantArm] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_GaleWings) && COMMON_SKILL_VALID(SID_GaleWings))
    [SID_GaleWings] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SolidRock) && COMMON_SKILL_VALID(SID_SolidRock))
    [SID_SolidRock] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Sturdy) && COMMON_SKILL_VALID(SID_Sturdy))
    [SID_Sturdy] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_FranticSwing) && COMMON_SKILL_VALID(SID_FranticSwing))
    [SID_FranticSwing] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_SocialButterfly) && COMMON_SKILL_VALID(SID_SocialButterfly))
    [SID_SocialButterfly] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_MeleeManiac) && COMMON_SKILL_VALID(SID_MeleeManiac))
    [SID_MeleeManiac] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_CriticalOverload) && COMMON_SKILL_VALID(SID_CriticalOverload))
    [SID_CriticalOverload] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Swarp) && COMMON_SKILL_VALID(SID_Swarp))
    [SID_Swarp] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Blacksmith) && COMMON_SKILL_VALID(SID_Blacksmith))
    [SID_Blacksmith] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Deal) && COMMON_SKILL_VALID(SID_Deal))
    [SID_Deal] = GFX_SkillIcon_WIP,
#endif

#if (defined(SID_Bargain) && COMMON_SKILL_VALID(SID_Bargain))
    [SID_Bargain] = GFX_SkillIcon_Bargain,
#endif
};
