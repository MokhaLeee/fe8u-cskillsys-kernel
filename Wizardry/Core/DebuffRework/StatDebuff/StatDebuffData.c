#include "common-chax.h"
#include "debuff.h"

const struct DebuffInfo gStatDebuffInfos[UNIT_STAT_DEBUFF_MAX] = {
    [UNIT_STAT_BUFF_RING_ATK] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .battle_status = { .atk = +10 }
    },
    [UNIT_STAT_BUFF_RING_DEF] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .battle_status = { .def = +10 }
    },
    [UNIT_STAT_BUFF_RING_CRT] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .battle_status = { .crit = +10 }
    },
    [UNIT_STAT_BUFF_RING_AVO] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .battle_status = { .avo = +10 }
    },

    [UNIT_STAT_DEBUFF_POW] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .pow = -4 },
    },
    [UNIT_STAT_DEBUFF_MAG] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .mag = -4 },
    },
    [UNIT_STAT_DEBUFF_SKL] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .skl = -4 },
    },
    [UNIT_STAT_DEBUFF_SPD] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .spd = -4 },
    },
    [UNIT_STAT_DEBUFF_LCK] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .lck = -4 },
    },
    [UNIT_STAT_DEBUFF_DEF] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .def = -4 },
    },
    [UNIT_STAT_DEBUFF_RES] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .res = -4 },
    },
    [UNIT_STAT_DEBUFF_MOV] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .mov = -4 },
    },

    [UNIT_STAT_BUFF_POW] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .unit_status = { .pow = +4 },
    },
    [UNIT_STAT_BUFF_MAG] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .unit_status = { .mag = +4 },
    },
    [UNIT_STAT_BUFF_SKL] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .unit_status = { .skl = +4 },
    },
    [UNIT_STAT_BUFF_SPD] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .unit_status = { .spd = +4 },
    },
    [UNIT_STAT_BUFF_LCK] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .unit_status = { .lck = +4 },
    },
    [UNIT_STAT_BUFF_DEF] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .unit_status = { .def = +4 },
    },
    [UNIT_STAT_BUFF_RES] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .unit_status = { .res = +4 },
    },
    [UNIT_STAT_BUFF_MOV] = {
        .type = STATUS_INFO_TYPE_BUFF,
        .unit_status = { .mov = +4 },
    },

    [UNIT_STAT_DEBUFF_AversaNight] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = {
            .pow = -3,
            .mag = -3,
            .spd = -3,
            .def = -3,
            .res = -3,
        },
    },
};
