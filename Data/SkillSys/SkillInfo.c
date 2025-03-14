#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "constants/gfx.h"

const struct SkillInfo gSkillInfos[MAX_SKILL_NUM + 1] = {
#if (defined(SID_HpBonus) && COMMON_SKILL_VALID(SID_HpBonus))
	[SID_HpBonus] = {
		.desc = MSG_SKILL_HpBonus,
		.icon = GFX_SkillIcon_HpBonus,
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
		.icon = GFX_SkillIcon_DefiantCrit,
	},
#endif

#if (defined(SID_DefiantAvoid) && COMMON_SKILL_VALID(SID_DefiantAvoid))
	[SID_DefiantAvoid] = {
		.desc = MSG_SKILL_DefiantAvoid,
		.icon = GFX_SkillIcon_DefiantAvoid,
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

#if (defined(SID_Nihil) && COMMON_SKILL_VALID(SID_Nihil))
	[SID_Nihil] = {
		.desc = MSG_SKILL_Nihil,
		.icon = GFX_SkillIcon_Nihil,
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
		.name = MSG_SKILL_RuinedBlade_NAME,
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
		.icon = GFX_SkillIcon_Adept,
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

#if (defined(SID_COMBAT_Galeforce) && COMMON_SKILL_VALID(SID_COMBAT_Galeforce))
	[SID_COMBAT_Galeforce] = {
		.desc = MSG_SKILL_COMBAT_Galeforce,
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

#if (defined(SID_Lunge) && COMMON_SKILL_VALID(SID_Lunge))
	[SID_Lunge] = {
		.desc = MSG_SKILL_Lunge,
		.icon = GFX_SkillIcon_Lunge,
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

#if (defined(SID_MovHone) && COMMON_SKILL_VALID(SID_MovHone))
	[SID_MovHone] = {
		.name = MSG_SKILL_MovHone_NAME,
		.desc = MSG_SKILL_MovHone,
		.icon = GFX_SkillIcon_HoneMov
	},
#endif

#if (defined(SID_InnerFlame1) && COMMON_SKILL_VALID(SID_InnerFlame1))
	[SID_InnerFlame1] = {
		.desc = MSG_SKILL_InnerFlame1,
		.icon = GFX_SkillIcon_InnerFlame1,
	},
#endif

#if (defined(SID_InnerFlame2) && COMMON_SKILL_VALID(SID_InnerFlame2))
	[SID_InnerFlame2] = {
		.desc = MSG_SKILL_InnerFlame2,
		.icon = GFX_SkillIcon_InnerFlame2,
	},
#endif

#if (defined(SID_InnerFlame3) && COMMON_SKILL_VALID(SID_InnerFlame3))
	[SID_InnerFlame3] = {
		.desc = MSG_SKILL_InnerFlame3,
		.icon = GFX_SkillIcon_InnerFlame3,
	},
#endif

#if (defined(SID_Steal) && COMMON_SKILL_VALID(SID_Steal))
	[SID_Steal] = {
		.desc = MSG_SKILL_Steal,
		.icon = GFX_SkillIcon_Steal,
	},
#endif

#if (defined(SID_HealingFocus) && COMMON_SKILL_VALID(SID_HealingFocus))
	[SID_HealingFocus] = {
		.desc = MSG_SKILL_HealingFocus,
		.icon = GFX_SkillIcon_WIP,
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

#if (defined(SID_MaleficAura) && (SID_MaleficAura < MAX_SKILL_NUM))
	[SID_MaleficAura] = {
		.desc = MSG_SKILL_MaleficAura,
		.icon = GFX_SkillIcon_MaleficAura,
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

#if (defined(SID_MagicEye) && COMMON_SKILL_VALID(SID_MagicEye))
	[SID_MagicEye] = {
		.desc = MSG_SKILL_MagicEye,
		.icon = GFX_SkillIcon_MagicEye,
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

#if (defined(SID_ChargePlus) && COMMON_SKILL_VALID(SID_ChargePlus))
	[SID_ChargePlus] = {
		.desc = MSG_SKILL_ChargePlus,
		.icon = GFX_SkillIcon_ChargePlus,
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
		.icon = GFX_SkillIcon_OutdoorFighter,
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

#if (defined(SID_StrongCon) && COMMON_SKILL_VALID(SID_StrongCon))
	[SID_StrongCon] = {
		.desc = MSG_SKILL_StrongCon,
		.icon = GFX_SkillIcon_StrongCon,
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

#if (defined(SID_DefenseBoost) && COMMON_SKILL_VALID(SID_DefenseBoost))
	[SID_DefenseBoost] = {
		.name = MSG_SKILL_DefenseBoost_NAME,
		.desc = MSG_SKILL_DefenseBoost,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_LuckBoost) && COMMON_SKILL_VALID(SID_LuckBoost))
	[SID_LuckBoost] = {
		.name = MSG_SKILL_LuckBoost_NAME,
		.desc = MSG_SKILL_LuckBoost,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_MagicBoost) && COMMON_SKILL_VALID(SID_MagicBoost))
	[SID_MagicBoost] = {
		.name = MSG_SKILL_MagicBoost_NAME,
		.desc = MSG_SKILL_MagicBoost,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_MoveBoost) && COMMON_SKILL_VALID(SID_MoveBoost))
	[SID_MoveBoost] = {
		.name = MSG_SKILL_MoveBoost_NAME,
		.desc = MSG_SKILL_MoveBoost,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_ResistanceBoost) && COMMON_SKILL_VALID(SID_ResistanceBoost))
	[SID_ResistanceBoost] = {
		.name = MSG_SKILL_ResistanceBoost_NAME,
		.desc = MSG_SKILL_ResistanceBoost,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_SkillBoost) && COMMON_SKILL_VALID(SID_SkillBoost))
	[SID_SkillBoost] = {
		.name = MSG_SKILL_SkillBoost_NAME,
		.desc = MSG_SKILL_SkillBoost,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_SpeedBoost) && COMMON_SKILL_VALID(SID_SpeedBoost))
	[SID_SpeedBoost] = {
		.name = MSG_SKILL_SpeedBoost_NAME,
		.desc = MSG_SKILL_SpeedBoost,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_StrengthBoost) && COMMON_SKILL_VALID(SID_StrengthBoost))
	[SID_StrengthBoost] = {
		.name = MSG_SKILL_StrengthBoost_NAME,
		.desc = MSG_SKILL_StrengthBoost,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_SpectrumBoost) && COMMON_SKILL_VALID(SID_SpectrumBoost))
	[SID_SpectrumBoost] = {
		.name = MSG_SKILL_SpectrumBoost_NAME,
		.desc = MSG_SKILL_SpectrumBoost,
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
		.name = MSG_SKILL_RuinedBladePlus_NAME,
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

#if (defined(SID_LightRune) && COMMON_SKILL_VALID(SID_LightRune))
	[SID_LightRune] = {
		.desc = MSG_SKILL_LightRune,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Mine) && COMMON_SKILL_VALID(SID_Mine))
	[SID_Mine] = {
		.desc = MSG_SKILL_Mine,
		.icon = GFX_SkillIcon_WIP,
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
		.icon = GFX_SkillIcon_Analytic,
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
		.icon = GFX_SkillIcon_LightAndDark,
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
		.icon = GFX_SkillIcon_DivinePulse,
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

#if (defined(SID_Bushido) && COMMON_SKILL_VALID(SID_Bushido))
	[SID_Bushido] = {
		.desc = MSG_SKILL_Bushido,
		.icon = GFX_SkillIcon_Bushido,
	},
#endif

#if (defined(SID_DragonWall) && COMMON_SKILL_VALID(SID_DragonWall))
	[SID_DragonWall] = {
		.desc = MSG_SKILL_DragonWall,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_DragonWarth) && COMMON_SKILL_VALID(SID_DragonWarth))
	[SID_DragonWarth] = {
		.name = MSG_SKILL_DragonWarth_NAME,
		.desc = MSG_SKILL_DragonWarth,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_BlueLionRule) && COMMON_SKILL_VALID(SID_BlueLionRule))
	[SID_BlueLionRule] = {
		.name = MSG_SKILL_BlueLionRule_NAME,
		.desc = MSG_SKILL_BlueLionRule,
		.icon = GFX_SkillIcon_BlueLionRule,
	},
#endif

#if (defined(SID_FaerghusAncestry) && COMMON_SKILL_VALID(SID_FaerghusAncestry))
	[SID_FaerghusAncestry] = {
		.name = MSG_SKILL_FaerghusAncestry_NAME,
		.desc = MSG_SKILL_FaerghusAncestry,
		.icon = GFX_SkillIcon_FaerghusAncestry,
	},
#endif

#if (defined(SID_CrusaderWard) && COMMON_SKILL_VALID(SID_CrusaderWard))
	[SID_CrusaderWard] = {
		.name = MSG_SKILL_CrusaderWard_NAME,
		.desc = MSG_SKILL_CrusaderWard,
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
		.icon = GFX_SkillIcon_Chlorophyll,
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
		.icon = GFX_SkillIcon_RainDish,
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

#if (defined(SID_Pass) && COMMON_SKILL_VALID(SID_Pass))
	[SID_Pass] = {
		.desc = MSG_SKILL_Pass,
		.icon = GFX_SkillIcon_Pass,
	},
#endif

#if (defined(SID_Obstruct) && COMMON_SKILL_VALID(SID_Obstruct))
	[SID_Obstruct] = {
		.desc = MSG_SKILL_Obstruct,
		.icon = GFX_SkillIcon_Obstruct,
	},
#endif

#if (defined(SID_Guidance) && COMMON_SKILL_VALID(SID_Guidance))
	[SID_Guidance] = {
		.desc = MSG_SKILL_Guidance,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_FlierGuidance) && COMMON_SKILL_VALID(SID_FlierGuidance))
	[SID_FlierGuidance] = {
		.name = MSG_SKILL_FlierGuidance_NAME,
		.desc = MSG_SKILL_FlierGuidance,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_SoaringGuidance) && COMMON_SKILL_VALID(SID_SoaringGuidance))
	[SID_SoaringGuidance] = {
		.name = MSG_SKILL_SoaringGuidance_NAME,
		.desc = MSG_SKILL_SoaringGuidance,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Aerobatics) && COMMON_SKILL_VALID(SID_Aerobatics))
	[SID_Aerobatics] = {
		.desc = MSG_SKILL_Aerobatics,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_FlierFormation) && COMMON_SKILL_VALID(SID_FlierFormation))
	[SID_FlierFormation] = {
		.name = MSG_SKILL_FlierFormation_NAME,
		.desc = MSG_SKILL_FlierFormation,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_SoaringWings) && COMMON_SKILL_VALID(SID_SoaringWings))
	[SID_SoaringWings] = {
		.name = MSG_SKILL_SoaringWings_NAME,
		.desc = MSG_SKILL_SoaringWings,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_DetailedReport) && COMMON_SKILL_VALID(SID_DetailedReport))
	[SID_DetailedReport] = {
		.name = MSG_SKILL_DetailedReport_NAME,
		.desc = MSG_SKILL_DetailedReport,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_VengefulFighter) && COMMON_SKILL_VALID(SID_VengefulFighter))
	[SID_VengefulFighter] = {
		 .name = MSG_SKILL_VengefulFighter_NAME,
		 .desc = MSG_SKILL_VengefulFighter,
		 .icon = GFX_SkillIcon_VengefulFighter,
	},
