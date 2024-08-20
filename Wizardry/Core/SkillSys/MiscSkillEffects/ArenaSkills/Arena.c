#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"


LYN_REPLACE_CHECK(ArenaGenerateBaseWeapons);
void ArenaGenerateBaseWeapons(void) {
    u8 arenaWeapons[] = {
        [ITYPE_SWORD] = ITEM_SWORD_IRON,
        [ITYPE_LANCE] = ITEM_LANCE_IRON,
        [ITYPE_AXE] = ITEM_AXE_IRON,
        [ITYPE_BOW] = ITEM_BOW_IRON,
        [ITYPE_STAFF] = ITEM_NONE,
        [ITYPE_ANIMA] = ITEM_ANIMA_FIRE,
        [ITYPE_LIGHT] = ITEM_LIGHT_LIGHTNING,
        [ITYPE_DARK] = ITEM_DARK_FLUX
    };

    gArenaState.playerWeapon = MakeNewItem(arenaWeapons[gArenaState.playerWpnType]);

    gArenaState.opponentWeapon = MakeNewItem(arenaWeapons[gArenaState.opponentWpnType]);

    gArenaState.range = 1;

    if (gArenaState.playerWpnType == ITYPE_BOW) {
        gArenaState.range = 2;
    }

    if (gArenaState.opponentWpnType == ITYPE_BOW) {
        gArenaState.range = 2;
    }

    /*
    ** If the opponent is using a bow, the skill holder's equipped weapon must
    ** allow the opponent to counter in order for the skill to activate
    */
#if (defined(SID_ConcealedWeapon) && COMMON_SKILL_VALID(SID_ConcealedWeapon))
    if (SkillTester(gActiveUnit, SID_ConcealedWeapon))
    {
        if (GetUnitEquippedWeapon(gActiveUnit) != 0)
        {
            if ((GetItemMinRange(gArenaState.opponentWeapon) == GetItemMinRange(GetUnitEquippedWeapon(gActiveUnit))) ||
                (GetItemMaxRange(gArenaState.opponentWeapon) == GetItemMaxRange(GetUnitEquippedWeapon(gActiveUnit))))
            {
                gArenaState.playerWeapon = GetUnitEquippedWeapon(gActiveUnit); 

                if (gArenaState.opponentWpnType != ITYPE_BOW && GetItemType(GetUnitEquippedWeapon(gActiveUnit)) != ITYPE_BOW)
                    gArenaState.range = 1;
            }
        }
    }
#endif

    return;
}