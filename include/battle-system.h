#pragma once


#include "debug-kit.h"

/* WTA bonus */
struct WeaponTriangleConf {
    s8 wtype_a;
    s8 wtype_b;

    u8 _pad_[3];

    bool is_buff;
    u16 sid;


    s8 bonus_atk;
    s8 bonus_def;
    s8 bonus_speed;
    s8 bonus_hit;
    s8 bonus_avoid;
    s8 bonus_crit;
    s8 bonus_dodge;
    s8 bonus_silencer;
};

extern struct WeaponTriangleConf const * const gpWeaponTriangleConfs;

struct WeaponTriangleItemConf {
    s8 valid, wtype;
    struct {
        s8 atk, def, hit, avo, crit, silencer;
    } battle_status;

    bool is_buff;
    u8 _pad_[3];
};

extern const struct WeaponTriangleItemConf gWeaponTriangleItemConf[0x100];
extern struct WeaponTriangleItemConf const * const gpWeaponTriangleItemConf;

/* Battle hit expansion */
#define NEW_BATTLE_HIT_MAX 0x20 /* This should align to gAnimRoundData */
extern struct BattleHit gBattleHitArrayRe[NEW_BATTLE_HIT_MAX];

bool CheckBattleHitOverflow(void);
bool CheckCanTwiceAttackOrder(struct BattleUnit * actor, struct BattleUnit * target);
int CalcBattleRealDamage(struct BattleUnit * attacker, struct BattleUnit * defender);

static inline int GetBattleHitRound(struct BattleHit * hit)
{
    return hit - gBattleHitArrayRe;
}

extern struct BattleGlobalFlags {
    u32 hitted : 1;
    u32 round_cnt : 5;      // align to NEW_BATTLE_HIT_MAX
    u32 round_cnt_hit : 5;  // align to NEW_BATTLE_HIT_MAX
    u32 round_cnt_avo : 5;  // align to NEW_BATTLE_HIT_MAX
    u32 enimy_defeated : 1;
    u32 skill_activated_double_lion : 1;
    u32 skill_activated_astra : 1;
    u32 skill_activated_galeforce : 1;
} gBattleActorGlobalFlag, gBattleTargetGlobalFlag;

static inline struct BattleGlobalFlags * GetBattleGlobalFlags(struct BattleUnit * bu)
{
    return (bu == &gBattleActor)
         ? &gBattleActorGlobalFlag
         : &gBattleTargetGlobalFlag;
}

void ClearBattleGlobalFlags(void);
void RegisterHitCnt(struct BattleUnit * bu, bool miss);

extern struct {
    u32 order_vantage : 1;
    u32 order_desperation : 1;
    u32 order_quick_riposte : 1;
    u32 order_dobule_lion : 1;
    u32 skill_activated_sure_shoot : 1;
    u32 skill_activated_dead_eye : 1;
} gBattleTemporaryFlag;

/* Battle skill act */
bool CheckBattleSkillActivate(struct BattleUnit * actor, struct BattleUnit * target, int sid, int rate);

static inline int GetItemFormSlot(struct Unit * unit, int slot)
{
    switch (slot) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return unit->items[slot];

    case BU_ISLOT_5:
        return gBmSt.um_tmp_item;

    case BU_ISLOT_ARENA_PLAYER:
        return gArenaState.playerWeapon;

    case BU_ISLOT_ARENA_OPPONENT:
        return gArenaState.opponentWeapon;

    case BU_ISLOT_BALLISTA:
        return GetBallistaItemAt(unit->xPos, unit->yPos);

    case -1:
    default:
        return 0;
    }
}

extern struct Vec2 gBattleTargetPositionBackup;

void PreBattleGenerateHook(void);
