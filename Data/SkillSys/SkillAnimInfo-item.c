#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/efx-skills.h"

const struct SkillAnimInfo gSkillAnimInfos_Item[0x100] = {

#if (defined(SID_RuinedBladePlus) && ITEM_SKILL_VALID(SID_RuinedBladePlus))
    [SKILL_INDEX_REAL(SID_RuinedBladePlus)] = {
        .aid = EFX_SKILL_MOKHA,
        .priority = EFX_PRIORITY_LOW,
        .sfx = 0x3D1,
    },
#endif

};
