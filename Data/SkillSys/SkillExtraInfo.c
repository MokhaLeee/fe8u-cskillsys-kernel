#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct SkillExtraInfo gSkillExtraInfo[MAX_SKILL_NUM + 1] = {
#if (defined(SID_HpBonus) && COMMON_SKILL_VALID(SID_HpBonus))
	[SID_HpBonus] = {{ 5 }},
#endif

#if (defined(SID_StrBonus) && COMMON_SKILL_VALID(SID_StrBonus))
	[SID_StrBonus] = {{ 2 }},
#endif

#if (defined(SID_MagBonus) && COMMON_SKILL_VALID(SID_MagBonus))
	[SID_MagBonus] = {{ 2 }},
#endif

#if (defined(SID_SklBonus) && COMMON_SKILL_VALID(SID_SklBonus))
	[SID_SklBonus] = {{ 2 }},
#endif

#if (defined(SID_SpdBonus) && COMMON_SKILL_VALID(SID_SpdBonus))
	[SID_SpdBonus] = {{ 2 }},
#endif

#if (defined(SID_LckBonus) && COMMON_SKILL_VALID(SID_LckBonus))
	[SID_LckBonus] = {{ 2 }},
#endif

#if (defined(SID_DefBonus) && COMMON_SKILL_VALID(SID_DefBonus))
	[SID_DefBonus] = {{ 2 }},
#endif

#if (defined(SID_ResBonus) && COMMON_SKILL_VALID(SID_ResBonus))
	[SID_ResBonus] = {{ 2 }},
#endif

#if (defined(SID_MovBonus) && COMMON_SKILL_VALID(SID_MovBonus))
	[SID_MovBonus] = {{ 2 }},
#endif

#if (defined(SID_DefiantStr) && COMMON_SKILL_VALID(SID_DefiantStr))
	[SID_DefiantStr] = {{ 7 }},
#endif

#if (defined(SID_DefiantMag) && COMMON_SKILL_VALID(SID_DefiantMag))
	[SID_DefiantMag] = {{ 7 }},
#endif

#if (defined(SID_DefiantSkl) && COMMON_SKILL_VALID(SID_DefiantSkl))
	[SID_DefiantSkl] = {{ 7 }},
#endif

#if (defined(SID_DefiantSpd) && COMMON_SKILL_VALID(SID_DefiantSpd))
	[SID_DefiantSpd] = {{ 7 }},
#endif

#if (defined(SID_DefiantLck) && COMMON_SKILL_VALID(SID_DefiantLck))
	[SID_DefiantLck] = {{ 7 }},
#endif

#if (defined(SID_DefiantDef) && COMMON_SKILL_VALID(SID_DefiantDef))
	[SID_DefiantDef] = {{ 7 }},
#endif

#if (defined(SID_DefiantRes) && COMMON_SKILL_VALID(SID_DefiantRes))
	[SID_DefiantRes] = {{ 7 }},
#endif

#if (defined(SID_DefiantCrit) && COMMON_SKILL_VALID(SID_DefiantCrit))
	[SID_DefiantCrit] = {{ 50 }},
#endif

#if (defined(SID_DefiantAvoid) && COMMON_SKILL_VALID(SID_DefiantAvoid))
	[SID_DefiantAvoid] = {{ 30 }},
#endif

#if (defined(SID_PushStrength) && COMMON_SKILL_VALID(SID_PushStrength))
	[SID_PushStrength] = {{ 5 }},
#endif

#if (defined(SID_PushMagic) && COMMON_SKILL_VALID(SID_PushMagic))
	[SID_PushMagic] = {{ 5 }},
#endif

#if (defined(SID_PushSkill) && COMMON_SKILL_VALID(SID_PushSkill))
	[SID_PushSkill] = {{ 5 }},
#endif

#if (defined(SID_PushSpeed) && COMMON_SKILL_VALID(SID_PushSpeed))
	[SID_PushSpeed] = {{ 5 }},
#endif

#if (defined(SID_PushLuck) && COMMON_SKILL_VALID(SID_PushLuck))
	[SID_PushLuck] = {{ 5 }},
#endif

#if (defined(SID_PushDefense) && COMMON_SKILL_VALID(SID_PushDefense))
	[SID_PushDefense] = {{ 5 }},
#endif

#if (defined(SID_PushResistance) && COMMON_SKILL_VALID(SID_PushResistance))
	[SID_PushResistance] = {{ 5 }},
#endif

#if (defined(SID_PushMovement) && COMMON_SKILL_VALID(SID_PushMovement))
	[SID_PushMovement] = {{ 5 }},
#endif

#if (defined(SID_PushSpectrum) && COMMON_SKILL_VALID(SID_PushSpectrum))
	[SID_PushSpectrum] = {{ 5 }},
#endif

#if (defined(SID_Fury) && COMMON_SKILL_VALID(SID_Fury))
	[SID_Fury] = {{ 3 }},
#endif

#if (defined(SID_FuryPlus) && COMMON_SKILL_VALID(SID_FuryPlus))
	[SID_FuryPlus] = {{ 4 }},
#endif

#if (defined(SID_EvenFooted) && COMMON_SKILL_VALID(SID_EvenFooted))
	[SID_EvenFooted] = {{ 1 }},
#endif

#if (defined(SID_OddFooted) && COMMON_SKILL_VALID(SID_OddFooted))
	[SID_OddFooted] = {{ 1 }},
#endif

#if (defined(SID_DefenseBoost) && COMMON_SKILL_VALID(SID_DefenseBoost))
	[SID_DefenseBoost] = {{ 6 }},
#endif

#if (defined(SID_LuckBoost) && COMMON_SKILL_VALID(SID_LuckBoost))
	[SID_LuckBoost] = {{ 6 }},
#endif

#if (defined(SID_MagicBoost) && COMMON_SKILL_VALID(SID_MagicBoost))
	[SID_MagicBoost] = {{ 6 }},
#endif

#if (defined(SID_MoveBoost) && COMMON_SKILL_VALID(SID_MoveBoost))
	[SID_MoveBoost] = {{ 6 }},
