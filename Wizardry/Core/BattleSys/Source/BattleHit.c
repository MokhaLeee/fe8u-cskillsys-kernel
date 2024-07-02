#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "strmag.h"
#include "debuff.h"
#include "unit-expa.h"
#include "kernel-lib.h"
#include "combat-art.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

STATIC_DECLAR bool CheckSkillHpDrain(struct BattleUnit * attacker, struct BattleUnit * defender)
{
#if (defined(SID_Aether) && (COMMON_SKILL_VALID(SID_Aether)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Aether, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aether);
        return true;
    }
#endif

#if (defined(SID_Sol) && (COMMON_SKILL_VALID(SID_Sol)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Sol, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Sol);
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

int CalcBattleRealDamage(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int damage = 0;

#if defined(SID_RuinedBlade) && (COMMON_SKILL_VALID(SID_RuinedBlade))
    if (SkillTester(&attacker->unit, SID_RuinedBlade))
        damage += 5;
#endif

#if defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus))
    if (SkillTester(&attacker->unit, SID_RuinedBladePlus))
        damage += 5;
#endif

#if defined(SID_LunaAttack) && (COMMON_SKILL_VALID(SID_LunaAttack))
    if (SkillTester(&attacker->unit, SID_LunaAttack))
        damage += defender->battleDefense / 4;
#endif

    return damage;
}

/* LynJump */
void BattleGenerateHitAttributes(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int attack, defense, amplifier;
    int damage, real_damage;
    bool crit_atk = false;

    gBattleStats.damage = 0;

    /* Fasten simulation */
    if (!BattleRoll2RN(gBattleStats.hitRate, FALSE))
    {
#if (defined(SID_DivinePulse) && (COMMON_SKILL_VALID(SID_DivinePulse)))
        if (CheckBattleSkillActivate(attacker, defender, SID_DivinePulse, 30 + attacker->unit.lck))
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DivinePulse);
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

    attack = gBattleStats.attack;
    defense = gBattleStats.defense;

#if (defined(SID_QuickDraw) && (COMMON_SKILL_VALID(SID_QuickDraw)))
    if (SkillTester(&attacker->unit, SID_QuickDraw) && attacker == &gBattleTarget)
        attack += 4;
#endif

#if (defined(SID_StrongRiposte) && (COMMON_SKILL_VALID(SID_StrongRiposte)))
    if (SkillTester(&attacker->unit, SID_StrongRiposte) && attacker == &gBattleTarget)
        attack += 3;
#endif

#if (defined(SID_Flare) && (COMMON_SKILL_VALID(SID_Flare)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Flare, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Flare);
        defense = defense / 2;
    }
#endif

    if (IsMagicAttack(attacker))
    {
#if (defined(SID_Corona) && (COMMON_SKILL_VALID(SID_Corona)))
        if (CheckBattleSkillActivate(attacker, defender, SID_Corona, attacker->unit.skl))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Corona);
            defense = 0;
        }
#endif
    }
    else
    {
#if (defined(SID_Luna) && (COMMON_SKILL_VALID(SID_Luna)))
        if (CheckBattleSkillActivate(attacker, defender, SID_Luna, attacker->unit.skl))
        {
            RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Luna);
            defense = 0;
        }
#endif
    }

#if (defined(SID_Ignis) && (COMMON_SKILL_VALID(SID_Ignis)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Ignis, attacker->unit.skl))
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

#if defined(SID_Glacies) && (COMMON_SKILL_VALID(SID_Glacies))
    if (CheckBattleSkillActivate(attacker, defender, SID_Glacies, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Glacies);
        attack += attacker->unit.res;
    }
#endif

#if defined(SID_Vengeance) && (COMMON_SKILL_VALID(SID_Vengeance))
    if (CheckBattleSkillActivate(attacker, defender, SID_Vengeance, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Vengeance);
        attack += (attacker->unit.maxHP - attacker->unit.curHP);
    }
#endif

    amplifier = 100;
    damage = attack - defense;

#if defined(SID_FlashingBladePlus) && (COMMON_SKILL_VALID(SID_FlashingBladePlus))
    if (SkillTester(&attacker->unit, SID_FlashingBladePlus))
        damage += 3;
#endif

#if defined(SID_DragonFang) && (COMMON_SKILL_VALID(SID_DragonFang))
    if (SkillTester(&attacker->unit, SID_DragonFang))
        amplifier += 50;
#endif

#if (defined(SID_Colossus) && (COMMON_SKILL_VALID(SID_Colossus)))
    if (CheckBattleSkillActivate(attacker, defender, SID_Colossus, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Colossus);
        amplifier += 200;
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
    }
#endif

#if defined(SID_Impale) && (COMMON_SKILL_VALID(SID_Impale))
    if (CheckBattleSkillActivate(attacker, defender, SID_Impale, attacker->unit.skl))
    {
        RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Impale);
        amplifier += 300;
        gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_CRIT;
    }
