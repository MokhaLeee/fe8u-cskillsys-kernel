#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const SkillActionFunc_t gSkillActionFuncTable[MAX_SKILL_NUM + 1] = {
    [SID_HealingFocus] = Action_HealingFocus,
    [SID_Teleportation] = Action_Teleportation,
    [SID_LightRune] = Action_LightRune,
    [SID_RallyDefense] = Action_RallyDefense,
    [SID_RallyLuck] = Action_RallyLuck,
    [SID_RallyMagic] = Action_RallyMagic,
    [SID_RallyMovement] = Action_RallyMovement,
    [SID_RallyResistance] = Action_RallyResistance,
    [SID_RallySkill] = Action_RallySkill,
    [SID_RallySpeed] = Action_RallySpeed,
    [SID_RallyStrength] = Action_RallyStrength,
    [SID_RallySpectrum] = Action_RallySpectrum,
};
