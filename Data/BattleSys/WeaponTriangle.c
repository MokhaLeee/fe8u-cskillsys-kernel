#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "constants/skills.h"

const struct WeaponTriangleConf gWeaponTriangleConfs[] = {
#if defined(SID_AxeBreaker) && (SID_AxeBreaker < MAX_SKILL_NUM)
    {
        .wtype_a = ITYPE_SWORD,
        .wtype_b = ITYPE_AXE,
        .sid = SID_AxeBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20,
    },
#endif

#if defined(SID_LanceBreaker) && (SID_LanceBreaker < MAX_SKILL_NUM)
    {
        .wtype_a = ITYPE_AXE,
        .wtype_b = ITYPE_LANCE,
        .sid = SID_LanceBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
#endif

#if defined(SID_SwordBreaker) && (SID_SwordBreaker < MAX_SKILL_NUM)
    {
        .wtype_a = ITYPE_LANCE,
        .wtype_b = ITYPE_SWORD,
        .sid = SID_SwordBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
#endif

#if defined(SID_BowBreaker) && (SID_BowBreaker < MAX_SKILL_NUM)
    {
        .wtype_a = ITYPE_ANIMA,
        .wtype_b = ITYPE_BOW,
        .sid = SID_BowBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_LIGHT,
        .wtype_b = ITYPE_BOW,
        .sid = SID_BowBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_DARK,
        .wtype_b = ITYPE_BOW,
        .sid = SID_BowBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
#endif

#if defined(SID_BMagBreaker) && (SID_BMagBreaker < MAX_SKILL_NUM)
    {
        .wtype_a = ITYPE_BOW,
        .wtype_b = ITYPE_ANIMA,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_BOW,
        .wtype_b = ITYPE_LIGHT,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_BOW,
        .wtype_b = ITYPE_DARK,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_SWORD,
        .wtype_b = ITYPE_ANIMA,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_SWORD,
        .wtype_b = ITYPE_LIGHT,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_SWORD,
        .wtype_b = ITYPE_DARK,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_AXE,
        .wtype_b = ITYPE_ANIMA,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_AXE,
        .wtype_b = ITYPE_LIGHT,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_AXE,
        .wtype_b = ITYPE_DARK,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_LANCE,
        .wtype_b = ITYPE_ANIMA,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_LANCE,
        .wtype_b = ITYPE_LIGHT,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
    {
        .wtype_a = ITYPE_LANCE,
        .wtype_b = ITYPE_DARK,
        .sid = SID_BMagBreaker,
        .is_buff = true,
        .bonus_hit = 20,
        .bonus_avoid = 20
    },
#endif

    {0}
};

const struct WeaponTriangleItemConf gWeaponTriangleItemConf[0x100] = {
    [ITEM_SWORD_LANCEREAVER] = {
        .valid = true,
        .wtype = ITYPE_LANCE,
        .is_buff = true,
        .battle_status.atk = 3,
        .battle_status.avo = 15,
    },
    [ITEM_LANCE_AXEREAVER] = {
        .valid = true,
        .wtype = ITYPE_AXE,
        .is_buff = true,
        .battle_status.atk = 3,
        .battle_status.def = 3,
    },
    [ITEM_AXE_SWORDREAVER] = {
        .valid = true,
        .wtype = ITYPE_SWORD,
        .is_buff = true,
        .battle_status.atk = 3,
        .battle_status.hit = 10,
    },
    [ITEM_AXE_SWORDSLAYER] = {
        .valid = true,
        .wtype = ITYPE_SWORD,
        .is_buff = true,
        .battle_status.atk = 3,
        .battle_status.hit = 15,
    }
};