#endif

#if (defined(SID_ResistanceBoost) && COMMON_SKILL_VALID(SID_ResistanceBoost))
	[SID_ResistanceBoost] = {{ 6 }},
#endif

#if (defined(SID_SpeedBoost) && COMMON_SKILL_VALID(SID_SpeedBoost))
	[SID_SpeedBoost] = {{ 6 }},
#endif

#if (defined(SID_SkillBoost) && COMMON_SKILL_VALID(SID_SkillBoost))
	[SID_SkillBoost] = {{ 6 }},
#endif

#if (defined(SID_StrengthBoost) && COMMON_SKILL_VALID(SID_StrengthBoost))
	[SID_StrengthBoost] = {{ 6 }},
#endif

#if (defined(SID_SpectrumBoost) && COMMON_SKILL_VALID(SID_SpectrumBoost))
	[SID_SpectrumBoost] = {{ 6 }},
#endif

#if (defined(SID_LuckySeven) && COMMON_SKILL_VALID(SID_LuckySeven))
	[SID_LuckySeven] = {{ 7 }},
#endif

#if (defined(SID_Poise) && COMMON_SKILL_VALID(SID_Poise))
	[SID_Poise] = {{ 1 }},
#endif

#if (defined(SID_ShrewdPotential) && COMMON_SKILL_VALID(SID_ShrewdPotential))
	[SID_ShrewdPotential] = {{ 1 }},
#endif

#if (defined(SID_FortressDef) && COMMON_SKILL_VALID(SID_FortressDef))
	[SID_FortressDef] = {{ -3, 5 }},
#endif

#if (defined(SID_FortressRes) && COMMON_SKILL_VALID(SID_FortressRes))
	[SID_FortressRes] = {{ -3, 5 }},
#endif

#if (defined(SID_LifeAndDeath) && COMMON_SKILL_VALID(SID_LifeAndDeath))
	[SID_LifeAndDeath] = {{ 5, -5 }},
#endif

#if (defined(SID_CritSword) && COMMON_SKILL_VALID(SID_CritSword))
	[SID_CritSword] = {{ 10 }},
#endif

#if (defined(SID_CritAxe) && COMMON_SKILL_VALID(SID_CritAxe))
	[SID_CritAxe] = {{ 10 }},
#endif

#if (defined(SID_CritLance) && COMMON_SKILL_VALID(SID_CritLance))
	[SID_CritLance] = {{ 10 }},
#endif

#if (defined(SID_CritBow) && COMMON_SKILL_VALID(SID_CritBow))
	[SID_CritBow] = {{ 10 }},
#endif

#if (defined(SID_FaireSword) && COMMON_SKILL_VALID(SID_FaireSword))
	[SID_FaireSword] = {{ 5 }},
#endif

#if (defined(SID_FaireLance) && COMMON_SKILL_VALID(SID_FaireLance))
	[SID_FaireLance] = {{ 5 }},
#endif

#if (defined(SID_FaireAxe) && COMMON_SKILL_VALID(SID_FaireAxe))
	[SID_FaireAxe] = {{ 5 }},
#endif

#if (defined(SID_FaireBow) && COMMON_SKILL_VALID(SID_FaireBow))
	[SID_FaireBow] = {{ 5 }},
#endif

#if (defined(SID_FaireBMag) && COMMON_SKILL_VALID(SID_FaireBMag))
	[SID_FaireBMag] = {{ 5 }},
#endif

#if (defined(SID_FaireAnima) && COMMON_SKILL_VALID(SID_FaireAnima))
	[SID_FaireAnima] = {{ 5 }},
#endif

#if (defined(SID_FaireLight) && COMMON_SKILL_VALID(SID_FaireLight))
	[SID_FaireLight] = {{ 5 }},
#endif

#if (defined(SID_FaireDark) && COMMON_SKILL_VALID(SID_FaireDark))
	[SID_FaireDark] = {{ 5 }},
#endif

#if (defined(SID_Avoid) && COMMON_SKILL_VALID(SID_Avoid))
	[SID_Avoid] = {{ 10 }},
#endif

#if (defined(SID_AvoidSword) && COMMON_SKILL_VALID(SID_AvoidSword))
	[SID_AvoidSword] = {{ 20 }},
#endif

#if (defined(SID_Vigilance) && COMMON_SKILL_VALID(SID_Vigilance))
	[SID_Vigilance] = {{ 20 }},
#endif

#if (defined(SID_RuinedBlade) && COMMON_SKILL_VALID(SID_RuinedBlade))
	[SID_RuinedBlade] = {{ -5, 5, 5 }},
#endif

#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
	[SID_RuinedBladePlus] = {{ 5, 5, 1 }},
#endif

#if (defined(SID_HeavyBlade) && COMMON_SKILL_VALID(SID_HeavyBlade))
	[SID_HeavyBlade] = {{ 15 }},
#endif

#if (defined(SID_FlashingBlade) && COMMON_SKILL_VALID(SID_FlashingBlade))
	[SID_FlashingBlade] = {{ 15 }},
#endif

#if (defined(SID_HeavyBladePlus) && COMMON_SKILL_VALID(SID_HeavyBladePlus))
	[SID_HeavyBladePlus] = {{ 5, 25 }},
#endif

#if (defined(SID_FlashingBladePlus) && COMMON_SKILL_VALID(SID_FlashingBladePlus))
	[SID_FlashingBladePlus] = {{ 3, 25 }},
#endif

#if (defined(SID_BattleVeteran) && COMMON_SKILL_VALID(SID_BattleVeteran))
	[SID_BattleVeteran] = {{ 1, 5 }},
#endif

#if (defined(SID_ElbowRoom) && COMMON_SKILL_VALID(SID_ElbowRoom))
	[SID_ElbowRoom] = {{ 3 }},
#endif

#if (defined(SID_ChaosStyle) && COMMON_SKILL_VALID(SID_ChaosStyle))
	[SID_ChaosStyle] = {{ 3 }},
#endif

#if (defined(SID_ArcaneBlade) && COMMON_SKILL_VALID(SID_ArcaneBlade))
	[SID_ArcaneBlade] = {{ 3 }},
