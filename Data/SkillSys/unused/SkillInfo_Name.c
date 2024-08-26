#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

/* This is optional */
const u16 gSkillNames[MAX_SKILL_NUM + 1] = {
#if (defined(SID_Skybreaker) && COMMON_SKILL_VALID(SID_Skybreaker))
    [SID_Skybreaker] = MSG_SKILL_Skybreaker_NAME,
#endif

#if (defined(SID_Resourceful) && COMMON_SKILL_VALID(SID_Resourceful))
    [SID_Resourceful] = MSG_SKILL_Resourceful_NAME,
#endif

#if (defined(SID_RuinedBlade) && COMMON_SKILL_VALID(SID_RuinedBlade))
    [SID_RuinedBlade] = MSG_SKILL_RuinedBlade_NAME,
#endif

#if (defined(SID_QuickRiposte) && COMMON_SKILL_VALID(SID_QuickRiposte))
    [SID_QuickRiposte] = MSA_SKILL_QuickRiposte_NAME,
#endif

#if (defined(SID_DoubleLion) && COMMON_SKILL_VALID(SID_DoubleLion))
    [SID_DoubleLion] = MSG_SKILL_DoubleLion_NAME,
#endif

#if (defined(SID_RunningStart) && COMMON_SKILL_VALID(SID_RunningStart))
    [SID_RunningStart] = MSG_SKILL_RunningStart_NAME,
#endif

#if (defined(SID_Thunderstorm) && COMMON_SKILL_VALID(SID_Thunderstorm))
    [SID_Thunderstorm] = MSG_SKILL_Thunderstorm_NAME,
#endif

#if (defined(SID_WonderGuard) && COMMON_SKILL_VALID(SID_WonderGuard))
    [SID_WonderGuard] = MSG_SKILL_WonderGuard_NAME,
#endif

#if (defined(SID_PoisonPoint) && COMMON_SKILL_VALID(SID_PoisonPoint))
    [SID_PoisonPoint] = MSG_SKILL_PoisonPoint_NAME,
#endif

#if (defined(SID_KillingMachine) && COMMON_SKILL_VALID(SID_KillingMachine))
    [SID_KillingMachine] = MSG_SKILL_KillingMachine_NAME,
#endif

#if (defined(SID_HeavyStrikes) && COMMON_SKILL_VALID(SID_HeavyStrikes))
    [SID_HeavyStrikes] = MSG_SKILL_HeavyStrikes_NAME,
#endif

#if (defined(SID_StanceSpectrum) && COMMON_SKILL_VALID(SID_StanceSpectrum))
    [SID_StanceSpectrum] = MSG_SKILL_StanceSpectrum_NAME,
#endif

#if (defined(SID_BattleVeteran) && COMMON_SKILL_VALID(SID_BattleVeteran))
    [SID_BattleVeteran] = MSG_SKILL_BattleVeteran_NAME,
#endif

#if (defined(SID_CriticalForce) && COMMON_SKILL_VALID(SID_CriticalForce))
    [SID_CriticalForce] = MSG_SKILL_CriticalForce_NAME,
#endif

#if (defined(SID_StrongRiposte) && COMMON_SKILL_VALID(SID_StrongRiposte))
    [SID_StrongRiposte] = MSG_SKILL_StrongRiposte_NAME,
#endif

#if (defined(SID_DragonSkin) && COMMON_SKILL_VALID(SID_DragonSkin))
    [SID_DragonSkin] = MSG_SKILL_DragonSkin_NAME,
#endif

#if (defined(SID_KeenFighter) && COMMON_SKILL_VALID(SID_KeenFighter))
    [SID_KeenFighter] = MSG_SKILL_KeenFighter_NAME,
#endif

#if (defined(SID_FieryBlood) && COMMON_SKILL_VALID(SID_FieryBlood))
    [SID_FieryBlood] = MSG_SKILL_FieryBlood_NAME,
#endif

#if (defined(SID_NaturalCover) && COMMON_SKILL_VALID(SID_NaturalCover))
    [SID_NaturalCover] = MSG_SKILL_NaturalCover_NAME,
#endif

#if (defined(SID_PushDefense) && COMMON_SKILL_VALID(SID_PushDefense))
    [SID_PushDefense] = MSG_SKILL_PushDefense_NAME,
#endif

#if (defined(SID_PushMagic) && COMMON_SKILL_VALID(SID_PushMagic))
    [SID_PushMagic] = MSG_SKILL_PushMagic_NAME,
#endif

#if (defined(SID_PushResistance) && COMMON_SKILL_VALID(SID_PushResistance))
    [SID_PushResistance] = MSG_SKILL_PushResistance_NAME,
#endif

#if (defined(SID_PushSkill) && COMMON_SKILL_VALID(SID_PushSkill))
    [SID_PushSkill] = MSG_SKILL_PushSkill_NAME,
#endif

#if (defined(SID_PushSpeed) && COMMON_SKILL_VALID(SID_PushSpeed))
    [SID_PushSpeed] = MSG_SKILL_PushSpeed_NAME,
#endif

#if (defined(SID_PushStrength) && COMMON_SKILL_VALID(SID_PushStrength))
    [SID_PushStrength] = MSG_SKILL_PushStrength_NAME,
#endif

#if (defined(SID_PushLuck) && COMMON_SKILL_VALID(SID_PushLuck))
    [SID_PushLuck] = MSG_SKILL_PushLuck_NAME,
#endif

#if (defined(SID_PushMovement) && COMMON_SKILL_VALID(SID_PushMovement))
    [SID_PushMovement] = MSG_SKILL_PushMovement_NAME,
#endif

#if (defined(SID_PushSpectrum) && COMMON_SKILL_VALID(SID_PushSpectrum))
    [SID_PushSpectrum] = MSG_SKILL_PushSpectrum_NAME,
#endif

#if (defined(SID_OutdoorFighter) && COMMON_SKILL_VALID(SID_OutdoorFighter))
    [SID_OutdoorFighter] = MSG_SKILL_OutdoorFighter_NAME,
#endif

#if (defined(SID_DancingBlade) && COMMON_SKILL_VALID(SID_DancingBlade))
    [SID_DancingBlade] = MSG_SKILL_DancingBlade_NAME,
#endif

#if (defined(SID_KnightAspirant) && COMMON_SKILL_VALID(SID_KnightAspirant))
    [SID_KnightAspirant] = MSG_SKILL_KnightAspirant_NAME,
#endif

#if (defined(SID_StunningSmile) && COMMON_SKILL_VALID(SID_StunningSmile))
    [SID_StunningSmile] = MSG_SKILL_StunningSmile_NAME,
#endif

#if (defined(SID_DefenseBoost) && COMMON_SKILL_VALID(SID_DefenseBoost))
    [SID_DefenseBoost] = MSG_SKILL_DefenseBoost_NAME,
#endif

#if (defined(SID_LuckBoost) && COMMON_SKILL_VALID(SID_LuckBoost))
    [SID_LuckBoost] = MSG_SKILL_LuckBoost_NAME,
#endif

#if (defined(SID_MagicBoost) && COMMON_SKILL_VALID(SID_MagicBoost))
    [SID_MagicBoost] = MSG_SKILL_MagicBoost_NAME,
#endif

#if (defined(SID_MoveBoost) && COMMON_SKILL_VALID(SID_MoveBoost))
    [SID_MoveBoost] = MSG_SKILL_MoveBoost_NAME,
#endif

#if (defined(SID_ResistanceBoost) && COMMON_SKILL_VALID(SID_ResistanceBoost))
    [SID_ResistanceBoost] = MSG_SKILL_ResistanceBoost_NAME,
#endif

#if (defined(SID_SkillBoost) && COMMON_SKILL_VALID(SID_SkillBoost))
    [SID_SkillBoost] = MSG_SKILL_SkillBoost_NAME,
#endif

#if (defined(SID_SpeedBoost) && COMMON_SKILL_VALID(SID_SpeedBoost))
    [SID_SpeedBoost] = MSG_SKILL_SpeedBoost_NAME,
#endif

#if (defined(SID_StrengthBoost) && COMMON_SKILL_VALID(SID_StrengthBoost))
    [SID_StrengthBoost] = MSG_SKILL_StrengthBoost_NAME,
#endif

#if (defined(SID_SpectrumBoost) && COMMON_SKILL_VALID(SID_SpectrumBoost))
    [SID_SpectrumBoost] = MSG_SKILL_SpectrumBoost_NAME,
#endif

#if (defined(SID_RuinedBladePlus) && COMMON_SKILL_VALID(SID_RuinedBladePlus))
    [SID_RuinedBladePlus] = MSG_SKILL_RuinedBladePlus_NAME,
#endif

#if (defined(SID_Teleportation) && COMMON_SKILL_VALID(SID_Teleportation))
    [SID_Teleportation] = MSG_SKILL_Teleportation_NAME,
#endif

#if (defined(SID_AversaNight) && COMMON_SKILL_VALID(SID_AversaNight))
    [SID_AversaNight] = MSG_SKILL_DEBUFF_Aversa_NAME,
#endif

#if (defined(SID_GentleFlower) && COMMON_SKILL_VALID(SID_GentleFlower))
    [SID_GentleFlower] = MSG_SKILL_GentleFlower_NAME,
#endif

#if (defined(SID_FairyTaleFolk) && COMMON_SKILL_VALID(SID_FairyTaleFolk))
    [SID_FairyTaleFolk] = MSG_SKILL_FairyTaleFolk_NAME,
#endif

#if (defined(SID_InitCalm) && COMMON_SKILL_VALID(SID_InitCalm))
    [SID_InitCalm] = MSG_SKILL_InitCalm_NAME,
#endif

#if (defined(SID_InitClever) && COMMON_SKILL_VALID(SID_InitClever))
    [SID_InitClever] = MSG_SKILL_InitClever_NAME,
#endif

#if (defined(SID_InitDeft) && COMMON_SKILL_VALID(SID_InitDeft))
    [SID_InitDeft] = MSG_SKILL_InitDeft_NAME,
#endif

#if (defined(SID_InitLucky) && COMMON_SKILL_VALID(SID_InitLucky))
    [SID_InitLucky] = MSG_SKILL_InitLucky_NAME,
#endif

#if (defined(SID_InitNimble) && COMMON_SKILL_VALID(SID_InitNimble))
    [SID_InitNimble] = MSG_SKILL_InitNimble_NAME,
#endif

#if (defined(SID_InitQuick) && COMMON_SKILL_VALID(SID_InitQuick))
    [SID_InitQuick] = MSG_SKILL_InitQuick_NAME,
#endif

#if (defined(SID_InitSpectrum) && COMMON_SKILL_VALID(SID_InitSpectrum))
    [SID_InitSpectrum] = MSG_SKILL_InitSpectrum_NAME,
#endif

#if (defined(SID_InitStrong) && COMMON_SKILL_VALID(SID_InitStrong))
    [SID_InitStrong] = MSG_SKILL_InitStrong_NAME,
#endif

#if (defined(SID_InitSturdy) && COMMON_SKILL_VALID(SID_InitSturdy))
    [SID_InitSturdy] = MSG_SKILL_InitSturdy_NAME,
#endif

#if (defined(SID_OathDefense) && COMMON_SKILL_VALID(SID_OathDefense))
    [SID_OathDefense] = MSG_SKILL_OathDefense_NAME,
#endif

#if (defined(SID_OathLuck) && COMMON_SKILL_VALID(SID_OathLuck))
    [SID_OathLuck] = MSG_SKILL_OathLuck_NAME,
#endif

#if (defined(SID_OathMagic) && COMMON_SKILL_VALID(SID_OathMagic))
    [SID_OathMagic] = MSG_SKILL_OathMagic_NAME,
#endif

#if (defined(SID_BeastAssault) && COMMON_SKILL_VALID(SID_BeastAssault))
    [SID_BeastAssault] = MSG_SKILL_BeastAssault_NAME,
#endif

#if (defined(SID_OathMovement) && COMMON_SKILL_VALID(SID_OathMovement))
    [SID_OathMovement] = MSG_SKILL_OathMovement_NAME,
#endif

#if (defined(SID_OathResistance) && COMMON_SKILL_VALID(SID_OathResistance))
    [SID_OathResistance] = MSG_SKILL_OathResistance_NAME,
#endif

#if (defined(SID_OathSkill) && COMMON_SKILL_VALID(SID_OathSkill))
    [SID_OathSkill] = MSG_SKILL_OathSkill_NAME,
#endif

#if (defined(SID_OathSpeed) && COMMON_SKILL_VALID(SID_OathSpeed))
    [SID_OathSpeed] = MSG_SKILL_OathSpeed_NAME,
#endif

#if (defined(SID_OathStrength) && COMMON_SKILL_VALID(SID_OathStrength))
    [SID_OathStrength] = MSG_SKILL_OathStrength_NAME,
#endif

#if (defined(SID_DivinePulse) && COMMON_SKILL_VALID(SID_DivinePulse))
    [SID_DivinePulse] = MSG_SKILL_DivinePulse_NAME,
#endif

#if (defined(SID_GuardBearing) && COMMON_SKILL_VALID(SID_GuardBearing))
    [SID_GuardBearing] = MSG_SKILL_GuardBearing_NAME,
#endif

#if (defined(SID_DragonWarth) && COMMON_SKILL_VALID(SID_DragonWarth))
    [SID_DragonWarth] = MSG_SKILL_DragonWarth_NAME,
#endif

#if (defined(SID_BlueLionRule) && COMMON_SKILL_VALID(SID_BlueLionRule))
    [SID_BlueLionRule] = MSG_SKILL_BlueLionRule_NAME,
#endif

#if (defined(SID_FaerghusAncestry) && COMMON_SKILL_VALID(SID_FaerghusAncestry))
    [SID_FaerghusAncestry] = MSG_SKILL_FaerghusAncestry_NAME,
#endif

#if (defined(SID_CrusaderWard) && COMMON_SKILL_VALID(SID_CrusaderWard))
    [SID_CrusaderWard] = MSG_SKILL_CrusaderWard_NAME,
#endif

#if (defined(SID_YuneWhispers) && COMMON_SKILL_VALID(SID_YuneWhispers))
    [SID_YuneWhispers] = MSG_SKILL_YuneWhispers_NAME,
#endif

#if (defined(SID_FlierGuidance) && COMMON_SKILL_VALID(SID_FlierGuidance))
    [SID_FlierGuidance] = MSG_SKILL_FlierGuidance_NAME,
#endif

#if (defined(SID_SoaringGuidance) && COMMON_SKILL_VALID(SID_SoaringGuidance))
    [SID_SoaringGuidance] = MSG_SKILL_SoaringGuidance_NAME,
#endif

#if (defined(SID_FlierFormation) && COMMON_SKILL_VALID(SID_FlierFormation))
    [SID_FlierFormation] = MSG_SKILL_FlierFormation_NAME,
#endif

#if (defined(SID_SoaringWings) && COMMON_SKILL_VALID(SID_SoaringWings))
    [SID_SoaringWings] = MSG_SKILL_SoaringWings_NAME,
#endif

#if (defined(SID_DetailedReport) && COMMON_SKILL_VALID(SID_DetailedReport))
    [SID_DetailedReport] = MSG_SKILL_DetailedReport_NAME,
#endif

#if (defined(SID_VengefulFighter) && COMMON_SKILL_VALID(SID_VengefulFighter))
    [SID_VengefulFighter] = MSG_SKILL_VengefulFighter_NAME,
#endif

#if (defined(SID_RecklessFighter) && COMMON_SKILL_VALID(SID_RecklessFighter))
    [SID_RecklessFighter] = MSG_SKILL_RecklessFighter_NAME,
#endif

#if (defined(SID_BoldFighter) && COMMON_SKILL_VALID(SID_BoldFighter))
    [SID_BoldFighter] = MSG_SKILL_BoldFighter_NAME,
#endif

#if (defined(SID_AdvantageChaser) && COMMON_SKILL_VALID(SID_AdvantageChaser))
    [SID_AdvantageChaser] = MSG_SKILL_AdvantageChaser_NAME,
#endif

#if (defined(SID_Nonconforming) && COMMON_SKILL_VALID(SID_Nonconforming))
    [SID_Nonconforming] = MSG_SKILL_Nonconforming_NAME,
#endif

#if (defined(SID_RouseDefense) && COMMON_SKILL_VALID(SID_RouseDefense))
    [SID_RouseDefense] = MSG_SKILL_RouseDefense_NAME,
#endif

#if (defined(SID_RouseLuck) && COMMON_SKILL_VALID(SID_RouseLuck))
    [SID_RouseLuck] = MSG_SKILL_RouseLuck_NAME,
#endif

#if (defined(SID_RouseMagic) && COMMON_SKILL_VALID(SID_RouseMagic))
    [SID_RouseMagic] = MSG_SKILL_RouseMagic_NAME,
#endif

#if (defined(SID_RouseMovement) && COMMON_SKILL_VALID(SID_RouseMovement))
    [SID_RouseMovement] = MSG_SKILL_RouseMovement_NAME,
#endif

#if (defined(SID_RouseResistance) && COMMON_SKILL_VALID(SID_RouseResistance))
    [SID_RouseResistance] = MSG_SKILL_RouseResistance_NAME,
#endif

#if (defined(SID_RouseSkill) && COMMON_SKILL_VALID(SID_RouseSkill))
    [SID_RouseSkill] = MSG_SKILL_RouseSkill_NAME,
#endif

#if (defined(SID_RouseSpeed) && COMMON_SKILL_VALID(SID_RouseSpeed))
    [SID_RouseSpeed] = MSG_SKILL_RouseSpeed_NAME,
#endif

#if (defined(SID_RouseStrength) && COMMON_SKILL_VALID(SID_RouseStrength))
    [SID_RouseStrength] = MSG_SKILL_RouseStrength_NAME,
#endif

#if (defined(SID_PridefulWarrior) && COMMON_SKILL_VALID(SID_PridefulWarrior))
    [SID_PridefulWarrior] = MSG_SKILL_PridefulWarrior_NAME,
#endif

#if (defined(SID_TriangleAdept) && COMMON_SKILL_VALID(SID_TriangleAdept))
    [SID_TriangleAdept] = MSG_SKILL_TriangleAdept_NAME,
#endif

#if (defined(SID_TriangleAdeptPlus) && COMMON_SKILL_VALID(SID_TriangleAdeptPlus))
    [SID_TriangleAdeptPlus] = MSG_SKILL_TriangleAdeptPlus_NAME,
#endif

#if (defined(SID_PoisonStrike) && COMMON_SKILL_VALID(SID_PoisonStrike))
    [SID_PoisonStrike] = MSG_SKILL_PoisonStrike_NAME,
#endif

#if (defined(SID_GrislyWound) && COMMON_SKILL_VALID(SID_GrislyWound))
    [SID_GrislyWound] = MSG_SKILL_GrislyWound_NAME,
#endif

#if (defined(SID_AirRaidAttack) && COMMON_SKILL_VALID(SID_AirRaidAttack))
    [SID_AirRaidAttack] = MSG_SKILL_AirRaidAttack_NAME,
#endif

#if (defined(SID_AirRaidAvoid) && COMMON_SKILL_VALID(SID_AirRaidAvoid))
    [SID_AirRaidAvoid] = MSG_SKILL_AirRaidAvoid_NAME,
#endif

#if (defined(SID_AirRaidCrit) && COMMON_SKILL_VALID(SID_AirRaidCrit))
    [SID_AirRaidCrit] = MSG_SKILL_AirRaidCrit_NAME,
#endif

#if (defined(SID_AirRaidDefense) && COMMON_SKILL_VALID(SID_AirRaidDefense))
    [SID_AirRaidDefense] = MSG_SKILL_AirRaidDefense_NAME,
#endif

#if (defined(SID_AirRaidHit) && COMMON_SKILL_VALID(SID_AirRaidHit))
    [SID_AirRaidHit] = MSG_SKILL_AirRaidHit_NAME,
#endif

#if (defined(SID_AirRaidResistance) && COMMON_SKILL_VALID(SID_AirRaidResistance))
    [SID_AirRaidResistance] = MSG_SKILL_AirRaidResistance_NAME,
#endif

#if (defined(SID_AirRaidSpeed) && COMMON_SKILL_VALID(SID_AirRaidSpeed))
    [SID_AirRaidSpeed] = MSG_SKILL_AirRaidSpeed_NAME,
#endif

#if (defined(SID_RallyDefense) && COMMON_SKILL_VALID(SID_RallyDefense))
    [SID_RallyDefense] = MSG_SKILL_RallyDefense_NAME,
#endif

#if (defined(SID_RallyLuck) && COMMON_SKILL_VALID(SID_RallyLuck))
    [SID_RallyLuck] = MSG_SKILL_RallyLuck_NAME,
#endif

#if (defined(SID_RallyMagic) && COMMON_SKILL_VALID(SID_RallyMagic))
    [SID_RallyMagic] = MSG_SKILL_RallyMagic_NAME,
#endif

#if (defined(SID_RallyMovement) && COMMON_SKILL_VALID(SID_RallyMovement))
    [SID_RallyMovement] = MSG_SKILL_RallyMovement_NAME,
#endif

#if (defined(SID_RallyResistance) && COMMON_SKILL_VALID(SID_RallyResistance))
    [SID_RallyResistance] = MSG_SKILL_RallyResistance_NAME,
#endif

#if (defined(SID_RallySkill) && COMMON_SKILL_VALID(SID_RallySkill))
    [SID_RallySkill] = MSG_SKILL_RallySkill_NAME,
#endif

#if (defined(SID_RallySpeed) && COMMON_SKILL_VALID(SID_RallySpeed))
    [SID_RallySpeed] = MSG_SKILL_RallySpeed_NAME,
#endif

#if (defined(SID_RallyStrength) && COMMON_SKILL_VALID(SID_RallyStrength))
    [SID_RallyStrength] = MSG_SKILL_RallyStrength_NAME,
#endif

#if (defined(SID_RallySpectrum) && COMMON_SKILL_VALID(SID_RallySpectrum))
    [SID_RallySpectrum] = MSG_SKILL_RallySpectrum_NAME,
#endif

#if (defined(SID_IndoorMarch) && COMMON_SKILL_VALID(SID_IndoorMarch))
    [SID_IndoorMarch] = MSG_SKILL_IndoorMarch_NAME,
#endif

#if (defined(SID_DevilsPact) && COMMON_SKILL_VALID(SID_DevilsPact))
    [SID_DevilsPact] = MSG_SKILL_DevilsPact_NAME,
#endif

#if (defined(SID_DevilsWhim) && COMMON_SKILL_VALID(SID_DevilsWhim))
    [SID_DevilsWhim] = MSG_SKILL_DevilsWhim_NAME,
#endif

#if (defined(SID_DevilsLuck) && COMMON_SKILL_VALID(SID_DevilsLuck))
    [SID_DevilsLuck] = MSG_SKILL_DevilsLuck_NAME,
#endif

#if (defined(SID_PassionsFlow) && COMMON_SKILL_VALID(SID_PassionsFlow))
    [SID_PassionsFlow] = MSG_SKILL_PassionsFlow_NAME,
#endif

#if (defined(SID_NatureRush) && COMMON_SKILL_VALID(SID_NatureRush))
    [SID_NatureRush] = MSG_SKILL_NatureRush_NAME,
#endif

#if (defined(SID_CounterMagic) && COMMON_SKILL_VALID(SID_CounterMagic))
    [SID_CounterMagic] = MSG_SKILL_CounterMagic_NAME,
#endif

#if (defined(SID_LadyBlade) && COMMON_SKILL_VALID(SID_LadyBlade))
    [SID_LadyBlade] = MSG_SKILL_LadyBlade_NAME,
#endif

#if (defined(SID_LullDefense) && COMMON_SKILL_VALID(SID_LullDefense))
    [SID_LullDefense] = MSG_SKILL_LullDefense_NAME,
#endif

#if (defined(SID_LullLuck) && COMMON_SKILL_VALID(SID_LullLuck))
    [SID_LullLuck] = MSG_SKILL_LullLuck_NAME,
#endif

#if (defined(SID_LullMagic) && COMMON_SKILL_VALID(SID_LullMagic))
    [SID_LullMagic] = MSG_SKILL_LullMagic_NAME,
#endif

#if (defined(SID_LullMovement) && COMMON_SKILL_VALID(SID_LullMovement))
    [SID_LullMovement] = MSG_SKILL_LullMovement_NAME,
#endif

#if (defined(SID_LullResistance) && COMMON_SKILL_VALID(SID_LullResistance))
    [SID_LullResistance] = MSG_SKILL_LullResistance_NAME,
#endif

#if (defined(SID_LullSkill) && COMMON_SKILL_VALID(SID_LullSkill))
    [SID_LullSkill] = MSG_SKILL_LullSkill_NAME,
#endif

#if (defined(SID_LullSpectrum) && COMMON_SKILL_VALID(SID_LullSpectrum))
    [SID_LullSpectrum] = MSG_SKILL_LullSpectrum_NAME,
#endif

#if (defined(SID_LullSpeed) && COMMON_SKILL_VALID(SID_LullSpeed))
    [SID_LullSpeed] = MSG_SKILL_LullSpeed_NAME,
#endif

#if (defined(SID_LullStrength) && COMMON_SKILL_VALID(SID_LullStrength))
    [SID_LullStrength] = MSG_SKILL_LullStrength_NAME,
#endif

#if (defined(SID_ShrewdPotential) && COMMON_SKILL_VALID(SID_ShrewdPotential))
    [SID_ShrewdPotential] = MSG_SKILL_ShrewdPotential_NAME,
#endif

#if (defined(SID_GoddessDance) && COMMON_SKILL_VALID(SID_GoddessDance))
    [SID_GoddessDance] = MSG_SKILL_GoddessDance_NAME,
#endif

#if (defined(SID_SteadyBrawler) && COMMON_SKILL_VALID(SID_SteadyBrawler))
    [SID_SteadyBrawler] = MSG_SKILL_SteadyBrawler_NAME,
#endif

#if (defined(SID_LiquidOoze) && COMMON_SKILL_VALID(SID_LiquidOoze))
    [SID_LiquidOoze] = MSG_SKILL_LiquidOoze_NAME,
#endif

#if (defined(SID_CloseCombat) && COMMON_SKILL_VALID(SID_CloseCombat))
    [SID_CloseCombat] = MSG_SKILL_CloseCombat_NAME,
#endif

#if (defined(SID_ArmorMarch) && COMMON_SKILL_VALID(SID_ArmorMarch))
    [SID_ArmorMarch] = MSG_SKILL_ArmorMarch_NAME,
#endif

#if (defined(SID_LunarBrace) && COMMON_SKILL_VALID(SID_LunarBrace))
    [SID_LunarBrace] = MSG_SKILL_LunarBrace_NAME,
#endif

#if (defined(SID_QuickLearner) && COMMON_SKILL_VALID(SID_QuickLearner))
    [SID_QuickLearner] = MSG_SKILL_QuickLearner_NAME,
#endif

#if (defined(SID_GoldDigger) && COMMON_SKILL_VALID(SID_GoldDigger))
    [SID_GoldDigger] = MSG_SKILL_GoldDigger_NAME,
#endif

#if (defined(SID_EffectSpore) && COMMON_SKILL_VALID(SID_EffectSpore))
    [SID_EffectSpore] = MSG_SKILL_EffectSpore_NAME,
#endif

#if (defined(SID_DominantArm) && COMMON_SKILL_VALID(SID_DominantArm))
    [SID_DominantArm] = MSG_SKILL_DominantArm_NAME,
#endif

#if (defined(SID_FranticSwing) && COMMON_SKILL_VALID(SID_FranticSwing))
    [SID_FranticSwing] = MSG_SKILL_FranticSwing_NAME,
#endif

#if (defined(SID_CriticalOverload) && COMMON_SKILL_VALID(SID_CriticalOverload))
    [SID_CriticalOverload] = MSG_SKILL_CriticalOverload_NAME,
#endif
};
