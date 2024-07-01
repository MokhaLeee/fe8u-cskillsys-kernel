#include "common-chax.h"
#include "debuff.h"

const struct DebuffInfo gStatDebuffInfos[UNIT_STAT_DEBUFF_MAX] = {
    [UNIT_STAT_BUFF_RING_ATK] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .atk = +10 }
    },
    [UNIT_STAT_BUFF_RING_DEF] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .def = +10 }
    },
    [UNIT_STAT_BUFF_RING_CRT] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .crit = +10 }
    },
    [UNIT_STAT_BUFF_RING_AVO] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .battle_status = { .avo = +10 }
    },

    [UNIT_STAT_DEBUFF_POW] = {
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .pow = -4 },
    },
    [UNIT_STAT_DEBUFF_MAG] = {
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .mag = -4 },
    },
    [UNIT_STAT_DEBUFF_SKL] = {
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .skl = -4 },
    },
    [UNIT_STAT_DEBUFF_SPD] = {
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .spd = -4 },
    },
    [UNIT_STAT_DEBUFF_LCK] = {
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .lck = -4 },
    },
    [UNIT_STAT_DEBUFF_DEF] = {
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .def = -4 },
    },
    [UNIT_STAT_DEBUFF_RES] = {
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .res = -4 },
    },
    [UNIT_STAT_DEBUFF_MOV] = {
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = { .mov = -4 },
    },

    [UNIT_STAT_BUFF_POW] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .pow = +4 },
    },
    [UNIT_STAT_BUFF_MAG] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .mag = +4 },
    },
    [UNIT_STAT_BUFF_SKL] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .skl = +4 },
    },
    [UNIT_STAT_BUFF_SPD] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .spd = +4 },
    },
    [UNIT_STAT_BUFF_LCK] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .lck = +4 },
    },
    [UNIT_STAT_BUFF_DEF] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .def = +4 },
    },
    [UNIT_STAT_BUFF_RES] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .res = +4 },
    },
    [UNIT_STAT_BUFF_MOV] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = { .mov = +4 },
    },

    [UNIT_STAT_BUFF_INIT_POW] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .pow = +7 },
    },
    [UNIT_STAT_BUFF_INIT_MAG] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .mag = +7 },
    },
    [UNIT_STAT_BUFF_INIT_SKL] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .skl = +7 },
    },
    [UNIT_STAT_BUFF_INIT_SPD] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .spd = +7 },
    },
    [UNIT_STAT_BUFF_INIT_LCK] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .lck = +7 },
    },
    [UNIT_STAT_BUFF_INIT_DEF] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .def = +7 },
    },
    [UNIT_STAT_BUFF_INIT_RES] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .res = +7 },
    },
    [UNIT_STAT_BUFF_INIT_MOV] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .mov = +7 },
    },

    [UNIT_STAT_BUFF_OATH_POW] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .pow = +4 },
    },
    [UNIT_STAT_BUFF_OATH_MAG] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .mag = +4 },
    },
    [UNIT_STAT_BUFF_OATH_SKL] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .skl = +4 },
    },
    [UNIT_STAT_BUFF_OATH_SPD] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .spd = +4 },
    },
    [UNIT_STAT_BUFF_OATH_LCK] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .lck = +4 },
    },
    [UNIT_STAT_BUFF_OATH_DEF] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .def = +4 },
    },
    [UNIT_STAT_BUFF_OATH_RES] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .res = +4 },
    },
    [UNIT_STAT_BUFF_OATH_MOV] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_TYPE_3, /* tick it on manual */
        .unit_status = { .mov = +4 },
    },

    [UNIT_STAT_DEBUFF_YuneWhispers] = {
        .positive_type = STATUS_DEBUFF_POSITIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = {
            .pow = -6,
            .mag = -6,
            .spd = -6,
        },
    },

    [UNIT_STAT_BUFF_JOB_HONE] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = {
            .pow = 6,
            .mag = 6,
            .skl = 6,
            .spd = 6,
        },
    },
    [UNIT_STAT_BUFF_JOB_FORTIFY] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ALLY,
        .unit_status = {
            .lck = 6,
            .def = 6,
            .res = 6,
        },
    },

    [UNIT_STAT_DEBUFF_AversaNight] = {
        .positive_type = STATUS_DEBUFF_NEGATIVE,
        .tick_type = STATUS_DEBUFF_TICK_ON_ENEMY,
        .unit_status = {
            .pow = -3,
            .mag = -3,
            .spd = -3,
            .def = -3,
            .res = -3,
        },
    },
};
