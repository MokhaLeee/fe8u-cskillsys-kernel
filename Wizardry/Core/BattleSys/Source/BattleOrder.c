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
bool CheckCanTwiceAttackOrder(struct BattleUnit *actor, struct BattleUnit *target)
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
        switch (GetCombatArtInfo(cid)->double_attack)
        {
        case COMBART_DOUBLE_DISABLED:
            return false;

        case COMBART_DOUBLE_FORCE_ENABLED:
            return true;

        case COMBART_DOUBLE_ENABLED:
        default:
            break;
        }
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
        if (basic_judgement == false && BattleSkillTester(actor, SID_BoldFighter) && (actor->hpInitial * 2) >= actor->unit.maxHP)
        {
            gBattleTemporaryFlag.act_force_twice_order = true;
            RegisterBattleOrderSkill(SID_BoldFighter, BORDER_ACT_TWICE);
            return true;
        }
#endif

#if defined(SID_RecklessFighter) && (COMMON_SKILL_VALID(SID_RecklessFighter))
        if (basic_judgement == false)
        {
            if (BattleSkillTester(actor, SID_RecklessFighter) && (actor->hpInitial * 2) >= actor->unit.maxHP)
            {
                RegisterBattleOrderSkill(SID_RecklessFighter, BORDER_ACT_TWICE);
                gBattleTemporaryFlag.act_force_twice_order = true;
                return true;
            }
            else if (BattleSkillTester(target, SID_RecklessFighter) && (target->hpInitial * 2) >= target->unit.maxHP)
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

#if defined(SID_AdvantageChaser) && (COMMON_SKILL_VALID(SID_AdvantageChaser))
        if (basic_judgement == false && BattleSkillTester(actor, SID_AdvantageChaser))
        {
            if (actor->wTriangleDmgBonus > 0 || actor->wTriangleHitBonus > 0)
            {
                gBattleTemporaryFlag.act_force_twice_order = true;
                RegisterBattleOrderSkill(SID_AdvantageChaser, BORDER_ACT_TWICE);
                return true;
            }
        }
#endif

#if defined(SID_Moonlight) && (COMMON_SKILL_VALID(SID_Moonlight))
        if (basic_judgement == true && BattleSkillTester(actor, SID_Moonlight))
            return false;
#endif

#if defined(SID_PridefulWarrior) && (COMMON_SKILL_VALID(SID_PridefulWarrior))
        if (BattleSkillTester(actor, SID_PridefulWarrior))
        {
            gBattleTemporaryFlag.act_force_twice_order = true;
            RegisterBattleOrderSkill(SID_PridefulWarrior, BORDER_ACT_TWICE);
            return true;
        }
#endif

#if defined(SID_PassionsFlow) && (COMMON_SKILL_VALID(SID_PassionsFlow))
        if (basic_judgement == false && BattleSkillTester(actor, SID_PassionsFlow))
        {
            struct SupportBonuses bonuses;
            if (GetUnitSupportBonuses(GetUnit(actor->unit.index), &bonuses) != 0)
            {
                gBattleTemporaryFlag.act_force_twice_order = true;
                RegisterBattleOrderSkill(SID_PassionsFlow, BORDER_ACT_TWICE);
                return true;
            }
        }
#endif
    }
    else if (&gBattleTarget == actor)
    {
        gBattleTemporaryFlag.tar_force_twice_order = false;

#if defined(SID_VengefulFighter) && (COMMON_SKILL_VALID(SID_VengefulFighter))
        if (basic_judgement == false && BattleSkillTester(actor, SID_VengefulFighter) && (actor->hpInitial * 2) >= actor->unit.maxHP)
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

#if defined(SID_AdvantageChaser) && (COMMON_SKILL_VALID(SID_AdvantageChaser))
        if (basic_judgement == false && BattleSkillTester(actor, SID_AdvantageChaser))
        {
            if (actor->wTriangleDmgBonus > 0 || actor->wTriangleHitBonus > 0)
            {
                gBattleTemporaryFlag.tar_force_twice_order = true;
                RegisterBattleOrderSkill(SID_AdvantageChaser, BORDER_TAR_TWICE);
                return true;
            }
        }
#endif

#if defined(SID_PassionsFlow) && (COMMON_SKILL_VALID(SID_PassionsFlow))
        if (basic_judgement == false && BattleSkillTester(actor, SID_PassionsFlow))
        {
            struct SupportBonuses bonuses;
            if (GetUnitSupportBonuses(GetUnit(actor->unit.index), &bonuses) != 0)
            {
                gBattleTemporaryFlag.tar_force_twice_order = true;
                RegisterBattleOrderSkill(SID_PassionsFlow, BORDER_TAR_TWICE);
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

#if defined(SID_CloseCombat) && (COMMON_SKILL_VALID(SID_CloseCombat))
    if (BattleSkillTester(&gBattleActor, SID_CloseCombat) && gBattleStats.range == 1)
    {
        gBattleTemporaryFlag.desperation_order = true;
        RegisterBattleOrderSkill(SID_CloseCombat, BORDER_DESPERATION);
        return true;
    }
#endif

#if defined(SID_Pursuer) && (COMMON_SKILL_VALID(SID_Pursuer))
    if (BattleSkillTester(&gBattleActor, SID_Pursuer))
    {
        gBattleTemporaryFlag.desperation_order = true;
        RegisterBattleOrderSkill(SID_Pursuer, BORDER_DESPERATION);
        return true;
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

#if defined(SID_GaleWings) && (COMMON_SKILL_VALID(SID_GaleWings))
    if (BattleSkillTester(&gBattleTarget, SID_GaleWings))
    {
        if (gBattleTarget.hpInitial == gBattleTarget.unit.maxHP)
        {
            RegisterBattleOrderSkill(SID_GaleWings, BORDER_VANTAGE);
            gBattleTemporaryFlag.vantage_order = true;
            return true;
        }
    }
#endif

#if defined(SID_PridefulWarrior) && (COMMON_SKILL_VALID(SID_PridefulWarrior))
    if (BattleSkillTester(&gBattleActor, SID_PridefulWarrior))
    {
        /* actor can enable the foe to attack first */
        gBattleTemporaryFlag.vantage_order = true;
        return true;
    }
#endif

    return false;
}

STATIC_DECLAR bool ContinueIfAccost(struct BattleUnit *attacker, struct BattleUnit *defender)
{
    int activationChance = 0;

    if (attacker->unit.curHP >= 25 && BattleSkillTester(attacker, SID_Accost))
        activationChance = (attacker->battleSpeed + attacker->unit.curHP / 2) - defender->battleSpeed;

    else if (defender->unit.curHP >= 25 && BattleSkillTester(defender, SID_Accost))
        activationChance = (defender->battleSpeed + defender->unit.curHP / 2) - attacker->battleSpeed;

    if (activationChance < 0)
        activationChance = 0;

    return BattleRoll1RN(activationChance, FALSE);
}

LYN_REPLACE_CHECK(BattleUnwind);
void BattleUnwind(void)
{
    int i;
    int ret = 0;
    int round_counter = 1;
    bool accost_active;
#ifdef CONFIG_USE_COMBO_ATTACK
    bool combo_atk_done = false;
#endif
    u8 round_mask = 0;
    const u8 *config;

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

    if (CheckDesperationOrder())
        round_mask |= UNWIND_DESPERA;

    if (CheckVantageOrder())
        round_mask |= UNWIND_VANTAGE;

    if (CheckCanTwiceAttackOrder(&gBattleActor, &gBattleTarget))
        round_mask |= UNWIND_DOUBLE_ACT;

    if (CheckCanTwiceAttackOrder(&gBattleTarget, &gBattleActor))
        round_mask |= UNWIND_DOUBLE_TAR;

    config = BattleUnwindConfig[round_mask];

#if (defined(SID_Accost) && COMMON_SKILL_VALID(SID_Accost))
    if (BattleSkillTester(&gBattleActor, SID_Accost) || BattleSkillTester(&gBattleTarget, SID_Accost))
        accost_active = true;
    else
        accost_active = false;
#endif

    do
    {
        for (i = 0; i < 4; i++)
        {
            struct BattleHit *old = gBattleHitIterator;

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
            {
                gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
                return;
            }

            if (gBattleActor.unit.curHP == 0 || gBattleTarget.unit.curHP == 0)
            {
                gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
                return;
            }
        }
        if (!accost_active)
        {
            gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
            return;
        }
        else
        {
            if (!ContinueIfAccost(&gBattleActor, &gBattleTarget))
            {
                gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
                return;
            }
        }
        ++round_counter;
    } while (round_counter < 20);
}

LYN_REPLACE_CHECK(BattleGenerateRoundHits);
bool BattleGenerateRoundHits(struct BattleUnit *attacker, struct BattleUnit *defender)
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

        /* I think this is a bug-fix for vanilla */
        if (!attacker->weapon)
            return false;
    }
    return false;
}

LYN_REPLACE_CHECK(BattleGetFollowUpOrder);
bool BattleGetFollowUpOrder(struct BattleUnit **outAttacker, struct BattleUnit **outDefender)
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

LYN_REPLACE_CHECK(GetBattleUnitHitCount);
int GetBattleUnitHitCount(struct BattleUnit *actor)
{
    int result = 1;
    struct BattleUnit *target = (actor == &gBattleActor)
                                    ? &gBattleTarget
                                    : &gBattleActor;

    if (BattleCheckBraveEffect(actor))
        result = result + 1;

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
    if (BattleSkillTester(actor, SID_RuinedBladePlus))
    {
        EnqueueRoundEfxSkill(SID_RuinedBladePlus);
        result = result + SKILL_EFF2(SID_RuinedBladePlus);
    }
#endif

#if defined(SID_Astra) && (COMMON_SKILL_VALID(SID_Astra))
    if (actor == &gBattleActor && CheckBattleSkillActivate(actor, target, SID_Astra, actor->unit.spd))
    {
        EnqueueRoundEfxSkill(SID_Astra);
        gBattleActorGlobalFlag.skill_activated_astra = true;
        result = result + SKILL_EFF0(SID_Astra);
    }
#endif

#if defined(SID_Adept) && (COMMON_SKILL_VALID(SID_Adept))
    if (BattleSkillTester(actor, SID_Adept) && actor->hpInitial == actor->unit.maxHP)
    {
        EnqueueRoundEfxSkill(SID_Adept);
        result = result + 1;
    }
#endif

#if defined(SID_ChargePlus) && (COMMON_SKILL_VALID(SID_ChargePlus))
    if (BattleSkillTester(actor, SID_ChargePlus))
    {
        if (MovGetter(gActiveUnit) == gActionData.moveCount)
            result = result + 1;
    }
#endif

#if defined(SID_DoubleLion) && (COMMON_SKILL_VALID(SID_DoubleLion))
    if (actor == &gBattleActor && BattleSkillTester(actor, SID_DoubleLion) && actor->hpInitial == actor->unit.maxHP)
    {
        gBattleActorGlobalFlag.skill_activated_double_lion = true;
        EnqueueRoundEfxSkill(SID_DoubleLion);
        result = result + 1;
    }
#endif

    return result;
}
