#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const SkillActionFunc_t gSkillActionFuncTable[MAX_SKILL_NUM + 1] = {
    [SID_HealingFocus] = HealingFocus_Action,
};
