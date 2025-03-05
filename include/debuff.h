#pragma once

#include "common-chax.h"

#define NEW_UNIT_STATUS_MAX_DURATION 4

struct UnitStatusRework {
	u8 index : 6;
	u8 duration: 2;
};

#define UNIT_STATUS(unit) ((struct UnitStatusRework *)((u8 *)(unit) + 0x30))
#define UNIT_STATUS_INDEX(unit) (UNIT_STATUS(unit)->index)
#define UNIT_STATUS_DURATION(unit) (UNIT_STATUS(unit)->duration)

bool UNIT_STONED(struct Unit *unit);

enum UNIT_STATUS_IDENTIFIER {
	/* Expand here */
	NEW_UNIT_STATUS_PIERCE_ARMOR = 14,
	NEW_UNIT_STATUS_PIERCE_MAGIC,
	NEW_UNIT_STATUS_HEAVY_GRAVITY,
	NEW_UNIT_STATUS_WEAKEN,
	NEW_UNIT_STATUS_AVOID,
	NEW_UNIT_STATUS_AVOID_PLUS,
	NEW_UNIT_STATUS_PANIC,

	NEW_UNIT_STATUS_MAX = 64
};

enum DEBUFF_POSITIVE_TYPE {
	/* DebuffInfo::positive_type */
	STATUS_DEBUFF_NONE,
	STATUS_DEBUFF_NEGATIVE,
	STATUS_DEBUFF_POSITIVE,

	STATUS_DEBUFF_NONE_NO_CALC,
};

enum STATUS_DEBUFF_TICK_TYPE {
	/* DebuffInfo::tick_type */
	STATUS_DEBUFF_NO_TICK = 0,
	STATUS_DEBUFF_TICK_ON_ENEMY = 1,
	STATUS_DEBUFF_TICK_ON_ALLY = 2,
};

enum DEBUFF_INFO_EFX_SPEED {
	/* DebuffInfo::efx_config::speed */
	EFX_DEBUFF_LOWLOW,
	EFX_DEBUFF_LOW,
	EFX_DEBUFF_NORMAL,
	EFX_DEBUFF_FAST,
};

struct DebuffInfo {
	const u8 *img;
	void (*on_draw)(struct Unit *unit, int ix, int iy);
	u16 name, desc;

	u8 positive_type;
	u8 tick_type;
	u8 duration;

	u8 _pad_;

	struct {
		u8 speed;
		u8 r, g, b;
	} efx_config;

	struct {
		s8 pow, mag, skl, spd, def, res, lck, mov;
	} unit_status;

	struct {
		s8 atk, def, hit, avo, crit, silencer, dodge;
	} battle_status;

	u8 cannot_move;
};

extern const struct DebuffInfo gDebuffInfos[NEW_UNIT_STATUS_MAX];
extern struct DebuffInfo const *const gpDebuffInfos;

int GetUnitStatusIndex(struct Unit *unit);
int GetUnitStatusDuration(struct Unit *unit);
void SetUnitStatusIndex(struct Unit *unit, int status);
void SetUnitStatusDuration(struct Unit *unit, int duration);
int TryTickUnitStatusDuration(struct Unit *unit);

static inline bool IsDebuff(int status_idx)
{
	return (gpDebuffInfos[status_idx].positive_type == STATUS_DEBUFF_NEGATIVE);
}

void PutUnitDanceRingBuffIcon(struct Unit *unit, int ix, int iy);
void PutUnitPoisonDebuffOAM(struct Unit *unit, int x, int y);
void PutUnitSleepDebuffOAM(struct Unit *unit, int x, int y);
void PutUnitBerserkDebuffOAM(struct Unit *unit, int x, int y);
void PutUnitSilenceDebuffOAM(struct Unit *unit, int x, int y);

void PreBattleCalcDebuffs(struct BattleUnit *attacker, struct BattleUnit *defender);
int PowGetterDebuff(int status, struct Unit *unit);
int MagGetterDebuff(int status, struct Unit *unit);
int SklGetterDebuff(int status, struct Unit *unit);
int SpdGetterDebuff(int status, struct Unit *unit);
int DefGetterDebuff(int status, struct Unit *unit);
int ResGetterDebuff(int status, struct Unit *unit);
int LckGetterDebuff(int status, struct Unit *unit);
int MovGetterDebuff(int status, struct Unit *unit);