#endif

#if (defined(SID_RecklessFighter) && COMMON_SKILL_VALID(SID_RecklessFighter))
	[SID_RecklessFighter] = {
		 .name = MSG_SKILL_RecklessFighter_NAME,
		 .desc = MSG_SKILL_RecklessFighter,
		 .icon = GFX_SkillIcon_RecklessFighter,
	},
#endif

#if (defined(SID_BoldFighter) && COMMON_SKILL_VALID(SID_BoldFighter))
	[SID_BoldFighter] = {
		 .name = MSG_SKILL_BoldFighter_NAME,
		 .desc = MSG_SKILL_BoldFighter,
		 .icon = GFX_SkillIcon_BoldFighter,
	},
#endif

#if (defined(SID_Dazzle) && COMMON_SKILL_VALID(SID_Dazzle))
	[SID_Dazzle] = {
		 .desc = MSG_SKILL_Dazzle,
		 .icon = GFX_SkillIcon_Dazzle,
	},
#endif

#if (defined(SID_Boon) && COMMON_SKILL_VALID(SID_Boon))
	[SID_Boon] = {
		 .desc = MSG_SKILL_Boon,
		 .icon = GFX_SkillIcon_Boon,
	},
#endif

#if (defined(SID_BidingBlow) && COMMON_SKILL_VALID(SID_BidingBlow))
	[SID_BidingBlow] = {
		 .desc = MSG_SKILL_BidingBlow,
		 .icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Discipline) && COMMON_SKILL_VALID(SID_Discipline))
	[SID_Discipline] = {
		 .desc = MSG_SKILL_Discipline,
		 .icon = GFX_SkillIcon_Discipline,
	},
#endif

#if (defined(SID_AdvantageChaser) && COMMON_SKILL_VALID(SID_AdvantageChaser))
	[SID_AdvantageChaser] = {
		 .name = MSG_SKILL_AdvantageChaser_NAME,
		 .desc = MSG_SKILL_AdvantageChaser,
		 .icon = GFX_SkillIcon_AdvantageChaser,
	},
#endif

#if (defined(SID_Nonconforming) && COMMON_SKILL_VALID(SID_Nonconforming))
	[SID_Nonconforming] = {
		.name = MSG_SKILL_Nonconforming_NAME,
		.desc = MSG_SKILL_Nonconforming,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Moonlight) && COMMON_SKILL_VALID(SID_Moonlight))
	[SID_Moonlight] = {
		 .desc = MSG_SKILL_Moonlight,
		 .icon = GFX_SkillIcon_Moonlight,
	},
