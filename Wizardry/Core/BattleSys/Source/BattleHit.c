#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "combat-art.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

STATIC_DECLAR bool CheckSkillHpDrain(struct BattleUnit * attacker, struct BattleUnit * defender)
{
#if (defined(SID_Aether) && (SID_Aether < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(attacker, defender, SID_Aether, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aether);
        return true;
    }
#endif

#if (defined(SID_Sol) && (SID_Sol < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(attacker, defender, SID_Sol, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Sol);
        return true;
    }
#endif

    return false;
}

int CalcBattleRealDamage(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int damage = 0;

#if defined(SID_RuinedBlade) && (SID_RuinedBlade < MAX_SKILL_NUM)
    if (SkillTester(&attacker->unit, SID_RuinedBlade))
        damage += 5;
#endif

#if defined(SID_RuinedBladePlus) && (SID_RuinedBladePlus < MAX_SKILL_NUM)
    if (SkillTester(&attacker->unit, SID_RuinedBladePlus))
        damage += 5;
#endif

#if defined(SID_LunaAttack) && (SID_LunaAttack < MAX_SKILL_NUM)
    if (SkillTester(&attacker->unit, SID_LunaAttack))
        damage += defender->battleDefense / 4;
#endif

    return damage;
}

/* LynJump */
void BattleGenerateHitAttributes(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int attack, defense, amplificatier;
    int damage, real_damage;
    bool in_art_atk = false;

    gBattleStats.damage = 0;

    /* Judge whether in combat-art attack */
    if (!!(gBattleStats.config & BATTLE_CONFIG_REAL) && attacker == &gBattleActor && COMBART_VALID(GetCombatArtInForce(&gBattleActor.unit)))
    {
        TriggerKtutorial(KTUTORIAL_COMBATART_MENU);
        in_art_atk = true;
    }

    if (!BattleRoll2RN(gBattleStats.hitRate, TRUE))
    {
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
        return;
    }

    if (gBattleStats.config & BATTLE_CONFIG_REAL)
    {
        /**
         * Register hitted
         */
        if (attacker == &gBattleActor)
        {
            gBattleActorGlobalFlag.hitted = true;
        }
        else
        {
            gBattleTargetGlobalFlag.hitted = true;
        }

        /**
         * Well it's true that it seems no need to introduce an extra function
         * for combat-art,
         *
         * I admit this is part of shit Mountain....
         */
        if (in_art_atk)
        {
            RegisterCombatArtHitted();
        }
    }

    attack = gBattleStats.attack;
    defense = gBattleStats.defense;

#if (defined(SID_Flare) && (SID_Flare < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(attacker, defender, SID_Flare, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Flare);
        defense = defense / 2;
    }
#endif

    if (IsMagicAttack(attacker))
    {
#if (defined(SID_Corona) && (SID_Corona < MAX_SKILL_NUM))
        if (CheckBattleSkillActivte(attacker, defender, SID_Corona, attacker->unit.skl))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corona);
            defense = 0;
        }
#endif
    }
    else
    {
#if (defined(SID_Luna) && (SID_Luna < MAX_SKILL_NUM))
        if (CheckBattleSkillActivte(attacker, defender, SID_Luna, attacker->unit.skl))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Luna);
            defense = 0;
        }
#endif
    }

#if (defined(SID_Ignis) && (SID_Ignis < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(attacker, defender, SID_Ignis, attacker->unit.skl))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Ignis);
        switch (attacker->weaponType) {
        case ITYPE_SWORD:
        case ITYPE_LANCE:
        case ITYPE_AXE:
        case ITYPE_BOW:
            attack += attacker->unit.def / 2;
            break;

        case ITYPE_ANIMA:
        case ITYPE_LIGHT:
        case ITYPE_DARK:
            attack += attacker->unit.res / 2;
            break;
        default:
            break;
        }
    }
#endif

#if defined(SID_Glacies) && (SID_Glacies < MAX_SKILL_NUM)
    if (CheckBattleSkillActivte(attacker, defender, SID_Glacies, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Glacies);
        attack += attacker->unit.res;
    }
#endif

#if defined(SID_Vengeance) && (SID_Vengeance < MAX_SKILL_NUM)
    if (CheckBattleSkillActivte(attacker, defender, SID_Vengeance, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Vengeance);
        attack += (attacker->unit.maxHP - attacker->unit.curHP);

    }
