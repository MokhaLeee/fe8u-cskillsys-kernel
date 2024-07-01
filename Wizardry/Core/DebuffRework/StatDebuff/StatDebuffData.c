#include "common-chax.h"
#include "debuff.h"

const struct DebuffInfo gStatDebuffInfos[UNIT_STAT_DEBUFF_MAX] = {
    [UNIT_STAT_BUFF_RING_ATK] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .atk = +10 }
    },
    [UNIT_STAT_BUFF_RING_DEF] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .def = +10 }
    },
    [UNIT_STAT_BUFF_RING_CRT] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .crit = +10 }
    },
    [UNIT_STAT_BUFF_RING_AVO] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .avo = +10 }
    },

    [UNIT_STAT_DEBUFF_POW] = {
        .is_debuff = true,
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .pow = -4 },
    },
    [UNIT_STAT_DEBUFF_MAG] = {
        .is_debuff = true,
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .mag = -4 },
    },
    [UNIT_STAT_DEBUFF_SKL] = {
        .is_debuff = true,
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .skl = -4 },
    },
    [UNIT_STAT_DEBUFF_SPD] = {
        .is_debuff = true,
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .spd = -4 },
    },
    [UNIT_STAT_DEBUFF_LCK] = {
        .is_debuff = true,
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .lck = -4 },
    },
    [UNIT_STAT_DEBUFF_DEF] = {
        .is_debuff = true,
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .def = -4 },
    },
    [UNIT_STAT_DEBUFF_RES] = {
        .is_debuff = true,
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .res = -4 },
    },
    [UNIT_STAT_DEBUFF_MOV] = {
        .is_debuff = true,
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .mov = -4 },
    },

    [UNIT_STAT_BUFF_POW] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .pow = +4 },
    },
    [UNIT_STAT_BUFF_MAG] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .mag = +4 },
    },
    [UNIT_STAT_BUFF_SKL] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .skl = +4 },
    },
    [UNIT_STAT_BUFF_SPD] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .spd = +4 },
    },
    [UNIT_STAT_BUFF_LCK] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .lck = +4 },
    },
    [UNIT_STAT_BUFF_DEF] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .def = +4 },
    },
    [UNIT_STAT_BUFF_RES] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .res = +4 },
    },
    [UNIT_STAT_BUFF_MOV] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .mov = +4 },
    },

    [UNIT_STAT_BUFF_INIT_POW] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .pow = +7 },
    },
    [UNIT_STAT_BUFF_INIT_MAG] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .mag = +7 },
    },
    [UNIT_STAT_BUFF_INIT_SKL] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .skl = +7 },
    },
    [UNIT_STAT_BUFF_INIT_SPD] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .spd = +7 },
    },
    [UNIT_STAT_BUFF_INIT_LCK] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .lck = +7 },
    },
    [UNIT_STAT_BUFF_INIT_DEF] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .def = +7 },
    },
    [UNIT_STAT_BUFF_INIT_RES] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .res = +7 },
    },
    [UNIT_STAT_BUFF_INIT_MOV] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .mov = +7 },
    },

    [UNIT_STAT_BUFF_OATH_POW] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .pow = +4 },
    },
    [UNIT_STAT_BUFF_OATH_MAG] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .mag = +4 },
    },
    [UNIT_STAT_BUFF_OATH_SKL] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .skl = +4 },
    },
    [UNIT_STAT_BUFF_OATH_SPD] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .spd = +4 },
    },
    [UNIT_STAT_BUFF_OATH_LCK] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .lck = +4 },
    },
    [UNIT_STAT_BUFF_OATH_DEF] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .def = +4 },
    },
    [UNIT_STAT_BUFF_OATH_RES] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .res = +4 },
    },
    [UNIT_STAT_BUFF_OATH_MOV] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .mov = +4 },
    },

    [UNIT_STAT_DEBUFF_YuneWhispers] = {
        .is_debuff = true,
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = {
            .pow = -6,
            .mag = -6,
            .spd = -6,
        },
    },

    [UNIT_STAT_BUFF_JOB_HONE] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = {
            .pow = 6,
            .mag = 6,
            .skl = 6,
            .spd = 6,
        },
    },
    [UNIT_STAT_BUFF_JOB_FORTIFY] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = {
            .lck = 6,
            .def = 6,
            .res = 6,
        },
    },

    [UNIT_STAT_DEBUFF_AversaNight] = {
        .is_debuff = false,
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = {
            .pow = -3,
            .mag = -3,
            .spd = -3,
            .def = -3,
            .res = -3,
        },
    },
};