#endif

#if (defined(SID_RouseDefense) && COMMON_SKILL_VALID(SID_RouseDefense))
	[SID_RouseDefense] = {
		.name = MSG_SKILL_RouseDefense_NAME,
		.desc = MSG_SKILL_RouseDefense,
		.icon = GFX_SkillIcon_RouseDefense,
	},
#endif

#if (defined(SID_RouseLuck) && COMMON_SKILL_VALID(SID_RouseLuck))
	[SID_RouseLuck] = {
		.name = MSG_SKILL_RouseLuck_NAME,
		.desc = MSG_SKILL_RouseLuck,
		.icon = GFX_SkillIcon_RouseLuck,
	},
#endif

#if (defined(SID_RouseMagic) && COMMON_SKILL_VALID(SID_RouseMagic))
	[SID_RouseMagic] = {
		.name = MSG_SKILL_RouseMagic_NAME,
		.desc = MSG_SKILL_RouseMagic,
		.icon = GFX_SkillIcon_RouseMagic,
	},
#endif

#if (defined(SID_RouseMovement) && COMMON_SKILL_VALID(SID_RouseMovement))
	[SID_RouseMovement] = {
		.name = MSG_SKILL_RouseMovement_NAME,
		.desc = MSG_SKILL_RouseMovement,
		.icon = GFX_SkillIcon_RouseMovement,
	},
#endif

#if (defined(SID_RouseResistance) && COMMON_SKILL_VALID(SID_RouseResistance))
	[SID_RouseResistance] = {
		.name = MSG_SKILL_RouseResistance_NAME,
		.desc = MSG_SKILL_RouseResistance,
		.icon = GFX_SkillIcon_RouseResistance,
	},
#endif

#if (defined(SID_RouseSkill) && COMMON_SKILL_VALID(SID_RouseSkill))
	[SID_RouseSkill] = {
		.name = MSG_SKILL_RouseSkill_NAME,
		.desc = MSG_SKILL_RouseSkill,
		.icon = GFX_SkillIcon_RouseSkill,
	},
#endif

#if (defined(SID_RouseSpeed) && COMMON_SKILL_VALID(SID_RouseSpeed))
	[SID_RouseSpeed] = {
		.name = MSG_SKILL_RouseSpeed_NAME,
		.desc = MSG_SKILL_RouseSpeed,
		.icon = GFX_SkillIcon_RouseSpeed,
	},
#endif

#if (defined(SID_RouseStrength) && COMMON_SKILL_VALID(SID_RouseStrength))
	[SID_RouseStrength] = {
		.name = MSG_SKILL_RouseStrength_NAME,
		.desc = MSG_SKILL_RouseStrength,
		.icon = GFX_SkillIcon_RouseStrength,
	},
#endif

#if (defined(SID_BlackMagic) && COMMON_SKILL_VALID(SID_BlackMagic))
	[SID_BlackMagic] = {
		.desc = MSG_SKILL_BlackMagic,
		.icon = GFX_SkillIcon_BlackMagic,
	},
#endif

#if (defined(SID_Relief) && COMMON_SKILL_VALID(SID_Relief))
	[SID_Relief] = {
		.desc = MSG_SKILL_Relief,
		.icon = GFX_SkillIcon_Relief,
	},
#endif

#if (defined(SID_Camaraderie) && COMMON_SKILL_VALID(SID_Camaraderie))
	[SID_Camaraderie] = {
		.desc = MSG_SKILL_Camaraderie,
		.icon = GFX_SkillIcon_Camaraderie,
	},
#endif

#if (defined(SID_Amaterasu) && (SID_Amaterasu < MAX_SKILL_NUM))
	[SID_Amaterasu] = {
		.desc = MSG_SKILL_Amaterasu,
		.icon = GFX_SkillIcon_Amaterasu,
	  },
#endif

#if (defined(SID_Counter) && COMMON_SKILL_VALID(SID_Counter))
	[SID_Counter] = {
		.desc = MSG_SKILL_Counter,
		.icon = GFX_SkillIcon_Counter,
	},
#endif

#if (defined(SID_LastWord) && COMMON_SKILL_VALID(SID_LastWord))
	[SID_LastWord] = {
		 .desc = MSG_SKILL_LastWord,
		 .icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_PowerStaff) && COMMON_SKILL_VALID(SID_PowerStaff))
	[SID_PowerStaff] = {
		.desc = MSG_SKILL_PowerStaff,
		.icon = GFX_SkillIcon_PowerStaff,
	},
#endif

#if (defined(SID_Multiscale) && COMMON_SKILL_VALID(SID_Multiscale))
	[SID_Multiscale] = {
		.desc = MSG_SKILL_Multiscale,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_PridefulWarrior) && COMMON_SKILL_VALID(SID_PridefulWarrior))
	[SID_PridefulWarrior] = {
		 .name = MSG_SKILL_PridefulWarrior_NAME,
		 .desc = MSG_SKILL_PridefulWarrior,
		 .icon = GFX_SkillIcon_PridefulWarrior,
	},
#endif

#if (defined(SID_LiveToServe) && COMMON_SKILL_VALID(SID_LiveToServe))
	[SID_LiveToServe] = {
		.desc = MSG_SKILL_LiveToServe,
		.icon = GFX_SkillIcon_LiveToServe,
	},
#endif

#if (defined(SID_MageSlayer) && COMMON_SKILL_VALID(SID_MageSlayer))
	[SID_MageSlayer] = {
		.desc = MSG_SKILL_MageSlayer,
		.icon = GFX_SkillIcon_MageSlayer,
	},
#endif

#if (defined(SID_FailGale) && COMMON_SKILL_VALID(SID_FailGale))
	[SID_FailGale] = {
		.desc = MSG_SKILL_FailGale,
		.icon = GFX_SkillIcon_FailGale,
	},
#endif

#if (defined(SID_Clearance) && COMMON_SKILL_VALID(SID_Clearance))
	[SID_Clearance] = {
		.desc = MSG_SKILL_Clearance,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_TriangleAdept) && COMMON_SKILL_VALID(SID_TriangleAdept))
	[SID_TriangleAdept] = {
		.name = MSG_SKILL_TriangleAdept_NAME,
		.desc = MSG_SKILL_TriangleAdept,
		.icon = GFX_SkillIcon_TriangleAdept,
	},
#endif

#if (defined(SID_TriangleAdeptPlus) && COMMON_SKILL_VALID(SID_TriangleAdeptPlus))
	[SID_TriangleAdeptPlus] = {
		.name = MSG_SKILL_TriangleAdeptPlus_NAME,
		.desc = MSG_SKILL_TriangleAdeptPlus,
		.icon = GFX_SkillIcon_TriangleAdeptPlus,
	},
