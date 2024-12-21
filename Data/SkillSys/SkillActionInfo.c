#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const SkillActionFunc_t gSkillActionFuncTable[MAX_SKILL_NUM + 1] = {
#if (defined(SID_HealingFocus) && COMMON_SKILL_VALID(SID_HealingFocus))
    [SID_HealingFocus] = Action_HealingFocus,
#endif

#if (defined(SID_Teleportation) && COMMON_SKILL_VALID(SID_Teleportation))
    [SID_Teleportation] = Action_Teleportation,
#endif

#if (defined(SID_LightRune) && COMMON_SKILL_VALID(SID_LightRune))
    [SID_LightRune] = Action_LightRune,
#endif

#if (defined(SID_Mine) && COMMON_SKILL_VALID(SID_Mine))
    [SID_Mine] = Action_Mine,
#endif

#if (defined(SID_RallyDefense) && COMMON_SKILL_VALID(SID_RallyDefense))
    [SID_RallyDefense] = Action_Rally,
#endif

#if (defined(SID_RallyLuck) && COMMON_SKILL_VALID(SID_RallyLuck))
    [SID_RallyLuck] = Action_Rally,
#endif

#if (defined(SID_RallyMagic) && COMMON_SKILL_VALID(SID_RallyMagic))
    [SID_RallyMagic] = Action_Rally,
#endif

#if (defined(SID_RallyMovement) && COMMON_SKILL_VALID(SID_RallyMovement))
    [SID_RallyMovement] = Action_Rally,
#endif

#if (defined(SID_RallyResistance) && COMMON_SKILL_VALID(SID_RallyResistance))
    [SID_RallyResistance] = Action_Rally,
#endif

#if (defined(SID_RallySkill) && COMMON_SKILL_VALID(SID_RallySkill))
    [SID_RallySkill] = Action_Rally,
#endif

#if (defined(SID_RallySpeed) && COMMON_SKILL_VALID(SID_RallySpeed))
    [SID_RallySpeed] = Action_Rally,
#endif

#if (defined(SID_RallyStrength) && COMMON_SKILL_VALID(SID_RallyStrength))
    [SID_RallyStrength] = Action_Rally,
#endif

#if (defined(SID_RallySpectrum) && COMMON_SKILL_VALID(SID_RallySpectrum))
    [SID_RallySpectrum] = Action_Rally,
#endif

#if (defined(SID_GoddessDance) && COMMON_SKILL_VALID(SID_GoddessDance))
    [SID_GoddessDance] = Action_GoddessDance,
#endif

#if (defined(SID_Stride) && COMMON_SKILL_VALID(SID_Stride))
    [SID_Stride] = Action_Stride,
#endif

#if (defined(SID_Swarp) && COMMON_SKILL_VALID(SID_Swarp))
    [SID_Swarp] = Action_Swarp,
#endif

#if (defined(SID_Blacksmith) && COMMON_SKILL_VALID(SID_Blacksmith))
    [SID_Blacksmith] = Action_Blacksmith,
#endif

#if (defined(SID_Swap) && COMMON_SKILL_VALID(SID_Swap))
    [SID_Swap] = Action_Swap,
#endif

#if (defined(SID_Transform) && COMMON_SKILL_VALID(SID_Transform))
    [SID_Transform] = Action_Transform,
#endif

#if (defined(SID_DrawBack) && COMMON_SKILL_VALID(SID_DrawBack))
    [SID_DrawBack] = Action_DrawBack,
#endif

#if (defined(SID_Pivot) && COMMON_SKILL_VALID(SID_Pivot))
    [SID_Pivot] = Action_Pivot,
#endif

#if (defined(SID_Reposition) && COMMON_SKILL_VALID(SID_Reposition))
    [SID_Reposition] = Action_Reposition,
#endif

#if (defined(SID_Shove) && COMMON_SKILL_VALID(SID_Shove))
    [SID_Shove] = Action_Shove,
#endif

#if (defined(SID_Smite) && COMMON_SKILL_VALID(SID_Smite))
    [SID_Smite] = Action_Smite,
#endif

#if (defined(SID_ArdentSacrifice) && COMMON_SKILL_VALID(SID_ArdentSacrifice))
    [SID_ArdentSacrifice] = Action_ArdentSacrifice,
#endif

#if (defined(SID_ReciprocalAid) && COMMON_SKILL_VALID(SID_ReciprocalAid))
    [SID_ReciprocalAid] = Action_ReciprocalAid,
#endif

#if (defined(SID_Sacrifice) && COMMON_SKILL_VALID(SID_Sacrifice))
    [SID_Sacrifice] = Action_Sacrifice,
#endif

#if (defined(SID_Capture) && COMMON_SKILL_VALID(SID_Capture))
    [SID_Capture] = Action_Capture,
#endif

#if (defined(SID_Doppleganger) && COMMON_SKILL_VALID(SID_Doppleganger))
    [SID_Doppleganger] = Action_Doppleganger,
#endif

#if (defined(SID_Dismount) && COMMON_SKILL_VALID(SID_Dismount))
    [SID_Dismount] = Action_Dismount,
#endif

#if (defined(SID_Entrepreneur) && COMMON_SKILL_VALID(SID_Entrepreneur))
    [SID_Entrepreneur] = Action_Entrepreneur,
#endif

#if (defined(SID_Arise) && COMMON_SKILL_VALID(SID_Arise))
    [SID_Arise] = Action_Arise,
#endif
};