#endif

#if (defined(SID_QuickDraw) && COMMON_SKILL_VALID(SID_QuickDraw))
	[SID_QuickDraw] = {{ 4 }},
#endif

#if (defined(SID_KeenFighter) && COMMON_SKILL_VALID(SID_KeenFighter))
	[SID_KeenFighter] = {{ 50 }},
#endif

#if (defined(SID_DragonSkin) && COMMON_SKILL_VALID(SID_DragonSkin))
	[SID_DragonSkin] = {{ 50 }},
#endif

#if (defined(SID_WaterBoost) && COMMON_SKILL_VALID(SID_WaterBoost))
	[SID_WaterBoost] = {{ 6, 3 }},
#endif

#if (defined(SID_EarthBoost) && COMMON_SKILL_VALID(SID_EarthBoost))
	[SID_EarthBoost] = {{ 6, 3 }},
#endif

#if (defined(SID_WindBoost) && COMMON_SKILL_VALID(SID_WindBoost))
	[SID_WindBoost] = {{ 6, 3 }},
#endif

#if (defined(SID_FireBoost) && COMMON_SKILL_VALID(SID_FireBoost))
	[SID_FireBoost] = {{ 6, 3 }},
#endif

#if (defined(SID_Pursuit) && COMMON_SKILL_VALID(SID_Pursuit))
	[SID_Pursuit] = {{ 2 }},
#endif

#if (defined(SID_Patience) && COMMON_SKILL_VALID(SID_Patience))
	[SID_Patience] = {{ 10 }},
#endif

#if (defined(SID_StrongRiposte) && COMMON_SKILL_VALID(SID_StrongRiposte))
	[SID_StrongRiposte] = {{ 3 }},
#endif

#if (defined(SID_Perfectionist) && COMMON_SKILL_VALID(SID_Perfectionist))
	[SID_Perfectionist] = {{ 15, 15 }},
#endif

#if (defined(SID_WindDisciple) && COMMON_SKILL_VALID(SID_WindDisciple))
	[SID_WindDisciple] = {{ 10, 10 }},
#endif

#if (defined(SID_Pragmatic) && COMMON_SKILL_VALID(SID_Pragmatic))
	[SID_Pragmatic] = {{ 3, 1 }},
#endif

#if (defined(SID_Chivalry) && COMMON_SKILL_VALID(SID_Chivalry))
	[SID_Chivalry] = {{ 2, 2 }},
#endif

#if (defined(SID_Charge) && COMMON_SKILL_VALID(SID_Charge))
	[SID_Charge] = {{ 1 }},
#endif

#if (defined(SID_FieryBlood) && COMMON_SKILL_VALID(SID_FieryBlood))
	[SID_FieryBlood] = {{ 4 }},
#endif

#if (defined(SID_Wrath) && COMMON_SKILL_VALID(SID_Wrath))
	[SID_Wrath] = {{ 20 }},
#endif

#if (defined(SID_OutdoorFighter) && COMMON_SKILL_VALID(SID_OutdoorFighter))
	[SID_OutdoorFighter] = {{ 10, 10 }},
#endif

#if (defined(SID_KnightAspirant) && COMMON_SKILL_VALID(SID_KnightAspirant))
	[SID_KnightAspirant] = {{ 2, 15 }},
#endif

#if (defined(SID_Outrider) && COMMON_SKILL_VALID(SID_Outrider))
	// [SID_Outrider] = {{ 1, 3 }},
#endif

#if (defined(SID_EvenRhythm) && COMMON_SKILL_VALID(SID_EvenRhythm))
	[SID_EvenRhythm] = {{ 10, 10 }},
#endif

#if (defined(SID_OddRhythm) && COMMON_SKILL_VALID(SID_OddRhythm))
	[SID_OddRhythm] = {{ 10, 10 }},
#endif

#if (defined(SID_Puissance) && COMMON_SKILL_VALID(SID_Puissance))
	[SID_Puissance] = {{ 3 }},
#endif

#if (defined(SID_Prescience) && COMMON_SKILL_VALID(SID_Prescience))
	[SID_Prescience] = {{ 15, 15 }},
#endif

#if (defined(SID_SilentPride) && COMMON_SKILL_VALID(SID_SilentPride))
	[SID_SilentPride] = {{ 2, 2 }},
#endif

#if (defined(SID_RightfulKing) && COMMON_SKILL_VALID(SID_RightfulKing))
	[SID_RightfulKing] = {{ 10 }},
#endif

#if (defined(SID_RightfulGod) && COMMON_SKILL_VALID(SID_RightfulGod))
	[SID_RightfulGod] = {{ 30 }},
#endif

#if (defined(SID_Hero) && COMMON_SKILL_VALID(SID_Hero))
	[SID_Hero] = {{ 30 }},
#endif

#if (defined(SID_Guts) && COMMON_SKILL_VALID(SID_Guts))
	[SID_Guts] = {{ 5 }},
#endif

#if (defined(SID_StrongCon) && COMMON_SKILL_VALID(SID_StrongCon))
	[SID_StrongCon] = {{ 5, 5 }},
#endif

#if (defined(SID_Spurn) && COMMON_SKILL_VALID(SID_Spurn))
	[SID_Spurn] = {{ 5, 4 }},
#endif

#if (defined(SID_Astra) && COMMON_SKILL_VALID(SID_Astra))
	[SID_Astra] = {{ 4, 50 }},
#endif

#if (defined(SID_Aether) && COMMON_SKILL_VALID(SID_Aether))
	[SID_Aether] = {{ 80 }},
#endif

#if (defined(SID_DragonFang) && COMMON_SKILL_VALID(SID_DragonFang))
	[SID_DragonFang] = {{ 50 }},
#endif

#if (defined(SID_Colossus) && COMMON_SKILL_VALID(SID_Colossus))
	[SID_Colossus] = {{ 20 }},
#endif

#if (defined(SID_Impale) && COMMON_SKILL_VALID(SID_Impale))
	[SID_Impale] = {{ 30 }},
#endif

