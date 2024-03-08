#pragma once

#include "global.h"
#include "bmunit.h"

#define NEW_UNIT_STATUS_MAX_DURATION 4

enum UNIT_STATUS_IDENTIFIER {
    /* Expand here */
    NEW_UNIT_STATUS_PIERCE_ARMOR = 14,
    NEW_UNIT_STATUS_PIERCE_MAGIC,
    NEW_UNIT_STATUS_HEAVY_GRAVITY,
    NEW_UNIT_STATUS_WEAKEN,
    NEW_UNIT_STATUS_AVOID,
    NEW_UNIT_STATUS_AVOID_PLUS,

    NEW_UNIT_STATUS_MAX = 64
};

enum DEBUFF_INFO_TYPE {
    STATUS_INFO_TYPE_DEFAULT = 0,
    STATUS_INFO_TYPE_DEBUFF = 1,
    STATUS_INFO_TYPE_BUFF = 2,
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
