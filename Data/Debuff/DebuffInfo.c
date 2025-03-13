#include "common-chax.h"
#include "debuff.h"
#include "constants/texts.h"
#include "constants/gfx.h"

const struct DebuffInfo gDebuffInfos[NEW_UNIT_STATUS_MAX] = {
    [UNIT_STATUS_NONE] = {
        .name = 0x536,
        .desc = 0x552,
    },
    [UNIT_STATUS_POISON] = {
        .name = 0x514,
        .desc = 0x553,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 3,
        .img = GFX_DebuffMinibox_Posion,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x10, 0, 0x10},
    },
    [UNIT_STATUS_SLEEP] = {
        .name = 0x515,
        .desc = 0x554,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 3,
        .img = GFX_DebuffMinibox_Sleep,
        .efx_config = {EFX_DEBUFF_NORMAL, 0, 0, 0x10},
    },
    [UNIT_STATUS_SILENCED] = {
        .name = 0x516,
        .desc = 0x556,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 3,
        .img = GFX_DebuffMinibox_Silence,
    },
    [UNIT_STATUS_BERSERK] = {
        .name = 0x517,
        .desc = 0x555,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 3,
        .img = GFX_DebuffMinibox_Berserk,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x10, 0, 0},
    },
    [UNIT_STATUS_ATTACK] = {
        .name = 0x51B,
        .desc = 0x558,
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .duration = 1,
        .battle_status.atk = 10,
        .img = GFX_DebuffMinibox_Attack,
        .on_draw = PutUnitDanceRingBuffIcon,
    },
    [UNIT_STATUS_DEFENSE] = {
        .name = 0x51C,
        .desc = 0x559,
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .duration = 1,
        .battle_status.def = 5,
        .img = GFX_DebuffMinibox_Defense,
        .on_draw = PutUnitDanceRingBuffIcon,
    },
    [UNIT_STATUS_CRIT] = {
        .name = 0x51D,
        .desc = 0x55A,
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .duration = 3,
        .battle_status.crit = 15,
        .img = GFX_DebuffMinibox_Crit,
        .on_draw = PutUnitDanceRingBuffIcon,
    },
    [UNIT_STATUS_AVOID] = {
        .name = 0x51E,
        .desc = 0x55B,
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .duration = 3,
        .battle_status.avo = 15,
        .img = GFX_DebuffMinibox_Aviod,
        .on_draw = PutUnitDanceRingBuffIcon,
    },
    [UNIT_STATUS_SICK] = {
        .name = 0x518,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 3,
    },
    [UNIT_STATUS_RECOVER] = {
        .name = 0x519,
        .duration = 3,
    },
    [UNIT_STATUS_PETRIFY] = {
        .name = 0x51A,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 3,
        .img = GFX_DebuffMinibox_Petrify,
        .efx_config = {EFX_DEBUFF_LOWLOW, 0x10, 0x10, 0x10},
    },
    [UNIT_STATUS_12] = {
        .name = 0,
        .desc = 0,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 3,
    },
    [UNIT_STATUS_13] = {
        .name = 0,
        .desc = 0,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 3,
        .efx_config = {EFX_DEBUFF_LOWLOW, 0x10, 0x10, 0x10},
    },
    [NEW_UNIT_STATUS_PIERCE_ARMOR] = {
        .name = MSG_DEBUFF_PIERCE_ARMOR_NAME,
        .desc = MSG_DEBUFF_PIERCE_ARMOR_DESC,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 1,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x8, 0x8, 0x0},
        .unit_status.def = -5,
        .img = GFX_DebuffMinibox_PierceArmor,
    },
    [NEW_UNIT_STATUS_PIERCE_MAGIC] = {
        .name = MSG_DEBUFF_PIERCE_MAGIC_NAME,
        .desc = MSG_DEBUFF_PIERCE_MAGIC_DESC,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 1,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x0, 0x8, 0x8},
        .unit_status.res = -5,
        .img = GFX_DebuffMinibox_PierceMagic,
    },
    [NEW_UNIT_STATUS_HEAVY_GRAVITY] = {
        .name = MSG_DEBUFF_HEAVY_GRAVITY_NAME,
        .desc = MSG_DEBUFF_HEAVY_GRAVITY_DESC,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 1,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x0, 0x0, 0x8},
        .unit_status.mov = -127,
        .img = GFX_DebuffMinibox_Gravity,
        .cannot_move = true,
    },
    [NEW_UNIT_STATUS_WEAKEN] = {
        .name = MSG_DEBUFF_WEAKEN_NAME,
        .desc = MSG_DEBUFF_WEAKEN_DESC,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 1,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x0, 0x0, 0x8},
        .unit_status.pow = -5,
        .unit_status.mag = -5,
        .img = GFX_DebuffMinibox_Weaken,
    },
    [NEW_UNIT_STATUS_AVOID] = {
        .name = MSG_DEBUFF_AVOID_NAME,
        .desc = MSG_DEBUFF_AVOID_DESC,
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .duration = 1,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x8, 0x8, 0x8},
        .battle_status.avo = 15,
        .img = GFX_DebuffMinibox_Aviod,
    },
    [NEW_UNIT_STATUS_AVOID_PLUS] = {
        .name = MSG_DEBUFF_AVOID_PLUS_NAME,
        .desc = MSG_DEBUFF_AVOID_PLUS_DESC,
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .duration = 1,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x10, 0x10, 0x10},
        .battle_status.avo = 30,
        .img = GFX_DebuffMinibox_AvoidPlus,
    },
    [NEW_UNIT_STATUS_PANIC] = {
        .name = MSG_DEBUFF_PANIC_NAME,
        .desc = MSG_DEBUFF_PANIC_DESC,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 1,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x10, 0, 0},
        .img = GFX_DebuffMinibox_Panic,
    },
    [NEW_UNIT_STATUS_BREAK] = {
        .name = MSG_DEBUFF_BREAK_NAME,
        .desc = MSG_DEBUFF_BREAK_DESC,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 1,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x10, 0, 0},
        .img = GFX_DebuffMinibox_Panic,
    },
    [NEW_UNIT_STATUS_TOXIC_POISON] = {
        .name = MSG_DEBUFF_TOXIC_NAME,
        .desc = MSG_DEBUFF_TOXIC_DESC,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .duration = 3,
        .img = GFX_DebuffMinibox_Posion,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x10, 0, 0x10},
    },
    [NEW_UNIT_STATUS_DECOY] = {
        .name = MSG_DEBUFF_DECOY_NAME,
        .desc = MSG_DEBUFF_DECOY_DESC,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .duration = 1,
        .img = GFX_DebuffMinibox_Panic,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x10, 0, 0x10},
    },
    [NEW_UNIT_STATUS_DAMNED] = {
        .name = MSG_DEBUFF_DAMNED_NAME,
        .desc = MSG_DEBUFF_DAMNED_DESC,
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .duration = 3,
        .img = GFX_DebuffMinibox_Panic,
        .efx_config = {EFX_DEBUFF_NORMAL, 0x10, 0, 0x10},
    },
};
