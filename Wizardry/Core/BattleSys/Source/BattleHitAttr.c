#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

bool CheckBattleHpDrain(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN)
        return true;

    if (gBattleTemporaryFlag.skill_activated_aether)
        return true;

#if (defined(SID_Sol) && (COMMON_SKILL_VALID(SID_Sol)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Sol, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Sol);
        return true;
    }
#endif

    return false;
}

bool CheckBattleHpHalve(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPHALVE)
        return true;

#if (defined(SID_Eclipse) && (COMMON_SKILL_VALID(SID_Eclipse)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Eclipse, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Eclipse);
        return true;
    }
#endif

    return false;
}

bool CheckDevilAttack(struct BattleUnit * attacker, struct BattleUnit * defender)
{

#if (defined(SID_Counter) && (COMMON_SKILL_VALID(SID_Counter)))
    if (BattleSkillTester(defender, SID_Counter) && gBattleStats.range == 1 && !IsMagicAttack(attacker))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Counter);
        return true;
    }
#endif

#if (defined(SID_CounterMagic) && (COMMON_SKILL_VALID(SID_CounterMagic)))
    if (BattleSkillTester(defender, SID_CounterMagic) && gBattleStats.range >= 2 && IsMagicAttack(attacker))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_CounterMagic);
        return true;
    }
#endif

    if (!BattleRoll1RN(31 - attacker->unit.lck, FALSE))
    {
        /* Lucky */
        return false;
    }

#if (defined(SID_DevilsLuck) && (COMMON_SKILL_VALID(SID_DevilsLuck)))
    if (BattleSkillTester(defender, SID_DevilsLuck) && GetItemWeaponEffect(defender->weapon) == WPN_EFFECT_DEVIL)
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsLuck);
        return true;
    }
    if (BattleSkillTester(attacker, SID_DevilsLuck) && GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL)
        return false;
#endif

    if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL)
        return true;

#if (defined(SID_DevilsPact) && (COMMON_SKILL_VALID(SID_DevilsPact)))
    if (BattleSkillTester(defender, SID_DevilsPact))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsPact);
        return true;
    }
#endif

#if (defined(SID_DevilsWhim) && (COMMON_SKILL_VALID(SID_DevilsWhim)))
    if (BattleSkillTester(defender, SID_DevilsWhim))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsWhim);
        return true;
    }

    if (BattleSkillTester(attacker, SID_DevilsWhim))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DevilsWhim);
        return true;
    }
#endif

    return false;
}

bool CheckBattleInori(struct BattleUnit * attacker, struct BattleUnit * defender)
{
#if (defined(SID_Mercy) && (COMMON_SKILL_VALID(SID_Mercy)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Mercy, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Mercy);
        return true;
    }
#endif

#if (defined(SID_Inori) && (COMMON_SKILL_VALID(SID_Inori)))
    if (CheckBattleSkillActivate(defender, attacker, SID_Inori, defender->unit.lck))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Inori);
        return true;
    }
#endif

#if (defined(SID_LEGEND_InoriAtk) && (COMMON_SKILL_VALID(SID_LEGEND_InoriAtk)))
    if (CheckBattleSkillActivate(defender, attacker, SID_LEGEND_InoriAtk, 100))
    {
        if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriAtk) == 0)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_InoriAtk);
            return true;
        }
    }
#endif

#if (defined(SID_LEGEND_InoriAvo) && (COMMON_SKILL_VALID(SID_LEGEND_InoriAvo)))
    if (CheckBattleSkillActivate(defender, attacker, SID_LEGEND_InoriAvo, 100))
    {
        if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriAvo) == 0)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_InoriAvo);
            return true;
        }
    }
#endif

#if (defined(SID_LEGEND_InoriDef) && (COMMON_SKILL_VALID(SID_LEGEND_InoriDef)))
    if (CheckBattleSkillActivate(defender, attacker, SID_LEGEND_InoriDef, 100))
    {
        if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriDef) == 0)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_InoriDef);
            return true;
        }
    }
#endif

    return false;
}

