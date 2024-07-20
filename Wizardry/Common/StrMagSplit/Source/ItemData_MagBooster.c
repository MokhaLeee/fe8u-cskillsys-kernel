#include "common-chax.h"

extern const struct ItemStatBonuses ItemBonus_MagBooster;

const struct ItemData ItemData_MagBooster = {
    .nameTextId = 0x3d5,
    .descTextId = 0x484,
    .useDescTextId = 0x4c2,
    .number = CONFIG_ITEM_INDEX_MAG_BOOSTER,
    .weaponType = ITYPE_ITEM,
    .pStatBonuses = &ItemBonus_MagBooster,
    .maxUses = 1,
    .encodedRange = 0x11,
    .costPerUse = 8000,
    .iconId = CONFIG_ICON_INDEX_MAG_BOOSTER,
    .useEffectId = 17,
};