#endif

#if (defined(SID_TowerShieldPlus) && COMMON_SKILL_VALID(SID_TowerShieldPlus))
	[SID_TowerShieldPlus] = {
		.desc = MSG_SKILL_TowerShieldPlus,
		.icon = GFX_SkillIcon_TowerShieldPlus,
	},
#endif

#if (defined(SID_SolarPower) && COMMON_SKILL_VALID(SID_SolarPower))
	[SID_SolarPower] = {
		.desc = MSG_SKILL_SolarPower,
		.icon = GFX_SkillIcon_SolarPower,
	},
#endif

#if (defined(SID_COMBAT_Gamble) && COMMON_SKILL_VALID(SID_COMBAT_Gamble))
	[SID_COMBAT_Gamble] = {
		.desc = MSG_SKILL_COMBAT_Gamble,
		.icon = GFX_SkillIcon_Gamble,
	},
#endif

#if (defined(SID_PoisonStrike) && COMMON_SKILL_VALID(SID_PoisonStrike))
	[SID_PoisonStrike] = {
		.name = MSG_SKILL_PoisonStrike_NAME,
		.desc = MSG_SKILL_PoisonStrike,
		.icon = GFX_SkillIcon_PoisonStrike,
	},
#endif

#if (defined(SID_GrislyWound) && COMMON_SKILL_VALID(SID_GrislyWound))
	[SID_GrislyWound] = {
		.name = MSG_SKILL_GrislyWound_NAME,
		.desc = MSG_SKILL_GrislyWound,
		.icon = GFX_SkillIcon_GrislyWound,
	},
#endif

#if (defined(SID_Poise) && COMMON_SKILL_VALID(SID_Poise))
	[SID_Poise] = {
		.desc = MSG_SKILL_Poise,
		.icon = GFX_SkillIcon_Poise,
	},
#endif

#if (defined(SID_COMBAT_Detonate) && COMMON_SKILL_VALID(SID_COMBAT_Detonate))
	[SID_COMBAT_Detonate] = {
		.desc = MSG_SKILL_COMBAT_Detonate,
		.icon = GFX_SkillIcon_Detonate,
	},
#endif

#if (defined(SID_Enrage) && COMMON_SKILL_VALID(SID_Enrage))
	[SID_Enrage] = {
		.desc = MSG_SKILL_Enrage,
		.icon = GFX_SkillIcon_Enrage,
	},
#endif

#if (defined(SID_Petrify) && COMMON_SKILL_VALID(SID_Petrify))
	[SID_Petrify] = {
		.desc = MSG_SKILL_Petrify,
		.icon = GFX_SkillIcon_Petrify,
	},
#endif

#if (defined(SID_AirRaidAttack) && COMMON_SKILL_VALID(SID_AirRaidAttack))
	[SID_AirRaidAttack] = {
		.name = MSG_SKILL_AirRaidAttack_NAME,
		.desc = MSG_SKILL_AirRaidAttack,
		.icon = GFX_SkillIcon_AirRaidAttack,
	},
#endif

#if (defined(SID_AirRaidAvoid) && COMMON_SKILL_VALID(SID_AirRaidAvoid))
	[SID_AirRaidAvoid] = {
		.name = MSG_SKILL_AirRaidAvoid_NAME,
		.desc = MSG_SKILL_AirRaidAvoid,
		.icon = GFX_SkillIcon_AirRaidAvoid,
	},
#endif

#if (defined(SID_AirRaidCrit) && COMMON_SKILL_VALID(SID_AirRaidCrit))
	[SID_AirRaidCrit] = {
		.name = MSG_SKILL_AirRaidCrit_NAME,
		.desc = MSG_SKILL_AirRaidCrit,
		.icon = GFX_SkillIcon_AirRaidCrit,
	},
#endif

#if (defined(SID_AirRaidDefense) && COMMON_SKILL_VALID(SID_AirRaidDefense))
	[SID_AirRaidDefense] = {
		.name = MSG_SKILL_AirRaidDefense_NAME,
		.desc = MSG_SKILL_AirRaidDefense,
		.icon = GFX_SkillIcon_AirRaidDefense,
	},
#endif

#if (defined(SID_AirRaidHit) && COMMON_SKILL_VALID(SID_AirRaidHit))
	[SID_AirRaidHit] = {
		.name = MSG_SKILL_AirRaidHit_NAME,
		.desc = MSG_SKILL_AirRaidHit,
		.icon = GFX_SkillIcon_AirRaidHit,
	},
#endif

#if (defined(SID_AirRaidResistance) && COMMON_SKILL_VALID(SID_AirRaidResistance))
	[SID_AirRaidResistance] = {
		.name = MSG_SKILL_AirRaidResistance_NAME,
		.desc = MSG_SKILL_AirRaidResistance,
		.icon = GFX_SkillIcon_AirRaidResistance,
	},
#endif

#if (defined(SID_AirRaidSpeed) && COMMON_SKILL_VALID(SID_AirRaidSpeed))
	[SID_AirRaidSpeed] = {
		.name = MSG_SKILL_AirRaidSpeed_NAME,
		.desc = MSG_SKILL_AirRaidSpeed,
		.icon = GFX_SkillIcon_AirRaidSpeed,
	},
#endif

#if (defined(SID_FaireAnima) && COMMON_SKILL_VALID(SID_FaireAnima))
	[SID_FaireAnima] = {
		.desc = MSG_SKILL_FaireAnima,
		.icon = GFX_SkillIcon_FaireAnima,
	},
#endif


#if (defined(SID_FaireLight) && COMMON_SKILL_VALID(SID_FaireLight))
	[SID_FaireLight] = {
		.desc = MSG_SKILL_FaireLight,
		.icon = GFX_SkillIcon_FaireLight,
	},
#endif


#if (defined(SID_FaireDark) && COMMON_SKILL_VALID(SID_FaireDark))
	[SID_FaireDark] = {
		.desc = MSG_SKILL_FaireDark,
		.icon = GFX_SkillIcon_FaireDark,
	},
#endif

#if (defined(SID_SealDefense) && COMMON_SKILL_VALID(SID_SealDefense))
	[SID_SealDefense] = {
		.name = MSG_SKILL_SealDefense_NAME,
		.desc = MSG_SKILL_SealDefense,
		.icon = GFX_SkillIcon_SealDefense
	},
#endif

#if (defined(SID_SealLuck) && COMMON_SKILL_VALID(SID_SealLuck))
	[SID_SealLuck] = {
		.name = MSG_SKILL_SealLuck_NAME,
		.desc = MSG_SKILL_SealLuck,
		.icon = GFX_SkillIcon_SealLuck
	},
#endif

#if (defined(SID_SealMagic) && COMMON_SKILL_VALID(SID_SealMagic))
	[SID_SealMagic] = {
		.name = MSG_SKILL_SealMagic_NAME,
		.desc = MSG_SKILL_SealMagic,
		.icon = GFX_SkillIcon_SealMagic
	},