#endif

    if (gBattleTemporaryFlag.skill_activated_sure_shoot)
        amplifier += 50;

#if defined(SID_Astra) && (COMMON_SKILL_VALID(SID_Astra))
    if (attacker == &gBattleActor && SkillTester(&attacker->unit, SID_Astra) && gBattleActorGlobalFlag.skill_activated_astra)
    {
        /* If we can deal damage, astra should not reduce it to 0 */
        if (damage > 1)
            amplifier /= 2;
    }
#endif

    if (damage > 0)
    {
#if (defined(SID_GreatShield) && (COMMON_SKILL_VALID(SID_GreatShield)))
            if (CheckBattleSkillActivate(defender, attacker, SID_GreatShield, defender->unit.skl))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_GreatShield);
                damage = 0;
            }
#endif
        if (IsMagicAttack(attacker))
        {
#if (defined(SID_Aegis) && (COMMON_SKILL_VALID(SID_Aegis)))
            if (CheckBattleSkillActivate(defender, attacker, SID_Aegis, defender->unit.skl))
            {
                RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_Aegis);
                damage = 0;
            }
#endif
        }
        else
        {
#if (defined(SID_Pavise) && (COMMON_SKILL_VALID(SID_Pavise)))
            if (CheckBattleSkillActivate(defender, attacker, SID_Pavise, defender->unit.skl))
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
#if defined(SID_Foresight) && (COMMON_SKILL_VALID(SID_Foresight))
        !SkillTester(&defender->unit, SID_Foresight) &&
#else
        0 &&
#endif
#if defined(SID_Fortune) && (COMMON_SKILL_VALID(SID_Fortune))
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
            crit_atk = true;

#if defined(SID_InfinityEdge) && (COMMON_SKILL_VALID(SID_InfinityEdge))
            if (SkillTester(&attacker->unit, SID_InfinityEdge))
#else
            if (0)
#endif
                amplifier += 300;
            else
                amplifier += 200;
        }
    }

#if (defined(SID_DragonSkin) && (COMMON_SKILL_VALID(SID_DragonSkin)))
    if (SkillTester(&defender->unit, SID_DragonSkin))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_DragonSkin);
        amplifier /= 2;
    }
#endif

#if (defined(SID_KeenFighter) && (COMMON_SKILL_VALID(SID_KeenFighter)))
    if (SkillTester(&defender->unit, SID_KeenFighter) && CheckCanTwiceAttackOrder(attacker, defender))
    {
        RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_KeenFighter);
        amplifier /= 2;
    }
#endif

#if defined(SID_GuardBearing) && (COMMON_SKILL_VALID(SID_GuardBearing))
    if (SkillTester(&defender->unit, SID_GuardBearing))
    {
        if (!AreUnitsAllied(defender->unit.index, gPlaySt.faction) &&
            GetBattleGlobalFlags(attacker)->round_cnt_hit == 1 &&
            !CheckBitUES(&defender->unit, UES_BIT_GUARDBEAR_SKILL_USED))
        {
            RegisterTargetEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_GuardBearing);
            SetBitUES(&defender->unit, UES_BIT_GUARDBEAR_SKILL_USED);
            amplifier /= 2;
        }
    }
#endif

#if (defined(SID_Spurn) && (COMMON_SKILL_VALID(SID_Spurn)))
    if (SkillTester(&defender->unit, SID_Spurn))
    {
        int as_diff = defender->battleSpeed - attacker->battleSpeed;
        LIMIT_AREA(as_diff, 0, 10);

        amplifier -= amplifier * as_diff / 25;
    }
#endif

    if (damage < BATTLE_MAX_DAMAGE)
    {
        int _damage = (damage * amplifier / 100);
        if (damage > 0 && _damage <= 0)
            _damage = 0;

        damage = _damage;
    }

    if (crit_atk)
    {
#if (defined(SID_Spurn) && (COMMON_SKILL_VALID(SID_Spurn)))
        if (SkillTester(&attacker->unit, SID_Spurn))
            damage += 5;
#endif
    }

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
    bool weapon_cost;

    attacker->wexpMultiplier++;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS))
    {
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
        else if (gBattleTemporaryFlag.skill_activated_dead_eye)
        {
            defender->statusOut = UNIT_STATUS_SLEEP;
        }
        else if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_POISON ||
#if (defined(SID_PoisonPoint) && (COMMON_SKILL_VALID(SID_PoisonPoint)))
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

#if (defined(SID_Galeforce) && (COMMON_SKILL_VALID(SID_Galeforce)))
            if (CheckBattleSkillActivate(&gBattleActor, &gBattleTarget, SID_Galeforce, gBattleActor.unit.skl))
                gBattleActorGlobalFlag.skill_activated_galeforce = true;
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
