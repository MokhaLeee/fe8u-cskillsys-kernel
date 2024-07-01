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

#define UNIT_STONED(unit) (UNIT_STATUS_INDEX(unit) == UNIT_STATUS_PETRIFY || UNIT_STATUS_INDEX(unit) == UNIT_STATUS_13)

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

enum STATUS_DEBUFF_TICK_TYPE {
    STATUS_DEBUFF_NO_TICK = 0,
    STATUS_DEBUFF_TICK_ON_ENEMY = 1,
    STATUS_DEBUFF_TICK_ON_ALLY = 2,
    STATUS_DEBUFF_TICK_TYPE_3 = 3,
};

enum DEBUFF_INFO_EFX_SPEED {
    EFX_DEBUFF_LOWLOW,
    EFX_DEBUFF_LOW,
    EFX_DEBUFF_NORMAL,
    EFX_DEBUFF_FAST,
};

struct DebuffInfo {
    const u8 * img;
    void (* on_draw)(struct Unit * unit);
    u16 name, desc;

    u8 type;
    u8 duration;

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

    u8 cannot_move : 1;
};

extern const struct DebuffInfo gDebuffInfos[NEW_UNIT_STATUS_MAX];
extern struct DebuffInfo const * const gpDebuffInfos;

int GetUnitStatusIndex(struct Unit * unit);
int GetUnitStatusDuration(struct Unit * unit);
void SetUnitStatusIndex(struct Unit * unit, int status);
void SetUnitStatusDuration(struct Unit * unit, int duration);
int TryTickUnitStatusDuration(struct Unit * unit);

static inline bool IsDebuff(int status_idx)
{
    return (gpDebuffInfos[status_idx].type == STATUS_DEBUFF_TICK_ON_ENEMY);
}

void PutUnitStatusIcon(struct Unit * unit);
void PutUnitDanceRingBuffIcon(struct Unit * unit);

void PreBattleCalcDebuffs(struct BattleUnit * attacker, struct BattleUnit * defender);
int PowGetterDebuff(int status, struct Unit * unit);
int MagGetterDebuff(int status, struct Unit * unit);
int SklGetterDebuff(int status, struct Unit * unit);
int SpdGetterDebuff(int status, struct Unit * unit);
int DefGetterDebuff(int status, struct Unit * unit);
int ResGetterDebuff(int status, struct Unit * unit);
int LckGetterDebuff(int status, struct Unit * unit);
int MovGetterDebuff(int status, struct Unit * unit);

/**
 * StatDebuff
 */

enum UNIT_STAT_DEBUFF_IDX {
    UNIT_STAT_BUFF_RING_ATK,
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

    UNIT_STAT_BUFF_OATH_POW,
    UNIT_STAT_BUFF_OATH_MAG,
    UNIT_STAT_BUFF_OATH_SKL,
    UNIT_STAT_BUFF_OATH_SPD,
    UNIT_STAT_BUFF_OATH_LCK,
    UNIT_STAT_BUFF_OATH_DEF,
    UNIT_STAT_BUFF_OATH_RES,
    UNIT_STAT_BUFF_OATH_MOV,

    UNIT_STAT_BUFF_JOB_HONE,
    UNIT_STAT_BUFF_JOB_FORTIFY,

    UNIT_STAT_DEBUFF_AversaNight,
    UNIT_STAT_DEBUFF_YuneWhispers,

    UNIT_STAT_DEBUFF_MAX_REAL,

    UNIT_STAT_DEBUFF_MAX = 128, /* DO NOT modify this */
};

extern const struct DebuffInfo gStatDebuffInfos[UNIT_STAT_DEBUFF_MAX];
extern struct DebuffInfo const * const gpStatDebuffInfos;

void MSU_SaveStatDebuff(u8 * dst, const u32 size);
void MSU_LoadStatDebuff(u8 * src, const u32 size);
void SetUnitStatDebuff(struct Unit * unit, enum UNIT_STAT_DEBUFF_IDX debuff);
void ClearUnitStatDebuff(struct Unit * unit, enum UNIT_STAT_DEBUFF_IDX debuff);
bool CheckUnitStatDebuff(struct Unit * unit, enum UNIT_STAT_DEBUFF_IDX debuff);
void TickUnitStatDebuff(struct Unit * unit, enum STATUS_DEBUFF_TICK_TYPE type);

void PreBattleCalcStatDebuffs(struct BattleUnit * attacker, struct BattleUnit * defender);
int PowGetterStatDebuff(int status, struct Unit * unit);
int MagGetterStatDebuff(int status, struct Unit * unit);
int SklGetterStatDebuff(int status, struct Unit * unit);
int SpdGetterStatDebuff(int status, struct Unit * unit);
int DefGetterStatDebuff(int status, struct Unit * unit);
int ResGetterStatDebuff(int status, struct Unit * unit);
int LckGetterStatDebuff(int status, struct Unit * unit);
int MovGetterStatDebuff(int status, struct Unit * unit);

void StatDeuff_OnNewGameInit(void);
void StatDeuff_OnNewGameSave(void);
void StatDeuff_OnLoadUnit(struct Unit * unit);