#endif

#if (defined(SID_SealResistance) && COMMON_SKILL_VALID(SID_SealResistance))
	[SID_SealResistance] = {
		.name = MSG_SKILL_SealResistance_NAME,
		.desc = MSG_SKILL_SealResistance,
		.icon = GFX_SkillIcon_SealResistance
	},
#endif

#if (defined(SID_SealSkill) && COMMON_SKILL_VALID(SID_SealSkill))
	[SID_SealSkill] = {
		.name = MSG_SKILL_SealSkill_NAME,
		.desc = MSG_SKILL_SealSkill,
		.icon = GFX_SkillIcon_SealSkill
	},
#endif

#if (defined(SID_SealSpeed) && COMMON_SKILL_VALID(SID_SealSpeed))
	[SID_SealSpeed] = {
		.name = MSG_SKILL_SealSpeed_NAME,
		.desc = MSG_SKILL_SealSpeed,
		.icon = GFX_SkillIcon_SealSpeed
	},
#endif

#if (defined(SID_SealStrength) && COMMON_SKILL_VALID(SID_SealStrength))
	[SID_SealStrength] = {
		.name = MSG_SKILL_SealStrength_NAME,
		.desc = MSG_SKILL_SealStrength,
		.icon = GFX_SkillIcon_SealStrength
	},
#endif

#if (defined(SID_RallyDefense) && COMMON_SKILL_VALID(SID_RallyDefense))
	[SID_RallyDefense] = {
		.name = MSG_SKILL_RallyDefense_NAME,
		.desc = MSG_SKILL_RallyDefense,
		.icon = GFX_SkillIcon_RallyDefense,
	},
#endif

#if (defined(SID_RallyLuck) && COMMON_SKILL_VALID(SID_RallyLuck))
	[SID_RallyLuck] = {
		.name = MSG_SKILL_RallyLuck_NAME,
		.desc = MSG_SKILL_RallyLuck,
		.icon = GFX_SkillIcon_RallyLuck,
	},
#endif

#if (defined(SID_RallyMagic) && COMMON_SKILL_VALID(SID_RallyMagic))
	[SID_RallyMagic] = {
		.name = MSG_SKILL_RallyMagic_NAME,
		.desc = MSG_SKILL_RallyMagic,
		.icon = GFX_SkillIcon_RallyMagic,
	},
#endif

#if (defined(SID_RallyMovement) && COMMON_SKILL_VALID(SID_RallyMovement))
	[SID_RallyMovement] = {
		.name = MSG_SKILL_RallyMovement_NAME,
		.desc = MSG_SKILL_RallyMovement,
		.icon = GFX_SkillIcon_RallyMovement,
	},
#endif

#if (defined(SID_RallyResistance) && COMMON_SKILL_VALID(SID_RallyResistance))
	[SID_RallyResistance] = {
		.name = MSG_SKILL_RallyResistance_NAME,
		.desc = MSG_SKILL_RallyResistance,
		.icon = GFX_SkillIcon_RallyResistance,
	},
#endif

#if (defined(SID_RallySkill) && COMMON_SKILL_VALID(SID_RallySkill))
	[SID_RallySkill] = {
		.name = MSG_SKILL_RallySkill_NAME,
		.desc = MSG_SKILL_RallySkill,
		.icon = GFX_SkillIcon_RallySkill,
	},
#endif

#if (defined(SID_RallySpeed) && COMMON_SKILL_VALID(SID_RallySpeed))
	[SID_RallySpeed] = {
		.name = MSG_SKILL_RallySpeed_NAME,
		.desc = MSG_SKILL_RallySpeed,
		.icon = GFX_SkillIcon_RallySpeed,
	},
#endif

#if (defined(SID_RallyStrength) && COMMON_SKILL_VALID(SID_RallyStrength))
	[SID_RallyStrength] = {
		.name = MSG_SKILL_RallyStrength_NAME,
		.desc = MSG_SKILL_RallyStrength,
		.icon = GFX_SkillIcon_RallyStrength,
	},
#endif

#if (defined(SID_RallySpectrum) && COMMON_SKILL_VALID(SID_RallySpectrum))
	[SID_RallySpectrum] = {
		.name = MSG_SKILL_RallySpectrum_NAME,
		.desc = MSG_SKILL_RallySpectrum,
		.icon = GFX_SkillIcon_RallySpectrum,
	},
#endif

#if (defined(SID_Despoil) && COMMON_SKILL_VALID(SID_Despoil))
	[SID_Despoil] = {
		.desc = MSG_SKILL_Despoil,
		.icon = GFX_SkillIcon_Despoil,
	},
#endif

#if (defined(SID_Pickup) && COMMON_SKILL_VALID(SID_Pickup))
	[SID_Pickup] = {
		.desc = MSG_SKILL_Pickup,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_IndoorMarch) && COMMON_SKILL_VALID(SID_IndoorMarch))
	[SID_IndoorMarch] = {
		.name = MSG_SKILL_IndoorMarch_NAME,
		.desc = MSG_SKILL_IndoorMarch,
		.icon = GFX_SkillIcon_IndoorMarch,
	},
#endif

#if (defined(SID_DevilsPact) && COMMON_SKILL_VALID(SID_DevilsPact))
	[SID_DevilsPact] = {
		.name = MSG_SKILL_DevilsPact_NAME,
		.desc = MSG_SKILL_DevilsPact,
		.icon = GFX_SkillIcon_DevilsPact,
	},
#endif

#if (defined(SID_DevilsWhim) && COMMON_SKILL_VALID(SID_DevilsWhim))
	[SID_DevilsWhim] = {
		.name = MSG_SKILL_DevilsWhim_NAME,
		.desc = MSG_SKILL_DevilsWhim,
		.icon = GFX_SkillIcon_DevilsWhim,
	},
#endif

#if (defined(SID_DevilsLuck) && COMMON_SKILL_VALID(SID_DevilsLuck))
	[SID_DevilsLuck] = {
		.name = MSG_SKILL_DevilsLuck_NAME,
		.desc = MSG_SKILL_DevilsLuck,
		.icon = GFX_SkillIcon_DevilsLuck,
	},
#endif

#if (defined(SID_Expertise) && COMMON_SKILL_VALID(SID_Expertise))
	[SID_Expertise] = {
		.desc = MSG_SKILL_Expertise,
		.icon = GFX_SkillIcon_Expertise,
	},
#endif

#if (defined(SID_PassionsFlow) && COMMON_SKILL_VALID(SID_PassionsFlow))
	[SID_PassionsFlow] = {
		 .name = MSG_SKILL_PassionsFlow_NAME,
		 .desc = MSG_SKILL_PassionsFlow,
		 .icon = GFX_SkillIcon_PassionsFlow,
	},
#endif

