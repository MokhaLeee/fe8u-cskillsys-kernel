#include "global.h"
#include "constants/items.h"

#include "common-chax.h"
#include "combat-art.h"
#include "constants/combat-arts.h"

const u8 gCombatArtWeaponTable[0x100] = {
    [ITEM_SWORD_SLIM] = CID_Hexblade,
    [ITEM_LANCE_SLIM] = CID_FrozenLance,
};

FEB_IDENTIFIER(gCombatArtWeaponTable);
