#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct SkillExtraInfo gSkillExtraInfo[MAX_SKILL_NUM + 1] = {
    [SID_HpBonus] = {{ 2 }},
    [SID_StrBonus] = {{ 2 }},
    [SID_MagBonus] = {{ 2 }},
    [SID_SklBonus] = {{ 2 }},
    [SID_SpdBonus] = {{ 2 }},
    [SID_LckBonus] = {{ 2 }},
    [SID_DefBonus] = {{ 2 }},
    [SID_ResBonus] = {{ 2 }},
    [SID_MovBonus] = {{ 2 }},

    [SID_DefiantStr] = {{ 7 }},
    [SID_DefiantMag] = {{ 7 }},
    [SID_DefiantSkl] = {{ 7 }},
    [SID_DefiantSpd] = {{ 7 }},
    [SID_DefiantLck] = {{ 7 }},
    [SID_DefiantDef] = {{ 7 }},
    [SID_DefiantRes] = {{ 7 }},
    [SID_DefiantCrit] = {{ 50 }},
    [SID_DefiantAvoid] = {{ 30 }},

    [SID_PushStrength] = {{ 5 }},
    [SID_PushMagic] = {{ 5 }},
    [SID_PushSkill] = {{ 5 }},
    [SID_PushSpeed] = {{ 5 }},
    [SID_PushLuck] = {{ 5 }},
    [SID_PushDefense] = {{ 5 }},
    [SID_PushResistance] = {{ 5 }},
    [SID_PushMovement] = {{ 5 }},
    [SID_PushSpectrum] = {{ 5 }},

    [SID_Fury] = {{ 3 }},
    [SID_FuryPlus] = {{ 4 }},

    [SID_LuckySeven] = {{ 7 }},

    [SID_FortressDef] = {{ -3, +5 }},
    [SID_FortressRes] = {{ -3, +5 }},
    [SID_LifeAndDeath] = {{ 5, -5 }},

    [SID_CritSword] = {{ 10 }},
    [SID_CritAxe] = {{ 10 }},
    [SID_CritLance] = {{ 10 }},
    [SID_CritBow] = {{ 10 }},

    [SID_FaireSword] = {{ 5 }},
    [SID_FaireLance] = {{ 5 }},
    [SID_FaireAxe] = {{ 5 }},
    [SID_FaireBow] = {{ 5 }},
    [SID_FaireBMag] = {{ 5 }},

    [SID_Avoid] = {{ 10 }},
    [SID_RuinedBlade] = {{ -5, +5, +5 }},
    [SID_RuinedBladePlus] = {{ 5, 5, 1 }},
    [SID_HeavyBlade] = {{ 15 }},
    [SID_FlashingBlade] = {{ 15 }},
    [SID_HeavyBladePlus] = {{ 5, 25 }},
    [SID_FlashingBladePlus] = {{ 3, 25 }},

    [SID_BattleVeteran] = {{ 1, 5 }},
    [SID_ElbowRoom] = {{ 3 }},
    [SID_ChaosStyle] = {{ 3 }},
    [SID_ArcaneBlade] = {{ 3 }},
    [SID_QuickDraw] = {{ 4 }},
    [SID_KeenFighter] = {{ 50 }},
    [SID_DragonSkin] = {{ 50 }},
    [SID_WaterBoost] = {{ 6, 3 }},
    [SID_EarthBoost] = {{ 6, 3 }},
    [SID_WindBoost] = {{ 6, 3 }},
    [SID_FireBoost] = {{ 6, 3 }},
    [SID_Pursuit] = {{ 2 }},
    [SID_Patience] = {{ 10 }},
    [SID_StrongRiposte] = {{ 3 }},
    [SID_Perfectionist] = {{ 15, 15 }},
    [SID_WindDisciple] = {{ 10, 10 }},
    [SID_Pragmatic] = {{ 3, 1 }},
};
