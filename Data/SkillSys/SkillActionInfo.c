#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const SkillActionFunc_t gSkillActionFuncTable[MAX_SKILL_NUM + 1] = {
    [SID_HealingFocus] = Action_HealingFocus,
    [SID_Teleportation] = Action_Teleportation,
    [SID_LightRune] = Action_LightRune,
    [SID_Mine] = Action_Mine,
    [SID_RallyDefense] = Action_Rally,
    [SID_RallyLuck] = Action_Rally,
    [SID_RallyMagic] = Action_Rally,
    [SID_RallyMovement] = Action_Rally,
    [SID_RallyResistance] = Action_Rally,
    [SID_RallySkill] = Action_Rally,
    [SID_RallySpeed] = Action_Rally,
    [SID_RallyStrength] = Action_Rally,
    [SID_RallySpectrum] = Action_Rally,
    [SID_GoddessDance] = Action_GoddessDance,
    [SID_Stride] = Action_Stride,
    [SID_Swarp] = Action_Swarp,
    [SID_Blacksmith] = Action_Blacksmith,
    [SID_ArdentSacrifice] = Action_ArdentSacrifice,
};