#if (defined(SID_DivinePulse) && COMMON_SKILL_VALID(SID_DivinePulse))
	[SID_DivinePulse] = {{ 30 }},
#endif

#if (defined(SID_Multiscale) && COMMON_SKILL_VALID(SID_Multiscale))
	[SID_Multiscale] = {{ 50 }},
#endif

#if (defined(SID_Expertise) && COMMON_SKILL_VALID(SID_Expertise))
	[SID_Expertise] = {{ 50 }},
#endif

#if (defined(SID_BlowDarting) && COMMON_SKILL_VALID(SID_BlowDarting))
	[SID_BlowDarting] = {{ 6 }},
#endif

#if (defined(SID_BlowDeath) && COMMON_SKILL_VALID(SID_BlowDeath))
	[SID_BlowDeath] = {{ 6 }},
#endif

#if (defined(SID_BlowArmored) && COMMON_SKILL_VALID(SID_BlowArmored))
	[SID_BlowArmored] = {{ 6 }},
#endif

#if (defined(SID_BlowFiendish) && COMMON_SKILL_VALID(SID_BlowFiendish))
	[SID_BlowFiendish] = {{ 6 }},
#endif

#if (defined(SID_BlowWarding) && COMMON_SKILL_VALID(SID_BlowWarding))
	[SID_BlowWarding] = {{ 6 }},
#endif

#if (defined(SID_BlowDuelist) && COMMON_SKILL_VALID(SID_BlowDuelist))
	[SID_BlowDuelist] = {{ 20 }},
#endif

#if (defined(SID_BlowUncanny) && COMMON_SKILL_VALID(SID_BlowUncanny))
	[SID_BlowUncanny] = {{ 30 }},
#endif

#if (defined(SID_BlowKilling) && COMMON_SKILL_VALID(SID_BlowKilling))
	[SID_BlowKilling] = {{ 20 }},
#endif

#if (defined(SID_StanceBracing) && COMMON_SKILL_VALID(SID_StanceBracing))
	[SID_StanceBracing] = {{ 4 }},
#endif

#if (defined(SID_StanceDarting) && COMMON_SKILL_VALID(SID_StanceDarting))
	[SID_StanceDarting] = {{ 6 }},
#endif

#if (defined(SID_StanceFierce) && COMMON_SKILL_VALID(SID_StanceFierce))
	[SID_StanceFierce] = {{ 6 }},
#endif

#if (defined(SID_StanceKestrel) && COMMON_SKILL_VALID(SID_StanceKestrel))
	[SID_StanceKestrel] = {{ 4, 4 }},
#endif

#if (defined(SID_StanceMirror) && COMMON_SKILL_VALID(SID_StanceMirror))
	[SID_StanceMirror] = {{ 4, 4 }},
#endif

#if (defined(SID_StanceReady) && COMMON_SKILL_VALID(SID_StanceReady))
	[SID_StanceReady] = {{ 4, 4 }},
#endif

#if (defined(SID_StanceSteady) && COMMON_SKILL_VALID(SID_StanceSteady))
	[SID_StanceSteady] = {{ 6 }},
#endif

#if (defined(SID_StanceSturdy) && COMMON_SKILL_VALID(SID_StanceSturdy))
	[SID_StanceSturdy] = {{ 4, 4 }},
#endif

#if (defined(SID_StanceSwift) && COMMON_SKILL_VALID(SID_StanceSwift))
	[SID_StanceSwift] = {{ 4, 4 }},
#endif

#if (defined(SID_StanceWarding) && COMMON_SKILL_VALID(SID_StanceWarding))
	[SID_StanceWarding] = {{ 6 }},
#endif

#if (defined(SID_StanceSpectrum) && COMMON_SKILL_VALID(SID_StanceSpectrum))
	[SID_StanceSpectrum] = {{ 2, 2, 2 }},
#endif

#if (defined(SID_RangeBonusBMag1) && COMMON_SKILL_VALID(SID_RangeBonusBMag1))
	[SID_RangeBonusBMag1] = {{ 1 }},
#endif

#if (defined(SID_RangeBonusBMag2) && COMMON_SKILL_VALID(SID_RangeBonusBMag2))
	[SID_RangeBonusBMag2] = {{ 2 }},
#endif

#if (defined(SID_RangeBonusBow1) && COMMON_SKILL_VALID(SID_RangeBonusBow1))
	[SID_RangeBonusBow1] = {{ 1 }},
#endif

#if (defined(SID_RangeBonusBow2) && COMMON_SKILL_VALID(SID_RangeBonusBow2))
	[SID_RangeBonusBow2] = {{ 2 }},
#endif

#if (defined(SID_StaffSavant) && COMMON_SKILL_VALID(SID_StaffSavant))
	[SID_StaffSavant] = {{ 1 }},
#endif

#if (defined(SID_HighGround) && COMMON_SKILL_VALID(SID_HighGround))
	[SID_HighGround] = {{ 2 }},
#endif

#if (defined(SID_InnerFlame1) && COMMON_SKILL_VALID(SID_InnerFlame1))
	[SID_InnerFlame1] = {{ 10 }},
#endif

#if (defined(SID_InnerFlame2) && COMMON_SKILL_VALID(SID_InnerFlame2))
	[SID_InnerFlame2] = {{ 7 }},
#endif

#if (defined(SID_InnerFlame3) && COMMON_SKILL_VALID(SID_InnerFlame3))
	[SID_InnerFlame3] = {{ 5 }},
#endif

#if (defined(SID_SavageBlow) && COMMON_SKILL_VALID(SID_SavageBlow))
	[SID_SavageBlow] = {{ 20 }},
#endif

#if (defined(SID_BreathOfLife) && COMMON_SKILL_VALID(SID_BreathOfLife))
	[SID_BreathOfLife] = {{ 20 }},
#endif

#if (defined(SID_Thunderstorm) && COMMON_SKILL_VALID(SID_Thunderstorm))
	[SID_Thunderstorm] = {{ 10 }},
#endif

#if (defined(SID_SoulSap) && COMMON_SKILL_VALID(SID_SoulSap))
	[SID_SoulSap] = {{ 10 }},
