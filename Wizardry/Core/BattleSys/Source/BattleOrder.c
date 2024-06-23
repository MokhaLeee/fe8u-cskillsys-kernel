#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "combat-art.h"
#include "combo-attack.h"
#include "constants/skills.h"

#define LOCAL_TRACE 1

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

static const u8 BattleUnwindConfig[14][4] = {
    { ACT_ATTACK, TAR_ATTACK, NOP_ATTACK, NOP_ATTACK }, // 0:default
    { TAR_ATTACK, ACT_ATTACK, NOP_ATTACK, NOP_ATTACK }, // 1   = 1
    { ACT_ATTACK, ACT_ATTACK, TAR_ATTACK, NOP_ATTACK }, // 2   = 2    

    { TAR_ATTACK, ACT_ATTACK, ACT_ATTACK, NOP_ATTACK }, // 12  = 3
    { ACT_ATTACK, TAR_ATTACK, ACT_ATTACK, NOP_ATTACK }, // 3   = 4
    { TAR_ATTACK, ACT_ATTACK, ACT_ATTACK, NOP_ATTACK }, // 13  = 5
    { ACT_ATTACK, TAR_ATTACK, NOP_ATTACK, NOP_ATTACK }, // 0:default
    { ACT_ATTACK, TAR_ATTACK, NOP_ATTACK, NOP_ATTACK }, // 0:default
    { ACT_ATTACK, TAR_ATTACK, TAR_ATTACK, NOP_ATTACK }, // 4   = 8
    { TAR_ATTACK, ACT_ATTACK, TAR_ATTACK, NOP_ATTACK }, // 14  = 9
    { ACT_ATTACK, ACT_ATTACK, TAR_ATTACK, TAR_ATTACK }, // 24  = 10

    { TAR_ATTACK, ACT_ATTACK, ACT_ATTACK, TAR_ATTACK }, // 124 = 11
    { ACT_ATTACK, TAR_ATTACK, ACT_ATTACK, TAR_ATTACK }, // 34  = 12
    { TAR_ATTACK, ACT_ATTACK, TAR_ATTACK, ACT_ATTACK }  // 134 = 13
};

extern struct {
    u8 cur, max;
    u16 skill_pool[14];
} sEfxSkillQueue;

static void ResetRoundEfxSkills(void)
{
    memset(&sEfxSkillQueue, 0, sizeof(sEfxSkillQueue));
}

static void EnqueueRoundEfxSkill(u16 sid)
{
    if (sEfxSkillQueue.max < (sizeof(sEfxSkillQueue.skill_pool) - 1))
        sEfxSkillQueue.skill_pool[sEfxSkillQueue.max++] = sid;
}

static u16 DequeueRoundEfxSkill(void)
{
    if (sEfxSkillQueue.cur < sEfxSkillQueue.max)
        return sEfxSkillQueue.skill_pool[sEfxSkillQueue.cur++];

    return 0;
}

/* This function should also be called by BKSEL, so non static */
bool CheckCanTwiceAttackOrder(struct BattleUnit * actor, struct BattleUnit * target)
{
    u8 cid;
    struct Unit * real_actor = GetUnit(gBattleActor.unit.index);
    struct Unit * real_target = GetUnit(gBattleTarget.unit.index);

    if (target->battleSpeed > 250)
        return false;

    if (GetItemWeaponEffect(actor->weaponBefore) == WPN_EFFECT_HPHALVE)
        return false;

    if (GetItemIndex(actor->weapon) == ITEM_MONSTER_STONE)
        return false;

    /* Check combat-art */
    cid = GetCombatArtInForce(&actor->unit);

    if (&gBattleActor == actor && COMBART_VALID(cid))
    {
        if (gpCombatArtInfos[cid].double_attack)
            return true;

        return false;
    }

    if (&gBattleActor == actor)
    {
#if defined(SID_WaryFighter) && (COMMON_SKILL_VALID(SID_WaryFighter))
        if (SkillTester(real_target, SID_WaryFighter))
            if ((GetUnitCurrentHp(real_target) * 2) > HpMaxGetter(real_target))
                return false;
#endif

        gBattleTemporaryFlag.order_dobule_lion = false;

#if defined(SID_DoubleLion) && (COMMON_SKILL_VALID(SID_DoubleLion))
        if (SkillTester(real_actor, SID_DoubleLion))
        {
            if (GetUnitCurrentHp(real_actor) == HpMaxGetter(real_actor))
            {
                gBattleActorGlobalFlag.skill_activated_double_lion = true;
                gBattleTemporaryFlag.order_dobule_lion = true;
                return true;
            }
        }
#endif
    }
    else if (&gBattleTarget == actor)
    {
        gBattleTemporaryFlag.order_quick_riposte = false;

#if defined(SID_QuickRiposte) && (COMMON_SKILL_VALID(SID_QuickRiposte))
        if (SkillTester(real_actor, SID_QuickRiposte))
        {
            if ((GetUnitCurrentHp(real_target) * 2) > HpMaxGetter(real_target))
            {
                gBattleTemporaryFlag.order_quick_riposte = true;
                return true;
            }
        }
#endif
    }

    if ((actor->battleSpeed - target->battleSpeed) < BATTLE_FOLLOWUP_SPEED_THRESHOLD)
        return false;

    return true;
}

