#pragma once

#include "common-chax.h"
#include "debuff.h"

struct StatDebuffStatus {
    union {
        struct {
            u32 is_buff_chk : 2;
            u32 f1 : 29;
            u32 f2, f3, f4;
        } bitfile;

        u32 bitmask[4];
    } st;
};

extern struct StatDebuffStatus sStatDebuffStatusAlly[CONFIG_UNIT_AMT_ALLY];
extern struct StatDebuffStatus sStatDebuffStatusEnemy[CONFIG_UNIT_AMT_ENEMY];
extern struct StatDebuffStatus sStatDebuffStatusNpc[CONFIG_UNIT_AMT_NPC];

extern struct StatDebuffStatus * const sStatDebuffStatusPool[0x100];