#endif

#if (defined(SID_Renewal) && COMMON_SKILL_VALID(SID_Renewal))
	[SID_Renewal] = {{ 30 }},
#endif

#if (defined(SID_Forager) && COMMON_SKILL_VALID(SID_Forager))
	[SID_Forager] = {{ 20 }},
#endif

#if (defined(SID_RainDish) && COMMON_SKILL_VALID(SID_RainDish))
	[SID_RainDish] = {{ 30 }},
#endif

#if (defined(SID_Relief) && COMMON_SKILL_VALID(SID_Relief))
	[SID_Relief] = {{ 20 }},
#endif

#if (defined(SID_Camaraderie) && COMMON_SKILL_VALID(SID_Camaraderie))
	[SID_Camaraderie] = {{ 10 }},
#endif

#if (defined(SID_Amaterasu) && COMMON_SKILL_VALID(SID_Amaterasu))
	[SID_Amaterasu] = {{ 20 }},
#endif

#if (defined(SID_Anathema) && COMMON_SKILL_VALID(SID_Anathema))
	[SID_Anathema] = {{ 10, 10 }},
#endif

#if (defined(SID_Bond) && COMMON_SKILL_VALID(SID_Bond))
	[SID_Bond] = {{ 2, 10 }},
#endif

#if (defined(SID_Charm) && COMMON_SKILL_VALID(SID_Charm))
	[SID_Charm] = {{ 3 }},
#endif

#if (defined(SID_Intimidate) && COMMON_SKILL_VALID(SID_Intimidate))
	[SID_Intimidate] = {{ 10 }},
#endif

#if (defined(SID_Tantivy) && COMMON_SKILL_VALID(SID_Tantivy))
	[SID_Tantivy] = {{ 10, 10 }},
#endif

#if (defined(SID_Focus) && COMMON_SKILL_VALID(SID_Focus))
	[SID_Focus] = {{ 10 }},
#endif

#if (defined(SID_Hex) && COMMON_SKILL_VALID(SID_Hex))
	[SID_Hex] = {{ 15 }},
#endif

#if (defined(SID_Infiltrator) && COMMON_SKILL_VALID(SID_Infiltrator))
	[SID_Infiltrator] = {{ 3, 15 }},
#endif

#if (defined(SID_Inspiration) && COMMON_SKILL_VALID(SID_Inspiration))
	[SID_Inspiration] = {{ 2, 2 }},
#endif

#if (defined(SID_DivinelyInspiring) && COMMON_SKILL_VALID(SID_DivinelyInspiring))
	[SID_DivinelyInspiring] = {{ 3, 1 }},
#endif

#if (defined(SID_VoiceOfPeace) && COMMON_SKILL_VALID(SID_VoiceOfPeace))
	[SID_VoiceOfPeace] = {{ 2 }},
#endif

#if (defined(SID_Peacebringer) && COMMON_SKILL_VALID(SID_Peacebringer))
	[SID_Peacebringer] = {{ 2 }},
#endif

#if (defined(SID_BloodTide) && COMMON_SKILL_VALID(SID_BloodTide))
	[SID_BloodTide] = {{ 5, 5 }},
#endif

#if (defined(SID_WhitePool) && COMMON_SKILL_VALID(SID_WhitePool))
	[SID_WhitePool] = {{ 5, 5 }},
#endif

#if (defined(SID_NightTide) && COMMON_SKILL_VALID(SID_NightTide))
	[SID_NightTide] = {{ 5 }},
#endif

#if (defined(SID_DriveStr) && COMMON_SKILL_VALID(SID_DriveStr))
	[SID_DriveStr] = {{ 4 }},
#endif

#if (defined(SID_DriveMag) && COMMON_SKILL_VALID(SID_DriveMag))
	[SID_DriveMag] = {{ 4 }},
#endif

#if (defined(SID_DriveSpd) && COMMON_SKILL_VALID(SID_DriveSpd))
	[SID_DriveSpd] = {{ 4 }},
#endif

#if (defined(SID_DriveDef) && COMMON_SKILL_VALID(SID_DriveDef))
	[SID_DriveDef] = {{ 4 }},
#endif

#if (defined(SID_DriveRes) && COMMON_SKILL_VALID(SID_DriveRes))
	[SID_DriveRes] = {{ 4 }},
#endif

#if (defined(SID_Charisma) && COMMON_SKILL_VALID(SID_Charisma))
	[SID_Charisma] = {{ 10, 10 }},
#endif

#if (defined(SID_Gentilhomme) && COMMON_SKILL_VALID(SID_Gentilhomme))
	[SID_Gentilhomme] = {{ 2 }},
#endif

#if (defined(SID_Demoiselle) && COMMON_SKILL_VALID(SID_Demoiselle))
	[SID_Demoiselle] = {{ 2 }},
#endif

#if (defined(SID_LilysPoise) && COMMON_SKILL_VALID(SID_LilysPoise))
	[SID_LilysPoise] = {{ 1, 3 }},
#endif

#if (defined(SID_Daunt) && COMMON_SKILL_VALID(SID_Daunt))
	[SID_Daunt] = {{ 5, 5 }},
#endif

#if (defined(SID_MaleficAura) && COMMON_SKILL_VALID(SID_MaleficAura))
	[SID_MaleficAura] = {{ 2}},
#endif

#if (defined(SID_SpurStr) && COMMON_SKILL_VALID(SID_SpurStr))
	[SID_SpurStr] = {{ 4 }},
#endif

#if (defined(SID_SpurMag) && COMMON_SKILL_VALID(SID_SpurMag))
	[SID_SpurMag] = {{ 4 }},
#endif

#if (defined(SID_SpurDef) && COMMON_SKILL_VALID(SID_SpurDef))
	[SID_SpurDef] = {{ 4 }},
#endif

#if (defined(SID_SpurRes) && COMMON_SKILL_VALID(SID_SpurRes))
	[SID_SpurRes] = {{ 4 }},
#endif

#if (defined(SID_SpurSpd) && COMMON_SKILL_VALID(SID_SpurSpd))
	[SID_SpurSpd] = {{ 4 }},
