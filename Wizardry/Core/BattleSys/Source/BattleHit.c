#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "combat-art.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

STATIC_DECLAR bool CheckSkillHpDrain(struct BattleUnit * attacker, struct BattleUnit * defender)
{
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

STATIC_DECLAR bool CheckSkillHpHalve(struct BattleUnit * attacker, struct BattleUnit * defender)
{

#if (defined(SID_Eclipse) && (COMMON_SKILL_VALID(SID_Eclipse)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Eclipse, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Eclipse);
        return true;
    }
#endif

    return false;
}

/* LynJump */
void BattleUpdateBattleStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int attack = attacker->battleAttack;
    int defense = defender->battleDefense;
    int hitRate = attacker->battleEffectiveHitRate;
    int critRate = attacker->battleEffectiveCritRate;
    int silencerRate = attacker->battleSilencerRate;

    /* Fasten simulation */
    if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
    {
        gBattleStats.attack = attack;
        gBattleStats.defense = defense;
        gBattleStats.hitRate = hitRate;
        gBattleStats.critRate = critRate;
        gBattleStats.silencerRate = silencerRate;
        return;
    }

#if defined(SID_AxeFaith) && (COMMON_SKILL_VALID(SID_AxeFaith))
    if (attacker->weaponType == ITYPE_AXE && CheckBattleSkillActivate(attacker, defender, SID_AxeFaith, attacker->battleAttack))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_AxeFaith);
        hitRate += attacker->battleAttack;
    }
#endif

    gBattleTemporaryFlag.skill_activated_sure_shoot = false;

#if (defined(SID_SureShot) && (COMMON_SKILL_VALID(SID_SureShot)))
    if (CheckBattleSkillActivate(attacker, defender, SID_SureShot, attacker->unit.skl))
    {
        gBattleTemporaryFlag.skill_activated_sure_shoot = true;
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_SureShot);
        hitRate = 100;
    }
#endif

    gBattleTemporaryFlag.skill_activated_dead_eye = false;

#if defined(SID_Deadeye) && (COMMON_SKILL_VALID(SID_Deadeye))
    if (CheckBattleSkillActivate(attacker, defender, SID_Deadeye, attacker->unit.skl))
    {
        gBattleTemporaryFlag.skill_activated_dead_eye = true;
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Deadeye);
        hitRate *= 2;
    }
#endif

    LIMIT_AREA(gBattleStats.attack, 0, 255);
    LIMIT_AREA(gBattleStats.defense, 0, 255);
    LIMIT_AREA(gBattleStats.hitRate, 0, 100);
    LIMIT_AREA(gBattleStats.critRate, 0, 100);
    LIMIT_AREA(gBattleStats.silencerRate, 0, 100);

    gBattleStats.attack = attack;
    gBattleStats.defense = defense;
    gBattleStats.hitRate = hitRate;
    gBattleStats.critRate = critRate;
    gBattleStats.silencerRate = silencerRate;
}

STATIC_DECLAR bool CheckDevilAttack(struct BattleUnit * attacker, struct BattleUnit * defender)
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

/* LynJump */
void BattleGenerateHitAttributes(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    gBattleStats.damage = 0;

    /* Fasten simulation */
    if (!BattleRoll2RN(gBattleStats.hitRate, FALSE))
    {
#if (defined(SID_DivinePulse) && (COMMON_SKILL_VALID(SID_DivinePulse)))
        if (BattleRoll2RN(gBattleStats.hitRate, FALSE) &&
            CheckBattleSkillActivate(attacker, defender, SID_DivinePulse, SKILL_EFF0(SID_DivinePulse) + attacker->unit.lck))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DivinePulse);
        }
        else
        {
            RegisterHitCnt(attacker, true);
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
            return;
        }
#else
        RegisterHitCnt(attacker, true);
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
        return;
#endif
    }

    RegisterHitCnt(attacker, false);

    /* Judge whether in combat-art attack */
    if (!!(gBattleStats.config & BATTLE_CONFIG_REAL) && attacker == &gBattleActor && COMBART_VALID(GetCombatArtInForce(&gBattleActor.unit)))
        TriggerKtutorial(KTUTORIAL_COMBATART_MENU);

    gBattleStats.damage = BattleHit_CalcDamage(attacker, defender);

    BattleCheckPetrify(attacker, defender);

    if (gBattleStats.damage != 0)
        attacker->nonZeroDamage = TRUE;
}

