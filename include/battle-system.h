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
    u32 allies_in_range3x3 : 5;
    u32 enemies_in_range3x3 : 5;
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
    u32 desperation_order : 1;
    u32 vantage_order : 1;
    u32 tar_force_twice_order : 1;
    u32 act_force_twice_order : 1;

    u32 skill_activated_sure_shoot : 1;
    u32 skill_activated_dead_eye : 1;
    u32 skill_activated_aether : 1;
} gBattleTemporaryFlag;

enum BattleOrderSkills_Type {
    BORDER_DESPERATION,
    BORDER_VANTAGE,
    BORDER_ACT_TWICE,
    BORDER_TAR_TWICE,

    BORDER_MAX
};
extern u16 BattleOrderSkills[BORDER_MAX];

#define RegisterBattleOrderSkill(sid, type) (BattleOrderSkills[type] = (sid))

/* Battle skill act */
bool CheckBattleSkillActivate(struct BattleUnit * actor, struct BattleUnit * target, int sid, int rate);

int GetWeaponCost(struct BattleUnit * bu, u16 item);
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
extern const u16 _DmgDecreaseRef[100];
#define DAMAGE_DECREASE(rate) _DmgDecreaseRef[rate]

void PreBattleGenerateHook(void);