#endif

#if (defined(SID_Solidarity) && COMMON_SKILL_VALID(SID_Solidarity))
	[SID_Solidarity] = {{ 10, 10 }},
#endif

#if (defined(SID_Aptitude) && COMMON_SKILL_VALID(SID_Aptitude))
	[SID_Aptitude] = {{ 20 }},
#endif

#if (defined(SID_Assist) && COMMON_SKILL_VALID(SID_Assist))
	[SID_Assist] = {{ 5 }},
#endif

#if (defined(SID_Synergism) && COMMON_SKILL_VALID(SID_Synergism))
	[SID_Synergism] = {{ 3 }},
#endif

#if (defined(SID_Skyguard) && COMMON_SKILL_VALID(SID_Skyguard))
	[SID_Skyguard] = {{ 4 }},
#endif

#if (defined(SID_Horseguard) && COMMON_SKILL_VALID(SID_Horseguard))
	[SID_Horseguard] = {{ 4 }},
#endif

#if (defined(SID_Armorboost) && COMMON_SKILL_VALID(SID_Armorboost))
	[SID_Armorboost] = {{ 4, 4 }},
#endif

#if (defined(SID_Teleportation) && COMMON_SKILL_VALID(SID_Teleportation))
	[SID_Teleportation] = {{ 50 }},
#endif

#if (defined(SID_Provoke) && COMMON_SKILL_VALID(SID_Provoke))
	[SID_Provoke] = {{ 50 }},
#endif

#if (defined(SID_Crit) && COMMON_SKILL_VALID(SID_Crit))
	[SID_Crit] = {{ 15 }},
#endif

#if (defined(SID_InfinityEdge) && COMMON_SKILL_VALID(SID_InfinityEdge))
	[SID_InfinityEdge] = {{ 100 }},
#endif

#if (defined(SID_WatchfulEye) && COMMON_SKILL_VALID(SID_WatchfulEye))
	[SID_WatchfulEye] = {{ 20 }},
#endif

#if (defined(SID_AversaNight) && COMMON_SKILL_VALID(SID_AversaNight))
	[SID_AversaNight] = {{ 3 }},
#endif

#if (defined(SID_LightWeight) && COMMON_SKILL_VALID(SID_LightWeight))
	[SID_LightWeight] = {{ 3 }},
#endif

#if (defined(SID_Analytic) && COMMON_SKILL_VALID(SID_Analytic))
	[SID_Analytic] = {{ 4, 10, 5 }},
#endif

#if (defined(SID_Lifetaker) && COMMON_SKILL_VALID(SID_Lifetaker))
	[SID_Lifetaker] = {{ 25 }},
#endif

#if (defined(SID_MysticBoost) && COMMON_SKILL_VALID(SID_MysticBoost))
	[SID_MysticBoost] = {{ 6 }},
#endif

#if (defined(SID_GuardBearing) && COMMON_SKILL_VALID(SID_GuardBearing))
	[SID_GuardBearing] = {{ 50 }},
#endif

#if (defined(SID_YuneWhispers) && COMMON_SKILL_VALID(SID_YuneWhispers))
	[SID_YuneWhispers] = {{ 3 }},
#endif

#if (defined(SID_CounterRoar) && COMMON_SKILL_VALID(SID_CounterRoar))
	[SID_CounterRoar] = {{ 30, 70 }},
#endif

#if (defined(SID_Gambit) && COMMON_SKILL_VALID(SID_Gambit))
	[SID_Gambit] = {{ 50 }},
#endif

#if (defined(SID_MagicGambit) && COMMON_SKILL_VALID(SID_MagicGambit))
	[SID_MagicGambit] = {{ 50 }},
#endif

#if (defined(SID_BeastAssault) && COMMON_SKILL_VALID(SID_BeastAssault))
	[SID_BeastAssault] = {{ 40 }},
#endif

#if (defined(SID_LightAndDark) && COMMON_SKILL_VALID(SID_LightAndDark))
	[SID_LightAndDark] = {{ 5, 5, 5, 5 }},
#endif

#if (defined(SID_Bushido) && COMMON_SKILL_VALID(SID_Bushido))
	[SID_Bushido] = {{ 4, 7 }},
#endif

#if (defined(SID_DragonWall) && COMMON_SKILL_VALID(SID_DragonWall))
	[SID_DragonWall] = {{ 4 }},
#endif

#if (defined(SID_DragonWarth) && COMMON_SKILL_VALID(SID_DragonWarth))
	[SID_DragonWarth] = {{ 20, 20, }},
#endif

#if (defined(SID_BlueLionRule) && COMMON_SKILL_VALID(SID_BlueLionRule))
	[SID_BlueLionRule] = {{ 4 }},
#endif

#if (defined(SID_CrusaderWard) && COMMON_SKILL_VALID(SID_CrusaderWard))
	[SID_CrusaderWard] = {{ 80 }},
#endif

#if (defined(SID_Vanity) && COMMON_SKILL_VALID(SID_Vanity))
	[SID_Vanity] = {{ 2, 10 }},
#endif

#if (defined(SID_ShortShield) && COMMON_SKILL_VALID(SID_ShortShield))
	[SID_ShortShield] = {{ 6 }},
#endif

#if (defined(SID_Opportunist) && COMMON_SKILL_VALID(SID_Opportunist))
	[SID_Opportunist] = {{ 4 }},
#endif

#if (defined(SID_TowerShield) && COMMON_SKILL_VALID(SID_TowerShield))
	[SID_TowerShield] = {{ 6 }},
#endif

#if (defined(SID_StunningSmile) && COMMON_SKILL_VALID(SID_StunningSmile))
	[SID_StunningSmile] = {{ 6 }},
#endif

#if (defined(SID_Trample) && COMMON_SKILL_VALID(SID_Trample))
	[SID_Trample] = {{ 5 }},
#endif

#if (defined(SID_Loyalty) && COMMON_SKILL_VALID(SID_Loyalty))
	[SID_Loyalty] = {{ 15, 3 }},
#endif