/**
 * StatDebuff
 */

enum UNIT_STAT_DEBUFF_IDX {
	/* 0/1 is set as a bitmask to identify postive/negative status */
	UNIT_STAT_POSITIVE_BIT0,
	UNIT_STAT_POSITIVE_BIT1,

	UNIT_STAT_DEBUFF_IDX_START,

	UNIT_STAT_BUFF_RING_ATK = UNIT_STAT_DEBUFF_IDX_START,
	UNIT_STAT_BUFF_RING_DEF,
	UNIT_STAT_BUFF_RING_CRT,
	UNIT_STAT_BUFF_RING_AVO,

	UNIT_STAT_DEBUFF_POW,
	UNIT_STAT_DEBUFF_MAG,
	UNIT_STAT_DEBUFF_SKL,
	UNIT_STAT_DEBUFF_SPD,
	UNIT_STAT_DEBUFF_LCK,
	UNIT_STAT_DEBUFF_DEF,
	UNIT_STAT_DEBUFF_RES,
	UNIT_STAT_DEBUFF_MOV,

	UNIT_STAT_BUFF_POW,
	UNIT_STAT_BUFF_MAG,
	UNIT_STAT_BUFF_SKL,
	UNIT_STAT_BUFF_SPD,
	UNIT_STAT_BUFF_LCK,
	UNIT_STAT_BUFF_DEF,
	UNIT_STAT_BUFF_RES,
	UNIT_STAT_BUFF_MOV,

	UNIT_STAT_BUFF_INIT_POW,
	UNIT_STAT_BUFF_INIT_MAG,
	UNIT_STAT_BUFF_INIT_SKL,
	UNIT_STAT_BUFF_INIT_SPD,
	UNIT_STAT_BUFF_INIT_LCK,
	UNIT_STAT_BUFF_INIT_DEF,
	UNIT_STAT_BUFF_INIT_RES,
	UNIT_STAT_BUFF_INIT_MOV,

	UNIT_STAT_BUFF_OATHROUSE_POW,
	UNIT_STAT_BUFF_OATHROUSE_MAG,
	UNIT_STAT_BUFF_OATHROUSE_SKL,
	UNIT_STAT_BUFF_OATHROUSE_SPD,
	UNIT_STAT_BUFF_OATHROUSE_LCK,
	UNIT_STAT_BUFF_OATHROUSE_DEF,
	UNIT_STAT_BUFF_OATHROUSE_RES,
	UNIT_STAT_BUFF_OATHROUSE_MOV,

	UNIT_STAT_BUFF_JOB_HONE,
	UNIT_STAT_BUFF_JOB_FORTIFY,

	UNIT_STAT_DEBUFF_AversaNight,
	UNIT_STAT_DEBUFF_YuneWhispers,

	UNIT_STAT_BUFF_PLUSMINUS,

	UNIT_STAT_BUFF_ROUSE_POW,
	UNIT_STAT_BUFF_ROUSE_MAG,
	UNIT_STAT_BUFF_ROUSE_SKL,
	UNIT_STAT_BUFF_ROUSE_SPD,
	UNIT_STAT_BUFF_ROUSE_LCK,
	UNIT_STAT_BUFF_ROUSE_DEF,
	UNIT_STAT_BUFF_ROUSE_RES,
	UNIT_STAT_BUFF_ROUSE_MOV,

	UNIT_STAT_BUFF_RALLY_POW,
	UNIT_STAT_BUFF_RALLY_MAG,
	UNIT_STAT_BUFF_RALLY_SKL,
	UNIT_STAT_BUFF_RALLY_SPD,
	UNIT_STAT_BUFF_RALLY_LCK,
	UNIT_STAT_BUFF_RALLY_DEF,
	UNIT_STAT_BUFF_RALLY_RES,
	UNIT_STAT_BUFF_RALLY_MOV,

