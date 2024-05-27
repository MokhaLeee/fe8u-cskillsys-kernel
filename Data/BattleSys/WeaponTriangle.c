#include "common-chax.h"
#include "battle-system.h"
#include "constants/skills.h"

const struct WeaponTriangleConf gWeaponTriangleConfs[] = {
#if defined(SID_AxeBreaker) && (SID_AxeBreaker < MAX_SKILL_NUM)
    {ITYPE_SWORD, ITYPE_AXE,   SID_AxeBreaker,   0, 0, 0, 0, 20, 20, 0, 0, 0},
#endif

#if defined(SID_LanceBreaker) && (SID_LanceBreaker < MAX_SKILL_NUM)
    {ITYPE_AXE,   ITYPE_LANCE, SID_LanceBreaker, 0, 0, 0, 0, 20, 20, 0, 0, 0},
#endif

#if defined(SID_SwordBreaker) && (SID_SwordBreaker < MAX_SKILL_NUM)
    {ITYPE_LANCE, ITYPE_SWORD, SID_SwordBreaker, 0, 0, 0, 0, 20, 20, 0, 0, 0},
#endif

#if defined(SID_BowBreaker) && (SID_BowBreaker < MAX_SKILL_NUM)
    {ITYPE_ANIMA, ITYPE_BOW,   SID_BowBreaker,   0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_LIGHT, ITYPE_BOW,   SID_BowBreaker,   0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_DARK,  ITYPE_BOW,   SID_BowBreaker,   0, 0, 0, 0, 20, 20, 0, 0, 0},
#endif

#if defined(SID_BMagBreaker) && (SID_BMagBreaker < MAX_SKILL_NUM)
    {ITYPE_BOW,   ITYPE_ANIMA, SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_BOW,   ITYPE_LIGHT, SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_BOW,   ITYPE_DARK,  SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_SWORD, ITYPE_ANIMA, SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_SWORD, ITYPE_LIGHT, SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_SWORD, ITYPE_DARK,  SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_AXE,   ITYPE_ANIMA, SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_AXE,   ITYPE_LIGHT, SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_AXE,   ITYPE_DARK,  SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_LANCE, ITYPE_ANIMA, SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_LANCE, ITYPE_LIGHT, SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
    {ITYPE_LANCE, ITYPE_DARK,  SID_BMagBreaker,  0, 0, 0, 0, 20, 20, 0, 0, 0},
#endif

    {0}
};

const struct WeaponTriangleItemConf gWeaponTriangleItemConf[0x100] = {
    [ITEM_SWORD_LANCEREAVER] = {
        .valid = true,
        .wtype = ITYPE_LANCE,
        .battle_status.atk = 3,
        .battle_status.avo = 15,
    },
    [ITEM_LANCE_AXEREAVER] = {
        .valid = true,
        .wtype = ITYPE_AXE,
        .battle_status.atk = 3,
        .battle_status.def = 3,
    },
    [ITEM_AXE_SWORDREAVER] = {
        .valid = true,
        .wtype = ITYPE_SWORD,
        .battle_status.atk = 3,
        .battle_status.hit = 10,
    },
    [ITEM_AXE_SWORDSLAYER] = {
        .valid = true,
        .wtype = ITYPE_SWORD,
        .battle_status.atk = 3,
        .battle_status.hit = 15,
    }
};
