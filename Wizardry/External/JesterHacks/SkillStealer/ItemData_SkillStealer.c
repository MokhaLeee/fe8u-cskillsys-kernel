#include "common-chax.h"
#include "constants/texts.h"

extern const struct ItemStatBonuses ItemBonus_SkillStealer;

const struct ItemData ItemData_SkillStealer = {
    .nameTextId = MSG_ITEM_SKILLSTEALER_NAME,
    .descTextId = MSG_ITEM_SKILLSTEALER_DESC,
    .useDescTextId = MSG_ITEM_SKILLSTEALER_USEDESC,
    .number = CONFIG_ITEM_INDEX_SKILL_STEALER,
    .weaponType = ITYPE_ITEM,
    .pStatBonuses = 0,
    .maxUses = 1,
    .encodedRange = 0x11,
    .costPerUse = 8000,
    .iconId = CONFIG_ICON_INDEX_SKILL_STEALER,
    .useEffectId = 17,
};
