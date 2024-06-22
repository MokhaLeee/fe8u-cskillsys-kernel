#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u8 gConstSkillTable_Item[0x100][2] = {
    [ITEM_SWORD_SLIM] = {
#if (defined(SID_RuinedBladePlus) && ITEM_SKILL_VALID(SID_RuinedBladePlus))
        SKILL_INDEX_REAL(SID_RuinedBladePlus),
#endif
    },
};
