#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

const u16 gSkillDescs[MAX_SKILL_NUM + 1] = {
#if (defined(SID_HpBonus) && COMMON_SKILL_VALID(SID_HpBonus))
    [SID_HpBonus] = MSG_SKILL_HpBonus,
#endif

#if (defined(SID_StrBonus) && COMMON_SKILL_VALID(SID_StrBonus))
    [SID_StrBonus] = MSG_SKILL_StrBonus,
#endif

#if (defined(SID_MagBonus) && COMMON_SKILL_VALID(SID_MagBonus))
    [SID_MagBonus] = MSG_SKILL_MagBonus,
#endif

#if (defined(SID_SklBonus) && COMMON_SKILL_VALID(SID_SklBonus))
    [SID_SklBonus] = MSG_SKILL_SklBonus,
#endif

#if (defined(SID_SpdBonus) && COMMON_SKILL_VALID(SID_SpdBonus))
    [SID_SpdBonus] = MSG_SKILL_SpdBonus,
#endif

#if (defined(SID_LckBonus) && COMMON_SKILL_VALID(SID_LckBonus))
    [SID_LckBonus] = MSG_SKILL_LukBonus,
#endif

#if (defined(SID_DefBonus) && COMMON_SKILL_VALID(SID_DefBonus))
    [SID_DefBonus] = MSG_SKILL_DefBonus,
#endif

#if (defined(SID_ResBonus) && COMMON_SKILL_VALID(SID_ResBonus))
    [SID_ResBonus] = MSG_SKILL_ResBonus,
#endif

#if (defined(SID_MovBonus) && COMMON_SKILL_VALID(SID_MovBonus))
    [SID_MovBonus] = MSG_SKILL_MovBonus,
#endif

#if (defined(SID_DefiantStr) && COMMON_SKILL_VALID(SID_DefiantStr))
    [SID_DefiantStr] = MSG_SKILL_DefiantStr,
#endif

#if (defined(SID_DefiantMag) && COMMON_SKILL_VALID(SID_DefiantMag))
    [SID_DefiantMag] = MSG_SKILL_DefiantMag,
#endif

#if (defined(SID_DefiantSkl) && COMMON_SKILL_VALID(SID_DefiantSkl))
    [SID_DefiantSkl] = MSG_SKILL_DefiantSkl,
#endif

#if (defined(SID_DefiantSpd) && COMMON_SKILL_VALID(SID_DefiantSpd))
    [SID_DefiantSpd] = MSG_SKILL_DefiantSpd,
#endif

#if (defined(SID_DefiantLck) && COMMON_SKILL_VALID(SID_DefiantLck))
    [SID_DefiantLck] = MSG_SKILL_DefiantLck,
#endif

#if (defined(SID_DefiantDef) && COMMON_SKILL_VALID(SID_DefiantDef))
    [SID_DefiantDef] = MSG_SKILL_DefiantDef,
#endif

#if (defined(SID_DefiantRes) && COMMON_SKILL_VALID(SID_DefiantRes))
    [SID_DefiantRes] = MSG_SKILL_DefiantRes,
#endif

#if (defined(SID_DefiantCrit) && COMMON_SKILL_VALID(SID_DefiantCrit))
    [SID_DefiantCrit] = MSG_SKILL_DefiantCrit,
#endif

#if (defined(SID_DefiantAvoid) && COMMON_SKILL_VALID(SID_DefiantAvoid))
    [SID_DefiantAvoid] = MSG_SKILL_DefiantAvoid,
#endif

#if (defined(SID_Fury) && COMMON_SKILL_VALID(SID_Fury))
    [SID_Fury] = MSG_SKILL_Fury,
#endif

#if (defined(SID_FuryPlus) && COMMON_SKILL_VALID(SID_FuryPlus))
    [SID_FuryPlus] = MSG_SKILL_FuryPlus,
#endif

#if (defined(SID_FortressDef) && COMMON_SKILL_VALID(SID_FortressDef))
    [SID_FortressDef] = MSG_SKILL_FortressDef,
#endif

#if (defined(SID_FortressRes) && COMMON_SKILL_VALID(SID_FortressRes))
    [SID_FortressRes] = MSG_SKILL_FortressRes,
#endif

#if (defined(SID_LifeAndDeath) && COMMON_SKILL_VALID(SID_LifeAndDeath))
    [SID_LifeAndDeath] = MSG_SKILL_LifeAndDeath,
#endif

#if (defined(SID_LuckySeven) && COMMON_SKILL_VALID(SID_LuckySeven))
    [SID_LuckySeven] = MSG_SKILL_LuckySeven,
#endif

#if (defined(SID_Nullify) && COMMON_SKILL_VALID(SID_Nullify))
    [SID_Nullify] = MSG_SKILL_Nullify,
#endif

#if (defined(SID_Slayer) && COMMON_SKILL_VALID(SID_Slayer))
    [SID_Slayer] = MSG_SKILL_Slayer,
#endif

#if (defined(SID_Skybreaker) && COMMON_SKILL_VALID(SID_Skybreaker))
    [SID_Skybreaker] = MSG_SKILL_Skybreaker,
#endif

#if (defined(SID_Resourceful) && COMMON_SKILL_VALID(SID_Resourceful))
    [SID_Resourceful] = MSG_SKILL_Resourceful,
#endif

#if (defined(SID_Nihil) && COMMON_SKILL_VALID(SID_Nihil))
    [SID_Nihil] = MSG_SKILL_Nihil,
#endif

#if (defined(SID_CritSword) && COMMON_SKILL_VALID(SID_CritSword))
    [SID_CritSword] = MSG_SKILL_CritSword,
#endif

#if (defined(SID_CritAxe) && COMMON_SKILL_VALID(SID_CritAxe))
    [SID_CritAxe] = MSG_SKILL_CritAxe,
#endif

#if (defined(SID_CritLance) && COMMON_SKILL_VALID(SID_CritLance))
    [SID_CritLance] = MSG_SKILL_CritLance,
#endif

#if (defined(SID_CritBow) && COMMON_SKILL_VALID(SID_CritBow))
    [SID_CritBow] = MSG_SKILL_CritBow,
#endif

#if (defined(SID_FaireSword) && COMMON_SKILL_VALID(SID_FaireSword))
    [SID_FaireSword] = MSG_SKILL_FaireSword,
#endif

#if (defined(SID_FaireLance) && COMMON_SKILL_VALID(SID_FaireLance))
    [SID_FaireLance] = MSG_SKILL_FaireLance,
#endif

#if (defined(SID_FaireAxe) && COMMON_SKILL_VALID(SID_FaireAxe))
    [SID_FaireAxe] = MSG_SKILL_FaireAxe,
#endif

#if (defined(SID_FaireBow) && COMMON_SKILL_VALID(SID_FaireBow))
    [SID_FaireBow] = MSG_SKILL_FaireBow,
#endif

#if (defined(SID_FaireBMag) && COMMON_SKILL_VALID(SID_FaireBMag))
    [SID_FaireBMag] = MSG_SKILL_FaireBMag,
#endif

#if (defined(SID_Avoid) && COMMON_SKILL_VALID(SID_Avoid))
    [SID_Avoid] = MSG_SKILL_Avoid,
#endif

#if (defined(SID_AvoidSword) && COMMON_SKILL_VALID(SID_AvoidSword))
    [SID_AvoidSword] = MSG_SKILL_AvoidSword,
#endif

#if (defined(SID_RuinedBlade) && COMMON_SKILL_VALID(SID_RuinedBlade))
    [SID_RuinedBlade] = MSG_SKILL_RuinedBlade,
#endif

#if (defined(SID_HeavyBlade) && COMMON_SKILL_VALID(SID_HeavyBlade))
    [SID_HeavyBlade] = MSG_SKILL_HeavyBlade,
#endif

#if (defined(SID_FlashingBlade) && COMMON_SKILL_VALID(SID_FlashingBlade))
    [SID_FlashingBlade] = MSG_SKILL_FlashingBlade,
#endif

#if (defined(SID_HeavyBladePlus) && COMMON_SKILL_VALID(SID_HeavyBladePlus))
    [SID_HeavyBladePlus] = MSG_SKILL_HeavyBladePlus,
#endif

#if (defined(SID_FlashingBladePlus) && COMMON_SKILL_VALID(SID_FlashingBladePlus))
    [SID_FlashingBladePlus] = MSG_SKILL_FlashingBladePlus,
#endif

#if (defined(SID_LunaAttack) && COMMON_SKILL_VALID(SID_LunaAttack))
    [SID_LunaAttack] = MSG_SKILL_LunaAttack,
#endif

#if (defined(SID_SorceryBlade) && COMMON_SKILL_VALID(SID_SorceryBlade))
    [SID_SorceryBlade] = MSG_SKILL_SorceryBlade,
#endif

#if (defined(SID_Fortune) && COMMON_SKILL_VALID(SID_Fortune))
    [SID_Fortune] = MSG_SKILL_Fortune,
#endif

#if (defined(SID_BlueFlame) && COMMON_SKILL_VALID(SID_BlueFlame))
    [SID_BlueFlame] = MSG_SKILL_BlueFlame,
#endif

#if (defined(SID_SwordBreaker) && COMMON_SKILL_VALID(SID_SwordBreaker))
    [SID_SwordBreaker] = MSG_SKILL_SwordBreaker,
#endif

#if (defined(SID_AxeBreaker) && COMMON_SKILL_VALID(SID_AxeBreaker))
    [SID_AxeBreaker] = MSG_SKILL_AxeBreaker,
#endif

#if (defined(SID_LanceBreaker) && COMMON_SKILL_VALID(SID_LanceBreaker))
    [SID_LanceBreaker] = MSG_SKILL_LanceBreaker,
#endif

#if (defined(SID_BowBreaker) && COMMON_SKILL_VALID(SID_BowBreaker))
    [SID_BowBreaker] = MSG_SKILL_BowBreaker,
#endif

#if (defined(SID_BMagBreaker) && COMMON_SKILL_VALID(SID_BMagBreaker))
    [SID_BMagBreaker] = MSG_SKILL_BMagBreaker,
#endif

#if (defined(SID_Vantage) && COMMON_SKILL_VALID(SID_Vantage))
    [SID_Vantage] = MSG_SKILL_Vantage,
#endif

#if (defined(SID_Desperation) && COMMON_SKILL_VALID(SID_Desperation))
    [SID_Desperation] = MSG_SKILL_Desperation,
#endif

#if (defined(SID_QuickRiposte) && COMMON_SKILL_VALID(SID_QuickRiposte))
    [SID_QuickRiposte] = MSG_SKILL_QuickRiposte,
#endif

#if (defined(SID_WaryFighter) && COMMON_SKILL_VALID(SID_WaryFighter))
    [SID_WaryFighter] = MSG_SKILL_WaryFighter,
#endif

#if (defined(SID_DoubleLion) && COMMON_SKILL_VALID(SID_DoubleLion))
    [SID_DoubleLion] = MSG_SKILL_DoubleLion,
#endif

#if (defined(SID_Adept) && COMMON_SKILL_VALID(SID_Adept))
    [SID_Adept] = MSG_SKILL_Adept,
#endif

#if (defined(SID_Astra) && COMMON_SKILL_VALID(SID_Astra))
    [SID_Astra] = MSG_SKILL_Astra,
#endif

#if (defined(SID_Aegis) && COMMON_SKILL_VALID(SID_Aegis))
    [SID_Aegis] = MSG_SKILL_Aegis,
#endif

#if (defined(SID_Pavise) && COMMON_SKILL_VALID(SID_Pavise))
    [SID_Pavise] = MSG_SKILL_Pavise,
#endif

#if (defined(SID_Bane) && COMMON_SKILL_VALID(SID_Bane))
    [SID_Bane] = MSG_SKILL_Bane,
#endif

#if (defined(SID_Mercy) && COMMON_SKILL_VALID(SID_Mercy))
    [SID_Mercy] = MSG_SKILL_Mercy,
#endif

#if (defined(SID_Aether) && COMMON_SKILL_VALID(SID_Aether))
    [SID_Aether] = MSG_SKILL_Aether,
#endif

#if (defined(SID_Corona) && COMMON_SKILL_VALID(SID_Corona))
    [SID_Corona] = MSG_SKILL_Corona,
#endif

#if (defined(SID_Luna) && COMMON_SKILL_VALID(SID_Luna))
    [SID_Luna] = MSG_SKILL_Luna,
#endif

#if (defined(SID_Flare) && COMMON_SKILL_VALID(SID_Flare))
    [SID_Flare] = MSG_SKILL_Flare,
#endif

#if (defined(SID_Sol) && COMMON_SKILL_VALID(SID_Sol))
    [SID_Sol] = MSG_SKILL_Sol,
#endif

#if (defined(SID_DragonFang) && COMMON_SKILL_VALID(SID_DragonFang))
    [SID_DragonFang] = MSG_SKILL_DragonFang,
#endif

#if (defined(SID_Foresight) && COMMON_SKILL_VALID(SID_Foresight))
    [SID_Foresight] = MSG_SKILL_Foresight,
#endif

#if (defined(SID_BlowDarting) && COMMON_SKILL_VALID(SID_BlowDarting))
    [SID_BlowDarting] = MSG_SKILL_BlowDarting,
#endif

#if (defined(SID_BlowDeath) && COMMON_SKILL_VALID(SID_BlowDeath))
    [SID_BlowDeath] = MSG_SKILL_BlowDeath,
#endif

#if (defined(SID_BlowArmored) && COMMON_SKILL_VALID(SID_BlowArmored))
    [SID_BlowArmored] = MSG_SKILL_BlowArmored,
#endif

#if (defined(SID_BlowFiendish) && COMMON_SKILL_VALID(SID_BlowFiendish))
    [SID_BlowFiendish] = MSG_SKILL_BlowFiendish,
#endif

#if (defined(SID_BlowWarding) && COMMON_SKILL_VALID(SID_BlowWarding))
    [SID_BlowWarding] = MSG_SKILL_BlowWarding,
#endif

#if (defined(SID_BlowDuelist) && COMMON_SKILL_VALID(SID_BlowDuelist))
    [SID_BlowDuelist] = MSG_SKILL_BlowDuelist,
#endif

#if (defined(SID_BlowUncanny) && COMMON_SKILL_VALID(SID_BlowUncanny))
    [SID_BlowUncanny] = MSG_SKILL_BlowUncanny,
#endif

#if (defined(SID_BlowKilling) && COMMON_SKILL_VALID(SID_BlowKilling))
    [SID_BlowKilling] = MSG_SKILL_BlowKilling,
#endif

#if (defined(SID_StanceBracing) && COMMON_SKILL_VALID(SID_StanceBracing))
    [SID_StanceBracing] = MSG_SKILL_StanceBracing,
#endif

#if (defined(SID_StanceDarting) && COMMON_SKILL_VALID(SID_StanceDarting))
    [SID_StanceDarting] = MSG_SKILL_StanceDarting,
#endif

#if (defined(SID_StanceFierce) && COMMON_SKILL_VALID(SID_StanceFierce))
    [SID_StanceFierce] = MSG_SKILL_StanceFierce,
#endif

#if (defined(SID_StanceKestrel) && COMMON_SKILL_VALID(SID_StanceKestrel))
    [SID_StanceKestrel] = MSG_SKILL_StanceKestrel,
#endif

#if (defined(SID_StanceMirror) && COMMON_SKILL_VALID(SID_StanceMirror))
    [SID_StanceMirror] = MSG_SKILL_StanceMirror,
#endif

#if (defined(SID_StanceReady) && COMMON_SKILL_VALID(SID_StanceReady))
    [SID_StanceReady] = MSG_SKILL_StanceReady,
#endif

#if (defined(SID_StanceSteady) && COMMON_SKILL_VALID(SID_StanceSteady))
    [SID_StanceSteady] = MSG_SKILL_StanceSteady,
#endif

#if (defined(SID_StanceSturdy) && COMMON_SKILL_VALID(SID_StanceSturdy))
    [SID_StanceSturdy] = MSG_SKILL_StanceSturdy,
#endif

#if (defined(SID_StanceSwift) && COMMON_SKILL_VALID(SID_StanceSwift))
    [SID_StanceSwift] = MSG_SKILL_StanceSwift,
#endif

#if (defined(SID_StanceWarding) && COMMON_SKILL_VALID(SID_StanceWarding))
    [SID_StanceWarding] = MSG_SKILL_StanceWarding,
#endif

#if (defined(SID_RangeBonusBMag1) && COMMON_SKILL_VALID(SID_RangeBonusBMag1))
    [SID_RangeBonusBMag1] = MSG_SKILL_RangeBonusBMag1,
#endif

#if (defined(SID_RangeBonusBMag2) && COMMON_SKILL_VALID(SID_RangeBonusBMag2))
    [SID_RangeBonusBMag2] = MSG_SKILL_RangeBonusBMag2,
#endif

#if (defined(SID_RangeBonusBow1) && COMMON_SKILL_VALID(SID_RangeBonusBow1))
    [SID_RangeBonusBow1] = MSG_SKILL_RangeBonusBow1,
#endif

#if (defined(SID_RangeBonusBow2) && COMMON_SKILL_VALID(SID_RangeBonusBow2))
    [SID_RangeBonusBow2] = MSG_SKILL_RangeBonusBow2,
#endif

#if (defined(SID_RunningStart) && COMMON_SKILL_VALID(SID_RunningStart))
    [SID_RunningStart] = MSG_SKILL_RunningStart,
#endif

#if (defined(SID_Canto) && COMMON_SKILL_VALID(SID_Canto))
    [SID_Canto] = MSG_SKILL_Canto,
#endif

#if (defined(SID_CantoPlus) && COMMON_SKILL_VALID(SID_CantoPlus))
    [SID_CantoPlus] = MSG_SKILL_CantoPlus,
#endif

#if (defined(SID_AlertStance) && COMMON_SKILL_VALID(SID_AlertStance))
    [SID_AlertStance] = MSG_SKILL_AlertStance,
#endif

#if (defined(SID_AlertStancePlus) && COMMON_SKILL_VALID(SID_AlertStancePlus))
    [SID_AlertStancePlus] = MSG_SKILL_AlertStancePlus,
#endif

#if (defined(SID_Galeforce) && COMMON_SKILL_VALID(SID_Galeforce))
    [SID_Galeforce] = MSG_SKILL_Galeforce,
#endif

#if (defined(SID_COMBAT_Galeforce) && COMMON_SKILL_VALID(SID_COMBAT_Galeforce))
    [SID_COMBAT_Galeforce] = MSG_SKILL_COMBAT_Galeforce,
#endif

#if (defined(SID_SavageBlow) && COMMON_SKILL_VALID(SID_SavageBlow))
    [SID_SavageBlow] = MSG_SKILL_SavageBlow,
#endif

#if (defined(SID_BreathOfLife) && COMMON_SKILL_VALID(SID_BreathOfLife))
    [SID_BreathOfLife] = MSG_SKILL_BreathOfLife,
#endif

#if (defined(SID_Thunderstorm) && COMMON_SKILL_VALID(SID_Thunderstorm))
    [SID_Thunderstorm] = MSG_SKILL_Thunderstorm,
#endif

#if (defined(SID_PosReturn) && COMMON_SKILL_VALID(SID_PosReturn))
    [SID_PosReturn] = MSG_SKILL_PosReturn,
#endif

#if (defined(SID_Lunge) && COMMON_SKILL_VALID(SID_Lunge))
    [SID_Lunge] = MSG_SKILL_Lunge,
#endif

#if (defined(SID_Renewal) && COMMON_SKILL_VALID(SID_Renewal))
    [SID_Renewal] = MSG_SKILL_Renewal,
#endif

#if (defined(SID_Imbue) && COMMON_SKILL_VALID(SID_Imbue))
    [SID_Imbue] = MSG_SKILL_Imbue,
#endif

#if (defined(SID_Forager) && COMMON_SKILL_VALID(SID_Forager))
    [SID_Forager] = MSG_SKILL_Forager,
#endif

#if (defined(SID_Anathema) && COMMON_SKILL_VALID(SID_Anathema))
    [SID_Anathema] = MSG_SKILL_Anathema,
#endif

#if (defined(SID_Bond) && COMMON_SKILL_VALID(SID_Bond))
    [SID_Bond] = MSG_SKILL_Bond,
#endif

#if (defined(SID_Charm) && COMMON_SKILL_VALID(SID_Charm))
    [SID_Charm] = MSG_SKILL_Charm,
#endif

#if (defined(SID_Intimidate) && COMMON_SKILL_VALID(SID_Intimidate))
    [SID_Intimidate] = MSG_SKILL_Intimidate,
#endif

#if (defined(SID_Tantivy) && COMMON_SKILL_VALID(SID_Tantivy))
    [SID_Tantivy] = MSG_SKILL_Tantivy,
#endif

#if (defined(SID_Focus) && COMMON_SKILL_VALID(SID_Focus))
    [SID_Focus] = MSG_SKILL_Focus,
#endif

#if (defined(SID_Hex) && COMMON_SKILL_VALID(SID_Hex))
    [SID_Hex] = MSG_SKILL_Hex,
#endif

#if (defined(SID_Infiltrator) && COMMON_SKILL_VALID(SID_Infiltrator))
    [SID_Infiltrator] = MSG_SKILL_Infiltrator,
#endif

#if (defined(SID_Inspiration) && COMMON_SKILL_VALID(SID_Inspiration))
    [SID_Inspiration] = MSG_SKILL_Inspiration,
#endif

#if (defined(SID_DivinelyInspiring) && COMMON_SKILL_VALID(SID_DivinelyInspiring))
    [SID_DivinelyInspiring] = MSG_SKILL_DivinelyInspiring,
#endif

#if (defined(SID_VoiceOfPeace) && COMMON_SKILL_VALID(SID_VoiceOfPeace))
    [SID_VoiceOfPeace] = MSG_SKILL_VoiceOfPeace,
#endif

#if (defined(SID_PowHone) && COMMON_SKILL_VALID(SID_PowHone))
    [SID_PowHone] = MSG_SKILL_PowHone,
#endif

#if (defined(SID_MagHone) && COMMON_SKILL_VALID(SID_MagHone))
    [SID_MagHone] = MSG_SKILL_MagHone,
#endif

#if (defined(SID_SklHone) && COMMON_SKILL_VALID(SID_SklHone))
    [SID_SklHone] = MSG_SKILL_SklHone,
#endif

#if (defined(SID_SpdHone) && COMMON_SKILL_VALID(SID_SpdHone))
    [SID_SpdHone] = MSG_SKILL_SpdHone,
#endif

#if (defined(SID_LckHone) && COMMON_SKILL_VALID(SID_LckHone))
    [SID_LckHone] = MSG_SKILL_LckHone,
#endif

#if (defined(SID_DefHone) && COMMON_SKILL_VALID(SID_DefHone))
    [SID_DefHone] = MSG_SKILL_DefHone,
#endif

#if (defined(SID_ResHone) && COMMON_SKILL_VALID(SID_ResHone))
    [SID_ResHone] = MSG_SKILL_ResHone,
#endif

#if (defined(SID_MovHone) && COMMON_SKILL_VALID(SID_MovHone))
    [SID_MovHone] = MSG_SKILL_MovHone,
#endif

#if (defined(SID_BattleRange_Todo1) && COMMON_SKILL_VALID(SID_BattleRange_Todo1))
    [SID_BattleRange_Todo1] = MSG_SKILL_BattleRange1,
#endif

#if (defined(SID_BattleRange_Todo2) && COMMON_SKILL_VALID(SID_BattleRange_Todo2))
    [SID_BattleRange_Todo2] = MSG_SKILL_BattleRange2,
#endif

#if (defined(SID_BattleRange_Todo3) && COMMON_SKILL_VALID(SID_BattleRange_Todo3))
    [SID_BattleRange_Todo3] = MSG_SKILL_BattleRange3,
#endif

#if (defined(SID_Steal) && COMMON_SKILL_VALID(SID_Steal))
    [SID_Steal] = MSG_SKILL_Steal,
#endif

#if (defined(SID_HealingFocus) && COMMON_SKILL_VALID(SID_HealingFocus))
    [SID_HealingFocus] = MSG_SKILL_HealingFocus,
#endif

#if (defined(SID_Blossom) && COMMON_SKILL_VALID(SID_Blossom))
    [SID_Blossom] = MSG_SKILL_Blossom,
#endif

#if (defined(SID_Paragon) && COMMON_SKILL_VALID(SID_Paragon))
    [SID_Paragon] = MSG_SKILL_Paragon,
#endif

#if (defined(SID_VoidCurse) && COMMON_SKILL_VALID(SID_VoidCurse))
    [SID_VoidCurse] = MSG_SKILL_VoidCurse,
#endif

#if (defined(SID_Aptitude) && COMMON_SKILL_VALID(SID_Aptitude))
    [SID_Aptitude] = MSG_SKILL_Aptitude,
#endif

#if (defined(SID_Peacebringer) && COMMON_SKILL_VALID(SID_VoiceOfPeace))
    [SID_Peacebringer] = MSG_SKILL_Peacebringer,
#endif

#if (defined(SID_LEGEND_InoriAtk) && COMMON_SKILL_VALID(SID_LEGEND_InoriAtk))
    [SID_LEGEND_InoriAtk] = MSG_SKILL_LEGEND_InoriAtk,
#endif

#if (defined(SID_LEGEND_InoriAvo) && COMMON_SKILL_VALID(SID_LEGEND_InoriAvo))
    [SID_LEGEND_InoriAvo] = MSG_SKILL_LEGEND_InoriAvo,
#endif

#if (defined(SID_LEGEND_InoriDef) && COMMON_SKILL_VALID(SID_LEGEND_InoriDef))
    [SID_LEGEND_InoriDef] = MSG_SKILL_LEGEND_InoriDef,
#endif

#if (defined(SID_LEGEND_4) && COMMON_SKILL_VALID(SID_LEGEND_4))
    [SID_LEGEND_4] = MSG_WIP,
#endif

#if (defined(SID_LEGEND_5) && COMMON_SKILL_VALID(SID_LEGEND_5))
    [SID_LEGEND_5] = MSG_WIP,
#endif

#if (defined(SID_Frenzy) && COMMON_SKILL_VALID(SID_Frenzy))
    [SID_Frenzy] = MSG_SKILL_Frenzy,
#endif

#if (defined(SID_WonderGuard) && COMMON_SKILL_VALID(SID_WonderGuard))
    [SID_WonderGuard] = MSG_SKILL_WonderGuard,
#endif

#if (defined(SID_PoisonPoint) && COMMON_SKILL_VALID(SID_PoisonPoint))
    [SID_PoisonPoint] = MSG_SKILL_PoisonPoint,
#endif

#if (defined(SID_Merciless) && COMMON_SKILL_VALID(SID_Merciless))
    [SID_Merciless] = MSG_SKILL_Merciless,
#endif

#if (defined(SID_CriticalPierce) && COMMON_SKILL_VALID(SID_CriticalPierce))
    [SID_CriticalPierce] = MSG_SKILL_CriticalPierce,
#endif

#if (defined(SID_KillingMachine) && COMMON_SKILL_VALID(SID_KillingMachine))
    [SID_KillingMachine] = MSG_SKILL_KillingMachine,
#endif

#if (defined(SID_HeavyStrikes) && COMMON_SKILL_VALID(SID_HeavyStrikes))
    [SID_HeavyStrikes] = MSG_SKILL_HeavyStrikes,
#endif

#if (defined(SID_QuickBurn) && COMMON_SKILL_VALID(SID_QuickBurn))
    [SID_QuickBurn] = MSG_SKILL_QuickBurn,
#endif

#if (defined(SID_SlowBurn) && COMMON_SKILL_VALID(SID_SlowBurn))
    [SID_SlowBurn] = MSG_SKILL_SlowBurn,
#endif

#if (defined(SID_StanceSpectrum) && COMMON_SKILL_VALID(SID_StanceSpectrum))
    [SID_StanceSpectrum] = MSG_SKILL_StanceSpectrum,
#endif

#if (defined(SID_Technician) && COMMON_SKILL_VALID(SID_Technician))
    [SID_Technician] = MSG_SKILL_Technician,
#endif

#if (defined(SID_CatchingUp) && COMMON_SKILL_VALID(SID_CatchingUp))
    [SID_CatchingUp] = MSG_SKILL_CatchingUp,
#endif

#if (defined(SID_BloodTide) && COMMON_SKILL_VALID(SID_BloodTide))
    [SID_BloodTide] = MSG_SKILL_BloodTide,
#endif

#if (defined(SID_WhitePool) && COMMON_SKILL_VALID(SID_WhitePool))
    [SID_WhitePool] = MSG_SKILL_WhitePool,
#endif

#if (defined(SID_NightTide) && COMMON_SKILL_VALID(SID_NightTide))
    [SID_NightTide] = MSG_SKILL_NightTide,
#endif

#if (defined(SID_DriveStr) && COMMON_SKILL_VALID(SID_DriveStr))
    [SID_DriveStr] = MSG_SKILL_DriveStr,
#endif

#if (defined(SID_DriveMag) && COMMON_SKILL_VALID(SID_DriveMag))
    [SID_DriveMag] = MSG_SKILL_DriveMag,
#endif

#if (defined(SID_DriveSpd) && COMMON_SKILL_VALID(SID_DriveSpd))
    [SID_DriveSpd] = MSG_SKILL_DriveSpd,
#endif

#if (defined(SID_DriveDef) && COMMON_SKILL_VALID(SID_DriveDef))
    [SID_DriveDef] = MSG_SKILL_DriveDef,
#endif

#if (defined(SID_DriveRes) && COMMON_SKILL_VALID(SID_DriveRes))
    [SID_DriveRes] = MSG_SKILL_DriveRes,
#endif

#if (defined(SID_Charisma) && COMMON_SKILL_VALID(SID_Charisma))
    [SID_Charisma] = MSG_SKILL_Charisma,
#endif

#if (defined(SID_LilysPoise) && COMMON_SKILL_VALID(SID_LilysPoise))
    [SID_LilysPoise] = MSG_SKILL_LilysPoise,
#endif

#if (defined(SID_Daunt) && COMMON_SKILL_VALID(SID_Daunt))
    [SID_Daunt] = MSG_SKILL_Daunt,
#endif

#if (defined(SID_SpurStr) && COMMON_SKILL_VALID(SID_SpurStr))
    [SID_SpurStr] = MSG_SKILL_SpurStr,
#endif

#if (defined(SID_SpurMag) && COMMON_SKILL_VALID(SID_SpurMag))
    [SID_SpurMag] = MSG_SKILL_SpurMag,
#endif

#if (defined(SID_SpurDef) && COMMON_SKILL_VALID(SID_SpurDef))
    [SID_SpurDef] = MSG_SKILL_SpurDef,
#endif

#if (defined(SID_SpurRes) && COMMON_SKILL_VALID(SID_SpurRes))
    [SID_SpurRes] = MSG_SKILL_SpurRes,
#endif

#if (defined(SID_SpurSpd) && COMMON_SKILL_VALID(SID_SpurSpd))
    [SID_SpurSpd] = MSG_SKILL_SpurSpd,
#endif

#if (defined(SID_BattleVeteran) && COMMON_SKILL_VALID(SID_BattleVeteran))
    [SID_BattleVeteran] = MSG_SKILL_BattleVeteran,
#endif

#if (defined(SID_Gentilhomme) && COMMON_SKILL_VALID(SID_Gentilhomme))
    [SID_Gentilhomme] = MSG_SKILL_Gentilhomme,
#endif

#if (defined(SID_Demoiselle) && COMMON_SKILL_VALID(SID_Demoiselle))
    [SID_Demoiselle] = MSG_SKILL_Demoiselle,
#endif

#if (defined(SID_MaleficAura) && (SID_MaleficAura < MAX_SKILL_NUM))
    [SID_MaleficAura] = MSG_SKILL_MaleficAura,
#endif

#if (defined(SID_Solidarity) && COMMON_SKILL_VALID(SID_Solidarity))
    [SID_Solidarity] = MSG_SKILL_Solidarity,
#endif

#if (defined(SID_StoneBody) && COMMON_SKILL_VALID(SID_StoneBody))
    [SID_StoneBody] = MSG_SKILL_StoneBody,
#endif

#if (defined(SID_Chivalry) && COMMON_SKILL_VALID(SID_Chivalry))
    [SID_Chivalry] = MSG_SKILL_Chivalry,
#endif

#if (defined(SID_Pragmatic) && COMMON_SKILL_VALID(SID_Pragmatic))
    [SID_Pragmatic] = MSG_SKILL_Pragmatic,
#endif

#if (defined(SID_WindDisciple) && COMMON_SKILL_VALID(SID_WindDisciple))
    [SID_WindDisciple] = MSG_SKILL_WindDisciple,
#endif

#if (defined(SID_Perfectionist) && COMMON_SKILL_VALID(SID_Perfectionist))
    [SID_Perfectionist] = MSG_SKILL_Perfectionist,
#endif

#if (defined(SID_CriticalForce) && COMMON_SKILL_VALID(SID_CriticalForce))
    [SID_CriticalForce] = MSG_SKILL_CriticalForce,
#endif

#if (defined(SID_StrongRiposte) && COMMON_SKILL_VALID(SID_StrongRiposte))
    [SID_StrongRiposte] = MSG_SKILL_StrongRiposte,
#endif

#if (defined(SID_Patience) && COMMON_SKILL_VALID(SID_Patience))
    [SID_Patience] = MSG_SKILL_Patience,
#endif

#if (defined(SID_Pursuit) && COMMON_SKILL_VALID(SID_Pursuit))
    [SID_Pursuit] = MSG_SKILL_Pursuit,
#endif

#if (defined(SID_DragonSkin) && COMMON_SKILL_VALID(SID_DragonSkin))
    [SID_DragonSkin] = MSG_SKILL_DragonSkin,
#endif

#if (defined(SID_Hawkeye) && COMMON_SKILL_VALID(SID_Hawkeye))
    [SID_Hawkeye] = MSG_SKILL_Hawkeye,
#endif

#if (defined(SID_MagicEye) && COMMON_SKILL_VALID(SID_MagicEye))
    [SID_MagicEye] = MSG_SKILL_MagicEye,
#endif

#if (defined(SID_KeenFighter) && COMMON_SKILL_VALID(SID_KeenFighter))
    [SID_KeenFighter] = MSG_SKILL_KeenFighter,
#endif

#if (defined(SID_FireBoost) && COMMON_SKILL_VALID(SID_FireBoost))
    [SID_FireBoost] = MSG_SKILL_FireBoost,
#endif

#if (defined(SID_WindBoost) && COMMON_SKILL_VALID(SID_WindBoost))
    [SID_WindBoost] = MSG_SKILL_WindBoost,
#endif

#if (defined(SID_EarthBoost) && COMMON_SKILL_VALID(SID_EarthBoost))
    [SID_EarthBoost] = MSG_SKILL_EarthBoost,
#endif

#if (defined(SID_WaterBoost) && COMMON_SKILL_VALID(SID_WaterBoost))
    [SID_WaterBoost] = MSG_SKILL_WaterBoost,
#endif

#if (defined(SID_FieryBlood) && COMMON_SKILL_VALID(SID_FieryBlood))
    [SID_FieryBlood] = MSG_SKILL_FieryBlood,
#endif

#if (defined(SID_Colossus) && COMMON_SKILL_VALID(SID_Colossus))
    [SID_Colossus] = MSG_SKILL_Colossus,
#endif

#if (defined(SID_LightWeight) && COMMON_SKILL_VALID(SID_LightWeight))
    [SID_LightWeight] = MSG_SKILL_LightWeight,
#endif

#if (defined(SID_EvenFooted) && COMMON_SKILL_VALID(SID_EvenFooted))
    [SID_EvenFooted] = MSG_SKILL_EvenFooted,
#endif

#if (defined(SID_QuickDraw) && COMMON_SKILL_VALID(SID_QuickDraw))
    [SID_QuickDraw] = MSG_SKILL_QuickDraw,
#endif

#if (defined(SID_ArcaneBlade) && COMMON_SKILL_VALID(SID_ArcaneBlade))
    [SID_ArcaneBlade] = MSG_SKILL_ArcaneBlade,
#endif

#if (defined(SID_ElbowRoom) && COMMON_SKILL_VALID(SID_ElbowRoom))
    [SID_ElbowRoom] = MSG_SKILL_ElbowRoom,
#endif

#if (defined(SID_NaturalCover) && COMMON_SKILL_VALID(SID_NaturalCover))
    [SID_NaturalCover] = MSG_SKILL_NaturalCover,
#endif

#if (defined(SID_ChaosStyle) && COMMON_SKILL_VALID(SID_ChaosStyle))
    [SID_ChaosStyle] = MSG_SKILL_ChaosStyle,
#endif

#if (defined(SID_PushDefense) && COMMON_SKILL_VALID(SID_PushDefense))
    [SID_PushDefense] = MSG_SKILL_PushDefense,
#endif

#if (defined(SID_PushMagic) && COMMON_SKILL_VALID(SID_PushMagic))
    [SID_PushMagic] = MSG_SKILL_PushMagic,
#endif

#if (defined(SID_PushResistance) && COMMON_SKILL_VALID(SID_PushResistance))
    [SID_PushResistance] = MSG_SKILL_PushResistance,
#endif

#if (defined(SID_PushSkill) && COMMON_SKILL_VALID(SID_PushSkill))
    [SID_PushSkill] = MSG_SKILL_PushSkill,
#endif

#if (defined(SID_PushSpeed) && COMMON_SKILL_VALID(SID_PushSpeed))
    [SID_PushSpeed] = MSG_SKILL_PushSpeed,
#endif

#if (defined(SID_PushStrength) && COMMON_SKILL_VALID(SID_PushStrength))
    [SID_PushStrength] = MSG_SKILL_PushStrength,
#endif

#if (defined(SID_PushLuck) && COMMON_SKILL_VALID(SID_PushLuck))
    [SID_PushLuck] = MSG_SKILL_PushLuck,
#endif

#if (defined(SID_PushMovement) && COMMON_SKILL_VALID(SID_PushMovement))
    [SID_PushMovement] = MSG_SKILL_PushMovement,
#endif

#if (defined(SID_PushSpectrum) && COMMON_SKILL_VALID(SID_PushSpectrum))
    [SID_PushSpectrum] = MSG_SKILL_PushSpectrum,
#endif

#if (defined(SID_Charge) && COMMON_SKILL_VALID(SID_Charge))
    [SID_Charge] = MSG_SKILL_Charge,
#endif

#if (defined(SID_ChargePlus) && COMMON_SKILL_VALID(SID_ChargePlus))
    [SID_ChargePlus] = MSG_SKILL_ChargePlus,
#endif

#if (defined(SID_Wrath) && COMMON_SKILL_VALID(SID_Wrath))
    [SID_Wrath] = MSG_SKILL_Wrath,
#endif

#if (defined(SID_Vigilance) && COMMON_SKILL_VALID(SID_Vigilance))
    [SID_Vigilance] = MSG_SKILL_Vigilance,
#endif

#if (defined(SID_OutdoorFighter) && COMMON_SKILL_VALID(SID_OutdoorFighter))
    [SID_OutdoorFighter] = MSG_SKILL_OutdoorFighter,
#endif

#if (defined(SID_DancingBlade) && COMMON_SKILL_VALID(SID_DancingBlade))
    [SID_DancingBlade] = MSG_SKILL_DancingBlade,
#endif

#if (defined(SID_KnightAspirant) && COMMON_SKILL_VALID(SID_KnightAspirant))
    [SID_KnightAspirant] = MSG_SKILL_KnightAspirant,
#endif

#if (defined(SID_Outrider) && COMMON_SKILL_VALID(SID_Outrider))
    [SID_Outrider] = MSG_SKILL_Outrider,
#endif

#if (defined(SID_EvenRhythm) && COMMON_SKILL_VALID(SID_EvenRhythm))
    [SID_EvenRhythm] = MSG_SKILL_EvenRhythm,
#endif

#if (defined(SID_OddRhythm) && COMMON_SKILL_VALID(SID_OddRhythm))
    [SID_OddRhythm] = MSG_SKILL_OddRhythm,
#endif

#if (defined(SID_OddFooted) && COMMON_SKILL_VALID(SID_OddFooted))
    [SID_OddFooted] = MSG_SKILL_OddFooted,
#endif

#if (defined(SID_Corrosion) && COMMON_SKILL_VALID(SID_Corrosion))
    [SID_Corrosion] = MSG_SKILL_Corrosion,
#endif

#if (defined(SID_NoGuard) && COMMON_SKILL_VALID(SID_NoGuard))
    [SID_NoGuard] = MSG_SKILL_NoGuard,
#endif

#if (defined(SID_Puissance) && COMMON_SKILL_VALID(SID_Puissance))
    [SID_Puissance] = MSG_SKILL_Puissance,
#endif

#if (defined(SID_Prescience) && COMMON_SKILL_VALID(SID_Prescience))
    [SID_Prescience] = MSG_SKILL_Prescience,
#endif

#if (defined(SID_Resolve) && COMMON_SKILL_VALID(SID_Resolve))
    [SID_Resolve] = MSG_SKILL_Resolve,
#endif

#if (defined(SID_SilentPride) && COMMON_SKILL_VALID(SID_SilentPride))
    [SID_SilentPride] = MSG_SKILL_SilentPride,
#endif

#if (defined(SID_RightfulKing) && COMMON_SKILL_VALID(SID_RightfulKing))
    [SID_RightfulKing] = MSG_SKILL_RightfulKing,
#endif

#if (defined(SID_RightfulGod) && COMMON_SKILL_VALID(SID_RightfulGod))
    [SID_RightfulGod] = MSG_SKILL_RightfulGod,
#endif

#if (defined(SID_RightfulArch) && COMMON_SKILL_VALID(SID_RightfulArch))
    [SID_RightfulArch] = MSG_SKILL_RightfulArch,
#endif

#if (defined(SID_Guts) && COMMON_SKILL_VALID(SID_Guts))
    [SID_Guts] = MSG_SKILL_Guts,
#endif

#if (defined(SID_StrongCon) && COMMON_SKILL_VALID(SID_StrongCon))
    [SID_StrongCon] = MSG_SKILL_StrongCon,
#endif

#if (defined(SID_Impale) && COMMON_SKILL_VALID(SID_Impale))
    [SID_Impale] = MSG_SKILL_Impale,
#endif

#if (defined(SID_Hero) && COMMON_SKILL_VALID(SID_Hero))
    [SID_Hero] = MSG_SKILL_Hero,
#endif

#if (defined(SID_Ignis) && COMMON_SKILL_VALID(SID_Ignis))
    [SID_Ignis] = MSG_SKILL_Ignis,
#endif

#if (defined(SID_HolyAura) && COMMON_SKILL_VALID(SID_HolyAura))
    [SID_HolyAura] = MSG_SKILL_HolyAura,
#endif

#if (defined(SID_Loyalty) && COMMON_SKILL_VALID(SID_Loyalty))
    [SID_Loyalty] = MSG_SKILL_Loyalty,
#endif

#if (defined(SID_TowerShield) && COMMON_SKILL_VALID(SID_TowerShield))
    [SID_TowerShield] = MSG_SKILL_TowerShield,
#endif

#if (defined(SID_StunningSmile) && COMMON_SKILL_VALID(SID_StunningSmile))
    [SID_StunningSmile] = MSG_SKILL_StunningSmile,
#endif

#if (defined(SID_Trample) && COMMON_SKILL_VALID(SID_Trample))
    [SID_Trample] = MSG_SKILL_Trample,
#endif

#if (defined(SID_Opportunist) && COMMON_SKILL_VALID(SID_Opportunist))
    [SID_Opportunist] = MSG_SKILL_Opportunist,
#endif

#if (defined(SID_SuperLuck) && COMMON_SKILL_VALID(SID_SuperLuck))
    [SID_SuperLuck] = MSG_SKILL_SuperLuck,
#endif

#if (defined(SID_ShortShield) && COMMON_SKILL_VALID(SID_ShortShield))
    [SID_ShortShield] = MSG_SKILL_ShortShield,
#endif

#if (defined(SID_Vanity) && COMMON_SKILL_VALID(SID_Vanity))
    [SID_Vanity] = MSG_SKILL_Vanity,
#endif

#if (defined(SID_DefenseBoost) && COMMON_SKILL_VALID(SID_DefenseBoost))
    [SID_DefenseBoost] = MSG_SKILL_DefenseBoost,
#endif

#if (defined(SID_LuckBoost) && COMMON_SKILL_VALID(SID_LuckBoost))
    [SID_LuckBoost] = MSG_SKILL_LuckBoost,
#endif

#if (defined(SID_MagicBoost) && COMMON_SKILL_VALID(SID_MagicBoost))
    [SID_MagicBoost] = MSG_SKILL_MagicBoost,
#endif

#if (defined(SID_MoveBoost) && COMMON_SKILL_VALID(SID_MoveBoost))
    [SID_MoveBoost] = MSG_SKILL_MoveBoost,
#endif

#if (defined(SID_ResistanceBoost) && COMMON_SKILL_VALID(SID_ResistanceBoost))
    [SID_ResistanceBoost] = MSG_SKILL_ResistanceBoost,
#endif

#if (defined(SID_SkillBoost) && COMMON_SKILL_VALID(SID_SkillBoost))
    [SID_SkillBoost] = MSG_SKILL_SkillBoost,
#endif

#if (defined(SID_SpeedBoost) && COMMON_SKILL_VALID(SID_SpeedBoost))
    [SID_SpeedBoost] = MSG_SKILL_SpeedBoost,
#endif

#if (defined(SID_StrengthBoost) && COMMON_SKILL_VALID(SID_StrengthBoost))
    [SID_StrengthBoost] = MSG_SKILL_StrengthBoost,
#endif

#if (defined(SID_SpectrumBoost) && COMMON_SKILL_VALID(SID_SpectrumBoost))
    [SID_SpectrumBoost] = MSG_SKILL_SpectrumBoost,
#endif

#if (defined(SID_Glacies) && COMMON_SKILL_VALID(SID_Glacies))
    [SID_Glacies] = MSG_SKILL_Glacies,
#endif

#if (defined(SID_Vengeance) && COMMON_SKILL_VALID(SID_Vengeance))
    [SID_Vengeance] = MSG_SKILL_Vengeance,
#endif

#if (defined(SID_Deadeye) && COMMON_SKILL_VALID(SID_Deadeye))
    [SID_Deadeye] = MSG_SKILL_Deadeye,
#endif

#if (defined(SID_AxeFaith) && COMMON_SKILL_VALID(SID_AxeFaith))
    [SID_AxeFaith] = MSG_SKILL_AxeFaith,
#endif

#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
    [SID_RuinedBladePlus] = MSG_SKILL_RuinedBladePlus,
#endif

#if (defined(SID_COMBAT_Grounder) && COMMON_SKILL_VALID(SID_COMBAT_Grounder))
    [SID_COMBAT_Grounder] = MSG_SKILL_COMBAT_Grounder,
#endif

#if (defined(SID_COMBAT_BaneOfMonsters) && COMMON_SKILL_VALID(SID_COMBAT_BaneOfMonsters))
    [SID_COMBAT_BaneOfMonsters] = MSG_SKILL_COMBAT_BaneOfMonsters,
#endif

#if (defined(SID_COMBAT_Hexblade) && COMMON_SKILL_VALID(SID_COMBAT_Hexblade))
    [SID_COMBAT_Hexblade] = MSG_SKILL_COMBAT_Hexblade,
#endif

#if (defined(SID_COMBAT_FoudroyantStrike) && COMMON_SKILL_VALID(SID_COMBAT_FoudroyantStrike))
    [SID_COMBAT_FoudroyantStrike] = MSG_SKILL_COMBAT_FoudroyantStrike,
#endif

#if (defined(SID_COMBAT_TempestLance) && COMMON_SKILL_VALID(SID_COMBAT_TempestLance))
    [SID_COMBAT_TempestLance] = MSG_SKILL_COMBAT_TempestLance,
#endif

#if (defined(SID_COMBAT_SwiftStrikes) && COMMON_SKILL_VALID(SID_COMBAT_SwiftStrikes))
    [SID_COMBAT_SwiftStrikes] = MSG_SKILL_COMBAT_SwiftStrikes,
#endif

#if (defined(SID_COMBAT_FrozenLance) && COMMON_SKILL_VALID(SID_COMBAT_FrozenLance))
    [SID_COMBAT_FrozenLance] = MSG_SKILL_COMBAT_FrozenLance,
#endif

#if (defined(SID_COMBAT_Vengeance) && COMMON_SKILL_VALID(SID_COMBAT_Vengeance))
    [SID_COMBAT_Vengeance] = MSG_SKILL_COMBAT_Vengeance,
#endif

#if (defined(SID_COMBAT_HelmSplitter) && COMMON_SKILL_VALID(SID_COMBAT_HelmSplitter))
    [SID_COMBAT_HelmSplitter] = MSG_SKILL_COMBAT_HelmSplitter,
#endif

#if (defined(SID_COMBAT_LightningAxe) && COMMON_SKILL_VALID(SID_COMBAT_LightningAxe))
    [SID_COMBAT_LightningAxe] = MSG_SKILL_COMBAT_LightningAxe,
#endif

#if (defined(SID_COMBAT_WarMasterStrike) && COMMON_SKILL_VALID(SID_COMBAT_WarMasterStrike))
    [SID_COMBAT_WarMasterStrike] = MSG_SKILL_COMBAT_WarMasterStrike,
#endif

#if (defined(SID_COMBAT_CurvedShot) && COMMON_SKILL_VALID(SID_COMBAT_CurvedShot))
    [SID_COMBAT_CurvedShot] = MSG_SKILL_COMBAT_CurvedShot,
#endif

#if (defined(SID_COMBAT_Deadeye) && COMMON_SKILL_VALID(SID_COMBAT_Deadeye))
    [SID_COMBAT_Deadeye] = MSG_SKILL_COMBAT_Deadeye,
#endif

#if (defined(SID_COMBAT_Encloser) && COMMON_SKILL_VALID(SID_COMBAT_Encloser))
    [SID_COMBAT_Encloser] = MSG_SKILL_COMBAT_Encloser,
#endif

#if (defined(SID_COMBAT_EncloserPlus) && COMMON_SKILL_VALID(SID_COMBAT_EncloserPlus))
    [SID_COMBAT_EncloserPlus] = MSG_SKILL_COMBAT_EncloserPlus,
#endif

#if (defined(SID_COMBAT_MonsterBlast) && COMMON_SKILL_VALID(SID_COMBAT_MonsterBlast))
    [SID_COMBAT_MonsterBlast] = MSG_SKILL_COMBAT_MonsterBlast,
#endif

#if (defined(SID_COMBAT_SchismShot) && COMMON_SKILL_VALID(SID_COMBAT_SchismShot))
    [SID_COMBAT_SchismShot] = MSG_SKILL_COMBAT_SchismShot,
#endif

#if (defined(SID_COMBAT_BreakShot) && COMMON_SKILL_VALID(SID_COMBAT_BreakShot))
    [SID_COMBAT_BreakShot] = MSG_SKILL_COMBAT_BreakShot,
#endif

#if (defined(SID_COMBAT_BreakShotPlus) && COMMON_SKILL_VALID(SID_COMBAT_BreakShotPlus))
    [SID_COMBAT_BreakShotPlus] = MSG_SKILL_COMBAT_BreakShotPlus,
#endif

#if (defined(SID_COMBAT_WaningShot) && COMMON_SKILL_VALID(SID_COMBAT_WaningShot))
    [SID_COMBAT_WaningShot] = MSG_SKILL_COMBAT_WaningShot,
#endif

#if (defined(SID_COMBAT_WaningShotPlus) && COMMON_SKILL_VALID(SID_COMBAT_WaningShotPlus))
    [SID_COMBAT_WaningShotPlus] = MSG_SKILL_COMBAT_WaningShotPlus,
#endif

#if (defined(SID_COMBAT_HuntersVolley) && COMMON_SKILL_VALID(SID_COMBAT_HuntersVolley))
    [SID_COMBAT_HuntersVolley] = MSG_SKILL_COMBAT_HuntersVolley,
#endif

#if (defined(SID_GreatShield) && COMMON_SKILL_VALID(SID_GreatShield))
    [SID_GreatShield] = MSG_SKILL_GreatShield,
#endif

#if (defined(SID_HoneCavalry) && COMMON_SKILL_VALID(SID_HoneCavalry))
    [SID_HoneCavalry] = MSG_SKILL_HoneCavalry,
#endif

#if (defined(SID_HoneFlier) && COMMON_SKILL_VALID(SID_HoneFlier))
    [SID_HoneFlier] = MSG_SKILL_HoneFlier,
#endif

#if (defined(SID_HoneArmor) && COMMON_SKILL_VALID(SID_HoneArmor))
    [SID_HoneArmor] = MSG_SKILL_HoneArmor,
#endif

#if (defined(SID_FortifyArmor) && COMMON_SKILL_VALID(SID_FortifyArmor))
    [SID_FortifyArmor] = MSG_SKILL_FortifyArmor,
#endif

#if (defined(SID_Shade) && COMMON_SKILL_VALID(SID_Shade))
    [SID_Shade] = MSG_SKILL_Shade,
#endif

#if (defined(SID_ShadePlus) && COMMON_SKILL_VALID(SID_ShadePlus))
    [SID_ShadePlus] = MSG_SKILL_ShadePlus,
#endif

#if (defined(SID_Provoke) && COMMON_SKILL_VALID(SID_Provoke))
    [SID_Provoke] = MSG_SKILL_Provoke,
#endif

#if (defined(SID_Teleportation) && COMMON_SKILL_VALID(SID_Teleportation))
    [SID_Teleportation] = MSG_SKILL_Teleportation,
#endif

#if (defined(SID_Dance) && COMMON_SKILL_VALID(SID_Dance))
    [SID_Dance] = MSG_SKILL_Dance,
#endif

#if (defined(SID_LockTouch) && COMMON_SKILL_VALID(SID_LockTouch))
    [SID_LockTouch] = MSG_SKILL_LockTouch,
#endif

#if (defined(SID_Summon) && COMMON_SKILL_VALID(SID_Summon))
    [SID_Summon] = MSG_SKILL_Summon,
#endif

#if (defined(SID_SureShot) && COMMON_SKILL_VALID(SID_SureShot))
    [SID_SureShot] = MSG_SKILL_SureShot,
#endif

#if (defined(SID_Crit) && COMMON_SKILL_VALID(SID_Crit))
    [SID_Crit] = MSG_SKILL_Crit,
#endif

#if (defined(SID_Lethality) && COMMON_SKILL_VALID(SID_Lethality))
    [SID_Lethality] = MSG_SKILL_Lethality,
#endif

#if (defined(SID_WatchfulEye) && COMMON_SKILL_VALID(SID_WatchfulEye))
    [SID_WatchfulEye] = MSG_SKILL_WatchfulEye,
#endif

#if (defined(SID_InfinityEdge) && COMMON_SKILL_VALID(SID_InfinityEdge))
    [SID_InfinityEdge] = MSG_SKILL_InfinityEdge,
#endif

#if (defined(SID_AversaNight) && COMMON_SKILL_VALID(SID_AversaNight))
    [SID_AversaNight] = MSG_SKILL_DEBUFF_Aversa,
#endif

#if (defined(SID_Supply) && COMMON_SKILL_VALID(SID_Supply))
    [SID_Supply] = MSG_SKILL_Supply,
#endif

#if (defined(SID_LightRune) && COMMON_SKILL_VALID(SID_LightRune))
    [SID_LightRune] = MSG_SKILL_LightRune,
#endif

#if (defined(SID_Mine) && COMMON_SKILL_VALID(SID_Mine))
    [SID_Mine] = MSG_SKILL_Mine,
#endif

#if (defined(SID_Inori) && COMMON_SKILL_VALID(SID_Inori))
    [SID_Inori] = MSG_SKILL_Inori,
#endif

#if (defined(SID_Armsthrift) && COMMON_SKILL_VALID(SID_Armsthrift))
    [SID_Armsthrift] = MSG_SKILL_Armsthrift,
#endif

#if (defined(SID_HighGround) && COMMON_SKILL_VALID(SID_HighGround))
    [SID_HighGround] = MSG_SKILL_HighGround,
#endif

#if (defined(SID_Airlock) && COMMON_SKILL_VALID(SID_Airlock))
    [SID_Airlock] = MSG_SKILL_Airlock,
#endif

#if (defined(SID_Drought) && COMMON_SKILL_VALID(SID_Drought))
    [SID_Drought] = MSG_SKILL_Drought,
#endif

#if (defined(SID_Drizzle) && COMMON_SKILL_VALID(SID_Drizzle))
    [SID_Drizzle] = MSG_SKILL_Drizzle,
#endif

#if (defined(SID_SnowWarning) && COMMON_SKILL_VALID(SID_SnowWarning))
    [SID_SnowWarning] = MSG_SKILL_SnowWarning,
#endif

#if (defined(SID_SandStream) && COMMON_SKILL_VALID(SID_SandStream))
    [SID_SandStream] = MSG_SKILL_SandStream,
#endif

#if (defined(SID_PointBlank) && COMMON_SKILL_VALID(SID_PointBlank))
    [SID_PointBlank] = MSG_SKILL_PointBlank,
#endif

#if (defined(SID_Synchronize) && COMMON_SKILL_VALID(SID_Synchronize))
    [SID_Synchronize] = MSG_SKILL_Synchronize,
#endif

#if (defined(SID_StaffSavant) && COMMON_SKILL_VALID(SID_StaffSavant))
    [SID_StaffSavant] = MSG_SKILL_StaffSavant,
#endif

#if (defined(SID_Skyguard) && COMMON_SKILL_VALID(SID_Skyguard))
    [SID_Skyguard] = MSG_SKILL_Skyguard,
#endif

#if (defined(SID_Horseguard) && COMMON_SKILL_VALID(SID_Horseguard))
    [SID_Horseguard] = MSG_SKILL_Horseguard,
#endif

#if (defined(SID_Armorboost) && COMMON_SKILL_VALID(SID_Armorboost))
    [SID_Armorboost] = MSG_SKILL_Armorboost,
#endif

#if (defined(SID_GentleFlower) && COMMON_SKILL_VALID(SID_GentleFlower))
    [SID_GentleFlower] = MSG_SKILL_GentleFlower,
#endif

#if (defined(SID_Admiration) && COMMON_SKILL_VALID(SID_Admiration))
    [SID_Admiration] = MSG_SKILL_Admiration,
#endif

#if (defined(SID_FairyTaleFolk) && COMMON_SKILL_VALID(SID_FairyTaleFolk))
    [SID_FairyTaleFolk] = MSG_SKILL_FairyTaleFolk,
#endif

#if (defined(SID_InitCalm) && COMMON_SKILL_VALID(SID_InitCalm))
    [SID_InitCalm] = MSG_SKILL_InitCalm,
#endif

#if (defined(SID_InitClever) && COMMON_SKILL_VALID(SID_InitClever))
    [SID_InitClever] = MSG_SKILL_InitClever,
#endif

#if (defined(SID_InitDeft) && COMMON_SKILL_VALID(SID_InitDeft))
    [SID_InitDeft] = MSG_SKILL_InitDeft,
#endif

#if (defined(SID_InitLucky) && COMMON_SKILL_VALID(SID_InitLucky))
    [SID_InitLucky] = MSG_SKILL_InitLucky,
#endif

#if (defined(SID_InitNimble) && COMMON_SKILL_VALID(SID_InitNimble))
    [SID_InitNimble] = MSG_SKILL_InitNimble,
#endif

#if (defined(SID_InitQuick) && COMMON_SKILL_VALID(SID_InitQuick))
    [SID_InitQuick] = MSG_SKILL_InitQuick,
#endif

#if (defined(SID_InitSpectrum) && COMMON_SKILL_VALID(SID_InitSpectrum))
    [SID_InitSpectrum] = MSG_SKILL_InitSpectrum,
#endif

#if (defined(SID_InitStrong) && COMMON_SKILL_VALID(SID_InitStrong))
    [SID_InitStrong] = MSG_SKILL_InitStrong,
#endif

#if (defined(SID_InitSturdy) && COMMON_SKILL_VALID(SID_InitSturdy))
    [SID_InitSturdy] = MSG_SKILL_InitSturdy,
#endif

#if (defined(SID_Lifetaker) && COMMON_SKILL_VALID(SID_Lifetaker))
    [SID_Lifetaker] = MSG_SKILL_Lifetaker,
#endif

#if (defined(SID_Analytic) && COMMON_SKILL_VALID(SID_Analytic))
    [SID_Analytic] = MSG_SKILL_Analytic,
#endif

#if (defined(SID_OathDefense) && COMMON_SKILL_VALID(SID_OathDefense))
    [SID_OathDefense] = MSG_SKILL_OathDefense,
#endif

#if (defined(SID_OathLuck) && COMMON_SKILL_VALID(SID_OathLuck))
    [SID_OathLuck] = MSG_SKILL_OathLuck,
#endif

#if (defined(SID_OathMagic) && COMMON_SKILL_VALID(SID_OathMagic))
    [SID_OathMagic] = MSG_SKILL_OathMagic,
#endif

#if (defined(SID_CounterRoar) && COMMON_SKILL_VALID(SID_CounterRoar))
    [SID_CounterRoar] = MSG_SKILL_CounterRoar,
#endif

#if (defined(SID_Gambit) && COMMON_SKILL_VALID(SID_Gambit))
    [SID_Gambit] = MSG_SKILL_Gambit,
#endif

#if (defined(SID_MagicGambit) && COMMON_SKILL_VALID(SID_MagicGambit))
    [SID_MagicGambit] = MSG_SKILL_MagicGambit,
#endif

#if (defined(SID_BeastAssault) && COMMON_SKILL_VALID(SID_BeastAssault))
    [SID_BeastAssault] = MSG_SKILL_BeastAssault,
#endif

#if (defined(SID_LightAndDark) && COMMON_SKILL_VALID(SID_LightAndDark))
    [SID_LightAndDark] = MSG_SKILL_LightAndDark,
#endif

#if (defined(SID_OathMovement) && COMMON_SKILL_VALID(SID_OathMovement))
    [SID_OathMovement] = MSG_SKILL_OathMovement,
#endif

#if (defined(SID_OathResistance) && COMMON_SKILL_VALID(SID_OathResistance))
    [SID_OathResistance] = MSG_SKILL_OathResistance,
#endif

#if (defined(SID_OathSkill) && COMMON_SKILL_VALID(SID_OathSkill))
    [SID_OathSkill] = MSG_SKILL_OathSkill,
#endif

#if (defined(SID_OathSpeed) && COMMON_SKILL_VALID(SID_OathSpeed))
    [SID_OathSpeed] = MSG_SKILL_OathSpeed,
#endif

#if (defined(SID_OathStrength) && COMMON_SKILL_VALID(SID_OathStrength))
    [SID_OathStrength] = MSG_SKILL_OathStrength,
#endif

#if (defined(SID_DivinePulse) && COMMON_SKILL_VALID(SID_DivinePulse))
    [SID_DivinePulse] = MSG_SKILL_DivinePulse,
#endif

#if (defined(SID_Tsuzuku) && COMMON_SKILL_VALID(SID_Tsuzuku))
    [SID_Tsuzuku] = MSG_SKILL_Tsuzuku,
#endif

#if (defined(SID_MysticBoost) && COMMON_SKILL_VALID(SID_MysticBoost))
    [SID_MysticBoost] = MSG_SKILL_MysticBoost,
#endif

#if (defined(SID_GuardBearing) && COMMON_SKILL_VALID(SID_GuardBearing))
    [SID_GuardBearing] = MSG_SKILL_GuardBearing,
#endif

#if (defined(SID_Spurn) && COMMON_SKILL_VALID(SID_Spurn))
    [SID_Spurn] = MSG_SKILL_Spurn,
#endif

#if (defined(SID_Bushido) && COMMON_SKILL_VALID(SID_Bushido))
    [SID_Bushido] = MSG_SKILL_Bushido,
#endif

#if (defined(SID_DragonWall) && COMMON_SKILL_VALID(SID_DragonWall))
    [SID_DragonWall] = MSG_SKILL_DragonWall,
#endif

#if (defined(SID_DragonWarth) && COMMON_SKILL_VALID(SID_DragonWarth))
    [SID_DragonWarth] = MSG_SKILL_DragonWarth,
#endif

#if (defined(SID_BlueLionRule) && COMMON_SKILL_VALID(SID_BlueLionRule))
    [SID_BlueLionRule] = MSG_SKILL_BlueLionRule,
#endif

#if (defined(SID_FaerghusAncestry) && COMMON_SKILL_VALID(SID_FaerghusAncestry))
    [SID_FaerghusAncestry] = MSG_SKILL_FaerghusAncestry,
#endif

#if (defined(SID_CrusaderWard) && COMMON_SKILL_VALID(SID_CrusaderWard))
    [SID_CrusaderWard] = MSG_SKILL_CrusaderWard,
#endif

#if (defined(SID_YuneWhispers) && COMMON_SKILL_VALID(SID_YuneWhispers))
    [SID_YuneWhispers] = MSG_SKILL_YuneWhispers,
#endif

#if (defined(SID_Plus) && COMMON_SKILL_VALID(SID_Plus))
    [SID_Plus] = MSG_SKILL_Plus,
#endif

#if (defined(SID_Minus) && COMMON_SKILL_VALID(SID_Minus))
    [SID_Minus] = MSG_SKILL_Minus,
#endif

#if (defined(SID_Chlorophyll) && COMMON_SKILL_VALID(SID_Chlorophyll))
    [SID_Chlorophyll] = MSG_SKILL_Chlorophyll,
#endif

#if (defined(SID_SlushRush) && COMMON_SKILL_VALID(SID_SlushRush))
    [SID_SlushRush] = MSG_SKILL_SlushRush,
#endif

#if (defined(SID_SandRush) && COMMON_SKILL_VALID(SID_SandRush))
    [SID_SandRush] = MSG_SKILL_SandRush,
#endif

#if (defined(SID_SwiftSwim) && COMMON_SKILL_VALID(SID_SwiftSwim))
    [SID_SwiftSwim] = MSG_SKILL_SwiftSwim,
#endif

#if (defined(SID_RainDish) && COMMON_SKILL_VALID(SID_RainDish))
    [SID_RainDish] = MSG_SKILL_RainDish,
#endif

#if (defined(SID_Assist) && COMMON_SKILL_VALID(SID_Assist))
    [SID_Assist] = MSG_SKILL_Assist,
#endif

#if (defined(SID_Synergism) && COMMON_SKILL_VALID(SID_Synergism))
    [SID_Synergism] = MSG_SKILL_Synergism,
#endif

#if (defined(SID_Pass) && COMMON_SKILL_VALID(SID_Pass))
    [SID_Pass] = MSG_SKILL_Pass,
#endif

#if (defined(SID_Obstruct) && COMMON_SKILL_VALID(SID_Obstruct))
    [SID_Obstruct] = MSG_SKILL_Obstruct,
#endif

#if (defined(SID_Guidance) && COMMON_SKILL_VALID(SID_Guidance))
    [SID_Guidance] = MSG_SKILL_Guidance,
#endif

#if (defined(SID_FlierGuidance) && COMMON_SKILL_VALID(SID_FlierGuidance))
    [SID_FlierGuidance] = MSG_SKILL_FlierGuidance,
#endif

#if (defined(SID_SoaringGuidance) && COMMON_SKILL_VALID(SID_SoaringGuidance))
    [SID_SoaringGuidance] = MSG_SKILL_SoaringGuidance,
#endif

#if (defined(SID_Aerobatics) && COMMON_SKILL_VALID(SID_Aerobatics))
    [SID_Aerobatics] = MSG_SKILL_Aerobatics,
#endif

#if (defined(SID_FlierFormation) && COMMON_SKILL_VALID(SID_FlierFormation))
    [SID_FlierFormation] = MSG_SKILL_FlierFormation,
#endif

#if (defined(SID_SoaringWings) && COMMON_SKILL_VALID(SID_SoaringWings))
    [SID_SoaringWings] = MSG_SKILL_SoaringWings,
#endif

#if (defined(SID_DetailedReport) && COMMON_SKILL_VALID(SID_DetailedReport))
    [SID_DetailedReport] = MSG_SKILL_DetailedReport,
#endif

#if (defined(SID_VengefulFighter) && COMMON_SKILL_VALID(SID_VengefulFighter))
    [SID_VengefulFighter] = MSG_SKILL_VengefulFighter,
#endif

#if (defined(SID_RecklessFighter) && COMMON_SKILL_VALID(SID_RecklessFighter))
    [SID_RecklessFighter] = MSG_SKILL_RecklessFighter,
#endif

#if (defined(SID_BoldFighter) && COMMON_SKILL_VALID(SID_BoldFighter))
    [SID_BoldFighter] = MSG_SKILL_BoldFighter,
#endif

#if (defined(SID_Dazzle) && COMMON_SKILL_VALID(SID_Dazzle))
    [SID_Dazzle] = MSG_SKILL_Dazzle,
#endif

#if (defined(SID_Boon) && COMMON_SKILL_VALID(SID_Boon))
    [SID_Boon] = MSG_SKILL_Boon,
#endif

#if (defined(SID_BidingBlow) && COMMON_SKILL_VALID(SID_BidingBlow))
    [SID_BidingBlow] = MSG_SKILL_BidingBlow,
#endif

#if (defined(SID_Discipline) && COMMON_SKILL_VALID(SID_Discipline))
    [SID_Discipline] = MSG_SKILL_Discipline,
#endif

#if (defined(SID_AdvantageChaser) && COMMON_SKILL_VALID(SID_AdvantageChaser))
    [SID_AdvantageChaser] = MSG_SKILL_AdvantageChaser,
#endif

#if (defined(SID_Nonconforming) && COMMON_SKILL_VALID(SID_Nonconforming))
    [SID_Nonconforming] = MSG_SKILL_Nonconforming,
#endif

#if (defined(SID_Moonlight) && COMMON_SKILL_VALID(SID_Moonlight))
    [SID_Moonlight] = MSG_SKILL_Moonlight,
#endif

#if (defined(SID_RouseDefense) && COMMON_SKILL_VALID(SID_RouseDefense))
    [SID_RouseDefense] = MSG_SKILL_RouseDefense,
#endif

#if (defined(SID_RouseLuck) && COMMON_SKILL_VALID(SID_RouseLuck))
    [SID_RouseLuck] = MSG_SKILL_RouseLuck,
#endif

#if (defined(SID_RouseMagic) && COMMON_SKILL_VALID(SID_RouseMagic))
    [SID_RouseMagic] = MSG_SKILL_RouseMagic,
#endif

#if (defined(SID_RouseMovement) && COMMON_SKILL_VALID(SID_RouseMovement))
    [SID_RouseMovement] = MSG_SKILL_RouseMovement,
#endif

#if (defined(SID_RouseResistance) && COMMON_SKILL_VALID(SID_RouseResistance))
    [SID_RouseResistance] = MSG_SKILL_RouseResistance,
#endif

#if (defined(SID_RouseSkill) && COMMON_SKILL_VALID(SID_RouseSkill))
    [SID_RouseSkill] = MSG_SKILL_RouseSkill,
#endif

#if (defined(SID_RouseSpeed) && COMMON_SKILL_VALID(SID_RouseSpeed))
    [SID_RouseSpeed] = MSG_SKILL_RouseSpeed,
#endif

#if (defined(SID_RouseStrength) && COMMON_SKILL_VALID(SID_RouseStrength))
    [SID_RouseStrength] = MSG_SKILL_RouseStrength,
#endif

#if (defined(SID_BlackMagic) && COMMON_SKILL_VALID(SID_BlackMagic))
    [SID_BlackMagic] = MSG_SKILL_BlackMagic,
#endif

#if (defined(SID_Relief) && COMMON_SKILL_VALID(SID_Relief))
    [SID_Relief] = MSG_SKILL_Relief,
#endif

#if (defined(SID_Camaraderie) && COMMON_SKILL_VALID(SID_Camaraderie))
    [SID_Camaraderie] = MSG_SKILL_Camaraderie,
#endif

#if (defined(SID_Amaterasu) && (SID_Amaterasu < MAX_SKILL_NUM))
    [SID_Amaterasu] = MSG_SKILL_Amaterasu,
#endif

#if (defined(SID_Counter) && COMMON_SKILL_VALID(SID_Counter))
    [SID_Counter] = MSG_SKILL_Counter,
#endif

#if (defined(SID_LastWord) && COMMON_SKILL_VALID(SID_LastWord))
    [SID_LastWord] = MSG_SKILL_LastWord,
#endif

#if (defined(SID_PowerStaff) && COMMON_SKILL_VALID(SID_PowerStaff))
    [SID_PowerStaff] = MSG_SKILL_PowerStaff,
#endif

#if (defined(SID_Multiscale) && COMMON_SKILL_VALID(SID_Multiscale))
    [SID_Multiscale] = MSG_SKILL_Multiscale,
#endif

#if (defined(SID_PridefulWarrior) && COMMON_SKILL_VALID(SID_PridefulWarrior))
    [SID_PridefulWarrior] = MSG_SKILL_PridefulWarrior,
#endif

#if (defined(SID_LiveToServe) && COMMON_SKILL_VALID(SID_LiveToServe))
    [SID_LiveToServe] = MSG_SKILL_LiveToServe,
#endif

#if (defined(SID_MageSlayer) && COMMON_SKILL_VALID(SID_MageSlayer))
    [SID_MageSlayer] = MSG_SKILL_MageSlayer,
#endif

#if (defined(SID_FailGale) && COMMON_SKILL_VALID(SID_FailGale))
    [SID_FailGale] = MSG_SKILL_FailGale,
#endif

#if (defined(SID_Clearance) && COMMON_SKILL_VALID(SID_Clearance))
    [SID_Clearance] = MSG_SKILL_Clearance,
#endif

#if (defined(SID_TriangleAdept) && COMMON_SKILL_VALID(SID_TriangleAdept))
    [SID_TriangleAdept] = MSG_SKILL_TriangleAdept,
#endif

#if (defined(SID_TriangleAdeptPlus) && COMMON_SKILL_VALID(SID_TriangleAdeptPlus))
    [SID_TriangleAdeptPlus] = MSG_SKILL_TriangleAdeptPlus,
#endif

#if (defined(SID_TowerShieldPlus) && COMMON_SKILL_VALID(SID_TowerShieldPlus))
    [SID_TowerShieldPlus] = MSG_SKILL_TowerShieldPlus,
#endif

#if (defined(SID_SolarPower) && COMMON_SKILL_VALID(SID_SolarPower))
    [SID_SolarPower] = MSG_SKILL_SolarPower,
#endif

#if (defined(SID_COMBAT_Gamble) && COMMON_SKILL_VALID(SID_COMBAT_Gamble))
    [SID_COMBAT_Gamble] = MSG_SKILL_COMBAT_Gamble,
#endif

#if (defined(SID_PoisonStrike) && COMMON_SKILL_VALID(SID_PoisonStrike))
    [SID_PoisonStrike] = MSG_SKILL_PoisonStrike,
#endif

#if (defined(SID_GrislyWound) && COMMON_SKILL_VALID(SID_GrislyWound))
    [SID_GrislyWound] = MSG_SKILL_GrislyWound,
#endif

#if (defined(SID_Poise) && COMMON_SKILL_VALID(SID_Poise))
    [SID_Poise] = MSG_SKILL_Poise,
#endif

#if (defined(SID_COMBAT_Detonate) && COMMON_SKILL_VALID(SID_COMBAT_Detonate))
    [SID_COMBAT_Detonate] = MSG_SKILL_COMBAT_Detonate,
#endif

#if (defined(SID_Enrage) && COMMON_SKILL_VALID(SID_Enrage))
    [SID_Enrage] = MSG_SKILL_Enrage,
#endif

#if (defined(SID_Petrify) && COMMON_SKILL_VALID(SID_Petrify))
    [SID_Petrify] = MSG_SKILL_Petrify,
#endif

#if (defined(SID_AirRaidAttack) && COMMON_SKILL_VALID(SID_AirRaidAttack))
    [SID_AirRaidAttack] = MSG_SKILL_AirRaidAttack,
#endif

#if (defined(SID_AirRaidAvoid) && COMMON_SKILL_VALID(SID_AirRaidAvoid))
    [SID_AirRaidAvoid] = MSG_SKILL_AirRaidAvoid,
#endif

#if (defined(SID_AirRaidCrit) && COMMON_SKILL_VALID(SID_AirRaidCrit))
    [SID_AirRaidCrit] = MSG_SKILL_AirRaidCrit,
#endif

#if (defined(SID_AirRaidDefense) && COMMON_SKILL_VALID(SID_AirRaidDefense))
    [SID_AirRaidDefense] = MSG_SKILL_AirRaidDefense,
#endif

#if (defined(SID_AirRaidHit) && COMMON_SKILL_VALID(SID_AirRaidHit))
    [SID_AirRaidHit] = MSG_SKILL_AirRaidHit,
#endif

#if (defined(SID_AirRaidResistance) && COMMON_SKILL_VALID(SID_AirRaidResistance))
    [SID_AirRaidResistance] = MSG_SKILL_AirRaidResistance,
#endif

#if (defined(SID_AirRaidSpeed) && COMMON_SKILL_VALID(SID_AirRaidSpeed))
    [SID_AirRaidSpeed] = MSG_SKILL_AirRaidSpeed,
#endif

#if (defined(SID_FaireAnima) && COMMON_SKILL_VALID(SID_FaireAnima))
    [SID_FaireAnima] = MSG_SKILL_FaireAnima,
#endif

#if (defined(SID_FaireLight) && COMMON_SKILL_VALID(SID_FaireLight))
    [SID_FaireLight] = MSG_SKILL_FaireLight,
#endif

#if (defined(SID_FaireDark) && COMMON_SKILL_VALID(SID_FaireDark))
    [SID_FaireDark] = MSG_SKILL_FaireDark,
#endif

#if (defined(SID_SealDefense) && COMMON_SKILL_VALID(SID_SealDefense))
    [SID_SealDefense] = MSG_SKILL_SealDefense,
#endif

#if (defined(SID_SealLuck) && COMMON_SKILL_VALID(SID_SealLuck))
    [SID_SealLuck] = MSG_SKILL_SealLuck,
#endif

#if (defined(SID_SealMagic) && COMMON_SKILL_VALID(SID_SealMagic))
    [SID_SealMagic] = MSG_SKILL_SealMagic,
#endif

#if (defined(SID_SealResistance) && COMMON_SKILL_VALID(SID_SealResistance))
    [SID_SealResistance] = MSG_SKILL_SealResistance,
#endif

#if (defined(SID_SealSkill) && COMMON_SKILL_VALID(SID_SealSkill))
    [SID_SealSkill] = MSG_SKILL_SealSkill,
#endif

#if (defined(SID_SealSpeed) && COMMON_SKILL_VALID(SID_SealSpeed))
    [SID_SealSpeed] = MSG_SKILL_SealSpeed,
#endif

#if (defined(SID_SealStrength) && COMMON_SKILL_VALID(SID_SealStrength))
    [SID_SealStrength] = MSG_SKILL_SealStrength,
#endif

#if (defined(SID_RallyDefense) && COMMON_SKILL_VALID(SID_RallyDefense))
    [SID_RallyDefense] = MSG_SKILL_RallyDefense,
#endif

#if (defined(SID_RallyLuck) && COMMON_SKILL_VALID(SID_RallyLuck))
    [SID_RallyLuck] = MSG_SKILL_RallyLuck,
#endif

#if (defined(SID_RallyMagic) && COMMON_SKILL_VALID(SID_RallyMagic))
    [SID_RallyMagic] = MSG_SKILL_RallyMagic,
#endif

#if (defined(SID_RallyMovement) && COMMON_SKILL_VALID(SID_RallyMovement))
    [SID_RallyMovement] = MSG_SKILL_RallyMovement,
#endif

#if (defined(SID_RallyResistance) && COMMON_SKILL_VALID(SID_RallyResistance))
    [SID_RallyResistance] = MSG_SKILL_RallyResistance,
#endif

#if (defined(SID_RallySkill) && COMMON_SKILL_VALID(SID_RallySkill))
    [SID_RallySkill] = MSG_SKILL_RallySkill,
#endif

#if (defined(SID_RallySpeed) && COMMON_SKILL_VALID(SID_RallySpeed))
    [SID_RallySpeed] = MSG_SKILL_RallySpeed,
#endif

#if (defined(SID_RallyStrength) && COMMON_SKILL_VALID(SID_RallyStrength))
    [SID_RallyStrength] = MSG_SKILL_RallyStrength,
#endif

#if (defined(SID_RallySpectrum) && COMMON_SKILL_VALID(SID_RallySpectrum))
    [SID_RallySpectrum] = MSG_SKILL_RallySpectrum,
#endif

#if (defined(SID_Despoil) && COMMON_SKILL_VALID(SID_Despoil))
    [SID_Despoil] = MSG_SKILL_Despoil,
#endif

#if (defined(SID_Pickup) && COMMON_SKILL_VALID(SID_Pickup))
    [SID_Pickup] = MSG_SKILL_Pickup,
#endif

#if (defined(SID_IndoorMarch) && COMMON_SKILL_VALID(SID_IndoorMarch))
    [SID_IndoorMarch] = MSG_SKILL_IndoorMarch,
#endif

#if (defined(SID_DevilsPact) && COMMON_SKILL_VALID(SID_DevilsPact))
    [SID_DevilsPact] = MSG_SKILL_DevilsPact,
#endif

#if (defined(SID_DevilsWhim) && COMMON_SKILL_VALID(SID_DevilsWhim))
    [SID_DevilsWhim] = MSG_SKILL_DevilsWhim,
#endif

#if (defined(SID_DevilsLuck) && COMMON_SKILL_VALID(SID_DevilsLuck))
    [SID_DevilsLuck] = MSG_SKILL_DevilsLuck,
#endif

#if (defined(SID_Expertise) && COMMON_SKILL_VALID(SID_Expertise))
    [SID_Expertise] = MSG_SKILL_Expertise,
#endif

#if (defined(SID_PassionsFlow) && COMMON_SKILL_VALID(SID_PassionsFlow))
    [SID_PassionsFlow] = MSG_SKILL_PassionsFlow,
#endif

#if (defined(SID_NatureRush) && COMMON_SKILL_VALID(SID_NatureRush))
    [SID_NatureRush] = MSG_SKILL_NatureRush,
#endif

#if (defined(SID_CounterMagic) && COMMON_SKILL_VALID(SID_CounterMagic))
    [SID_CounterMagic] = MSG_SKILL_CounterMagic,
#endif

#if (defined(SID_LadyBlade) && COMMON_SKILL_VALID(SID_LadyBlade))
    [SID_LadyBlade] = MSG_SKILL_LadyBlade,
#endif

#if (defined(SID_LullDefense) && COMMON_SKILL_VALID(SID_LullDefense))
    [SID_LullDefense] = MSG_SKILL_LullDefense,
#endif

#if (defined(SID_LullLuck) && COMMON_SKILL_VALID(SID_LullLuck))
    [SID_LullLuck] = MSG_SKILL_LullLuck,
#endif

#if (defined(SID_LullMagic) && COMMON_SKILL_VALID(SID_LullMagic))
    [SID_LullMagic] = MSG_SKILL_LullMagic,
#endif

#if (defined(SID_LullMovement) && COMMON_SKILL_VALID(SID_LullMovement))
    [SID_LullMovement] = MSG_SKILL_LullMovement,
#endif

#if (defined(SID_LullResistance) && COMMON_SKILL_VALID(SID_LullResistance))
    [SID_LullResistance] = MSG_SKILL_LullResistance,
#endif

#if (defined(SID_LullSkill) && COMMON_SKILL_VALID(SID_LullSkill))
    [SID_LullSkill] = MSG_SKILL_LullSkill,
#endif

#if (defined(SID_LullSpectrum) && COMMON_SKILL_VALID(SID_LullSpectrum))
    [SID_LullSpectrum] = MSG_SKILL_LullSpectrum,
#endif

#if (defined(SID_LullSpeed) && COMMON_SKILL_VALID(SID_LullSpeed))
    [SID_LullSpeed] = MSG_SKILL_LullSpeed,
#endif

#if (defined(SID_LullStrength) && COMMON_SKILL_VALID(SID_LullStrength))
    [SID_LullStrength] = MSG_SKILL_LullStrength,
#endif

#if (defined(SID_ShrewdPotential) && COMMON_SKILL_VALID(SID_ShrewdPotential))
    [SID_ShrewdPotential] = MSG_SKILL_ShrewdPotential,
#endif

#if (defined(SID_GoddessDance) && COMMON_SKILL_VALID(SID_GoddessDance))
    [SID_GoddessDance] = MSG_SKILL_GoddessDance,
#endif

#if (defined(SID_SteadyBrawler) && COMMON_SKILL_VALID(SID_SteadyBrawler))
    [SID_SteadyBrawler] = MSG_SKILL_SteadyBrawler,
#endif

#if (defined(SID_KeepUp) && COMMON_SKILL_VALID(SID_KeepUp))
    [SID_KeepUp] = MSG_SKILL_KeepUp,
#endif

#if (defined(SID_LiquidOoze) && COMMON_SKILL_VALID(SID_LiquidOoze))
    [SID_LiquidOoze] = MSG_SKILL_LiquidOoze,
#endif

#if (defined(SID_CloseCombat) && COMMON_SKILL_VALID(SID_CloseCombat))
    [SID_CloseCombat] = MSG_SKILL_CloseCombat,
#endif

#if (defined(SID_ArmorMarch) && COMMON_SKILL_VALID(SID_ArmorMarch))
    [SID_ArmorMarch] = MSG_SKILL_ArmorMarch,
#endif

#if (defined(SID_Eclipse) && COMMON_SKILL_VALID(SID_Eclipse))
    [SID_Eclipse] = MSG_SKILL_Eclipse,
#endif

#if (defined(SID_Barricade) && COMMON_SKILL_VALID(SID_Barricade))
    [SID_Barricade] = MSG_SKILL_Barricade,
#endif

#if (defined(SID_BarricadePlus) && COMMON_SKILL_VALID(SID_BarricadePlus))
    [SID_BarricadePlus] = MSG_SKILL_BarricadePlus,
#endif

#if (defined(SID_LunarBrace) && COMMON_SKILL_VALID(SID_LunarBrace))
    [SID_LunarBrace] = MSG_SKILL_LunarBrace,
#endif

#if (defined(SID_SoulSap) && COMMON_SKILL_VALID(SID_SoulSap))
    [SID_SoulSap] = MSG_SKILL_SoulSap,
#endif

#if (defined(SID_Pursuer) && COMMON_SKILL_VALID(SID_Pursuer))
    [SID_Pursuer] = MSG_SKILL_Pursuer,
#endif

#if (defined(SID_Stride) && COMMON_SKILL_VALID(SID_Stride))
    [SID_Stride] = MSG_SKILL_Stride,
#endif

#if (defined(SID_QuickLearner) && COMMON_SKILL_VALID(SID_QuickLearner))
    [SID_QuickLearner] = MSG_SKILL_QuickLearner,
#endif

#if (defined(SID_GoldDigger) && COMMON_SKILL_VALID(SID_GoldDigger))
    [SID_GoldDigger] = MSG_SKILL_GoldDigger,
#endif

#if (defined(SID_EffectSpore) && COMMON_SKILL_VALID(SID_EffectSpore))
    [SID_EffectSpore] = MSG_SKILL_EffectSpore,
#endif

#if (defined(SID_DominantArm) && COMMON_SKILL_VALID(SID_DominantArm))
    [SID_DominantArm] = MSG_SKILL_DominantArm,
#endif

#if (defined(SID_GaleWings) && COMMON_SKILL_VALID(SID_GaleWings))
    [SID_GaleWings] = MSG_SKILL_GaleWings,
#endif

#if (defined(SID_SolidRock) && COMMON_SKILL_VALID(SID_SolidRock))
    [SID_SolidRock] = MSG_SKILL_SolidRock,
#endif

#if (defined(SID_Sturdy) && COMMON_SKILL_VALID(SID_Sturdy))
    [SID_Sturdy] = MSG_SKILL_Sturdy,
#endif

#if (defined(SID_FranticSwing) && COMMON_SKILL_VALID(SID_FranticSwing))
    [SID_FranticSwing] = MSG_SKILL_FranticSwing,
#endif

#if (defined(SID_SocialButterfly) && COMMON_SKILL_VALID(SID_SocialButterfly))
    [SID_SocialButterfly] = MSG_SKILL_SocialButterfly,
#endif

#if (defined(SID_MeleeManiac) && COMMON_SKILL_VALID(SID_MeleeManiac))
    [SID_MeleeManiac] = MSG_SKILL_MeleeManiac,
#endif

#if (defined(SID_CriticalOverload) && COMMON_SKILL_VALID(SID_CriticalOverload))
    [SID_CriticalOverload] = MSG_SKILL_CriticalOverload,
#endif

#if (defined(SID_Swarp) && COMMON_SKILL_VALID(SID_Swarp))
    [SID_Swarp] = MSG_SKILL_Swarp,
#endif

#if (defined(SID_Blacksmith) && COMMON_SKILL_VALID(SID_Blacksmith))
    [SID_Blacksmith] = MSG_SKILL_Blacksmith,
#endif

#if (defined(SID_Deal) && COMMON_SKILL_VALID(SID_Deal))
    [SID_Deal] = MSG_SKILL_Deal,
#endif

#if (defined(SID_Bargain) && COMMON_SKILL_VALID(SID_Bargain))
    [SID_Bargain] = MSG_SKILL_Bargain,
#endif
};
