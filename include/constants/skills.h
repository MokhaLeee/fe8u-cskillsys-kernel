#ifndef CONFIG_SKILLS_INSTALLED
#define CONFIG_SKILLS_INSTALLED

#include "skill-system.h"

enum SkillIndex {
    SID_DEFAULT = 0x0,

    /* Unit status skills */
    // SID_HpBonus,
    SID_StrBonus,
    SID_MagBonus,
    SID_SklBonus,
    SID_SpdBonus,
    SID_LckBonus,
    SID_DefBonus,
    SID_ResBonus,
    SID_MovBonus,

    SID_DefiantStr,
    SID_DefiantMag,
    SID_DefiantSkl,
    SID_DefiantSpd,
    SID_DefiantLck,
    SID_DefiantDef,
    SID_DefiantRes,
    SID_DefiantCrit,
    SID_DefiantAvoid,

    SID_Fury,
    SID_FuryPlus,
    SID_FortressDef,
    SID_FortressRes,
    SID_LifeAndDeath,
    SID_LuckySeven,

    /* Battle effective */
    SID_Nullify,
    SID_Slayer,
    SID_Skybreaker,
    SID_Resourceful,

    /* Battle status skills */
    SID_Lethality,
    SID_Crit,
    SID_WatchfulEye,
    SID_CritSword,
    SID_CritAxe,
    SID_CritLance,
    SID_CritBow,
    SID_FaireSword,
    SID_FaireLance,
    SID_FaireAxe,
    SID_FaireBow,
    SID_FaireBMag,
    SID_Avoid,
    SID_AvoidSword,
    SID_RuinedBlade,
    SID_RuinedBladePlus,
    SID_InfinityEdge,
    SID_HeavyBlade,
    SID_FlashingBlade,
    SID_HeavyBladePlus,
    SID_FlashingBladePlus,
    SID_LunaAttack,
    SID_SorceryBlade,
    SID_Fortune,
    SID_BlueFlame,

    /* Weapon triangle */
    SID_SwordBreaker,
    SID_AxeBreaker,
    SID_LanceBreaker,
    SID_BowBreaker,
    SID_BMagBreaker,

    /* Battle order */
    SID_Vantage,
    SID_Desperation,
    SID_QuickRiposte,
    SID_WaryFighter,
    SID_DoubleLion,
    SID_Adept, /* +1 hit if full HP */

    /* Battle activation skills */
    SID_Astra, /* +4 hits with half damage (speed * 2%) */
    SID_Aegis,
    SID_Pavise,
    SID_Bane,
    SID_Aether,
    SID_Corona,
    SID_Luna,
    SID_Flare,
    SID_Sol,
    SID_DragonFang,
    SID_Foresight,
    SID_Inori,

    /* Blow skills */
    SID_BlowDarting,
    SID_BlowDeath,
    SID_BlowArmored,
    SID_BlowFiendish,
    SID_BlowWarding,
    SID_BlowDuelist,
    SID_BlowUncanny,

    /* Stance skills */
    SID_StanceBracing,
    SID_StanceDarting,
    SID_StanceFierce,
    SID_StanceKestrel,
    SID_StanceMirror,
    SID_StanceReady,
    SID_StanceSteady,
    SID_StanceSturdy,
    SID_StanceSwift,
    SID_StanceWarding,

    /* Weapon range */
    SID_RangeBonusBMag1,
    SID_RangeBonusBMag2,
    SID_RangeBonusBow1,
    SID_RangeBonusBow2,

    /* Post action */
    SID_Canto,
    SID_CantoPlus,
    SID_AlertStance,
    SID_AlertStancePlus,
    SID_Galeforce,
    SID_SavageBlow,
    SID_BreathOfLife,

    /* Pre-phase */
    SID_Renewal,

    /* Range debuff */
    SID_Anathema,
    SID_Bond,
    SID_Charm,
    SID_Intimidate,
    SID_Tantivy,
    SID_Focus,
    SID_Hex,
    SID_Infiltrator,
    SID_Inspiration,
    SID_DivinelyInspiring,
    SID_VoiceOfPeace,

    /* AI skills */
    SID_Shade,      /* Less likely to be attacked */
    SID_ShadePlus,  /* Avoid to be a target */
    SID_Provoke,    /* More likely to be attacked */

    /* Menu skills */
    SID_Dance,
    SID_LockTouch,
    SID_Summon,
    SID_Supply,
    SID_Steal,

    /* Lvup skills */
    SID_Blossom,
    SID_Paragon,
    SID_VoidCurse,
    SID_Aptitude,

    /* Legendary skill */
    SID_LEGEND_InoriAtk,   /* Inori, then atk + 10, crit+100% */
    SID_LEGEND_InoriAvo,   /* Inori, then speed + 10, Avoid + 100% */
    SID_LEGEND_InoriDef,   /* Inori, then recover full HP, def/res + 10 */
    SID_LEGEND_4,
    SID_LEGEND_5,

    /* Combat-art skill */
    SID_COMBAT_Grounder,
    SID_COMBAT_BaneOfMonsters,
    SID_COMBAT_Hexblade,
    SID_COMBAT_FoudroyantStrike,
    SID_COMBAT_TempestLance,
    SID_COMBAT_SwiftStrikes,
    SID_COMBAT_FrozenLance,
    SID_COMBAT_Vengeance,
    SID_COMBAT_HelmSplitter,
    SID_COMBAT_LightningAxe,
    SID_COMBAT_WarMasterStrike,
    SID_COMBAT_CurvedShot,
    SID_COMBAT_Deadeye,
    SID_COMBAT_Encloser,
    SID_COMBAT_EncloserPlus,
    SID_COMBAT_MonsterBlast,
    SID_COMBAT_SchismShot,
    SID_COMBAT_BreakShot,
    SID_COMBAT_BreakShotPlus,
    SID_COMBAT_WaningShot,
    SID_COMBAT_WaningShotPlus,
    SID_COMBAT_HuntersVolley,

    SID_MAX
};

enum AbandonedSkills {
    SID_HpBonus = MAX_SKILL_NUM,
};

#endif