#if (defined(SID_HolyAura) && COMMON_SKILL_VALID(SID_HolyAura))
	[SID_HolyAura] = {{ 1, 5, 5, 5 }},
#endif

#if (defined(SID_BlueFlame) && COMMON_SKILL_VALID(SID_BlueFlame))
	[SID_BlueFlame] = {{ 2, 2 }},
#endif

#if (defined(SID_DancingBlade) && COMMON_SKILL_VALID(SID_DancingBlade))
	[SID_DancingBlade] = {{ 4, 2 }},
#endif

#if (defined(SID_QuickBurn) && COMMON_SKILL_VALID(SID_QuickBurn))
	[SID_QuickBurn] = {{ 15 }},
#endif

#if (defined(SID_SlowBurn) && COMMON_SKILL_VALID(SID_SlowBurn))
	[SID_SlowBurn] = {{ 15 }},
#endif

#if (defined(SID_NaturalCover) && COMMON_SKILL_VALID(SID_NaturalCover))
	[SID_NaturalCover] = {{ 3 }},
#endif

#if (defined(SID_FlierGuidance) && COMMON_SKILL_VALID(SID_FlierGuidance))
	[SID_FlierGuidance] = {{ 3 }},
#endif

#if (defined(SID_Guidance) && COMMON_SKILL_VALID(SID_Guidance))
	[SID_Guidance] = {{ 2 }},
#endif

#if (defined(SID_SoaringGuidance) && COMMON_SKILL_VALID(SID_SoaringGuidance))
	[SID_SoaringGuidance] = {{ 1 }},
#endif

#if (defined(SID_FlierFormation) && COMMON_SKILL_VALID(SID_FlierFormation))
	[SID_FlierFormation] = {{ 3 }},
#endif

#if (defined(SID_Aerobatics) && COMMON_SKILL_VALID(SID_Aerobatics))
	[SID_Aerobatics] = {{ 2 }},
#endif

#if (defined(SID_SoaringWings) && COMMON_SKILL_VALID(SID_SoaringWings))
	[SID_SoaringWings] = {{ 1 }},
#endif

#if (defined(SID_MageSlayer) && COMMON_SKILL_VALID(SID_MageSlayer))
	[SID_MageSlayer] = {{ 2, 10 }},
#endif

#if (defined(SID_SolarPower) && COMMON_SKILL_VALID(SID_SolarPower))
	[SID_SolarPower] = {{ 25 }},
#endif

#if (defined(SID_Obstruct) && COMMON_SKILL_VALID(SID_Obstruct))
	[SID_Obstruct] = {{ 25, 1 }},
#endif

#if (defined(SID_DetailedReport) && COMMON_SKILL_VALID(SID_DetailedReport))
	[SID_DetailedReport] = {{ 50, 2, 2 }},
#endif

#if (defined(SID_AirRaidAttack) && COMMON_SKILL_VALID(SID_AirRaidAttack))
	[SID_AirRaidAttack] = {{ 5 }},
#endif

#if (defined(SID_AirRaidAvoid) && COMMON_SKILL_VALID(SID_AirRaidAvoid))
	[SID_AirRaidAvoid] = {{ 15 }},
#endif

#if (defined(SID_AirRaidCrit) && COMMON_SKILL_VALID(SID_AirRaidCrit))
	[SID_AirRaidCrit] = {{ 15 }},
#endif

#if (defined(SID_AirRaidDefense) && COMMON_SKILL_VALID(SID_AirRaidDefense))
	[SID_AirRaidDefense] = {{ 5 }},
#endif

#if (defined(SID_AirRaidHit) && COMMON_SKILL_VALID(SID_AirRaidHit))
	[SID_AirRaidHit] = {{ 15 }},
#endif

#if (defined(SID_AirRaidResistance) && COMMON_SKILL_VALID(SID_AirRaidResistance))
	[SID_AirRaidResistance] = {{ 5 }},
#endif

#if (defined(SID_AirRaidSpeed) && COMMON_SKILL_VALID(SID_AirRaidSpeed))
	[SID_AirRaidSpeed] = {{ 5 }},
#endif

#if (defined(SID_HealingFocus) && COMMON_SKILL_VALID(SID_HealingFocus))
	[SID_HealingFocus] = {{ 50 }},
#endif

#if (defined(SID_LightRune) && COMMON_SKILL_VALID(SID_LightRune))
	[SID_LightRune] = {{ 20 }},
#endif

#if (defined(SID_Mine) && COMMON_SKILL_VALID(SID_Mine))
	[SID_Mine] = {{ 20 }},
#endif

#if (defined(SID_RallyDefense) && COMMON_SKILL_VALID(SID_RallyDefense))
	[SID_RallyDefense] = {{ 4 }},
#endif

#if (defined(SID_RallyLuck) && COMMON_SKILL_VALID(SID_RallyLuck))
	[SID_RallyLuck] = {{ 4 }},
#endif

#if (defined(SID_RallyMagic) && COMMON_SKILL_VALID(SID_RallyMagic))
	[SID_RallyMagic] = {{ 4 }},
#endif

#if (defined(SID_RallyMovement) && COMMON_SKILL_VALID(SID_RallyMovement))
	[SID_RallyMovement] = {{ 4 }},
#endif

#if (defined(SID_RallyResistance) && COMMON_SKILL_VALID(SID_RallyResistance))
	[SID_RallyResistance] = {{ 4 }},
#endif

#if (defined(SID_RallySkill) && COMMON_SKILL_VALID(SID_RallySkill))
	[SID_RallySkill] = {{ 4 }},
#endif

#if (defined(SID_RallySpeed) && COMMON_SKILL_VALID(SID_RallySpeed))
	[SID_RallySpeed] = {{ 4 }},
#endif

#if (defined(SID_RallyStrength) && COMMON_SKILL_VALID(SID_RallyStrength))
	[SID_RallyStrength] = {{ 4 }},
#endif

#if (defined(SID_CloseCombat) && COMMON_SKILL_VALID(SID_CloseCombat))
	[SID_CloseCombat] = {{ 2 }},
#endif