STATIC_DECLAR bool CheckDesperationOrder(void)
{
    struct Unit * actor = GetUnit(gBattleActor.unit.index);

    gBattleTemporaryFlag.order_desperation = false;

#if defined(SID_Desperation) && (COMMON_SKILL_VALID(SID_Desperation))
    if (SkillTester(actor, SID_Desperation))
    {
        if ((GetUnitCurrentHp(actor) * 2) < HpMaxGetter(actor))
        {
            gBattleTemporaryFlag.order_desperation = true;
            return true;
        }
    }
#endif
    return false;
}

STATIC_DECLAR bool CheckVantageOrder(void)
{
    struct Unit * target = GetUnit(gBattleTarget.unit.index);

    gBattleTemporaryFlag.order_vantage = false;

    /* Combat art will not allow vantage */
    if (COMBART_VALID(GetCombatArtInForce(&gBattleActor.unit)))
        return false;

    if (SkillTester(target, SID_Vantage))
    {
        if ((GetUnitCurrentHp(target) * 2) < HpMaxGetter(target))
        {
            gBattleTemporaryFlag.order_vantage = true;
            return true;
        }
    }
    return false;
}

/* LynJump */
void BattleUnwind(void)
{
    int i, ret;
#ifdef CONFIG_USE_COMBO_ATTACK
    bool combo_atk_done = false;
#endif
    u8 round_mask = 0;
    const u8 * config;

    /* Identifier to record attack amount for skill anim triger */
    int actor_count = 0;
    int target_count = 0;

    ClearBattleHits();
    gBattleHitIterator->info |= BATTLE_HIT_INFO_BEGIN;

    /**
     * BattleGlobalFlag should not clear in battle routine
     * because combat art flag is configured in pre-combat.
     * It is cleared in:
     *  a). post action
     *  b). game init
     */

    // ClearBattleGlobalFlags();

    if (CheckDesperationOrder())
        round_mask |= UNWIND_DESPERA;

    if (CheckVantageOrder())
        round_mask |= UNWIND_VANTAGE;

    if (CheckCanTwiceAttackOrder(&gBattleActor, &gBattleTarget))
        round_mask |= UNWIND_DOUBLE_ACT;

    if (CheckCanTwiceAttackOrder(&gBattleTarget, &gBattleActor))
        round_mask |= UNWIND_DOUBLE_TAR;

    config = BattleUnwindConfig[round_mask];

    for (i = 0; i < 4; i++)
    {
        struct BattleHit * old = gBattleHitIterator;

        if (ACT_ATTACK == config[i])
        {
#ifdef CONFIG_USE_COMBO_ATTACK
            /* Combo-attack first */
            if (!combo_atk_done)
            {
                combo_atk_done = true;
                ret = BattleComboGenerateHits();
                if (ret)
                    break;

                /* Reload battle-hit */
                old = gBattleHitIterator;
                LTRACEF("Combo end at round round %d", GetBattleHitRound(old));
            }
#endif
            ret = BattleGenerateRoundHits(&gBattleActor, &gBattleTarget);
            actor_count++;
        }
        else if (TAR_ATTACK == config[i])
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_RETALIATE;
            ret = BattleGenerateRoundHits(&gBattleTarget, &gBattleActor);
            target_count++;
        }
        else if (NOP_ATTACK == config[i])
        {
            break;
        }

        /* Combat art first */
        if (i == 0)
        {
            int cid = GetCombatArtInForce(&gBattleActor.unit);
            if (COMBART_VALID(cid))
                RegisterEfxSkillCombatArt(GetBattleHitRound(old), cid);
        }

        if (i != 0 && config[i - 1] == config[i])
            gBattleHitIterator->attributes = BATTLE_HIT_ATTR_FOLLOWUP;

#if defined(SID_Vantage) && (COMMON_SKILL_VALID(SID_Vantage))
        /* Vantage */
        if (i == 0 && (round_mask & UNWIND_VANTAGE))
        {
            if (gBattleTemporaryFlag.order_vantage)
                RegisterActorEfxSkill(GetBattleHitRound(old), SID_Vantage);
        }
#endif

#if defined(SID_Desperation) && (COMMON_SKILL_VALID(SID_Desperation))
        /* Desperation */
        if (i == 1 && (round_mask & UNWIND_DESPERA))
        {
            if (config[0] == ACT_ATTACK && config[1] == ACT_ATTACK && config[2] == TAR_ATTACK)
            {
                if (gBattleTemporaryFlag.order_desperation)
                    RegisterActorEfxSkill(GetBattleHitRound(old), SID_Desperation);
            }
        }
#endif

#if defined(SID_QuickRiposte) && (COMMON_SKILL_VALID(SID_QuickRiposte))
        /* Target double attack */
        if (target_count > 1 && config[i] == TAR_ATTACK)
        {
            if (gBattleTemporaryFlag.order_quick_riposte)
                RegisterActorEfxSkill(GetBattleHitRound(old), SID_QuickRiposte);
        }
#endif

#if defined(SID_DoubleLion) && (COMMON_SKILL_VALID(SID_DoubleLion))
        /* Actor double attack */
        if (actor_count > 1 && config[i] == ACT_ATTACK)
        {
            if (gBattleTemporaryFlag.order_dobule_lion)
                RegisterActorEfxSkill(GetBattleHitRound(old), SID_DoubleLion);
        }
#endif

        if (ret)
            break;
    }
    gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
}