	UNIT_STAT_BUFF_INDOOR_MARCH_MOV,
	UNIT_STAT_BUFF_NATURE_RUSH_MOV,

	UNIT_STAT_BUFF_KEEPUP,

	UNIT_STAT_BUFF_ARMOR_MARCH,

	UNIT_STAT_BUFF_STRIDE,

	UNIT_STAT_DEBUFF_CHILL_POW,
	UNIT_STAT_DEBUFF_CHILL_MAG,
	UNIT_STAT_DEBUFF_CHILL_SKL,
	UNIT_STAT_DEBUFF_CHILL_SPD,
	UNIT_STAT_DEBUFF_CHILL_LCK,
	UNIT_STAT_DEBUFF_CHILL_DEF,
	UNIT_STAT_DEBUFF_CHILL_RES,

	UNIT_STAT_BUFF_GLORIFIER,
	UNIT_STAT_BUFF_FREESPIRIT,

	UNIT_STAT_DEBUFF_MAX_REAL,
	UNIT_STAT_DEBUFF_MAX = 128, /* DO NOT modify this */
};

extern const struct DebuffInfo gStatDebuffInfos[UNIT_STAT_DEBUFF_MAX];
extern struct DebuffInfo const *const gpStatDebuffInfos;

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
extern struct StatDebuffStatus sStatDebuffStatusBattleUnit[2];
// extern struct StatDebuffStatus *const sStatDebuffStatusPool[0x100];
extern struct StatDebuffStatus *const *const gpStatDebuffStatusPool;

struct StatDebuffStatus *GetUnitStatDebuffStatus(struct Unit *unit);

#define STAT_DEBUFF_MSG_BUF_AMT 7
#define STAT_DEBUFF_MSG_BUF_NEXT(idx) (((idx) - 1) & 3)

struct StatDebuffMsgBuf {
	u32 bitfile[4];
	u32 special_mask;
	s8 uid;
	s16 pow, mag, skl, spd, def, res, lck, mov;
};
extern struct StatDebuffMsgBuf sStatDebuffMsgBuf[STAT_DEBUFF_MSG_BUF_AMT];
extern u8 sStatDebuffMsgBufNext;

enum STAT_BUFF_MSG_BUF_SPECIAL_MASK {
	SP_STAT_CANNOT_MOVE = (1 << 0x00),
};

int IsPositiveStatDebuff(struct Unit *unit);
void MSU_SaveStatDebuff(u8 *dst, const u32 size);
void MSU_LoadStatDebuff(u8 *src, const u32 size);
void SetUnitStatDebuff(struct Unit *unit, enum UNIT_STAT_DEBUFF_IDX debuff);
void ClearUnitStatDebuff(struct Unit *unit, enum UNIT_STAT_DEBUFF_IDX debuff);
bool CheckUnitStatDebuff(struct Unit *unit, enum UNIT_STAT_DEBUFF_IDX debuff);
void TickUnitStatDebuff(struct Unit *unit, enum STATUS_DEBUFF_TICK_TYPE type);
int SimulateStatDebuffPositiveType(struct Unit *unit);

void PreBattleCalcStatDebuffs(struct BattleUnit *attacker, struct BattleUnit *defender);
int PowGetterStatDebuff(int status, struct Unit *unit);
int MagGetterStatDebuff(int status, struct Unit *unit);
int SklGetterStatDebuff(int status, struct Unit *unit);
int SpdGetterStatDebuff(int status, struct Unit *unit);
int DefGetterStatDebuff(int status, struct Unit *unit);
int ResGetterStatDebuff(int status, struct Unit *unit);
int LckGetterStatDebuff(int status, struct Unit *unit);
int MovGetterStatDebuff(int status, struct Unit *unit);

void StatDeuff_OnNewGameInit(void);
void ResetStatDeuffBuf(void);
void StatDeuff_OnLoadUnit(struct Unit *unit);

/* Misc API */
bool UnitHasNegativeStatus(struct Unit *unit);
bool UnitHasPositiveStatus(struct Unit *unit);
void RemoveUnitNegativeStatus(struct Unit *unit);
void RemoveUnitPositiveStatus(struct Unit *unit);
