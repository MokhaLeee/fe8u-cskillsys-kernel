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

/**
 * BattleOrder
 */
enum {
    NOP_ATTACK = 0,
    ACT_ATTACK = 1,
    TAR_ATTACK = 2,
};

enum {
    UNWIND_VANTAGE = 1 << 0,
    UNWIND_DESPERA = 1 << 1,
    UNWIND_DOUBLE_ACT = 1 << 2,
    UNWIND_DOUBLE_TAR = 1 << 3,
};

extern const u8 BattleUnwindConfig[14][4];

struct EfxSkillQueue {
    u8 cur, max;
    u16 skill_pool[14];
};
extern struct EfxSkillQueue sEfxSkillQueue;

void ResetRoundEfxSkills(void);
void EnqueueRoundEfxSkill(u16 sid);
u16 DequeueRoundEfxSkill(void);

/**
 * BattleHit
 */
enum damage_decrease_sheet_idx {
    DMG_DECREASE_IDX_05Perc,
    DMG_DECREASE_IDX_10Perc,
    DMG_DECREASE_IDX_15Perc,
    DMG_DECREASE_IDX_20Perc,
    DMG_DECREASE_IDX_25Perc,
    DMG_DECREASE_IDX_30Perc,
    DMG_DECREASE_IDX_35Perc,
    DMG_DECREASE_IDX_40Perc,
    DMG_DECREASE_IDX_45Perc,
    DMG_DECREASE_IDX_50Perc,
    DMG_DECREASE_IDX_55Perc,
    DMG_DECREASE_IDX_60Perc,
    DMG_DECREASE_IDX_65Perc,
    DMG_DECREASE_IDX_70Perc,
    DMG_DECREASE_IDX_75Perc,
    DMG_DECREASE_IDX_80Perc,
    DMG_DECREASE_IDX_85Perc,
    DMG_DECREASE_IDX_90Perc,
    DMG_DECREASE_IDX_95Perc,

    DMG_DECREASE_IDX_MAX
};

enum damage_decrease_cheat {
    /**
     * (1 - percentage%) = 1 / (100% + decrease%)
     * decrease = 1/(1 - percentage%) - 1
     * 
     * here we calculate on (decrease * 0x100)
     */
    DMG_DECREASE_95Perc = 4864, // 19.00000
    DMG_DECREASE_90Perc = 2304, // 9.00000
    DMG_DECREASE_85Perc = 1451, // 5.66667
    DMG_DECREASE_80Perc = 1024, // 4.00000
    DMG_DECREASE_75Perc = 768,  // 3.00000
    DMG_DECREASE_70Perc = 597,  // 2.33333
    DMG_DECREASE_65Perc = 475,  // 1.85714
    DMG_DECREASE_60Perc = 384,  // 2.50000
    DMG_DECREASE_55Perc = 313,  // 1.22222
    DMG_DECREASE_50Perc = 256,  // 1.00000
    DMG_DECREASE_45Perc = 210,  // 0.81818
    DMG_DECREASE_40Perc = 171,  // 0.66667
    DMG_DECREASE_35Perc = 138,  // 0.53846
    DMG_DECREASE_30Perc = 110,  // 0.42857
    DMG_DECREASE_25Perc = 85,   // 0.33333
    DMG_DECREASE_20Perc = 64,   // 0.25000
    DMG_DECREASE_15Perc = 45,   // 0.17647
    DMG_DECREASE_10Perc = 28,   // 0.11111
    DMG_DECREASE_05Perc = 13,   // 0.05263
};

extern const u16 DmgDecreaseRef[DMG_DECREASE_IDX_MAX];

void PreBattleGenerateHook(void);