void BattleHit_InjectNegativeStatus(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int debuff;

    if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_PETRIFY)
    {
        switch (gPlaySt.faction) {
        case FACTION_BLUE:
            if (UNIT_FACTION(&defender->unit) == FACTION_BLUE)
                defender->statusOut = UNIT_STATUS_13;
            else
                defender->statusOut = UNIT_STATUS_PETRIFY;
            break;

        case FACTION_RED:
            if (UNIT_FACTION(&defender->unit) == FACTION_RED)
                defender->statusOut = UNIT_STATUS_13;
            else
                defender->statusOut = UNIT_STATUS_PETRIFY;
            break;

        case FACTION_GREEN:
            if (UNIT_FACTION(&defender->unit) == FACTION_GREEN)
                defender->statusOut = UNIT_STATUS_13;
            else
                defender->statusOut = UNIT_STATUS_PETRIFY;
            break;
        }
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_PETRIFY;
    }
#if (defined(SID_Petrify) && (COMMON_SKILL_VALID(SID_Petrify)))
    else if (CheckBattleSkillActivate(attacker, defender, SID_Petrify, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Petrify);
        defender->statusOut = UNIT_STATUS_PETRIFY;
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_PETRIFY;
    }
#endif
    else if (gBattleTemporaryFlag.skill_activated_dead_eye)
    {
        defender->statusOut = UNIT_STATUS_SLEEP;
    }
    else if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_POISON)
    {
        defender->statusOut = UNIT_STATUS_POISON;
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

        // "Ungray" defender if it was petrified (as it won't be anymore)
        debuff = GetUnitStatusIndex(&defender->unit);
        if (debuff == UNIT_STATUS_PETRIFY || debuff == UNIT_STATUS_13)
            defender->unit.state = defender->unit.state &~ US_UNSELECTABLE;
    }
#if (defined(SID_PoisonPoint) && (COMMON_SKILL_VALID(SID_PoisonPoint)))
    else if (BattleSkillTester(attacker, SID_PoisonPoint))
    {
        defender->statusOut = UNIT_STATUS_POISON;
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

        // "Ungray" defender if it was petrified (as it won't be anymore)
        debuff = GetUnitStatusIndex(&defender->unit);
        if (debuff == UNIT_STATUS_PETRIFY || debuff == UNIT_STATUS_13)
            defender->unit.state = defender->unit.state &~ US_UNSELECTABLE;
    }
#endif
#if (defined(SID_Enrage) && (COMMON_SKILL_VALID(SID_Enrage)))
    else if (CheckBattleSkillActivate(attacker, defender, SID_Enrage, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Enrage);
        defender->statusOut = UNIT_STATUS_BERSERK;
    }
#endif
}

void BattleHit_ConsumeWeapon(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    bool weapon_cost;

    /**
     * Consume enemy weapons
     */
#if (defined(SID_Corrosion) && (COMMON_SKILL_VALID(SID_Corrosion)))
    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS) && CheckBattleSkillActivate(attacker, defender, SID_Corrosion, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corrosion);
        int cost = attacker->levelPrevious;
    
        while (cost-- > 0)
        {
            u16 weapon = GetItemAfterUse(defender->weapon);
            defender->weapon = weapon;

            if (!weapon)
                break;
        }

        if (!defender->weapon)
            defender->weaponBroke = TRUE;
    }
#endif

    /**
     * Consumes the durability of the own weapon
     */
    weapon_cost = false;
    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
        weapon_cost = true;
    else if (attacker->weaponAttributes & (IA_UNCOUNTERABLE | IA_MAGIC))
        weapon_cost = true;

#if defined(SID_Armsthrift) && (COMMON_SKILL_VALID(SID_Armsthrift))
    if (CheckBattleSkillActivate(attacker, defender, SID_Armsthrift, attacker->unit.lck))
    {
        weapon_cost = false;
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Armsthrift);
    }
#endif

    if (weapon_cost)
    {
#ifdef CHAX
        /* Check on combat-art */
        int cost = GetWeaponCost(attacker, attacker->weapon);
        while (cost-- > 0)
        {
            u16 weapon = GetItemAfterUse(attacker->weapon);
            attacker->weapon = weapon;

            if (!weapon)
                break;
        }
#else
        attacker->weapon = GetItemAfterUse(attacker->weapon);
#endif
        if (!attacker->weapon)
            attacker->weaponBroke = TRUE;
    }
}