#endif

    amplificatier = 100;
    damage = attack - defense;

#if defined(SID_FlashingBladePlus) && (SID_FlashingBladePlus < MAX_SKILL_NUM)
    if (SkillTester(&attacker->unit, SID_FlashingBladePlus))
        damage += 3;
#endif

#if defined(SID_DragonFang) && (SID_DragonFang < MAX_SKILL_NUM)
    if (SkillTester(&attacker->unit, SID_DragonFang))
        amplificatier += 50;
#endif

#if (defined(SID_Colossus) && (SID_Colossus < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(attacker, defender, SID_Colossus, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Colossus);
        amplificatier += 200;
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
    }
#endif

#if defined(SID_Impale) && (SID_Impale < MAX_SKILL_NUM)
    if (CheckBattleSkillActivte(attacker, defender, SID_Impale, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Impale);
        amplificatier += 300;
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
    }
#endif

#if defined(SID_Astra) && (SID_Astra < MAX_SKILL_NUM)
    if (attacker == &gBattleActor && SkillTester(&attacker->unit, SID_Astra) && gBattleActorGlobalFlag.skill_activated_astra)
    {
        /* If we can deal damage, astra should not reduce it to 0 */
        if (damage > 1)
            amplificatier /= 2;
    }
#endif

#if (defined(SID_DragonSkin) && (SID_DragonSkin < MAX_SKILL_NUM))
    if (SkillTester(&defender->unit, SID_DragonSkin))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DragonSkin);
        amplificatier /= 2;
    }
#endif

#if (defined(SID_KeenFighter) && (SID_KeenFighter < MAX_SKILL_NUM))
    if (SkillTester(&defender->unit, SID_KeenFighter) && CheckCanTwiceAttackOrder(attacker, defender))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_KeenFighter);
        amplificatier /= 2;
    }
#endif

    if (damage > 0)
    {
#if (defined(SID_GreatShield) && (SID_GreatShield < MAX_SKILL_NUM))
            if (CheckBattleSkillActivte(defender, attacker, SID_GreatShield, defender->unit.skl))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_GreatShield);
                damage = 0;
            }
#endif
        if (IsMagicAttack(attacker))
        {
#if (defined(SID_Aegis) && (SID_Aegis < MAX_SKILL_NUM))
            if (CheckBattleSkillActivte(defender, attacker, SID_Aegis, defender->unit.skl))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aegis);
                damage = 0;
            }
#endif
        }
        else
        {
#if (defined(SID_Pavise) && (SID_Pavise < MAX_SKILL_NUM))
            if (CheckBattleSkillActivte(defender, attacker, SID_Pavise, defender->unit.skl))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Pavise);
                damage = 0;
            }
#endif
        }
    }

    /* Minus zero */
    if (damage < 0)
        damage = 0;

    /**
     * Roll critical hit
     */
    if (BattleRoll1RN(gBattleStats.critRate, FALSE) == TRUE &&
#if defined(SID_Foresight) && (SID_Foresight < MAX_SKILL_NUM)
        !SkillTester(&defender->unit, SID_Foresight) &&
#else
        0 &&
#endif
#if defined(SID_Fortune) && (SID_Fortune < MAX_SKILL_NUM)
        !SkillTester(&defender->unit, SID_Fortune)
#else
        0
#endif
    )
    {
        if (BattleRoll1RN(gBattleStats.silencerRate, FALSE))
        {
            /* Silencer */
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_SILENCER;
            damage = BATTLE_MAX_DAMAGE;
        }
        else
        {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;

#if defined(SID_InfinityEdge) && (SID_InfinityEdge < MAX_SKILL_NUM)
            if (SkillTester(&attacker->unit, SID_InfinityEdge))
#else
            if (0)
#endif
                amplificatier += 300;
            else
                amplificatier += 200;
        }
    }

    if (damage < BATTLE_MAX_DAMAGE)
        damage = simple_div(damage * amplificatier, 100);

    /**
     * Real damage:
     *  1. Ignore damage calculation
     *  2. No damage increase
     *  3. Cannot Crit
     */
    real_damage = CalcBattleRealDamage(attacker, defender);
    if (real_damage > 0 && gBattleStats.config & BATTLE_CONFIG_REAL)
        TriggerKtutorial(KTUTORIAL_REAL_DAMAGE);

    damage += real_damage;

    /* Post calc */
    if (damage > BATTLE_MAX_DAMAGE)
        damage = BATTLE_MAX_DAMAGE;

    gBattleStats.damage = damage;

    BattleCheckPetrify(attacker, defender);

    if (gBattleStats.damage != 0)
        attacker->nonZeroDamage = TRUE;
}

