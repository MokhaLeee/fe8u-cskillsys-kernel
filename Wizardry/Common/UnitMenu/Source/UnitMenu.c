#include "global.h"
#include "uimenu.h"
#include "bmmenu.h"
#include "fontgrp.h"

#include "common-chax.h"
#include "skill-system.h"
#include "combat-art.h"
#include "constants/texts.h"

#define MenuItemsEnd {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

const struct MenuItemDef gUnitActionMenuItemsRework[] = {
    {"　制圧", 0x67A, 0x6CC, 0, 0x4E, UnitActionMenu_CanSeize, 0, UnitActionMenu_Seize, 0, 0, 0}, // Seize
    {"　攻撃", 0x67B, 0x6C0, 0, 0x4F, AttackCommandUsability, 0, UnitActionMenu_Attack, 0, DisplayUnitStandingAttackRange, HideMoveRangeGraphicsWrapper}, // Attack >
    {"　攻撃", 0x67B, 0x6C0, 0, 0x50, AttackBallistaCommandUsability, 0, UnitActionMenu_Attack, 0, DisplayUnitStandingAttackRange, HideMoveRangeGraphicsWrapper}, // Attack w/Ballista >

    /* Combat Art */
    {
        "　戰技",
        MSG_COMBATART_UM_NAME,
        MSG_COMBATART_UM_DESC,
        TEXT_COLOR_SYSTEM_WHITE,
        0x6C,
        CombatArtActionCommandUsability,
        NULL,
        CombatArtActionCommandEffect,
        NULL,
        CombatArtActionCommandHover,
        CombatArtActionCommandUnhover
    },

    {"　杖", 0x67C, 0x6C1, 0, 0x51, StaffCommandUsability, 0, StaffCommandEffect, 0, StaffCommandRange, HideMoveRangeGraphicsWrapper2}, // Staff
    {"　乗る", 0x691, 0x6D6, 4, 0x52, RideCommandUsability, 0, RideCommandEffect, 0, 0, 0}, // Ride (Ballista) >
    {"　降りる", 0x692, 0x6D7, 4, 0x53, ExitCommandUsability, 0, ExitCommandEffect, 0, 0, 0}, // Exit (Ballista) >
    {"　奏でる", 0x67D, 0x6C3, 0, 0x54, PlayCommandUsability, 0, PlayCommandEffect, 0, 0, 0}, // Play >
    {"　踊る", 0x67E, 0x6C2, 0, 0x55, DanceCommandUsabilityRework /* DanceCommandUsability */, 0, PlayCommandEffect, 0, 0, 0}, // Dance
    {"　盗む", 0x67F, 0x6C4, 0, 0x56, StealCommandUsabilityRework /* StealCommandUsability */, 0, StealCommandEffect, 0, 0, 0}, // Steal
    {"　召喚", 0x693, 0x6DD, 0, 0x57, SummonCommandUsabilityRework /* SummonCommandUsability */, 0, SummonCommandEffect, 0, 0, 0}, // Summon (Shoukanshi) >
    {"　呼魔", 0x693, 0x6DD, 0, 0x58, YobimaCommandUsability, 0, YobimaCommandEffect, 0, 0, 0}, // Summon (Deamon King) >
    {"　かぎ開", 0x694, 0x6DE, 0, 0x59, PickCommandUsabilityRework /* PickCommandUsability */, 0, PickCommandEffect, 0, 0, 0}, // Pick >
    {"　話す", 0x680, 0x6C9, 0, 0x5A, TalkCommandUsability, 0, TalkCommandEffect, 0, 0, 0}, // Talk >
    {"　支援", 0x681, 0x6CA, 0, 0x5B, SupportCommandUsability, 0, SupportCommandEffect, 0, 0, 0}, // Support
    {"　訪問", 0x682, 0x6CB, 0, 0x5C, VisitCommandUsability, 0, VisitCommandEffect, 0, 0, 0}, // Visit
    {"　宝箱", 0x683, 0x6CE, 0, 0x5D, ChestCommandUsability, 0, ChestCommandEffect, 0, 0, 0}, // Chest
    {"　扉", 0x684, 0x6CD, 0, 0x5E, DoorCommandUsability, 0, DoorCommandEffect, 0, 0, 0}, // Door >
    {"　武器屋", 0x685, 0x6CF, 0, 0x5F, ArmoryCommandUsability, 0, ArmoryCommandEffect, 0, 0, 0}, //Armory > 
    {"　道具屋", 0x686, 0x6D0, 0, 0x60, VendorCommandUsability, 0, VendorCommandEffect, 0, 0, 0}, //Vendor > 
    {"　秘密店", 0x687, 0x6D1, 0, 0x61, SecretShopCommandUsability, 0, SecretShopCommandEffect, 0, 0, 0}, //SecretShop > 
    {"　闘技場", 0x688, 0x6D2, 0, 0x62, ArenaCommandUsability, 0, ArenaCommandEffect, 0, 0, 0}, // Arena
    {"　救出", 0x689, 0x6C5, 0, 0x63, RescueUsability, 0, RescueEffect, 0, 0, 0}, // Rescue > 
    {"　降ろす", 0x68A, 0x6C6, 0, 0x64, DropUsability, 0, DropEffect, 0, 0, 0}, // Drop > 
    {"　引受け", 0x68B, 0x6C8, 4, 0x65, TakeUsability, 0, TakeEffect, 0, 0, 0}, // Take > 
    {"　引渡し", 0x68C, 0x6C7, 4, 0x66, GiveUsability, 0, GiveEffect, 0, 0, 0}, // Give > 
    {"　持ち物", 0x68D, 0x6D3, 0, 0x67, ItemCommandUsability, 0, ItemCommandEffect, 0, 0, 0}, // Item > 
    {"　交換", 0x68E, 0x6D4, 4, 0x68, ItemSubMenu_IsTradeAvailable, 0, TradeCommandEffect, 0, 0, 0}, // Trade > 
    {"　輸送隊", 0x68F, 0x6D5, 4, 0x69, SupplyUsabilityRework /* SupplyUsability */, 0, SupplyCommandEffect, 0, 0, 0}, // Supply > 
    {"　待機", 0x695, 0x6BF, 0, 0x6B, MenuAlwaysEnabled, 0, EffectWait, 0, 0, 0}, // Wait > 
    MenuItemsEnd
};
