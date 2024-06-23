#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u8 gConstSkillTable_Item[0x100][2] = {
    [ITEM_SWORD_SLIM] = {
        SKILL_INDEX_REAL(SID_RuinedBladePlus),
    },
};