/* LynJump */
void BattleGenerateHitEffects(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int debuff;

    attacker->wexpMultiplier++;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
    {
        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_POISON ||
#if (defined(SID_PoisonPoint) && (SID_PoisonPoint < MAX_SKILL_NUM))
            SkillTester(&attacker->unit, SID_PoisonPoint)
#else
            0
#endif
        )
        {
            defender->statusOut = UNIT_STATUS_POISON;
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

            // "Ungray" defender if it was petrified (as it won't be anymore)
            debuff = GetUnitStatusIndex(&defender->unit);
            if (debuff == UNIT_STATUS_PETRIFY || debuff == UNIT_STATUS_13)
                defender->unit.state = defender->unit.state &~ US_UNSELECTABLE;
        }

        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPHALVE)
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPHALVE;

        if ((GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL) && (BattleRoll1RN(31 - attacker->unit.lck, FALSE)))
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

#if defined(SID_Bane) && (SID_Bane < MAX_SKILL_NUM)
            if (gBattleStats.damage < (defender->unit.curHP - 1))
            {
                if (CheckBattleSkillActivte(attacker, defender, SID_Bane, attacker->unit.skl))
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
    }

    gBattleHitIterator->hpChange = gBattleStats.damage;

#if (defined(SID_Corrosion) && (SID_Corrosion < MAX_SKILL_NUM))
    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS) && CheckBattleSkillActivte(attacker, defender, SID_Corrosion, attacker->unit.skl))
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

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS) || attacker->weaponAttributes & (IA_UNCOUNTERABLE | IA_MAGIC))
    {
#ifdef CHAX
        /* Check on combat-art */
        int cost = 1;
        if (attacker == &gBattleActor)
        {
            int cid = GetCombatArtInForce(&attacker->unit);
            if (COMBART_VALID(cid) && gpCombatArtInfos[cid].cost > 0)
                cost = gpCombatArtInfos[cid].cost;
        }

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
#if (defined(SID_Mercy) && (SID_Mercy < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(attacker, defender, SID_Mercy, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Mercy);
        return true;
    }
#endif

#if (defined(SID_Inori) && (SID_Inori < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(defender, attacker, SID_Inori, defender->unit.lck))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Inori);
        return true;
    }
#endif

#if (defined(SID_LEGEND_InoriAtk) && (SID_LEGEND_InoriAtk < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(defender, attacker, SID_LEGEND_InoriAtk, 100))
    {
        if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriAtk) == 0)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_InoriAtk);
            return true;
        }
    }
#endif

#if (defined(SID_LEGEND_InoriAvo) && (SID_LEGEND_InoriAvo < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(defender, attacker, SID_LEGEND_InoriAvo, 100))
    {
        if (TryActivateLegendSkill(&defender->unit, SID_LEGEND_InoriAvo) == 0)
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_LEGEND_InoriAvo);
            return true;
        }
    }
#endif

#if (defined(SID_LEGEND_InoriDef) && (SID_LEGEND_InoriDef < MAX_SKILL_NUM))
    if (CheckBattleSkillActivte(defender, attacker, SID_LEGEND_InoriDef, 100))
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
        attacker->wexpMultiplier++;

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

#if (defined(SID_Galeforce) && (SID_Galeforce < MAX_SKILL_NUM))
            if (CheckBattleSkillActivte(&gBattleActor, &gBattleTarget, SID_Galeforce, gBattleActor.unit.skl))
                gBattleActorGlobalFlag.skill_activated_galeforce = true;
#endif
            gBattleHitIterator->info |= BATTLE_HIT_INFO_KILLS_TARGET;
        }

        gBattleHitIterator++;
        return true;
    }
    else if (defender->statusOut == UNIT_STATUS_PETRIFY || defender->statusOut == UNIT_STATUS_13)
    {
        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;
        gBattleHitIterator++;
        return true;
    }

    gBattleHitIterator++;
    return false;
}
