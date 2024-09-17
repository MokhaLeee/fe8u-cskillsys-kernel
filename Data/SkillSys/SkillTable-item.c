#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const u16 gConstSkillTable_Item[0x100][2] = {
    [ITEM_SWORD_SLIM] = {
        SID_RuinedBladePlus,
        SID_TriangleAttack,
    },
    [ITEM_SWORD_RAPIER] = {
        SID_RiskItAll,
        SID_DarkHorse,
    },
    [ITEM_AXE_IRON] = {
        SID_NiceThighs,
    },
    [ITEM_STAFF_HEAL] = {
        SID_LiveToServe,
    },
    [ITEM_ANIMA_ELFIRE] = {
        SID_TriangleAttack,
        SID_Casual,
    },
    [ITEM_LANCE_SLIM] = {
        SID_Swarm,
    },
    [ITEM_LANCE_SILVER] = {
        SID_Swarm,
        SID_Capture,
    }
};