#if (defined(SID_Barricade) && COMMON_SKILL_VALID(SID_Barricade))
	[SID_Barricade] = {{ 50 }},
#endif

#if (defined(SID_BarricadePlus) && COMMON_SKILL_VALID(SID_BarricadePlus))
	[SID_BarricadePlus] = {{ 50 }},
#endif

#if (defined(SID_Pursuer) && COMMON_SKILL_VALID(SID_Pursuer))
	[SID_Pursuer] = {{ 5, 4 }},
#endif

#if (defined(SID_GoldDigger) && COMMON_SKILL_VALID(SID_GoldDigger))
	[SID_GoldDigger] = {{ 100 }},
#endif

#if (defined(SID_EffectSpore) && COMMON_SKILL_VALID(SID_EffectSpore))
	[SID_EffectSpore] = {{ 50 }},
#endif

#if (defined(SID_DominantArm) && COMMON_SKILL_VALID(SID_DominantArm))
	[SID_DominantArm] = {{ 50 }},
#endif

#if (defined(SID_FranticSwing) && COMMON_SKILL_VALID(SID_FranticSwing))
	[SID_FranticSwing] = {{ 50 }},
#endif

#if (defined(SID_CriticalOverload) && COMMON_SKILL_VALID(SID_CriticalOverload))
	[SID_CriticalOverload] = {{ 3 }},
#endif

#if (defined(SID_SealDefense) && COMMON_SKILL_VALID(SID_SealDefense))
	[SID_SealDefense] = {{ 6 }},
#endif

#if (defined(SID_SealLuck) && COMMON_SKILL_VALID(SID_SealLuck))
	[SID_SealLuck] = {{ 6 }},
#endif

#if (defined(SID_SealMagic) && COMMON_SKILL_VALID(SID_SealMagic))
	[SID_SealMagic] = {{ 6 }},
#endif

#if (defined(SID_SealResistance) && COMMON_SKILL_VALID(SID_SealResistance))
	[SID_SealResistance] = {{ 6 }},
#endif

#if (defined(SID_SealSkill) && COMMON_SKILL_VALID(SID_SealSkill))
	[SID_SealSkill] = {{ 6 }},
#endif

#if (defined(SID_SealSpeed) && COMMON_SKILL_VALID(SID_SealSpeed))
	[SID_SealSpeed] = {{ 6 }},
#endif

#if (defined(SID_SealStrength) && COMMON_SKILL_VALID(SID_SealStrength))
	[SID_SealStrength] = {{ 6 }},
#endif

#if (defined(SID_WyvernFlight) && COMMON_SKILL_VALID(SID_WyvernFlight))
	[SID_WyvernFlight] = {{ 10, 7 }},
#endif

#if (defined(SID_Scendscale) && COMMON_SKILL_VALID(SID_Scendscale))
	[SID_Scendscale] = {{ 25, 7 }},
#endif

#if (defined(SID_LawsOfSacae) && COMMON_SKILL_VALID(SID_LawsOfSacae))
	[SID_LawsOfSacae] = {{ 4 }},
#endif

#if (defined(SID_ShieldPulse) && COMMON_SKILL_VALID(SID_ShieldPulse))
	[SID_ShieldPulse] = {{ 50 }},
#endif

#if (defined(SID_Leader) && COMMON_SKILL_VALID(SID_Leader))
	[SID_Leader] = {{ 2 }},
#endif

#if (defined(SID_Momentum) && COMMON_SKILL_VALID(SID_Momentum))
	[SID_Momentum] = {{ 10 }},
#endif

#if (defined(SID_FatalTen) && COMMON_SKILL_VALID(SID_FatalTen))
	[SID_FatalTen] = {{ 10 }},
#endif

#if (defined(SID_LionFlame) && COMMON_SKILL_VALID(SID_LionFlame))
	[SID_LionFlame] = {{ 15, 4, 100, 3 }},
#endif

#if (defined(SID_LimitBreak) && COMMON_SKILL_VALID(SID_LimitBreak))
	[SID_LimitBreak] = {{ 5, 100 }},
#endif

#if (defined(SID_ImmovableObject) && COMMON_SKILL_VALID(SID_ImmovableObject))
	[SID_ImmovableObject] = {{ 50 }},
#endif

#if (defined(SID_UnstoppableForce) && COMMON_SKILL_VALID(SID_UnstoppableForce))
	[SID_UnstoppableForce] = {{ 100 }},
#endif

#if (defined(SID_WeaponHeal) && COMMON_SKILL_VALID(SID_WeaponHeal))
	[SID_WeaponHeal] = {{ 50 }},
#endif

#if (defined(SID_BloodSurge) && COMMON_SKILL_VALID(SID_BloodSurge))
	[SID_BloodSurge] = {{ 5, 20 }},
#endif

#if (defined(SID_BloodReaver) && COMMON_SKILL_VALID(SID_BloodReaver))
	[SID_BloodReaver] = {{ 5, 20 }},
#endif

#if (defined(SID_Witch) && COMMON_SKILL_VALID(SID_Witch))
	[SID_Witch] = {{ 5 }},
#endif

#if (defined(SID_COMBAT_BloodTribute) && COMMON_SKILL_VALID(SID_COMBAT_BloodTribute))
	[SID_COMBAT_BloodTribute] = {{ 30, 100 }},
#endif

#if (defined(SID_COMBAT_CrimsonStrike) && COMMON_SKILL_VALID(SID_COMBAT_CrimsonStrike))
	[SID_COMBAT_CrimsonStrike] = {{ 50, 100 }},
#endif

#if (defined(SID_COMBAT_VitalReckoning) && COMMON_SKILL_VALID(SID_COMBAT_VitalReckoning))
	[SID_COMBAT_VitalReckoning] = {{ 80, 100 }},
#endif

#if (defined(SID_Stubborn) && COMMON_SKILL_VALID(SID_Stubborn))
	[SID_Stubborn] = {{ 5, 5 }},
#endif

#if (defined(SID_EndlessVitality) && COMMON_SKILL_VALID(SID_EndlessVitality))
	[SID_EndlessVitality] = {{ 20 }},
#endif
};