/* LynJump */
bool BattleGenerateRoundHits(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int i, count;
    u32 attrs;

    if (!attacker->weapon)
        return FALSE;

    /* Clear the round related efx skill pool */
    ResetRoundEfxSkills();

    attrs = gBattleHitIterator->attributes;
    count = GetBattleUnitHitCount(attacker);

    for (i = 0; i < count; ++i)
    {
        int efx_sid, round = GetBattleHitRound(gBattleHitIterator);

        gBattleHitIterator->attributes |= attrs;

        if (i != 0)
        {
            efx_sid = DequeueRoundEfxSkill();
            if (SKILL_VALID(efx_sid))
            {
                LTRACEF("Round skill %02x registered at round %d", efx_sid, round);
                RegisterActorEfxSkill(round, efx_sid);
            }
        }

        if (BattleGenerateHit(attacker, defender))
            return true;

        /* Hack here: we need avoid hit array overflow */
        if (CheckBattleHitOverflow())
        {
            Error("Battle hit overflowed!");
            gBattleHitIterator = gBattleHitIterator - 1;
            gBattleHitIterator->info |= (BATTLE_HIT_INFO_FINISHES | BATTLE_HIT_INFO_END);
            return true;
        }
    }
    return false;
}

/* LynJump */
bool BattleGetFollowUpOrder(struct BattleUnit ** outAttacker, struct BattleUnit ** outDefender)
{
    if (CheckCanTwiceAttackOrder(&gBattleActor, &gBattleTarget))
    {
        *outAttacker = &gBattleActor;
        *outDefender = &gBattleTarget;
        return true;
    }
    else if (CheckCanTwiceAttackOrder(&gBattleTarget, &gBattleActor))
    {
        *outAttacker = &gBattleTarget;
        *outDefender = &gBattleActor;
        return true;
    }
    return false;
}

/* LynJump */
int GetBattleUnitHitCount(struct BattleUnit * actor)
{
    int result = 1;
    struct BattleUnit * target = (actor == &gBattleActor)
                               ? &gBattleTarget
                               : &gBattleActor;

    if (BattleCheckBraveEffect(actor))
        result = result + 1;

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
    if (SkillTester(&actor->unit, SID_RuinedBladePlus))
    {
        EnqueueRoundEfxSkill(SID_RuinedBladePlus);
        result = result + 1;
    }
#endif

#if defined(SID_Astra) && (COMMON_SKILL_VALID(SID_Astra))
    if (actor == &gBattleActor && CheckBattleSkillActivte(actor, target, SID_Astra, actor->unit.spd))
    {
        EnqueueRoundEfxSkill(SID_Astra);
        gBattleActorGlobalFlag.skill_activated_astra = true;
        result = result + 4;
    }
#endif

#if defined(SID_Adept) && (COMMON_SKILL_VALID(SID_Adept))
    if (SkillTester(&actor->unit, SID_Adept) && actor->hpInitial == actor->unit.maxHP)
    {
        EnqueueRoundEfxSkill(SID_Adept);
        result = result + 1;
    }
#endif

    return result;
}
