#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Weapon[0x100][2] = {
    [ITEM_SWORD_IRON] = {
        SID_COMBAT_Hexblade,
    },
    [ITEM_SWORD_SLIM] = {
        SID_RuinedBladePlus,
    },

    [ITEM_BLADE_SILVER] = {
        SID_COMBAT_BaneOfMonsters,
    },

    [ITEM_SWORD_RAPIER] = {
        SID_COMBAT_Grounder,
    },
};
