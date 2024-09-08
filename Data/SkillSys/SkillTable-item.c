#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Item[0x100][2] = {
    [ITEM_SWORD_SLIM] = {
        SID_RuinedBladePlus,
    },
    [ITEM_SWORD_RAPIER] = {
        SID_Roll12,
    },
    [ITEM_AXE_IRON] = {
        SID_FullMetalBody,
    },
    [ITEM_STAFF_HEAL] = {
        SID_LiveToServe,
    }
};
