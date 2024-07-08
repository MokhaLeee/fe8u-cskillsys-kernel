#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "combat-art.h"
#include "combo-attack.h"
#include "constants/skills.h"

#define LOCAL_TRACE 1

/* This function should also be called by BKSEL, so non static */
bool CheckCanTwiceAttackOrder(struct BattleUnit * actor, struct BattleUnit * target)
{
    bool basic_judgement;
    u8 cid;

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

    /* Basic judgement */
    basic_judgement = (actor->battleSpeed - target->battleSpeed) >= BATTLE_FOLLOWUP_SPEED_THRESHOLD;

    if (&gBattleActor == actor)
    {
        gBattleTemporaryFlag.act_force_twice_order = false;

#if defined(SID_WaryFighter) && (COMMON_SKILL_VALID(SID_WaryFighter))
        if (basic_judgement == true && BattleSkillTester(target, SID_WaryFighter))
            if ((target->hpInitial * 2) > target->unit.maxHP)
                return false;
#endif

#if defined(SID_BoldFighter) && (COMMON_SKILL_VALID(SID_BoldFighter))
        if (basic_judgement == false && BattleSkillTester(actor, SID_BoldFighter))
        {
            gBattleTemporaryFlag.act_force_twice_order = true;
            RegisterBattleOrderSkill(SID_BoldFighter, BORDER_ACT_TWICE);
            return true;
        }
#endif

#if defined(SID_DoubleLion) && (COMMON_SKILL_VALID(SID_DoubleLion))
        if (basic_judgement == false && BattleSkillTester(actor, SID_DoubleLion))
        {
            if (actor->hpInitial == actor->unit.maxHP)
            {
                gBattleActorGlobalFlag.skill_activated_double_lion = true;
                gBattleTemporaryFlag.act_force_twice_order = true;
                RegisterBattleOrderSkill(SID_DoubleLion, BORDER_ACT_TWICE);
                return true;
            }
        }
#endif

#if defined(SID_RecklessFighter) && (COMMON_SKILL_VALID(SID_RecklessFighter))
        if (basic_judgement == false)
        {
            if (BattleSkillTester(actor, SID_RecklessFighter))
            {
                RegisterBattleOrderSkill(SID_RecklessFighter, BORDER_ACT_TWICE);
                gBattleTemporaryFlag.act_force_twice_order = true;
                return true;
            }
            else if (BattleSkillTester(target, SID_RecklessFighter))
            {
                gBattleTemporaryFlag.act_force_twice_order = true;
                return true;
            }
        }
#endif

#if defined(SID_BidingBlow) && (COMMON_SKILL_VALID(SID_BidingBlow))
        if (basic_judgement == false && BattleSkillTester(actor, SID_BidingBlow))
        {
            if (target->canCounter == false)
            {
                gBattleTemporaryFlag.act_force_twice_order = true;
                RegisterBattleOrderSkill(SID_BidingBlow, BORDER_ACT_TWICE);
                return true;
            }
        }
#endif

#if defined(SID_LastWord) && (COMMON_SKILL_VALID(SID_LastWord))
        if (basic_judgement == false && BattleSkillTester(actor, SID_LastWord))
        {
            if (target->battleSpeed >= actor->battleSpeed + 4)
            {
                gBattleTemporaryFlag.act_force_twice_order = true;
                RegisterBattleOrderSkill(SID_LastWord, BORDER_ACT_TWICE);
                return true;
            }
        }
#endif
    }
    else if (&gBattleTarget == actor)
    {
        gBattleTemporaryFlag.tar_force_twice_order = false;

#if defined(SID_VengefulFighter) && (COMMON_SKILL_VALID(SID_VengefulFighter))
        if (basic_judgement == false && BattleSkillTester(actor, SID_VengefulFighter))
        {
            gBattleTemporaryFlag.tar_force_twice_order = true;
            RegisterBattleOrderSkill(SID_VengefulFighter, BORDER_TAR_TWICE);
            return true;
        }
#endif

#if defined(SID_RecklessFighter) && (COMMON_SKILL_VALID(SID_RecklessFighter))
        if (basic_judgement == false)
        {
            if (BattleSkillTester(actor, SID_RecklessFighter))
            {
                RegisterBattleOrderSkill(SID_RecklessFighter, BORDER_TAR_TWICE);
                gBattleTemporaryFlag.tar_force_twice_order = true;
                return true;
            }
            else if (BattleSkillTester(target, SID_RecklessFighter))
            {
                gBattleTemporaryFlag.tar_force_twice_order = true;
                return true;
            }
        }
#endif

#if defined(SID_QuickRiposte) && (COMMON_SKILL_VALID(SID_QuickRiposte))
        if (basic_judgement == false && BattleSkillTester(actor, SID_QuickRiposte))
        {
            if ((actor->hpInitial * 2) > actor->unit.maxHP)
            {
                gBattleTemporaryFlag.tar_force_twice_order = true;
                RegisterBattleOrderSkill(SID_QuickRiposte, BORDER_TAR_TWICE);
                return true;
            }
        }
#endif
    }

    return basic_judgement;
}