#if (defined(SID_NatureRush) && COMMON_SKILL_VALID(SID_NatureRush))
	[SID_NatureRush] = {
		.name = MSG_SKILL_NatureRush_NAME,
		.desc = MSG_SKILL_NatureRush,
		.icon = GFX_SkillIcon_NatureRush,
	},
#endif

#if (defined(SID_CounterMagic) && COMMON_SKILL_VALID(SID_CounterMagic))
	[SID_CounterMagic] = {
		.name = MSG_SKILL_CounterMagic_NAME,
		.desc = MSG_SKILL_CounterMagic,
		.icon = GFX_SkillIcon_CounterMagic,
	},
#endif

#if (defined(SID_LadyBlade) && COMMON_SKILL_VALID(SID_LadyBlade))
	[SID_LadyBlade] = {
		.name = MSG_SKILL_LadyBlade_NAME,
		.desc = MSG_SKILL_LadyBlade,
		.icon = GFX_SkillIcon_LadyBlade,
	},
#endif

#if (defined(SID_LullDefense) && COMMON_SKILL_VALID(SID_LullDefense))
	[SID_LullDefense] = {
		.name = MSG_SKILL_LullDefense_NAME,
		.desc = MSG_SKILL_LullDefense,
		.icon = GFX_SkillIcon_LullDefense,
	},
#endif

#if (defined(SID_LullLuck) && COMMON_SKILL_VALID(SID_LullLuck))
	[SID_LullLuck] = {
		.name = MSG_SKILL_LullLuck_NAME,
		.desc = MSG_SKILL_LullLuck,
		.icon = GFX_SkillIcon_LullLuck,
	},
#endif

#if (defined(SID_LullMagic) && COMMON_SKILL_VALID(SID_LullMagic))
	[SID_LullMagic] = {
		.name = MSG_SKILL_LullMagic_NAME,
		.desc = MSG_SKILL_LullMagic,
		.icon = GFX_SkillIcon_LullMagic,
	},
#endif

#if (defined(SID_LullMovement) && COMMON_SKILL_VALID(SID_LullMovement))
	[SID_LullMovement] = {
		.name = MSG_SKILL_LullMovement_NAME,
		.desc = MSG_SKILL_LullMovement,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_LullResistance) && COMMON_SKILL_VALID(SID_LullResistance))
	[SID_LullResistance] = {
		.name = MSG_SKILL_LullResistance_NAME,
		.desc = MSG_SKILL_LullResistance,
		.icon = GFX_SkillIcon_LullResistance,
	},
#endif

#if (defined(SID_LullSkill) && COMMON_SKILL_VALID(SID_LullSkill))
	[SID_LullSkill] = {
		.name = MSG_SKILL_LullSkill_NAME,
		.desc = MSG_SKILL_LullSkill,
		.icon = GFX_SkillIcon_LullSkill,
	},
#endif

#if (defined(SID_LullSpectrum) && COMMON_SKILL_VALID(SID_LullSpectrum))
	[SID_LullSpectrum] = {
		.name = MSG_SKILL_LullSpectrum_NAME,
		.desc = MSG_SKILL_LullSpectrum,
		.icon = GFX_SkillIcon_LullSpectrum,
	},
#endif

#if (defined(SID_LullSpeed) && COMMON_SKILL_VALID(SID_LullSpeed))
	[SID_LullSpeed] = {
		.name = MSG_SKILL_LullSpeed_NAME,
		.desc = MSG_SKILL_LullSpeed,
		.icon = GFX_SkillIcon_LullSpeed,
	},
#endif

#if (defined(SID_LullStrength) && COMMON_SKILL_VALID(SID_LullStrength))
	[SID_LullStrength] = {
		.name = MSG_SKILL_LullStrength_NAME,
		.desc = MSG_SKILL_LullStrength,
		.icon = GFX_SkillIcon_LullStrength,
	},
#endif

#if (defined(SID_ShrewdPotential) && COMMON_SKILL_VALID(SID_ShrewdPotential))
	[SID_ShrewdPotential] = {
		.name = MSG_SKILL_ShrewdPotential_NAME,
		.desc = MSG_SKILL_ShrewdPotential,
		.icon = GFX_SkillIcon_ShrewdPotential,
	},
#endif

#if (defined(SID_GoddessDance) && COMMON_SKILL_VALID(SID_GoddessDance))
	[SID_GoddessDance] = {
		.name = MSG_SKILL_GoddessDance_NAME,
		.desc = MSG_SKILL_GoddessDance,
		.icon = GFX_SkillIcon_GoddessDance,
	},
#endif

#if (defined(SID_SteadyBrawler) && COMMON_SKILL_VALID(SID_SteadyBrawler))
	[SID_SteadyBrawler] = {
		.name = MSG_SKILL_SteadyBrawler_NAME,
		.desc = MSG_SKILL_SteadyBrawler,
		.icon = GFX_SkillIcon_SteadyBrawler,
	},
#endif

#if (defined(SID_KeepUp) && COMMON_SKILL_VALID(SID_KeepUp))
	[SID_KeepUp] = {
		.desc = MSG_SKILL_KeepUp,
		.icon = GFX_SkillIcon_KeepUp,
	},
#endif

#if (defined(SID_LiquidOoze) && COMMON_SKILL_VALID(SID_LiquidOoze))
	[SID_LiquidOoze] = {
		.name = MSG_SKILL_LiquidOoze_NAME,
		.desc = MSG_SKILL_LiquidOoze,
		.icon = GFX_SkillIcon_LiquidOoze,
	},
#endif

#if (defined(SID_CloseCombat) && COMMON_SKILL_VALID(SID_CloseCombat))
	[SID_CloseCombat] = {
		.name = MSG_SKILL_CloseCombat_NAME,
		.desc = MSG_SKILL_CloseCombat,
		.icon = GFX_SkillIcon_CloseCombat,
	},
#endif

#if (defined(SID_ArmorMarch) && COMMON_SKILL_VALID(SID_ArmorMarch))
	[SID_ArmorMarch] = {
		.name = MSG_SKILL_ArmorMarch_NAME,
		.desc = MSG_SKILL_ArmorMarch,
		.icon = GFX_SkillIcon_ArmorMarch,
	},
#endif

#if (defined(SID_Eclipse) && COMMON_SKILL_VALID(SID_Eclipse))
	[SID_Eclipse] = {
		.desc = MSG_SKILL_Eclipse,
		.icon = GFX_SkillIcon_Eclipse,
	},
#endif

#if (defined(SID_Barricade) && COMMON_SKILL_VALID(SID_Barricade))
	[SID_Barricade] = {
		.desc = MSG_SKILL_Barricade,
		.icon = GFX_SkillIcon_Barricade,
	},
#endif

#if (defined(SID_BarricadePlus) && COMMON_SKILL_VALID(SID_BarricadePlus))
	[SID_BarricadePlus] = {
		.desc = MSG_SKILL_BarricadePlus,
		.icon = GFX_SkillIcon_BarricadePlus,
	},
#endif

