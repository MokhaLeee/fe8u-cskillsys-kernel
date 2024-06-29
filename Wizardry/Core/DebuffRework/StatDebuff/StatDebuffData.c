#include "common-chax.h"
#include "debuff.h"

const struct DebuffInfo gStatDebuffInfos[UNIT_STAT_DEBUFF_MAX] = {
    [UNIT_STAT_BUFF_RING_ATK] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .atk = +10 }
    },
    [UNIT_STAT_BUFF_RING_DEF] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .def = +10 }
    },
    [UNIT_STAT_BUFF_RING_CRT] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .crit = +10 }
    },
    [UNIT_STAT_BUFF_RING_AVO] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .avo = +10 }
    },

    [UNIT_STAT_DEBUFF_POW] = {
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .pow = -4 },
    },
    [UNIT_STAT_DEBUFF_MAG] = {
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .mag = -4 },
    },
    [UNIT_STAT_DEBUFF_SKL] = {
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .skl = -4 },
    },
    [UNIT_STAT_DEBUFF_SPD] = {
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .spd = -4 },
    },
    [UNIT_STAT_DEBUFF_LCK] = {
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .lck = -4 },
    },
    [UNIT_STAT_DEBUFF_DEF] = {
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .def = -4 },
    },
    [UNIT_STAT_DEBUFF_RES] = {
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .res = -4 },
    },
    [UNIT_STAT_DEBUFF_MOV] = {
        .type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .mov = -4 },
    },

    [UNIT_STAT_BUFF_POW] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .pow = +4 },
    },
    [UNIT_STAT_BUFF_MAG] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .mag = +4 },
    },
    [UNIT_STAT_BUFF_SKL] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .skl = +4 },
    },
    [UNIT_STAT_BUFF_SPD] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .spd = +4 },
    },
    [UNIT_STAT_BUFF_LCK] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .lck = +4 },
    },
    [UNIT_STAT_BUFF_DEF] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .def = +4 },
    },
    [UNIT_STAT_BUFF_RES] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .res = +4 },
    },
    [UNIT_STAT_BUFF_MOV] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .mov = +4 },
    },

    [UNIT_STAT_BUFF_INIT_POW] = {
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .pow = +7 },
    },
    [UNIT_STAT_BUFF_INIT_MAG] = {
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .mag = +7 },
    },
    [UNIT_STAT_BUFF_INIT_SKL] = {
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .skl = +7 },
    },
    [UNIT_STAT_BUFF_INIT_SPD] = {
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .spd = +7 },
    },
    [UNIT_STAT_BUFF_INIT_LCK] = {
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .lck = +7 },
    },
    [UNIT_STAT_BUFF_INIT_DEF] = {
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .def = +7 },
    },
    [UNIT_STAT_BUFF_INIT_RES] = {
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .res = +7 },
    },
    [UNIT_STAT_BUFF_INIT_MOV] = {
        .type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .mov = +7 },
    },

    [UNIT_STAT_BUFF_JOB_HONE] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = {
            .pow = 6,
            .mag = 6,
            .skl = 6,
            .spd = 6,
        },
    },
    [UNIT_STAT_BUFF_JOB_FORTIFY] = {
        .type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = {
            .lck = 6,
            .def = 6,
            .res = 6,
        },
    },

    [UNIT_STAT_DEBUFF_AversaNight] = {
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