/* LynJump */
void BattleGenerateHitEffects(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int debuff;
    bool weapon_cost;

#if (defined(SID_Discipline) && (COMMON_SKILL_VALID(SID_Discipline)))
        if (BattleSkillTester(attacker, SID_Discipline))
            attacker->wexpMultiplier += 2;
        else
            attacker->wexpMultiplier++;
#else
        attacker->wexpMultiplier++;
#endif

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
    {
        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPHALVE || CheckSkillHpHalve(attacker, defender))
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPHALVE;
            gBattleStats.damage = defender->unit.curHP / 2;
        }

        if (CheckDevilAttack(attacker, defender))
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_DEVIL;

            attacker->unit.curHP -= gBattleStats.damage;

            if (attacker->unit.curHP < 0)
                attacker->unit.curHP = 0;
        }
        else
        {
            if (gBattleStats.damage > defender->unit.curHP)
                gBattleStats.damage = defender->unit.curHP;

#if defined(SID_Bane) && (COMMON_SKILL_VALID(SID_Bane))
            if (gBattleStats.damage < (defender->unit.curHP - 1))
            {
                if (CheckBattleSkillActivate(attacker, defender, SID_Bane, attacker->unit.skl))
                {
                    RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Bane);
                    gBattleStats.damage = defender->unit.curHP - 1;
                    gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
                }
            }
#endif
            defender->unit.curHP -= gBattleStats.damage;

            if (defender->unit.curHP < 0)
                defender->unit.curHP = 0;
        }

#ifdef CHAX
        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN || CheckSkillHpDrain(attacker, defender))
#else
        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN)
#endif
        {
            if (attacker->unit.maxHP < (attacker->unit.curHP + gBattleStats.damage))
                attacker->unit.curHP = attacker->unit.maxHP;
            else
                attacker->unit.curHP += gBattleStats.damage;

            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
        }

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

    gBattleHitIterator->hpChange = gBattleStats.damage;

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

STATIC_DECLAR bool InoriCheck(struct BattleUnit * attacker, struct BattleUnit * defender)
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

/* LynJump */
bool BattleGenerateHit(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (attacker == &gBattleTarget)
        gBattleHitIterator->info |= BATTLE_HIT_INFO_RETALIATION;

    BattleUpdateBattleStats(attacker, defender);

    BattleGenerateHitTriangleAttack(attacker, defender);
    BattleGenerateHitAttributes(attacker, defender);
    BattleGenerateHitEffects(attacker, defender);

    if (attacker->unit.curHP == 0 || defender->unit.curHP == 0)
    {
#if (defined(SID_Discipline) && (COMMON_SKILL_VALID(SID_Discipline)))
        if (BattleSkillTester(attacker, SID_Discipline))
            attacker->wexpMultiplier += 2;
        else
            attacker->wexpMultiplier++;
#else
        attacker->wexpMultiplier++;
#endif

        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;

#if CHAX
        if (defender->unit.curHP == 0)
        {
            if (InoriCheck(attacker, defender))
            {
                gBattleStats.damage = gBattleStats.damage - 1;
                gBattleHitIterator->hpChange = gBattleStats.damage;
                defender->unit.curHP = 1;

                gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
                gBattleHitIterator++;
                return true;
            }
        }
#endif

        if (gBattleTarget.unit.curHP == 0)
        {
            gBattleActorGlobalFlag.enimy_defeated = true;

#if (defined(SID_Galeforce) && (COMMON_SKILL_VALID(SID_Galeforce)))
            if (CheckBattleSkillActivate(&gBattleActor, &gBattleTarget, SID_Galeforce, gBattleActor.unit.skl))
                gBattleActorGlobalFlag.skill_activated_galeforce = true;
#endif

#if (defined(SID_Pickup) && (COMMON_SKILL_VALID(SID_Pickup)))
            if (CheckBattleSkillActivate(&gBattleActor, &gBattleTarget, SID_Pickup, gBattleActor.unit.lck))
            {
                struct Unit * unit_tar = &gBattleTarget.unit;
                unit_tar->state |= US_DROP_ITEM;
            }
#endif
            gBattleHitIterator->info |= BATTLE_HIT_INFO_KILLS_TARGET;
        }

        gBattleHitIterator++;
        return true;
    }
    else if (defender->statusOut == UNIT_STATUS_PETRIFY || defender->statusOut == UNIT_STATUS_13 || defender->statusOut == UNIT_STATUS_SLEEP)
    {
        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
        gBattleHitIterator++;
        return true;
    }

    gBattleHitIterator++;
    return false;
}