#if (defined(SID_LunarBrace) && COMMON_SKILL_VALID(SID_LunarBrace))
	[SID_LunarBrace] = {
		.name = MSG_SKILL_LunarBrace_NAME,
		.desc = MSG_SKILL_LunarBrace,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_SoulSap) && COMMON_SKILL_VALID(SID_SoulSap))
	[SID_SoulSap] = {
		.desc = MSG_SKILL_SoulSap,
		.icon = GFX_SkillIcon_SoulSap,
	},
#endif

#if (defined(SID_Pursuer) && COMMON_SKILL_VALID(SID_Pursuer))
	[SID_Pursuer] = {
		.desc = MSG_SKILL_Pursuer,
		.icon = GFX_SkillIcon_Pursuer,
	},
#endif

#if (defined(SID_Stride) && COMMON_SKILL_VALID(SID_Stride))
	[SID_Stride] = {
		.desc = MSG_SKILL_Stride,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_QuickLearner) && COMMON_SKILL_VALID(SID_QuickLearner))
	[SID_QuickLearner] = {
		 .name = MSG_SKILL_QuickLearner_NAME,
		 .desc = MSG_SKILL_QuickLearner,
		 .icon = GFX_SkillIcon_QuickLearner,
	},
#endif

#if (defined(SID_GoldDigger) && COMMON_SKILL_VALID(SID_GoldDigger))
	[SID_GoldDigger] = {
		.name = MSG_SKILL_GoldDigger_NAME,
		.desc = MSG_SKILL_GoldDigger,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_EffectSpore) && COMMON_SKILL_VALID(SID_EffectSpore))
	[SID_EffectSpore] = {
		.name = MSG_SKILL_EffectSpore_NAME,
		.desc = MSG_SKILL_EffectSpore,
		.icon = GFX_SkillIcon_EffectSpore,
	},
#endif

#if (defined(SID_DominantArm) && COMMON_SKILL_VALID(SID_DominantArm))
	[SID_DominantArm] = {
		.name = MSG_SKILL_DominantArm_NAME,
		.desc = MSG_SKILL_DominantArm,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_GaleWings) && COMMON_SKILL_VALID(SID_GaleWings))
	[SID_GaleWings] = {
		.desc = MSG_SKILL_GaleWings,
		.icon = GFX_SkillIcon_GaleWings,
	},
#endif

#if (defined(SID_SolidRock) && COMMON_SKILL_VALID(SID_SolidRock))
	[SID_SolidRock] = {
		.desc = MSG_SKILL_SolidRock,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Sturdy) && COMMON_SKILL_VALID(SID_Sturdy))
	[SID_Sturdy] = {
		.desc = MSG_SKILL_Sturdy,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_FranticSwing) && COMMON_SKILL_VALID(SID_FranticSwing))
	[SID_FranticSwing] = {
		.name = MSG_SKILL_FranticSwing_NAME,
		.desc = MSG_SKILL_FranticSwing,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_SocialButterfly) && COMMON_SKILL_VALID(SID_SocialButterfly))
	[SID_SocialButterfly] = {
		.name = MSG_SKILL_SocialButterfly_NAME,
		.desc = MSG_SKILL_SocialButterfly,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_MeleeManiac) && COMMON_SKILL_VALID(SID_MeleeManiac))
	[SID_MeleeManiac] = {
		.name = MSG_SKILL_MeleeManiac_NAME,
		.desc = MSG_SKILL_MeleeManiac,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_CriticalOverload) && COMMON_SKILL_VALID(SID_CriticalOverload))
	[SID_CriticalOverload] = {
		.name = MSG_SKILL_CriticalOverload_NAME,
		.desc = MSG_SKILL_CriticalOverload,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Swarp) && COMMON_SKILL_VALID(SID_Swarp))
	[SID_Swarp] = {
		.desc = MSG_SKILL_Swarp,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Blacksmith) && COMMON_SKILL_VALID(SID_Blacksmith))
	[SID_Blacksmith] = {
		.desc = MSG_SKILL_Blacksmith,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Deal) && COMMON_SKILL_VALID(SID_Deal))
	[SID_Deal] = {
		.desc = MSG_SKILL_Deal,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Bargain) && COMMON_SKILL_VALID(SID_Bargain))
	[SID_Bargain] = {
		.desc = MSG_SKILL_Bargain,
		.icon = GFX_SkillIcon_Bargain,
	},
#endif

#if (defined(SID_WyvernFlight) && COMMON_SKILL_VALID(SID_WyvernFlight))
	[SID_WyvernFlight] = {
		.name = MSG_SKILL_WyvernFlight_NAME,
		.desc = MSG_SKILL_WyvernFlight,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_YngviAscendant) && COMMON_SKILL_VALID(SID_YngviAscendant))
	[SID_YngviAscendant] = {
		.name = MSG_SKILL_YngviAscendant_NAME,
		.desc = MSG_SKILL_YngviAscendant,
		.icon = GFX_SkillIcon_YngviAscendant,
	},
#endif

#if (defined(SID_ChillPow) && COMMON_SKILL_VALID(SID_ChillPow))
	[SID_ChillPow] = {
		.desc = MSG_SKILL_ChillPow,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_ChillMag) && COMMON_SKILL_VALID(SID_ChillMag))
	[SID_ChillMag] = {
		.desc = MSG_SKILL_ChillMag,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_ChillSkl) && COMMON_SKILL_VALID(SID_ChillSkl))
	[SID_ChillSkl] = {
		.desc = MSG_SKILL_ChillSkl,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_ChillSpd) && COMMON_SKILL_VALID(SID_ChillSpd))
	[SID_ChillSpd] = {
		.desc = MSG_SKILL_ChillSpd,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_ChillLck) && COMMON_SKILL_VALID(SID_ChillLck))
	[SID_ChillLck] = {
		.desc = MSG_SKILL_ChillLck,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_ChillDef) && COMMON_SKILL_VALID(SID_ChillDef))
	[SID_ChillDef] = {
		.desc = MSG_SKILL_ChillDef,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_ChillRes) && COMMON_SKILL_VALID(SID_ChillRes))
	[SID_ChillRes] = {
		.desc = MSG_SKILL_ChillRes,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_DragonWrath) && COMMON_SKILL_VALID(SID_DragonWrath))
	[SID_DragonWrath] = {
		.desc = MSG_SKILL_DragonWrath,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_BrashAssault) && COMMON_SKILL_VALID(SID_BrashAssault))
	[SID_BrashAssault] = {
		.desc = MSG_SKILL_BrashAssault,
		.icon = GFX_SkillIcon_BrashAssault,
	},
#endif

#if (defined(SID_Scendscale) && COMMON_SKILL_VALID(SID_Scendscale))
	[SID_Scendscale] = {
		.desc = MSG_SKILL_Scendscale,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_LawsOfSacae) && COMMON_SKILL_VALID(SID_LawsOfSacae))
	[SID_LawsOfSacae] = {
		.name = MSG_SKILL_LawsOfSacae_NAME,
		.desc = MSG_SKILL_LawsOfSacae,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_BonusDoubler) && COMMON_SKILL_VALID(SID_BonusDoubler))
	[SID_BonusDoubler] = {
		.desc = MSG_SKILL_BonusDoubler,
		.icon = GFX_SkillIcon_BonusDoubler,
	},