STATIC_DECLAR bool CheckDesperationOrder(void)
{
    gBattleTemporaryFlag.desperation_order = false;

#if defined(SID_Desperation) && (COMMON_SKILL_VALID(SID_Desperation))
    if (BattleSkillTester(&gBattleActor, SID_Desperation))
    {
        if ((gBattleActor.hpInitial * 2) < gBattleActor.unit.maxHP)
        {
            gBattleTemporaryFlag.desperation_order = true;
            RegisterBattleOrderSkill(SID_Desperation, BORDER_DESPERATION);
            return true;
        }
    }
#endif
    return false;
}

STATIC_DECLAR bool CheckVantageOrder(void)
{
    gBattleTemporaryFlag.vantage_order = false;

    /* Combat art will not allow vantage */
    if (COMBART_VALID(GetCombatArtInForce(&gBattleActor.unit)))
        return false;

#if defined(SID_Vantage) && (COMMON_SKILL_VALID(SID_Vantage))
    if (BattleSkillTester(&gBattleTarget, SID_Vantage))
    {
        if ((gBattleTarget.hpInitial * 2) < gBattleTarget.unit.maxHP)
        {
            RegisterBattleOrderSkill(SID_Vantage, BORDER_VANTAGE);
            gBattleTemporaryFlag.vantage_order = true;
            return true;
        }
    }
#endif

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

    /* fasten calc! */
    if (!(gBattleStats.config & BATTLE_CONFIG_REAL))
    {
        gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
        return;
    }

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

        /* Vantage */
        if (i == 0 && (round_mask & UNWIND_VANTAGE))
            if (gBattleTemporaryFlag.vantage_order)
                RegisterActorEfxSkill(GetBattleHitRound(old), BattleOrderSkills[BORDER_VANTAGE]);

        /* Desperation */
        if (i == 1 && (round_mask & UNWIND_DESPERA))
            if (config[0] == ACT_ATTACK && config[1] == ACT_ATTACK && config[2] == TAR_ATTACK)
                if (gBattleTemporaryFlag.desperation_order)
                    RegisterActorEfxSkill(GetBattleHitRound(old), BattleOrderSkills[BORDER_DESPERATION]);

        /* Target double attack */
        if (target_count > 1 && config[i] == TAR_ATTACK)
            if (gBattleTemporaryFlag.tar_force_twice_order)
                RegisterActorEfxSkill(GetBattleHitRound(old), BattleOrderSkills[BORDER_TAR_TWICE]);

        /* Actor double attack */
        if (actor_count > 1 && config[i] == ACT_ATTACK)
            if (gBattleTemporaryFlag.act_force_twice_order)
                RegisterActorEfxSkill(GetBattleHitRound(old), BattleOrderSkills[BORDER_ACT_TWICE]);

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
            if (COMMON_SKILL_VALID(efx_sid))
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
    if (BattleSkillTester(actor, SID_RuinedBladePlus))
    {
        EnqueueRoundEfxSkill(SID_RuinedBladePlus);
        result = result + 1;
    }
#endif

#if defined(SID_Astra) && (COMMON_SKILL_VALID(SID_Astra))
    if (actor == &gBattleActor && CheckBattleSkillActivate(actor, target, SID_Astra, actor->unit.spd))
    {
        EnqueueRoundEfxSkill(SID_Astra);
        gBattleActorGlobalFlag.skill_activated_astra = true;
        result = result + 4;
    }
#endif

#if defined(SID_Adept) && (COMMON_SKILL_VALID(SID_Adept))
    if (BattleSkillTester(actor, SID_Adept) && actor->hpInitial == actor->unit.maxHP)
    {
        EnqueueRoundEfxSkill(SID_Adept);
        result = result + 1;
    }
#endif

    return result;
}
