#pragma once

#include "common-chax.h"

#define EMS_CHUNK_INVALID_OFFSET ((u16)-1)

struct EmsChunk {
	/* 00 */ u16 _identifier_;
	/* 02 */ u16 size;

	/* 04 */ void (*save)(u8 *dst, const u32 size);
	/* 08 */ void (*load)(u8 *src, const u32 size);
	/* 0C */ int chunk_idx;
};

extern const struct EmsChunk gEmsSavChunks[];
extern const struct EmsChunk gEmsSusChunks[];
extern const u16 gEmsOffsets[SAVE_ID_MAX];

extern const u16 gEmsSizes[2];
#define EMS_SIZE_SAV (gEmsSizes[0])
#define EMS_SIZE_SUS (gEmsSizes[1])

/* Chunk index */
#define EMS_CHUNK_CHAPTERSTATE   1
#define EMS_CHUNK_SAVEUNITS      2
#define EMS_CHUNK_CONVOYITEMS    3
#define EMS_CHUNK_BWLENTRIES     4
#define EMS_CHUNK_WINDATA        5
#define EMS_CHUNK_EIDS_PERMANENT 6
#define EMS_CHUNK_BONUSCLAIMDATA 7
#define EMS_CHUNK_WMDATA         8
#define EMS_CHUNK_DUNGEON        9

const struct EmsChunk *GetEmsChunkByIndex_Sav(int idx);
const struct EmsChunk *GetEmsChunkByIndex_Sus(int idx);

u32 CalcChunkOffset_Sav(const struct EmsChunk *chunk);
u32 CalcChunkOffset_Sus(const struct EmsChunk *chunk);

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

#if !CHAX
			 u32 status   : 4;
			 u32 duration : 4;
#else
			 u32 status   : 6;
			 u32 duration : 2;
#endif

	/* 10 */ union {
				struct {
					u8 skills[7];
					u8 cur_hp;
					u8 ballista;

					/**
					 * Enemy cannot:
					 * 1. rescue unit
					 * 2. use torch
					 * 3. use barrier staff
					 * 4. gain support
					 */
					u8 rescue;
					u8 torch   : 4;
					u8 barrier : 4;
					s8 support_gain;
				} ally;

				struct {
					u8 skills[3];
					u8 cur_hp;
					u8 ballista;

					u8 ai1;
					u8 ai2;
					u8 ai1_cur;
					u8 ai2_cur;
					u8 ai_flag;
					u16 ai_config;
				} ai;
			} pad;

	/* 1C */ u8 ranks[0x8];

	/* 24 */ u32 state;
	/* 28 */ u16 items[UNIT_ITEM_COUNT];

	/* 32 */ u8 _u3A;
	/* 33 */ u8 _u3B;
} BITPACKED;

#define SIZE_OF_SAV_UNIT_PACK 0x2C
#define SIZE_OF_SUS_UNIT_PACK 0x34

extern void (*const gpMsa_Saver)(u8 *dst, const u32 size);
extern void (*const gpMsa_Loader)(u8 *src, const u32 size);
extern void (*const gpMsu_Saver)(u8 *dst, const u32 size);
extern void (*const gpMsu_Loader)(u8 *src, const u32 size);

void MSA_ExternalSaver(u8 *dst, const u32 size);
void MSA_ExternalLoader(u8 *src, const u32 size);
void MSU_ExternalSaver(u8 *dst, const u32 size);
void MSU_ExternalLoader(u8 *src, const u32 size);

typedef void (*new_save_hook)(void);
// extern const new_save_hook gNewSaveHooks[];
extern new_save_hook const *const gpNewSaveHooks;