#endif

#if (defined(SID_ShieldPulse) && COMMON_SKILL_VALID(SID_ShieldPulse))
	[SID_ShieldPulse] = {
		.desc = MSG_SKILL_ShieldPulse,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_BestDefense) && COMMON_SKILL_VALID(SID_BestDefense))
	[SID_BestDefense] = {
		.desc = MSG_SKILL_BestDefense,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_DoOrDie) && COMMON_SKILL_VALID(SID_DoOrDie))
	[SID_DoOrDie] = {
		.desc = MSG_SKILL_DoOrDie,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Glorifier) && COMMON_SKILL_VALID(SID_Glorifier))
	[SID_Glorifier] = {
		.desc = MSG_SKILL_Glorifier,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Leader) && COMMON_SKILL_VALID(SID_Leader))
	[SID_Leader] = {
		.desc = MSG_SKILL_Leader,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_OverKill) && COMMON_SKILL_VALID(SID_OverKill))
	[SID_OverKill] = {
		.desc = MSG_SKILL_OverKill,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Momentum) && COMMON_SKILL_VALID(SID_Momentum))
	[SID_Momentum] = {
		.desc = MSG_SKILL_OverKill,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_FatalTen) && COMMON_SKILL_VALID(SID_FatalTen))
	[SID_FatalTen] = {
		.desc = MSG_SKILL_FatalTen,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_LionFlame) && COMMON_SKILL_VALID(SID_LionFlame))
	[SID_LionFlame] = {
		.name = MSG_SKILL_LionFlame_NAME,
		.desc = MSG_SKILL_LionFlame,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_LimitBreak) && COMMON_SKILL_VALID(SID_LimitBreak))
	[SID_LimitBreak] = {
		.desc = MSG_SKILL_LimitBreak,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_ImmovableObject) && COMMON_SKILL_VALID(SID_ImmovableObject))
	[SID_ImmovableObject] = {
		.name = MSG_SKILL_ImmovableObject_NAME,
		.desc = MSG_SKILL_ImmovableObject,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_UnstoppableForce) && COMMON_SKILL_VALID(SID_UnstoppableForce))
	[SID_UnstoppableForce] = {
		.name = MSG_SKILL_UnstoppableForce_NAME,
		.desc = MSG_SKILL_UnstoppableForce,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_WeaponHeal) && COMMON_SKILL_VALID(SID_WeaponHeal))
	[SID_WeaponHeal] = {
		.desc = MSG_SKILL_WeaponHeal,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_BloodSurge) && COMMON_SKILL_VALID(SID_BloodSurge))
	[SID_BloodSurge] = {
		.desc = MSG_SKILL_BloodSurge,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_BloodReaver) && COMMON_SKILL_VALID(SID_BloodReaver))
	[SID_BloodReaver] = {
		.desc = MSG_SKILL_BloodReaver,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Latona) && COMMON_SKILL_VALID(SID_Latona))
	[SID_Latona] = {
		.desc = MSG_SKILL_Latona,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Saint) && COMMON_SKILL_VALID(SID_Saint))
	[SID_Saint] = {
		.desc = MSG_SKILL_Saint,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Witch) && COMMON_SKILL_VALID(SID_Witch))
	[SID_Witch] = {
		.desc = MSG_SKILL_Witch,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_COMBAT_BloodTribute) && COMMON_SKILL_VALID(SID_COMBAT_BloodTribute))
	[SID_COMBAT_BloodTribute] = {
		.name = MSG_SKILL_COMBAT_BloodTribute_NAME,
		.desc = MSG_SKILL_COMBAT_BloodTribute,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_COMBAT_CrimsonStrike) && COMMON_SKILL_VALID(SID_COMBAT_CrimsonStrike))
	[SID_COMBAT_CrimsonStrike] = {
		.name = MSG_SKILL_COMBAT_CrimsonStrike_NAME,
		.desc = MSG_SKILL_COMBAT_CrimsonStrike,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_COMBAT_VitalReckoning) && COMMON_SKILL_VALID(SID_COMBAT_VitalReckoning))
	[SID_COMBAT_VitalReckoning] = {
		.name = MSG_SKILL_COMBAT_VitalReckoning_NAME,
		.desc = MSG_SKILL_COMBAT_VitalReckoning,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Perch) && COMMON_SKILL_VALID(SID_Perch))
	[SID_Perch] = {
		.desc = MSG_SKILL_Perch,
		.icon = GFX_SkillIcon_Perch,
	},
#endif

#if (defined(SID_WingedShield) && COMMON_SKILL_VALID(SID_WingedShield))
	[SID_WingedShield] = {
		.desc = MSG_SKILL_WingedShield,
		.icon = GFX_SkillIcon_WingedShield,
	},
#endif

#if (defined(SID_BeastShield) && COMMON_SKILL_VALID(SID_BeastShield))
	[SID_BeastShield] = {
		.desc = MSG_SKILL_BeastShield,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Dishonorbreaker) && COMMON_SKILL_VALID(SID_Dishonorbreaker))
	[SID_Dishonorbreaker] = {
		.desc = MSG_SKILL_Dishonorbreaker,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Stubborn) && COMMON_SKILL_VALID(SID_Stubborn))
	[SID_Stubborn] = {
		.desc = MSG_SKILL_Stubborn,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_Shakedown) && COMMON_SKILL_VALID(SID_Shakedown))
	[SID_Shakedown] = {
		.desc = MSG_SKILL_Shakedown,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_EndlessVitality) && COMMON_SKILL_VALID(SID_EndlessVitality))
	[SID_EndlessVitality] = {
		.name = MSG_SKILL_EndlessVitality_NAME,
		.desc = MSG_SKILL_EndlessVitality_DESC,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_SeaWays) && COMMON_SKILL_VALID(SID_SeaWays))
	[SID_SeaWays] = {
		.desc = MSG_SKILL_SeaWays,
		.icon = GFX_SkillIcon_SeaWays,
	},
#endif

#if (defined(SID_FreeSpirit) && COMMON_SKILL_VALID(SID_FreeSpirit))
	[SID_FreeSpirit] = {
		.desc = MSG_SKILL_FreeSpirit,
		.icon = GFX_SkillIcon_WIP,
	},
#endif

#if (defined(SID_DivineReprieve) && COMMON_SKILL_VALID(SID_DivineReprieve))
	[SID_DivineReprieve] = {
		.name = MSG_MenuSkill_DivineReprieve_NAME,
		.desc = MSG_MenuSkill_DivineReprieve_DESC,
		.icon = GFX_SkillIcon_WIP,
	},
#endif
};
