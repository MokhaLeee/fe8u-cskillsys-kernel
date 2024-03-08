#pragma once

#include "global.h"
#include "bmsave.h"
#include "bmitem.h"
#include "sram-layout.h"

#define EMS_CHUNK_INVALID_OFFSET ((u16)-1)

struct EmsChunk {
    /* 00 */ u16 offset;
    /* 02 */ u16 size;

    /* 04 */ void (* save)(u8 * dst, const u32 size);
    /* 08 */ void (* load)(u8 * src, const u32 size);
};

extern const struct EmsChunk gEmsSavChunks[];
extern const struct EmsChunk gEmsSusChunks[];
extern const u16 gEmsOffsets[SAVE_ID_MAX];

extern const u16 gEmsSizes[2];
#define EMS_SIZE_SAV (gEmsSizes[0])
#define EMS_SIZE_SUS (gEmsSizes[1])

struct EmsPackedSavUnit {
    /* 00 */ u8 jid;
    /* 01 */ u8 pid;
    /* 02 */ u8 max_hp;
    /* 03 */ u8 pow, mag, skl, spd, lck, def, res, mov, con;
    /* 0C */ u32 level : 5;
             u32 exp   : 7;
             u32 xPos  : 6;
             u32 yPos  : 6;
    /* 0F */ u8 ranks[0x8];
    /* 17 */ u8 skills[7];
    /* 1E */ u16 items[UNIT_ITEM_COUNT];
    /* 28 */ u32 state;
    /* 2C */
} BITPACKED;

struct EmsPackedSusUnit {
    /* 00 */ u8 jid;
    /* 01 */ u8 pid;
    /* 02 */ u8 max_hp;
    /* 03 */ u8 pow, mag, skl, spd, lck, def, res, mov, con;
    /* 0C */ u32 level : 5;
             u32 exp   : 7;
             u32 xPos  : 6;
             u32 yPos  : 6;
    /* 0F */ u8 ranks[0x8];

    /* 17 */ u8 cur_hp;
    /* 18 */ u8 rescue;
    /* 19 */ u8 ballista;
    /* 1A */ u8 status   : 4;
             u8 duration : 4;
    /* 1B */ u8 torch    : 4;
             u8 barrier  : 4;

    /* 1C */ union {
                struct {
                    u8 skills[7];
                    s8 support_gain;
                } ally;

                struct {
                    u8 skill;
                    u8 ai1;
                    u8 ai2;
                    u8 ai1_cur;
                    u8 ai2_cur;
                    u8 ai_flag;
                    u16 ai_config;
                } ai;
            } pad;

    /* 24 */ u32 state;
    /* 28 */ u16 items[UNIT_ITEM_COUNT];

    /* 32 */ u8 _u3A;
    /* 33 */ u8 _u3B;
} BITPACKED;

#define SIZE_OF_SUS_UNIT_PACK 0x34
